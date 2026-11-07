# Registro delle issue — Elevance / TTF

**Questo è il documento base del lavoro.** Raccoglie tutte le osservazioni, le proposte di correzione e le verifiche aperte sul progetto (firmware TTF-Base e TTF-CX, hardware, documentazione, normativa, prove) e ne segue lo stato fino alla chiusura.

- Posizione: `docs/registro_issue.md` (questo file).
- Analisi di partenza (narrativa, non aggiornata): [`analisi.md`](analisi.md).
- Diagrammi: [`schema_funzionale.svg`](schema_funzionale.svg) (elettrico, catena delle sicurezze) e [`diagramma_funzionale.svg`](diagramma_funzionale.svg) (funzionale della macchina, con i riquadri rossi che rimandano agli ID di questo registro).
- Stato di partenza: commit di import `180b04b`; nessuna correzione al firmware applicata alla data di creazione (2026-11-07).

## Come si usa e come si aggiorna

1. **Ogni nuova osservazione o proposta** va aggiunta qui con un ID nuovo (prefisso per area, numero progressivo, mai riutilizzato), sia nella tabella riepilogativa sia nel blocco di dettaglio.
2. **Quando un'issue cambia stato** si aggiorna la riga della tabella, il campo **Stato** del blocco e la riga **Storia** (data, commit o prova che la chiude). Una issue risolta resta nel registro.
3. **Diagrammi**: i riquadri rossi dei diagrammi rimandano agli ID. Quando un'issue è *Risolta* o *Chiarita*, togliere il riquadro dagli script `gen_diagramma_funzionale.py` / `gen_schema_funzionale.py`, rigenerare SVG e PNG, e aggiornare la nota nel diagramma se il comportamento è cambiato.
4. **Controllo di coerenza**: `python3 docs/check_registro.py` verifica che tabella e dettagli coincidano, che gli stati siano validi e che i diagrammi non citino issue chiuse o inesistenti. Va eseguito prima di ogni commit che tocca questo file o i diagrammi.
5. Le modifiche al firmware si applicano **solo su richiesta esplicita** e vengono registrate qui prima di essere fatte (proposta → approvazione → modifica → verifica).

**Prefissi**: `FW` firmware · `HW` hardware e architettura · `NOR` normativa e analisi dei rischi · `DOC` documentazione · `PRV` verifica in prova · `CHI` chiarimenti e correzioni al lavoro già fatto.

**Gravità**: *Alta* (sicurezza funzionale o requisito dei manuali non rispettato) · *Media* · *Bassa*.

**Stato**: *Aperta* (confermata, da correggere) · *Da decidere* (serve una scelta di progetto) · *Da verificare* (serve una prova, un datasheet o un documento che manca) · *In corso* · *Risolta* · *Chiarita* (non è un difetto).

## Riepilogo

<!-- RIEPILOGO-INIZIO -->
| ID | Titolo | Gravità | Stato | Area |
|---|---|---|---|---|
| FW-01 | Collaudo paracadute avviabile da qualsiasi voce di menu | Alta | Aperta | Firmware |
| FW-02 | Freno non verificato: nessun timeout né plausibilità su `brk_fb`; marcia SU illimitata in discesa | Alta | Aperta | Firmware |
| FW-03 | Sblocco porta senza verifica di arresto del motore | Alta | Aperta | Firmware |
| FW-04 | VFD offline in marcia non ferma la macchina; parametro di timeout senza ack | Alta | Aperta | Firmware |
| FW-05 | Timer `ERR_SYNC` condiviso: controllo cablaggio di tetto inattivo | Alta | Aperta | Firmware |
| FW-06 | Tetto sensibile: manca il bypass trap in discesa | Alta | Aperta | Firmware |
| FW-07 | Timeout del ciclo porta non gestiti; `TRIP_DOOR` mai attivato | Media | Aperta | Firmware |
| FW-08 | Timeout verso la marcia usa timer e trip sbagliati | Media | Aperta | Firmware |
| FW-09 | Spina X8: i comandi di cabina e radiocomando restano attivi | Media | Da decidere | Firmware |
| FW-10 | Frame `CAN_STATUS` di UDEC.C trattato come status word | Media | Aperta | Firmware |
| FW-11 | Serratura alimentata fuori dalla zona di sblocco | Media | Da verificare | Firmware |
| FW-12 | Modalità manutenzione dal quadro assente; manca il cicalino «fuori piano» | Media | Aperta | Firmware |
| FW-13 | Ripristino dopo corto sulla catena: automatico (manuale) o memorizzato (firmware) | Media | Da decidere | Firmware |
| FW-14 | Contattori eccitati fino a 30 s dopo lo stop | Media | Aperta | Firmware |
| FW-15 | Soglia di sovratensione provvisoria e vicina alla tensione di boost | Media | Aperta | Firmware |
| FW-16 | Rilevazione batteria scarica senza filtro e con scrittura ripetuta sul LTC2959 | Bassa | Da verificare | Firmware |
| FW-17 | Timeout di carica (10 h) trattato come carica completa | Bassa | Da decidere | Firmware |
| FW-18 | Menu del quadro: indice fuori scala, voci e temporizzazione tasti | Bassa | Aperta | Firmware |
| FW-19 | Uscita dal collaudo paracadute: 20 s contro i «due minuti» del commento | Bassa | Da decidere | Firmware |
| FW-20 | Tempi e rampe fissi nel codice, i manuali li descrivono come configurabili | Bassa | Aperta | Firmware |
| FW-21 | Commenti errati o obsoleti nel codice | Bassa | Aperta | Firmware |
| FW-22 | Sovraccarico non controllato alla partenza da fermo | Bassa | Da decidere | Firmware |
| FW-23 | Frame CAN estesi ritrasmessi in eco; DLC non controllato | Bassa | Aperta | Firmware |
| FW-24 | Filtro ingressi diverso fra TTF-Base e TTF-CX | Bassa | Da decidere | Firmware |
| FW-25 | Debito tecnico segnato con `XXX`/`TODO` | Bassa | Aperta | Firmware |
| HW-01 | Spina X8: esclude quasi tutta la catena con un solo ingresso di rilevamento | Media | Da decidere | Hardware |
| HW-02 | Bypass trap/tray comandati solo via software | Media | Da decidere | Hardware |
| HW-03 | Relè K1…K4 non dichiarati a contatti guidati sugli schemi | Media | Da verificare | Hardware |
| HW-04 | `brk_fb` misura la tensione dopo K4, non lo stato meccanico del freno | Media | Da decidere | Hardware |
| HW-05 | Gate dei MOSFET OTR-EN/BRK-EN: pull-down al reset non visibile | Media | Da verificare | Hardware |
| HW-06 | Nodo S3B non letto dalla TTF (ponte X15) | Bassa | Da decidere | Hardware |
| HW-07 | Contattore di uscita motore S22 fuori dalla catena | Media | Da decidere | Hardware |
| HW-08 | Nomenclatura incoerente fra schemi e manuali (K3A/K5A, F1/F7, Q15B/Q16A) | Bassa | Aperta | Hardware |
| HW-09 | Punti da confermare sugli schemi: ponticelli TC/FT, STO sul nodo S4, percorso del pesacarico | Bassa | Da verificare | Hardware |
| NOR-01 | Analisi dei rischi Rev.5 non letta | Alta | Aperta | Normativa |
| NOR-02 | EN 81-41 5.5.7.3: alimentazione di motore e freno da interrompere alla fine del comando | Media | Da verificare | Normativa |
| NOR-03 | Freno sul lato motore con trasmissione a cinghie e monitoraggio della cinghia | Media | Da verificare | Normativa |
| NOR-04 | Giustificazione nell'analisi dei rischi del contatto paracadute e del suo bypass | Bassa | Da verificare | Normativa |
| DOC-01 | Precisione di fermata: ±20 mm (IM.159) contro ±10 mm (IM.160) | Bassa | Aperta | Documentazione |
| DOC-02 | IM.160 e IM.159: prove duplicate e refusi su fondo e tetto sensibili | Bassa | Aperta | Documentazione |
| DOC-03 | IM.159 §9: porte «chiuse e sbloccate» prima della messa fuori servizio | Bassa | Aperta | Documentazione |
| DOC-04 | IM.161: discesa di emergenza incompleta e tetto sensibile non descritto | Media | Aperta | Documentazione |
| DOC-05 | Note di progetto: numerazione S2/S3 e fusibile F1/F7 | Bassa | Aperta | Documentazione |
| DOC-06 | IM.158: parti ancora da completare (UDEC.C/CX, codici di errore, menu) | Media | Aperta | Documentazione |
| PRV-01 | Precisione di fermata ±10 mm | Media | Da verificare | Prove |
| PRV-02 | Velocità ≤ 0,15 m/s misurata e conversione giri → m/s | Bassa | Da verificare | Prove |
| PRV-03 | Sblocco della serratura con cabina appena in oltrecorsa | Media | Da verificare | Prove |
| PRV-04 | Fermata immediata al rilascio del tasto | Bassa | Da verificare | Prove |
| PRV-05 | Prove fondo e tetto sensibili (IM.160 2.03.20 e 2.03.21) | Alta | Da verificare | Prove |
| PRV-06 | Prova della manutenzione dal quadro (IM.160 2.03.01) | Media | Da verificare | Prove |
| PRV-07 | Ripristino dopo corto sulla catena (IM.160 2.03.15) | Media | Da verificare | Prove |
| CHI-01 | Velocità in bypass scritta come 5 rpm nell'analisi | Media | Risolta | Chiarimenti |
| CHI-02 | Nomi dei trip S2/S3 contro commenti di `read_inputs` | Bassa | Chiarita | Chiarimenti |
| CHI-03 | Rete S5 dello schema e ingresso «S8» del firmware | Bassa | Chiarita | Chiarimenti |
| CHI-04 | La spina X8 esclude il paracadute: è voluto | Bassa | Chiarita | Chiarimenti |
| CHI-05 | Assegnazione dei bus CAN: CAN250 cappello, CAN125 cabina | Bassa | Chiarita | Chiarimenti |
| CHI-06 | Ponte X15 / nodo S3B: contatto del VFD non presente | Bassa | Chiarita | Chiarimenti |
| CHI-07 | Errore nel confronto con i manuali: fondo e tetto sensibili | Media | Risolta | Chiarimenti |
<!-- RIEPILOGO-FINE -->

