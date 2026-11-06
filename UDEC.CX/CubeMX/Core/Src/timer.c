#include "main.h"
#include "timer_inl.h"

/** Gestione dei timer rapidi ==========================================*/

/*
  Note di progetto:

  - La base dei tempi al momento è il systick, quindi 1ms;

  - I contatori sono a 32 bit perché con 1ms di base dei tempi si fa
    solo un minuto abbondante a 16 bit. Per tempi più lunghi sarebbero
    necessari contatori extra quindi tanto vale fare tutto a 32 bit.

  - I contatori del PV contano sempre verso l'alto, il cambio di stato
    avviene quando *raggiungono o superano* il SV corrente;

  - L'aggiornamento non si può fare in base alla differenza fra il tempo
    di fine scan e quello corrente, dato che in generale non si può
    prevedere quanto durerà la scan corrente, nè in base alla durata
    della scan precedente in quanto le scan non sono tutte della stessa
    durata; per gestire la situazione è necessario per ogni timer tenere
    traccia dell'ultimo update (che è il valore del systick di 32 bit);
    se si occupa troppa memoria si può utilizzare uno dei due modi sopra
    con la conseguente perdita di precisione;

  - Alcune modalità usano due SV, al momento non sono gestite;

  - Durante lo scan si può alterare il segnale in ingresso (Y) sia verso
    l'alto (normalmente) che verso il basso (può servire?
    comunque è gratis); il segnale di reset resetta tutto il ciclo, non
    solo il contatore (per timer che hanno stati complessi); reset_pv
    invece azzera solo il PV senza cambiare lo stato (quindi con alcuni
    tipi di timer non è utile). In alcune modalità è importante il
    fronte, in altre il livello;

  - Alcune funzioni sono semplicemente varianti con gato/enable che
    mettono in pausa il conteggio (e non sono implementate);

  - Se durante una scan un timer non viene toccato (con ingresso né in
    alto né in basso) a fine scan viene considerato come ingresso basso;

  - La variazione dell'SV viene recepita alla scan successiva;

  - Per comodità lo stato corrente dell'uscita viene ritornato dalla
    funzione di aggiornamento ma è disponibile anche come valore a se
    stante; due chiamate nella stessa scan non vengono gestite, le
    successive semplicemente ritornano lo stato *senza* effettuare
    l'update; fare due update per scan è comunque sospetto, specialmente
    se con ingressi differenti (considerare un errore se viene fatto,
    confrontando il valore con last_y, ma attenzione alla gestione di
    fine scan);

  Funzioni base (da manuale Crouzet Syr-line; non è considerato il
  segnale di alimentazione, generalmente viene assorbito dall'ingresso):

  Se indicato con (*) non è implementato! L'idea è che si implementano
  quando servono.

  - A/Ad/IEC TON
    Ritardo all'accensione da fronte salita; timer base

  - Ab/Ah/P (*)
    Impulso ritardato da fronte salita (doppio SV); Ah è la versione
    a singolo SV; P ha la durata dell'impulso fissa a 500ms

  - Ac (*)
    Ritardo all'accensione da fronte salita e ritardo allo
    spegnimento spegnimento da fronte discesa (doppio SV);

  - At (*)
    Ritardato all'accensione con gate. Y *ferma* il conteggio che
    altrimenti è permanente;

  - T/IEC TONR
    Ritardato all'accensione con enable. Come At ma Y funziona come
    enable e non come gate;

  - B/H/IEC TP
    Impulso da fronte di salita, funziona come Ab con tempo iniziale
    nullo (quindi basta un solo SV);

  - Bw (*)
    One shot da fronte di salita o discesa (differenziatore);

  - C/IEC TOF
    Ritardo allo spegnimento da fronte discesa;

  - D
    Alternato acceso/spento simmetrico, start con uscita spenta;

  - Di
    Alternato acceso/spento asimmetrico, start con uscita attiva;

  - D1 (non Crozuet)
    Impulso periodico, rimane attivo per *un ciclo* dopo il tempo;

  - Ht (*)
    Ritardo allo spegnimento con enable;

  - L (*)
    Alternato acceso/spento asimmetrico, start con uscita spenta
    (doppio SV);

  - Li (*)
    Alternato acceso/spento asimmetrico, start con uscita attiva
    (doppio SV);

  - O (*)
    Ritardo all'accensione con reset automatico su fronte di salita
    (watchdog ritardato);

  - N (*)
    Impulso da fronte di salita con reset automatico su fronte di salita
    (watchdog);

  - Pt (*)
    Impulso di 500ms con gate;

  - W (*)
    Impulso da fronte di discesa;
*/

