# Correzioni proposte alla relazione ST.TEC.068 per la Rev.1

Raccolta dei testi corretti, punto per punto. Data di inizio: 2026-11-09. Stato di ogni punto nel registro (`docs/registro_issue.md`).

## 1. §12 Paracadute (DOC-17, chiarita)

Il perno (taglio 4T/(3A) = 76,4 MPa) è corretto. Il calcolo sulla barra va sostituito. Il materiale è lamiera DD11 EN 10111 (non S235 né alluminio), la sezione resistente è formata da due piani di taglio, non da una sezione unica.

### Cabina (sostituire il blocco sulla guida di ingaggio)

> **Barra di ingaggio** (F500.05.5001): lamiera DD11 EN 10111, spessore b = 4 mm. In arresto il perno spinge il bordo della finestra e la linguetta di lamiera fra due finestre consecutive (passo 60 mm, finestra 36 mm, quindi h = 24 mm) tende a sfilarsi lungo due piani di taglio, ciascuno di area b · h = 4 · 24 = 96 mm². Area resistente a taglio A = 2 · b · h = 192 mm².
>
> Taglio medio τ = Tmax / A = 8829 / 192 = 46 MPa. Taglio di picco, per distribuzione non uniforme: τmax = 1,5 · τ = 69 MPa.
>
> Resistenza: DD11 EN 10111, ReH ≥ 170 MPa (minimo garantito dalla norma per spessori da 2 a 12 mm). Limite a taglio τy = ReH / √3 = 98 MPa.
>
> Coefficiente di sicurezza: 98 / 69 = 1,4 sul picco, 98 / 46 = 2,1 sul valore medio. La barra risulta verificata.
>
> Pressione di contatto sul bordo della finestra: σ = Tmax / (13 · 4) = 8829 / 52 = 170 MPa, pari al ReH minimo: schiacciamento locale ammesso in evento eccezionale. La capacità complessiva è confermata dalla prova di tipo n. … (da completare).

### Contrappeso

> WDCWT = 30 · 5 = 150 kg; Tmax = 150 · 9,81 / 2 = 736 N. Perno: τ = 4 · 736 / (3 · 154) = 6,4 MPa. Barra: τ = 736 / 192 = 3,8 MPa medio, 5,8 MPa di picco; coefficiente 98 / 5,8 = 17.

### Altre correzioni nel §12

1. Nel calcolo della guida della cabina usare 8829 N al posto di 9000 N.
2. Nel contrappeso il 9000 N era il valore della cabina: usare 736 N.
3. Limite del perno: «adottato τ = 235 MPa (cautelativo rispetto a 540 / √3 = 312 MPa)»; togliere il riferimento al limite della guida.
4. Contrappeso: «sostiene la pedana» → «sostiene il contrappeso»; perno 6,4 MPa invece di 7; togliere Wx = 269 mm³ non usato, o aggiungere la verifica a flessione.

## 2. Coppia frenante di emergenza e verifiche a valle (DOC-18, chiarita)

Il riduttore trasmette una coppia totale massima Cp, che si ripartisce in parti uguali fra le due pulegge (carico 50 e 50 fra le due cinghie, come assunto nella relazione). L'ingranaggio di sincronismo trasmette solo la coppia della puleggia che comanda, cioè Cp/2.

### Definizione unica (§5, da richiamare in §7, §8, §10, §11)

> In frenata dinamica, con il carico che trascina il riduttore e il freno sull'albero motore, la coppia massima alla puleggia di trazione è Cp = 2 · Cmax · Ebr · RR / Erid = 2 · 14 · 0,85 · 25,3 / 0,92 = **654 Nm** (coppia totale; 327 Nm per ciascuna delle due pulegge).

Il valore 554 Nm (rendimento moltiplicato) vale quando è il motore a muovere il carico e va eliminato dai §5, §7 e §10.

### Valori da riportare