## Ordine di lavoro consigliato

1. **NOR-01**: leggere l'Analisi dei rischi Rev.5 (file su `\\SRV01\db_disegni\F510 - ELEVANCE\FASCICOLO TECNICO\Cap.2 - Analisi dei rischi`) e riportare qui i guasti analizzati: orienta le priorità delle altre issue.
2. **Sicurezza funzionale**: FW-01, FW-02, FW-03, FW-04, poi FW-06 (requisito dei manuali) e FW-05.
3. **Decisioni di progetto** da prendere prima di toccare il codice: FW-09, FW-13, FW-14, HW-01, HW-04, HW-07.
4. **Verifiche** che non richiedono codice: HW-03, HW-05, HW-09, NOR-02, NOR-03.
5. **Documentazione**: DOC-04 e DOC-06 prima del rilascio delle macchine pilota.
6. **Prove** (PRV): dopo ogni correzione che le riguarda; PRV-05 dipende da FW-06, PRV-06 da FW-12, PRV-07 da FW-13.

Percorsi dei file firmware: `TTF-Base/Firmware/TTF-Base/Core/` (per brevità `Src/…` e `Inc/…`).

---

## Firmware

### FW-01 · Collaudo paracadute avviabile da qualsiasi voce di menu
**Gravità:** Alta · **Stato:** Aperta · **Fonte:** analisi §2.1.1, confronto manuali §4.1
- **Dove:** `Src/application.c:81-86` (`hmi_process_input`), `Src/run_mode.c:17-57` (`run_mode_free_fall`).
- **Problema:** premendo OK nel menu si imposta sempre `MACHINE_STATE_FALL_TEST`; la voce scelta (`menu_index`) è ignorata e anche «NORMALE» avvia il collaudo. Si imposta da qualsiasi stato, anche con la cabina in marcia, senza protezioni. In `FALL_TEST` il freno si apre con il tasto 2 o con `IN4`. Il manuale (IM.159, prova 20) prevede il collaudo con la pedana al piano superiore e a pieno carico.
- **Proposta:** (1) agire solo se la voce scelta è COLLAUDO; (2) consentirlo solo a cabina ferma (`NORMAL`, VFD fermo) e al P1; (3) richiedere una conferma deliberata (OK prolungato ≥ 3 s o doppia conferma sul display); (4) segnalare sul display e con il cicalino; (5) sistemare gli indici del menu (FW-18).
- **Chiusura:** OK su «NORMALE» non cambia stato; il collaudo parte solo al piano alto con conferma; prova IM.159 n. 20.
- **Storia:** 2026-11-07 aperta.

