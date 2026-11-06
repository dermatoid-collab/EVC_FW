#include <string.h>
#include "main.h"
#include "can_inl.h"
#include "pmode.h"
#include "trips.h"

/* uso due istanze per i due canali can
 * 0 => CAN1 => 250kbps
 * 1 => CAN2 => 125kbps */
struct can_fifo_buffer can_tx_buffer[2], can_rx_buffer[2];

/* Instanze inline */
bool can_fifo_buffer_is_empty(const struct can_fifo_buffer *fifo);
bool can_fifo_buffer_is_full(const struct can_fifo_buffer *fifo);
void can_tx_buffer_reset(enum can_channel_id ch);
bool can_tx_buffer_push(const struct can_frame *frame, enum can_channel_id ch);
bool can_tx_buffer_pop(struct can_frame *frame, enum can_channel_id ch);
bool can_tx_buffer_is_empty(enum can_channel_id ch);
bool can_tx_buffer_is_full(enum can_channel_id ch);
bool can_tx_buffer_is_overflowed(enum can_channel_id ch);
void can_rx_buffer_reset(enum can_channel_id ch);
bool can_rx_buffer_push(const struct can_frame *frame, enum can_channel_id ch);
bool can_rx_buffer_pop(struct can_frame *frame, enum can_channel_id ch);
bool can_rx_buffer_is_empty(enum can_channel_id ch);
bool can_rx_buffer_is_full(enum can_channel_id ch);
bool can_rx_buffer_is_overflowed(enum can_channel_id ch);
uint32_t can_extract_uint32(const uint8_t *data);
uint16_t can_extract_uint16(const uint8_t *data);
int16_t can_extract_int16(const uint8_t *data);
void can_store_uint32(uint8_t *data, uint32_t v);
void can_store_uint16(uint8_t *data, uint16_t v);
bool can_tx_debug(const uint8_t *v);
enum can_channel_id can_channel_id(const CAN_HandleTypeDef *hcan);

/* Reset completo, incluso il caso di overflow */
void can_fifo_buffer_reset(struct can_fifo_buffer *fifo)
{
    fifo->write_ptr = 0;
    fifo->read_ptr = 0;
    fifo->overflowed = false;
}

/* Trasferisce da HAL a buffer */
static void can_frame_fetch(struct can_frame *frame, const CAN_RxHeaderTypeDef *hdr, const uint8_t data[8])
{
    frame->RTR = hdr->RTR;
    frame->IDE = hdr->IDE;
    frame->DLC = hdr->DLC;
    if (hdr->IDE) {
        frame->id = hdr->ExtId;
    } else {
        frame->id = hdr->StdId;
    }
    if (!frame->RTR) {
        memcpy(frame->data, data, hdr->DLC);
    }
}

/* Trasferisce da buffer a HAL */
static void can_frame_store(CAN_TxHeaderTypeDef *hdr, uint8_t data[8], const struct can_frame *frame)
{
    hdr->RTR = frame->RTR ? CAN_RTR_REMOTE : CAN_RTR_DATA;
    hdr->IDE = frame->IDE ? CAN_ID_EXT : CAN_ID_STD;
    hdr->DLC = frame->DLC;
    /* Tanto poi ne usa solo uno… */
    hdr->StdId = frame->id;
    hdr->ExtId = frame->id;
    if (!frame->RTR) {
        memcpy(data, frame->data, hdr->DLC);
    }
    hdr->TransmitGlobalTime = false; /* Non usiamo TTCAN */
}

bool can_fifo_buffer_push(const struct can_frame *frame, struct can_fifo_buffer *fifo)
{
    /* Il modo più semplice per sapere se è pieno è provare a incrementarlo */
    unsigned next_ptr = (fifo->read_ptr + 1) % CAN_FIFO_SIZE;
    if (fifo->read_ptr == next_ptr) {
        fifo->overflowed = true; /* Ricorda la tragedia per i posteri */
        trip_assert(TRIP_CAN_BUFFER, true);
        return false;
    }
    fifo->frames[fifo->write_ptr] = *frame;
    fifo->write_ptr = next_ptr;
    return true;
}

bool can_fifo_buffer_pop(struct can_frame *frame, struct can_fifo_buffer *fifo)
{
    /* Caso facile: buffer vuoto */
    if (fifo->write_ptr == fifo->read_ptr) {
        return false;
    }
    *frame = fifo->frames[fifo->read_ptr];
    /* Importante che venga fatto per ultimo in modo da non farsi
     * pestare i piedi da un eventuale interrupt. */
    fifo->read_ptr = (fifo->read_ptr + 1) % CAN_FIFO_SIZE;
    return true;
}

/** Trasporta i dati dal buffer circolare di trasmissione nel controller
 * per la spedizione */
