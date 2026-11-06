#include "main.h"
#include "utility.h"
#include "HMI.h"
#include "lcd_hd44780_i2c.h"
#include "i2c_bb.h"
#include "can.h"

static void hmi_lcd_refresh(void);

/** Address to the I2C port expander and transfer timeout */
enum {
    I2C_IOEXP_ADDR = 0x40,
    I2C_TIMEOUT = 100
};

struct HMI_Manager
{
    enum {
        FSM_I2CSelectRegister,
        FSM_I2CReadRegister,
        FSM_LCDRefresh,
    } FSMstate;
    union HMI_Buttons butt_status;
    union HMI_Buttons butt_rise;
    union HMI_Buttons butt_fall;
    union HMI_Buttons butt_old;
    /* Communication buffer - one byte is enough */
    uint8_t i2c_buf;
};
static struct HMI_Manager hmi_manager;

/** One row of the display image */
struct HMI_LCDImage
{
    bool refresh;
    /* The text buffer can be used as string so keep a place for the
       terminating NUL */
    char text[LCD_COLS + 1];
};
static struct HMI_LCDImage lcd_image[LCD_ROWS];

union HMI_Buttons const *hmi_buttons(void)
{
    return &hmi_manager.butt_status;
}

union HMI_Buttons const *hmi_button_rise(void)
{
    return &hmi_manager.butt_rise;
}

union HMI_Buttons const *hmi_button_fall(void)
{
    return &hmi_manager.butt_rise;
}

/** Acquire the address for an LCD line and mark it as dirty */
char *hmi_lcd_row(unsigned row)
{
    lcd_image[row].refresh = true;
    return &lcd_image[row].text[0];
}

/** Utility: clear a row */
char *hmi_lcd_row_clear(unsigned row)
{
    for (unsigned i = 0; i < LCD_COLS; ++i) {
        lcd_image[row].text[i] = ' ';
    }
    return hmi_lcd_row(row);
}

/** Send the modified rows to the LCD display */
static void hmi_lcd_refresh(void)
{
    for (unsigned i = 0; i < LCD_ROWS; ++i) {
        if (lcd_image[i].refresh) {
            lcdSetCursorPosition(0, i);
            const uint8_t *uints8 = (uint8_t*)&lcd_image[i].text;
            lcdPrintStr(uints8, LCD_COLS);
        }
    }
}

void hmi_handler_bb(void)
{
    struct HMI_Manager *const d = &hmi_manager;
    switch (d->FSMstate)
    {
    case FSM_I2CSelectRegister:
        /* Indirizza l'expander per leggere port 0 */
        d->i2c_buf = 0; // Input port 0 register
        if (I2C_bb_transmit(I2C_IOEXP_ADDR,
                        &d->i2c_buf, 1) != HAL_OK)
        {
            Error_Handler();
        }
        d->FSMstate = FSM_I2CReadRegister;
        break;

    case FSM_I2CReadRegister:
        /* Avvia la ricezione della port 0 */
        if (I2C_bb_receive(I2C_IOEXP_ADDR,
                        &d->i2c_buf, 1) != HAL_OK)
        {
            Error_Handler();
        }
        d->FSMstate = FSM_LCDRefresh;
        break;

    case FSM_LCDRefresh:
        /* È completa la ricezione dello stato, prima del display si elabora.
           I bit alti fanno parte del controllo display */
        d->butt_status.w = d->i2c_buf & 0x0f;
        d->butt_rise.w = rise(d->butt_status.w, d->butt_old.w);
        d->butt_fall.w = fall(d->butt_status.w, d->butt_old.w);
        d->butt_old.w = d->butt_status.w;
        hmi_lcd_refresh();
        d->FSMstate = FSM_I2CSelectRegister;
        break;
    }
}

void hmi_init_bb(void)
{
    /* Configurazione espansione GPIO I2C per avere i 4 tasti +
       l'interfaccia con il display LCD. Vengono scritti i registri
       6 e 7 che gestiscono la direzione delle porte */
    static const uint8_t i2c_exp_setup[3] = { 0x06, 0x0F, 0x00 };

//    if ((I2C_bb_transmit(I2C_IOEXP_ADDR, &i2c_exp_setup[0], 3)) != HAL_OK)
    if ((I2C_bb_transmit(I2C_IOEXP_ADDR, i2c_exp_setup, 3)) != HAL_OK)
    {
        Error_Handler();
    }
    /* Necessario tra set espansione e display, per liberare la I2C */
    HAL_Delay(10);

    /* Inizializza il modulo LCD */
    if (!(lcdInit_bb(I2C_IOEXP_ADDR>>1, LCD_ROWS, LCD_COLS))) {
        Error_Handler();
    }
    lcdCursorOff();
    lcdBlinkOff();

    /* Pulisce il display, verrà caricato al primo refresh */
    for (unsigned i = 0; i < LCD_ROWS; ++i) {
        hmi_lcd_row_clear(i);
    }
}
/* Local Variables: */
/* tab-width: 4 */
/* End: */