### FW-02 · Freno non verificato: nessun timeout né plausibilità su `brk_fb`; marcia SU illimitata in discesa
**Gravità:** Alta · **Stato:** Aperta · **Fonte:** analisi §2.1.2; HW-04
- **Dove:** `Src/run_mode.c:120-135` (`run_mode_brake_release`), `Src/run_mode.c:165-200` (`run_mode_down`), `Inc/vfd.h`.
- **Problema:** prima di scendere si comanda una marcia SU a 11 rpm finché `brk_fb` non resta attivo 100 ms, senza timeout. In `DOWN` non si controlla `is_at_P1`: se il feedback non arriva la cabina sale finché la chiamata è premuta, fino all'extracorsa S2. Manca il controllo opposto: con `brk_fb` bloccato attivo si va a velocità piena a freno chiuso. OTR e bypass hanno un trip di incongruenza, il freno no.
- **Proposta:** (1) timeout su `brk_fb` (ad es. 1,5 s dal comando) → `vfd_halt` + nuovo trip dedicato (in coda a `trip_list.h`, prima di `TRIP_ID_SIZE`, con messaggio in `display_next_trip`); (2) limitare tempo e corsa della marcia SU in discesa; (3) plausibilità: `brk_fb` attivo con `BRK-EN` spento e macchina ferma oltre 1 s → trip; (4) aggiornare gli indicatori H30/H31 in IM.158.
- **Chiusura:** con il feedback scollegato o forzato la macchina si ferma entro il tempo previsto e il trip è visibile; prova con carico.
- **Storia:** 2026-11-07 aperta.

### FW-03 · Sblocco porta senza verifica di arresto del motore
**Gravità:** Alta · **Stato:** Aperta · **Fonte:** analisi §2.1.3; IM.160 2.03.12
- **Dove:** `Src/run_mode.c:312-345` (`run_mode_open_door`), `Src/door.c` (`door_unlock`).
- **Problema:** raggiunta la zona di sblocco si fa `vfd_stop` e si passa a `OPEN_DOOR`, che comanda subito lo sblocco; non si controlla `vfd.motor_speed` mentre la decelerazione dura almeno 1 s (`VFD_ATIME_SLOW`). La gravità dipende dall'ampiezza della zona (IM.160 2.03.12 b: al massimo 50 mm).
- **Proposta:** consentire `door_unlock()` solo con VFD non in marcia e `|motor_speed|` sotto la soglia già usata in `vfd_scan_cycle` (10), oppure dopo un tempo ≥ rampa + margine.
- **Chiusura:** misura in prova (PRV-03) e verifica che nessuna manovra di sblocco inizi con velocità non nulla.
- **Storia:** 2026-11-07 aperta.

### FW-04 · VFD offline in marcia non ferma la macchina; parametro di timeout senza ack
**Gravità:** Alta · **Stato:** Aperta · **Fonte:** analisi §2.1.4
- **Dove:** `Src/application.c:484` (`abilitation_check`), `Inc/vfd_inl.h:21` (`vfd_in_fault`), `Src/vfd.c:364` e `Src/vfd.c:125-143` (`vfd_param_setup`), `Src/run_mode.c:429-433` (`run_mode_init_seq`).
- **Problema:** `vfd_in_fault()` esclude il comm-timeout; `run_mode.enable` non include `vfd.online`; `vfd_scan_cycle` esce subito se il VFD è offline e il comando di marcia resta attivo. La protezione delegata al VFD è il parametro di timeout comunicazione (`0x3000+46 = 200`), scritto una sola volta senza leggere la risposta SDO. In `run_mode_init_seq` manca un `break` dopo `case VFD_PARAM`: si passa a NORMAL anche se il frame non è stato accodato e non si riprova mai.
- **Proposta:** (1) includere `vfd.online` nell'abilitazione (o generare un trip e un arresto) ; (2) ritornare a NORMAL solo a scrittura riuscita, con verifica della risposta SDO e nuovo tentativo; (3) aggiungere il `break` mancante; (4) verificare il valore e le unità del parametro sul VFD.
- **Chiusura:** scollegando il CAN del VFD in marcia la macchina si ferma; il parametro risulta scritto dopo ogni avvio.
- **Storia:** 2026-11-07 aperta.

### FW-05 · Timer `ERR_SYNC` condiviso: controllo cablaggio di tetto inattivo
**Gravità:** Alta · **Stato:** Aperta · **Fonte:** analisi §2.1.5
- **Dove:** `Src/application.c:329` e `Src/application.c:339`, `Src/timer.c:231`.
- **Problema:** `TRIP_CHAIN_CABLE` e `TRIP_TTF_CX_CABLE` usano lo stesso `TMR_ID_ERR_SYNC`. `tmr_update` restituisce l'uscita già calcolata se il timer è già stato aggiornato nello stesso ciclo: quando il primo controllo viene eseguito, il secondo riceve il suo risultato e la sua condizione (`s36_ft != s3a`) non viene valutata. Per il cortocircuito `!chain_fail && tmr_update(...)` il timer non avanza durante un guasto in catena.
- **Proposta:** timer dedicato per il secondo controllo (nuova voce in `timer_list.h`, configurata in `main.c`) e chiamata di `tmr_update` fuori dalle condizioni logiche.
- **Chiusura:** scollegando FT o S3A compare `TRIP_TTF_CX_CABLE` entro il tempo previsto.
- **Storia:** 2026-11-07 aperta.

### FW-06 · Tetto sensibile: manca il bypass trap in discesa
**Gravità:** Alta · **Stato:** Aperta · **Fonte:** confronto manuali §4.1; IM.158 «Funzionamento normale», IM.160 2.03.21
- **Dove:** `Src/run_mode.c` (`run_mode_down`, `run_mode_normal_car_call`), `Src/traptray.c`, `Inc/traptray_inl.h`.
- **Problema:** i manuali prevedono che con il tetto premuto la cabina si fermi e possa solo scendere. `TRAPTRAY_TRAP_BYPASS` non viene mai richiesto (`run_mode_down` usa `NO_BYPASS`), e con il tetto aperto sono falsi sia `is_trap_ok()` sia `is_tray_ok()` (S4 sta dopo il trap nella catena): non partono né la salita né la discesa. Il fondo è corretto: in salita richiede `TRAY_BYPASS`. IM.161 4.09 descrive solo il fondo (DOC-04).
- **Proposta:** in discesa, se il trap è premuto richiedere `TRAPTRAY_TRAP_BYPASS`, attendere `traptray_in_position()` e valutare il tray su S4 (con il bypass inserito riflette il solo tray); consentire la partenza in discesa con il tetto premuto in `run_mode_normal_car_call`. Da validare con l'analisi dei rischi (NOR-01) e con la BS 5900 citata nelle note di progetto.
- **Chiusura:** prove IM.160 2.03.20 e 2.03.21 (PRV-05) superate.
- **Storia:** 2026-11-07 aperta; il rilievo corregge un'affermazione errata fatta nel confronto con i manuali (CHI-07).

