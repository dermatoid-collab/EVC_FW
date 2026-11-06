#ifndef TRAPTRAY_H
#define TRAPTRAY_H
/** Gestione della commutazione dei bypass su trap e tray.

    Regole:
    - Solo uno dei bypass può essere inserito; per evitare commutazioni
      spurie ne viene commutato solo uno per volta (si passa sempre dal
      no-bypass)

    - Il tempo di manovra è complicato dal fatto che i relè sono remoti
      ma fortunatamente sono dotati di contatto specchio

    - La congruenza fra pilotaggio e contatto specchio è fatto nei
      controlli delle sicurezze

    - Non è necessario gestire un timeout di commutazione dato che viene
      intercettato dal test sopra
*/
enum traptray_state {
    TRAPTRAY_NO_BYPASS,
    TRAPTRAY_TRAP_BYPASS,
    TRAPTRAY_TRAY_BYPASS,
    TRAPTRAY_FAULT
};

struct traptray_info {
    enum traptray_state req_state;
    enum traptray_state state;
};
extern struct traptray_info traptray;

void traptray_scan_cycle(void);

#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
