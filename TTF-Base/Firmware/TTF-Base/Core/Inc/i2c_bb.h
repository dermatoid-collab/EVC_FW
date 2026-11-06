/*
 * hmi_i2c.h
 *
 *  Created on: Apr 22, 2026
 *      Author: abertazza
 */
#ifndef INC_I2C_BB_H_
#define INC_I2C_BB_H_
#include <stdint.h>
#define I2C_PORT GPIOB
// old
//#define SDA_PIN  GPIO_PIN_8  // Pin SDA scambiato
//#define SCL_PIN  GPIO_PIN_7  // Pin SCL scambiato

#define SDA_PIN  GPIO_PIN_7  // Pin SDA scambiato
#define SCL_PIN  GPIO_PIN_8  // Pin SCL scambiato

/* Tempo di lavoro per i (semi)bit sulla I2C emulata */
enum {
    I2C_BB_US = 5
};

// Rilascia la linea (va alta con pull-up)
#define SDA_H() HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_SET)
// Tira la linea a Massa (va bassa)
#define SDA_L() HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_RESET)

#define SCL_H() HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_SET)
#define SCL_L() HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_RESET)

// Legge lo stato della linea SDA
#define SDA_READ() HAL_GPIO_ReadPin(I2C_PORT, SDA_PIN)

void delay_us(uint32_t us);
void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_write(uint8_t data);
uint8_t i2c_read(uint8_t ack);
void write_device_reg(uint8_t dev_addr, uint8_t reg, uint8_t val);
uint8_t I2C_bb_transmit(uint8_t dev_addr, const uint8_t *pData, uint16_t size);
uint8_t I2C_bb_receive(uint8_t dev_addr, uint8_t *pData, uint16_t size);
uint16_t LTC2959_ReadReg16(uint8_t dev_addr, uint8_t reg);
void LTC2959_WriteReg16(uint8_t dev_addr, uint8_t reg, uint16_t value);
int I2C_IsBusFree(void);

#endif /* INC_I2C_BB_H_ */
/* Local Variables: */
/* tab-width: 4 */
/* End: */