### FW-07 · Timeout del ciclo porta non gestiti; `TRIP_DOOR` mai attivato
**Gravità:** Media · **Stato:** Aperta · **Fonte:** analisi §2.2.6
- **Dove:** `Src/door.c:104`, `Src/door.c:139`, `Src/door.c:176`.
- **Problema:** le tre `trip_assert(TRIP_DOOR, true)` sono commentate. `door_closing` non ha una vera uscita (il timer `LOCK_DELAY_CL` da 1,5 s non ha effetto ed è breve rispetto ai 10 s di manovra); in sblocco e blocco dopo il timeout si torna a `STOPPED` e il ciclo si ripete senza limite.
- **Proposta:** dare a ogni stato un timeout con tentativi limitati e `TRIP_DOOR` (non bloccante o con messaggio) all'esaurimento; togliere l'alimentazione agli operatori dopo il timeout.
- **Chiusura:** prova con porta ostruita e con serratura bloccata: il ciclo si arresta e il trip è visibile.
- **Storia:** 2026-11-07 aperta.

### FW-08 · Timeout verso la marcia usa timer e trip sbagliati
**Gravità:** Media · **Stato:** Aperta · **Fonte:** analisi §2.2.7
- **Dove:** `Src/pmode.c:108-111` (`pmode_scan_to_drive`), `Src/main.c:190`.
- **Problema:** usa `TMR_ID_PMODE_TO_CHARGER` e asserisce `TRIP_PMODE_TO_CHARGER_TOUT`; `TRIP_PMODE_TO_DRIVE_TOUT` non viene mai attivato e il display mostra «PMode 1» invece di «PMode 2». Il timer `TMR_ID_PMODE_TO_DRIVE` è configurato ma non usato.
- **Proposta:** usare `TMR_ID_PMODE_TO_DRIVE` e `TRIP_PMODE_TO_DRIVE_TOUT`; decidere se il timeout deve anche forzare il ritorno a `TO_CHARGER`.
- **Chiusura:** simulazione di VFD che non diventa pronto: compare «PMode 2».
- **Storia:** 2026-11-07 aperta.

### FW-09 · Spina X8: i comandi di cabina e radiocomando restano attivi
**Gravità:** Media · **Stato:** Da decidere · **Fonte:** confronto manuali §4.2; HW-01
- **Dove:** `Src/application.c:498-523` (`set_prenotation`), `Inc/udec_c_inl.h`.
- **Problema:** IM.158 dice che con la spina restano attivi solo i comandi del pannello principale. `set_prenotation` non dipende dal bypass: con UDEC.C online anche i tasti di cabina e il radiocomando possono muovere la macchina.
- **Proposta:** con `run_mode.bypass_manutenzione` ignorare gli ingressi di UDEC.C e usare solo `IN1/IN2` del quadro; oppure aggiornare IM.158 se il comportamento attuale è voluto. Decidere con HW-01.
- **Chiusura:** con la spina inserita i tasti di cabina non muovono la macchina (o il manuale lo dichiara).
- **Storia:** 2026-11-07 aperta.

### FW-10 · Frame `CAN_STATUS` di UDEC.C trattato come status word
**Gravità:** Media · **Stato:** Aperta · **Fonte:** analisi §2.2.9
- **Dove:** `Src/application.c:251-253`, `Src/udec_c.c:62-66`.
- **Problema:** `UDEC_ID_CAN_STATUS` viene passato a `udec_c_process_status_word` invece che a `udec_c_process_can_stats`: i dati sono letti come status word e come latch delle uscite, con ritrasmissioni inutili.
- **Proposta:** chiamare `udec_c_process_can_stats`.
- **Chiusura:** nessuna ritrasmissione indotta da questo frame (analisi del traffico).
- **Storia:** 2026-11-07 aperta.

### FW-11 · Serratura alimentata fuori dalla zona di sblocco
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** analisi §2.2.10; IM.160 2.03.12 c
- **Dove:** `Src/door.c:237-241` e `door_stopped`.
- **Problema:** fuori zona si chiama `door_reset()` ma, con porta non chiusa e bloccata, si va in `DOOR_STOPPED` che chiama `door_unlock()`: l'elettroserratura resta alimentata anche in vano. È accettabile solo se esiste un interblocco meccanico di zona (IM.160 dice che fuori zona l'impianto funziona solo con le porte chiuse e bloccate).
- **Proposta:** non alimentare `ELT` quando nessuna zona è attiva, salvo manovre esplicite; verificare l'interblocco meccanico.
- **Chiusura:** misura della tensione sulla serratura fuori zona; revisione del codice.
- **Storia:** 2026-11-07 aperta.

### FW-12 · Modalità manutenzione dal quadro assente; manca il cicalino «fuori piano»
**Gravità:** Media · **Stato:** Aperta · **Fonte:** confronto manuali §4.1; IM.160 2.03.01, IM.158
- **Dove:** `Src/application.c` (menu), `Inc/udec_c_inl.h` (`hl_buz_ft` mai impostato).
- **Problema:** il manuale prevede una manutenzione attivabile dal quadro, consentita solo con la pedana al piano basso, con segnalatore acustico di «pedana fuori piano». Nel firmware esiste solo la spina; la voce MANUALE del menu non fa nulla.
- **Proposta:** implementare la modalità (abilitazione solo al P0, velocità ridotta, cicalino con cabina fuori piano) dopo aver definito i requisiti nell'analisi dei rischi.
- **Chiusura:** prova IM.160 2.03.01 (PRV-06).
- **Storia:** 2026-11-07 aperta.

### FW-13 · Ripristino dopo corto sulla catena: automatico (manuale) o memorizzato (firmware)
**Gravità:** Media · **Stato:** Da decidere · **Fonte:** confronto manuali §4.1; IM.160 2.03.15, IM.158
- **Dove:** `Src/application.c:302-367` (`check_safety`), `Src/trips.c`.
- **Problema:** IM.160 dice che l'impianto si ripristina in automatico dopo l'eliminazione del cortocircuito; il firmware memorizza `TRIP_S1_FUSE` e `TRIP_SAFETY` fino al riavvio. IM.158 lascia il punto «da decidere».
- **Proposta:** scegliere e allineare i due documenti: ripristino automatico per S1 (fusibile/corto) oppure memorizzazione con reset, e aggiornare IM.160 2.03.15 di conseguenza.
- **Chiusura:** decisione registrata qui; prova PRV-07.
- **Storia:** 2026-11-07 aperta.

### FW-14 · Contattori eccitati fino a 30 s dopo lo stop
**Gravità:** Media · **Stato:** Aperta · **Fonte:** confronto manuali §4.4; NOR-02
- **Dove:** `Inc/pmode.h` (`TIMEOUT_PMODE_WAIT_TO_CHARGER = 30000`), `Src/pmode.c:141-154`.
- **Problema:** dopo lo stop `pmode` passa a `ST_WAIT_TO_CHARGER` e tiene K3/K4 eccitati per 30 s, mentre il commento dice «una decina di secondi». La pagina Notion su UDEC.M descrive lo stesso rischio (movimento non comandato con contattori eccitati) e propone di limitarlo a pochi secondi.
- **Proposta:** ridurre l'attesa alla sola decelerazione più un margine, oppure terminarla al primo istante con VFD fermo; correggere il commento; valutare il costo (ricarica del DC link a ogni corsa).
- **Chiusura:** decisione in NOR-02; prova di stop e ripartenza.
- **Storia:** 2026-11-07 aperta.

