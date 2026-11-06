#ifndef __TIMER_H
#define __TIMER_H
#include "main.h"

/* Da main.h *deve* essere definita la enum tmr_id */

/*======================================================================
  Timer rapidi - pilotati direttamente dal SysTick, lavorano in
  millisecondi */

enum tmr_id {
	TMR_ID_UDEC_CX_UPDATE, 	/* timeout invio status word */
	TMR_ID_ELT_PWM,			/* timeout pwm serratura MD2 */
	TMR_ID_DEBUG_LED,
	TMR_ID_NMT_DELAY,
	TMR_ID_NMT_TIMEOUT,
    TMR_ID_SIZE,
};

/* Tipologia del timer; per iniziare sono utili i timer IEC, nel caso si
   implementano altre tipologie gestite da Crouzet o OMRON */
enum tmr_type {
    TMR_TYPE_OFF,
    TMR_TYPE_IEC_TON,
    TMR_TYPE_IEC_TOF,
    TMR_TYPE_IEC_TP,
    TMR_TYPE_IEC_TONR,
    TMR_TYPE_D,
    TMR_TYPE_DI,
    TMR_TYPE_D1,
};

/* Con PV/SV a 32 bit non c'è problema di range */
typedef uint32_t tmr_size;
enum {
    TMR_SIZE_MAX = 0xFFFFFFFF
};

/* Stati possibili per un timer; sfortunatamente coprono gli stati
   possibili di *tutti* i tipi di timer quindi non sono sempre tutti
   raggiungibili. Si usano macchine di Moore quindi l'uscita dipende
   solo dallo stato (anche se per convenienza si salva il valore).

   Non sono soddisfatto del fatto che UP/DN negli stati ARMED si
   riferiscano alle transizioni in ingresso mentre negli stati COUNTING
   si riferiscano allo stato dell'uscita.
*/
enum tmr_state {
    TMR_STATE_ARMED_UP,         /* Pronto allo start in salita */
    TMR_STATE_ARMED_DN,         /* Pronto allo start in discesa */
    TMR_STATE_COUNTING_UP,      /* In conteggio con uscita alta */
    TMR_STATE_COUNTING_DN,      /* In conteggio con uscita bassa */
    TMR_STATE_STOPPED,          /* Fermo in attesa di reset */
};

/* Struttura di controllo di *un* timer; al momento si gestisce solo un
   SV, con base dei tempi fissa, senza gate/enable */
struct tmr_t {
    tmr_size sv;
    tmr_size pv;
    uint32_t systick_prev; /* Systick all'ultimo update */
    bool y_prev : 1;           /* Valore precedente per rilevare i fronti */
    bool output : 1;           /* Uscita del timer */
    bool touched : 1;          /* Indicatore di timer aggiornato in questa
                              scan */
    enum tmr_type type : 4;
    enum tmr_state state : 3;
};

/* L'array che contiene tutti i timer */
extern struct tmr_t tmr_array[TMR_ID_SIZE];

/* Prototipi */
void tmr_config(enum tmr_id id, enum tmr_type type, tmr_size sv);
void tmr_reset(enum tmr_id id);
bool tmr_update(enum tmr_id id, bool y);
void tmr_end_scan(void);

#endif
