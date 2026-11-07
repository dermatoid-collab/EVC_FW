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
| FW-03 | Sblocco porta senza verifica di arresto del motore | Media | Aperta | Firmware |
| FW-04 | VFD offline in marcia non ferma la macchina; parametro di timeout senza ack | Alta | Aperta | Firmware |
| FW-05 | Timer `ERR_SYNC` condiviso: controllo cablaggio di tetto inattivo | Alta | Aperta | Firmware |
| FW-06 | Tetto sensibile: manca il bypass trap in discesa | Alta | Aperta | Firmware |
| FW-07 | Timeout del ciclo porta non gestiti; `TRIP_DOOR` mai attivato | Media | Aperta | Firmware |
| FW-08 | Timeout verso la marcia usa timer e trip sbagliati | Media | Aperta | Firmware |
| FW-09 | Spina X8: i comandi di cabina e radiocomando restano attivi | Media | Aperta | Firmware |
| FW-10 | Frame `CAN_STATUS` di UDEC.C trattato come status word | Media | Aperta | Firmware |
| FW-11 | Serratura alimentata fuori dalla zona di sblocco | Media | Da verificare | Firmware |
| FW-12 | Modalità manutenzione dal quadro assente; manca il cicalino «fuori piano» | Media | Aperta | Firmware |
| FW-13 | Ripristino dopo corto sulla catena: automatico (manuale) o memorizzato (firmware) | Media | Da decidere | Firmware |
| FW-14 | Contattori eccitati fino a 30 s dopo lo stop | Media | Chiarita | Firmware |
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
| FW-26 | Tempo limite per manovre di emergenza e manutenzione non implementato | Media | Aperta | Firmware |
| FW-27 | Controllo software di assenza passeggeri con sicurezze escluse non implementato | Media | Aperta | Firmware |
| FW-28 | Priorità dei comandi di cabina ed esclusione delle altre postazioni non implementate | Media | Da verificare | Firmware |
| FW-29 | Monitoraggio software del freno e controllo esterno di velocità e corrente dichiarati nell'analisi ma assenti | Alta | Aperta | Firmware |
| FW-30 | Encoder motore SIN/COS (7EBLR002) non letto dal firmware | Media | Da decidere | Firmware |
| FW-31 | Rampe del firmware diverse dall'ipotesi della relazione di calcolo (0,125 m/s²) | Media | Da verificare | Firmware |
| HW-01 | Spina X8: esclude quasi tutta la catena con un solo ingresso di rilevamento | Media | Da decidere | Hardware |
| HW-02 | Bypass trap/tray comandati solo via software | Media | Chiarita | Hardware |
| HW-03 | Relè K1…K4 non dichiarati a contatti guidati sugli schemi | Media | Da verificare | Hardware |
| HW-04 | `brk_fb` misura la tensione dopo K4, non lo stato meccanico del freno | Media | Da decidere | Hardware |
| HW-05 | Gate dei MOSFET OTR-EN/BRK-EN: pull-down al reset non visibile | Media | Da verificare | Hardware |
| HW-06 | Nodo S3B non letto dalla TTF (ponte X15) | Bassa | Da decidere | Hardware |
| HW-07 | Contattore di uscita motore S22 fuori dalla catena | Media | Chiarita | Hardware |
| HW-08 | Nomenclatura incoerente fra schemi e manuali (K3A/K5A, F1/F7, Q15B/Q16A) | Bassa | Aperta | Hardware |
| HW-09 | Punti da confermare sugli schemi: ponticelli TC/FT, STO sul nodo S4, percorso del pesacarico | Bassa | Da verificare | Hardware |
| NOR-01 | Analisi dei rischi Rev.7: lettura e confronto con il firmware | Alta | Risolta | Normativa |
| NOR-02 | EN 81-41 5.5.7.3: alimentazione di motore e freno da interrompere alla fine del comando | Media | Chiarita | Normativa |
| NOR-03 | Freno sul lato motore con trasmissione a cinghie e monitoraggio della cinghia | Media | Da verificare | Normativa |
| NOR-04 | Giustificazione nell'analisi dei rischi del contatto paracadute e del suo bypass | Bassa | Da verificare | Normativa |
| NOR-05 | Qualificazione di K3 e K4 come «contattori» ai fini di EN 81-41 | Media | Da verificare | Normativa |
| NOR-06 | Ipotesi di 60 arresti dinamici del freno nella vita contro gli eventi che aprono la catena in marcia | Media | Da verificare | Normativa |
| DOC-01 | Precisione di fermata: ±20 mm (IM.159) contro ±10 mm (IM.160) | Bassa | Aperta | Documentazione |
| DOC-02 | IM.160 e IM.159: prove duplicate e refusi su fondo e tetto sensibili | Bassa | Aperta | Documentazione |
| DOC-03 | IM.159 §9: porte «chiuse e sbloccate» prima della messa fuori servizio | Bassa | Aperta | Documentazione |
| DOC-04 | IM.161: discesa di emergenza incompleta e tetto sensibile non descritto | Media | Aperta | Documentazione |
| DOC-05 | Note di progetto: numerazione S2/S3 e fusibile F1/F7 | Bassa | Aperta | Documentazione |
| DOC-06 | IM.158: parti ancora da completare (UDEC.C/CX, codici di errore, menu) | Media | Aperta | Documentazione |
| DOC-07 | Definizione di «trap» nell'analisi dei rischi: tetto sensibile o bordi superiori | Bassa | Aperta | Documentazione |
| DOC-08 | Valori di carico non uniformi: 200, 260 e 275 kg | Media | Da verificare | Documentazione |
| DOC-09 | Velocità lineare della cinghia nei calcoli Megadyne (0,2 m/s) contro 0,15 m/s di progetto | Bassa | Da verificare | Documentazione |
| PRV-01 | Precisione di fermata ±10 mm | Media | Da verificare | Prove |
| PRV-02 | Velocità ≤ 0,15 m/s misurata e conversione giri → m/s | Bassa | Da verificare | Prove |
| PRV-03 | Sblocco della serratura con cabina appena in oltrecorsa | Media | Da verificare | Prove |
| PRV-04 | Fermata immediata al rilascio del tasto | Bassa | Da verificare | Prove |
| PRV-05 | Prove fondo e tetto sensibili (IM.160 2.03.20 e 2.03.21) | Alta | Da verificare | Prove |
| PRV-06 | Prova della manutenzione dal quadro (IM.160 2.03.01) | Media | Da verificare | Prove |
| PRV-07 | Ripristino dopo corto sulla catena (IM.160 2.03.15) | Media | Da verificare | Prove |
| PRV-08 | Arresto per perdita del segnale del radiocomando oltre 500 ms | Media | Da verificare | Prove |
| PRV-09 | Verifica «categoria 2» dei bypass trap e tray a ogni cambio di direzione | Media | Da verificare | Prove |
| PRV-10 | Consumo in stand-by di 1 W dichiarato | Bassa | Da verificare | Prove |
| CHI-01 | Velocità in bypass scritta come 5 rpm nell'analisi | Media | Risolta | Chiarimenti |
| CHI-02 | Nomi dei trip S2/S3 contro commenti di `read_inputs` | Bassa | Chiarita | Chiarimenti |
| CHI-03 | Rete S5 dello schema e ingresso «S8» del firmware | Bassa | Chiarita | Chiarimenti |
| CHI-04 | La spina X8 esclude il paracadute: è voluto | Bassa | Chiarita | Chiarimenti |
| CHI-05 | Assegnazione dei bus CAN: CAN250 cappello, CAN125 cabina | Bassa | Chiarita | Chiarimenti |
| CHI-06 | Ponte X15 / nodo S3B: contatto del VFD non presente | Bassa | Chiarita | Chiarimenti |
| CHI-07 | Errore nel confronto con i manuali: fondo e tetto sensibili | Media | Risolta | Chiarimenti |
| CHI-08 | Paracadute del contrappeso coperto dai sensori di allentamento cinghie | Bassa | Chiarita | Chiarimenti |
<!-- RIEPILOGO-FINE -->

