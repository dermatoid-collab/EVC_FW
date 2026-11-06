#ifndef INC_SPIFLASH_H_
#define INC_SPIFLASH_H_

// Comandi standard per memorie Flash QSPI
enum {
    CMD_READ_ID          = 0x9F,  // Legge il codice identificativo del chip
    CMD_WRITE_ENABLE     = 0x06,  // Abilita la scrittura/cancellazione
    CMD_SECTOR_ERASE     = 0x20,  // Cancella un settore da 4KB
    CMD_PAGE_PROGRAM     = 0x02,  // Scrive una pagina (fino a 256 byte)
    CMD_DUAL_READ        = 0xBB,  // Lettura veloce in modalità Quad I/O
    CMD_READ_STATUS      = 0x05,  // Legge lo Status Register del chip esterno
    CMD_WRITE_STATUS     = 0x01,
    QSPI_TIMEOUT_VAL   	 = 100,
    CMD_FAST_READ        = 0x0B,  // Lettura veloce a 1 linea (richiede 8 dummy)
    CMD_RESET_MEMORY     = 0x99,
    CMD_RESET_ENABLE     = 0x66
};

#endif /* INC_FLASH_H_ */
/* Local Variables: */
/* tab-width: 4 */
/* End: */
