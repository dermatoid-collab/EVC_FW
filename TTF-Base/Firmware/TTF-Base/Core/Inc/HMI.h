#ifndef INC_HMI_H_
#define INC_HMI_H_

#define LCD_ROWS 2
#define LCD_COLS 8
#define LCD_BUFSIZE (LCD_COLS+1)

union HMI_Buttons {
    struct {
        bool up : 1;
        bool dn : 1;
        bool ok : 1;
        bool esc : 1;
    };
    uint8_t w;
};

//void hmi_init(void);
void hmi_init_bb(void);
char *hmi_lcd_row(unsigned row);
char *hmi_lcd_row_clear(unsigned row);
//void hmi_handler(void);
void hmi_handler_bb(void);
union HMI_Buttons const *hmi_buttons(void);

#endif /* INC_HMI_H_ */
/* Local Variables: */
/* tab-width: 4 */
/* End: */
