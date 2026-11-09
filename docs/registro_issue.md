# Registro delle issue — Elevance / TTF

**Questo è il documento base del lavoro.** Raccoglie tutte le osservazioni, le proposte di correzione e le verifiche aperte sul progetto (firmware TTF-Base e TTF-CX, hardware, documentazione, normativa, prove) e ne segue lo stato fino alla chiusura.

- Posizione: `docs/registro_issue.md` (questo file).
- Analisi di partenza (narrativa, non aggiornata): [`analisi.md`](analisi.md).
- Diagrammi: [`docs/diagrammi/01.schema_funzionale.svg`](diagrammi/01.schema_funzionale.svg) (elettrico, catena delle sicurezze), [`docs/diagrammi/02.diagramma_funzionale.svg`](diagrammi/02.diagramma_funzionale.svg) (funzionale della macchina, con i riquadri rossi che rimandano agli ID di questo registro) e [`docs/diagrammi/03.diagramma_funzionale_light.svg`](diagrammi/03.diagramma_funzionale_light.svg) (versione per il dipartimento commerciale, in linguaggio semplice, senza riferimenti alle issue).
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
| FW-32 | Commento di `battery_charge_wait_discharge` non coerente con le costanti | Bassa | Aperta | Firmware |
| FW-33 | Carica residua prima della corsa: la proposta a tre livelli non è fattibile con il metodo di carica attuale | Bassa | Chiarita | Firmware |
| FW-34 | Avviso sonoro in cabina quando la macchina viaggia senza alimentazione esterna | Bassa | Da decidere | Firmware |
| FW-35 | Pilotaggio serratura (UDEC.CX): PWM a circa 1,2 kHz udibile, eccitazione a gradino al 100% e diseccitazione istantanea | Bassa | Da verificare | Firmware |
| FW-36 | Criterio di partenza basato solo sulla tensione a riposo e sulla caduta della corsa precedente | Bassa | Da decidere | Firmware |
| HW-01 | Spina X8: esclude quasi tutta la catena con un solo ingresso di rilevamento | Media | Da decidere | Hardware |
| HW-02 | Bypass trap/tray comandati solo via software | Media | Chiarita | Hardware |
| HW-03 | Relè K1…K4 non dichiarati a contatti guidati sugli schemi | Media | Da verificare | Hardware |
| HW-04 | `brk_fb` misura la tensione dopo K4, non lo stato meccanico del freno | Media | Da decidere | Hardware |
| HW-05 | Gate dei MOSFET OTR-EN/BRK-EN: pull-down al reset non visibile | Media | Da verificare | Hardware |
| HW-06 | Nodo S3B non letto dalla TTF (ponte X15) | Bassa | Da decidere | Hardware |
| HW-07 | Contattore di uscita motore S22 fuori dalla catena | Media | Chiarita | Hardware |
| HW-08 | Nomenclatura incoerente fra schemi e manuali (K3A/K5A, F1/F7, Q15B/Q16A) | Bassa | Aperta | Hardware |
| HW-09 | Punti da confermare sugli schemi: ponticelli TC/FT, STO sul nodo S4, percorso del pesacarico | Bassa | Da verificare | Hardware |
| HW-10 | Serratura MD2 LOCK molto rumorosa nel sollevamento e nell'abbassamento del catenaccio | Media | Aperta | Hardware |
| NOR-01 | Analisi dei rischi Rev.7: lettura e confronto con il firmware | Alta | Risolta | Normativa |
| NOR-02 | EN 81-41 5.5.7.3: alimentazione di motore e freno da interrompere alla fine del comando | Media | Chiarita | Normativa |
| NOR-03 | Freno sul lato motore con trasmissione a cinghie e monitoraggio della cinghia | Media | Da verificare | Normativa |
| NOR-04 | Giustificazione nell'analisi dei rischi del contatto paracadute e del suo bypass | Bassa | Da verificare | Normativa |
| NOR-05 | Qualificazione di K3 e K4 come «contattori» ai fini di EN 81-41 | Media | Da verificare | Normativa |
| NOR-06 | Ipotesi di 60 arresti dinamici del freno nella vita contro gli eventi che aprono la catena in marcia | Media | Da verificare | Normativa |
| NOR-07 | Obbligo di manutenzione semestrale: fonte normativa e applicabilità a Elevance (DPR 162/1999, Regolamento 2023/1230, EN 81-41) | Media | Da verificare | Normativa |
| DOC-01 | Precisione di fermata: ±20 mm (IM.159) contro ±10 mm (IM.160) | Bassa | Aperta | Documentazione |
| DOC-02 | IM.160 e IM.159: prove duplicate e refusi su fondo e tetto sensibili | Bassa | Aperta | Documentazione |
| DOC-03 | IM.159 §9: porte «chiuse e sbloccate» prima della messa fuori servizio | Bassa | Aperta | Documentazione |
| DOC-04 | IM.161: discesa di emergenza incompleta e tetto sensibile non descritto | Media | Aperta | Documentazione |
| DOC-05 | Note di progetto: numerazione S2/S3 e fusibile F1/F7 | Bassa | Aperta | Documentazione |
| DOC-06 | IM.158: parti ancora da completare (UDEC.C/CX, codici di errore, menu) | Media | Aperta | Documentazione |
| DOC-07 | Definizione di «trap» nell'analisi dei rischi: tetto sensibile o bordi superiori | Bassa | Aperta | Documentazione |
| DOC-08 | Valori di carico non uniformi: 200, 260 e 275 kg | Media | Da verificare | Documentazione |
| DOC-09 | Velocità lineare della cinghia nei calcoli Megadyne (0,2 m/s) contro 0,15 m/s di progetto | Bassa | Da verificare | Documentazione |
| DOC-10 | Diagramma funzionale commerciale: voci da aggiornare quando cambiano le funzioni descritte | Bassa | Aperta | Documentazione |
| DOC-11 | Diagramma energia e batterie: numeri di esempio da sostituire con misure | Bassa | Aperta | Documentazione |
| DOC-12 | Capacità della batteria discordante: 12 Ah, 7,5 Ah, 7,2 Ah | Media | Chiarita | Documentazione |
| DOC-13 | Messaggi commerciali (zero consumi, zero manutenzione, zero intrappolamenti) da allineare ai dati di prodotto | Media | Da verificare | Documentazione |
| DOC-14 | Periodicità della manutenzione: 3 mesi contro 6 mesi | Bassa | Da verificare | Documentazione |
| DOC-15 | ST.TEC.068: massa lineare della cinghia (0,33 contro 3,3 kg/m) e corsa di riferimento | Media | Chiarita | Documentazione |
| DOC-16 | ST.TEC.068: refusi nelle formule e decelerazione di arresto di emergenza da verificare | Bassa | Chiarita | Documentazione |
| DOC-17 | ST.TEC.068 cap.12: taglio sulla guida di ingaggio del paracadute non riproducibile | Bassa | Chiarita | Documentazione |
| DOC-18 | ST.TEC.068: coppia frenante di emergenza 554 Nm contro 654 Nm e verifiche a valle (ingranaggio, linguetta) | Alta | Chiarita | Documentazione |
| DOC-19 | ST.TEC.068 cap.4: frecce del coperchio interpiano non riproducibili | Bassa | Chiarita | Documentazione |
| DOC-20 | ST.TEC.068: carichi su guide e trave superiore non tracciabili, carichi orizzontali non verificati | Bassa | Chiarita | Documentazione |
| DOC-21 | ST.TEC.068 cap.8: origine delle coppie da citare e potenza a 0,15 m/s oltre la targa da confermare con Amer | Bassa | Da verificare | Documentazione |
| DOC-22 | ST.TEC.068: metodo di calcolo della frenata, dei rinvii e della giunzione, refusi residui | Bassa | Chiarita | Documentazione |
| DOC-23 | Manuale di manutenzione IM.TEC.159: lubrificazione e parti di usura in contrasto con «zero manutenzione» | Media | Da verificare | Documentazione |
| DOC-24 | Manuali IM.TEC.159 e 161 e cartelli: residui da altri prodotti, refusi e rimandi errati | Bassa | Da verificare | Documentazione |
| DOC-25 | Certificato IMQ DR 637 DM Rev.0: incoerenze interne e dati da allineare | Bassa | Da verificare | Documentazione |
| DOC-26 | Serratura MD2 LOCK: certificato per porte di piano, impiegata come serratura della porta di cabina | Media | Da verificare | Documentazione |
| DOC-27 | Disegni della serratura MD2 con revisioni posteriori al certificato IMQ CA50.00808 | Media | Da verificare | Documentazione |
| DOC-28 | ST.TEC.068 cap.8: documenti Amer non citati, mozzo e linguetta a carico del cliente, tenuta del freno senza rendimento del riduttore | Bassa | Da verificare | Documentazione |
| DOC-29 | Frenature dinamiche del motoriduttore: 60 in 5 anni contro le prove previste dai manuali | Bassa | Da verificare | Documentazione |
| DOC-30 | Cinghie: deformazione plastica in emergenza con margine 1,26 sul valore medio e limite di 50 eventi senza fonte | Bassa | Da verificare | Documentazione |
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
| PRV-11 | Caduta di tensione del bus in corsa peggiore con batteria scarica e invecchiata | Media | Da verificare | Prove |
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
| DCM-6329 Rev.3 (disegno), rapporto Amer «verifica statica e a fatica» e dati del freno 10 Nm (e-mail Amer 03/03/2026), scheda 7EBLR002 Rev.2.0 | caricati nella conversazione | letti (2026-11-09); F353.01.0029 non letto |
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
  2026-11-09 dal disegno DCM-6329 Rev.3 e dalla scheda 7EBLR002 Rev.2.0 (18/03/2025): connettore Molex Mini-Fit a 8 poli, pin 1 5 V, 3 COS+, 5 0 V, 6 SIN+, 7 SIN−, 8 COS−. Dedotto: 4 periodi per giro motore × 25,3 = 101,2 periodi per giro di uscita, cioè 3,16 mm di cabina per periodo; a 0,15 m/s (28,1 giri/min in uscita, 712 giri/min al motore) la frequenza elettrica è 47 Hz.

