#ifndef INC_CAN_H_
#define INC_CAN_H_

#include "main.h"

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
    TTF_CAN_ID_BEAT = 0x77F,
    CX_CAN_ID_BEAT = 0x702,
};
/* stati NMT */
enum {
    NMT_STATE_BOOT = 0x00,
    NMT_STATE_STOP = 0x04,
    NMT_STATE_OPER = 0x05,
    NMT_STATE_PRE_OPER = 0x7F,
};

#define UDEC_CAN_ID(type, address, id) \
    ((type << 16) | (address << 11) | id)

/* Un frame CAN completo, non quella schifezza di CAN_TxHeaderTypeDef con l'array al seguito */
struct can_frame {
    uint32_t id;               /* Standard o esteso, dipende dall'IDE */
    bool IDE;                  /* true se indirizzo esteso */
    bool RTR;                  /* Deprecati ma si usano lo stesso… */
    uint8_t DLC;               /* Lunghezza frame, byte dati validi */
    uint8_t data[8];           /* CAN standard, non flex o XL */
};
/* Travasi da struct con HAL */
void can_frame_fetch(struct can_frame *frame, const CAN_RxHeaderTypeDef *hdr, const uint8_t data[8]);
void can_frame_store(CAN_TxHeaderTypeDef *hdr, uint8_t data[8], const struct can_frame *frame);

/* Dunque: in ricezione una coda FIFO sarebbe corretta, in uscita la
   struttura da usare sarebbe una priority queue. *Ricordarsi* il CAN
   non garantisce l'ordine di trasmissione dei frame!

   Al momento, finché non saltan fuori problemi usiamo una FIFO anche
   per i frame in uscita */
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
extern struct can_fifo_buffer can_tx_buffer, can_rx_buffer;
/* L'esito è false se non c'è spazio nel buffer risp. il buffer è vuoto */
bool can_fifo_buffer_push(const struct can_frame *frame, struct can_fifo_buffer *fifo);
bool can_fifo_buffer_pop(struct can_frame *frame, struct can_fifo_buffer *fifo);
void can_fifo_buffer_reset(struct can_fifo_buffer *fifo);

/* Il buffer è vuoto quando i due puntatori sono identici */
inline bool can_fifo_buffer_is_empty(const struct can_fifo_buffer *fifo)
{
    return fifo->write_ptr == fifo->read_ptr;
}
/* Il buffer è pieno se, avanzando il puntatore in scrittura, si raggiunge quello in lettura */
inline bool can_fifo_buffer_is_full(const struct can_fifo_buffer *fifo)
{
    return fifo->read_ptr == ((fifo->write_ptr + 1) % CAN_FIFO_SIZE);
}

/* Specializzazioni su code di trasmissione e ricezione */
inline void can_tx_buffer_reset(void) { can_fifo_buffer_reset(&can_tx_buffer); }
inline bool can_tx_buffer_push(const struct can_frame *frame) {
    return can_fifo_buffer_push(frame, &can_tx_buffer);
}
inline bool can_tx_buffer_pop(struct can_frame *frame) {
    return can_fifo_buffer_pop(frame, &can_tx_buffer);
}
inline bool can_tx_buffer_is_empty(void) { return can_fifo_buffer_is_empty(&can_tx_buffer); }
inline bool can_tx_buffer_is_full(void) { return can_fifo_buffer_is_full(&can_tx_buffer); }
inline bool can_tx_buffer_is_overflowed(void) { return can_tx_buffer.overflowed; }

inline void can_rx_buffer_reset(void) { can_fifo_buffer_reset(&can_rx_buffer); }
inline bool can_rx_buffer_push(const struct can_frame *frame) {
    return can_fifo_buffer_push(frame, &can_rx_buffer);
}
inline bool can_rx_buffer_pop(struct can_frame *frame) {
    return can_fifo_buffer_pop(frame, &can_rx_buffer);
}
inline bool can_rx_buffer_is_empty(void) { return can_fifo_buffer_is_empty(&can_tx_buffer); }
inline bool can_rx_buffer_is_full(void) { return can_fifo_buffer_is_full(&can_rx_buffer); }
inline bool can_rx_buffer_is_overflowed(void) { return can_rx_buffer.overflowed; }

/* Trasferisce da FIFO HW a buffer e viceversa. False se ci sono stati problemi */
bool can_pull_hw_fifo(CAN_HandleTypeDef *hcan);
bool can_push_hw_fifo(CAN_HandleTypeDef *hcan);

/* Estrazione/inserimento da/in area dati */
inline uint32_t can_extract_uint32(const uint8_t *data) {
    return data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
}
inline uint16_t can_extract_uint16(const uint8_t *data) {
    return data[0] | (data[1] << 8);
}
inline int16_t can_extract_int16(const uint8_t *data) {
    return (int16_t)(data[0] | (data[1] << 8));
}
inline void can_store_uint32(uint8_t *data, uint32_t v) {
    data[0] = v;
    data[1] = v >> 8;
    data[2] = v >> 16;
    data[3] = v >> 24;
}
inline void can_store_uint16(uint8_t *data, uint16_t v) {
    data[0] = v;
    data[1] = v >> 8;
}
/* Utility: trasmette una word di 32 bit ad un indirizzo esteso (caso comunissimo) */
bool can_tx_push_halfword_std(uint16_t std_id, uint16_t data);
bool can_tx_push_halfword_ext(uint32_t ext_id, uint16_t data);
bool can_tx_push_word_ext(uint32_t ext_id, uint32_t data);
bool can_tx_push_fullframe_ext(uint32_t ext_id, uint8_t* data);
bool canopen_tx_push_heartbeat(uint16_t cobid);

unsigned can_channel_id(const CAN_HandleTypeDef *hcan);

extern CAN_HandleTypeDef hcan1, hcan2;


#endif /* INC_CAN_H_ */

/* Local Variables: */
/* tab-width: 4 */
/* End: */