### FW-15 · Soglia di sovratensione provvisoria e vicina alla tensione di boost
**Gravità:** Media · **Stato:** Aperta · **Fonte:** note iniziali di analisi
- **Dove:** `Inc/battery.h:59` (`DCLINK_OVERVOLTAGE 63.0f /* XXX */`), `Src/battery.c:329-335`.
- **Problema:** 63 V contro 58,0 V di boost; il commento stesso la dice un compromesso. L'evento è memorizzato fino al riavvio e apre S22 con reboot del VFD.
- **Proposta:** misurare i picchi di rigenerazione e fissare la soglia con margine rispetto al boost e ai limiti del VFD, con una breve conferma temporale per evitare falsi trip; documentarla.
- **Chiusura:** soglia misurata e motivata; prova di frenata rigenerativa a pieno carico.
- **Storia:** 2026-11-07 aperta.

### FW-16 · Rilevazione batteria scarica senza filtro e con scrittura ripetuta sul LTC2959
**Gravità:** Bassa · **Stato:** Da verificare · **Fonte:** lettura di `battery.c` (confronto manuali)
- **Dove:** `Src/battery.c:248-263`.
- **Problema:** fuori da `ST_CHARGER`, ogni scansione con `dclink ≤ 38,4 V` imposta `low_battery` e azzera il contatore di carica a −7,5 Ah scrivendo sul LTC2959 via I²C: un affondamento transitorio sotto carico può falsare lo stato di carica; la scrittura si ripete a ogni giro.
- **Proposta:** filtrare con un tempo di conferma e scrivere il registro solo al passaggio di stato; verificare in prova se gli affondamenti sotto carico raggiungono la soglia.
- **Chiusura:** misura della tensione sotto carico; revisione.
- **Storia:** 2026-11-07 aperta.

### FW-17 · Timeout di carica (10 h) trattato come carica completa
**Gravità:** Bassa · **Stato:** Da decidere · **Fonte:** commento `XXX` in `battery.c`
- **Dove:** `Src/battery.c:68-72`.
- **Problema:** allo scadere dei 36 000 s si fa finta che la batteria sia completa («cosa fare? XXX»).
- **Proposta:** decidere se segnalare un guasto del caricabatteria o della batteria invece di considerarla carica.
- **Chiusura:** decisione e codice.
- **Storia:** 2026-11-07 aperta.

### FW-18 · Menu del quadro: indice fuori scala, voci e temporizzazione tasti
**Gravità:** Bassa · **Stato:** Aperta · **Fonte:** analisi §2.3
- **Dove:** `Src/application.c:70`, `Src/application.c:202`, `Src/application.c:37-93`.
- **Problema:** `menu_index > HMI_STATE_MAX` ammette il valore 2 (fuori scala); il testo ha 3 voci con `HMI_STATE_MAX = 2`; il ritardo dei tasti conta cicli di loop e non millisecondi.
- **Proposta:** correggere i limiti e usare un timer temporale (parte di FW-01).
- **Chiusura:** prova al quadro con tutte le voci.
- **Storia:** 2026-11-07 aperta.

### FW-19 · Uscita dal collaudo paracadute: 20 s contro i «due minuti» del commento
**Gravità:** Bassa · **Stato:** Da decidere · **Fonte:** analisi §2.3
- **Dove:** `Src/run_mode.c:49-57`, `Inc/application.h` (`TIMEOUT_MACH_PARK_TIME`).
- **Problema:** il commento dice due minuti, il codice usa il tempo di parcheggio (20 s).
- **Proposta:** definire il tempo con la procedura IM.159 n. 20 e dare al collaudo una costante propria.
- **Chiusura:** decisione e allineamento di codice e commento.
- **Storia:** 2026-11-07 aperta.

### FW-20 · Tempi e rampe fissi nel codice, i manuali li descrivono come configurabili
**Gravità:** Bassa · **Stato:** Aperta · **Fonte:** confronto manuali §4.5
- **Dove:** `Inc/application.h` (`TIMEOUT_*`, `VFD_ATIME_*`).
- **Problema:** IM.161 parla di «tempo configurato» per la richiusura della porta, IM.159 di accelerazioni «impostate originariamente».
- **Proposta:** raccogliere i parametri in una struttura salvata in flash (esiste `flash.c`) con menu di servizio, oppure correggere i manuali.
- **Chiusura:** parametri configurabili o testi corretti.
- **Storia:** 2026-11-07 aperta.

