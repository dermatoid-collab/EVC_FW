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
