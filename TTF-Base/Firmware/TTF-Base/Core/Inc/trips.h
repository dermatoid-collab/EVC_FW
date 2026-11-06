#ifndef TRIPS_H
#define TRIPS_H
#include <stdbool.h>
#include "trip_list.h"

/* Definire enum trip_id in trip_list.h tipo
enum trip_id {
    TRIP_ID_SIZE,               // MARKER
    TRIP_ID_NONE = TRIP_ID_SIZE
}; */

/** Stato di un trip, può essere inattivo, attivo o memorizzato */
enum trip_state {
    TRIP_INACTIVE,
    TRIP_ACTIVE,
    TRIP_HISTORY
};

enum trip_state trip_state_get(enum trip_id id);
bool trip_assert(enum trip_id id, bool cond);
bool trip_retract(enum trip_id id, bool cond);
bool trip_assign(enum trip_id id, bool cond);
enum trip_state trip_next(enum trip_id *id);
extern enum trip_id trips_new_trip;
#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
