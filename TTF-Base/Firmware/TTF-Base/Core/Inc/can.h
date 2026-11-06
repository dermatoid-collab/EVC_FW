#ifndef INC_CAN_H_
#define INC_CAN_H_
#include <stdbool.h>
#include <stdint.h>
#include "main.h"

/** Ci sono due bus: uno in cappello a 250 kbps e uno in catena da 125
 * kbps */
enum can_channel_id {
    CAN250 = 0,
    CAN125 = 1
};

/* Macro per generare gli indirizzi UDEC */
enum {
    UDEC_TYPE_C      = 0x0A,
    UDEC_TYPE_CX     = 0x0B,
    UDEC_TYPE_M      = 0x0F,
    UDEC_TYPE_D      = 0x14,
    UDEC_TYPE_L2D    = 0x15,
    UDEC_TYPE_LCDCOP = 0x16
};
enum {
    UDEC_ID_DIN = 0x64,
    UDEC_ID_DOUT = 0xC8,
    UDEC_ID_ELT = 0xC9,
    UDEC_ID_AIN = 0x12C,
    UDEC_ID_STATUS_WORD = 0x190,
    UDEC_ID_CONTROL_WORD = 0x19A,
    UDEC_ID_CAN_STATUS = 0x1A4,
    UDEC_ID_RD_PARAM_REQ = 0x1F4,
    UDEC_ID_RD_PARAM_RSP = 0x1F5,
    UDEC_ID_WR_PARAM_REQ = 0x1FE,
    UDEC_ID_WR_PARAM_RSP = 0x1FF
};
enum {
    NMT_STATE_BOOT = 0x00,
    NMT_STATE_STOP = 0x04,
    NMT_STATE_OPER = 0x05,
    NMT_STATE_PRE_OPER = 0x7F,
};
enum {
    TTF_CAN_ID_BEAT = 0x77F,
    CX_CAN_ID_BEAT = 0x702,
    TTF_CAN_ID_DEBUG1 = 0x1FF,
    TTF_CAN_ID_DEBUG2 = 0x2FF,
    TTF_CAN_ID_DEBUG3 = 0x3FF,
};

#define UDEC_CAN_ID(type, address, id) \
    ((type << 16) | (address << 11) | id)

/** Un frame CAN completo, non quella schifezza di CAN_TxHeaderTypeDef con l'array al seguito */
struct can_frame {
    uint32_t id;               /* Standard o esteso, dipende dall'IDE */
    bool IDE;                  /* true se indirizzo esteso */
    bool RTR;                  /* Deprecati ma si usano lo stesso… */
    uint8_t DLC;               /* Lunghezza frame, byte dati validi */
    uint8_t data[8];           /* CAN standard, non flex o XL */
};

/* Dunque: in ricezione una coda FIFO sarebbe corretta, in uscita la
   struttura da usare sarebbe una priority queue. *Ricordarsi* il CAN
   non garantisce l'ordine di trasmissione dei frame!

   Al momento, finché non saltan fuori problemi di priorità, usiamo una FIFO anche
   per i frame in uscita. */
enum {
    CAN_FIFO_SIZE = 16,
};
struct can_fifo_buffer {
    /* È un normale buffer circolare. Il puntatore in scrittura
     * è potenzialmente aggiornato da interrupt; è necessario usare
     * quello in lettura per verificare lo stato ma non c'è rischio, al
     * massimo va in overflow un elemento in anticipo. */
    volatile uint8_t write_ptr;
    uint8_t read_ptr;
    volatile bool overflowed;
    struct can_frame frames[CAN_FIFO_SIZE];
};
extern struct can_fifo_buffer can_tx_buffer[2], can_rx_buffer[2];
/* L'esito è false se non c'è spazio nel buffer risp. il buffer è vuoto */
bool can_fifo_buffer_push(const struct can_frame *frame, struct can_fifo_buffer *fifo);
bool can_fifo_buffer_pop(struct can_frame *frame, struct can_fifo_buffer *fifo);
void can_fifo_buffer_reset(struct can_fifo_buffer *fifo);

/* Trasferisce da FIFO HW a buffer e viceversa. False se ci sono stati problemi */
bool can_pull_hw_fifo(CAN_HandleTypeDef *hcan);
bool can_push_hw_fifo(CAN_HandleTypeDef *hcan);

/* Altre utility per frame standard */
bool can_tx_push_halfword_std(uint16_t std_id, uint16_t data, enum can_channel_id ch);
bool can_tx_push_halfword_ext(uint32_t ext_id, uint16_t data, enum can_channel_id ch);
bool can_tx_push_word_ext(uint32_t ext_id, uint32_t data, enum can_channel_id ch);
bool can_tx_push_fullframe_ext(uint32_t ext_id, const uint8_t* data, enum can_channel_id ch);
bool canopen_tx_push_heartbeat(uint16_t cobid, enum can_channel_id bus);
bool can_tx_debugn(uint16_t cobid, const uint8_t *v);
extern CAN_HandleTypeDef hcan1, hcan2;

#endif /* INC_CAN_H_ */
/* Local Variables: */
/* tab-width: 4 */
/* End: */
