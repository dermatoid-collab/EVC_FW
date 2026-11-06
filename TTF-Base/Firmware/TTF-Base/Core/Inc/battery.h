#ifndef BATTERY_H
#define BATTERY_H
#include <stdint.h>
#include <stdbool.h>

/** Costanti per carica/scarica batteria e DC bus*/
enum {
    /* Tensioni dal VFD */
    /** Tensione minima sul DC link per poter dare lo start */
    VFD_DCLINK_MIN = 38,
    /* Tensioni e correnti dal caricabatteria, in decimi */
    /** Tensione di carica in carica completa */
    NPB_BOOST_VOLTAGE = 5800,
    /** Tensione di carica in carica di rabbocco */
    NPB_FLOAT_VOLTAGE = 5400,
    /** Massima corrente erogabile, punto di partenza per la
     * regolazione */
    NPB_MAX_CURRENT = 680,
    /** Step di regolazione della corrente di carica (100 mA) */
    NPB_CV_CURRENT_STEP = 10,
    /** Tensione e corrente minime impostabili (per recovery). Al di
     * sotto di questi valori il caricabatteria non accetta il
     * comando! */
    NPB_MIN_VOLTAGE = 420,
    NPB_MIN_CURRENT = 150,
    /** Numero di ricariche in float prima di una in boost per
     * ricalibrare il contatore di cariche */
    NPB_FLOAT_CHARGE_COUNT = 10,
    /** Tempo di ciclo regolazione caricabatteria (1s)*/
    TIMEOUT_NPB_CURRENT_REG = 1000,
    /** Capacità nominale della batteria per il reset del contatore su rilevamento batteria scarica */
    BATTERY_DISCHARGED_MAH = 7500,
    /* Questi tempi sono in multipli del ciclo di regolazione */
    /** Timeout carica (10 ore, in secondi) */
    NPB_CHARGE_TIMEOUT = 36000,
    /** Tempo di mantenimento in taper (10s) */
    NPB_TAPER_TIME = 10,
};

/* Queste sono float dal power meter, quindi niente enum! */
/** Soglia di scarica completa (mette fuori servizio) */
#define BAT_DISABLE_VOLTAGE 38.4f
/** Soglia di corrente che indica una carica minima per rimessa in
 * servizio, ovvero la fine della carica di bulk */
#define BAT_RESTART_CURRENT 2.8f
/** Corrente sulla batteria di fine carica */
#define BAT_TAPER_CURRENT 1.2f
/** Corrente nominale di carica sulla batteria da regolare in CC */
#define BAT_TARGET_CURRENT 3.0f
/** Energia sulla batteria per una carica in float */
#define BAT_FLOAT_START_CHARGE -0.72f
/** Tensione minima per avviamento forzato di carica */
#define BAT_FLOAT_START_VOLTAGE 48.0f
/** Energia sulla batteria per una carica in boost di reset */
#define BAT_BOOST_START_CHARGE -2.0f
/** Tensione massima prima di andare in emergenza sovratensione
 Problema: è troppo vicina sia alla tensione di boost che a quella di
 guasto, è difficile trovare il giusto compromesso */
#define DCLINK_OVERVOLTAGE 63.0f /* XXX */
/** Corrente di soglia per indicazione su led */
#define BAT_LED_CURRENT 0.25f

/** Stato algoritmo caricabatteria */
enum battery_charging_state {
    BATCHA_INIT,
    BATCHA_WAIT_DISCH,
    BATCHA_BOOST,
    BATCHA_FLOAT,
};

/* La tensione è quella del DC bus, la corrente e la carica sono
   relative alla batteria mentre la temperatura è quella della scheda */
struct battery_info {
    /** Tensione sul DC link, non quella di batteria! */
    float dclink;
    /** Corrente sulla batteria */
    float current;
    /** Temperatura (poco utile, è sulla scheda) */
    float ttf_temperature;
    /** Carica batteria (Ah) */
    float charge;
    /** Flag emergenza sovratensione */
    bool overvoltage;
    /** Flag batteria scarica (undervoltage) */
    bool low_battery;
    /** Stato processo di carica */
    enum battery_charging_state charge_state;
    /** Corrente impostata per mantenere la corrente di carica, viene
     * regolata automaticamente, anche se lentamente */
    uint16_t current_cv;
    /** Timeout di fine carica (terminazione di backup) */
    uint16_t charge_timer;
    /** Timeout di fine carica in taper */
    uint16_t taper_timer;
    /** Conta i cicli di float prima di forzare una carica completa */
    uint8_t float_cycle_counter;
};
extern struct battery_info battery;

void battery_scan(void);
void bat_reset_charge_counter_full(void);
void bat_reset_charge_counter_empty(void);
void npb_chrg_enter_charge(void);
void npb_chrg_enter_boost(void);
#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
