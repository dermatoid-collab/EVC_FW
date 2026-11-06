#ifndef INC_APPLICATION_H_
#define INC_APPLICATION_H_
#include <stdint.h>
#include <stdbool.h>
#include "vfd.h"

/** Numero ingressi digitali e cicli di filtraggio */
enum {
    DInCount = 12,
    DInDebounce = 5
};

typedef union
{
    struct
    {
        bool In_1:1;
        bool In_2:1;
        bool In_3:1;
        bool In_4:1;
        bool bypass:1;
        bool s1:1;
        bool s2:1;
        bool s3:1;
        bool s4:1;
        bool s8:1;
        bool otr_fb:1;
        bool brk_fb:1;
    };
    uint32_t w;
} DigitalInput_t;

enum {
    /** Tempo di parcheggio a porte aperte */
    TIMEOUT_MACH_PARK_TIME = 20000,
    /** Tempo di manovra porte */
    TIMEOUT_MACH_DOOR_TIME = 10000,
    /** Tempo di pressione per reset da quadro */
    TIMEOUT_HMI_RESET = 3000,
    /** Tempo di pressione per reset da COP */
    TIMEOUT_COP_RESET = 3000,
    /** Tempo aggiuntivo di luce occupato */
    TIMEOUT_OCC = 10000,
    /** Timeout sblocco serratura */
    TIMEOUT_LOCK_DELAY_OP = 1500,
    /** Timeout blocco serratura */
    TIMEOUT_LOCK_DELAY_CL = 1500,
    /** Tempo di grazia per il reset del VFD prima del reset scheda */
    TIMEOUT_VFD_ON_RESTART = 100,
    /** Intervallo di trasmissione beat NMT */
    TIMEOUT_NMT_PERIOD = 100,
    /** Intervallo generico per commutazione relè */
    TIMEOUT_RELAY_SWITCH = 200,
    /** Tempo massimo per commutazione contatto specchio. Deve essere
     * lungo abbastanza da ricevere quelli remoti */
    TIMEOUT_MIRROR_SWITCH = 2000,
    /** Velocità di movimento rapida */
    VFD_SPEED_FAST = RPM_SPEED_10,
    /** Tempo di accelerazione velocità rapid1 */
    VFD_ATIME_FAST = 40,
    /** Velocità di movimento lenta */
    VFD_SPEED_SLOW = RPM_SPEED_5,
    /** Tempo di accelerazione velocità lenta */
    VFD_ATIME_SLOW = 10,
    /** Tempo di grazia prima di verificare gli errori e le sicurezze (in modo che si inizializzi tutto) */
    TIMEOUT_INITIAL_ERRCHECK = 5000,
    /** Tempo di rotazione errori su display */
    TIMEOUT_TRIP_DISPLAY = 2000,
    /** Tempo di attesa a 10 rpm per rilascio freno */
    TIMEOUT_BRAKE_RELEASE = 100,
    /** Tempo di attesa da accostamento a blocco porte. */
    TIMEOUT_DOOR_APPROACH = 500,
};

extern DigitalInput_t DIn;
extern bool halt_sent;

void main_loop(void);

#endif /* INC_APPLICATION_H_ */
/* Local Variables: */
/* tab-width: 4 */
/* End: */
