/*
 * ltc2959.c
 *
 *  Created on: Apr 29, 2026
 *      Author: abertazza
 */
#include "main.h"
#include "i2c_bb.h"
#include "ltc2959.h"

/** lettura di 1 byte da I2C (con o senza ACK in coda) */
static uint8_t ltc_read8(uint8_t ack)
{
    uint8_t data = 0;
    SDA_H(); // rilascia SDA per permettere allo slave di scrivere
    for (unsigned i = 0; i < 8; i++)
    {
        delay_us(I2C_BB_US);
        SCL_H();
        delay_us(I2C_BB_US);
        data <<= 1;
        if (SDA_READ()) data |= 0x01;
        SCL_L();
        delay_us(I2C_BB_US); //TODO: test lettura
    }
    // invia ACK (0) per continuare la lettura o NACK (1) per terminare
    if (ack)
    	SDA_L();
    else
    	SDA_H();

    delay_us(I2C_BB_US);
    SCL_H(); delay_us(I2C_BB_US);
    SCL_L(); delay_us(I2C_BB_US);
    SDA_H(); // rilascia SDA
    return data;
}

/** Scrittura di registro/i, lunghezza da parametro. Attenzione all'endian! */
static bool ltc_writereg(enum ltc_reg_addr reg_addr, unsigned cnt, const uint8_t *data)
{
    i2c_start();
    // invia slave address (write)
    if (i2c_write(LTC_ADDR_W) == 0) { i2c_stop(); return 1; } // Errore NACK
    // invia indirizzo registro (command byte)
    if (i2c_write(reg_addr) == 0) { i2c_stop(); return 1; }
    // invia i dati
    for (unsigned i = 0; i < cnt; ++i)
    {
        if (i2c_write(data[i]) == 0) { i2c_stop(); return 1; }
    }
    i2c_stop();
    return 0; // ok (ACK ricevuto)
}

/** Scrittura di un registro su 8 bit */
bool ltc_writereg8(enum ltc_reg_addr reg_addr, uint8_t data)
{
    return ltc_writereg(reg_addr, 1, &data);
}

/** Scrittura di un registro su 16 bit */
bool ltc_writereg16(enum ltc_reg_addr reg_addr, uint16_t data)
{
    const uint8_t bigend[2] = {
        data >> 8, data
    };
    return ltc_writereg(reg_addr, 2, bigend);
}

/** Scrittura di un registro su 32 bit */
bool ltc_writereg32(enum ltc_reg_addr reg_addr, uint32_t data)
{
    const uint8_t bigend[4] = {
        data >> 24, data >> 16, data >> 8, data
    };
    return ltc_writereg(reg_addr, 4, bigend);
}

/** Parte iniziale di comunicazione su I2C comune a tutte le
 * dimensioni di trasferimento*/
static void ltc_readreg_start(enum ltc_reg_addr reg_addr)
{
    // fase di scrittura indirizzo registro
    i2c_start();
    i2c_write(LTC_ADDR_W);        // slave address + write
    i2c_write(reg_addr);    	// command byte (registro da leggere)

    // repeated START
    i2c_start();
    i2c_write(LTC_ADDR_R);        // slave address + read
}

/** Lettura di un registro su 8 bit */
uint8_t ltc_readreg8(enum ltc_reg_addr reg_addr)
{
    ltc_readreg_start(reg_addr);
    // riceve il dato, 1 byte con NACK per fine lettura
    const uint8_t data = ltc_read8(0);
    i2c_stop();
    return data;
}

/** Lettura di un registro su 16 bit */
uint16_t ltc_readreg16(enum ltc_reg_addr reg_addr)
{
    ltc_readreg_start(reg_addr);

    uint8_t data[2];
    uint16_t raw_value;
    // lettura burst di 2 byte
    data[0] = i2c_read(1);  // Leggi MSB e invia ACK per continuare
    data[1] = i2c_read(0);  // Leggi LSB e invia NACK per terminare

    i2c_stop();

    // composizione del valore a 16 bit
    raw_value = (uint16_t)(data[0] << 8) | data[1];

    return raw_value;
}

/** Lettura di un registro su 32 bit */
uint32_t ltc_readreg32(enum ltc_reg_addr reg_addr)
{
    ltc_readreg_start(reg_addr);

    uint8_t data[4];
    uint32_t raw_value;
    // Lettura burst di 4 byte
    data[0] = i2c_read(1);  // Leggi Byte 3 (MSB) -> invia ACK
    data[1] = i2c_read(1);  // Leggi Byte 2       -> invia ACK
    data[2] = i2c_read(1);  // Leggi Byte 1       -> invia ACK
    data[3] = i2c_read(0);  // Leggi Byte 0 (LSB) -> invia NACK per terminare

    i2c_stop();

    raw_value = ((uint32_t)data[0] << 24) |
                ((uint32_t)data[1] << 16) |
                ((uint32_t)data[2] <<  8) |
                 (uint32_t)data[3];

    return raw_value;
}
/* Local Variables: */
/* tab-width: 4 */
/* End: */
