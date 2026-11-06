#include <string.h>
#include <stdio.h>
#include "application_inl.h"
#include "HMI_menu.h"
#include "door_inl.h"
#include "can_inl.h"
#include "udec_c_inl.h"
#include "udec_cx_inl.h"
#include "traptray.h"
#include "vfd_inl.h"
#include "trips_inl.h"
#include "npb.h"
#include "l2d.h"
#include "run_mode.h"

/* INGRESSI / USCITE */
DigitalInput_t DIn;
static uint8_t DInDebounceCnt[DInCount];

/* HMI */
hmi_display_mode_t hmi_display_mode = HMI_SHOW_STATS;
int menu_index = 0; // Voce del menu evidenziata

/** enum di appoggio per scorrere il menu */
enum machine_state_hmi {
    HMI_STATE_NORMAL,
    HMI_STATE_FALL_TEST,
    HMI_STATE_MAX
};

/* Inlines */
bool is_reset_btn(void);
bool is_at_P0(void);
bool is_at_P1(void);

/* gestione modalita statistiche e scorrimento menu */
static void hmi_process_input(union HMI_Buttons btn)
{
	static uint32_t press_delay_tmr = 0;

	if (btn.w == 0)
    {
        press_delay_tmr = 0;
        return;
    }

	/* se il "timer" è attivo esco */
    if (press_delay_tmr > 0)
    {
        press_delay_tmr--;
        return;
    }

    if (hmi_display_mode == HMI_SHOW_STATS)
    {
        // Se siamo nelle statistiche, solo il tasto ok ha effetto
        if (btn.ok)
        {
            menu_index = run_mode.state; // parto dallo stato attuale
            hmi_display_mode = HMI_SHOW_MENU; // entro nel menu
            press_delay_tmr = 300;
        }
    }
    else if (hmi_display_mode == HMI_SHOW_MENU)
    {
        // scorro le voci
        if (btn.dn)
        {
            menu_index++;
            if (menu_index > HMI_STATE_MAX)
            	menu_index = 0;
            press_delay_tmr = 300;
        }
        if (btn.up)
        {
            menu_index--;
            if (menu_index < 0)
            	menu_index = HMI_STATE_MAX - 1;
            press_delay_tmr = 300;
        }
        if (btn.ok)
        {
            run_mode.state = MACHINE_STATE_FALL_TEST;
            hmi_display_mode = HMI_SHOW_STATS;      // esco dal menu
            press_delay_tmr = 300;
        }
        if (btn.esc)
        {
            hmi_display_mode = HMI_SHOW_STATS;   // esco dal menu
            press_delay_tmr = 300;
        }
    }
}

/** Enumeratore per scorrere su tutti gli errori attivi */
static enum trip_id trip_next_id = TRIP_ID_NONE;

/** Mostra il prossimo trip. Ritorna false se non ce ne sono */
static bool display_next_trip(void)
{
    enum trip_state trip_next_state = trip_next(&trip_next_id);
    if (trip_next_id == TRIP_ID_NONE)
    {
        /* Nessun messaggio da mostrare */
        return false;
    }
    else
    {
        /* In trip_next_id c'è il codice, e in trip_next_state il flag
         * di corrente */
        static const char *trip_msg[TRIP_ID_SIZE] = {
            "PMode 1 ",
            "PMode 2 ",
            " Safety ",
            "S1 Fuse ",
            "S2 Trave",
            "S3 Belt ",
            "S31 Ospd",
            "S32 EStp",
            "ChnCable",
            "U.C Offl",
            "T.CX Off",
            "TCXCable",
            "UZone   ",
            "OTR FB  ",
            "Tray FB ",
            "Trap FB ",
            "StopTime",
            "CAN Buff",
            "CAN Tx  ",
            "TimerErr",
            "TrapTray",
            "OverVolt",
            "VFDFault",
            "NPBFault",
            "DoorTime"
        };
        strcpy(hmi_lcd_row(0), trip_msg[trip_next_id]);
        /* Marchia un errore archiviato */
        if (trip_next_state == TRIP_HISTORY) {
            hmi_lcd_row(0)[7] = '*';
        }
        return true;
    }
}

