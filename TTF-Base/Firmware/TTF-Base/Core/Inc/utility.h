#ifndef UTILITY_H
#define UTILITY_H
#include <stdbool.h>
#include <stdint.h>

/* Generici per differenziazione logica */
inline bool rise1(bool new, bool old) { return !old & new; }
inline uint8_t rise8(uint8_t new, uint8_t old) { return ~old & new; }
inline uint16_t rise16(uint16_t new, uint16_t old) { return ~old & new; }
inline uint32_t rise32(uint32_t new, uint32_t old) { return ~old & new; }
inline bool fall1(bool new, bool old) { return old & !new; }
inline uint8_t fall8(uint8_t new, uint8_t old) { return old & ~new; }
inline uint16_t fall16(uint16_t new, uint16_t old) { return old & ~new; }
inline uint32_t fall32(uint32_t new, uint32_t old) { return old & ~new; }

#define rise(new, old)                          \
    _Generic(new,                               \
             bool: rise1,                       \
             uint8_t: rise8,                    \
             uint16_t: rise16,                  \
             uint32_t: rise32)((new), (old))

#define fall(new, old)                          \
    _Generic(new,                               \
             bool: fall1,                       \
             uint8_t: fall8,                    \
             uint16_t: fall16,                  \
             uint32_t: fall32)((new), (old))

#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
