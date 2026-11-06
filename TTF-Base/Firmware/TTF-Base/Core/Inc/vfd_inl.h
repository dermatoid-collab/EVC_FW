#ifndef VFD_INL_H
#define VFD_INL_H
#include "vfd.h"
#include "battery.h"
#include "application.h"

/* Predicati */
/** Il VFD è bloccato in errore? Attenzione, alcuni errori non sono
 * bloccati, ma sono attesi (a causa del relè di alimentazione), quindi
 * vengono ignorati.
 *
 * Questo però non significa che non vadano resettati (nel qual caso si
 * controlla direttamente vfd.fault)
 */
inline bool vfd_in_pseudofault(void) {
    return (vfd.fault == VFD_FAULT_POWER_FUSE_RELAY) ||
            (vfd.fault == VFD_FAULT_UNDERVOLTAGE) ||
            (vfd.fault == VFD_FAULT_OVERVOLTAGE) ||
            (vfd.fault == VFD_FAULT_COMM_TIMEOUT);
}
inline bool vfd_in_fault(void) {
    return (vfd.fault != VFD_FAULT_NO)
            && !vfd_in_pseudofault();
}

/** Il VFD è in running se è attivo uno dei due bit di direzione nello
 * stato motore */
inline bool vfd_is_running(void) {
    return vfd_is_ready() &&
            (vfd.motor_status & (VFD_MOTOR_FWD | VFD_MOTOR_REV));
}

/** Relè di uscita stabile in condizione aperta */
inline bool vfd_output_is_open(void) {
    return vfd.output_state == VFD_OUTPUT_OPEN;
}

/** Relè di uscita stabile in condizione chiusa */
inline bool vfd_output_is_closed(void) {
    return vfd.output_state == VFD_OUTPUT_CLOSED;
}

/** Reset del contattore: passa in open e toglie l'emergenza */
inline void vfd_output_reset(void)
{
    vfd.output_req = VFD_OUTPUT_OPEN_REQ;
}

/** Richiede l'apertura di emergenza */
inline void vfd_output_emergency_open(void) {
    vfd.output_req = VFD_OUTPUT_EMERGENCY_REQ;
}

/** Richiede l'apertura normale, ma non perde l'emergenza */
inline void vfd_output_open(void) {
    /* Se si controllasse lo stato del relè non potremmo sapere se siamo
       aperti normalmente o in emergenza */
    if (vfd.output_req != VFD_OUTPUT_EMERGENCY_REQ)
        vfd.output_req = VFD_OUTPUT_OPEN_REQ;
}

/** Richiede la chiusura, solo se non siamo già in emergenza */
inline void vfd_output_close(void) {
    if (vfd.output_req != VFD_OUTPUT_EMERGENCY_REQ)
        vfd.output_req = VFD_OUTPUT_CLOSE_REQ;
}

/** Stop */
inline void vfd_stop(uint8_t time_dec)
{
    vfd_move_request(0, VFD_RUN_STOP, time_dec, time_dec);
}

/** Come lo stop ma al massimo della decelerazione */
inline void vfd_halt(void)
{
    vfd_move_request(0, VFD_RUN_STOP, 1, 1);
}

#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