| Verifica | Valore corretto con Cp = 654 Nm |
|---|---|
| §5 dente della cinghia, ΔT = 0,5 · Cp / Rp | 6417 N; SF = 56.400 / 6417 = **8,8** |
| §7 ingranaggio, coppia 327 Nm: Ft = 5454 N, Lewis σ = 155 MPa | SF = 300 / 155 = **1,9** |
| §7 linguetta, F = 18,7 kN: taglio 53 MPa, pressione 134 MPa | SF **3,2** e **2,2** |
| §7 albero Ø35, torsione sul tratto fra riduttore e ingranaggio (654 Nm): 77,7 MPa | SF **2,2**; sul tratto di una sola puleggia (327 Nm): SF 4,4 |
| §10 cuscinetti, P0 = 6417 / 2 = 3208 N | 6007: s0 **3,2**, L10h 79.800 h; 6006: s0 **2,6**, L10h 44.200 h (i rinvii vanno ricalcolati col metodo del punto 7) |
| §11 giunzione a piastra (7 denti): 32.900 / 6417 | SF **5,1** |
| §11 vite M12 cabina con 6417 N | SF **8,4** |
| §11 piastra alternativa con 6417 N | SF **15,7** |

Sensibilità: se la ripartizione fra le due cinghie fosse 60 e 40, la coppia sull'ingranaggio sarebbe 393 Nm: SF denti 1,6, linguetta a pressione 1,9. Resta sopra 1.

### Da decidere

- Quale ripartizione assumere per l'ingranaggio (50 e 50 come ora, oppure 60 e 40 come prudenza).
- Se nel §7 indicare il tratto d'albero dove si verifica la torsione (fra riduttore e ingranaggio, a 654 Nm).

## 3. §4 Coperchio interpiano (DOC-19, chiarita)

La verifica del pannello è stata eseguita dal fornitore. La relazione deve citarla, non ricalcolarla: il calcolo interno delle frecce (modello a striscia) dà 13,7 e 9,4 mm e non è coerente con i 2,7 e 1,5-2 mm riportati, che sono i valori del fornitore.

### Testo proposto (sostituisce il calcolo delle frecce nel §4)

> La verifica del coperchio interpiano è stata eseguita dal fornitore del pannello (CEL, pannello sandwich POLISTEP da 25 mm, pelli in vetro/epossidica, anima in polipropilene alveolare), rapporto n. … del … . Condizioni verificate: carico distribuito di 200 kg sul foro libero Ø1100 mm, pannello Ø1200 mm appoggiato sul bordo su guarnizione morbida; carico concentrato di 100 kg su 100 × 100 mm. Esito del fornitore: tensione di flessione 32,5 MPa (ammissibile …, fonte …); taglio nell'anima 45 kPa (ammissibile 500 kPa); freccia 2,7 mm con carico distribuito e 1,5-2 mm con carico concentrato, contro un limite di L/200 = 5,5 mm (criterio: …). Le condizioni al contorno del rapporto corrispondono a quelle di montaggio. I valori sono quelli del rapporto del fornitore e non sono ricalcolati nella presente relazione.

### Controlli prima di citare il rapporto

1. Il rapporto copre i due casi di carico (200 kg distribuiti e 100 kg su 100 × 100 mm) con la stessa luce e lo stesso vincolo del montaggio.
2. Il limite di freccia L/200 e il valore ammissibile di 120 MPa hanno una fonte scritta (norma o scheda tecnica).
3. Se il calcolo della striscia (M = 312 Nm, σ = 32,5 MPa) resta nella relazione, indicarlo come stima prudente delle sole tensioni e togliere le frecce che ne derivano.

## 4. Carichi su guide e trave superiore (DOC-20)

Scelte del progettista (2026-11-09): cinghie 35 kg; masse della struttura in testa confermate (trave 210, parti elettriche 30, motoriduttore 21, batterie 18); pulegge di trazione coassiali alle guide; CP.TEC.025 non aggiornata per ora.

### 4a. §1.1 Masse e carichi sotto le guide

Elenco unico delle masse: guide 120, trave superiore 210, parti elettriche 30, motoriduttore 21, batterie 18, contrappesi 60, **cinghie 35**, cabina 160, portata 200. Somma 854 kg; masse fisse (senza cabina e portata) 494 kg.

> Carico sotto ogni guida in funzionamento normale = 854 / 2 = **427 kg**.
> Carico sotto ogni guida in intervento paracadute = ((854 − 200 − 160) + 720) / 2 = (494 + 720) / 2 = **607 kg**.

La relazione scrive 800 / 2 = 400 kg e 580 kg. Il valore di CP.TEC.025 («580 daN sotto ogni colonna») resta invariato per scelta; la differenza con 607 è del 4,7%.

