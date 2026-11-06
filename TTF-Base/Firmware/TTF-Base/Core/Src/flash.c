#include <stdint.h>
#include <stdbool.h>
#include "flash.h"

FLASH_EraseInitTypeDef EraseInitStruct;
static uint32_t pErr;
//uint32_t virtual_sector0[PARAM_COUNT];
uint32_t virtual_sector0[PARAM_COUNT];
uint32_t virtual_sector0_old[PARAM_COUNT]; // copia del vettore

/* prima di eliminare il settore >dovrei< copiarlo nel settore di backup */
void flash_erase(uint32_t start_addr, uint32_t num_sectors)
{
	HAL_FLASH_Unlock();
	EraseInitStruct.TypeErase 	= FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.Sector = start_addr;
	EraseInitStruct.NbSectors 	= 1; //(numberofwords/FLASH_PAGE_SIZE)+1;
	HAL_FLASHEx_Erase(&EraseInitStruct, &pErr);
	HAL_FLASH_Lock();
}

void flash_write_8(uint32_t start_addr,  uint8_t data)
{
	HAL_FLASH_Unlock();
	EraseInitStruct.TypeErase 	= FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.Sector = start_addr;
	EraseInitStruct.NbSectors 	= 1; //(numberofwords/FLASH_PAGE_SIZE)+1;
	HAL_FLASHEx_Erase(&EraseInitStruct, &pErr);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, start_addr, data);
	HAL_FLASH_Lock();
}

void flash_write_16(uint32_t start_addr,  uint16_t data)
{
	HAL_FLASH_Unlock();
	EraseInitStruct.TypeErase 	= FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.Sector = start_addr;
	EraseInitStruct.NbSectors 	= 1; //(numberofwords/FLASH_PAGE_SIZE)+1;
	HAL_FLASHEx_Erase(&EraseInitStruct, &pErr);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, start_addr, data);
	HAL_FLASH_Lock();
}

void flash_write_32(uint32_t start_addr,  uint32_t data)
{
	HAL_FLASH_Unlock();
//	int numberofwords = (strlen(DATA_32)/4) + ((strlen(DATA_32)%4) != 0);
	EraseInitStruct.TypeErase 	= FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.Sector = start_addr;
	EraseInitStruct.NbSectors 	= 1; //(numberofwords/FLASH_PAGE_SIZE)+1;
	HAL_FLASHEx_Erase(&EraseInitStruct, &pErr);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, start_addr, data);
	HAL_FLASH_Lock();
}

void flash_write_buffer(uint32_t start_addr, uint32_t *data, uint32_t size)
{
	HAL_FLASH_Unlock();
	EraseInitStruct.TypeErase 	= FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.Sector = start_addr;
	EraseInitStruct.NbSectors 	= 1;
	HAL_FLASHEx_Erase(&EraseInitStruct, &pErr);
	for (unsigned i=0; i<size; i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, start_addr + (i*4), data[i]);
	}
	HAL_FLASH_Lock();
}


/* leggo il settore dalla flash e lo divido */
void flash_to_ram(uint32_t start_addr)
{
    volatile uint32_t* flash_ptr = (volatile uint32_t*)FLASH_SECTOR_0_ADDR;

    for (unsigned i = 0; i < PARAM_COUNT; i++)
    {
        virtual_sector0[i] = flash_ptr[i];
        virtual_sector0_old[i] = virtual_sector0[i];
    }
}

/* se devo aggiornre la flash */
void ram_to_flash(void)
{
    static volatile bool diff;

    /* controllo cambiamenti nel vettore */
    for (unsigned i=0; i<PARAM_COUNT; i++)
    {
        if (virtual_sector0[i] != virtual_sector0_old[i])
            diff = true;
    }

    /* riscrivo in flash */
    if (diff)
    {
        flash_write_buffer(FLASH_SECTOR_0_ADDR, virtual_sector0, PARAM_COUNT);
        diff = false;
    }
}
/* Local Variables: */
/* tab-width: 4 */
/* End: */