## Fonti consultate

| Documento | Dove | Stato di lettura |
|---|---|---|
| Firmware e schemi del repository | `TTF-Base`, `TTF-CX`, `UDEC.CX`, `Schema Macchina` | letti (TTF-Base e TTF-CX a livello di moduli di sicurezza; UDEC.CX non letto) |
| Analisi dei rischi Elevance Rev.7 (2026-07-02) | Notion, fascicolo tecnico Cap.2 | letta la parte sul comando, sulla manutenzione e le checklist EN 60204-1 e EN 81-41 |
| ST.TEC.068 rev0, relazione tecnica (2026-01-12) | Notion, Cap.3 | letta nelle parti su cinghie, freni, paracadute e allentamento |
| CP.TEC.025 Rev.5, configurazione di prodotto (2026-11-03) | Notion, Cap.1 | letta |
| IM.TEC.158, 159, 160, 161 | Notion, Cap.4 (158 come pagina; 159-161 come PDF) | letti |
| Note di progetto e di collaudo Elevance, pagine sulla base di conoscenza | Notion | lette (elenco in `analisi.md` §4) |
| Cap.6: nota Amer sul freno, scheda sensore 7EBLR002, calcolo cinghia Megadyne MFC 20260211-1, rapporto Megadyne RI 007-26, certificato IMQ del blocco porte MD2 | Notion, Cap.6 | letti; il rapporto Megadyne dice 940 N/cm contro 760 N/cm dichiarati (nessuna issue) |
| DCM-6329 Rev.3 (motoriduttore), verifica statica e a fatica, F353.01.0029 | Notion, Cap.6 | non letti |
| IM.TEC.157 (montaggio), 162, 163, 164 (GAD), TARGA, cartellonistica | Notion, Cap.3 e Cap.4 | non letti |
| Segnalazioni macchine pilota, punti aperti tecnico-commerciali | Notion, «Da fare» | solo i titoli (le pagine sono vuote); la segnalazione «Riprogettazione sistema contatti in testata: consenso porte e extracorsa» (2026-11-05) riguarda la catena e va seguita |

## Ordine di lavoro consigliato

