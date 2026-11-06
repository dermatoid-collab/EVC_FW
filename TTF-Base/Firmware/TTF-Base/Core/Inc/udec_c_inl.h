#ifndef UDEC_C_INL
#define UDEC_C_INL

#include "timer_inl.h"
#include "udec_c.h"
#include "udec_cx.h"
#include "application.h"
#include "run_mode.h"

/** Predicato: il magnete di piano è attivo */
inline bool magnet_ftc(void) {
    return udec_c.in_good && udec_c.in.ftc1;
}

/** Predicato: la porta è bloccata oppure siamo in manutenzione */
inline bool door_is_locked(void) {
    return run_mode.bypass_manutenzione
            || (udec_c.in_good && udec_c.in.s34_col);
}

/** Predicato: la porta è in accostamento oppure siamo in manutenzione. */
inline bool door_is_closed(void) {
    return run_mode.bypass_manutenzione
            || (udec_c.in_good && udec_c.in.s33_pet);
}

/** Predicato: la cabina è sovraccarica */
inline bool overload_in(void) {
    return udec_c.in_good && udec_c.in.ftc3;
}

/** Aziona la luce occupato */
inline void set_occ(bool s) {
    udec_c.out.hl_occ = s;
}

/** Predicato: è premuto il pulsante (ri)apertura porte */
inline bool reopen_door_button(void) {
    return udec_c.in_good && udec_c.in.sb_pc5;
}

/** Comanda l'apertura delle porte */
inline void door_set_open(void) {
    udec_c.out.op1_ap = true;
    udec_c.out.op2_ap = true;
    udec_c.out.op3_ap = true;
    udec_c.out.op1_ch = false;
    udec_c.out.op2_ch = false;
    udec_c.out.op3_ch = false;
}

/** Comanda la chiusura delle porte */
inline void door_set_close(void) {
    udec_c.out.op1_ch = true;
    udec_c.out.op2_ch = true;
    udec_c.out.op3_ch = true;
    udec_c.out.op1_ap = false;
    udec_c.out.op2_ap = false;
    udec_c.out.op3_ap = false;
}

/** Disattiva il comando alle porte */
inline void door_set_neutral(void) {
    udec_c.out.op1_ch = false;
    udec_c.out.op2_ch = false;
    udec_c.out.op3_ch = false;
    udec_c.out.op1_ap = false;
    udec_c.out.op2_ap = false;
    udec_c.out.op3_ap = false;
}

/** Predicato: è attiva una chiamata verso il P0 */
inline bool is_P0_on(void) {
    return udec_c.in_good && (udec_c.in.sb_pc0 || udec_c.in.sb_pc6)
            && !udec_c.in.sb_pc1;
}

/** Predicato: è attiva una chiamata verso il P1 */
inline bool is_P1_on(void) {
    return udec_c.in_good && (udec_c.in.sb_pc1 || udec_c.in.sb_pc7)
            && !udec_c.in.sb_pc0;
}

/** Segnalazione sovraccarico */
inline void overload_lamp(bool s)
{
    udec_c.out.hl_ovl = s;
    udec_c.out.hl_buz = s;
}

/** Segnalazione riapertura */
inline void reopen_lamp(bool s) {
    udec_c.out.hl_pc5 = s;
}

inline void enable_tlc(void)
{
    udec_c.out.hl_pc6 = true;
    udec_c.out.hl_pc7 = true;
}

inline void disable_tlc(void)
{
    udec_c.out.hl_pc6 = false;
    udec_c.out.hl_pc7 = false;
}

inline void enable_p71_out(void)
{
    udec_c.out.hl_pc8 = true;
    udec_c.out.hl_pc9 = true;
}

inline void disable_p71_out(void)
{
    udec_c.out.hl_pc8 = false;
    udec_c.out.hl_pc9 = false;
}
#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
