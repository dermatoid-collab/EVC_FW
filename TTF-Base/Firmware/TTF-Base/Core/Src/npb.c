#include "can.h"
#include "timer.h"
#include "npb_inl.h"
#include "can_inl.h"
#include "trips.h"

/* Caricabatteria Meanwell serie NPB */
struct npb_info npb;

/* Registri interi al caricabatterie */
enum {
    NPB_REG_OPERATION          = 0x00,
    NPB_REG_VOUT_SET           = 0x20,
    NPB_REG_IOUT_SET           = 0x30,
    NPB_REG_FAULT_STATUS       = 0x40,
    NPB_REG_READ_VIN           = 0x50,
    NPB_REG_READ_VOUT          = 0x60,
    NPB_REG_READ_IOUT          = 0x61,
    NPB_REG_READ_TEMPERATURE_1 = 0x62,
    NPB_REG_MFR_ID_B0B5        = 0x80,
    NPB_REG_MFR_ID_B6B11       = 0x81,
    NPB_REG_MFR_MODEL_B0B5     = 0x82,
    NPB_REG_MFR_MODEL_B6B11    = 0x83,
    NPB_REG_MFR_REVISION_B0B5  = 0x84,
    NPB_REG_MFR_LOCATION_B0B2  = 0x85,
    NPB_REG_MFR_DATE_B0B5      = 0x86,
    NPB_REG_MFR_SERIAL_B0B5    = 0x87,
    NPB_REG_MFR_SERIAL_B6B11   = 0x88,
    NPB_REG_CURVE_CC           = 0xB0,
    NPB_REG_CURVE_CV           = 0xB1,
    NPB_REG_CURVE_FV           = 0xB2,
    NPB_REG_CURVE_TC           = 0xB3,
    NPB_REG_CURVE_CONFIG       = 0xB4,
    NPB_REG_CURVE_CC_TIMEOUT   = 0xB5,
    NPB_REG_CURVE_CV_TIMEOUT   = 0xB6,
    NPB_REG_CURVE_FV_TIMEOUT   = 0xB7,
    NPB_REG_CHG_STATUS         = 0xB8,
    NPB_REG_CHG_RST_VBAT       = 0xB9,
    NPB_REG_SCALING_FACTOR     = 0xC0,
    NPB_REG_SYSTEM_STATUS      = 0xC1,
    NPB_REG_SYSTEM_CONFIG      = 0xC2,
};

/* Bit del registro SYSTEM_STATUS */
enum {
    NPB_SYSTEM_DC_OK         = 0x02,
    NPB_SYSTEM_INITIAL_STATE = 0x20,
    NPB_SYSTEM_EEPER         = 0x40,
};

/* Bit del registro CURVE_CONFIG */
enum {
    NPB_CURVE_CONFIG_CUVS   = 0x0003,
    NPB_CURVE_CONFIG_TCS    = 0x000A,
    NPB_CURVE_CONFIG_CUVE   = 0x0080,
    NPB_CURVE_CONFIG_CCTOE  = 0x0100,
    NPB_CURVE_CONFIG_CVTOE  = 0x0200,
    NPB_CURVE_CONFIG_FVTOE  = 0x0400,
    NPB_CURVE_CONFIG_RSTE   = 0x0800,
    NPB_CURVE_CONFIG_CVTSSE = 0x2000
};

/* Lista dei registri da richiedere/settare, in ordine */
static const uint8_t npb_poll_list[] = {
    NPB_REG_VOUT_SET,
    NPB_REG_IOUT_SET,
    NPB_REG_READ_VOUT,
    NPB_REG_READ_IOUT,
    NPB_REG_FAULT_STATUS,
};

/* Inlines */
bool npb_is_online(void);
uint16_t npb_voltage(void);
uint16_t npb_current(void);
bool npb_in_fault(void);
uint8_t npb_fault(void);
uint16_t npb_chg_status(void);

/* Ricezione frame di risposta da NPB, solo da indirizzo 00C0003
   Da chiamare nel dispatch di ricezione CAN */
void npb_process_can_rx(const struct can_frame *frame)
{
    uint16_t reg_num = can_extract_uint16(frame->data);
    uint16_t reg_val = can_extract_uint16(frame->data + 2);
    switch (reg_num) {
    case NPB_REG_READ_VOUT:
        npb.out_voltage = reg_val;
        break;
    case NPB_REG_READ_IOUT:
        npb.out_current = reg_val;
        break;
    case NPB_REG_FAULT_STATUS:
        npb.fault = reg_val;
        trip_assign(TRIP_NPB, npb_in_fault());
        break;
    case NPB_REG_CHG_STATUS:
        npb.chg_status = reg_val;
        break;
    case NPB_REG_VOUT_SET:
        npb.out_voltage = reg_val;
    	break;
    case NPB_REG_IOUT_SET:
        npb.out_current = reg_val;
    	break;
    default:
        break;
    }
    /* C'è stata una risposta quindi è tutto regolare */
    npb.poll_pending = false;
    npb.online = true;
}