1. **Sicurezza funzionale del firmware**: FW-01, FW-02 insieme a FW-29, FW-04, poi FW-06 (requisito dei manuali e dell'analisi) e FW-05.
2. **Funzioni che l'analisi dei rischi dichiara e il firmware non ha**: FW-26, FW-27, FW-28, FW-09, FW-12. Se non si implementano, va corretta l'analisi.
3. **Decisioni di progetto** da prendere prima di toccare il codice: FW-13, FW-30, HW-01, HW-04, HW-06.
4. **Verifiche** che non richiedono codice: HW-03 con NOR-05, HW-05, HW-09, NOR-03, NOR-04, NOR-06, FW-31.
5. **Documentazione**: DOC-04, DOC-06 e DOC-08 prima del rilascio delle macchine pilota.
6. **Prove** (PRV): dopo ogni correzione che le riguarda; PRV-05 dipende da FW-06, PRV-06 da FW-12, PRV-07 da FW-13, PRV-09 da HW-02.

Percorsi dei file firmware: `TTF-Base/Firmware/TTF-Base/Core/` (per brevità `Src/…` e `Inc/…`).

---

## Firmware

### FW-01 · Collaudo paracadute avviabile da qualsiasi voce di menu
**Gravità:** Alta · **Stato:** Aperta · **Fonte:** analisi §2.1.1, confronto manuali §4.1
- **Dove:** `Src/application.c:81-86` (`hmi_process_input`), `Src/run_mode.c:17-57` (`run_mode_free_fall`).
- **Problema:** premendo OK nel menu si imposta sempre `MACHINE_STATE_FALL_TEST`; la voce scelta (`menu_index`) è ignorata e anche «NORMALE» avvia il collaudo. Si imposta da qualsiasi stato, anche con la cabina in marcia, senza protezioni. In `FALL_TEST` il freno si apre con il tasto 2 o con `IN4`. Il manuale (IM.159, prova 20) prevede il collaudo con la pedana al piano superiore e a pieno carico.
- **Proposta:** (1) agire solo se la voce scelta è COLLAUDO; (2) consentirlo solo a cabina ferma (`NORMAL`, VFD fermo) e al P1; (3) richiedere una conferma deliberata (OK prolungato ≥ 3 s o doppia conferma sul display); (4) segnalare sul display e con il cicalino; (5) sistemare gli indici del menu (FW-18).
- **Chiusura:** OK su «NORMALE» non cambia stato; il collaudo parte solo al piano alto con conferma; prova IM.159 n. 20.
- **Storia:** 2026-11-07 aperta. 2026-11-07 Analisi dei rischi Rev.7 (16): «le operazioni potenzialmente pericolose devono essere confermate» sul pannello di servizio; la conferma della proposta (3) è quindi un requisito.

### FW-02 · Freno non verificato: nessun timeout né plausibilità su `brk_fb`; marcia SU illimitata in discesa
**Gravità:** Alta · **Stato:** Aperta · **Fonte:** analisi §2.1.2; HW-04
- **Dove:** `Src/run_mode.c:120-135` (`run_mode_brake_release`), `Src/run_mode.c:165-200` (`run_mode_down`), `Inc/vfd.h`.
- **Problema:** prima di scendere si comanda una marcia SU a 11 rpm finché `brk_fb` non resta attivo 100 ms, senza timeout. In `DOWN` non si controlla `is_at_P1`: se il feedback non arriva la cabina sale finché la chiamata è premuta, fino all'extracorsa S2. Manca il controllo opposto: con `brk_fb` bloccato attivo si va a velocità piena a freno chiuso. OTR e bypass hanno un trip di incongruenza, il freno no.
- **Proposta:** (1) timeout su `brk_fb` (ad es. 1,5 s dal comando) → `vfd_halt` + nuovo trip dedicato (in coda a `trip_list.h`, prima di `TRIP_ID_SIZE`, con messaggio in `display_next_trip`); (2) limitare tempo e corsa della marcia SU in discesa; (3) plausibilità: `brk_fb` attivo con `BRK-EN` spento e macchina ferma oltre 1 s → trip; (4) aggiornare gli indicatori H30/H31 in IM.158.
- **Chiusura:** con il feedback scollegato o forzato la macchina si ferma entro il tempo previsto e il trip è visibile; prova con carico.
- **Storia:** 2026-11-07 aperta. 2026-11-07 Analisi dei rischi Rev.7: EN 60204-1 14.6 dichiara «gestito in software monitorando il segnale di controllo dei freni» e 9.3.4 «controllo esterno del segnale di velocità e della corrente assorbita»; nel firmware non esistono (FW-29). Freno: doppio, a molle, 10 Nm (14 Nm max), 48 V, 0,5 A, pull-in ≤ 35 V, drop-out ≥ 1 V (nota Amer del 03/03/2026).

### FW-03 · Sblocco porta senza verifica di arresto del motore
**Gravità:** Media · **Stato:** Aperta · **Fonte:** analisi §2.1.3; IM.160 2.03.12
- **Dove:** `Src/run_mode.c:312-345` (`run_mode_open_door`), `Src/door.c` (`door_unlock`).
- **Problema:** raggiunta la zona di sblocco si fa `vfd_stop` e si passa a `OPEN_DOOR`, che comanda subito lo sblocco; non si controlla `vfd.motor_speed` mentre la decelerazione dura almeno 1 s (`VFD_ATIME_SLOW`). La gravità dipende dall'ampiezza della zona (IM.160 2.03.12 b: al massimo 50 mm).
- **Proposta:** consentire `door_unlock()` solo con VFD non in marcia e `|motor_speed|` sotto la soglia già usata in `vfd_scan_cycle` (10), oppure dopo un tempo ≥ rampa + margine.
- **Chiusura:** misura in prova (PRV-03) e verifica che nessuna manovra di sblocco inizi con velocità non nulla.
- **Storia:** 2026-11-07 aperta. 2026-11-07 gravità abbassata da Alta a Media: l'Analisi dei rischi Rev.7 (67) assume un circuito di sicurezza che impedisce lo sblocco fuori piano e la zona di sblocco è al massimo 50 mm (IM.160 2.03.12 b); l'esposizione resta alla decelerazione finale (stima 25 mm a 0,05 m/s).

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
**Gravità:** Media · **Stato:** Aperta · **Fonte:** confronto manuali §4.2; HW-01
- **Dove:** `Src/application.c:498-523` (`set_prenotation`), `Inc/udec_c_inl.h`.
- **Problema:** IM.158 dice che con la spina restano attivi solo i comandi del pannello principale. `set_prenotation` non dipende dal bypass: con UDEC.C online anche i tasti di cabina e il radiocomando possono muovere la macchina.
- **Proposta:** con `run_mode.bypass_manutenzione` ignorare gli ingressi di UDEC.C e usare solo `IN1/IN2` del quadro; oppure aggiornare IM.158 se il comportamento attuale è voluto. Decidere con HW-01.
- **Chiusura:** con la spina inserita i tasti di cabina non muovono la macchina (o il manuale lo dichiara).
- **Storia:** 2026-11-07 aperta. 2026-11-07 Analisi dei rischi Rev.7 (16) e (22): in emergenza o manutenzione l'operatore, con azione deliberata, deve poter escludere le altre postazioni di comando e operare dal pannello di servizio; il firmware non lo fa. Non è più «da decidere».

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
- **Storia:** 2026-11-07 aperta. 2026-11-07 Analisi dei rischi Rev.7 (22) descrive modalità speciali dal pannello di servizio, a comando mantenuto e con controllo software di assenza passeggeri (FW-27) e tempo limite (FW-26).

### FW-13 · Ripristino dopo corto sulla catena: automatico (manuale) o memorizzato (firmware)
**Gravità:** Media · **Stato:** Da decidere · **Fonte:** confronto manuali §4.1; IM.160 2.03.15, IM.158
- **Dove:** `Src/application.c:302-367` (`check_safety`), `Src/trips.c`.
- **Problema:** IM.160 dice che l'impianto si ripristina in automatico dopo l'eliminazione del cortocircuito; il firmware memorizza `TRIP_S1_FUSE` e `TRIP_SAFETY` fino al riavvio. IM.158 lascia il punto «da decidere».
- **Proposta:** scegliere e allineare i due documenti: ripristino automatico per S1 (fusibile/corto) oppure memorizzazione con reset, e aggiornare IM.160 2.03.15 di conseguenza.
- **Chiusura:** decisione registrata qui; prova PRV-07.
- **Storia:** 2026-11-07 aperta.

### FW-14 · Contattori eccitati fino a 30 s dopo lo stop
**Gravità:** Media · **Stato:** Chiarita · **Fonte:** confronto manuali §4.4; NOR-02
- **Dove:** `Inc/pmode.h` (`TIMEOUT_PMODE_WAIT_TO_CHARGER = 30000`), `Src/pmode.c:141-154`.
- **Problema:** dopo lo stop `pmode` passa a `ST_WAIT_TO_CHARGER` e tiene K3/K4 eccitati per 30 s, mentre il commento dice «una decina di secondi». La pagina Notion su UDEC.M descrive lo stesso rischio (movimento non comandato con contattori eccitati) e propone di limitarlo a pochi secondi.
- **Proposta:** ridurre l'attesa alla sola decelerazione più un margine, oppure terminarla al primo istante con VFD fermo; correggere il commento; valutare il costo (ricarica del DC link a ogni corsa).
- **Chiusura:** decisione in NOR-02; prova di stop e ripartenza.
- **Storia:** 2026-11-07 aperta. 2026-11-07 chiarita: l'Analisi dei rischi Rev.7 (18) e la checklist EN 81-41 5.5.7 documentano la finestra di «qualche decina di secondi» come rischio residuo accettato («meccanismo di backup», doppio contattore che alimenta VFD e freno, disattivazione ritardata per non stressare il DC link). Resta da allineare il commento in `Inc/pmode.h` (FW-21).

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

### FW-26 · Tempo limite per manovre di emergenza e manutenzione non implementato
**Gravità:** Media · **Stato:** Aperta · **Fonte:** Analisi dei rischi Rev.7, checklist EN 81-41 5.4.3.1
- **Dove:** `Src/application.c` (`run_mode.bypass_manutenzione`), `Src/battery.c` (contatore di carica).
- **Problema:** la checklist dichiara «contatore di carica su batteria e tempo limite per la manutenzione». Il contatore di carica esiste; un tempo limite in modalità spina X8 o manutenzione non c'è: la macchina resta in bypass finché la spina è inserita.
- **Proposta:** definire il tempo limite (e il comportamento allo scadere: arresto con segnalazione) e implementarlo insieme a FW-12.
- **Chiusura:** prova con spina inserita oltre il tempo previsto.
- **Storia:** 2026-11-07 aperta.

### FW-27 · Controllo software di assenza passeggeri con sicurezze escluse non implementato
**Gravità:** Media · **Stato:** Aperta · **Fonte:** Analisi dei rischi Rev.7 (22)
- **Dove:** `Src/application.c`, `Src/run_mode.c` (stato `bypass_manutenzione`).
- **Problema:** «le operazioni di manutenzione a sicurezze escluse devono essere tassativamente effettuate senza passeggeri (è previsto un controllo software, non di sicurezza, che minimizza la possibilità)». Nel firmware non esiste alcun controllo.
- **Proposta:** usare il pesacarico (`ftc3`) o l'assenza di comandi di cabina per negare la marcia in bypass con carico presente, oppure aggiornare l'analisi se il controllo non si fa.
- **Chiusura:** prova con carico in cabina e spina inserita.
- **Storia:** 2026-11-07 aperta.

### FW-28 · Priorità dei comandi di cabina ed esclusione delle altre postazioni non implementate
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** Analisi dei rischi Rev.7 (16), (75), (79), ESR 6.4.2
- **Dove:** `Src/application.c:498-523` (`set_prenotation`), `Inc/udec_c_inl.h` (`is_P0_on`, `is_P1_on`).
- **Problema:** l'analisi dichiara che «il posto di comando in cabina è prioritario» e che con un passeggero «i comandi provenienti dalle altre postazioni sono ignorati»; i comandi ai piani non devono avviare la macchina se i comandi di cabina sono azionati (ESR 6.4.2). `set_prenotation` unisce gli ingressi di cabina e radiocomando senza priorità. Non verificato come siano mappati gli ingressi di UDEC.C sulle postazioni.
- **Proposta:** verificare la mappatura e implementare la priorità cabina; decidere con FW-09.
- **Chiusura:** prova con comandi contrastanti da cabina e radiocomando.
- **Storia:** 2026-11-07 aperta.

### FW-29 · Monitoraggio software del freno e controllo esterno di velocità e corrente dichiarati nell'analisi ma assenti
**Gravità:** Alta · **Stato:** Aperta · **Fonte:** Analisi dei rischi Rev.7, EN 60204-1 9.3.4 e 14.6
- **Dove:** `Src/run_mode.c`, `Src/vfd.c`, `Inc/vfd.h` (`vfd.motor_speed`, `vfd.motor_current`).
- **Problema:** l'analisi dichiara per i freni «gestito in software monitorando il segnale di controllo dei freni (il VFD non supporta il monitoraggio)» e per gli interblocchi «controllo esterno del segnale di velocità e della corrente assorbita». Nel firmware il freno è usato solo per il rilascio (FW-02) e la corrente motore è letta ma non usata; la velocità serve solo al timeout di arresto.
- **Proposta:** implementare i controlli dichiarati (coerenza fra comando, `brk_fb`, velocità e corrente) oppure correggere l'analisi. Va deciso insieme a FW-02.
- **Chiusura:** controlli implementati e provati, oppure analisi aggiornata.
- **Storia:** 2026-11-07 aperta.

### FW-30 · Encoder motore SIN/COS (7EBLR002) non letto dal firmware
**Gravità:** Media · **Stato:** Da decidere · **Fonte:** scheda 7EBLR002, IM.158 (H10-H11), note di progetto
- **Dove:** `TTF-Base/Encoder.kicad_sch` (SIN-A/COS-A, `ENC0`), nessun modulo di lettura nel firmware.
- **Problema:** il sensore SIN/COS per motori BLDC a 8 poli (4 periodi per giro, 5 V, offset 2,35-2,45 V) è montato ma IM.158 dice «al momento non funziona, forse sarà tolto». Le note di progetto lo indicavano per la posizione al piano superiore e per confermare la direzione di marcia (utile ai bypass). L'analisi non lo cita.
- **Proposta:** decidere se rimuoverlo dal progetto o usarlo (direzione, velocità, controllo di movimento non comandato); se resta, aggiornare IM.158.
- **Chiusura:** decisione e allineamento di schede e manuali.
- **Storia:** 2026-11-07 aperta.

### FW-31 · Rampe del firmware diverse dall'ipotesi della relazione di calcolo (0,125 m/s²)
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** ST.TEC.068 §5
- **Dove:** `Inc/application.h` (`VFD_ATIME_FAST = 40`, `VFD_ATIME_SLOW = 10`), `Inc/vfd_inl.h` (`vfd_halt` con rampa 1), `Src/vfd.c`.
- **Problema:** la verifica di strappo del dente in frenata nominale (SF 1,2) usa una decelerazione di −0,125 m/s² «da impostazioni di fabbrica» del regolatore. Il firmware impone rampe proprie: stop di 1 s (circa 0,1 m/s² da 0,10 m/s) e `vfd_halt` di 0,1 s (circa 1 m/s², se l'unità è il decimo di secondo come in `vfd.c`). Con decelerazione maggiore il margine scende; non ho ricalcolato.
- **Proposta:** rifare la verifica con le rampe reali di stop e di halt, oppure limitare la rampa di halt.
- **Chiusura:** relazione ricalcolata o rampe adeguate.
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
**Gravità:** Media · **Stato:** Chiarita · **Fonte:** analisi §3.3.2
- **Dove:** schede TTF-CX (K1/K2, `Q2A/Q2B`), `Src/application.c:339-351`.
- **Problema:** i relè K1/K2 si attivano via CAN senza interblocco hardware; la protezione è il feedback con TON da 2 s: un contatto incollato lascia scoperto il rilevamento ostacoli fino a 2 s. La nota di progetto riconosce che il circuito non è «completamente di sicurezza».
- **Proposta:** valutare un interblocco hardware (ad es. bypass abilitato solo con direzione compatibile) e ridurre il tempo di rilevamento; decidere con l'analisi dei rischi.
- **Chiusura:** decisione e revisione dello schema.
- **Storia:** 2026-11-07 aperta. 2026-11-07 chiarita: Analisi dei rischi Rev.7 (14) dichiara la circuiteria dei bypass «monitorata ma non ridondata (categoria 2)», con verifica a ogni cambio di direzione mediante contatti meccanicamente collegati. Resta da provare che la verifica avvenga (PRV-09).

### HW-03 · Relè K1…K4 non dichiarati a contatti guidati sugli schemi
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** analisi §3.3.3
- **Dove:** K3 (AHES4192), K4 (G2RL-2), K1/K2 (Finder 50.12.9.024.x000).
- **Problema:** gli schemi non indicano contatti guidati forzatamente; la scheda Notion «Bypass trap/tray» dichiara «relè monitorati a contatti guidati». Il controllo di saldatura vale solo se il contatto di feedback è guidato rispetto a quello di potenza.
- **Proposta:** controllare i datasheet dei codici montati e documentarlo in IM.158.
- **Chiusura:** datasheet allegati o relè sostituiti.
- **Storia:** 2026-11-07 aperta. 2026-11-07 l'Analisi dei rischi Rev.7 dichiara «contatti specchio» (EN 81-41 5.5.6.3) e «azionamento positivo e contatti specchio/guidati» (EN 60204-1 9.2.4.2.2): resta da verificare sui datasheet dei codici montati.

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
**Gravità:** Media · **Stato:** Chiarita · **Fonte:** analisi §3.3.7; NOR-02
- **Dove:** `Src/vfd.c:166-311` (macchina a stati dell'uscita), schema `Cappello` (S22).
- **Problema:** il motore è staccato da K3 (alimentazione del VFD) e dall'ingresso STO; S22 è comandato dal firmware e «non fa parte del circuito di sicurezza». Se serve un secondo mezzo indipendente di interruzione è una valutazione normativa.
- **Proposta:** valutare con l'organismo di certificazione se S22 debba entrare nella catena.
- **Chiusura:** decisione.
- **Storia:** 2026-11-07 aperta. 2026-11-07 chiarita: l'Analisi dei rischi Rev.7 considera «doppio contattore» quelli che comandano alimentazione del VFD (K3) e freno (K4); S22 non è richiesto nella catena. La qualificazione di K3/K4 come contattori è NOR-05.

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

### NOR-01 · Analisi dei rischi Rev.7: lettura e confronto con il firmware
**Gravità:** Alta · **Stato:** Risolta · **Fonte:** fascicolo tecnico Notion, Cap.2
- **Dove:** `Analisi_Rischi_Elevance_-_Rev.7.pdf` (39 pagine, datata 2026-07-02), allegata alla pagina «Cap.2 - Analisi dei rischi».
- **Esito:** letta per intero la parte di sistema di comando (ESR 1.2.x), movimenti incontrollati, manutenzione, EN 60204-1, EN 81-41 e variazioni BS 5900. Il confronto con il firmware ha prodotto FW-26, FW-27, FW-28, FW-29, FW-31, NOR-05, NOR-06, DOC-07, DOC-08, PRV-08 e PRV-09, e ha cambiato lo stato di FW-03, FW-09, FW-14, HW-02, HW-07, NOR-02.
- **Cosa conferma:** comando solo a uomo presente e arresto normale al rilascio (17, 18); STOP in cabina che toglie alimentazione a tutti gli attuatori (20); doppio freno a molle interbloccato in catena (EN 81-41 5.4.2.2.1); sensori di allentamento su tutti i rami (5.4.1.5); bypass di fondo e tetto per il movimento in senso opposto all'ostacolo (EN 60204-1 9.3.6); sicurezze «essenzialmente in logica cablata», software senza parametri di sicurezza (13, 14).
- **Limite:** non sono state lette le sezioni sui rischi meccanici puri (ESR 1.3, 1.5, 4.x non legati al comando) né le considerazioni sul touch.
- **Storia:** 2026-11-07 aperta come «Rev.5 non letta»; stessa data: la Rev.7 è stata caricata su Notion, letta e chiusa.

### NOR-02 · EN 81-41 5.5.7.3: alimentazione di motore e freno da interrompere alla fine del comando
**Gravità:** Media · **Stato:** Chiarita · **Fonte:** pagina Notion «Controllo dei contattori di OTR/FRS su UDEC.M»
- **Dove:** `Src/pmode.c` (`ST_WAIT_TO_CHARGER`), HW-07.
- **Problema:** il requisito è citato per UDEC.M; in TTF-Base K3/K4 restano eccitati fino a 30 s (FW-14) e S22 resta chiuso ≥ 10 s.
- **Proposta:** decidere come l'analisi dei rischi giustifica o corregge questi tempi.
- **Chiusura:** decisione registrata.
- **Storia:** 2026-11-07 aperta. 2026-11-07 chiarita: vedi FW-14; la tempistica di 30 s è dichiarata e motivata nell'Analisi dei rischi Rev.7 (18) e nella checklist EN 81-41 5.5.7 («la norma non specifica esattamente le tempistiche»).

### NOR-03 · Freno sul lato motore con trasmissione a cinghie e monitoraggio della cinghia
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** pagina Notion «Requisiti del freno elettromeccanico (da EN 81-41)»
- **Dove:** schema `Cappello` (freno M2 vicino al motore), sensori di allentamento S3–S6.
- **Problema:** per impianti reversibili con cinghia o catena la nota ricorda che il freno deve stare sul lato del carico e che la cinghia va monitorata con sensori di sicurezza. Elevance ha il freno sul motore, il paracadute e sensori di allentamento in catena.
- **Proposta:** documentare nell'analisi dei rischi come la configurazione soddisfa o motiva la deroga.
- **Chiusura:** giustificazione nel fascicolo.
- **Storia:** 2026-11-07 aperta. 2026-11-07 Analisi dei rischi Rev.7: 5.4.2.2.7 «OK, motore elettrofrenato (sull'asse)», 5.4.5.6 paracadute, 5.4.1.5 sensori di allentamento su tutti i rami; la riga 5.4.1.4 è «Intermediate drives: N/A», mentre la nota Notion cita lo stesso numero di clausola per il freno lato carico: verificare la numerazione nel testo EN 81-41:2024.

### NOR-04 · Giustificazione nell'analisi dei rischi del contatto paracadute e del suo bypass
**Gravità:** Bassa · **Stato:** Da verificare · **Fonte:** pagina Notion «Necessità di contatto di sicurezza su paracadute»
- **Dove:** catena S3.1 PAR, spina X8.
- **Problema:** la nota sostiene che il contatto di sicurezza sul paracadute non è obbligatorio se l'analisi dimostra l'assenza di rischi; in Elevance il contatto è in catena e la spina lo esclude.
- **Proposta:** riportare nell'analisi la motivazione di entrambe le scelte.
- **Chiusura:** riferimento nel fascicolo.
- **Storia:** 2026-11-07 aperta. 2026-11-07 ST.TEC.068 §6: il paracadute del contrappeso non ha contatto sul corpo (EN 81-41 5.3.1.5) e viene coperto dai 4 sensori di allentamento sui tratti liberi delle cinghie; vedi CHI-08. Il contatto S15/S16 riguarda il paracadute di cabina.

### NOR-05 · Qualificazione di K3 e K4 come «contattori» ai fini di EN 81-41
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** Analisi dei rischi Rev.7 (EN 81-41 5.4.2.2.8, 5.5.6, 5.5.7), pagina Notion sul freno
- **Dove:** K3 (AHES4192), K4 (G2RL-2) sulla scheda `TTF-Base`.
- **Problema:** l'analisi parla di «doppio contattore che comanda direttamente l'alimentazione di VFD e freno» con contatti specchio; la pagina Notion sul freno osserva che EN 81-41:2024 parla esplicitamente di contattori e non ammette l'interruzione con ausiliari. Sulla scheda sono montati relè.
- **Proposta:** verificare con il laboratorio di certificazione se K3 e K4 possono essere considerati contattori ai fini della norma (HW-03).
- **Chiusura:** parere o sostituzione dei componenti.
- **Storia:** 2026-11-07 aperta.

### NOR-06 · Ipotesi di 60 arresti dinamici del freno nella vita contro gli eventi che aprono la catena in marcia
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** ST.TEC.068 (verifica a fatica del motoriduttore)
- **Dove:** `Src/application.c` (`check_safety`), `Src/vfd.c` (`vfd_timed_out`, reboot), catena di sicurezze.
- **Problema:** la relazione assume che il doppio freno «lavori dinamicamente per 60 volte nella vita del motoriduttore (1 intervento al mese per 5 anni)», con coppia fino a 654 Nm alla puleggia. Ogni apertura della catena in marcia (ostacolo su fondo o tetto, stop di cabina, trip) fa cadere K3/K4 e produce un arresto dinamico; anche `vfd_timed_out` e il reboot del VFD in marcia.
- **Proposta:** stimare la frequenza reale degli arresti dinamici (soprattutto per ostacoli e false aperture) e confrontarla con l'ipotesi.
- **Chiusura:** stima documentata; eventuale revisione dei calcoli.
- **Storia:** 2026-11-07 aperta.

---

## Documentazione

### DOC-01 · Precisione di fermata: ±20 mm (IM.159) contro ±10 mm (IM.160)
**Gravità:** Bassa · **Stato:** Aperta · **Fonte:** confronto manuali §4.2
- **Dove:** IM.159 §10.06 prova 1, IM.160 2.03.04 e 2.03.06.
- **Problema:** due limiti diversi per la stessa prova.
- **Proposta:** adottare un unico valore (vedi PRV-01).
- **Chiusura:** manuali allineati.
- **Storia:** 2026-11-07 aperta. 2026-11-07 Analisi dei rischi Rev.7, EN 81-41 5.4.2.3: «±10 mm per il posizionamento e 20 mm in frenata di emergenza»: IM.159 prova 1 (20 mm) va corretta o distinta.

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
- **Storia:** 2026-11-07 aperta. 2026-11-07 la lettura del firmware e del registro fornisce già trip, indicatori e messaggi; l'Analisi dei rischi Rev.7 rimanda a IM.158 per le procedure di emergenza.

### DOC-07 · Definizione di «trap» nell'analisi dei rischi: tetto sensibile o bordi superiori
**Gravità:** Bassa · **Stato:** Aperta · **Fonte:** Analisi dei rischi Rev.7
- **Dove:** definizioni iniziali, (78), (82), (83).
- **Problema:** la definizione dice «trap: il tetto/bordi superiori sensibili di sicurezza»; la voce (82) dice «bordi sensibili non presenti a causa dell'altezza» e la (83) tratta l'ostruzione dell'infill, «OK». Il termine è usato con tre significati.
- **Proposta:** definire trap come rilevamento di ostruzione dell'infill (e dell'eventuale tetto) e riportarlo in IM.158 e nei manuali.
- **Chiusura:** definizione unica nei documenti.
- **Storia:** 2026-11-07 aperta.

### DOC-08 · Valori di carico non uniformi: 200, 260 e 275 kg
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** CP.TEC.025 Rev.5, ST.TEC.068, IM.160 2.03.07, MFC 20260211-1
- **Dove:** portata 200 kg (CP.TEC.025, EN 81-41 5.1.6: minimo 250 kg, deroga dichiarata); carico statico massimo con sovraccarico 260 kg (ST.TEC.068); prova di sovraccarico a 275 kg (IM.160) e «massa persone 275 kg» nel calcolo Megadyne.
- **Problema:** la prova non discrimina la soglia di intervento del pesacarico; la soglia effettiva non è scritta nei documenti letti.
- **Proposta:** definire la soglia di intervento in kg e allineare prova e calcoli.
- **Chiusura:** documenti coerenti; prova di sovraccarico alla soglia definita.
- **Storia:** 2026-11-07 aperta.

### DOC-09 · Velocità lineare della cinghia nei calcoli Megadyne (0,2 m/s) contro 0,15 m/s di progetto
**Gravità:** Bassa · **Stato:** Da verificare · **Fonte:** MFC 20260211-1-Rev0
- **Dove:** foglio di calcolo cinghia, riga «velocità lineare cinghia 0,2 m/s» con «omega motrice 28 rpm».
- **Problema:** con puleggia Ø101,86 mm i 28 rpm corrispondono a 0,149 m/s; il valore 0,2 m/s non è coerente con la riga accanto né con il limite 0,15 m/s.
- **Proposta:** chiedere conferma a Megadyne e correggere il calcolo se serve.
- **Chiusura:** calcolo coerente.
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
- **Storia:** 2026-11-07 aperta. 2026-11-07 con i dati Amer (rapporto 25,3:1) e Megadyne (puleggia Z40, Ø primitivo 101,86 mm) la conversione dà RPM_SPEED_10 = 455 rpm ≈ 0,096 m/s, RPM_SPEED_15 = 683 rpm ≈ 0,144 m/s, RPM_SPEED_5 = 227 rpm ≈ 0,048 m/s, rilascio freno 11 rpm ≈ 0,0023 m/s (stima mia, da confermare in prova).

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

### PRV-08 · Arresto per perdita del segnale del radiocomando oltre 500 ms
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** Analisi dei rischi Rev.7 (15), EN 60204-1 9.2.4.6
- **Prova:** tenere premuto un comando via radio e spegnere il trasmettitore: la macchina deve fermarsi entro 500 ms. La funzione è dichiarata ma il firmware di TTF-Base non la implementa: se esiste, è in UDEC.C.
- **Storia:** 2026-11-07 aperta.

### PRV-09 · Verifica «categoria 2» dei bypass trap e tray a ogni cambio di direzione
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** Analisi dei rischi Rev.7 (14); HW-02
- **Prova:** forzare l'incollamento del contatto di K1 o K2 e verificare che la macchina non parta nella direzione successiva (il firmware passa da `NO_BYPASS` e confronta comando e feedback, con TON di 2 s).
- **Storia:** 2026-11-07 aperta.

### PRV-10 · Consumo in stand-by di 1 W dichiarato
**Gravità:** Bassa · **Stato:** Da verificare · **Fonte:** CP.TEC.025 Rev.5 (Reg. UE 2023/826, «da certificare»)
- **Prova:** misura del consumo a macchina ferma con batteria carica; il firmware non ha modalità di risparmio (display, LED, CAN e NPB sempre attivi).
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

### CHI-08 · Paracadute del contrappeso coperto dai sensori di allentamento cinghie
**Gravità:** Bassa · **Stato:** Chiarita · **Fonte:** ST.TEC.068 §6
- **Chiarimento:** il paracadute del contrappeso non ha un contatto sul proprio corpo (EN 81-41 5.3.1.5): il suo intervento è rilevato dai 4 sensori di allentamento sui tratti liberi delle cinghie, che sono i contatti S3-S6 in catena. `TRIP_S3_BELT` indica quindi sia l'allentamento sia l'intervento del paracadute del contrappeso. Il contatto S15/S16 è quello del paracadute di cabina.
- **Storia:** 2026-11-07 chiarita.

---

## Cronologia degli aggiornamenti

| Data | Modifica |
|---|---|
| 2026-11-07 | Creazione del registro: 25 issue firmware, 9 hardware, 4 normative, 6 documentazione, 7 prove, 7 chiarimenti. Aggiunti il diagramma funzionale e lo script di controllo. |
| 2026-11-07 | Letti i nuovi documenti del fascicolo (Analisi dei rischi Rev.7, ST.TEC.068, CP.TEC.025 Rev.5, Cap.6). Aggiunte 15 voci (FW-26…FW-31, NOR-05, NOR-06, DOC-07…DOC-09, PRV-08…PRV-10, CHI-08); chiuse NOR-01 e NOR-02; chiarite FW-14, HW-02, HW-07; FW-03 scende a Media e FW-09 passa ad Aperta. Totale 73 voci. |
