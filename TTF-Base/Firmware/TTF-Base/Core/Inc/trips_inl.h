#ifndef TRIPS_INL_H
#define TRIPS_INL_H
#include "trips.h"

/** Controllo comune per verificare lo stato di attivo */
inline bool is_trip_active(enum trip_id id) {
    return trip_state_get(id) == TRIP_ACTIVE;
}

/** Verifica lo stato di attivo o stored */
inline bool is_trip_stored(enum trip_id id) {
    enum trip_state state = trip_state_get(id);
    return (state == TRIP_ACTIVE) || (state == TRIP_HISTORY);
}

/** Predicato: c'è un nuovo trip attivo */
inline bool is_new_trip_active(void)
{
    return trips_new_trip != TRIP_ID_NONE;
}

/** Caso comune in cui il trip segue direttamente una condizione.
   La condizione è passante sul valore di ritorno per comodità di
   programmazione. */
inline bool trip_state_set(enum trip_id id, bool trip) {
    trip_assert(id, trip);
    trip_retract(id, !trip);
    return trip;
}
#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
