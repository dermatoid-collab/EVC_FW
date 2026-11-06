#ifndef POWER_MODE_H
#define POWER_MODE_H
#include <stdbool.h>
/** Commutazione master relay da caricabatteria a VFD.

   In modalità VFD viene attivata la catena delle sicurezze e l'ok to
   run del motore, ma è necessario qualche secondo per caricare il DC
   link quindi è meglio tenerlo attivo per una decina di secondi dopo
   il movimento.

   In modalità di carica il VFD non è alimentato, ma non necessariamente
   il caricabatterie è disponibile (se manca tensione di rete per
   esempio). Quindi nel momento in cui va offline il VFD si considera
   completa la transizione. Nel contempo il VFD viene comunque
   considerato non disponibile e vengono ignorati i fault (in quando in
   spegnimento va sicuramente in undervoltage)

   Il tempo di commutazione è soggetto a timeout, sia durante
   l'accensione del VFD (contatti guasti o DC link che non si carica),
   sia durante lo spegnimento (non viene tolta alimentazione
   correttamente).
*/
enum pmode_requested {
    PMODE_REQ_CHARGER,          /* Commutazione a NPB con ritardo */
    PMODE_REQ_CHARGER_NOW,      /* Commutazione istantanea a NPB */
    PMODE_REQ_DRIVE             /* Commutazione a VFD */
};

enum pmode_state {
    PMODE_ST_TO_CHARGER, /* Commutazione in corso da movimento a ricarica */
    PMODE_ST_CHARGER, /* Commutazione completata a stato di ricarica */
    PMODE_ST_TO_DRIVE, /* Commutazione in corso da ricarica a movimento */
    PMODE_ST_DRIVE,   /* Commutazione completata a stato di movimento */
    PMODE_ST_WAIT_TO_CHARGER, /* Tempo di grazia prima dello spegnimento del VFD */
};
enum {
    TIMEOUT_PMODE_TO_CHARGER = 5000,
    TIMEOUT_PMODE_TO_DRIVE = 5000,
    TIMEOUT_PMODE_WAIT_TO_CHARGER = 30000
};
struct pmode_info {
    enum pmode_requested req_mode;
    enum pmode_state state;
    bool output_coil;
};
extern struct pmode_info pmode;

void pmode_scan_cycle(void);
void pmode_reset(void);
void pmode_relay_set(bool s);
bool pmode_in_fault(void);
#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
