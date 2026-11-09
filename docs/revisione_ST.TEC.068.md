# Revisione della relazione tecnica ST.TEC.068 Rev.0 (12/01/2026)

Data della revisione: 2026-11-08 · Destinatario: il progettista della relazione · Voci del registro: **DOC-15 … DOC-22** (DOC-15 e DOC-16 già aperte nella prima lettura).

## 1. Scopo e metodo

- Ho letto l'intera relazione (43 pagine, 12 capitoli) e ricalcolato con un programma tutti i valori numerici ricavabili dal testo.
- **Non ho avuto a disposizione:** le figure, i rapporti di prova (pannello CEL POLISTEP, Megadyne), le schede di catalogo (SKF, AMER, Megadyne), il disegno del paracadute e la tabella SKF citata al capitolo 8. Non ho verificato i valori di catalogo.
- Distinguo tre tipi di rilievo: **errore o valore non riproducibile** (da correggere), **ambiguità o lacuna** (da chiarire o dimostrare), **conservativo** (l'errore va a favore della sicurezza, da sistemare per pulizia).
- Questa è una revisione tecnica di lettura e ricalcolo, non una verifica indipendente ai sensi di norma.

## 2. Esito in sintesi

| Priorità | Rilievo | Capitolo | Voce |
|---|---|---|---|
| **Alta** | Taglio sulla guida di ingaggio del paracadute: ricalcolo 138-141 MPa contro 1,2 MPa riportati; limite 235 MPa non è quello dell'alluminio | 12 | DOC-17 |
| **Alta** | Coppia frenante di emergenza: 554 Nm (η moltiplicato) nei capitoli 5, 7 e 10 contro 654 Nm (η a denominatore) nel capitolo 8; a 654 Nm il dente dell'ingranaggio scende a SF 0,97 | 5, 7, 8, 10 | DOC-18 |
| Media | Coperchio interpiano: frecce riportate non riproducibili col modello scritto (13,7 mm e 9,4 mm contro 2,7 mm e 1,5-2 mm) | 4 | DOC-19 |
| Media | Carichi sulle guide non riproducibili (1015 kg; 800 kg contro 869 kg) e carichi orizzontali dichiarati «trascurabili» senza dimostrazione | 1.1, 3 | DOC-20 |
| Media | Motoriduttore: coppie di tabella non riconducibili ai carichi; potenza a 0,15 m/s oltre la targa; tabella SKF assente | 8 | DOC-21 |
| Media | Massa lineare della cinghia 0,33 contro 3,3 kg/m; corsa di riferimento | 5, 6, 1.1, Tab.1 | DOC-15 |
| Bassa | Metodo e refusi: modello con a > g, carico radiale dei rinvii, verifica dente nominale non divisa per 2, giunzione senza carico d'esercizio, refusi numerici | 5, 10, 11 e altri | DOC-16, DOC-22 |

**Cosa è corretto.** Quasi tutti i passaggi aritmetici tornano al ricalcolo (cinghia, freno, viti, trave di cabina, trave superiore, Eulero delle guide, cuscinetti). Molte scelte vanno a favore della sicurezza (sezione 4). La struttura del documento è ordinata: ipotesi dichiarate, riferimenti normativi, tabella dei coefficienti.

## 3. Verifica capitolo per capitolo

### Cap. 1 · Dati generali

| Voce | Relazione | Ricalcolo | Nota |
|---|---|---|---|
| Portata da area | 250 kg/m «senza ²» → 200 kg | 250 kg/m² × 0,785 m² = 196 kg | conforme; correggere l'unità |
| QC = 1,25 × QSL | testo 325 kg; Tab.2 **315 kg, 3,01 kN** | 325 kg = 3,19 kN; 3,01 kN = 307 kg | Tab.2 incoerente col testo; WC = 485 kg è coerente col testo (160 + 325) |
| QWL | 275 kg → 2,67 kN | 2,70 kN | refuso lieve |
| Gli altri valori di Tab.2 | | tornano | |
| Somma masse §1.1 | «800 kg» | 869 kg (guide 120, trave 210, parti elettriche 30, motoriduttore 21, batterie 18, contrappesi 60, cinghie 50, cabina 160, portata 200) | vedi DOC-20 |
| Carico per guida a paracadute | ((800−200−160)+720)/2 = 580 kg | masse fuori da cabina e portata = 509 kg → (509+720)/2 = 615 kg | 6% più alto; 580 è ripreso in CP.TEC.025 |

### Cap. 2 · Cabina

| Verifica | Relazione | Ricalcolo | Esito |
|---|---|---|---|
| Trave di base | I = 58.992 mm⁴; σ = 78,1 MPa; f = 1,82 mm | I = 58.912 mm⁴ (46.656 × 26 = 1.213.056, non 1.212.096); σ = 78,2 MPa; f = 1,82 mm | corretta; differenza 0,14% |
| Carico q | «200 kg/m, include il coefficiente di impatto» | 2 travi × 0,969 m × 200 kg/m = 388 kg = 1,5 × 260 kg | esplicitare la derivazione |
| Freccia | «verificata secondo EN 81-41» | 1,82 mm; limite non scritto | indicare il limite |
| Viti base (§2.3) | SF 8,2 | SF 8,19 | corretto; il testo dice «un solo lato» ma m = 1400 kg è il carico di entrambi i lati con n = 8 (equivale a 700 kg su 4 viti) |
| Montanti (§2.4) | σ = 4,22 MPa | 6082 N / 1440 mm² = 4,22 MPa | corretto |
| Viti cinghia (§2.5) | SF 11,9 | SF 11,94 | corretto: 360 kg = (360/2) × 2 per cinghia |

### Cap. 3 · Guide

- **Eulero:** Ncr,A = 21,74 kN (λ = 312), Ncr,B = 177,5 kN (λ = 109), i = 44,83 mm, massa guida 58,8 kg. Tutto torna.
- **Carico assiale:** «m = 1015 kg» deriva da un elenco che somma **590 kg** (120 + 35 + 60 + 15 + 200 + 160); anche con coefficiente 2 sulla sola cabina e portata si arriva a 950 kg. Inoltre «suddiviso equamente sulle due guide», ma Next = m·g è usato per intero su una guida. Il carico non è riproducibile.
- **Il risultato resta coperto:** con WD = 1800 kg di Tab.2 diviso per due guide, più il peso proprio, si ha 9,41 kN, minore dei 10,53 kN usati. Quindi l'esito non cambia, ma l'input va reso tracciabile.
- **Lacuna:** si verifica solo la compressione assiale. Non sono verificati flessione delle guide per i carichi orizzontali (il testo dice «trascurabili» ma una cabina tonda con persone fuori asse genera reazioni sui pattini), freccia delle guide, sforzi dei fissaggi. Il paracadute applica forze sulla guida lungo il suo asse che la relazione non traduce in flessione.

### Cap. 4 · Coperchio interpiano

| | Relazione | Ricalcolo (modello della relazione: trave L = 1,10 m, EI = 24.000 MPa × 120.000 mm⁴/m) |
|---|---|---|
| Momento, tensione, taglio | 312 Nm, 32,5 MPa, 1136 N, 45 kPa | tornano |
| **Freccia, carico distribuito** | **2,7 mm** | **13,7 mm** (limite L/200 = 5,5 mm; L/300 = 3,67 mm) |
| Momento, tensione con 100 kg | 270 Nm, 28 MPa | tornano |
| **Freccia, carico concentrato** | **1,5-2 mm** | **9,4 mm** |

- Con un modello a piastra circolare appoggiata (Ø1100 mm, stesso EI, senza deformazione a taglio dell'anima) la freccia sotto 200 kg è circa **3,8 mm**: più vicina a quanto riportato, ma il modello non è quello scritto e va aggiunta la deformazione a taglio dell'anima alveolare.
- In ogni caso 2,7 mm non è ricavabile dal modello dichiarato. Se la freccia reale è quella da piastra, oltre i 3,67 mm del limite L/300 il coefficiente è sotto 1. La relazione rimanda ai rapporti di prova del pannello: citare il valore misurato.
- Il limite 120 MPa («σamm») non ha fonte; per le pelli in vetro/epossidica va indicata la resistenza del laminato.

### Cap. 5 · Cinghie e freni

| Verifica | Relazione | Ricalcolo | Nota |
|---|---|---|---|
| Rottura cinghia | SF 15,7 (Tbr 32.300 / 2060 N) | 15,68 | conservativa: Tst somma il contrappeso (30 kg) al ramo cabina, il ramo cabina vale 1766 N (SF 18,3) |
| Coppia frenante alla puleggia | **554 Nm** = 25,3 × 0,92 × 28 × 0,85 | 553,97 | **diversa dal cap. 8: 654,2 Nm** (vedi DOC-18) |
| ΔT sul dente (emergenza) | 5431 N, SF 10,4 | 5431 N; con 654 Nm: 6417 N, SF 8,8 | accettabile in entrambi i casi |
| Decelerazione di emergenza | 14,5 m/s² | 14,51 | vedi sotto |
| Tstop e Sstop | 0,01 s e 0,77 mm | 0,0103 s; 0,77 mm | la formula scritta «0.15/14.5» dà 0,0103, non 0,77 mm |
| **Dente a frenata nominale** | **ΔTstd = 3730 N (totale), SF 1,2** | 3680 N + 62 N ≈ 3740 N totali, cioè **1870 N per cinghia**; capacità 4470 N **per cinghia** → **SF 2,4**; con i 21 denti reali **4,2** | nei capitoli 5 (emergenza) e statico il carico è per cinghia, qui no: errore a favore della sicurezza |
| Dente statico | 2060 N, SF 2,2 | 2,17 | corretto |
| Carico max di trazione | SF 3,8 | 3,78 | corretto |
| Stazionamento freno | Cs 180 Nm; SF 1,4 | 179,8 Nm; 253/179,8 = 1,41 | corretto; la formula riporta «+ Rp» al posto di «· Rp» e «− Mcwt» al posto di «− 2·Mcwt» |

**Modello di frenata di emergenza (osservazione di metodo).** Con a = 14,5 m/s² maggiore di g, il ramo scarico della cinghia non può essere teso: per 180 kg sul ramo cabina la tensione sarebbe 4376 N (a = 14,5), mentre il limite imposto dal freno è 5431 N, cui corrisponde a ≈ 20 m/s². Il modello rigido «ΔT = g(…) + a(…)» non è fisicamente coerente. Gli esiti sono comunque limitati dalla coppia del freno (ΔT = 5431 N), quindi conviene usare ΔT come carico sul ramo cabina in tutti i controlli a valle (viti M12 cabina: SF 9,9 invece di 12,3). Il ramo contrappeso si allenta e poi riprende tensione con un urto: l'urto sul contrappeso da 30 kg è modesto rispetto alla vite M10 (SF 51).

### Cap. 6 · Allentamento cinghie

Il calcolo torna (210 N × cos 86° = 14,7 N; 15,2 N; 7,6 N per molla; 32,2 N disponibili, SF 2,1). Due note: il testo scrive «30 × 0,46 = 16,1 N» mentre 16,1 = 35 mm × 0,46 (la precompressione è 35 mm); il carico 210 N usa 3,3 kg/m (DOC-15), con 0,33 kg/m il margine aumenta.

### Cap. 7 · Ingranaggi e collegamento albero-mozzo

| Verifica | A 554 Nm (relazione) | A 654,5 Nm (coppia del cap. 8) |
|---|---|---|
| Dente (Lewis, Y = 0,39, σlim 300 MPa) | 263 MPa, **SF 1,14** | 310 MPa, **SF 0,97** |
| Linguetta a taglio (τlim 173 MPa) | 90,5 MPa, SF 1,91 | 107 MPa, SF 1,62 |
| Linguetta a pressione (300 MPa) | 226 MPa, SF 1,33 | 267 MPa, **SF 1,12** |
| Albero a torsione | 65,8 MPa, SF 2,63 | 77,8 MPa, SF 2,22 |

L'aritmetica è corretta e la formula di Lewis è usata correttamente (Y = 0,389 per z = 40, 20°). **Domanda al progettista:** l'ingranaggio sincronizza le due pulegge controrotanti. La coppia che lo attraversa è tutta la coppia di frenatura all'uscita del riduttore o la quota di una sola puleggia (circa metà)? Nel secondo caso i coefficienti raddoppiano; nel primo, con 654 Nm, il dente è sotto 1 col criterio statico scelto. Vedi DOC-18.

### Cap. 8 · Motoriduttore

- **Coppie di tabella** (salita 222 Nm, spunto 260, discesa 147, spunto 195): non sono riproducibili dai carichi del documento. La coppia teorica in salita con 275 kg è 187 Nm (149,9 Nm a 200 kg; 179,9 Nm a 260 kg). Il testo non dice come sono ottenute (sovraccarichi, accelerazione, attriti).
- **Potenza a 0,15 m/s:** a 222 Nm e 28,1 rpm la potenza meccanica è **654 W**, oltre i **600 W** di targa (la coppia di targa a 30 rpm è 191 Nm). Allo spunto (260 Nm) sono 766 W. A 0,10 m/s sono 436 W. Va chiarito se la targa 600 W vale in servizio intermittente e come si comporta il motore a 48 V con la batteria che cala.
- **Cuscinetti del riduttore:** Lnmh = 6680 h contro 5000 h richieste (margine 1,34). Statico s0 = 1,15 «da confrontare con la tabella SKF»: **la tabella non è riportata**, il valore richiesto non è scritto.
- **Freno:** vedi 654 Nm in DOC-18; 60 interventi dinamici a vita in confronto con gli eventi reali: NOR-06 già aperta.

### Cap. 9 · Trave superiore

Geometria e risultati tornano (A = 882 mm², yG = 16,5 mm, Ix = 306.841 mm⁴, σ = 15,1 MPa, f = 0,19 mm). **Lacuna:** la verifica considera solo 60 kg in mezzeria. Si dichiara che i carichi delle cinghie si controbilanciano e scaricano sulle guide, ma non si dimostra: le due pulegge portano in totale (160 + 200 + 60) kg = 4,1 kN statici e circa il doppio in emergenza, e la coppia di reazione del riduttore (554-654 Nm) viene scaricata nella trave. Servono posizione delle pulegge rispetto agli appoggi e un calcolo della reazione.

### Cap. 10 · Cuscinetti

I numeri tornano (C/P = 6,18 e 5,08; L10h = 131.555 h e 72.800 h). **Metodo:** per i rinvii il carico radiale è la somma vettoriale delle tensioni dei due rami (T·2·sin(α/2)), non la differenza ΔT usata. Con 4376 N per ramo e deviazione di 90° si hanno circa 6200 N, 3100 N per cuscinetto: s0 = 2,7 invece di 3,06. L'esito non cambia ma il criterio va corretto. La vita è calcolata col carico di emergenza applicato sempre a 30 rpm: conservativa.

### Cap. 11 · Unione cinghie e collegamenti

- **Piastra:** «9 × 4700 = 32.900 N» ma 9 × 4700 = 42.300 N; 32.900 è 7 × 4700. Il risultato (SF 6,1) è quello coi 7 denti minimi, quindi conservativo: correggere il testo.
- **Manca il confronto con il carico d'esercizio:** si usa solo la resistenza del dente in emergenza (940 N/cm, max 50 volte). Va chiarito con Megadyne quale valore vale per il fissaggio a piastra con carico continuo (2134 N con 275 kg).
- **Vite contrappeso M10:** SF 50,9 (F = 729 N), corretta; vite cabina M12: SF 12,3 con 4376 N, **9,9 con 5431 N** (vedi cap. 5); piastra alternativa: SF 23 (4376 N), 18,6 con 5431 N.

### Cap. 12 · Paracadute

| Verifica | Relazione | Ricalcolo |
|---|---|---|
| Perno Ø14: area, Wx | 154 mm², 269 mm³ | 153,9 mm², 269,4 mm³ |
| Taglio perno cabina | 77 MPa | 76,4 MPa (corretto) |
| **Taglio guida di ingaggio cabina** | **1,2 MPa** | **τ = 3T/(2bh) con T = 9000 N: 140,6 MPa; con T = 8829 N: 138,0 MPa; se la sezione è «doppia rettangolare» (area 2bh): 69,0 MPa** |
| Limite usato | τP(0,2) = 235 MPa | l'ingaggio avviene su una barra in lamiera DD11 EN 10111 spessa 4 mm (disegno F500.05.5001), non su alluminio né su S235: del DD11 si garantisce un ReH minimo di circa 170 MPa (da confermare sul certificato), cioè un taglio al limite di circa 98 MPa |
| Taglio perno contrappeso | 7 MPa | 6,4 MPa (corretto, arrotondato per eccesso) |
| **Taglio guida di ingaggio contrappeso** | **0,3 MPa**, calcolato «con 9000 N» | con 9000 N: 140,6 MPa; con 736 N: 11,5 MPa |

- I due valori di taglio non sono riproducibili. Il disegno F500.05.5001 mostra una barra DD11 di 4 mm, larga 36,5 mm e lunga 600 mm, con 10 finestre da 36 × 13 mm e feritoie da 30,2 × 4,2 mm, saldata con cordoni z2 10-60 alla guida F500.05.5002: la verifica va rifatta su questa geometria (taglio del ligamento di 6 mm sotto la finestra, pressione sul bordo della finestra, cordoni di saldatura, fissaggio al profilo). Con b = 4 mm e h ≈ 24 mm (5,3 + 13 + 6 = 24,3) e T = 8829 N il taglio vale 138 MPa, contro circa 98 MPa del DD11. (Nella prima stesura avevo ipotizzato un ingaggio sull'alluminio: non è così.)
- Wx è calcolato e **non usato**: manca la verifica a flessione del perno, che porta il carico tramite bilanciere con braccio.
- Nel paracadute del contrappeso il testo dice «sostiene la pedana» (refuso per «contrappeso»).
- Il paracadute è un componente di sicurezza con esame di tipo: la relazione è complementare alle prove di tipo, che hanno la priorità; ma la relazione non deve contenere valori in contraddizione con esse.

## 4. Verifiche che vanno a favore della sicurezza

- Dente continuo calcolato su 12 denti in presa invece dei 21 reali.
- Giunzione a piastra calcolata su 7 denti invece di 9.
- Tst con il contrappeso sommato al ramo cabina.
- Fr dei cuscinetti con la coppia massima di emergenza applicata in continuo a 30 rpm.
- Stazionamento del freno con un solo freno a coppia minima.
- Dente a frenata nominale: ΔTstd totale confrontato con la capacità di una sola cinghia (SF 1,2 contro 2,4 reale).
- Eulero sulle guide con L = 7 m e caso A «a mensola».

## 5. Domande al progettista

1. Qual è la guida di ingaggio del paracadute (materiale e sezione resistente) e da dove vengono 1,2 MPa e 0,3 MPa?
2. La coppia frenante di emergenza è 554 Nm o 654 Nm? Cosa trasmette l'ingranaggio di sincronismo?
3. Qual è la freccia misurata del pannello CEL POLISTEP sotto 200 kg e 100 kg, e quale limite si applica?
4. Da dove viene il carico di 1015 kg sulle guide e dove sono considerati i carichi orizzontali?
5. Come sono state ottenute le coppie 222/260/147/195 Nm e la targa 600 W vale a 0,15 m/s?
6. La massa lineare della cinghia Megalinear RPP8 50 è 0,33 o 3,3 kg/m?
7. La trave superiore viene verificata con le reazioni delle pulegge e del riduttore?

## 6. Raccomandazioni

1. Rifare il capitolo 12 e verificarlo con la prova di tipo del paracadute (priorità massima).
2. Unificare la coppia frenante e rifare le verifiche a valle (cinghia, ingranaggio, linguetta, cuscinetti, giunzioni, viti) con un solo valore.
3. Rifare le frecce del coperchio interpiano con modello a piastra e deformazione a taglio, o citare le misure di prova.
4. Rendere tracciabili i carichi su guide e trave; aggiungere flessione delle guide e reazioni della trave.
5. Correggere i refusi e il metodo dei rinvii; aggiungere la revisione indipendente (oggi redattore e approvatore coincidono) e allineare le citazioni EN 81-41:2024 e :2025.
6. Dopo le correzioni, emettere la Rev.1 e aggiornare il fascicolo; il firmware (FW-31) e la configurazione di prodotto (CP.TEC.025) citano valori della relazione.