### 4b. §3 Guide: carico di compressione per guida

Sostituire «m = 1015 kg» con:

> Carico verticale su ogni guida in intervento paracadute: struttura in testa (210 + 30 + 21 + 18) / 2 = 139,5 kg; contrappesi e cinghie appesi (60 + 35) / 2 = 47,5 kg; reazione del paracadute WD / 2 = 900 kg (k1 = 5 su cabina e portata); totale 1087 kg, cioè 10,66 kN. Peso proprio della guida 0,58 kN. Ntot = 11,24 kN.

| Caso | Ncr | SF (relazione) | SF corretto |
|---|---|---|---|
| A, incastro-libero (L efficace 14 m) | 21,74 kN | 2,06 | **1,93** |
| B, incastro-carrello (L efficace 4,9 m) | 177,5 kN | 16,85 | **15,8** |

### 4c. §9 Trave superiore

> Le pulegge di trazione sono coassiali alle guide: i carichi verticali delle cinghie (4,1 kN in statico) si scaricano direttamente sulle guide e non sollecitano la trave a flessione. La trave porta le masse proprie dell'apparecchiatura (motoriduttore 21 kg, batterie 18 kg, rinvio e accessori 21 kg, totale 60 kg), verificate a mezzeria.

> **Momento di coppia sulla trave.** Gli alberi delle pulegge sono perpendicolari all'asse della trave: la differenza di tensione fra i due rami di ogni cinghia (fino a Cp/2 = 327 Nm per puleggia in frenata di emergenza) è un momento flettente nel piano verticale della trave, trasmesso alle guide. A favore di sicurezza si assume come momento massimo l'intera coppia Cp = 654 Nm, sommata al momento dei 60 kg a mezzeria (147 Nm): Mtot = 801 Nm. La sezione è chiusa da carter e piastre, che non sono considerati nel calcolo. σ = Mtot / Winf = 801.000 / 9741 = 82 MPa.
>
> Per il DC01 si adotta Rp0,2 = 140 MPa (valore cautelativo, come nel §2.4: il 280 MPa è il massimo ammesso dalla norma e non un minimo garantito). Coefficiente di sicurezza = 140 / 82 = **1,7**. Nessun momento torcente: la verifica a torsione resta non rilevante.

Correggere nella verifica a flessione già presente: σ = 15,1 MPa (60 kg a mezzeria) con Rp0,2 = 140 MPa dà coefficiente 9,3 (non 18,5 come con 280 MPa); la verifica combinata Von Mises coincide.

**Nota sul materiale (Tab.3 e §9):** i valori del DC01 riportati in Tab.3 (Rm 410, Rp0,2 280 MPa) sono i valori massimi di norma. Per le verifiche usare quelli minimi cautelativi (Rm 270, Rp0,2 140 MPa), come già nel §2.4, e riportare la dicitura nella tabella.

## 5. §8 Verifica motoriduttore (DOC-21, DOC-28, DOC-29)

Fonti lette il 2026-11-09: Amer, «DCM-6329 Rev.3: verifica statica e a fatica» (allegato alla e-mail di F. Baldisserotto del 03/03/2026), disegno DCM-6329 Rev.3 (03/12/2025, modificato 11/02/2026), dati di targa del freno 48 Vdc 10 Nm (stessa e-mail), scheda sensore 7EBLR002 Rev.2.0. Il §8 della relazione riporta il testo del rapporto Amer senza citarlo e senza le sue premesse.

### 5a. §2 Dati del motoriduttore: da completare

> Dati di targa (Amer, DCM-6329 Rev.3): MRP19BL, **34 Vac, 600 W, 30 giri/min**; rapporto di riduzione 25,3:1; rendimento del riduttore 0,92; albero di uscita Ø35 h7 con linguetta 10×8×35. Freno doppio 48 Vdc, 10 Nm statici nominali (14 Nm massimi) ciascuno, 24 W, 0,50 A, attrazione ≤ 35 V, rilascio ≥ 1 V.