static void display_stats(void)
{
    char *row_1 = hmi_lcd_row(0);
    char *row_2 = hmi_lcd_row(1);
    if (tmr_update(TMR_ID_TRIP_DISPLAY, true) || is_new_trip_active())
    {
        if (display_next_trip())
        {
            /* Abbiamo già chiuso con la visualizzazione */
            return;
        }
    }
    if (trip_next_id == TRIP_ID_NONE)
    {
        if (run_mode.bypass_manutenzione)
        {
            strcpy(row_1, "Emergenc");
        }
        else
        {
            uint8_t ss = 0;
            if (DIn.s1) ss |= 0x01;
            if (DIn.s2) ss |= 0x02;
            if (DIn.s3) ss |= 0x04;
            if (udec_cx.in_good && udec_cx.in.s3c) ss |= 0x08;
            if (DIn.s4) ss |= 0x08;
            if (DIn.s8) ss |= 0x10;
            snprintf(row_1, 9, "S%02X s%02d", ss, run_mode.state);
        }
        row_2[0] = magnet_ftc() ? 'M' : 'm';
        row_2[1] = bot_unlock_zone() ? 'B' : 'b';
        row_2[2] = top_unlock_zone() ? 'T' : 't';
        row_2[3] = udec_cx.in_good && udec_cx.out.elt ? 'L' : 'l';
        row_2[4] = udec_c.in_good && door_is_locked() ? 'L' : 'l';
        row_2[5] = udec_c.in_good && door_is_closed() ? 'C' : 'c';
    }
}

/* Dispatch disegno schermo */
static void main_update_display(void)
{
    if (hmi_display_mode == HMI_SHOW_STATS)
    {
        display_stats();
        /* Riga 2 */
        if (run_mode.state == MACHINE_STATE_FALL_TEST)
        {
            strcpy(hmi_lcd_row(1), "FALLTEST");
        }
    }
    else
	{
        strcpy(hmi_lcd_row(0), "MODALITA");
        if (menu_index >= 0 && menu_index < HMI_STATE_MAX)
        {
            const char* hmi_mode_screen[3] = {"NORMALE", "MANUALE", "COLLAUD"};
            snprintf(hmi_lcd_row(1), 9, ">%s", hmi_mode_screen[menu_index]);
        } else
        {
            snprintf(hmi_lcd_row(1), 9, "ERR: %d ", menu_index);
        }
	}
}

/* Dispatch frame CAN, indirizzi standard */
static void process_can_rx_std(const struct can_frame *frame)
{
	switch (frame->id) {
    case CX_CAN_ID_BEAT:
        udec_cx_process_beat();
        break;
	case VFD_CAN_ID_TPDO1:
		vfd_process_can_tpdo1(frame);
		break;
	case VFD_CAN_ID_TPDO2:
		vfd_process_can_tpdo2(frame);
		break;
	case VFD_CAN_ID_TPDO3:
		vfd_process_can_tpdo3(frame);
		break;
	case VFD_CAN_ID_BEAT:
		vfd_process_can_beat(frame);
		main_update_display();
		break;
	default:
		break;
	}
}

/* Dispatch frame CAN, indirizzi estesi (caricabatteria) */
static void process_can_rx_ext(const struct can_frame *frame)
{
    /* Extended ID (battery charger e UDEC) */
    switch (frame->id) {
    case NPB_CAN_RSP_ID:
        npb_process_can_rx(frame);
        main_update_display();
        break;
    case UDEC_CAN_ID(UDEC_TYPE_C, 0, UDEC_ID_DIN):
        udec_c_process_inputs(frame);
        break;
    case UDEC_CAN_ID(UDEC_TYPE_C, 0, UDEC_ID_STATUS_WORD):
        udec_c_process_status_word(frame);
        break;
    case UDEC_CAN_ID(UDEC_TYPE_C, 0, UDEC_ID_CAN_STATUS):
        udec_c_process_status_word(frame);
        break;
    case UDEC_CAN_ID(UDEC_TYPE_L2D, 0, UDEC_ID_STATUS_WORD):
    case UDEC_CAN_ID(UDEC_TYPE_L2D, 8, UDEC_ID_STATUS_WORD):
    	l2d_process_status_word(frame);
    	break;
    case UDEC_CAN_ID(UDEC_TYPE_L2D, 0, UDEC_ID_RD_PARAM_RSP):
    case UDEC_CAN_ID(UDEC_TYPE_L2D, 8, UDEC_ID_RD_PARAM_RSP):
    	l2d_process_read_resp(frame);
    	break;
    case UDEC_CAN_ID(UDEC_TYPE_L2D, 0, UDEC_ID_WR_PARAM_RSP):
    case UDEC_CAN_ID(UDEC_TYPE_L2D, 8, UDEC_ID_WR_PARAM_RSP):
    	l2d_process_write_resp(frame);
    	break;
    case UDEC_CAN_ID(UDEC_TYPE_CX, 0, UDEC_ID_STATUS_WORD):
        udec_cx_process_dout(frame);
		break;
    default:
        can_fifo_buffer_push(frame, &can_tx_buffer[CAN125]);
        break;
    }
}

