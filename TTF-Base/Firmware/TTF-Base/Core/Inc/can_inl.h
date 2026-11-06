#ifndef CAN_INL_H
#define CAN_INL_H
#include "can.h"
/** Ritorna il numero di canale dall'indirizzo dell'handler HAL */
inline enum can_channel_id can_channel_id(const CAN_HandleTypeDef *hcan)
{
    return (hcan == &hcan2) ? CAN125 : CAN250;
}

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
inline void can_tx_buffer_reset(enum can_channel_id ch) { can_fifo_buffer_reset(&can_tx_buffer[ch]); }
inline bool can_tx_buffer_push(const struct can_frame *frame, enum can_channel_id ch) {
    /* DEBUG XXX ripete quello spedito sul bus 0 anche sul bus 1 */
    if (ch == CAN125)
        return can_fifo_buffer_push(frame, &can_tx_buffer[CAN125]);
    else
    {
        return can_fifo_buffer_push(frame,
                &can_tx_buffer[CAN125]) &&
                can_fifo_buffer_push(frame,
                &can_tx_buffer[CAN250]);
    }
}
inline bool can_tx_buffer_pop(struct can_frame *frame, enum can_channel_id ch) {
    return can_fifo_buffer_pop(frame, &can_tx_buffer[ch]);
}
inline bool can_tx_buffer_is_empty(enum can_channel_id ch) { return can_fifo_buffer_is_empty(&can_tx_buffer[ch]); }
inline bool can_tx_buffer_is_full(enum can_channel_id ch) { return can_fifo_buffer_is_full(&can_tx_buffer[ch]); }
inline bool can_tx_buffer_is_overflowed(enum can_channel_id ch) { return can_tx_buffer[ch].overflowed; }

inline void can_rx_buffer_reset(enum can_channel_id ch) { can_fifo_buffer_reset(&can_rx_buffer[ch]); }
inline bool can_rx_buffer_push(const struct can_frame *frame, enum can_channel_id ch) {
    return can_fifo_buffer_push(frame, &can_rx_buffer[ch]);
}
inline bool can_rx_buffer_pop(struct can_frame *frame, enum can_channel_id ch) {
    return can_fifo_buffer_pop(frame, &can_rx_buffer[ch]);
}
inline bool can_rx_buffer_is_empty(enum can_channel_id ch) { return can_fifo_buffer_is_empty(&can_rx_buffer[ch]); }
inline bool can_rx_buffer_is_full(enum can_channel_id ch) { return can_fifo_buffer_is_full(&can_rx_buffer[ch]); }
inline bool can_rx_buffer_is_overflowed(enum can_channel_id ch) { return can_rx_buffer[ch].overflowed; }

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
/** Per comodità, buffer 1 */
inline bool can_tx_debug(const uint8_t *v) {
    return can_tx_debugn(TTF_CAN_ID_DEBUG1, v);
}

#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