La relazione indica oggi solo «600 W @ 30 RPM». Aggiungere la tensione (34 Vac, cioè il bus a 48 Vdc) e citare il documento Amer con titolo, revisione e data: il rapporto è dell'e-mail del 03/03/2026 e il disegno è stato modificato l'11/02/2026, mentre la relazione Rev.0 porta la data 12/01/2026. Confermare che il testo del §8 coincide con la versione aggiornata (nel confronto fatto il 2026-11-09 coincide).

### 5b. §8 Origine delle coppie di tabella

Sostituire la frase che introduce la tabella con:

> Le coppie in uscita al riduttore sono quelle del rapporto Amer «DCM-6329 Rev.3: verifica statica e a fatica». Origine (dal rapporto): nelle prove di settembre 2025 un motoriduttore MRP11 (26,8:1, η 0,92) ha sollevato 330 kg a 0,10 m/s con 204,6 Nm in uscita, ricavati dalla curva motore a 10,3 Aac; per la specifica 340 kg a 0,15 m/s la coppia è 210,8 Nm, aumentata in modo cautelativo a 222 Nm. Le coppie di spunto e di discesa (260, 147, 195 Nm) sono stime di Amer.
>
> Confronto con i carichi dell'Elevance (raggio primitivo Rp = 0,0509 m, massa non bilanciata = cabina 160 kg + carico − contrappesi 60 kg): coppia teorica in salita 150 Nm con Q = 200 kg, 180 Nm con Qsl = 260 kg, **187 Nm con Qwl = 275 kg**. La coppia di 222 Nm copre il caso peggiore con un margine del 19% per attriti e accelerazione; 260 Nm copre lo spunto con il 39%.

Valori verificati dal rapporto Amer e dalle masse e dal raggio della relazione: 150, 180, 187 Nm (calcolo: g · (100 + Q) · Rp). Dedotto da me e da confermare con Amer: la coppia ideale implicita nella coppia salita/discesa di tabella è (222 + 147) / 2 = 184,5 Nm, cioè il caso Qwl = 275 kg (187 Nm) con perdite costanti di ±37,5 Nm (circa 20%). Il rapporto non spiega 260, 147 e 195 Nm.

### 5c. §8 Potenza e servizio

Il rapporto Amer usa 30 giri/min in uscita; a 0,15 m/s con Rp = 0,0509 m servono **28,1 giri/min** (a 0,10 m/s: 18,7), quindi la scelta dei 30 giri/min è cautelativa per la fatica.

| Coppia in uscita | Potenza a 0,15 m/s (28,1 giri/min) | Potenza a 0,10 m/s |
|---|---|---|
| 147 Nm (discesa) | 433 W | 289 W |
| 222 Nm (salita) | **654 W** | 436 W |
| 260 Nm (spunto) | 766 W | 510 W |

La targa di 600 W a 30 giri/min corrisponde a 191 Nm (204 Nm a 28,1 giri/min). Quindi la salita al carico di 275 kg a 0,15 m/s supera la targa del 9%, lo spunto del 28%. Con Q = 200 kg la coppia stimata (150 Nm teorici + perdite del 20% = 180 Nm) dà circa 530 W, sotto la targa.

Testo da inserire:

> Il servizio è intermittente: una corsa di 4,5 m a 0,15 m/s dura 30 s. Con 300 corse al mese (riferimento d'uso residenziale) il motore lavora 2,5 h al mese. Il costruttore (Amer) conferma che la potenza di 600 W è ammessa [in servizio S1 / S3 …, **da confermare**] e che la corrente a 222 Nm (circa 11,2 A, con 10,3 A a 204,6 Nm misurati) e a 260 Nm (circa 13 A, stima proporzionale) è ammissibile per i tempi indicati.

Dedotto: 11,2 A e 13 A sono proporzionali alla coppia (10,3 A × 222/204,6 e × 260/204,6); la curva di Amer può dare valori diversi. **Da chiedere ad Amer**: servizio nominale della targa (S1 o altro), curva coppia/corrente a 34 Vac e a tensione di bus ridotta (batteria a 38-42 V), conferma di 30 giri/min a bus basso con 222 Nm. Collegato a PRV-11.

### 5d. §8 Cuscinetti: confronto con la tabella SKF

Il rapporto Amer dà s0 = 1,15 alla coppia di frenatura massima (654 Nm) e rimanda alla Tab.7 del catalogo SKF, che la relazione non riporta. Riportarla e aggiungere:

> La Tab.7 SKF (cuscinetti a sfere, carichi continui o occasionali) prescrive per movimento poco frequente con deformazione permanente accettabile s0 ≥ 0,4 (elevata certezza del carico) o ≥ 1 (bassa certezza, carichi di picco). La frenatura dinamica a 654 Nm avviene al massimo 60 volte nella vita: s0 = 1,15 ≥ 1, verificato.
>
> In esercizio continuo il carico è proporzionale alla coppia: con 260 Nm (spunto) s0 ≈ 1,15 × 654 / 260 = 2,9 e con 222 Nm s0 ≈ 3,4, entrambi maggiori di 2, il valore massimo della tabella (movimento continuo, nessuna deformazione accettabile).

Il calcolo con 654/260 è una scala lineare mia (dedotto): confermarla con Amer. Vita dei cuscinetti: Lnmh = 6680 h contro 5000 h richieste (rapporto 1,34). Le 5000 h valgono 167 anni di uso a 300 corse al mese (2,5 h/mese): la vita richiesta non limita il progetto. Per i cuscinetti 6007 e 6006 delle pulegge (cap. 6 della relazione) il suffisso 2Z va aggiunto alla distinta e alla relazione (risposta del progettista, DOC-23).

### 5e. §2 e §8 Tenuta del freno alla coppia minima (nuova verifica, DOC-28)

La relazione definisce Cmin = 10 + 10 Nm ma non la usa. La norma citata nel §5 (EN 81-41, 5.4.2.1) richiede che il freno tenga la cabina con il carico statico massimo. Proposta di verifica a favore di sicurezza (rendimento applicato come perdita):

> Coppia di tenuta minima all'uscita = n° freni × Cmin × RR × η = 2 × 10 × 25,3 × 0,92 = **465 Nm**. Carico statico massimo (Qwl = 275 kg): 187 Nm. Coefficiente di sicurezza = 465 / 187 = **2,5** (2,6 con Qsl = 260 kg).

Dedotto: il fattore richiesto dalla norma per la tenuta non è stato verificato da me; indicare nella relazione il criterio adottato.

### 5f. §8 Mozzo, linguetta, norme e prova di validazione (DOC-28)

Il rapporto Amer dice che «mozzo e linguetta sono componenti definiti e montati dal cliente» e dà i coefficienti sulla chiavetta con mozzo e linguetta in C45 (1,48 e 1,52 a 654 Nm); il calettamento dà 1,23. Aggiungere:

> I coefficienti di sicurezza della linguetta 10×8×35 (1,48) e del mozzo (1,52) sono validi per mozzo e linguetta in C45 o migliore. Il mozzo dell'ingranaggio di ingresso (z = 40, m = 3) è in C45 [**da confermare in disegno**]; la linguetta è in C45. Il rapporto Amer è valido solo per i dati di input indicati e deve essere seguito da prove di validazione: sulla macchina pilota sono state eseguite le prove di validazione: (1) prova di carico a 275 kg in salita e discesa a 0,15 m/s con misura della corrente del motore, (2) prova con batteria scarica (bus a tensione ridotta), (3) prova di frenatura di emergenza a carico nominale e a 275 kg. Riferimenti: verbale n. [da completare], data [da completare]; corrente misurata a 275 kg in salita [da completare] A (attesa da Amer: circa 11,2 A a 222 Nm).

Amer non ha applicato norme di settore («da indicare a cura del cliente»): la relazione deve dire quali criteri accetta per i coefficienti di sicurezza statici del motoriduttore (1,23 calettamento, 1,48 e 1,52 linguetta e mozzo).

### 5g. §8 Numero di frenature dinamiche (DOC-29)

Il rapporto Amer ipotizza 60 frenature dinamiche in 5 anni (1 al mese). I manuali prevedono prove periodiche con la cabina in movimento (controllo semestrale del tecnico, prove a carico dell'utente anche della discesa di emergenza: IM.TEC.159 e 161). Aggiungere nella relazione il conteggio degli interventi attesi (emergenze, prove, mancanza di rete) e, se supera 60, chiedere ad Amer di confermare l'esito. Sensibilità: i coefficienti statici non dipendono dal numero di eventi; la fatica degli ingranaggi (1E6 h) ha un margine enorme.

## Riepilogo dei dati da completare prima della Rev.1

1. Numero del rapporto di prova di tipo del paracadute (§12).
2. Numero e data del rapporto CEL sul coperchio; fonte di L/200 e 120 MPa (§4).
3. Scelta 50/50 o 60/40 per la ripartizione del carico sugli ingranaggi (§7).
4. Dal costruttore del motoriduttore: servizio nominale dei 600 W, curva a bus ridotto, derivazione di 260/147/195 Nm (§8).
5. Numero e data dei verbali delle tre prove di validazione, corrente misurata a 275 kg e valori a bus ridotto; conteggio delle frenature dinamiche (§8). Mozzo in C45 e linguetta 10×8×35 confermati dal progettista il 2026-11-09. Linguetta in C45 (confermato).

## 6. Cinghie: massa lineare e corsa di riferimento (DOC-15)

Scheda tecnica Megadyne con la massa lineare non trovata: né il calcolo MFC 20260211-1 né il rapporto RI 007-26 la riportano, e la ricerca sul web non ha dato un valore per la Megalinear 50 RPP8 con anima in acciaio. Il controllo che segue è quindi geometrico (dedotto), da confermare con la scheda Megadyne o pesando uno spezzone.

### 6a. Massa lineare: il valore plausibile è 0,33 kg/m

Sezione di una cinghia 50 mm con passo 8 mm: circa 50 × 5,5 mm = 275 mm². Con poliuretano a 1,2 g/cm³ (i cavi d'acciaio sono pochi e piccoli) si ottengono circa **0,33 kg/m**. Per avere 3,3 kg/m servirebbe una sezione di 2750 mm², dieci volte più grande: impossibile per una cinghia da 50 mm.

La relazione ha quindi un solo valore sbagliato:

- «Massa = 0.33 kg/m» e la nota di esclusione («μ = 0,33 kg/m, L ≤ 10 m ⇒ m_b ≤ 3,3 kg per linea», meno dell'1,5% di circa 250 kg): coerenti tra loro.
- «Massa lineare (Megalinear RPP8 50) = 3.3 kg/m» e «6 × 3,3 = ~20 kg» (verifica dell'allentamento delle cinghie): refuso, da correggere in **0,33 kg/m** e **6 × 0,33 = ~2 kg**.

Testo proposto per quel passaggio:

> Massa lineare (Megalinear RPP8 50, dato Megadyne) = 0,33 kg/m. Massa cinghia libera max = 6 × 0,33 ≈ 2 kg.

### 6b. Massa totale delle cinghie negli elenchi (50 kg e 35 kg)

Con 0,33 kg/m, 35 kg corrisponderebbero a circa 106 m di cinghia, e 50 kg a circa 150 m. Le masse degli elenchi (§1.1 «50 kg», verifica guide «35 kg») non sono coerenti con il valore corretto. La massa totale va ricalcolata dallo sviluppo reale delle due cinghie (somma dei rami fra capocinghia, pulegge e contrappesi, compreso il gruppo di attacco da 1 kg) e riportata in un solo punto. Con 0,33 kg/m è dell'ordine di pochi chilogrammi. Conseguenza sulla relazione, se confermato: i carichi sotto le guide (DOC-20, §4a e 4b) usano 35 kg e restano **a favore di sicurezza**; ricalcolarli con la massa reale abbassa il carico di circa 13 kg per guida. Lo squilibrio di massa fra ramo cabina e ramo contrappeso lungo la corsa è di pochi chilogrammi: trascurabile.

### 6c. Tab.1: corsa di riferimento

La Tab.1 dichiara «Corsa 3000 mm (corsa massima = 4500 mm)» e «tutti i parametri sono quelli massimi accettabili». I calcoli usano invece la corsa massima: guide 60 + 60 kg «corsa max 4,5 m», lunghezza di libera inflessione di 7 m, cinghia libera massima 6 m. Testo proposto:

> **TM** Corsa massima verificata: 4500 mm (la macchina di riferimento VR260038 ha corsa 3000 mm; le verifiche delle guide, delle cinghie e dei rinvii sono eseguite con la corsa massima).

Il calcolo MFC 20260211-1 di Megadyne non dipende dalla corsa. Le verifiche che ne dipendono sono: masse e instabilità delle guide, lunghezza e massa delle cinghie, velocità e tempi di corsa.