/* Iterazione e dispatch frame CAN */
static void process_can_rx(const CAN_HandleTypeDef *hcan)
{
    struct can_frame frame;

    /* seleziono il buffer a seconda del canale */
    enum can_channel_id ch = can_channel_id(hcan);

    while (can_rx_buffer_pop(&frame, ch)) {
        /* Due rami completamente distinti per i due tipi di indirizzo */
        if (frame.IDE == CAN_ID_STD) {
            process_can_rx_std(&frame);
        } else {
            process_can_rx_ext(&frame);
        }
    }
}

/** Utility: assert sia dell'id specificato che del generico di sicurezza */
static bool trip_safety(enum trip_id id, bool cond)
{
    trip_assert(id, cond);
    trip_assert(TRIP_SAFETY, cond);
    return cond;
}

/** Controlli assortiti sulle sicurezze */
static void check_safety(void)
{
	/* CONTROLLO CONGRUENZA CATENA SICUREZZE */
    if (!run_mode.bypass_manutenzione)
    {
        /* Cerca di minimizzare gli errori rilevati in cascata */
        bool chain_fail = false;
        /* buchi nelle sicurezze bloccanti, in catena */
        chain_fail |= trip_safety(TRIP_S1_FUSE,
                !chain_fail && !DIn.s1);
        chain_fail |= trip_safety(TRIP_S2_OVERTRAVEL,
                !chain_fail && !DIn.s2);
        chain_fail |= trip_safety(TRIP_S3_BELT,
                !chain_fail && !DIn.s3);

        /* Controlli vincolati a dati da UDEC.C */
        /* Timeout comunicazione con UDEC.C */
        trip_assign(TRIP_UDEC_C_NMT, !udec_c.online);
        if (udec_c.in_good)
        {
            chain_fail |= trip_safety(TRIP_S31_OVERSPEED,
                    !chain_fail && !udec_c.in.s31_par);
            /* Stop in cabina, non bloccante */
            chain_fail |= trip_assign(TRIP_S32_CAR_ESTOP,
                     !chain_fail && udec_c.in.s31_par && !udec_c.in.s32_pec);
            /* errore cablaggio cavo catena */
            trip_safety(TRIP_CHAIN_CABLE,
                    !chain_fail && tmr_update(TMR_ID_ERR_SYNC,
                            (udec_cx.in.s3 != DIn.s3)
                            || (udec_cx.in.s4 != DIn.s4)));
        }
        /* Timeout comunicazione con TTF.CX */
        trip_assign(TRIP_TTF_CX_NMT, !udec_cx.online);
        /* Errori vincolati da segnali da UDEC.CX */
        if (udec_cx.in_good)
        {
            /* Errore cablaggio tetto */
            trip_safety(TRIP_TTF_CX_CABLE, tmr_update(TMR_ID_ERR_SYNC,
                            udec_c.in_good && (udec_c.in.s36_ft != udec_cx.in.s3a)));
            /* rilevate zone sblocco contemporanee */
            trip_safety(TRIP_UZONE,
                    udec_cx.in.bot_uzone && udec_cx.in.top_uzone);
            /* Guasto relè bypass tray */
            trip_safety(TRIP_TRAY_FB,
                    tmr_update(TMR_ID_FBK_BOT_REL,
                            udec_cx.out.en_byp_bot == udec_cx.in.fbk_byp_bot));
            /* Guasto relè bypass trap */
            trip_safety(TRIP_TRAP_FB,
                    tmr_update(TMR_ID_FBK_TOP_REL,
                            udec_cx.in.fbk_byp_top == udec_cx.out.en_byp_top));
            /* È funzionalmente (ma non elettricamente) impossibile */
            trip_safety(TRIP_TRAPTRAY,
                    traptray.state == TRAPTRAY_FAULT);
        }
        /* Guasto relè alimentazione.
         * Lo controllo se ho alimentazione sulle sicurezze, altrimenti in caso di arresto
         * non bloccante (stop cabina, tetto o fondo) va in fault */
        trip_safety(TRIP_OTR_FB, tmr_update(TMR_ID_OTR_REL,
                        (DIn.otr_fb == pmode.output_coil) && DIn.s8));
    }
    /* Ferma il mondo */
    if (is_trip_active(TRIP_SAFETY))
    {
        pmode_request(PMODE_REQ_CHARGER_NOW);
    }
}