/* Inlines */
void tmr_sv_set(enum tmr_id id, tmr_size sv);
void tmr_sv_extend(enum tmr_id id, tmr_size sv);
void tmr_pv_reset(enum tmr_id id);
tmr_size tmr_pv(enum tmr_id id);
bool tmr_output(enum tmr_id id);

/* L'array che contiene tutti i timer */
struct tmr_t tmr_array[TMR_ID_SIZE];

/* Configurazione di un timer, imposta tipo e sv iniziale, poi procede
   con il reset; il reset è completo */
void tmr_config(enum tmr_id id, enum tmr_type type, tmr_size sv)
{
    struct tmr_t * const tmr = &tmr_array[id];
    tmr->type = type;
    tmr->sv = sv;
    tmr_reset(id);
}

/* La procedura di reset varia leggermente in base al tipo di timer
   e per questo ci sono funzioni dedicate al riguardo; in ogni caso
   vengono azzerati i valori precedenti per tick e ingresso, nonché
   l'output. Finora *tutti* i timer hanno semplicemente lo start nello
   stesso stato quindi non c'è bisogno di funzioni aggiuntive */
void tmr_reset(enum tmr_id id)
{
    struct tmr_t * const tmr = &tmr_array[id];
    tmr->pv = 0;
    tmr->y_prev = false;
    tmr->systick_prev = HAL_GetTick();
    tmr->output = false;
    switch (tmr->type)
    {
    case TMR_TYPE_OFF:
        break;
        /* Reset standard comune a tutti i timer correnti: attesa del
           fronte di start */
    case TMR_TYPE_IEC_TON:
    case TMR_TYPE_IEC_TOF:
    case TMR_TYPE_IEC_TP:
    case TMR_TYPE_IEC_TONR:
    case TMR_TYPE_D:
    case TMR_TYPE_DI:
    case TMR_TYPE_D1:
        tmr->state = TMR_STATE_ARMED_UP;
        break;
    }
}

/* Utility: calcola quanto tempo è passato fra due systick */
static tmr_size tick_delta(uint32_t now, uint32_t prev)
{
    uint32_t delta;
    if (now >= prev) {
        /* Caso semplice: non c'è rollover */
        delta = now - prev;
    } else {
        /* C'è stato rollover quindi la situazione è divertente.
         La parte fino a 0x100000000, la parte da 0 ad adesso e il
         carry */
        delta = (UINT32_MAX - prev) + now + 1;
    }
    /* Satura il risultato */
    if (delta > TMR_SIZE_MAX) {
        return TMR_SIZE_MAX;
    } else {
        return delta;
    }
}

/* Utility: incrementa il PV e segnala se ha raggiunto il SV; il PV
   viene clampato all'SV per conformità */
static bool tmr_update_pv(struct tmr_t * const tmr, tmr_size delta)
{
    /* Gli overflow sono in agguato sempre */
    tmr_size new_pv = tmr->pv + delta;
    if ((new_pv < tmr->pv) || (new_pv >= tmr->sv))
    {
        tmr->pv = tmr->sv;
        return true;
    } else {
        tmr->pv = new_pv;
        return false;
    }
}

/* Utility: rileva i fronti del segnale di ingresso di un timer */
static inline bool tmr_y_rise(struct tmr_t * const tmr, bool now) {
    return now && (!tmr->y_prev);
}
static inline bool tmr_y_fall(struct tmr_t * const tmr, bool now) {
    return (!now) && (tmr->y_prev);
}

