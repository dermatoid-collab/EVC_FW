# Proposte di correzione firmware da sottoporre al tecnico

Data: 2026-11-08 · Stato: **solo proposte, nessuna modifica applicata al firmware.**
Dettagli e storia nel registro `docs/registro_issue.md`: voci **FW-33** e **FW-34** (collegate: FW-32, DOC-12).
Percorsi relativi a `TTF-Base/Firmware/TTF-Base/Core/`.

## Premessa

**Verificato nel codice**
- La macchina non entra in movimento con il bus del convertitore (VFD) a 38 V o meno (`Src/vfd.c:152-165`, `vfd_is_ready`).
- Se `low_battery` è attivo (soglia 38,4 V sotto carico, `Src/battery.c:256-262`) e la macchina è al piano, resta fuori servizio e si può solo riaprire la porta (`Src/run_mode.c:524`).

**Dedotto, da provare**
- La decisione di partire guarda solo la tensione istantanea. Il contatore di carica `battery.charge` serve solo alla ricarica.
- Se la tensione cala sotto carico, in marcia `pmode_scan_drive` (`Src/pmode.c:121-135`) riporta lo stato da DRIVE a TO_DRIVE e la cabina si ferma fra i piani.
- Le soglie di avvio (38 V) e di fuori servizio (38,4 V) sono quasi coincidenti.

## Proposta 1 (FW-33): non partire senza carica sufficiente

Avviare una corsa solo se la carica basta per una corsa più l'apertura porte. Tre livelli, calcolati da `battery.charge` (12 Ah di capacità, confermati) e dalla tensione:

| Livello | Condizione | Comportamento |
|---|---|---|
| Normale | carica sopra S1 | come oggi |
| Riserva | tra S2 e S1 | una sola chiamata. Poi porte aperte e fuori servizio al piano fino alla ricarica |
| Esaurita | sotto S2 | non parte. Fuori servizio al piano, con la sola riapertura della porta |

- **Taratura:** S2 è la carica per la corsa peggiore (4,5 m, 200 kg) più apertura porte, più un margine. S1 è S2 più una corsa di margine. A tavolino la salita peggiore vale circa 0,13 Ah (circa 6 Wh a 48 V, con rendimento ipotizzato): il valore va misurato.
- **Cosa riusa:** il percorso di `low_battery` in `run_mode.c:524`, con un latch «ultima corsa consumata» in più, e il ripristino già presente in `battery.c:189` (corrente di carica sotto 2,8 A in carica completa).
- **Controllo sulla tensione:** affianca il contatore. Si memorizza la caduta massima della corsa precedente e non si parte se la tensione a riposo meno quella caduta scende sotto `VFD_DCLINK_MIN` più un margine. Serve perché il contatore sovrastima la carica con la batteria invecchiata.
- **Cosa non tocca:** catena di sicurezza e freno. Cambia solo la logica che abilita le chiamate.
- **Richiede:** aggiornare l'analisi dei rischi (comportamento a batteria scarica).

## Proposta 2 (FW-34): avviso sonoro senza rete

- **Rete assente:** dichiararla se `npb.online` è falso (`Src/npb.c:176`) o se il bit `NPB_FLT_AC_FAIL` (`Inc/npb.h`, oggi inutilizzato) è attivo, per più di alcuni secondi, con conferma per evitare falsi allarmi.
- **Suono:** bip intermittente durante la marcia, con uno schema diverso nello stato di riserva della proposta 1.
- **Mezzo:** il bit `hl_buz` delle uscite UDEC.C (`Inc/udec_c.h`, «Buzzer su scheda») non è mai usato dal firmware della base. In alternativa c'è la sezione audio di UDEC.CX (`UDEC.CX/Audio.kicad_sch`).

## Domande per il tecnico

1. Qual è il punto di sottotensione del VFD, e quanto cala il bus nella corsa peggiore a batteria quasi scarica?
2. Il caricabatteria risponde ancora o segnala `AC_FAIL` quando si stacca la rete?
3. Cosa fa `hl_buz` nel firmware di UDEC.C, e il buzzer è montato? Altrimenti, qual è il comando per l'audio di UDEC.CX?
4. `BATTERY_DISCHARGED_MAH = 7500` è una scelta voluta con batteria da 12 Ah, o un residuo di un modello precedente? Il commento in `battery.c` (`battery_charge_wait_discharge`, «50% = 3,6 Ah») non coincide con `BAT_BOOST_START_CHARGE` (-2,0 Ah): FW-32.

## Prove da fare prima di fissare le soglie

- Corsa peggiore a batteria quasi scarica, con misura di Ah consumati e caduta di tensione.
- Stacco della rete, con misura del tempo di rilevamento.
- Ripristino dopo la ricarica.