bool can_push_hw_fifo(CAN_HandleTypeDef *hcan)
{
    /* seleziono il buffer a seconda del canale */
    enum can_channel_id ch = can_channel_id(hcan);

    while (!can_tx_buffer_is_empty(ch) &&
            (HAL_CAN_GetTxMailboxesFreeLevel(hcan) > 0)) {
        CAN_TxHeaderTypeDef hdr;
        uint8_t data[8];
        struct can_frame frame;
        if (!can_tx_buffer_pop(&frame, ch)) {
            trip_assert(TRIP_CAN_TX, true);
            return false;
        }
        can_frame_store(&hdr, data, &frame);
        uint32_t mbx; /* Non interessa ma va indicata */

        if (HAL_CAN_AddTxMessage(hcan, &hdr, data, &mbx) != HAL_OK) {
            trip_assert(TRIP_CAN_TX, true);
            return false; /* Ed è stato perso un frame! */
        }
    }
    return true;
}

/** Trasporta i dati dalle due FIFO del controller nel buffer circolare
 * (per un totale di sei possibili frame) */
bool can_pull_hw_fifo(CAN_HandleTypeDef *hcan)
{
    /* seleziono il buffer a seconda del canale */
    enum can_channel_id ch = can_channel_id(hcan);

    for (unsigned fifo = 0; fifo < 2; ++fifo) {
        if (HAL_CAN_GetRxFifoFillLevel(hcan, fifo)) {
            /* Se il buffer in ricezione è pieno non è immediatamente
             * grave, finché regge la FIFO HW */
            if (can_rx_buffer_is_full(ch)) {
                return true;
            }
            struct can_frame rxbuf;
            CAN_RxHeaderTypeDef hdr;
            uint8_t data[8];
            if (HAL_CAN_GetRxMessage(hcan, fifo, &hdr, data) == HAL_ERROR) {
                /* Uhmmm */
                return false;
            }
            can_frame_fetch(&rxbuf, &hdr, data);
            bool rc = can_rx_buffer_push(&rxbuf, ch);
            if (!rc) {
                /* 99% un overflow inatteso */
                return false;
            }
        }
    }
    return true;
}

/* Utility per comporre frame CAN banali */
bool can_tx_push_halfword_std(uint16_t std_id, uint16_t data, enum can_channel_id ch)
{
    struct can_frame txbuf;
    txbuf.id = std_id;
    txbuf.IDE = CAN_ID_STD;
    txbuf.RTR = false;
    txbuf.DLC = 2;
    can_store_uint16(txbuf.data, data);
    return can_tx_buffer_push(&txbuf, ch);
}

bool can_tx_push_halfword_ext(uint32_t ext_id, uint16_t data, enum can_channel_id ch)
{
    struct can_frame txbuf;
    txbuf.id = ext_id;
    txbuf.IDE = CAN_ID_EXT;
    txbuf.RTR = false;
    txbuf.DLC = 2;
    can_store_uint16(txbuf.data, data);
    return can_tx_buffer_push(&txbuf, ch);
}

bool can_tx_push_word_ext(uint32_t ext_id, uint32_t data, enum can_channel_id ch)
{
    struct can_frame txbuf;
    txbuf.id = ext_id;
    txbuf.IDE = CAN_ID_EXT;
    txbuf.RTR = false;
    txbuf.DLC = 4;
    can_store_uint32(txbuf.data, data);
    return can_tx_buffer_push(&txbuf, ch);
}

bool can_tx_push_fullframe_ext(uint32_t ext_id, const uint8_t* data, enum can_channel_id ch)
{
    struct can_frame txbuf;
    txbuf.id = ext_id;
    txbuf.IDE = CAN_ID_EXT;
    txbuf.RTR = false;
    txbuf.DLC = 8;
    memcpy(txbuf.data, data, txbuf.DLC);
    return can_tx_buffer_push(&txbuf, ch);
}

/** Trasmette un heartbeat CANopen, sempre in operational */
bool canopen_tx_push_heartbeat(uint16_t cobid, enum can_channel_id bus)
{
    /* Lo stato NMT si riferisce allo stack CAN, quindi sempre OPER */
    struct can_frame txbuf;
    txbuf.id = cobid;
    txbuf.IDE = CAN_ID_STD;
    txbuf.RTR = false;
    txbuf.DLC = 1;
    txbuf.data[0] = NMT_STATE_OPER;

    return can_tx_buffer_push(&txbuf, bus);
}

/** Invia frame di debug, TPDO del nodo 7F, essenzialmente; sempre
    8 byte freeform. La trasmissione avviene sempre sul bus a bassa
    velocità. */
bool can_tx_debugn(uint16_t cobid, const uint8_t *v)
{
    struct can_frame txbuf;
    txbuf.id = cobid;
    txbuf.IDE = CAN_ID_STD;
    txbuf.RTR = false;
    txbuf.DLC = 8;
    memcpy(txbuf.data, v, 8);

    return can_tx_buffer_push(&txbuf, CAN125);
}
/* Local Variables: */
/* tab-width: 4 */
/* End: */