/* La procedura di update fa sostanzialmente tutto, a seconda della
   tipologia di timer. Se il timer non è attivo non perde neppure
   tempo a calcolare il delta.
*/
static void tmr_update_IEC_TON(struct tmr_t * const tmr, bool y, tmr_size delta_t);
static void tmr_update_IEC_TOF(struct tmr_t * const tmr, bool y, tmr_size delta_t);
static void tmr_update_IEC_TP(struct tmr_t * const tmr, bool y, tmr_size delta_t);
static void tmr_update_IEC_TONR(struct tmr_t * const tmr, bool y, tmr_size delta_t);
static void tmr_update_D(struct tmr_t * const tmr, bool y, tmr_size delta_t);
static void tmr_update_DI(struct tmr_t * const tmr, bool y, tmr_size delta_t);
static void tmr_update_D1(struct tmr_t * const tmr, bool y, tmr_size delta_t);
bool tmr_update(enum tmr_id id, bool y)
{
    struct tmr_t * const tmr = &tmr_array[id];
    /* Ce ne importa se un timer spento è stato toccato? no! */
    if (tmr->touched || (tmr->type == TMR_TYPE_OFF)) {
        return tmr->output;
    }
    /* Calcola quanto tempo è passato dall'ultimo update */
    uint32_t systick_now = HAL_GetTick();
    tmr_size delta_t = tick_delta(systick_now, tmr->systick_prev);
    /* È orribile ma un array di funzioni non cambierebbe molto la
       storia. In compenso così il compilatore può ottimizzare meglio
       ed essendo le funzioni static e inutilizzate altrimenti
       probabilmente le forza inline. */
    switch (tmr->type)
    {
    case TMR_TYPE_OFF:
        break;
    case TMR_TYPE_IEC_TON:
        tmr_update_IEC_TON(tmr, y, delta_t);
        break;
    case TMR_TYPE_IEC_TOF:
        tmr_update_IEC_TOF(tmr, y, delta_t);
        break;
    case TMR_TYPE_IEC_TP:
        tmr_update_IEC_TP(tmr, y, delta_t);
        break;
    case TMR_TYPE_IEC_TONR:
        tmr_update_IEC_TONR(tmr, y, delta_t);
        break;
    case TMR_TYPE_D:
        tmr_update_D(tmr, y, delta_t);
        break;
    case TMR_TYPE_DI:
        tmr_update_DI(tmr, y, delta_t);
        break;
    case TMR_TYPE_D1:
        tmr_update_D1(tmr, y, delta_t);
        break;
    }
    /* Aggiorna lo stato per il prossimo giro */
    tmr->touched = true;
    tmr->y_prev = y;
    tmr->systick_prev = systick_now;
    return tmr->output;
}

/* A fine scan viene semplicemente mandato un update su tutti i timer
   e poi azzerato il flag touched. Dato che viene automaticamente
   ignorato se è già stato toccato nel ciclo è facile. */
void tmr_end_scan(void)
{
    for (enum tmr_id tid = 0 ; tid < TMR_ID_SIZE; ++tid) {
        tmr_update(tid, false);
        tmr_array[tid].touched = false;
    }
}

/** Timer TON ---------------------------------------------------------
  Ritardo all'accensione sul fronte di salita dell'ingresso, senza
  retrigger, reset sul fronte di discesa.
*/
static void tmr_update_IEC_TON(struct tmr_t * const tmr,
        bool y, tmr_size delta_t)
{
    switch (tmr->state) {
        /* Aspetta il fronte di salita iniziale */
    case TMR_STATE_ARMED_UP:
        tmr->output = false;
        tmr->pv = 0;
        if (tmr_y_rise(tmr, y)) {
            tmr->state = TMR_STATE_COUNTING_DN;
        }
        break;
        /* Conteggio prima di alzare l'uscita */
    case TMR_STATE_COUNTING_DN:
        tmr->output = false;
        if (tmr_y_fall(tmr, y)) {
            tmr->state = TMR_STATE_ARMED_UP;
        } else if (tmr_update_pv(tmr, delta_t)) {
            tmr->state = TMR_STATE_ARMED_DN;
        }
        break;
        /* Attesa del rilascio dell'ingresso */
    case TMR_STATE_ARMED_DN:
        tmr->output = true;
        if (tmr_y_fall(tmr, y)) {
            tmr->state = TMR_STATE_ARMED_UP;
        }
        break;
        /* Non significativi */
    case TMR_STATE_COUNTING_UP:
    case TMR_STATE_STOPPED:
        break;
    }
}

