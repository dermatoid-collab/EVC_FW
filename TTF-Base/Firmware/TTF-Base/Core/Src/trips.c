#include <stdint.h>
#include <stdbool.h>
#include "trips_inl.h"

/* Inlines */
bool is_trip_active(enum trip_id id);
bool is_trip_stored(enum trip_id id);
bool trip_state_set(enum trip_id id, bool trip);

/* Idea di base: ci sono due bitmap, con un bit per ogni errore.
   Nel primo sono settati tutti gli errori attivi, nel secondo anche
   quelli che sono stati attivati in precedenza, in modo da avere una
   storia prima del reset. E con reset, al momento, si intende
   letteralmente il master clear del micro.

   Le bitmap sono array di word di 16 bit, in questo modo è facile
   associarle al codice esadecimale. L'enum trip_id deve provenire
   dall'esterno (trip_list.h), TRIP_ID_SIZE e TRIP_ID_NONE devono essere
   uguali e gli ultimi valori dell'enum */

enum {
    TRIP_BANKS = TRIP_ID_SIZE / 16 + 1
};
static uint16_t trips_active[TRIP_BANKS], trips_history[TRIP_BANKS];
/** Indica che ci sono nuovi errori da visualizzare */
enum trip_id trips_new_trip;
bool is_new_trip_active(void);

/** Utility per accedere ai bitarray */
static inline bool bitarray_get(uint16_t const *a, unsigned bit)
{
    return a[bit >> 4] & (1 << (bit & 0xF));
}

static inline void bitarray_set(uint16_t *a, unsigned bit)
{
    a[bit >> 4] |= 1 << (bit & 0xF);
}

static inline void bitarray_reset(uint16_t *a, unsigned bit)
{
    a[bit >> 4] &= ~(1 << (bit & 0xF));
}

/** Restituisce lo stato corrente consultando le due bitmap */
enum trip_state trip_state_get(enum trip_id id)
{
    bool active = bitarray_get(trips_active, id);
    bool history = bitarray_get(trips_history, id);
    if (active) {
        return TRIP_ACTIVE;
    } else if (history) {
        return TRIP_HISTORY;
    } else {
        return TRIP_INACTIVE;
    }
}

/** Attiva un errore, sia nella bitmap corrente che in quella storica */
bool trip_assert(enum trip_id id, bool cond)
{
    if (cond) {
        /* Se è un errore nuovo forza l'aggiornamento del display */
        if (!bitarray_get(trips_active, id)) {
            trips_new_trip = id;
        }
        bitarray_set(trips_active, id);
        bitarray_set(trips_history, id);
    }
    return cond;
}

/** Disattiva un errore, ma lo lascia nella bitmap storica */
bool trip_retract(enum trip_id id, bool cond)
{
    if (cond) {
        bitarray_reset(trips_active, id);
    }
    return cond;
}

/** Attiva/disattiva un errore */
bool trip_assign(enum trip_id id, bool cond)
{
    if (cond)
    {
        trip_assert(id, true);
    }
    else
    {
        trip_retract(id, true);
    }
    return cond;
}

/** Cerca il prossimo trip attivo, modificando l'id passato.
   Se si entra con TRIP_ID_NONE:
   - Cerca il primo attivo o memorizzato, mette l'indice nell'id
     e ritorna lo stato;
   - Se non ci sono valori attivi o memorizzati lascia TRIP_ID_NONE e
     ritorna TRIP_INACTIVE;

   Se si entra con un altro valore:
   - Cerca il primo attivo o memorizzato *successivo* a quello passato,
     ne mette l'indice nell'id e ritorna lo stato;
   - Se non ci sono valori attivi o memorizzati successivi mette
     TRIP_ID_NONE e ritorna TRIP_INACTIVE;

   In sostanza, entrando con TRIP_ID_NONE si comincia a cercare
   dall'inizio, altrimenti si riprende dal punto precedente fino alla
   fine dell'array

   In ogni caso viene azzerato l'indicatore di nuovo trip */
enum trip_state trip_next(enum trip_id *id)
{
    trips_new_trip = TRIP_ID_NONE;
    if (*id == TRIP_ID_NONE) {
        *id = 0;                /* Riparte da capo */
    } else {
        *id = *id + 1;          /* Continua dal successivo */
    }
    while (*id != TRIP_ID_NONE) {
        enum trip_state state = trip_state_get(*id);
        if (state > TRIP_INACTIVE) {
            return state;
        }
        *id = *id + 1;
    }
    return TRIP_INACTIVE;
}