static void read_inputs(void)
{
    bool DInRaw[DInCount];

    DInRaw[0] = HAL_GPIO_ReadPin(IN1_1_GPIO_Port, IN1_1_Pin);       // Bit 0: Su
    DInRaw[1] = HAL_GPIO_ReadPin(IN1_2_GPIO_Port, IN1_2_Pin);       // Bit 1: Giù
    DInRaw[2] = HAL_GPIO_ReadPin(IN1_3_GPIO_Port, IN1_3_Pin);       // Bit 2: Sblocco superiore
    DInRaw[3] = HAL_GPIO_ReadPin(IN1_4_GPIO_Port, IN1_4_Pin);       // Bit 3: Sblocco inferiore
    DInRaw[4] = HAL_GPIO_ReadPin(BYP_S14_GPIO_Port, BYP_S14_Pin);   // Bit 4: Bypass manutenzione
    DInRaw[5] = HAL_GPIO_ReadPin(S1_GPIO_Port, S1_Pin);				// Bit 5: inizio catena sicurezze
    DInRaw[6] = HAL_GPIO_ReadPin(S2_GPIO_Port, S2_Pin);				// Bit 6: allentamento funi
    DInRaw[7] = HAL_GPIO_ReadPin(S3_GPIO_Port, S3_Pin);				// Bit 7: inizio catena sicurezze cabina
    DInRaw[8] = HAL_GPIO_ReadPin(S4_GPIO_Port, S4_Pin);				// Bit 8: fine catena sicurezze cabina
    DInRaw[9] = HAL_GPIO_ReadPin(S8_GPIO_Port, S8_Pin);				// Bit 9: chisura catena sicurezze
    DInRaw[10] = HAL_GPIO_ReadPin(OTR_FB_GPIO_Port, OTR_FB_Pin);	// Bit 10: feedback relè principale
    DInRaw[11] = HAL_GPIO_ReadPin(BRK_FB_GPIO_Port, BRK_FB_Pin);	// Bit 11: feedback attivazione freno

    /* Filtro sugli ingressi: la priorità è sullo spento, che viene
       rilevato immediatamente; il passaggio ad attivo viene validato
       dopo alcuni cicli. Il tempo di ciclo NON è costante ma non
       è critico */
    for (unsigned i = 0; i < DInCount; i++)
    {
        if (DInRaw[i])
        {
            /* Filtro prima di passare a 1 */
            if (DInDebounceCnt[i] < DInDebounce)
        	{
        		DInDebounceCnt[i]++;
        	}
            else
            {
                DIn.w |= DInRaw[i] << i;
            }
        }
        else
        {
            /* Passaggio a zero immediato */
            DIn.w &= ~(0x1 << i);
            DInDebounceCnt[i] = 0;
        }
    }
    run_mode.bypass_manutenzione = DIn.bypass;

    /* Check reset hardware da quadro premendo UP e DN insieme */
    if (tmr_update(TMR_ID_HMI_RESET_TIMEOUT, (hmi_buttons()->w == 3)))
    {
    	vfd_reset();
        run_mode.reset_request = true;
    }
    if (tmr_update(TMR_ID_COP_RESET_TIMEOUT, is_reset_btn()))
	{
        vfd_reset();
        run_mode.reset_request = true;
    }
}