/** Timer TOFF ---------------------------------------------------------
  Ritardo allo spegnimento sul fronte di discesa dell'ingresso, con
  retrigger, sul fronte di salita.
*/
static void tmr_update_IEC_TOF(struct tmr_t * const tmr,
        bool y, tmr_size delta_t)
{
    switch (tmr->state) {
        /* Aspetta il fronte di salita iniziale */
    case TMR_STATE_ARMED_UP:
        tmr->output = false;
        tmr->pv = 0;
        if (tmr_y_rise(tmr, y)) {
            tmr->state = TMR_STATE_ARMED_DN;
        }
        break;
        /* Aspetta il fronte di discesa che avvia il conteggio */
    case TMR_STATE_ARMED_DN:
        tmr->output = true;
        if (tmr_y_fall(tmr, y)) {
            tmr->state = TMR_STATE_COUNTING_UP;
        }
        break;
    case TMR_STATE_COUNTING_UP:
        /* Conteggio prima di rilasciare l'uscita */
        tmr->output = true;
        if (tmr_y_rise(tmr, y)) {
            /* Retrigger */
            tmr->pv = 0;
            tmr->state = TMR_STATE_ARMED_DN;
        } else if (tmr_update_pv(tmr, delta_t)) {
            tmr->state = TMR_STATE_ARMED_UP;
        }
        break;
        /* Non significativi */
    case TMR_STATE_COUNTING_DN:
    case TMR_STATE_STOPPED:
        break;
    }
}

/** Timer TP ---------------------------------------------------------
  Impulso sul fronte di salita dell'ingresso, senza retrigger, reset a
  fine ciclo (monostabile puro).
*/
static void tmr_update_IEC_TP(struct tmr_t * const tmr,
        bool y, tmr_size delta_t)
{
    switch (tmr->state) {
        /* Aspetta il fronte di salita iniziale */
    case TMR_STATE_ARMED_UP:
        tmr->output = false;
        tmr->pv = 0;
        if (tmr_y_rise(tmr, y)) {
            tmr->state = TMR_STATE_COUNTING_UP;
        }
        break;
        /* Conteggio prima di abbassare l'uscita */
    case TMR_STATE_COUNTING_UP:
        tmr->output = true;
        /* A questo punto non importa cosa succede all'ingresso */
        if (tmr_update_pv(tmr, delta_t)) {
            tmr->state = TMR_STATE_ARMED_UP;
        }
        break;
        /* Non significativi */
    case TMR_STATE_ARMED_DN:
    case TMR_STATE_COUNTING_DN:
    case TMR_STATE_STOPPED:
        break;
    }
}

/** Timer TONR ---------------------------------------------------------
  Ritardo all'accensione sul fronte di salita dell'ingresso, senza
  reset (accumulatore); Y funziona come enable.

  Il timer deve essere resettato manualmente a fine ciclo.
*/
static void tmr_update_IEC_TONR(struct tmr_t * const tmr,
        bool y, tmr_size delta_t)
{
    switch (tmr->state) {
        /* Aspetta il fronte di salita iniziale */
    case TMR_STATE_ARMED_UP:
        tmr->output = false;
        if (tmr_y_rise(tmr, y)) {
            tmr->state = TMR_STATE_COUNTING_DN;
        }
        break;
        /* Conteggio mentre l'ingresso è attivo */
    case TMR_STATE_COUNTING_DN:
        tmr->output = false;
        if (tmr_y_fall(tmr, y)) {
            tmr->state = TMR_STATE_ARMED_UP;
        } else if (tmr_update_pv(tmr, delta_t)) {
            tmr->state = TMR_STATE_STOPPED;
        }
        break;
        /* Attesa del rilascio dell'ingresso */
    case TMR_STATE_STOPPED:
        tmr->output = true;
        break;
        /* Non significativi */
    case TMR_STATE_COUNTING_UP:
    case TMR_STATE_ARMED_DN:
        break;
    }
}

