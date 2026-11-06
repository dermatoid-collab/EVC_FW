/*
 * hmi_i2c.c
 *
 *  Created on: Apr 22, 2026
 *      Author: abertazza
 */
#include <stdbool.h>
#include "main.h"
#include "i2c_bb.h"

extern TIM_HandleTypeDef htim6;

// Funzione di delay in microsecondi
void delay_us(uint32_t us)
{
    /* uso TIM6 per aspettare (bloccare) i microsecondi che mi servono */
    uint16_t start = __HAL_TIM_GET_COUNTER(&htim6);
    /* XXX Attenzione us è uint32_t ma il contatore viene usato come uint16_t */
    while ((uint16_t)(__HAL_TIM_GET_COUNTER(&htim6) - start) < us);
}

void i2c_start(void)
{
    // Assicuriamo uno stato noto per il Repeated Start:
    // SDA deve essere alto PRIMA che SCL salga,
    // ma deve salire mentre SCL è ancora basso.
    SDA_H();
    delay_us(I2C_BB_US);
    SCL_H();
    delay_us(I2C_BB_US);

    // Ora eseguiamo la transizione di START vera e propria
    SDA_L(); // SDA cade mentre SCL è alto
    delay_us(I2C_BB_US);
    SCL_L(); // SCL scende per preparare il primo bit
    delay_us(I2C_BB_US);
}

void i2c_stop(void)
{
    SDA_L(); delay_us(I2C_BB_US);
    SCL_H(); delay_us(I2C_BB_US); // Stop: SDA sale mentre SCL è alto
    SDA_H(); delay_us(I2C_BB_US);
}

uint8_t i2c_write(uint8_t data)
{
    for (unsigned i = 0; i < 8; i++)
    {
        if (data & 0x80)
        	SDA_H();
        else
        	SDA_L();

        delay_us(I2C_BB_US);
        SCL_H(); delay_us(I2C_BB_US);
        SCL_L(); delay_us(I2C_BB_US);
        data <<= 1;
    }
    // Lettura ACK
    SDA_H(); delay_us(I2C_BB_US);
    SCL_H(); delay_us(I2C_BB_US);
    uint8_t ack = !SDA_READ(); // Se SDA è basso, lo slave ha risposto
    SCL_L(); delay_us(I2C_BB_US);
    return ack;
}

uint8_t i2c_read(uint8_t ack)
{
    uint8_t data = 0;

    // Rilascia SDA per permettere allo slave di pilotarlo
    SDA_H();
    delay_us(I2C_BB_US);

    for (unsigned i = 0; i < 8; i++)
    {
        data <<= 1;
        SCL_H();
        delay_us(I2C_BB_US); // Aspetta che il dato sia stabile

        if (SDA_READ()) data |= 0x01;

        SCL_L();
        delay_us(I2C_BB_US);
    }

    // Gestione ACK/NACK (inviato dal Master allo Slave)
    if (ack)
        SDA_L(); // ACK: "Manda pure il prossimo byte"
    else
        SDA_H(); // NACK: "Basta così, sto per chiudere"

    delay_us(I2C_BB_US);
    SCL_H();
    delay_us(I2C_BB_US);
    SCL_L();

    // Rilascia SDA per futuri Start/Stop
    SDA_H();
    delay_us(I2C_BB_US);

    return data;
}

void write_device_reg(uint8_t dev_addr, uint8_t reg, uint8_t val)
{
    i2c_start();
    i2c_write(dev_addr); // Indirizzo + bit di scrittura (0)
    i2c_write(reg);           // Registro
    i2c_write(val);           // Valore
    i2c_stop();
}

// Ritorna 0 se OK, 1 se errore (NACK)
uint8_t I2C_bb_transmit(uint8_t dev_addr, const uint8_t *pData, uint16_t size)
{
    i2c_start();
    // Invia indirizzo dispositivo (scriviamo l'8-bit address fornito)
    if (i2c_write(dev_addr) == 0)
    { // Se riceve NACK
        i2c_stop();
        return 1;
    }
    // Invia i dati del buffer
    for (unsigned i = 0; i < size; i++)
    {
        if (i2c_write(pData[i]) == 0)
        {
            i2c_stop();
            return 1;
        }
    }
    i2c_stop();
    return 0;
}

uint8_t I2C_bb_receive(uint8_t dev_addr, uint8_t *pData, uint16_t size)
{
    i2c_start();

    // Invia indirizzo + Bit di Lettura (1)
    // Se dev_addr è già shiftato, usa: (dev_addr | 0x01)
    if (i2c_write(dev_addr | 0x01) == 0)
    {
        i2c_stop();
        return 1; // Errore: nessuno slave ha risposto
    }
    for (unsigned i = 0; i < size; i++)
    {
        // Se è l'ultimo byte, invia NACK (0), altrimenti ACK (1)
        bool send_ack = (i != (size - 1));
        pData[i] = i2c_read(send_ack);
    }
    i2c_stop();
    return 0;
}

/* XXX tecnicamente non corretto, il bus è libero quando si
 * è riconosciuta una condizione di stop; ma apparentemente funziona
 * e il bus non è multimaster */
int I2C_IsBusFree(void)
{
    // Il bus è libero se SDA e SCL sono entrambi ALTI
    return (SDA_READ() && HAL_GPIO_ReadPin(I2C_PORT, SCL_PIN));
}
/* Local Variables: */
/* tab-width: 4 */
/* End: */