static void write_outputs(void)
{
    static uint32_t timestamp = 0;

    /* DEBUG (qui ci sta un timer D) */
    if ((uwTick - timestamp) >= 1000)
    {
        HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
        timestamp = HAL_GetTick();
    }

    /* Indicazioni per corrente di batteria */
    if (battery.current > BAT_LED_CURRENT)
    {
        HAL_GPIO_WritePin(LED_CHG_GPIO_Port, LED_CHG_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_DIS_GPIO_Port, LED_DIS_Pin, GPIO_PIN_RESET);
    }
    else if (battery.current < -BAT_LED_CURRENT)
    {
        HAL_GPIO_WritePin(LED_CHG_GPIO_Port, LED_CHG_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_DIS_GPIO_Port, LED_DIS_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(LED_CHG_GPIO_Port, LED_CHG_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_DIS_GPIO_Port, LED_DIS_Pin, GPIO_PIN_RESET);
    }
}

/** Attende che il reset sia inviato al VFD prima di resettare il micro */
static void check_reset(void)
{
    if (run_mode.reset_request)
	{
        vfd_reset();
        if (tmr_update(TMR_ID_VFD_RESET_DELAY, true))
		{
			NVIC_SystemReset();
        }
	}
}

/** gestione molto basilare delle abilitazioni */
static void abilitation_check(void)
{
    /* Attenzione: il VFD non è ancora necessariamente attivo in quanto
       c'è il tempo di commutazione da carica a run (la transizione
       è fatta dalla richiesta motore). Anche la presenza di un errore
       non è *necessariamente* bloccante, in quanto alcuni errori
       sono previsti.

       In pratica la condizione va pensata bene, per il momento ci
       aspettiamo solo che sia attivo sul CAN e non abbia fault
       appariscenti
    */

    /* Il bypass non è onnipotente anche se fornisce tutte le sicurezze
     * elettriche */
    run_mode.enable = DIn.s8
            && !vfd_in_fault()
            && !pmode_in_fault();
    if (!run_mode.bypass_manutenzione)
    {
        run_mode.enable = run_mode.enable
                && !battery.overvoltage
                && udec_cx.online && udec_cx.in_good
                && udec_c.online && udec_c.in_good
                && !is_trip_active(TRIP_SAFETY);
    }
}

/** Gestione prenotazioni in corso */
static void set_prenotation(void)
{
    /* hl_pc0 e hl_pc1 sono le luminarie */
    if (is_P0_on() || (DIn.In_2 && !DIn.In_1))
	{
        run_mode.call = PREN_P0;
        udec_c.out.hl_pc0 = true;
        udec_c.out.hl_pc1 = false;
	}
    else if (is_P1_on() || (DIn.In_1 && !DIn.In_2))
    {
        run_mode.call = PREN_P1;
        udec_c.out.hl_pc0 = false;
        udec_c.out.hl_pc1 = true;
    }
    else if (!is_P0_on() && !is_P1_on() && reopen_door_button())
    {
        set_open_door_request();
    }
    else
	{
        run_mode.call = PREN_NONE;
        udec_c.out.hl_pc0 = false;
        udec_c.out.hl_pc1 = false;
    }
}

/** Invio beat a VFD e a CX */
static void send_heartbeat(void)
{
    /* Il beat viene inviato su entrambi i bus; se non entra nella coda
     * e perde la finestra è corretto che vada offline (per sovraccarico
     * del bus) */
    if (tmr_update(TMR_ID_NMT_PERIOD, true)) {
        canopen_tx_push_heartbeat(TTF_CAN_ID_BEAT, CAN125);
        canopen_tx_push_heartbeat(TTF_CAN_ID_BEAT, CAN250);
    }
}

void main_loop(void)
{
    read_inputs();
    abilitation_check();

    /* Aspetta il controllo delle sicurezze a quando è stato inizializzato tutto */
    if ((run_mode.state > MACHINE_STATE_INIT_SEQ)
            && (HAL_GetTick() > TIMEOUT_INITIAL_ERRCHECK))
    {
		check_safety();
    }
	set_prenotation();
	main_update_display();
    /* Trasferisce dalle fifo hardware al buffer circolare */
    /* TODO: sarà in IRQ! */
    can_pull_hw_fifo(&hcan1);
    can_pull_hw_fifo(&hcan2);
    /* Elabora i frame in coda */
    process_can_rx(&hcan1);
    process_can_rx(&hcan2);
    /* Scan cycle */
    run_mode_scan_cycle();
    /* Caricabatterie */
    npb_scan_cycle();
    /* Inverter */
    vfd_scan_cycle();
    /* Comunicazione con schede remote */
    udec_c_scan_cycle();
    udec_cx_scan_cycle();
    l2d_scan();
    /* Altri servizi essenziali */
    pmode_scan_cycle();
    battery_scan();
    traptray_scan_cycle();
    /* Aggiorna le uscite */
    write_outputs();
    udec_c_send_outputs();
    udec_cx_send_outputs();
    send_heartbeat();
    check_reset();
    hmi_handler_bb();
    /* Menu cambio modalita */
    hmi_process_input(*hmi_buttons());
    tmr_end_scan();
    /* Scarica il buffer CAN */
    can_push_hw_fifo(&hcan1);
    can_push_hw_fifo(&hcan2);
}
/* Local Variables: */
/* tab-width: 4 */
/* End: */