/** Timer D/Di ---------------------------------------------------------
  Timer Crouzet D
  Uscita alternata simmetrica acceso/spento, start su fase acceso.

  Timer Crouzet Di
  Uscita alternata simmetrica acceso/spento, start su fase spento.

  Sono assolutamente identici *tranne* per la transizione iniziale, che
  in un caso è ARMED_UP->COUNTING_UP, nell'altro ARMED_UP->COUNTING_DN;
  il grosso della macchina quindi è in una funzione a parte.
*/
static void tmr_update_D_base(struct tmr_t * const tmr,
        bool y, tmr_size delta_t)
{
    switch (tmr->state) {
        /* Conteggio prima di abbassare l'uscita */
    case TMR_STATE_COUNTING_UP:
        tmr->output = true;
        if (tmr_y_fall(tmr, y)) {
            tmr->state = TMR_STATE_ARMED_UP;
        } else if (tmr_update_pv(tmr, delta_t)) {
            tmr->pv = 0; /* Riparte e conta basso */
            tmr->state = TMR_STATE_COUNTING_DN;
        }
        break;
        /* Conteggio prima di alzare l'uscita */
    case TMR_STATE_COUNTING_DN:
        tmr->output = false;
        if (tmr_y_fall(tmr, y)) {
            tmr->state = TMR_STATE_ARMED_UP;
        } else if (tmr_update_pv(tmr, delta_t)) {
            tmr->pv = 0; /* Riparte e conta alto */
            tmr->state = TMR_STATE_COUNTING_UP;
        }
        break;
        /* Non significativi */
    case TMR_STATE_ARMED_DN:
    case TMR_STATE_STOPPED:
        /* Gestiti esternamente */
    case TMR_STATE_ARMED_UP:
        break;
    }
}

static void tmr_update_D(struct tmr_t * const tmr,
        bool y, tmr_size delta_t)
{
    /* Aspetta il fronte di salita iniziale */
    if (tmr->state == TMR_STATE_ARMED_UP) {
        tmr->output = false;
        tmr->pv = 0;
        if (tmr_y_rise(tmr, y)) {
            tmr->state = TMR_STATE_COUNTING_UP;
        }
    } else {
        tmr_update_D_base(tmr, y, delta_t);
    }
}

static void tmr_update_DI(struct tmr_t * const tmr,
        bool y, tmr_size delta_t)
{
    /* Aspetta il fronte di salita iniziale */
    if (tmr->state == TMR_STATE_ARMED_UP) {
        tmr->output = false;
        tmr->pv = 0;
        if (tmr_y_rise(tmr, y)) {
            tmr->state = TMR_STATE_COUNTING_DN;
        }
    } else {
        tmr_update_D_base(tmr, y, delta_t);
    }
}

static void tmr_update_D1(struct tmr_t * const tmr,
        bool y, tmr_size delta_t)
{

    switch (tmr->state) {
    case TMR_STATE_ARMED_UP:
        /* Aspetta che l'enable sia alto */
        tmr->output = false;
        tmr->pv = 0;
        if (y) {
            tmr->state = TMR_STATE_COUNTING_DN;
        }
        break;

        /* Conteggio prima di alzare l'uscita */
    case TMR_STATE_COUNTING_DN:
        if (tmr_y_fall(tmr, y)) {
            tmr->output = false;
            tmr->state = TMR_STATE_ARMED_UP;
        } else if (tmr_update_pv(tmr, delta_t)) {
            /* Alza l'uscita solo per questo ciclo */
            tmr->output = true;
            tmr->pv = 0; /* Riparte */
            tmr->state = TMR_STATE_ARMED_UP;
        } else {
            tmr->output = false;
        }
        break;

        /* Non significativi */
    case TMR_STATE_ARMED_DN:
    case TMR_STATE_COUNTING_UP:
    case TMR_STATE_STOPPED:
        break;
    }
}

