#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include "stm32f4xx_hal.h"

#define PARAM_COUNT 500
#define FLASH_SECTOR_0_ADDR 0x08000000

extern uint32_t virtual_sector0[PARAM_COUNT];
extern uint32_t virtual_sector0_old[PARAM_COUNT]; // copia del vettore

void flash_erase(uint32_t start_addr, uint32_t num_sectors);
void flash_write_8(uint32_t start_addr,  uint8_t data);
void flash_write_16(uint32_t start_addr,  uint16_t data);
void flash_write_32(uint32_t start_addr,  uint32_t data);

void flash_read(uint32_t start_addr);

#define flash_write(start_addr, data)          		   \
    _Generic(data,                       			   \
             uint8_t: flash_write_8,                    \
             uint16_t: flash_write_16,                  \
             uint32_t: flash_write_32)((start_addr), (data))

/* divido la flash in pagine da 32 bit
 * ogni pagina e' un parametro e uso questo
 * enum per definire le posizioni */
enum {
	ENCODER_P1 = 0
};

#endif /* INC_FLASH_H_ */
/* Local Variables: */
/* tab-width: 4 */
/* End: */
