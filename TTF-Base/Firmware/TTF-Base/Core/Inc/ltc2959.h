/*
 * ltc2959.h
 *
 *  Created on: Apr 29, 2026
 *      Author: abertazza
 */

#ifndef INC_LTC2959_H_
#define INC_LTC2959_H_
#include <stdint.h>
#include <stdbool.h>
/** Conversione del contatore di carica. Da datasheet sono 533nAh con 50
    mohm di shunt; sulla scheda è montata una shunt da 2 mohm, quindi il
    valore corretto è di 25 volte tanto, 13,33 µAh */

#define LTC_AH_PER_LSB     (13.33E-6f)
// dclink_voltage = 62.6f * ((float)rawdata / 65536.0f);
// 62.6 / 65536   = 0.0009552f
#define LTC_V_PER_LSB      (0.0009552f)
// bat_temp       = 825*(rawdata/65536)-273.15f in Kelvin;
#define LTC_KELVIN_PER_LSB (0.0125885f)
#define LTC_A_PER_LSB      (0.00148773f)

/** Registri del contatore di cariche; per i multibyte è codificato solo
 * il primo */
enum ltc_reg_addr {
    LTC_STATUS_ADDR              = 0x00,
    LTC_ADC_CONTROL_ADDR         = 0x01,
    LTC_ACC_CONTROL_ADDR         = 0x02,
    LTC_ACC_CHARGE_ADDR          = 0x03,
    LTC_ACC_THRESH_LOW_ADDR      = 0x07,
    LTC_ACC_THRESH_HIGH_ADDR     = 0x0B,
    LTC_VOLTAGE_ADDR             = 0x0F,
    LTC_VOLTAGE_THRESH_HIGH_ADDR = 0x11,
    LTC_VOLTAGE_THRESH_LOW_ADDR  = 0x13,
    LTC_VOLTAGE_MAX_ADDR         = 0x15,
    LTC_VOLTAGE_MIN_ADDR         = 0x17,
    LTC_CURRENT_ADDR             = 0x19,
    LTC_CURRENT_THRESH_HIGH_ADDR = 0x1B,
    LTC_CURRENT_THRESH_LOW_ADDR  = 0x1D,
    LTC_CURRENT_MAX_ADDR         = 0x1F,
    LTC_CURRENT_MIN_ADDR         = 0x21,
    LTC_TEMP_ADDR                = 0x23,
    LTC_TEMP_THRESH_HIGH_ADDR    = 0x25,
    LTC_TEMP_THRESH_LOW_ADDR     = 0x27,
    LTC_GPIO_ADDR                = 0x29,
    LTC_GPIO_THRESH_HIGH_ADDR    = 0x2B,
    LTC_GPIO_THRESH_LOW_ADDR     = 0x2D,
};

/** Indirizzi I2C del power monitor */
enum {
    LTC_ADDR_W = 0xC6,
    LTC_ADDR_R = 0xC7,
};

uint8_t ltc_readreg8(enum ltc_reg_addr reg_addr);
uint16_t ltc_readreg16(enum ltc_reg_addr reg_addr);
uint32_t ltc_readreg32(enum ltc_reg_addr reg_addr);
bool ltc_writereg8(enum ltc_reg_addr reg_addr, uint8_t data);
bool ltc_writereg16(enum ltc_reg_addr reg_addr, uint16_t data);
bool ltc_writereg32(enum ltc_reg_addr reg_addr, uint32_t data);

#endif /* INC_LTC2959_H_ */
/* Local Variables: */
/* tab-width: 4 */
/* End: */