### FW-31 · Rampe del firmware diverse dall'ipotesi della relazione di calcolo (0,125 m/s²)
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** ST.TEC.068 §5
- **Dove:** `Inc/application.h` (`VFD_ATIME_FAST = 40`, `VFD_ATIME_SLOW = 10`), `Inc/vfd_inl.h` (`vfd_halt` con rampa 1), `Src/vfd.c`.
- **Problema:** la verifica di strappo del dente in frenata nominale (SF 1,2) usa una decelerazione di −0,125 m/s² «da impostazioni di fabbrica» del regolatore. Il firmware impone rampe proprie: stop di 1 s (circa 0,1 m/s² da 0,10 m/s) e `vfd_halt` di 0,1 s (circa 1 m/s², se l'unità è il decimo di secondo come in `vfd.c`). Con decelerazione maggiore il margine scende; non ho ricalcolato.
- **Proposta:** rifare la verifica con le rampe reali di stop e di halt, oppure limitare la rampa di halt.
- **Chiusura:** relazione ricalcolata o rampe adeguate.
- **Storia:** 2026-11-07 aperta.

---

## Hardware e architettura

### FW-32 · Commento di `battery_charge_wait_discharge` non coerente con le costanti
**Gravità:** Bassa · **Stato:** Aperta · **Fonte:** lettura di `battery.c` e `battery.h`
- **Dove:** `battery.c` `battery_charge_wait_discharge` e `battery.h` `BAT_BOOST_START_CHARGE`.
- **Problema:** il commento dice che si passa in carica completa sotto il 50% (3,6 Ah consumati), il codice usa -2,0 Ah. Un commento obsoleto o una soglia cambiata senza aggiornarlo.
- **Proposta:** allineare il commento (nessuna modifica di comportamento) quando si toccherà il file; da decidere insieme a DOC-12.
- **Chiusura:** commento e costante coerenti.
- **Storia:** 2026-11-07 aperta.

### FW-33 · Carica residua prima della corsa: la proposta a tre livelli non è fattibile con il metodo di carica attuale
**Gravità:** Bassa · **Stato:** Chiarita · **Fonte:** lettura di `vfd.c`, `pmode.c`, `battery.c`, `run_mode.c`; risposta del tecnico del firmware del 2026-11-09
- **Dove:** `vfd.c:152-165` `vfd_is_ready` (DC link > `VFD_DCLINK_MIN` = 38 V), `pmode.c:121-135` (in DRIVE, se il VFD non è pronto torna a TO_DRIVE), `battery.c:256-262` (flag `low_battery` a 38,4 V sotto carico), `run_mode.c:524` (fuori servizio solo se la macchina è al piano).
- **Verificato:** a batteria scarica la macchina non entra in movimento (DC link ≤ 38 V) e, se `low_battery` è attivo al piano, resta fuori servizio con la sola riapertura porta.
- **Problema (dedotto, da verificare con prova):** la decisione di partire guarda solo la tensione istantanea, non la carica residua rispetto al tratto da percorrere (il contatore di carica serve solo alla ricarica). Una corsa iniziata con tensione appena sopra 38 V può, per la caduta sotto carico, far uscire il sistema da DRIVE a metà corsa e fermare la cabina fra i piani; la ripartenza dipende dal recupero della tensione a riposo. Le soglie di avvio (38 V) e di fuori servizio (38,4 V) sono quasi coincidenti. Non verificato il livello di sottotensione del VFD.
- **Proposta (richiesta dell'utente del 2026-11-08, non applicata):** avviare una corsa solo se la carica misurata basta per almeno una corsa più l'apertura porte; sotto una soglia di riserva consentire un'ultima corsa, aprire le porte e restare fuori servizio al piano. Schema proposto, con tre livelli calcolati da `battery.charge` (Ah consumati dall'ultima carica completa; 12 Ah di capacità confermati) e dalla tensione:
  - *Normale:* carica residua sopra la soglia S1. Comportamento attuale.
  - *Riserva* (tra S2 e S1): accettare una sola chiamata; a fine corsa porte aperte (apriporta come oggi) e macchina fuori servizio fino alla ricarica. Si può riusare il percorso già esistente di `low_battery` in `run_mode.c:524`, aggiungendo un latch «ultima corsa consumata».
  - *Esaurita* (sotto S2): non si parte; fuori servizio al piano con la sola riapertura porta, come oggi.
  - *Come tarare S2 e S1:* S2 = carica in Ah per la peggiore corsa (4,5 m, 200 kg) più apertura porte più margine; S1 = S2 più una corsa di margine. Il dato va misurato in prova (la stima a tavolino è di circa 6 Wh per salita peggiore, cioè circa 0,13 Ah a 48 V, con rendimento ipotizzato). Ripristino: si esce dal fuori servizio con la logica già presente (`battery.c:189`, corrente di carica sotto 2,8 A in carica completa).
  - *Limite del contatore:* `battery.charge` è relativo all'ultima carica completa e con batteria invecchiata sovrastima la carica disponibile. Affiancare un criterio sulla tensione: memorizzare la caduta massima della corsa precedente e non partire se la tensione a riposo meno quella caduta scende sotto `VFD_DCLINK_MIN` più un margine (da valutare).
  - *Cosa non toccare:* catena di sicurezza e freno. La modifica è solo nella logica di abilitazione delle chiamate.
  - *Richiede:* aggiornare l'analisi dei rischi (comportamento a batteria scarica) e provare: corsa peggiore a batteria quasi scarica, caduta di tensione e punto di intervento del VFD, ripristino dopo la ricarica.
- **Chiusura:** soglie misurate, logica approvata, implementata e provata a batteria scarica; comportamento documentato nel fascicolo.
- **Storia:** 2026-11-08 aperta. 2026-11-08 proposta di soluzione a tre livelli su richiesta dell'utente; in attesa di approvazione.
  2026-11-09 risposta del tecnico (L. Marcantonio): la proposta a tre livelli è **non fattibile** perché (1) la carica necessaria dipende anche dal carico di cabina; (2) una batteria al piombo si considera scarica sulla tensione, non sui mAh stimati; (3) al collegamento della batteria il power meter indica «carica completa» anche se è vuota, e c'è una deriva del 10% o più a ogni ciclo di rabbocco; (4) `battery.charge` non è significativo se il caricabatteria non è calibrato. Il metodo di carica oggi parte sul livello di carica e termina sulla corrente assorbita, con carica completa rapida solo ogni 10 scariche maggiori del 10% e terminata poco sopra il nominale (metodo a oscillazione, lo stesso degli UPS OVA/Aros). Il requisito funzionale è che anche a batteria scarica la corsa si completi; i 38 V sono la soglia assoluta di partenza per non degradare la batteria. Correzioni alle mie deduzioni: lo stato `TO_DRIVE` è lo stato di riabilitazione del VFD e serve a recuperare da altre anomalie di `vfd_is_ready` (fra cui il fault 8), non a fermare la cabina fra i piani; 38 V (tensione vista dal VFD, troncata a intero) e 38,4 V (power meter) sono misure diverse con tolleranze grandi, quindi non «quasi coincidenti». Nessuna modifica proposta.

### FW-34 · Avviso sonoro in cabina quando la macchina viaggia senza alimentazione esterna
**Gravità:** Bassa · **Stato:** Da decidere · **Fonte:** richiesta dell'utente del 2026-11-08; lettura di `npb.c`, `npb.h`, `udec_c.h`
- **Dove:** `npb.c` (`npb.online` va a falso se il caricabatteria non risponde, «probabilmente manca la tensione di rete»; il bit `NPB_FLT_AC_FAIL` è definito in `npb.h` ma non risulta usato), `udec_c.h` (uscite UDEC.C con `hl_buz` «Buzzer su scheda» mai scritto dal firmware della base), scheda `UDEC.CX/Audio.kicad_sch`.
- **Problema:** oggi chi è in cabina non sa che la corsa avviene a batteria. L'utente propone un avviso sonoro durante la marcia senza rete.
- **Proposta (non applicata):** definire «rete assente» come `npb.online` falso o `AC_FAIL` attivo per più di alcuni secondi (con ritardo di conferma per evitare falsi allarmi); durante la marcia, e con schema diverso nello stato di riserva di FW-33, comandare un bip intermittente. Sul mezzo da usare: verificare nel firmware di UDEC.C cosa fa il bit `hl_buz` e se il buzzer è montato, oppure usare l'audio di UDEC.CX; da verificare anche se il caricabatteria risponde ancora o segnala `AC_FAIL` quando manca la rete. Prove: staccare la rete e misurare il tempo di rilevamento; verifica acustica in cabina.
- **Chiusura:** segnale definito, implementato e provato; descritto nel manuale d'uso.
- **Storia:** 2026-11-08 aperta.
  2026-11-09 risposta del tecnico: «può avere senso, è un perfezionamento»; per specifica la macchina deve fare 10 corse senza alimentazione (verificato, circa una giornata intera di prove prima di andare in errore). Correzioni alle mie ipotesi: il caricabatteria a rete mancante si spegne del tutto e `AC_FAIL` dura troppo poco per essere rilevato; la rete assente si riconosce dalla presenza degli ausiliari (punto del codice da indicare); il bit `hl_buz` è già usato dal firmware: `overload_lamp()` in `udec_c_inl.h` lo attiva insieme alla lampada di sovraccarico (mia affermazione «mai usato» sbagliata, la ricerca non aveva incluso gli header). Basta quindi comandare `hl_buz` con uno schema a impulsi durante la marcia senza rete.
  2026-11-09 dal tecnico: il rilevamento della mancanza rete richiede circa 100-200 ms (non misurato con precisione e dipendente dal consumo della macchina): il ritardo di conferma dell'avviso va scelto sopra questo valore.

### FW-35 · Pilotaggio serratura (UDEC.CX): PWM a circa 1,2 kHz udibile, eccitazione a gradino al 100% e diseccitazione istantanea
**Gravità:** Bassa · **Stato:** Da verificare · **Fonte:** lettura di `UDEC.CX/CubeMX/Core/Src/application.c` e `main.c`; segnalazione di rumorosità dell'utente (HW-10)
- **Dove:** `UDEC.CX/CubeMX/Core/Src/application.c:153-188` (`outputs_scan`, gestione elettroserratura), `main.c:172` (timer 100 ms `TMR_ID_ELT_PWM`), `main.c:717-722` e `UDEC.CX.ioc` (TIM4: prescaler 0, periodo 65535, clock timer 80 MHz).
- **Verificato nel codice:** quando `elt` è attivo e la scheda base è online, la bobina riceve il 100% di duty per i primi 100 ms (sblocco), poi il 30% circa (`0x4CC2`, mantenimento); quando `elt` è disattivo il duty va a 0 subito. Il commento nel codice dice «da calcolare Hz effettivi»: con questi parametri la frequenza di PWM è 80 MHz / 65536, cioè circa 1,22 kHz, in banda udibile.
- **Problema (dedotto, da provare):** il passo di tensione al 100% sul solenoide può far battere il nucleo a fine corsa; la diseccitazione istantanea lascia alle molle della serratura tutta la spinta; il PWM a 1,2 kHz può produrre un ronzio in mantenimento. Il duty di mantenimento del 30% non è verificato con margine alle vibrazioni.
- **Proposta (non applicata):** a banco, misurare corrente e rumore (sale o scende, ON o OFF); poi valutare nel firmware di UDEC.CX: PWM a 20 kHz o più (per esempio periodo 3999 con prescaler 0, ricalcolando i duty), eccitazione progressiva invece del gradino, rampa di diseccitazione dal mantenimento a zero in poche decine di millisecondi, e verifica del mantenimento al 30% con margine. Il blocco in assenza di alimentazione resta garantito dalle molle e non dipende dal firmware.
- **Chiusura:** misure di rumore prima e dopo, mantenimento verificato, nessuna modifica alla funzione di blocco.
- **Storia:** 2026-11-08 aperta.
  2026-11-08 l'utente esclude il ronzio (il rumore è uno schianto in battuta, identico in apertura e in chiusura) e segnala che ridurre tensioni o modo di azionamento rischia di non far funzionare la serratura: gravità abbassata a Bassa; la causa principale è meccanica (HW-10).

### FW-36 · Criterio di partenza basato solo sulla tensione a riposo e sulla caduta della corsa precedente
**Gravità:** Bassa · **Stato:** Da decidere · **Fonte:** risposta del tecnico del firmware del 2026-11-09 a FW-33; lettura di `vfd.c` e `battery.c`
- **Dove:** `vfd.c:152-165` (`vfd_is_ready`, DC link > 38 V), `battery.c` (flag `low_battery`).
- **Problema:** il tecnico respinge i livelli basati sul contatore di carica, che non è affidabile. Non ha commentato la parte basata solo sulla tensione della mia proposta (non partire se la tensione a riposo meno la caduta massima della corsa precedente scende sotto la soglia del VFD più un margine). Questo criterio non usa mAh e potrebbe ridurre le fermate a metà corsa; per il tecnico la caduta sotto carico è già il parametro con cui si giudica una batteria (stima 6-7 V). Non è detto che sia compatibile con il requisito «anche a batteria scarica la corsa si completa».
- **Proposta (non applicata):** chiedere al tecnico se il criterio basato solo sulla tensione è utilizzabile, con quale margine e con quale comportamento al piano (per esempio fuori servizio con riapertura porte, come con `low_battery`). Da decidere dopo i dati di PRV-11.
- **Chiusura:** decisione presa con i dati misurati e, se approvata, implementata e provata.
- **Storia:** 2026-11-09 aperta.

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

### HW-10 · Serratura MD2 LOCK molto rumorosa nel sollevamento e nell'abbassamento del catenaccio
**Gravità:** Media · **Stato:** Aperta · **Fonte:** segnalazione dell'utente del 2026-11-08; certificato IMQ CA50.00808 Rev.0; `door.c`
- **Dove:** serratura MD2 LOCK (disegno F353.02.0080), bobina a 24 V c.c., 1,9 A, comandata dall'uscita `ELT` (`udec_cx.out.elt`); ciclo porta in `door.c` (bobina eccitata = sbloccata, diseccitata = bloccata dalla molla).
- **Problema:** rumore forte a ogni sollevamento e abbassamento del catenaccio (segnalato dall'utente; non misurato e non analizzato nel progetto). Il prodotto è residenziale e ha un obiettivo di silenziosità dichiarato per la sala macchine (CP.TEC.025, non superiore a 40 dB): per la serratura non c'è un obiettivo scritto. Ipotesi, non verificate: urto del nucleo mobile a fine corsa a ogni eccitazione, caduta del catenaccio sotto la molla senza smorzamento alla diseccitazione, urto del catenaccio nel foro del battente, risonanza del telaio della porta.
- **Proposta:** misurare il rumore (dB(A) a 1 m, registrazione) e isolare la sorgente con prove a banco e montata; poi valutare, in ordine, rimedi meccanici (battute elastiche, bussole smorzanti, molla più dolce, corsa ridotta, lubrificazione), elettrici (eccitazione progressiva e corrente di mantenimento ridotta, diseccitazione graduale, da verificare sullo schema di UDEC.C/UDEC.CX) e di sequenza nel firmware. Ogni modifica a un componente di sicurezza certificato va concordata con IMQ; nessuna modifica senza richiesta.
- **Chiusura:** obiettivo di rumorosità fissato, rimedio provato e misurato, ammissibilità verificata con IMQ.
- **Storia:** 2026-11-08 aperta.
  2026-11-08 ulteriori risposte dell'utente: nessun ronzio; il rumore è uno schianto all'arrivo in battuta, praticamente identico in apertura e in chiusura; ridurre le tensioni o il modo di azionamento rischia di compromettere il funzionamento. Direzione: smorzamento meccanico delle due battute (elastomero, bussole, massa mobile ridotta, supporti elastici del corpo serratura) preservando traferro, forza di attrazione e profondità di ingaggio del catenaccio; eventuale prova elettrica solo con margini misurati (tensione minima, bobina calda). Ogni modifica alla MD2 LOCK da concordare con IMQ.
  2026-11-08 risposte dell'utente: attuatore a solenoide senza molla di ritorno (le molle sono nella serratura) e rumore presente anche a banco, quindi la sorgente è interna alla serratura e non dipende dalla porta. Il pilotaggio nel firmware di UDEC.CX è riportato in FW-35.
  2026-11-08 domanda dell'utente sull'uso di un motore al posto del solenoide, con il vincolo che a bobina diseccitata la serratura deve chiudersi da sola con le molle: valutato come possibile ma non come prima scelta. Servirebbe un meccanismo che scolleghi il motore dal catenaccio a motore spento (motore retro-azionabile o frizione), la corrente di mantenimento in sblocco sarebbe a motore bloccato, e cambierebbe il tipo certificato: nuovo esame IMQ (EN 81-20:2020 ed EN 81-50:2020, da leggere sui testi). Prima via: smorzamento meccanico delle battute.
  2026-11-08 proposta dell'utente: micro motore con rotazione di un quarto di giro e camma che non supera il punto morto (nessuna posizione bistabile), con ritorno a molla. Valutata come promettente se: il motore è retro-azionabile (rapporto basso o accoppiamento che si stacca a motore spento), la camma resta sotto il punto morto, lo sblocco è tenuto con corrente a motore fermo (PWM ridotta), l'arrivo in battuta in sblocco è a velocità controllata e il ritorno a molla può essere frenato in corto circuito sugli avvolgimenti (smorzamento proporzionale alla velocità, non necessario per la sicurezza). Alternativa di serie da considerare: attuatore rotativo a 90° con ritorno a molla. Resta da vedere con IMQ se è una modifica o un nuovo tipo.
  2026-11-08 ricerca di opzioni in commercio (fonti secondarie, schede non verificate): attuatori rotativi monostabili con ritorno a molla a 24 V e rotazione di 45° o 95° (famiglia Kendrion/Kuhnke D, coppie piccole da verificare; il Kendrion CDR a 90° è bistabile e quindi non adatto); serrature per porte di piano con attuazione a motore e molla di ritorno nel catenaccio (cataloghi Kronenberg serie MO; interblocchi Schmersal AV e Pizzato FG certificati EN 81-20/-50); micro motoriduttori di catalogo (non cercati modelli); articoli di settore su serrature silenziose con smorzatore o senza solenoide. Nessuna fonte letta conferma il testo esatto della norma sul ritorno a molla a diseccitazione: da leggere in EN 81-20:2020 ed EN 81-50.
  2026-11-09 letti i disegni dell'assieme F353.02.0080 e dei componenti (11 PDF su Notion). Meccanismo: solenoide Landa SQ1250L-24A16 (scheda non trovata) azionato sul bilanciere F353.05.5285, che solleva la staffa perni con due perni Ø18/Ø15 da 0,10 kg ciascuno contro due molle a compressione (filo 0,9 mm, Dint 17,1, lunghezza libera 35, 4 spire, 0,28-0,37 N/mm circa ciascuna; stima da verificare). Dal disegno: corsa stimata circa 20 mm, molle compresse a circa 12 mm nello stato sbloccato (circa 13-17 N in totale) e quasi libere nello stato bloccato, massa mobile circa 0,3 kg, energia delle molle circa 0,15 J e velocità d'urto stimata attorno a 1 m/s (ipotesi, da misurare). Sono già presenti due piedini PVC da 2 mm (Essentra 50493) come battute e lo sblocco finisce sull'arresto interno del solenoide; il corpo è in lamiera zincata sottile. Attuatori rotativi di catalogo trovati sono troppo deboli per questa forza (coppia richiesta circa 0,15-0,3 N·m); un motore dovrebbe essere un motoriduttore e difficilmente retro-azionabile dalle molle: da ridurre prima la forza delle molle. Non copiati disegni nel repository.

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

### NOR-07 · Obbligo di manutenzione semestrale: fonte normativa e applicabilità a Elevance (DPR 162/1999, Regolamento 2023/1230, EN 81-41)
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** domanda dell'utente del 2026-11-08; ricerca su Normattiva (DPR 162/1999 art. 1, 12, 15), EUR-Lex (Regolamento (UE) 2023/1230), CP.TEC.025 Rev.5, IM.TEC.159
- **Dove:** claim «manutenzione obbligatoria, verifica semestrale in Europa»; IM.TEC.159 §10.06 («periodicità consigliata»); CP.TEC.025 §9b (semestrale).
- **Verificato (letture parziali, vedi limiti):**
  - Il Regolamento (UE) 2023/1230 (Macchine, sostituisce la direttiva 2006/42/CE dal gennaio 2027) è una norma di prodotto: obbliga il fabbricante a fornire istruzioni con tipi e frequenze di ispezioni e manutenzioni necessarie per la sicurezza (Allegato III, 1.3.2 e 1.7.4.2). Nelle parti lette non impone ispezioni periodiche a proprietario o utilizzatore.
  - La EN 81-41 (edizione 2024 in vigore) è una norma tecnica armonizzata: non crea un obbligo di legge di manutenzione per il proprietario.
  - Il DPR 162/1999 art. 15 (Italia) impone al proprietario di affidare la manutenzione a persona abilitata o ditta specializzata, con controlli di sicurezza almeno ogni sei mesi per gli ascensori (comma 4). L'art. 1 comma 3 lett. a) esclude dal regolamento di prodotto gli apparecchi con velocità non superiore a 0,15 m/s, ma gli art. 12 e 15 citano espressamente gli «apparecchi di sollevamento rispondenti alla definizione di ascensore la cui velocità di spostamento non supera 0,15 m/s». Il DPR prevede inoltre verifiche periodiche biennali (art. 13, secondo fonti secondarie).
- **Certificato IMQ (2026-11-08):** certificato di esame CE del tipo n. IMQ DR 637 DM Rev.0, Allegato IX della direttiva 2006/42/CE, organismo notificato 0051, emesso il 2026-07-15 e valido fino al 2031-07-14, per «piattaforma elevatrice» LiftingItalia Elevance, luogo di installazione «edifici privati». Questa classificazione riguarda la messa sul mercato del prodotto (direttiva macchine) e non esenta da sola dagli obblighi di esercizio nazionali: il DPR 162/1999 art. 12 e 15 citano espressamente gli apparecchi di sollevamento rispondenti alla definizione di ascensore con velocità non superiore a 0,15 m/s; una nota ANCE elenca come eccezioni miniere, navi, corsa inferiore a 2 m, azionamento manuale e installazioni non stabili (Elevance ha corsa da 2,5 a 4,5 m). Il Ministero ha chiarito (lettera circolare MiSE 17595/08, secondo ATS Brianza) che gli organismi notificati possono fare le verifiche biennali anche su piattaforme per disabili, che seguono la circolare MICA 157296/1997.
- **Non verificato:** se Elevance rientra nella «definizione di ascensore» (art. 2 DPR 162/1999) e quindi negli obblighi di manutenzione semestrale e di verifica biennale; le norme degli altri Paesi europei (non esiste un obbligo semestrale unico UE: l'esercizio è materia nazionale); il testo integrale degli articoli e degli allegati (letture automatiche di Normattiva e EUR-Lex, parziali); la data esatta di applicazione del Regolamento 2023/1230 (fonti EU-OSHA e tekom: 20 gennaio 2027; la lettura automatica di EUR-Lex ha dato 14 gennaio 2027).
- **Problema:** il messaggio «zero manutenzione» va formulato tenendo conto che, se il DPR 162/1999 si applica, la manutenzione periodica a cura di ditta abilitata è obbligatoria; la periodicità di 6 mesi in CP.TEC.025 e IM.TEC.159 («consigliata») andrebbe presentata come obbligo dove applicabile; il manuale d'uso dice almeno 3 mesi (DOC-14).
- **Proposta:** far confermare da un organismo notificato, dall'ASL o da un consulente legale l'applicabilità del DPR 162/1999 a Elevance e le regole dei mercati di vendita previsti (UK, Francia, Benelux, Svizzera, Iberica, Far East, Australia); riportare nei manuali la base normativa della periodicità.
- **Chiusura:** parere scritto sull'applicabilità e manuali allineati.
- **Storia:** 2026-11-08 aperta.

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
- **Dove:** portata 200 kg (CP.TEC.025): conforme al carico unitario minimo di 250 kg/m² di EN 81-41, perché la cabina Ø1000 mm ha 0,785 m² e 250 × 0,785 = 196 kg (ST.TEC.068: «portata nominale max 200 kg»; la nota «minimo 250 kg, deroga dichiarata» scritta all'apertura era un mio errore); carico statico massimo con sovraccarico 260 kg (ST.TEC.068); prova di sovraccarico a 275 kg (IM.160) e «massa persone 275 kg» nel calcolo Megadyne.
- **Problema:** la prova non discrimina la soglia di intervento del pesacarico; la soglia effettiva non è scritta nei documenti letti.
- **Proposta:** definire la soglia di intervento in kg e allineare prova e calcoli.
- **Chiusura:** documenti coerenti; prova di sovraccarico alla soglia definita.
- **Storia:** 2026-11-07 aperta. 2026-11-07 corretta la lettura della portata: 200 kg su 0,785 m² sono conformi a 250 kg/m² (segnalazione dell'utente); restano da allineare soglia del pesacarico, prova a 275 kg e calcolo Megadyne.

### DOC-09 · Velocità lineare della cinghia nei calcoli Megadyne (0,2 m/s) contro 0,15 m/s di progetto
**Gravità:** Bassa · **Stato:** Da verificare · **Fonte:** MFC 20260211-1-Rev0
- **Dove:** foglio di calcolo cinghia, riga «velocità lineare cinghia 0,2 m/s» con «omega motrice 28 rpm».
- **Problema:** con puleggia Ø101,86 mm i 28 rpm corrispondono a 0,149 m/s; il valore 0,2 m/s non è coerente con la riga accanto né con il limite 0,15 m/s.
- **Proposta:** chiedere conferma a Megadyne e correggere il calcolo se serve.
- **Chiusura:** calcolo coerente.
- **Storia:** 2026-11-07 aperta.

### DOC-10 · Diagramma funzionale commerciale: voci da aggiornare quando cambiano le funzioni descritte
**Gravità:** Bassa · **Stato:** Aperta · **Fonte:** richiesta del 2026-11-07
- **Dove:** `docs/diagrammi/03.diagramma_funzionale_light.svg` e `.png`, generati da `docs/gen_diagramma_light.py`.
- **Problema:** il diagramma per il dipartimento commerciale descrive la macchina in linguaggio semplice e non cita le issue. Alcune funzioni sono descritte come «prevista» o «in definizione» (modalità manutenzione, ripartenza in discesa dopo il tetto sensibile, funzioni di servizio dell'analisi dei rischi): diventano vere o false in base a FW-06, FW-12, FW-26 e FW-27. Altre voci (servizi connessi, dati di prodotto) vengono dalla configurazione CP.TEC.025 Rev.5 e non dal firmware.
- **Proposta:** a ogni chiusura di FW-06, FW-12, FW-26, FW-27 aggiornare il riquadro «In sviluppo» e gli stati «Fermata per ostacolo» e «Blocco di sicurezza»; far rivedere il documento a R&D prima di diffonderlo.
- **Chiusura:** revisione di R&D registrata e diagramma allineato alle funzioni correnti.
- **Storia:** 2026-11-07 aperta con la prima versione del diagramma.

### DOC-11 · Diagramma energia e batterie: numeri di esempio da sostituire con misure
**Gravità:** Bassa · **Stato:** Aperta · **Fonte:** richiesta del 2026-11-07
- **Dove:** `docs/diagrammi/04.diagramma_energia_light.svg` e `.png`, generati da `docs/gen_diagramma_energia.py`; versione sintetica e visuale `docs/diagrammi/05.diagramma_energia_sintesi.svg` e `.png`, generata da `docs/gen_diagramma_energia_sintesi.py` (stessi dati e ipotesi; da tenere o eliminare dopo la valutazione dell'utente).
- **Problema:** il diagramma per il commerciale spiega ciclo della batteria, contrappesi, autonomia, giornata tipica e costi. I consumi sono un ESEMPIO calcolato dalle masse (cabina 160 kg, contrappesi 2×30 kg) con ipotesi dichiarate: corsa 3 m, 0,10 m/s, rendimenti 60% e 80%, 5 salite e 5 discese al giorno, stand-by 1 W, 0,25 €/kWh. Il recupero di energia in discesa è volutamente omesso. Verifica del 2026-11-07: dati di prodotto (10 corse di emergenza, 230 V 2 A 0,5 kW, 0,10/0,15 m/s, 160 kg, 2×30 kg, 3 anni/6 mesi) confermati da CP.TEC.025 Rev.5 e ST.TEC.068. Capacità batteria 12 Ah confermata (DOC-12). Incerti: stand-by 1 W «da certificare» (non include l'elettronica a batteria, comunicazione, luci); rendimenti 60% e 80% ipotizzati; consumi di freno, VFD, porte non inclusi (costi = limite inferiore); tempi di rabbocco e carica completa calcolati, non misurati; 10 corse di emergenza non verificate da prova (il manuale d'uso dice «numero limitato»); comportamento in discesa non valutato (i commenti del firmware prevedono assorbimento di energia rigenerata e c'è una protezione a 63 V). Rimossi dal diagramma il «fusibile 100 A» e la protezione da inversione di polarità, non trovati nei documenti.
- **Proposta:** misurare Wh per corsa (salita/discesa, vuota/carico) e stand-by, verificare il tempo di rabbocco e di carica completa, confermare la capacità della batteria; poi sostituire gli esempi con misure, togliere le diciture «stima» e allinearlo al calcolo ufficiale di classe energetica se esiste. Rivedere con R&D prima di diffonderlo.
- **Chiusura:** valori misurati e diagramma allineato; revisione R&D registrata.
- **Storia:** 2026-11-07 aperta con la prima versione del diagramma.
  2026-11-09 dal tecnico del firmware: le 10 corse senza rete sono state provate (circa una giornata di prove prima dell'errore, una sola volta, senza misure); la capacità disponibile a scarica rapida è circa 7,5 Ah (360 Wh) contro 12 Ah (576 Wh) a scarica lenta; tempi di ricarica indicati dal tecnico: 10-15 minuti per tornare «utilizzabile», 4-8 ore teorici per la completa, non misurati. Diagrammi 04 e 05 allineati.

### DOC-12 · Capacità della batteria discordante: 12 Ah, 7,5 Ah, 7,2 Ah
**Gravità:** Media · **Stato:** Chiarita · **Fonte:** schema `Schema Macchina/Cappello.kicad_sch`, `battery.h`, `battery.c`
- **Dove:** schema elettrico, simbolo batteria «48V 12Ah»; `battery.h` `BATTERY_DISCHARGED_MAH = 7500` («capacità nominale»); commento in `battery.c` (`battery_charge_wait_discharge`): «sotto il 50% (3.6 Ah consumati)» che implica circa 7,2 Ah. Massa batterie 18 kg (ST.TEC.068). CP.TEC.025 non indica la capacità.
- **Problema:** tre valori diversi. Se la capacità reale è 12 Ah il contatore viene azzerato su «scarica» a 7,5 Ah consumati (circa 62%), se è 7,5 Ah il margine è nullo. Cambia l'autonomia in blackout e le soglie di ricarica.
- **Proposta:** leggere modello e capacità sulla batteria installata e sul datasheet; allineare schema, costante e commento; verificare l'autonomia con la prova delle 10 corse di emergenza.
- **Chiusura:** capacità confermata e documenti allineati.
- **Storia:** 2026-11-07 aperta durante la verifica dei dati del diagramma energia. 2026-11-07 chiarita: l'utente conferma 12 Ah (valore dello schema). Restano da allineare `BATTERY_DISCHARGED_MAH = 7500` (è circa il 62% di 12 Ah: verificare che sia la scelta voluta per il reset del contatore a batteria scarica) e il commento obsoleto (FW-32); nessuna modifica al firmware senza richiesta.
  2026-11-09 risposta del tecnico: `BATTERY_DISCHARGED_MAH` = 7500 è la capacità nominale a scarica rapida (C/1); i 12 Ah sono il nominale a scarica lenta (C/20): tutti valori indicativi. Il conteggio del 62% che avevo ricavato non è quindi un'incoerenza ma due condizioni di scarica diverse; resta il commento obsoleto in `battery.c` (FW-32).

### DOC-13 · Messaggi commerciali (zero consumi, zero manutenzione, zero intrappolamenti) da allineare ai dati di prodotto
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** indicazioni dell'utente del 2026-11-08; CP.TEC.025 Rev.5; manuale di manutenzione
- **Dove:** appunti in `docs/commerciale.md` (USP e brevetti forniti dall'utente).
- **Problema:** «zero consumi» contro 1 W di stand-by da certificare e 0,5 kW in ricarica; «zero manutenzione, nessun consumabile» contro manutenzione semestrale e batterie da sostituire ogni 3 anni; «zero intrappolamenti» non è dimostrabile: l'Analisi dei rischi Rev.7 elenca scenari residui di intrappolamento (porte bloccate, guasto allo sblocco, comandi non coerenti, valutati remoti/occasionali) e a batteria scarica la corsa può fermarsi fra i piani (FW-33); è difendibile «non parte con la batteria scarica» (verificato) e «ritorno al piano a batteria in blackout» (dato di prodotto, da provare). L'elenco dei brevetti è diverso da quello di CP.TEC.025 (paracadute attivo dal 2021, modello estetico cabina registrato, modello azionamento contrappesi e batterie depositato).
- **Proposta:** riformulare le USP con valori verificabili (consumo certificato, intervalli di controllo, numero di corse di emergenza provato) e riconciliare numeri e stato dei titoli di proprietà industriale con l'ufficio brevetti.
- **Chiusura:** USP e titoli approvati dal commerciale e coerenti con i documenti di prodotto.
- **Storia:** 2026-11-08 aperta.

### DOC-14 · Periodicità della manutenzione: 3 mesi contro 6 mesi
**Gravità:** Bassa · **Stato:** Da verificare · **Fonte:** manuale d'uso IM.TEC (manutenzione «almeno ogni tre mesi»), CP.TEC.025 Rev.5 e manuale di manutenzione (semestrale)
- **Dove:** manuale d'uso, paragrafo sulla manutenzione periodica; CP.TEC.025 §9b; manuale di manutenzione, tabella al §10.06 (nel testo richiamata come «§ 9.06»).
- **Problema:** il manuale d'uso prescrive almeno ogni tre mesi, la configurazione di prodotto e il manuale di manutenzione sei mesi.
- **Proposta:** decidere un solo intervallo e allineare i tre documenti prima di usarlo in comunicazioni commerciali.
- **Chiusura:** intervallo unico nei documenti.
- **Storia:** 2026-11-08 aperta.

### DOC-15 · ST.TEC.068: massa lineare della cinghia (0,33 contro 3,3 kg/m) e corsa di riferimento
**Gravità:** Media · **Stato:** Chiarita · **Fonte:** ST.TEC.068 Rev.0 (pagine 22-28), revisione del 2026-11-08
- **Dove:** specifica cinghia «Massa = 0.33 kg/m» e nota di esclusione («m_b ≤ 3,3 kg per linea, meno dell'1,5%»); verifica allentamento cinghie «Massa lineare (Megalinear RPP8 50) = 3.3 kg/m, 6 m = ~20 kg»; elenco masse «Massa cinghie = 50 kg» (e «35 kg cinghie» nella verifica guide); Tab.1 «Corsa 3000 mm» con la nota «parametri massimi accettabili» (corsa massima 4500 mm).
- **Problema:** i due valori di massa lineare differiscono di un fattore 10. Con 3,3 kg/m i 50 kg totali di cinghie sono coerenti, ma la massa di una linea sarebbe ben oltre il 1,5% e non trascurabile; inoltre lo squilibrio di massa fra ramo cabina e ramo contrappeso varierebbe con la posizione (ordine di ±15 kg per linea sulla corsa massima) e andrebbe considerato nelle verifiche e nei consumi. Anche 50 e 35 kg non coincidono. La corsa di riferimento è 3000 mm, non la massima.
- **Proposta:** confermare la massa lineare dalla scheda tecnica Megadyne; correggere il valore errato e rivedere la nota di esclusione e le verifiche che ne dipendono; esplicitare quali verifiche dipendono dalla corsa e per quali si è assunta la corsa massima.
- **Chiusura:** valori coerenti in tutto il documento e verifiche riviste se serve.
- **Storia:** 2026-11-08 aperta.
  2026-11-09 verifica geometrica (dedotta; la scheda Megadyne non è nei documenti letti né sul web): una cinghia da 50 mm di sezione circa 275 mm² pesa circa 0,33 kg/m; 3,3 kg/m richiederebbe una sezione dieci volte maggiore. Il valore giusto è quindi 0,33 kg/m (specifica e nota di esclusione), e «3.3 kg/m, 6 × 3,3 = 20 kg» è un refuso (2 kg). Le masse totali di 50 e 35 kg negli elenchi non sono coerenti con 0,33 kg/m (corrisponderebbero a 150 e 106 m di cinghia): da ricalcolare dallo sviluppo reale; i carichi sotto le guide con 35 kg restano a favore di sicurezza. Tab.1: le verifiche usano la corsa massima 4500 mm, quindi la tabella deve dichiararla. Testi in `docs/correzioni_ST.TEC.068.md` §6. Da confermare: scheda Megadyne (o pesata di uno spezzone) e sviluppo reale delle cinghie. Il progettista aveva chiesto di partire da 3,3 kg/m; il controllo geometrico lo smentisce.
  2026-11-09 il progettista conferma 0,33 kg/m. Resta da fornire lo sviluppo reale delle due cinghie per ricalcolare la massa totale (§1.1).
  2026-11-09 chiusa dal progettista: correzioni alla relazione applicate a sua discrezione, secondo l'elenco `docs/elenco_correzioni_ST.TEC.068.md`; i dati ancora mancanti restano segnaposto per la Rev.1.

### DOC-16 · ST.TEC.068: refusi nelle formule e decelerazione di arresto di emergenza da verificare
**Gravità:** Bassa · **Stato:** Chiarita · **Fonte:** ST.TEC.068 Rev.0 (pagina 26-27), revisione del 2026-11-08
- **Dove:** «Sstop = V²/2·Amax = 0.15/14.5 = 0.77 mm» (la formula scritta dà un altro risultato, il valore 0,77 mm è invece corretto come V²/(2·Amax)); «Cs = g·(Qsl + P − Mcwt) + Rp» con valori calcolati su «2·30» e moltiplicati per Rp (il «+» è un «·»); Amax = 14,5 m/s² (circa 1,5 g) in frenata di emergenza con i due freni a coppia massima.
- **Problema:** i risultati numerici controllati (SF 15,7; 10,4; 3,8; 1,4; 554 Nm; 180 Nm; 14,5 m/s²) sono corretti, ma le formule stampate hanno refusi. Non è stato verificato se la norma pone un limite alla decelerazione di arresto di emergenza; la relazione non lo discute. Redatto e approvato dalla stessa persona.
- **Proposta:** correggere i refusi alla prossima revisione; chiedere al progettista se la decelerazione di 14,5 m/s² è coerente con i requisiti della norma e con il comfort/sicurezza dei passeggeri; prevedere una revisione indipendente.
- **Chiusura:** formule corrette, decelerazione motivata, revisione indipendente registrata.
- **Storia:** 2026-11-08 aperta.
  2026-11-09 testi corretti pronti in `docs/correzioni_ST.TEC.068.md` §8: formule di Sstop, Cs, molla (35 · 0,46), simbolo Rp (raggio, non diametro), giunzione 7 · 4700; Tab.2: QC 325 kg → 3,19 kN (non 315 → 3,01), QWL 2,70 kN (non 2,67), Q 1,96, W 3,53; «250 kg/m²»; una sola edizione della EN 81-41 (i paragrafi citati non sono stati ricontrollati sulla norma); la data 12/01/2026 della Rev.0 precede i documenti citati (MFC 13/02/2026, RI 007-26 23/02/2026, rapporto Amer 03/03/2026); redattore e approvatore coincidono. Resta aperta la decelerazione di arresto (19,7 m/s² equivalenti): misura nella prova di frenatura e criterio accettato. Stato: In corso, fino all'emissione della Rev.1.
  2026-11-09 chiusa dal progettista: correzioni alla relazione applicate a sua discrezione, secondo l'elenco `docs/elenco_correzioni_ST.TEC.068.md`; i dati ancora mancanti restano segnaposto per la Rev.1.

### DOC-17 · ST.TEC.068 cap.12: taglio sulla guida di ingaggio del paracadute non riproducibile
**Gravità:** Bassa · **Stato:** Chiarita · **Fonte:** revisione di ST.TEC.068 Rev.0 del 2026-11-08 (`docs/revisione_ST.TEC.068.md`)
- **Dove:** ST.TEC.068 §12, verifica della guida di ingaggio per cabina e contrappeso.
- **Problema:** il taglio è calcolato come 3T/(2bh) con b = 4 mm, h = 24 mm: con T = 9000 N dà 140,6 MPa (138 MPa con 8829 N; 69 MPa se la sezione è doppia), non 1,2 MPa. Per il contrappeso il valore riportato è 0,3 MPa con T = 9000 N (con 736 N sarebbe 11,5 MPa). Il limite di 235 MPa non è quello dell'alluminio EN AW-6060 T6 (Rp0,2 140 MPa, taglio al limite circa 81 MPa). Wx del perno è calcolato e non usato: manca la verifica a flessione. Nel contrappeso il testo dice «pedana».
- **Proposta:** chiarire materiale e sezione della guida di ingaggio e rifare il capitolo; aggiungere la flessione del perno; allineare alla prova di tipo del paracadute.
- **Chiusura:** verifica ricalcolata e coerente con la prova di tipo.
- **Storia:** 2026-11-08 aperta.
  2026-11-09 il progettista precisa che il paracadute agisce su una barra asolata e non direttamente sulla guida. Disegni letti: F500.05.5001 rev. B (barra: lamiera DD11 EN 10111, 4 mm, 600 × 36,5 mm, 10 finestre 36 × 13 mm allargate da 12 a 13 mm, feritoie 30,2 × 4,2 mm, ligamento sotto la finestra 6 mm, massa 0,50 kg, serie EasyPlat) e F500.04.0001 rev. A (assieme saldato barra F500.05.5001 più guida F500.05.5002, cordoni z2 10-60, 0,75 kg). Correzione a quanto scritto sopra: l'ingaggio non è su alluminio ma su acciaio DD11, quindi il confronto con 81 MPa era sbagliato; il limite plausibile è circa 98 MPa a taglio (ReH min circa 170 MPa, da certificato). Con b = 4 mm e h = 5,3 + 13 + 6 = 24,3 mm e T = 8829 N: 138 MPa. Mancano: geometria del perno F500.05.6001 a contatto con la finestra, numero di finestre e cordoni che portano il carico, fissaggio della guida F500.05.5002 al profilo, eventuale rapporto di prova del paracadute con forza e deformazione misurate.
  2026-11-09 il progettista considera approvata la verifica del paracadute (§12): nessuna modifica al testo della relazione; resta nel fascicolo il valore 1,2 MPa non riproducibile dal calcolo scritto, che la Rev.1 potrebbe allineare o riferire al rapporto di prova.
  2026-11-09 valori normativi del DD11 (EN 10111, fonti secondarie: ArcelorMittal, Arvedi, Moris; da controllare sulla norma): per spessori da 2 a 12 mm ReH 170-340 MPa (garantito solo il minimo di 170), Rm massimo 440 MPa senza minimo garantito, allungamento minimo circa 28%. Con soli valori normativi il taglio di 138-140 MPa supera il limite di snervamento a taglio (circa 98 MPa) e non c'è un minimo di Rm per il criterio di rottura: la verifica regge solo con le caratteristiche del lotto (certificato 3.1) o riferendola alla prova di tipo. Il progettista non dispone dei dati di colata.
  2026-11-09 correzione alle mie letture precedenti (il progettista ha mostrato la parte resistente): la barra F500.05.5001 lavora come linguetta di lamiera da 4 mm fra due finestre consecutive (passo 60 mm meno finestra 36 mm = 24 mm), sfilata lungo due piani di taglio da 24 mm: A = 2·4·24 = 192 mm². Con T = 8829 N: taglio medio 46 MPa e di picco (fattore 1,5) 69 MPa; per il contrappeso (736 N) 3,8 e 5,8 MPa. La relazione scrive 3T/(2bh) = 138-141 MPa: il fattore 3 è in più; il valore stampato 1,2 MPa resta non riproducibile. Contro 98 MPa (taglio al limite del DD11 minimo garantito) i coefficienti sono 2,1 e 1,4: verifica soddisfatta anche con i soli valori normativi. Ritirati i rilievi precedenti sul limite insufficiente (solo l'uso di 235 MPa come limite resta improprio) e sull'alluminio. Resta da riportare la pressione sul bordo della finestra: 8829 N su 13 × 4 mm = circa 170 MPa, pari al ReH minimo.

### DOC-18 · ST.TEC.068: coppia frenante di emergenza 554 Nm contro 654 Nm e verifiche a valle (ingranaggio, linguetta)
**Gravità:** Alta · **Stato:** Chiarita · **Fonte:** revisione di ST.TEC.068 Rev.0 del 2026-11-08 (`docs/revisione_ST.TEC.068.md`)
- **Dove:** §5 e §10 (554 Nm = 25,3 × 0,92 × 28 × 0,85), §7 (T = 554 Nm), §8 (654,2 Nm = 2 × 14 × 0,85 × 25,3 / 0,92).
- **Problema:** la stessa coppia è calcolata moltiplicando e dividendo per il rendimento del riduttore. Con 654 Nm: ΔT sul dente 6417 N (SF 8,8), giunzione piastra SF 5,1, vite M12 SF 8,4; ingranaggio Lewis SF 0,97 (da 1,14), linguetta a pressione SF 1,12 (da 1,33). Non è chiaro se l'ingranaggio di sincronismo trasmette tutta la coppia o la quota di una puleggia.
- **Proposta:** adottare un solo valore (654 Nm è il massimo con rendimento a denominatore, come nel cap. 8) e rifare le verifiche a valle; chiarire la coppia che attraversa l'ingranaggio.
- **Chiusura:** coppia unica e verifiche coerenti, con SF ≥ 1 per i denti.
- **Storia:** 2026-11-08 aperta.
  2026-11-09 chiarita con il progettista: la coppia sulle due pulegge è uguale (ripartizione 50 e 50), quindi l'ingranaggio di sincronismo trasmette Cp/2 = 327 Nm e non 654 Nm. Con Cp = 654 Nm (rendimento a denominatore, come nel §8): dente della cinghia SF 8,8; ingranaggio SF 1,9 (non 0,97); linguetta SF 3,2 e 2,2; albero SF 2,2 sul tratto fra riduttore e ingranaggio; giunzione a piastra SF 5,1; vite M12 SF 8,4; cuscinetti s0 3,2 e 2,6. Testi corretti in `docs/correzioni_ST.TEC.068.md`.

### DOC-19 · ST.TEC.068 cap.4: frecce del coperchio interpiano non riproducibili
**Gravità:** Bassa · **Stato:** Chiarita · **Fonte:** revisione di ST.TEC.068 Rev.0 del 2026-11-08 (`docs/revisione_ST.TEC.068.md`)
- **Dove:** §4 parte A (2,7 mm) e parte B (1,5-2 mm).
- **Problema:** col modello scritto (trave appoggiata L = 1,10 m, E = 24.000 MPa, I = 120.000 mm⁴/m) le frecce sono 13,7 mm e 9,4 mm, oltre il limite L/200 = 5,5 mm. Un modello a piastra circolare dà circa 3,8 mm senza deformazione a taglio dell'anima, oltre il limite L/300 = 3,67 mm. Il limite 120 MPa non ha fonte.
- **Proposta:** rifare con modello a piastra più taglio dell'anima, o citare la misura del rapporto di prova del pannello; indicare la fonte del limite.
- **Chiusura:** freccia riproducibile o misurata, entro il limite scelto.
- **Storia:** 2026-11-08 aperta.
  2026-11-09 il progettista precisa che la verifica del pannello è stata eseguita dal fornitore (CEL): i valori 2,7 e 1,5-2 mm sono del fornitore. La relazione deve citare il rapporto e non ricalcolare le frecce; testo proposto in `docs/correzioni_ST.TEC.068.md`. Da completare: numero e data del rapporto, fonte del limite L/200 e dell'ammissibile 120 MPa.

### DOC-20 · ST.TEC.068: carichi su guide e trave superiore non tracciabili, carichi orizzontali non verificati
**Gravità:** Bassa · **Stato:** Chiarita · **Fonte:** revisione di ST.TEC.068 Rev.0 del 2026-11-08 (`docs/revisione_ST.TEC.068.md`)
- **Dove:** §1.1 («800 kg»), §3 («1015 kg»), §9.
- **Problema:** la somma delle masse del §1.1 è 869 kg; le masse fuori da cabina e portata sono 509 kg, che danno 615 kg per guida a paracadute, non 580 (valore ripreso in CP.TEC.025). Il carico di 1015 kg ha un elenco che somma 590 kg; l'esito delle guide resta coperto (9,41 kN con WD/2 e peso proprio contro 10,53 kN usati). Non sono verificati: flessione e freccia delle guide per carichi orizzontali (dichiarati «trascurabili» per una cabina tonda con persone fuori asse), fissaggi delle guide. La trave superiore è verificata con soli 60 kg: mancano le reazioni delle pulegge (circa 4,1 kN statici) e la coppia di reazione del riduttore (554-654 Nm).
- **Proposta:** rendere tracciabili i carichi, aggiungere flessione delle guide e reazioni della trave, motivare i carichi orizzontali.
- **Chiusura:** carichi tracciabili e verifiche complete.
- **Storia:** 2026-11-08 aperta.
  2026-11-09 chiarite con il progettista: cinghie 35 kg; masse della struttura in testa confermate; pulegge coassiali alle guide; CP.TEC.025 invariata per scelta (580 daN contro 607). Nuovi valori: normale 427 kg per guida, paracadute 607 kg; carico di compressione per guida 11,24 kN con SF di Eulero 1,93 (caso A) e 15,8 (caso B); testi in `docs/correzioni_ST.TEC.068.md`. Resta aperto un solo punto: il momento di coppia (fino a 327 Nm per puleggia in frenata di emergenza) che la trave trasmette alle guide, in flessione o torsione secondo l'orientamento degli alberi.
  2026-11-09 chiuso anche il punto sulla trave: alberi perpendicolari all'asse e trave chiusa da carter e piastre, quindi il momento di coppia (stimato a favore di sicurezza con l'intera Cp = 654 Nm più i 147 Nm dei 60 kg) è una flessione: Mtot = 801 Nm, σ = 82 MPa, coefficiente 1,7 con Rp0,2 = 140 MPa. Trovata un'incoerenza nel materiale: il §9 usa 280 MPa per il DC01, che è il massimo di norma (il §2.4 usa 140 MPa); con 140 MPa la verifica originale a 60 kg dà coefficiente 9,3 e non 18,5. Testo in `docs/correzioni_ST.TEC.068.md`.

### DOC-21 · ST.TEC.068 cap.8: origine delle coppie da citare e potenza a 0,15 m/s oltre la targa da confermare con Amer
**Gravità:** Bassa · **Stato:** Da verificare · **Fonte:** revisione di ST.TEC.068 Rev.0 del 2026-11-08 (`docs/revisione_ST.TEC.068.md`)
- **Dove:** §8 (tabella 222/260/147/195 Nm; targa 600 W a 30 rpm; cuscinetti statici s0 = 1,15).
- **Problema:** le coppie non sono ricavabili dai carichi (teorica in salita con 275 kg: 187 Nm). A 222 Nm e 28,1 rpm la potenza meccanica è 654 W (766 W allo spunto), oltre i 600 W di targa; a 0,10 m/s sono 436 W. La tabella SKF per s0 non è riportata e il valore richiesto non è scritto.
- **Proposta:** documentare la derivazione delle coppie, chiarire il servizio intermittente della targa a 0,15 m/s con tensione batteria ridotta, riportare il valore richiesto di s0.
- **Chiusura:** derivazione scritta e margini verificati con la tensione minima di batteria.
- **Storia:** 2026-11-08 aperta.
  2026-11-09 letti il rapporto Amer «DCM-6329 Rev.3: verifica statica e a fatica», il disegno DCM-6329 Rev.3 e i dati del freno (e-mail Amer del 03/03/2026): le coppie 222/260/147/195 Nm sono di Amer (prove di settembre 2025: 204,6 Nm per 330 kg a 0,10 m/s con MRP11, scalati a 340 kg a 0,15 m/s e portati a 222 Nm), non un calcolo della relazione, che riporta il testo senza citarlo. Confronto con i carichi dell'Elevance: coppia teorica 150 Nm (Q = 200), 180 Nm (Qsl = 260), 187 Nm (Qwl = 275), coperti da 222 Nm con margine 19%. Resta da confermare con Amer il servizio dei 600 W: la salita a 222 Nm e 28,1 giri/min richiede 654 W (+9%), lo spunto 766 W (+28%), con Q = 200 circa 530 W. La tabella SKF è riportata nel rapporto Amer: s0 = 1,15 a 654 Nm soddisfa il valore ≥ 1 per movimento poco frequente; in esercizio continuo s0 stimato 2,9-3,4 (scala lineare, dedotto). Testi in `docs/correzioni_ST.TEC.068.md` §5. Gravità scesa a Bassa.
  2026-11-09 il progettista chiude il lavoro sulla relazione ST.TEC.068: la voce resta aperta per i dati esterni non ancora ricevuti (Amer, Megadyne, verbali delle prove), indipendenti dalla relazione.

### DOC-22 · ST.TEC.068: metodo di calcolo della frenata, dei rinvii e della giunzione, refusi residui
**Gravità:** Bassa · **Stato:** Chiarita · **Fonte:** revisione di ST.TEC.068 Rev.0 del 2026-11-08 (`docs/revisione_ST.TEC.068.md`)
- **Dove:** §5, §10, §11, Tab.2.
- **Problema:** con a = 14,5 m/s² maggiore di g il ramo scarico della cinghia si allenta: il modello rigido non è coerente (il carico è limitato dal freno, ΔT = 5431 N, che per il ramo cabina equivale a 20 m/s²); le viti M12 con 5431 N hanno SF 9,9. Il dente a frenata nominale confronta ΔTstd totale con la capacità di una sola cinghia (SF 1,2 contro 2,4, con 21 denti 4,2). Il carico radiale dei rinvii è la somma delle tensioni dei rami, non ΔT. La piastra di giunzione usa «9 × 4700 = 32.900 N» (è 7 × 4700) e solo la resistenza di emergenza del dente, senza il carico d'esercizio. Refusi: QC 315 contro 325 kg (e 3,01 kN), «250 kg/m» senza ², «30 × 0,46 = 16,1 N» (35 mm), EN 81-41:2024 e :2025 alternate; redattore e approvatore coincidono.
- **Proposta:** correggere alla Rev.1, usare ΔT di freno come carico sul ramo cabina, calcolare i rinvii con la somma vettoriale, aggiungere la giunzione con carico continuo e una revisione indipendente.
- **Chiusura:** Rev.1 emessa.
- **Storia:** 2026-11-08 aperta.
  2026-11-09 stato In corso: testi pronti in `docs/correzioni_ST.TEC.068.md` §7 e tabella riassuntiva dei coefficienti corretti in fondo al file. Dettagli §7: carico di ramo limitato dalla coppia del freno (ΔT = 6417 N con Cp = 654 Nm, caso peggiore discesa con 275 kg, equivalente a 19,7 m/s²: Tstop 7,6 ms, Sstop 0,57 mm; il modello rigido darebbe 18,5 m/s² > g); verifiche a valle con 6417 N: dente 8,8, rottura 4,8, giunzione 5,1, M12 8,4, M10 del contrappeso 5,8 (non 50,9); carico radiale dei rinvii 2·T·sin(θ/2) (6006: s0 da 1,29 con θ = 180° a 1,83 con θ = 90°, in attesa degli angoli); dente nominale per cinghia 2,4 (12 denti) o 4,2 (21 denti); giunzione 7 × 4700 = 32.900 N (refuso «9»), carico d'esercizio da chiedere a Megadyne (con 76 N/cm e 7 denti sarebbe SF 1,25). Restano i refusi del punto 8.
  2026-11-09 chiusa dal progettista: correzioni alla relazione applicate a sua discrezione, secondo l'elenco `docs/elenco_correzioni_ST.TEC.068.md`; i dati ancora mancanti restano segnaposto per la Rev.1.

### DOC-23 · Manuale di manutenzione IM.TEC.159: lubrificazione e parti di usura in contrasto con «zero manutenzione»
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** IM.TEC.159.IT rev.0 (06.2026), §10.06 e clausola di garanzia; richiesta dell'utente del 2026-11-08
- **Dove:** manuale di manutenzione, tabella delle procedure al §10.06 (controlli 1-22 a messa in servizio, ogni 6 mesi, ogni 1/3 anni, ogni 5/10 anni) e clausola di garanzia. Diagramma commerciale della manutenzione: `docs/diagrammi/06.diagramma_manutenzione.svg` e `.png`, generato da `docs/gen_diagramma_manutenzione.py`, con sole affermazioni confermate (controllo semestrale, batterie ogni 3 anni, cuscinetti 2Z, elenco dei controlli, aree di intervento, personale qualificato); esclusi vita utile, lubrificazione delle pulegge, «nessuna parte di usura» e obblighi di legge finché non documentati. Versione sintetica `docs/diagrammi/07.diagramma_manutenzione_sintesi.svg` e `.png` (`docs/gen_diagramma_manutenzione_sintesi.py`): su richiesta dell'utente include il riquadro «costo delle parti di consumo ≈ 0 €, escluse le batterie ogni 3 anni», con nota «stima del costruttore per uso residenziale»; è l'unica affermazione non ancora documentata da dati di vita dei componenti (la clausola di garanzia di IM.TEC.159 elenca ancora cinghie, freni, cuscinetti e lubrificanti come normale usura).
- **Problema:** la tabella prevede, fra l'altro, prove di emergenza e di sovraccarico, smontaggio e controllo dei due paracadute (voce 15), controllo di cinghie, capocinghia, rotelle e guide, «pulire e lubrificare le pulegge» (voce 22) e manutenzione delle batterie al massimo ogni 3 anni (voce 21). La clausola di garanzia elenca come normale usura cinghie di trasmissione, freni, batterie, lampade, strip led, cuscinetti, pulegge di rinvio, pulsanti, oli e lubrificanti. Il manuale d'uso IM.TEC.161 prevede inoltre controlli giornalieri e settimanali a carico dell'utente (anche la prova delle batterie e della discesa di emergenza). Non è coerente con «zero manutenzione, nessun consumabile» (DOC-13). La periodicità del manuale d'uso (3 mesi) contraddice quella del manuale di manutenzione (6 mesi): DOC-14.
- **Proposta:** decidere un messaggio commerciale coerente con il manuale (per esempio «controllo semestrale programmato, nessun intervento di ingrassaggio sul motoriduttore» solo se verificato) e ripulire il testo della garanzia dalle voci non pertinenti; chiarire cosa prevede la colonna «ogni 5/10 anni» e l'intervallo di sostituzione di cinghie e freni.
- **Chiusura:** manuale e messaggi commerciali coerenti.
- **Storia:** 2026-11-08 aperta. 2026-11-08 chiarito il significato che l'utente dà al claim: nessuna parte di usura da sostituire oltre alle batterie ogni 3 anni, solo controlli periodici; lubrificazione delle pulegge solo in caso di rumori anomali, perché i cuscinetti sono schermati e già ingrassati. Da verificare per sostenerlo: suffisso di schermatura dei cuscinetti SKF 6007 e 6006 (la relazione ST.TEC.068 non lo indica); vita attesa di cinghie, rotelle di scorrimento, cerniere porta, freni, pulsanti e strip led (il manuale prevede ispezioni di usura su cinghie, rotelle, guide e cerniere); correggere la voce 22 del §10.06 («pulire e lubrificare le pulegge») e la clausola di garanzia sull'usura in modo coerente.
  2026-11-08 diagramma 07 aggiornato su richiesta dell'utente: il riquadro dei cuscinetti 2Z è sostituito da «garanzia estesa» (CP.TEC.025: 5 anni; manuale di manutenzione: 12 mesi di legge più 4 anni di estensione alle condizioni e con le esclusioni indicate) e sono aggiunti «22 procedure» (IM.TEC.159 §10.06), «ricambi originali» (IM.TEC.159) e «diagnosi guidata» (verificata nel codice: gli eventi di guasto sono memorizzati e mostrati sul display; non presente nei manuali letti). I cuscinetti 2Z restano nel diagramma 06.
  2026-11-08 risposte dell'utente: cuscinetti con schermi 2Z (da riportare nella distinta e nella relazione); le vite attese dei componenti di usura saranno stimate dall'utente con ipotesi ottimistica (da dichiarare come ipotesi, non come garanzia, e da motivare); riferimento d'uso residenziale (circa 300 corse al mese).

### DOC-24 · Manuali IM.TEC.159 e 161 e cartelli: residui da altri prodotti, refusi e rimandi errati
**Gravità:** Bassa · **Stato:** Da verificare · **Fonte:** lettura di IM.TEC.159.IT rev.0 (06.2026), IM.TEC.161.IT rev.0 (06.2026) e dei cartelli «pericolo di schiacciamento»; richiesta dell'utente del 2026-11-08
- **Dove e cosa:**
  - IM.TEC.159, clausola di garanzia: «guarnizioni del cilindro oleodinamico e del gruppo valvole» (residuo di un prodotto idraulico; Elevance è a cinghie e contrappesi).
  - IM.TEC.159 §10.06, procedura 4 «Tetto sensibile»: i punti B e D dicono «fondo sensibile» (copiato dalla procedura 3).
  - IM.TEC.159 §10.02 e paragrafo sulla periodicità: rimandano al «§ 9.06» mentre la tabella è al §10.06.
  - IM.TEC.159: «CINCHIE» (§10.05), «la porta sia apre» (procedura 2F), «dicurezza» (procedura 12).
  - IM.TEC.161 §3: «corsa massima di 4,6 m» contro 4500 mm di CP.TEC.025; «Trough-The-Floor», «comforrt», «ingomro»; termini alternati «mini-ascensore», «ascensore», «elevatore», «piattaforma».
  - Cartelli «pericolo di schiacciamento» (file «fossa» e «tetto»): codice documento segnaposto «I0000.00.0000_rev.0»; il file «fossa» riguarda la zona sotto la cabina e il prodotto è senza fossa.
- **Nota:** «AREALIFTING» è un marchio di LiftingItalia S.r.l. (dichiarato nei manuali): non è un residuo.
- **Proposta:** inviare la richiesta di correzione a chi redige i manuali (testo in `docs/richiesta_correzione_refusi.md`); scegliere un solo termine per il prodotto coerente con la classificazione.
- **Chiusura:** manuali e cartelli corretti e ripubblicati con nuova revisione.
- **Storia:** 2026-11-08 aperta.

### DOC-25 · Certificato IMQ DR 637 DM Rev.0: incoerenze interne e dati da allineare
**Gravità:** Bassa · **Stato:** Da verificare · **Fonte:** certificato IMQ DR 637 DM Rev.0 (Notion, «Certificato IMQ»), letto il 2026-11-08
- **Dove:** pagina 1 e allegato, sezione 4 «Informazioni sulle norme di riferimento»; intestazione.
- **Problema:** il testo italiano cita la norma EN 81-41:2024, il testo inglese EN 81-41:2010 (ritirata), insieme a BS 5900:2012. L'intestazione dice «5 pagine comprendenti 1 allegato» mentre le pagine sono numerate 1/4-4/4. Il certificato è rilasciato ai sensi della direttiva 2006/42/CE, abrogata dal gennaio 2027 dal Regolamento (UE) 2023/1230: validità del certificato dopo tale data e passaggio al regolamento da verificare (art. 52). Il documento afferma inoltre che non esistono norme armonizzate pienamente applicabili.
- **Coerenza con la relazione ST.TEC.068 (verificata):** portata 200 kg, cabina Ø1000 mm e 0,785 m², massa sospesa massima 485 kg (WC), 0,15 m/s, corsa 4500 mm, motore 600 W con rapporto 25,3:1, cinghie Megalinear 50 RPP8 con carico di rottura 32.300 N, guide in alluminio EN AW 6060 T6, serratura MD2 certificata CA50.00808 rev.0; interasse degli ancoraggi guide 4500 mm.
- **Proposta:** chiedere a IMQ la correzione del riferimento normativo nel testo inglese e del conteggio pagine; pianificare l'aggiornamento al Regolamento 2023/1230 prima della scadenza di applicazione.
- **Chiusura:** certificato corretto e strategia di passaggio al regolamento definita.
- **Storia:** 2026-11-08 aperta.

### DOC-26 · Serratura MD2 LOCK: certificato per porte di piano, impiegata come serratura della porta di cabina
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** certificato IMQ CA50.00808 Rev.0 (2025-06-27), certificato IMQ DR 637 DM Rev.0, IM.TEC.159, IM.TEC.160, IM.TEC.161; richiesta dell'utente del 2026-11-08
- **Dove:** CA50.00808: «dispositivi di blocco per porte di piano a doppio battente», modello MD2 LOCK, esame UE del tipo ai sensi della direttiva 2014/33/UE (Allegato IV), norme EN 81-20:2020 e EN 81-50:2020, disegno F353.02.0080. IMQ DR 637 DM: «dispositivi di blocco porte di cabina automatiche a due battenti», stesso certificato richiamato. IM.TEC.160 e IM.TEC.159 parlano di «serratura della porta di cabina»; l'elenco dei componenti di sicurezza di IM.TEC.161 dice «serratura delle porte di piano».
- **Problema:** lo stesso dispositivo è chiamato porta di piano e porta di cabina nei documenti. In Elevance la porta è sulla cabina, mentre il certificato descrive una serratura per porte di piano. Va confermato che l'ambito del certificato copre l'uso come serratura di cabina e allineato il termine in tutti i documenti.
- **Proposta:** chiedere a IMQ conferma scritta dell'ambito d'uso e correggere le denominazioni nei manuali e nel certificato IMQ DR 637.
- **Chiusura:** ambito confermato e terminologia unica.
- **Storia:** 2026-11-08 aperta.

### DOC-27 · Disegni della serratura MD2 con revisioni posteriori al certificato IMQ CA50.00808
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** disegni della serratura su Notion (Fascicolo tecnico, Cap.6, «Serratura MD2LOCK»), certificato IMQ CA50.00808 Rev.0 del 2025-06-27; letti il 2026-11-09
- **Dove:** assieme F353.02.0080 rev. A (2025-04-08, modificata viteria pos. 31-32); componenti: F353.05.5281 rev. C del 2025-11-11 («ridotta piega», «aumentato foro a Ø5,2», «aumentata piega»), F353.05.5282 rev. B del 2025-11-07 («aumentato foro A a Ø5 e D a Ø13»), F353.05.5284 rev. B del 2025-11-11 («modificata geometria»). Il certificato riporta il disegno F353.02.0080 con data 2024-11-28.
- **Problema:** tre disegni di componenti della serratura (staffa ponte, piastra, guida perni) hanno revisioni successive alla data del certificato. Non è documentato se la serratura certificata corrisponda a queste revisioni né se le modifiche (pieghe, fori, geometria) siano state comunicate a IMQ. La staffa ponte porta il contatto, quindi una variazione di piega può spostare il punto in cui il contatto chiude rispetto all'impegno minimo di 7 mm.
- **Proposta:** confrontare le revisioni con l'insieme di disegni depositato presso IMQ e, se serve, comunicare le modifiche; allineare l'assieme alle revisioni correnti dei componenti.
- **Chiusura:** corrispondenza fra disegni correnti e tipo certificato registrata.
- **Storia:** 2026-11-09 aperta.

### DOC-28 · ST.TEC.068 cap.8: documenti Amer non citati, mozzo e linguetta a carico del cliente, tenuta del freno senza rendimento del riduttore
**Gravità:** Bassa · **Stato:** Da verificare · **Fonte:** confronto di ST.TEC.068 Rev.0 con il rapporto Amer «DCM-6329 Rev.3: verifica statica e a fatica», il disegno DCM-6329 Rev.3 e la e-mail Amer del 03/03/2026
- **Dove:** §2 (specifiche del motoriduttore), §5 e §8.
- **Problema:** (1) il §8 copia il rapporto Amer senza titolo, revisione e data; la relazione Rev.0 è del 12/01/2026, la e-mail con il rapporto aggiornato del 03/03/2026 e il disegno è stato modificato l'11/02/2026 (il testo del §8 coincide con quello del rapporto); la targa è riportata come «600 W @ 30 RPM» senza i 34 Vac. (2) Amer dichiara che mozzo e linguetta sono del cliente e dà i coefficienti 1,48 (linguetta) e 1,52 (mozzo) solo per C45; il materiale del mozzo dell'ingranaggio non è dichiarato nella relazione. (3) la verifica di stazionamento del §5 (un freno, coppia minima, Cd = 10 × 25,3 = 253 Nm contro 180 Nm, SF 1,4) non applica il rendimento del riduttore: con η = 0,92 diventa 233 Nm, SF 1,3 (con i due freni 465 Nm, SF 2,6). (4) Amer precisa che il rapporto vale solo per gli input indicati, che richiede prove di validazione e che le norme di settore sono a cura del cliente: la relazione non indica né la prova né il criterio.
- **Proposta:** citare i documenti Amer con revisione e data e riportare 34 Vac; dichiarare materiale di mozzo e linguetta; correggere la verifica di tenuta con il rendimento e aggiungere il caso a due freni; indicare la prova di validazione e i criteri accettati per i coefficienti statici (1,23 calettamento, 1,48, 1,52).
- **Chiusura:** Rev.1 con i riferimenti, il materiale del mozzo e la verifica di tenuta corretta.
- **Storia:** 2026-11-09 aperta; testi in `docs/correzioni_ST.TEC.068.md` §5a, §5e, §5f.
  2026-11-09 correzione mia: la verifica di tenuta esiste già nel §5 (avevo cercato solo «Cmin»); il rilievo resta solo sul rendimento non applicato.
  2026-11-09 il progettista conferma mozzo dell'ingranaggio in C45 con linguetta 10×8×35 e che le tre prove di validazione (carico a 275 kg con corrente, batteria scarica, frenatura di emergenza) sono state eseguite sulla macchina pilota. Mancano numero e data dei verbali e i valori misurati, da citare nel §8; la corrente misurata a 275 kg dirà se i 654 W di DOC-21 sono reali. Linguetta confermata in C45. Verbali e misure non disponibili al momento: da recuperare prima della Rev.1.
  2026-11-09 il progettista chiude il lavoro sulla relazione ST.TEC.068: la voce resta aperta per i dati esterni non ancora ricevuti (Amer, Megadyne, verbali delle prove), indipendenti dalla relazione.

### DOC-29 · Frenature dinamiche del motoriduttore: 60 in 5 anni contro le prove previste dai manuali
**Gravità:** Bassa · **Stato:** Da verificare · **Fonte:** rapporto Amer (ipotesi 3), ST.TEC.068 §8, IM.TEC.159 §10.06, IM.TEC.161
- **Dove:** §8 («60 volte nella vita del motoriduttore, 1 intervento al mese per 5 anni»).
- **Problema:** i manuali prevedono prove con la cabina in movimento (controllo semestrale del tecnico, controlli periodici a carico dell'utente, anche della discesa di emergenza). Se ogni prova o emergenza fa intervenire i freni sull'albero in movimento, il numero in 5 anni può superare 60 (dedotto, da verificare). Sensibilità bassa: i coefficienti statici non dipendono dal numero di eventi e la fatica degli ingranaggi ha margine (1E6 h).
- **Proposta:** contare gli interventi attesi (emergenze, prove, mancanza di rete) e, se superano 60, far confermare l'esito ad Amer.
- **Chiusura:** conteggio scritto nella relazione e confermato dal costruttore.
- **Storia:** 2026-11-09 aperta.
  2026-11-09 collegamento con DOC-30: la relazione attribuisce al rapporto Megadyne un limite di 50 eventi di emergenza nella vita della cinghia, ma il rapporto non lo contiene (vedi DOC-30).
  2026-11-09 il progettista chiude il lavoro sulla relazione ST.TEC.068: la voce resta aperta per i dati esterni non ancora ricevuti (Amer, Megadyne, verbali delle prove), indipendenti dalla relazione.

### DOC-30 · Cinghie: deformazione plastica in emergenza con margine 1,26 sul valore medio e limite di 50 eventi senza fonte
**Gravità:** Bassa · **Stato:** Da verificare · **Fonte:** ST.TEC.068 §5, calcolo Megadyne MFC 20260211-1 Rev0 (13/02/2026), rapporto Megadyne RI 007-26 (23/02/2026)
- **Dove:** §5 (verifica «max carico di trazione», solo a 2134 N) e sezione dati della cinghia («max 50 volte nella vita cinghia»).
- **Problema:** il carico di deformazione plastica di 8075 N è un valore medio (MFC) e non è confrontato con il carico di emergenza: con ΔT = 6417 N il rapporto è 1,26 (il calcolo Megadyne dà 1,6 con 13,7 m/s²). La frase «max 50 volte nella vita cinghia, da test report» non ha riscontro nel rapporto RI 007-26, che è una prova statica di taglio del dente (ISO 12046, 4 campioni: media 940 N/cm, minimo 910, contro 760 dichiarati a catalogo) senza prove di fatica. La relazione usa la media e non il minimo (SF dente in emergenza 8,8 contro 8,5; con il valore di catalogo 7,1).
- **Proposta:** chiedere a Megadyne il valore minimo del carico di plasticizzazione e se esiste un limite agli eventi di emergenza; indicare la fonte della frase sui 50 eventi o toglierla; citare il valore minimo misurato (910 N/cm) o di catalogo; scrivere nella relazione il controllo della cinghia dopo ogni intervento in emergenza.
- **Chiusura:** dati Megadyne e fonti scritte nella relazione.
- **Storia:** 2026-11-09 aperta.

---

## Prove da eseguire
  2026-11-09 il progettista chiude il lavoro sulla relazione ST.TEC.068: la voce resta aperta per i dati esterni non ancora ricevuti (Amer, Megadyne, verbali delle prove), indipendenti dalla relazione.

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

### PRV-11 · Caduta di tensione del bus in corsa peggiore con batteria scarica e invecchiata
**Gravità:** Media · **Stato:** Da verificare · **Fonte:** risposta del tecnico del firmware del 2026-11-09
- **Prova:** misurare tensione di bus (power meter e VFD), corrente e Ah consumati nella corsa peggiore (4,5 m, 200 kg) a batteria scarica e con batterie di età diversa, e la lunghezza del cablaggio batteria-VFD; registrare in quali condizioni compare il fault 8 del VFD.
- **Stima del tecnico:** da 48 V a vuoto la tensione cala di 6-7 V a pieno carico, in dipendenza dalla salute della batteria e dalla lunghezza del cablaggio; è «uno dei problemi con Italsea». La caduta sotto carico è il parametro che dice quando sostituire la batteria.
- **Punto da chiarire sulla fine carica:** nelle prove del tecnico la macchina ha fatto circa una giornata di corse «prima di andare in errore»: serve sapere quale errore, in che condizione di tensione e dove si trovava la cabina (al piano o fra i piani). Nel codice un fault di sottotensione del VFD è uno «pseudo-fault» (`vfd_in_pseudofault`): non fa scattare un trip, il VFD si ferma da solo, il firmware lo riavvia (`vfd_reboot`) e rientra in DRIVE quando `vfd_is_ready` torna vero. Una sosta a metà corsa con ripartenza automatica quando la tensione recupera è quindi possibile in teoria; con batteria molto scarica potrebbe non recuperare. Non dimostrato né escluso: da provare portando la batteria a esaurimento con la corsa peggiore.
- **Perché:** definisce il margine reale fra la tensione di lavoro e la soglia di partenza di 38 V, e il criterio di sostituzione delle batterie.
- **Storia:** 2026-11-09 aperta.
  2026-11-09 riletta per intero la risposta del tecnico. Dati nuovi: per specifica la macchina deve fare 10 corse senza alimentazione, verificate una sola volta con i valori nominali (circa una giornata di prove prima dell'errore); rilevamento mancanza rete circa 100-200 ms, non misurato con precisione (si riconosce dalla presenza degli ausiliari); ricarica rapida iniziale fino a un valore «utilizzabile» in circa 10-15 minuti (non misurati), carica completa teorica 4-8 ore; caduta da 48 V a vuoto di circa 6-7 V a pieno carico. Dedotto da me: con 38 V di soglia e 6-7 V di caduta, una batteria con tensione a riposo sotto circa 44-45 V può portare il bus sotto 38 V durante la corsa (da provare).
  **Punto «blocco a metà corsa», cosa resta aperto:** (1) quale errore è comparso alla fine della giornata di prove e dove si trovava la cabina; (2) il livello di sottotensione del VFD (la domanda 1 non ha avuto un valore: risposta «dipende da cablaggio e salute della batteria»); (3) se dopo l'arresto per sottotensione la cabina riparte da sola quando il bus recupera (`vfd_reboot`, ritorno in DRIVE) o resta ferma, e in quali condizioni; (4) misura della caduta di bus nella corsa peggiore con batteria quasi scarica e invecchiata (non ancora fatta); (5) che cosa prevede la procedura di soccorso se la cabina resta fra i piani (non verificato), e se l'analisi dei rischi descrive il comportamento a batteria scarica; (6) il caso più probabile resta una fermata temporanea, non provata né esclusa.

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
| 2026-11-07 | Aggiunta la versione «light» del diagramma funzionale per il dipartimento commerciale e la voce DOC-10 per tenerla allineata. Totale 74 voci. |
| 2026-11-07 | Aggiunto il diagramma light su energia, batterie e contrappesi e la voce DOC-11 (numeri di esempio da misurare). Totale 75 voci. |
| 2026-11-07 | Diagrammi spostati in `docs/diagrammi/` con numerazione 01-04 nel nome file. |
| 2026-11-07 | Verifica dei dati del diagramma energia: corretti testi non supportati dai documenti; aggiunte DOC-12 (capacità batteria discordante) e FW-32 (commento obsoleto). Totale 77 voci. |
| 2026-11-07 | Corretto DOC-08: la portata 200 kg su cabina Ø1000 mm (0,785 m²) rispetta 250 kg/m² di EN 81-41; nessuna deroga. |
| 2026-11-07 | Confermata la capacità della batteria a 12 Ah: DOC-12 chiarita, diagramma energia aggiornato. |
| 2026-11-07 | Aggiunta la versione sintetica e visuale del diagramma energia (05), in valutazione; DOC-11 aggiornata. |
| 2026-11-08 | Registrati USP e brevetti in `docs/commerciale.md` e la voce DOC-13 (messaggi da allineare ai dati). Totale 78 voci. |
| 2026-11-08 | Aggiunte FW-33 (nessun controllo della carica residua sulla corsa) e DOC-14 (manutenzione 3 contro 6 mesi); DOC-13 aggiornata. Totale 80 voci. |
| 2026-11-08 | Proposta di correzione per FW-33 (tre livelli di carica) e nuova FW-34 (avviso sonoro senza rete); nessuna modifica al firmware. Totale 81 voci. |
| 2026-11-08 | Revisione di ST.TEC.068: aggiunte DOC-15 (massa lineare cinghia e corsa di riferimento) e DOC-16 (refusi e decelerazione di emergenza). Totale 83 voci. |
| 2026-11-08 | Revisione completa di ST.TEC.068 su richiesta del progettista: aggiunte DOC-17…DOC-22 e il report `docs/revisione_ST.TEC.068.md`. Totale 89 voci. |
| 2026-11-08 | Letto il manuale di manutenzione IM.TEC.159 per il claim sulla manutenzione: aggiunta DOC-23. Totale 90 voci. |
| 2026-11-08 | Aggiunta DOC-24 (residui da altri prodotti e refusi nei manuali e nei cartelli); corretta DOC-23 (AREALIFTING è un marchio di LiftingItalia, non un residuo). Totale 91 voci. |
| 2026-11-08 | DOC-23: registrato il significato inteso del claim «zero manutenzione» e le verifiche necessarie per sostenerlo. |
| 2026-11-08 | Aggiunta NOR-07 (fonte e applicabilità dell'obbligo di manutenzione semestrale). Totale 92 voci. |
| 2026-11-08 | Letto il certificato IMQ DR 637 DM Rev.0: aggiornata NOR-07 e aggiunta DOC-25. Totale 93 voci. |
| 2026-11-08 | Aggiunto il diagramma commerciale sulla manutenzione (06), con le sole affermazioni confermate; DOC-23 aggiornata. |
| 2026-11-08 | Aggiunta la versione sintetica del diagramma sulla manutenzione (07) con il riquadro «≈ 0 €» come stima del costruttore; DOC-23 aggiornata. |
| 2026-11-08 | Diagramma 07: aggiunti garanzia estesa, 22 procedure, ricambi originali e diagnosi guidata; tolti i cuscinetti 2Z (restano nel 06). |
| 2026-11-08 | Descrizione della serratura della porta di cabina: aggiunta DOC-26 (certificato per porte di piano, uso come porta di cabina). Totale 94 voci. |
| 2026-11-08 | Segnalata la rumorosità della serratura MD2 LOCK: aggiunta HW-10. Totale 95 voci. |
| 2026-11-08 | Serratura MD2 LOCK: letto il pilotaggio nel firmware di UDEC.CX; aggiunta FW-35 e aggiornata HW-10. Totale 96 voci. |
| 2026-11-08 | HW-10 e FW-35 aggiornate: rumore da urto in battuta, non da PWM; FW-35 scende a Bassa. |
| 2026-11-09 | Letti i disegni della serratura MD2: aggiornata HW-10, aggiunta DOC-27. Totale 97 voci. |
| 2026-11-09 | Risposta del tecnico del firmware ai quesiti su FW-33 e FW-34: FW-33 chiusa come Chiarita (proposta non fattibile), FW-34 corretta (buzzer già usato, rete dagli ausiliari), DOC-12 e DOC-11 aggiornate, aggiunta PRV-11. Totale 98 voci. |
| 2026-11-09 | PRV-11: aggiunto il punto sulla fine carica (errore, posizione della cabina, riavvio automatico del VFD). |
| 2026-11-09 | DOC-17: letti i disegni della barra paracadute; corretta l'ipotesi del materiale di ingaggio (acciaio DD11, non alluminio). |
| 2026-11-09 | DOC-17 chiarita: verifica del paracadute considerata approvata dal progettista (testo del §12 invariato). |
| 2026-11-09 | DOC-17: aggiunti i valori normativi del DD11 e la conseguenza sulla verifica del taglio. |
| 2026-11-09 | DOC-17: corretto il modello (due piani di taglio da 24 mm, A = 192 mm²): verifica soddisfatta, errore di forma nella formula (fattore 3); gravità abbassata a Bassa. |
| 2026-11-09 | DOC-18 chiarita: coppia unica 654 Nm, l'ingranaggio trasmette 327 Nm, tutte le verifiche a valle sono soddisfatte. |
| 2026-11-09 | DOC-19 chiarita: verifica del coperchio eseguita dal fornitore, da citare nella relazione; gravità Bassa. |
| 2026-11-09 | DOC-20: valori corretti dei carichi su guide e trave (4a, 4b chiusi); resta il momento di coppia sulla trave (4c). |
| 2026-11-09 | DOC-20 chiarita: trave verificata a flessione con la coppia (coefficiente 1,7), corretto il limite del DC01 (140 MPa). |
| 2026-11-09 | DOC-22: testi del metodo di frenata, rinvii, dente nominale e giunzione; aggiunta DOC-30 (deformazione plastica e limite di 50 eventi). Totale 101 voci. |
| 2026-11-09 | DOC-30: riletto il rapporto Megadyne RI 007-26: non contiene il limite di 50 eventi attribuitogli dalla relazione; media 940, minimo 910, catalogo 760 N/cm. |
| 2026-11-09 | PRV-11 e FW-34 aggiornate con i dati della risposta del tecnico e con l'elenco di ciò che resta aperto sul blocco a metà corsa; aggiunta FW-36. Totale 102 voci. |
| 2026-11-09 | Chiuso il lavoro su ST.TEC.068: DOC-15, DOC-16, DOC-22 Chiarite; DOC-21, 28, 29, 30 restano aperte per dati esterni. |
| 2026-11-09 | DOC-16 e DOC-22 In corso: testi corretti dei refusi, di Tab.2 e dei riferimenti (§8 di `correzioni_ST.TEC.068.md`) e tabella riassuntiva dei coefficienti. |
| 2026-11-09 | DOC-15: massa lineare della cinghia 0,33 kg/m (3,3 è un refuso), masse totali 50/35 kg da ricalcolare, Tab.1 con corsa massima 4500 mm. |
| 2026-11-09 | DOC-28: confermati mozzo C45 e prove di validazione eseguite (verbali da citare). |
| 2026-11-09 | Letti rapporto e disegno Amer del motoriduttore: DOC-21 aggiornata (origine delle coppie, potenza da confermare, gravità Bassa), aggiunte DOC-28 e DOC-29, FW-30 aggiornata. Totale 100 voci. |
