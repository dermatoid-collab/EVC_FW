/*
 * HMI_menu.c
 *
 *  Created on: Jun 8, 2026
 *      Author: abertazza
 */

#include "HMI_menu.h"

//hmi_display_mode_t hmi_display_mode = HMI_SHOW_STATS;
//uint8_t menu_index = 0; // Voce del menu evidenziata

//void hmi_process_input(union HMI_Buttons btn)
//{
//	if (btn.w == 0) return;
//
//    if (hmi_display_mode == HMI_SHOW_STATS)
//    {
//        // Se siamo nelle statistiche, solo il tasto OK (Enter) ha effetto
//        if (btn.ok) {
//            menu_index = machine_state; // Parte evidenziando lo stato attuale
//            hmi_display_mode = HMI_SHOW_MENU; // Entra nel menu
//        }
//    }
//    else if (hmi_display_mode == HMI_SHOW_MENU)
//    {
//        // Se siamo nel menu, i tasti gestiscono lo scorrimento e la conferma
//        if (btn.dn) {
//            menu_index++;
//            if (menu_index > 2) menu_index = 0;
//        }
//        if (btn.up) {
//            menu_index--;
//            if (menu_index < 0) menu_index = MACHINE_STATE_MAX - 1;
//        }
//        if (btn.ok) {
//            machine_state = menu_index; // Salva il nuovo stato
//            hmi_display_mode = HMI_SHOW_STATS;          // Torna alle statisticheK
//        }
//        if (btn.esc) {
//            hmi_display_mode = HMI_SHOW_STATS;          // Torna alle statistiche senza salvare
//        }
//
//    }
//}
/* Local Variables: */
/* tab-width: 4 */
/* End: */