### FW-21 · Commenti errati o obsoleti nel codice
**Gravità:** Bassa · **Stato:** Aperta · **Fonte:** analisi §2.3; CHI-02
- **Dove:** `Src/application.c:373-384` (S2 «allentamento funi», S3 «inizio catena cabina»), `Src/run_mode.c:202-205` (STUCK_UP «non è possibile muoversi in nessuna direzione»), `Inc/pmode.h` («decina di secondi»), `Src/run_mode.c:283` (commento di `run_mode_wait_for_call_release`).
- **Problema:** i commenti non coincidono con il comportamento o con gli schemi (S2 è il nodo dopo l'extracorsa, S3 dopo le cinghie).
- **Proposta:** correggere i commenti dopo le decisioni su FW-06 e FW-14.
- **Chiusura:** revisione.
- **Storia:** 2026-11-07 aperta.

### FW-22 · Sovraccarico non controllato alla partenza da fermo
**Gravità:** Bassa · **Stato:** Da decidere · **Fonte:** analisi §2.3; IM.160 2.03.07
- **Dove:** `Src/run_mode.c:297-309`, `run_mode_normal`.
- **Problema:** il controllo è fatto solo in `OPEN_DOOR` e `CLOSE_DOOR`. Nella prova IM.160 2.03.07 funziona (si carica a porta aperta), ma con porta chiusa e carico aggiunto non viene rivalutato prima di partire.
- **Proposta:** valutare un controllo in partenza; decidere con l'analisi dei rischi.
- **Chiusura:** decisione.
- **Storia:** 2026-11-07 aperta.

### FW-23 · Frame CAN estesi ritrasmessi in eco; DLC non controllato
**Gravità:** Bassa · **Stato:** Aperta · **Fonte:** analisi §2.3
- **Dove:** `Src/application.c:270`, `Src/udec_c.c`, `Src/udec_cx.c`.
- **Problema:** i frame estesi sconosciuti vengono ritrasmessi su CAN125 qualunque sia il bus di arrivo; i dati dei frame sono usati senza verificare il DLC (non verificato che cosa contenga il buffer).
- **Proposta:** limitare il gateway ai soli bus e ID previsti; controllare il DLC prima di leggere.
- **Chiusura:** revisione e prova con traffico forzato.
- **Storia:** 2026-11-07 aperta.

### FW-24 · Filtro ingressi diverso fra TTF-Base e TTF-CX
**Gravità:** Bassa · **Stato:** Da decidere · **Fonte:** analisi §2.3
- **Dove:** `TTF-CX/Firmware/TTF-CX/Core/Src/application.c` (`DInDebounce = 20`), `Src/application.c:395-409` (TTF-Base).
- **Problema:** su TTF-Base l'apertura è rilevata subito e la chiusura dopo 5 cicli; su TTF-CX il filtro è simmetrico (20 cicli): la perdita di S3/S4 dalla CX è ritardata.
- **Proposta:** adottare il filtro asimmetrico anche sulla CX per gli ingressi di sicurezza.
- **Chiusura:** decisione e prova del tempo di risposta.
- **Storia:** 2026-11-07 aperta.

### FW-25 · Debito tecnico segnato con `XXX`/`TODO`
**Gravità:** Bassa · **Stato:** Aperta · **Fonte:** note iniziali di analisi
- **Dove:** `Src/run_mode.c:419` (`L2D_PARAM` saltato), `Src/i2c_bb.c:18` e `:159`, `Src/ltc2959.c:24`, `Src/lcd_hd44780_i2c.c:405` (CPU bloccata), `Inc/can_inl.h:24` (traffico ripetuto su CAN125), `Src/vfd.c:455` (più comandi in sospeso), `Src/l2d.c:163`, `Src/application.c:551` (lettura CAN da portare in IRQ).
- **Problema:** punti lasciati aperti dallo sviluppo; il debug sul secondo bus e `L2D_PARAM` saltato non devono arrivare in produzione.
- **Proposta:** elencare e chiudere uno per uno (sotto-issue se necessario).
- **Chiusura:** nessun `XXX`/`TODO` residuo o motivato.
- **Storia:** 2026-11-07 aperta.

---

## Hardware e architettura

### HW-01 · Spina X8: esclude quasi tutta la catena con un solo ingresso di rilevamento
**Gravità:** Media · **Stato:** Da decidere · **Fonte:** analisi §3.3.1; CHI-04
- **Dove:** spina X8 (pin 1–3 ponte, pin 2–4 verso `BYP-S14`, `PD8`); `Src/application.c:411`.
- **Problema:** il ponte esclude extracorsa, allentamenti, paracadute, stop cabina, porte, trap e tray (resta S9). L'esclusione è voluta (IM.159 prova 20 D la usa per disinserire il paracadute). Resta che il rilevamento è un solo ingresso senza ridondanza e che in bypass il firmware disattiva tutti i controlli e considera porta chiusa e bloccata, trap e tray veri.
- **Proposta:** mantenere come indicazioni non bloccanti almeno S31 e le porte; valutare una lettura ridondante della spina; decidere con FW-09.
- **Chiusura:** decisione e, se serve, modifica hardware/firmware.
- **Storia:** 2026-11-07 aperta; parte chiarita come CHI-04.

### HW-02 · Bypass trap/tray comandati solo via software
**Gravità:** Media · **Stato:** Da decidere · **Fonte:** analisi §3.3.2
- **Dove:** schede TTF-CX (K1/K2, `Q2A/Q2B`), `Src/application.c:339-351`.
- **Problema:** i relè K1/K2 si attivano via CAN senza interblocco hardware; la protezione è il feedback con TON da 2 s: un contatto incollato lascia scoperto il rilevamento ostacoli fino a 2 s. La nota di progetto riconosce che il circuito non è «completamente di sicurezza».
- **Proposta:** valutare un interblocco hardware (ad es. bypass abilitato solo con direzione compatibile) e ridurre il tempo di rilevamento; decidere con l'analisi dei rischi.
- **Chiusura:** decisione e revisione dello schema.
- **Storia:** 2026-11-07 aperta.

### HW-03 · Relè K1…K4 non dichiarati a contatti guidati sugli schemi
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** analisi §3.3.3
- **Dove:** K3 (AHES4192), K4 (G2RL-2), K1/K2 (Finder 50.12.9.024.x000).
- **Problema:** gli schemi non indicano contatti guidati forzatamente; la scheda Notion «Bypass trap/tray» dichiara «relè monitorati a contatti guidati». Il controllo di saldatura vale solo se il contatto di feedback è guidato rispetto a quello di potenza.
- **Proposta:** controllare i datasheet dei codici montati e documentarlo in IM.158.
- **Chiusura:** datasheet allegati o relè sostituiti.
- **Storia:** 2026-11-07 aperta.

### HW-04 · `brk_fb` misura la tensione dopo K4, non lo stato meccanico del freno
**Gravità:** Media · **Stato:** Da decidere · **Fonte:** analisi §3.3.4; FW-02
- **Dove:** pagina `Output` di TTF-Base (`BRK-FB`, `PC6`).
- **Problema:** il feedback indica che c'è tensione sul freno, non che si è aperto. Aggrava FW-02.
- **Proposta:** valutare un sensore di posizione o di corrente del freno; in alternativa usare nel firmware un controllo temporale e di coerenza con la corrente del motore.
- **Chiusura:** decisione.
- **Storia:** 2026-11-07 aperta.

### HW-05 · Gate dei MOSFET OTR-EN/BRK-EN: pull-down al reset non visibile
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** analisi §3.3.5
- **Dove:** NUD3160D `Q16A/Q16B` (TTF-Base), `Q2A/Q2B` (TTF-CX).
- **Problema:** dopo un reset da watchdog i GPIO restano in ingresso e non si vedono pull-down esplicite sui gate; da verificare se il driver ne ha una interna.
- **Proposta:** leggere il datasheet e, se serve, aggiungere resistenze di pull-down.
- **Chiusura:** verifica sul componente o modifica.
- **Storia:** 2026-11-07 aperta.

### HW-06 · Nodo S3B non letto dalla TTF (ponte X15)
**Gravità:** Bassa · **Stato:** Da decidere · **Fonte:** analisi §3.3.6; CHI-06
- **Dove:** UDEC.CX X15, nota «S3B non presente su TTF».
- **Problema:** il ponte sostituisce il contatto del VFD (assente); un'interruzione si vede come S3C basso (trap premuto), senza distinguerla.
- **Proposta:** decidere se monitorare S3B o rimuovere il tratto dal progetto.
- **Chiusura:** decisione.
- **Storia:** 2026-11-07 aperta.

### HW-07 · Contattore di uscita motore S22 fuori dalla catena
**Gravità:** Media · **Stato:** Da decidere · **Fonte:** analisi §3.3.7; NOR-02
- **Dove:** `Src/vfd.c:166-311` (macchina a stati dell'uscita), schema `Cappello` (S22).
- **Problema:** il motore è staccato da K3 (alimentazione del VFD) e dall'ingresso STO; S22 è comandato dal firmware e «non fa parte del circuito di sicurezza». Se serve un secondo mezzo indipendente di interruzione è una valutazione normativa.
- **Proposta:** valutare con l'organismo di certificazione se S22 debba entrare nella catena.
- **Chiusura:** decisione.
- **Storia:** 2026-11-07 aperta.

### HW-08 · Nomenclatura incoerente fra schemi e manuali (K3A/K5A, F1/F7, Q15B/Q16A)
**Gravità:** Bassa · **Stato:** Aperta · **Fonte:** analisi §3.3.9; CHI-03
- **Dove:** schema macchina (K3A/K5A, Q15B/Q16B, F1), scheda (K3/K4, Q16A/Q16B), IM.158 (F7), firmware (`S8` = S5).
- **Problema:** nomi diversi per gli stessi componenti.
- **Proposta:** adottare una tabella unica di corrispondenza e aggiornare schemi e manuali.
- **Chiusura:** tabella pubblicata e documenti allineati.
- **Storia:** 2026-11-07 aperta.

### HW-09 · Punti da confermare sugli schemi: ponticelli TC/FT, STO sul nodo S4, percorso del pesacarico
**Gravità:** Bassa · **Stato:** Da verificare · **Fonte:** analisi §3.4
- **Dove:** connettori P64/P65/P66, UDEC.CX X15, sezione `LoadCell`.
- **Problema:** se i ponticelli TC/FT sono intenzionali, se lo STO del VFD è collegato davvero al nodo S4 e come il segnale delle celle di carico arrivi all'ingresso `ftc3`.
- **Proposta:** confermare sugli schemi di costruzione o in prova.
- **Chiusura:** conferma registrata.
- **Storia:** 2026-11-07 aperta.

---

## Normativa e analisi dei rischi

### NOR-01 · Analisi dei rischi Rev.5 non letta
**Gravità:** Alta · **Stato:** Aperta · **Fonte:** conversazione del 2026-11-07
- **Dove:** `\\SRV01\db_disegni\F510 - ELEVANCE\FASCICOLO TECNICO\Cap.2 - Analisi dei rischi` (la pagina Notion contiene solo il percorso).
- **Problema:** non so quali guasti l'analisi consideri; molte priorità del registro dipendono da essa.
- **Proposta:** caricare il file nella sessione (o in una cartella del repo) e confrontare ogni pericolo con le issue FW/HW; aggiungere qui i riferimenti.
- **Chiusura:** confronto completato e riportato.
- **Storia:** 2026-11-07 aperta.

### NOR-02 · EN 81-41 5.5.7.3: alimentazione di motore e freno da interrompere alla fine del comando
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** pagina Notion «Controllo dei contattori di OTR/FRS su UDEC.M»
- **Dove:** `Src/pmode.c` (`ST_WAIT_TO_CHARGER`), HW-07.
- **Problema:** il requisito è citato per UDEC.M; in TTF-Base K3/K4 restano eccitati fino a 30 s (FW-14) e S22 resta chiuso ≥ 10 s.
- **Proposta:** decidere come l'analisi dei rischi giustifica o corregge questi tempi.
- **Chiusura:** decisione registrata.
- **Storia:** 2026-11-07 aperta.

### NOR-03 · Freno sul lato motore con trasmissione a cinghie e monitoraggio della cinghia
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** pagina Notion «Requisiti del freno elettromeccanico (da EN 81-41)»
- **Dove:** schema `Cappello` (freno M2 vicino al motore), sensori di allentamento S3–S6.
- **Problema:** per impianti reversibili con cinghia o catena la nota ricorda che il freno deve stare sul lato del carico e che la cinghia va monitorata con sensori di sicurezza. Elevance ha il freno sul motore, il paracadute e sensori di allentamento in catena.
- **Proposta:** documentare nell'analisi dei rischi come la configurazione soddisfa o motiva la deroga.
- **Chiusura:** giustificazione nel fascicolo.
- **Storia:** 2026-11-07 aperta.

### NOR-04 · Giustificazione nell'analisi dei rischi del contatto paracadute e del suo bypass
**Gravità:** Bassa · **Stato:** Da verificare · **Fonte:** pagina Notion «Necessità di contatto di sicurezza su paracadute»
- **Dove:** catena S3.1 PAR, spina X8.
- **Problema:** la nota sostiene che il contatto di sicurezza sul paracadute non è obbligatorio se l'analisi dimostra l'assenza di rischi; in Elevance il contatto è in catena e la spina lo esclude.
- **Proposta:** riportare nell'analisi la motivazione di entrambe le scelte.
- **Chiusura:** riferimento nel fascicolo.
- **Storia:** 2026-11-07 aperta.

---

## Documentazione

### DOC-01 · Precisione di fermata: ±20 mm (IM.159) contro ±10 mm (IM.160)
**Gravità:** Bassa · **Stato:** Aperta · **Fonte:** confronto manuali §4.2
- **Dove:** IM.159 §10.06 prova 1, IM.160 2.03.04 e 2.03.06.
- **Problema:** due limiti diversi per la stessa prova.
- **Proposta:** adottare un unico valore (vedi PRV-01).
- **Chiusura:** manuali allineati.
- **Storia:** 2026-11-07 aperta.

### DOC-02 · IM.160 e IM.159: prove duplicate e refusi su fondo e tetto sensibili
**Gravità:** Bassa · **Stato:** Aperta · **Fonte:** confronto manuali §4.5
- **Dove:** IM.160 2.03.04/06 e 2.03.05/08 duplicate; 2.03.21 III e IV («verso l'alto», «fondo sensibile»); IM.159 prova 4 («fondo sensibile» per il tetto).
- **Proposta:** correggere i testi.
- **Chiusura:** manuali corretti.
- **Storia:** 2026-11-07 aperta.

### DOC-03 · IM.159 §9: porte «chiuse e sbloccate» prima della messa fuori servizio
**Gravità:** Bassa · **Stato:** Aperta · **Fonte:** confronto manuali §4.5
- **Dove:** IM.159 §9, §12.
- **Problema:** il firmware tiene le porte chiuse e bloccate; la chiave di emergenza richiede i sezionatori aperti.
- **Proposta:** chiarire il testo («chiuse e bloccate»).
- **Chiusura:** manuale corretto.
- **Storia:** 2026-11-07 aperta.

### DOC-04 · IM.161: discesa di emergenza incompleta e tetto sensibile non descritto
**Gravità:** Media · **Stato:** Aperta · **Fonte:** confronto manuali §4.5
- **Dove:** IM.161 4.10 e 4.09.
- **Problema:** 4.10 elenca inserimento della spina e uscita dei passeggeri ma non come muovere la cabina né come aprire la porta (con la spina il firmware non comanda le porte); 4.09 descrive solo il fondo sensibile.
- **Proposta:** completare i due paragrafi dopo aver deciso FW-06 e FW-09.
- **Chiusura:** manuale aggiornato.
- **Storia:** 2026-11-07 aperta.

### DOC-05 · Note di progetto: numerazione S2/S3 e fusibile F1/F7
**Gravità:** Bassa · **Stato:** Aperta · **Fonte:** confronto manuali §4.5
- **Dove:** note per il collaudo di certificazione («su allentamento cade S2»), schema macchina (F1) contro IM.158 (F7).
- **Proposta:** allineare alla numerazione dei nodi (allentamento → S3) e al fusibile (vedi HW-08).
- **Chiusura:** note corrette.
- **Storia:** 2026-11-07 aperta.

### DOC-06 · IM.158: parti ancora da completare (UDEC.C/CX, codici di errore, menu)
**Gravità:** Media · **Stato:** Aperta · **Fonte:** confronto manuali §4
- **Dove:** IM.158 (descrizione UDEC.C/CX, prove di isolamento, codici di errore, menu e parametri, LED degli operatori porta).
- **Proposta:** completare con i 25 trip di `trip_list.h`, i messaggi del display, le voci di menu e il diagramma funzionale di questo repository.
- **Chiusura:** manuale completo.
- **Storia:** 2026-11-07 aperta.

---

## Prove da eseguire

### PRV-01 · Precisione di fermata ±10 mm
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** confronto manuali §4.2; DOC-01
- **Prova:** IM.160 2.03.04/06, cabina vuota e a pieno carico. Stima del firmware: fermata al primo bordo della zona di sblocco con rampa `VFD_ATIME_SLOW`; se il valore è in decimi di secondo (`vfd.c`), a 0,05 m/s servono circa 25 mm. Da misurare.
- **Storia:** 2026-11-07 aperta.

### PRV-02 · Velocità ≤ 0,15 m/s misurata e conversione giri → m/s
**Gravità:** Bassa · **Stato:** Da verificare · **Fonte:** confronto manuali §4.2
- **Prova:** IM.160 2.03.03; confermare che `RPM_SPEED_5` = 227 rpm e `RPM_SPEED_10` = 455 rpm corrispondano a 0,05 e 0,10 m/s.
- **Storia:** 2026-11-07 aperta.

### PRV-03 · Sblocco della serratura con cabina appena in oltrecorsa
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** IM.160 2.03.12 b
- **Prova:** misura sulla macchina; collegata a FW-03 e FW-11.
- **Storia:** 2026-11-07 aperta.

### PRV-04 · Fermata immediata al rilascio del tasto
**Gravità:** Bassa · **Stato:** Da verificare · **Fonte:** IM.160 2.03.10
- **Prova:** il firmware usa `vfd_stop` (circa 1 s); il manuale dice «immediatamente»; valutare `vfd_halt` o chiarire il testo.
- **Storia:** 2026-11-07 aperta.

### PRV-05 · Prove fondo e tetto sensibili (IM.160 2.03.20 e 2.03.21)
**Gravità:** Alta · **Stato:** Da verificare · **Fonte:** FW-06
- **Prova:** dopo FW-06, eseguire entrambe le prove complete (arresto in movimento, direzione consentita, movimento opposto).
- **Storia:** 2026-11-07 aperta.

### PRV-06 · Prova della manutenzione dal quadro (IM.160 2.03.01)
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** FW-12
- **Prova:** dopo FW-12.
- **Storia:** 2026-11-07 aperta.

### PRV-07 · Ripristino dopo corto sulla catena (IM.160 2.03.15)
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** FW-13
- **Prova:** dopo la decisione su FW-13.
- **Storia:** 2026-11-07 aperta.

---

## Chiarimenti e correzioni al lavoro già fatto

### CHI-01 · Velocità in bypass scritta come 5 rpm nell'analisi
**Gravità:** Media · **Stato:** Risolta · **Fonte:** lettura di `Inc/vfd.h`
- **Problema:** `RPM_SPEED_5` vale 227 rpm (0,05 m/s), non 5 rpm.
- **Storia:** 2026-11-07 corretto `docs/analisi.md` (commit `778af34`).

### CHI-02 · Nomi dei trip S2/S3 contro commenti di `read_inputs`
**Gravità:** Bassa · **Stato:** Chiarita · **Fonte:** schema macchina, IM.158
- **Chiarimento:** i nodi sono S1 (dopo F1), S2 (dopo l'extracorsa), S3 (dopo le cinghie): i nomi dei trip sono corretti; sono sbagliati i commenti (FW-21).
- **Storia:** 2026-11-07 chiarita.

### CHI-03 · Rete S5 dello schema e ingresso «S8» del firmware
**Gravità:** Bassa · **Stato:** Chiarita · **Fonte:** IM.158 (test point P14, indicatore H9)
- **Chiarimento:** l'ingresso `S8` (`PD7`) del firmware legge la rete S5 («fine catena, OK to run»).
- **Storia:** 2026-11-07 chiarita.

### CHI-04 · La spina X8 esclude il paracadute: è voluto
**Gravità:** Bassa · **Stato:** Chiarita · **Fonte:** IM.159 prova 20 D
- **Chiarimento:** la spina serve a disinserire il paracadute dopo la prova; restano le questioni HW-01 e NOR-04.
- **Storia:** 2026-11-07 chiarita.

### CHI-05 · Assegnazione dei bus CAN: CAN250 cappello, CAN125 cabina
**Gravità:** Bassa · **Stato:** Chiarita · **Fonte:** `Inc/can.h`, `Src/vfd.c`, `Src/npb.c`, `Src/udec_*.c`, `Src/l2d.c`
- **Chiarimento:** VFD e NPB sono su CAN250; UDEC.C, UDEC.CX e operatori porta L2D su CAN125.
- **Storia:** 2026-11-07 chiarita.

### CHI-06 · Ponte X15 / nodo S3B: contatto del VFD non presente
**Gravità:** Bassa · **Stato:** Chiarita · **Fonte:** IM.158 (indicatore H6)
- **Chiarimento:** il ponte sostituisce un contatto del VFD previsto ma non montato; resta la decisione HW-06.
- **Storia:** 2026-11-07 chiarita.

### CHI-07 · Errore nel confronto con i manuali: fondo e tetto sensibili
**Gravità:** Media · **Stato:** Risolta · **Fonte:** IM.160 2.03.20/21
- **Problema:** nel confronto avevo scritto che il firmware rispetta le regole di fondo e tetto; per il tetto era sbagliato (vedi FW-06).
- **Storia:** 2026-11-07 corretto nel confronto e riportato come FW-06.

---

## Cronologia degli aggiornamenti

| Data | Modifica |
|---|---|
| 2026-11-07 | Creazione del registro: 25 issue firmware, 9 hardware, 4 normative, 6 documentazione, 7 prove, 7 chiarimenti. Aggiunti il diagramma funzionale e lo script di controllo. |
