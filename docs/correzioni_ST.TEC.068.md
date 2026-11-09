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
