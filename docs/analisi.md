# Analisi del progetto TTF / UDEC.CX

Documento redatto a partire dal contenuto del repo (stato del commit di import). Raccoglie tre analisi:

1. [Panoramica del progetto](#1-panoramica-del-progetto)
2. [Revisione del codice di sicurezza](#2-revisione-del-codice-di-sicurezza)
3. [Analisi della catena delle sicurezze](#3-analisi-della-catena-delle-sicurezze)

**Limiti.** Sono analisi di lettura: nessun codice è stato eseguito o modificato. Non sono stati esaminati layout PCB, datasheet dei componenti, firmware di UDEC.CX né i modelli FreeCAD. Non è una valutazione di conformità alle norme di prodotto. I riferimenti a file e righe del firmware sono relativi a `TTF-Base/Firmware/TTF-Base/Core/`.

---

## 1. Panoramica del progetto

Sistema di controllo di un ascensore domestico a trazione (TTF) di LiftingItalia s.r.l.: quattro schede basate su STM32F446, collegate in CAN, con motore comandato da un VFD.

| Cartella | Contenuto |
|---|---|
| `TTF-Base/` | Scheda master (STM32F446VC, PCB a 4 strati, 420 componenti): schemi KiCad, Gerber, firmware CubeIDE (~8.200 righe di C). |
| `TTF-CX/` | Scheda di tetto cabina (F446RC, 4 strati): ingressi S3/S4, bypass trap/tray, zone di sblocco, cella di carico, illuminazione. |
| `UDEC.CX/` | Scheda collegata alla cabina (F446VC, 4 strati, 464 componenti): audio, pulsantiera, interfaccia VFD, cella di carico; simulazioni LTspice. |
| `Schema Macchina/` | Schema elettrico generale (cabina, cappello, servizi). |
| `F510.05.9019 Scheda Consenso Paracadute/` | PCB a 1 strato e modelli FreeCAD del sensore paracadute. |

### Firmware TTF-Base

Super-loop con timer software e macchina a stati. Moduli principali:

- `run_mode.c`: macchina a stati del movimento (`NORMAL`, `UP`, `DOWN`, `OPEN_DOOR`, `CLOSE_DOOR`, `STUCK_UP`, `FALL_TEST`, `WAIT_FOR_CALL_RELEASE`) e sequenza di abilitazione.
- `door.c`: ciclo porte.
- `pmode.c`, `battery.c`, `npb.c`, `ltc2959.c`: commutazione caricabatteria/VFD, carica e misura batteria.
- `vfd.c`: comando dell'azionamento via CANopen.
- `trips.c`, `trip_list.h`: 25 tipi di errore (inattivo / attivo / memorizzato).
- `udec_c.c`, `udec_cx.c`, `l2d.c`, `can.c`: comunicazione CAN con le altre schede (CAN a circa 250 kbit/s se APB1 è a 45 MHz; non verificato).
- `HMI.c`, `HMI_menu.c`, `lcd_hd44780_i2c.c`: display LCD e menu.

### Note iniziali

- Il Changelog è molto recente (ultima voce 2026-11-05): sviluppo attivo.
- Sono presenti diversi `XXX`/`TODO` aperti: timeout porta (`door.c`), timeout di carica (`battery.c:69`), soglia `DCLINK_OVERVOLTAGE` provvisoria, `L2D_PARAM` saltato in `run_mode_init_seq`, ripetizione di traffico CAN sul secondo bus lasciata dal debug (`can_inl.h:24`), anomalie in `i2c_bb.c` e LCD bloccante.
- Il workaround in `vfd_timed_out` ammette nel Changelog: "in questo modo si perde la fermata!".

---

## 2. Revisione del codice di sicurezza

File esaminati: `application.c`, `run_mode.c`, `door.c`, `vfd.c`, `pmode.c`, `trips.c`, `traptray.c`, `udec_c.c`, `udec_cx.c`, i relativi `_inl.h`, `timer.c` e il firmware di TTF-CX.

### 2.1 Gravi

1. **Il collaudo paracadute parte da qualsiasi voce di menu** (`Src/application.c:81-86`).
   Premendo OK nel menu si imposta sempre `MACHINE_STATE_FALL_TEST`: la voce selezionata (`menu_index`) è ignorata, quindi anche "NORMALE" avvia il collaudo. Lo stato viene impostato da qualsiasi stato, anche con la cabina in marcia, senza protezioni (chiave, combinazione di tasti). In FALL_TEST, `run_mode_free_fall` apre il freno con il tasto 2 o con `In_4`.
   *Suggerito:* controllo sulla voce scelta, abilitazione deliberata, condizione "cabina ferma".

2. **Freno non verificato e discesa che può partire verso l'alto senza limiti** (`Src/run_mode.c:120-135`, `165-200`).
   Prima di scendere, `run_mode_brake_release` comanda una marcia SU a 11 rpm finché `brk_fb` non resta attivo 100 ms, senza timeout. In `run_mode_down` non si controlla `is_at_P1`: se il feedback del freno non arriva, la cabina sale a 11 rpm finché la chiamata è premuta, fino all'extracorsa S2. Manca anche il controllo opposto: con `brk_fb` bloccato attivo si va in velocità piena a freno chiuso. Le altre incongruenze (OTR, bypass) hanno un trip; il freno no.

3. **Lo sblocco porta non attende l'arresto del motore** (`Src/run_mode.c:312-345`).
   Entrati nella zona di sblocco si fa `vfd_stop` e si passa a `OPEN_DOOR`, che comanda subito lo sblocco. Non si controlla `vfd.motor_speed`, mentre la decelerazione dura almeno 1 s (`VFD_ATIME_SLOW`). La gravità dipende dall'ampiezza meccanica della zona di sblocco (non verificabile qui). *Suggerito:* condizione "velocità < soglia" prima di `door_unlock()`.

4. **Se il VFD va offline durante la marcia il firmware non ferma nulla** (`Src/application.c:484`, `Inc/vfd_inl.h:21`, `Src/vfd.c:364`).
   `vfd_in_fault()` esclude il comm-timeout, `run_mode.enable` non include `vfd.online`, e `vfd_scan_cycle` esce subito se il VFD è offline: il comando di marcia resta attivo. La protezione delegata al VFD è il parametro di timeout comunicazione (`0x3000+46 = 200`), scritto una sola volta da `vfd_param_setup` senza leggere la risposta SDO. In `run_mode_init_seq` manca un `break` dopo `case VFD_PARAM` (`Src/run_mode.c:429-433`): si passa a NORMAL anche se il frame non è stato accodato (buffer pieno) e non si riprova mai.
   *Suggerito:* abilitazione con `vfd.online`, ritorno a NORMAL solo a scrittura riuscita, verifica dell'ack.

5. **Un controllo di cablaggio è disattivato** (`Src/application.c:329` e `339`).
   `TRIP_CHAIN_CABLE` e `TRIP_TTF_CX_CABLE` usano lo stesso `TMR_ID_ERR_SYNC`. `tmr_update` restituisce l'uscita già calcolata se il timer è già stato aggiornato nello stesso ciclo (`Src/timer.c:231`): quando il primo controllo viene eseguito, il secondo riceve il suo risultato e la sua condizione (`s36_ft != s3a`) non viene mai valutata. A causa del cortocircuito `!chain_fail && tmr_update(...)` il timer inoltre non avanza mentre c'è un guasto in catena. Serve un timer dedicato.

### 2.2 Medi

6. **Timeout porta non gestiti** (`Src/door.c:104`, `139`, `176`). Le tre `trip_assert(TRIP_DOOR, true)` sono commentate: `TRIP_DOOR` non viene mai attivato. `door_closing` non ha una vera uscita (il timer `LOCK_DELAY_CL` da 1500 ms non ha effetto ed è comunque breve rispetto ai 10 s di manovra); in sblocco e blocco, dopo il timeout si torna a `STOPPED` e il ciclo si ripete senza limite.
7. **Trip sbagliato nel timeout verso il VFD** (`Src/pmode.c:108-111`). `pmode_scan_to_drive` usa `TMR_ID_PMODE_TO_CHARGER` e asserisce `TRIP_PMODE_TO_CHARGER_TOUT`; `TRIP_PMODE_TO_DRIVE_TOUT` non viene mai attivato (il display mostra "PMode 1" invece di "PMode 2").
8. **Bypass manutenzione su un solo ingresso** (`Src/application.c:411`). `DIn.bypass` spegne tutti i controlli di `check_safety` e rende veri `door_is_closed`, `door_is_locked`, `is_trap_ok`, `traptray_in_position`. Restano solo `s8`, assenza di guasti VFD e di guasti pmode. È una scelta progettuale, ma va decisa consapevolmente.
9. **Frame CAN trattato come un altro** (`Src/application.c:251-253`). `UDEC_ID_CAN_STATUS` viene passato a `udec_c_process_status_word` invece che a `udec_c_process_can_stats`: i dati sono interpretati come status word e come latch delle uscite, con ritrasmissioni inutili.
10. **Serratura fuori zona di sblocco** (`Src/door.c:237-241`). Fuori zona si chiama `door_reset()`, ma se la porta non è chiusa e bloccata si va in `DOOR_STOPPED`, che chiama `door_unlock()`: l'elettroserratura resta alimentata anche in vano. Accettabile solo con un interblocco meccanico di zona.

### 2.3 Minori

- `menu_index > HMI_STATE_MAX` permette il valore 2 (fuori scala); il testo mostra 3 voci con `HMI_STATE_MAX = 2` (`Src/application.c:70`, `202`).
- Il ritardo anti-rimbalzo dei tasti (`press_delay_tmr`) conta cicli di loop, non millisecondi.
- Il tempo di uscita dal collaudo è 20 s (`TIMEOUT_MACH_PARK_TIME`), mentre il commento in `run_mode_free_fall` dice due minuti.
- Commenti errati in `read_inputs`: S2 e S3 sono descritti come "allentamento funi" e "inizio catena cabina", mentre i nomi dei trip sono corretti (vedi sezione 3).
- La partenza non controlla il sovraccarico (solo in `OPEN_DOOR` e `CLOSE_DOOR`).
- `DCLINK_OVERVOLTAGE 63.0f /* XXX */` è un valore provvisorio.
- I frame estesi sconosciuti vengono ritrasmessi su CAN125 qualunque sia il bus di arrivo (`Src/application.c:270`): rischio di eco.
- Nei frame CAN si usano `data[0..7]` senza controllare il DLC (`udec_c.c`, `udec_cx.c`); non verificato cosa contenga il buffer.
- Su TTF-CX il filtro ingressi è simmetrico (20 cicli in entrambe le direzioni), su TTF-Base la caduta è immediata: la perdita di S3/S4 dalla CX è ritardata.

### 2.4 Aspetti positivi

- Stato iniziale sicuro di `pmode` (carica, relè aperti); `pmode_relay_set` forza lo spegnimento con un trip di sicurezza attivo.
- Trip di sicurezza memorizzati con storico, azzerabili solo con il reset.
- Filtro ingressi su TTF-Base: apertura rilevata subito, chiusura confermata dopo 5 cicli.
- TTF-CX disattiva bypass e serratura se manca il battito della scheda base.
- Watchdog IWDG attivo su entrambe le schede.
- Controlli di congruenza sul relè OTR e sui bypass.

---

## 3. Analisi della catena delle sicurezze

Fonti: `Schema Macchina/fabs/TTF.pdf` (fogli 1–3), pagine `SafetyIn` e `Output` di TTF-Base, `SafetyIn` di TTF-CX, confrontate con il firmware della sezione 2.

### 3.1 Topologia

Catena in serie a 24 V, protetta da F1 (1 A) a monte del nodo S1:

```
+24V ─F1─ S1 ─[overtravel S8, X23]─ S2 ─[cinghie cabina S3,S4 + contrappesi S5,S6]─ S3
   └─► cabina (UDEC.C): PAR (paracadute S15,S16) ─ PEC (stop cabina S17) ─ PET (accostamento)
        ─ COL (blocco porta) ─ TC ─ FT      [TC e FT ponticellati: P64/P65/P66]
   └─► S3A ─[X15 UDEC.CX: ponte]─ S3B ─[trap/tetto S11,S12 ‖ K1C]─ S3C ─[tray/fondo S13,S14 ‖ K2C]─ S4
        ─ ritorno su TTF X20/S4 ─[E-stop quadro S9, X15]─ S5 ─► bobine K3 (OTR) e K4 (freno)
```

- **Il taglio avviene in hardware.** Le bobine di K3 e K4 sono alimentate dal nodo S5 e chiuse a massa dagli NUD3160D comandati dal micro: AND tra catena e firmware. Il firmware può solo togliere il consenso, non richiudere una catena aperta.
- **Cosa tagliano K3 e K4.** K3 stacca entrambi i poli dell'alimentazione del VFD, K4 entrambi i poli del freno. Un contatto aperto toglie coppia e chiude il freno insieme, senza passare dal firmware.
- **Diagnostica.** Ogni nodo (S1…S5 sulla TTF-Base, S3/S3A/S3C/S4 sulla CX, S3.1…S3.6 sulla UDEC.C) è letto con un optoisolatore TLP293, resistenza 3k3, varistore e diodo di protezione.
- **Spina di emergenza X8.** Il pin 1 è il 24 V (nodo S1) e il pin 3 va al nodo prima dell'e-stop quadro. Nella spina si chiudono anche i pin 2–4 verso `BYP-S14`, che il micro legge per sapere se la spina è inserita.

### 3.2 Mappa nodi, ingressi e trip

| Nodo | Cosa protegge | Ingresso firmware | Trip |
|---|---|---|---|
| S1 | fusibile, 24 V | `DIn.s1` | `TRIP_S1_FUSE` |
| S2 | extracorsa superiore | `DIn.s2` | `TRIP_S2_OVERTRAVEL` |
| S3 | allentamento cinghie | `DIn.s3` | `TRIP_S3_BELT` |
| S3.1 PAR | paracadute | `udec_c.in.s31_par` | `TRIP_S31_OVERSPEED` |
| S3.2 PEC | stop cabina | `s32_pec` | `TRIP_S32_CAR_ESTOP` (non bloccante) |
| S3.3 PET / S3.4 COL | accostamento, blocco porta | `s33`, `s34` | nessuno: servono a `door_is_closed` e `door_is_locked` |
| S3.5 TC / S3.6 FT | ponticellati | `s36` | solo confronto con S3A |
| S3A / S3C / S4 | cablaggio di tetto, trap, tray | CX | `TRIP_TTF_CX_CABLE`, `TRIP_CHAIN_CABLE`, `is_trap_ok`, `is_tray_ok` |
| S5 (pin firmware `s8`) | e-stop quadro, bobine | `DIn.s8` | nessuno: toglie `run_mode.enable` |

Tutti i trip legati a `TRIP_SAFETY` sono memorizzati fino al riavvio; con `TRIP_SAFETY` attivo `pmode_relay_set` toglie anche il comando ai relè dal lato firmware.

### 3.3 Osservazioni

**Hardware / architettura**

1. **La spina di emergenza esclude quasi tutta la catena**: extracorsa, allentamenti, paracadute, stop cabina, porte, trap e tray. Resta solo S9. Il firmware disattiva tutti i controlli, considera la porta chiusa e bloccata, e `is_trap_ok`/`is_tray_ok` diventano veri; la velocità è limitata a `VFD_SPEED_SLOW` (`RPM_SPEED_5` = 227 rpm, che corrisponde a 0,05 m/s secondo il nome della costante e il rapporto con `RPM_SPEED_10` = 455 rpm, 0,10 m/s) e il movimento è a uomo presente (rilasciando il tasto la cabina si ferma). Conviene tenere almeno S31 e le porte come indicazioni visibili in bypass. Il rilevamento della spina è un solo ingresso, senza ridondanza.
2. **I bypass trap/tray dipendono solo dal software.** K1/K2 sulla CX si attivano via CAN senza interblocco hardware. La protezione è il feedback con TON da 2 s (`TIMEOUT_MIRROR_SWITCH`): un contatto incollato lascia scoperto il rilevamento ostacoli fino a 2 s. Nel codice letto `TRAPTRAY_TRAP_BYPASS` non viene mai richiesto.
3. **Relè non dichiarati a contatti guidati.** K3 (AHES4192), K4 (G2RL-2) e K1/K2 (Finder 50.12) risultano dagli schemi come relè ordinari. Se il feedback usa un contatto ausiliario non guidato rispetto a quello di potenza, il controllo di saldatura non è garantito. Da verificare sui datasheet.
4. **`brk_fb` misura la tensione dopo K4**, non lo stato meccanico del freno: aggrava il punto 2 della sezione 2.
5. **Gate dei MOSFET.** Dopo un reset da watchdog i GPIO restano in ingresso; nello schema non sono visibili pull-down esplicite su `OTR-EN` e `BRK-EN`. Da verificare se il driver ne ha una interna.
6. **Tratto S3B non monitorato.** Il ponte su X15 della UDEC.CX non è letto dalla TTF ("S3B non presente su TTF"). Un'interruzione appare come S3C basso, cioè come trap premuto: in direzione sicura ma non distinguibile.
7. **Il contattore d'uscita del motore non è nella catena.** Il motore è staccato da K3 (alimentazione VFD) e dall'ingresso STO, che dal disegno risulta sul nodo S4. Se serve un secondo mezzo indipendente, è una valutazione normativa da fare con l'organismo di certificazione.

**Firmware vs hardware**

8. **I nomi dei trip sono corretti** (S1 = nodo dopo il fusibile, S2 = dopo l'overtravel, S3 = dopo le cinghie); sono sbagliati i commenti in `read_inputs`.
9. **Nomi incoerenti negli schemi.** Il pin del micro `S8` (`PD7`) legge la rete S5; la legenda cita S5 e S8 senza spiegare la differenza. Nello schema macchina i relè sono K3A/K5A con Q15B/Q16B, sulla scheda K3/K4 con Q16A/Q16B.
10. **`TRIP_TTF_CX_CABLE` inattivo** per il timer condiviso (sezione 2, punto 5); `TRIP_CHAIN_CABLE` gira solo con UDEC.C online.
11. **Prima dei 5 s iniziali la catena è protetta solo dall'hardware.** Il comando di movimento richiede comunque `DIn.s8`, cioè catena chiusa.

### 3.4 Da verificare

- Se i ponticelli su `TC/FT` (P64/P65/P66) sono intenzionali.
- Quale "inverter" è collegato al connettore X15 della UDEC.CX (il connettore ha anche i pin `STO`, `S3A`, `S3B`).
- Datasheet dei relè (punti 3 e 5 della sezione 3.3).
- Layout PCB: instradamento della catena, distanze di isolamento, protezioni dei nodi.