/** Manda una richiesta di lettura */
static void npb_request_register(uint16_t addr)
{
    struct can_frame txbuf;
    txbuf.id = NPB_CAN_REQ_ID;
    txbuf.IDE = CAN_ID_EXT;
    txbuf.RTR = false;
    txbuf.DLC = 2;
    can_store_uint16(txbuf.data, addr);
    can_tx_buffer_push(&txbuf, CAN250);
}

/** Scrive in un registro */
static void npb_write_register(uint16_t addr, uint16_t val)
{
    struct can_frame txbuf;
    txbuf.id = NPB_CAN_REQ_ID;
    txbuf.IDE = CAN_ID_EXT;
    txbuf.RTR = false;
    txbuf.DLC = 4;
    can_store_uint16(&txbuf.data[0], addr);
    can_store_uint16(&txbuf.data[2], val);
    can_tx_buffer_push(&txbuf, CAN250);
}

/** Comandi di scrittura durante il ciclo di poll */
static void npb_psu_cmd(uint16_t reg)
{
    switch (reg)
    {
    case NPB_REG_IOUT_SET:
        if ((npb.cmd.set_current != npb.cmd_old.set_current))
        {
            npb.cmd_old.set_current = npb.cmd.set_current;
            npb.cmd.poll_index = 1;
            npb.cmd.poll_pending = true;
            npb_write_register(reg, npb.cmd.set_current);
        }
        break;
    case NPB_REG_VOUT_SET:
        if ((npb.cmd.set_voltage != npb.cmd_old.set_voltage))
    	{
            npb.cmd_old.set_voltage = npb.cmd.set_voltage;
            npb.cmd.poll_index = 2;
            npb.cmd.poll_pending = true;
            npb_write_register(reg, npb.cmd.set_voltage);
        }
        break;
    default:
        break;
    }
}

/** Chiede il prossimo parametro al caricabatterie (o ne imposta uno) */
static void npb_poll(void)
{
    /* Solo se ci sono buffer disponibili. In questo caso andrà
     * temporaneamente offline il caricabatteria (forse!). */
    if (can_tx_buffer_is_full(CAN250)) {
        return;
    }
    /* Non c'è stata risposta dal caricabatteria, probabilmente manca
       la tensione di rete; continua comunque con il poll per vedere
       quando riparte */
    if (npb.poll_pending) {
        npb.online = false;
    }
    /* Prende il prossimo parametro dalla lista e avanza il puntatore */
    uint16_t req_reg = npb_poll_list[npb.poll_index++];
    if (npb.poll_index >= sizeof(npb_poll_list)) {
        npb.poll_index = 0;
    }

    /* Per quanto riguarda tensione e corrente di uscita: così non leggo
     * mai il valore impostato, lo scrivo e basta */
    switch (req_reg) {
    case NPB_REG_IOUT_SET:
    case NPB_REG_VOUT_SET:
        npb_psu_cmd(req_reg);
        break;
    default:
        npb_request_register(req_reg);
        npb.poll_pending = true;
        break;
    }
}

/** Ciclo di scansione, parte relativa al caricabatteria */
void npb_scan_cycle(void)
{
    /* Polling su CAN per stato e raggiungibilità */
    if (tmr_update(TMR_ID_NPB_SCAN, true)) {
        npb_poll();
    }
}

/** Imposta il caricabatterie in power supply mode ma il cambio modalita
    necessita un riavvio del psu o dello stato di remote. In pratica la
    prima volta che la macchina passa in drive si riallinea.
*/
void npb_set_psumode(void)
{
    npb_write_register(NPB_REG_CURVE_CONFIG, 0x0004);
}

/** Disattiva la scrittura dei parametri su EEPROM, altrimenti dura
 * veramente poco considerando che in carica vengono modificati ad ogni
 * secondo */
void npb_disable_eeprom(void)
{
    /* Power on with last settings, disable write parameters to
     * EEPROM */
    npb_write_register(NPB_REG_SYSTEM_CONFIG, 0x0404);
}
/* Local Variables: */
/* tab-width: 4 */
/* End: */
