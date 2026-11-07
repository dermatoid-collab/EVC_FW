#!/usr/bin/env python3
"""Genera il diagramma «light» su energia, batterie e contrappesi di Elevance (SVG).

Uso:  python3 docs/gen_diagramma_energia.py docs/diagrammi/04.diagramma_energia_light.svg

Per il dipartimento commerciale: linguaggio semplice, nessun riferimento a codice o issue.
Tutti i numeri di consumo sono un ESEMPIO con ipotesi dichiarate nel diagramma (non misure).
Il recupero di energia in discesa non è dichiarato finché non è misurato (vedi DOC-11).
Capacità della batteria: 12 Ah, confermata dall'utente il 07/11/2026 (DOC-12).
Quando arrivano le misure sostituire gli esempi con i valori misurati (DOC-11).
"""
import html
import sys

W, H = 2400, 2150
o = []

TXT = '#1f2937'
SUB = '#4b5563'
GRN = '#2e8b57'
AMB = '#c27c0e'
RED = '#c0392b'
BLU = '#2563eb'
GRY = '#6b7280'
NAV = '#2a3b55'
PUR = '#7b3fa0'
ORA = '#d35400'

# ------------------------------------------------------------------ ipotesi e calcoli (esempio)
G = 9.81
CORSA = 3.0            # m (ipotesi)
M_CAB = 160.0          # kg, cabina (ST.TEC.068)
M_CW = 60.0            # kg, due contrappesi da 30 kg (ST.TEC.068)
M_PERS = 75.0          # kg, una persona (ipotesi)
Q_MAX = 200.0          # kg, portata
V = 0.10               # m/s nominale
ETA_TRAZ = 0.60        # rendimento batteria -> cinghia (ipotesi)
ETA_CARICA = 0.80      # rendimento rete -> batteria (ipotesi)
STANDBY_W = 1.0        # W, dato di prodotto da certificare
SALITE_DIE = 5         # salite al giorno (10 corse: 5 salite e 5 discese)
PREZZO = 0.25          # euro/kWh (ipotesi)
BAT_WH = 12 * 48       # 576 Wh nominali (12 Ah x 48 V)


def wh(kg, h=CORSA):
    return kg * G * h / 3600.0


def it(x, nd=1):
    return f'{x:.{nd}f}'.replace('.', ',')


loads = [('Cabina vuota', 0.0), ('Una persona (75 kg)', M_PERS), ('Carico massimo (200 kg)', Q_MAX)]
rows = []
for name, q in loads:
    w_con = wh(M_CAB + q - M_CW)
    w_sen = wh(M_CAB + q)
    rows.append((name, q, w_con, w_sen, 1 - w_con / w_sen))

e_up_con = wh(M_CAB + M_PERS - M_CW) / ETA_TRAZ        # Wh dalla batteria per una salita
e_up_sen = wh(M_CAB + M_PERS) / ETA_TRAZ
day_con = SALITE_DIE * e_up_con / ETA_CARICA + STANDBY_W * 24
day_sen = SALITE_DIE * e_up_sen / ETA_CARICA + STANDBY_W * 24
year_con = day_con * 365 / 1000
year_sen = day_sen * 365 / 1000
p_con = (M_CAB + M_PERS - M_CW) * G * V
p_sen = (M_CAB + M_PERS) * G * V
i_con = p_con / ETA_TRAZ / 48
i_sen = p_sen / ETA_TRAZ / 48
worst = wh(M_CAB + Q_MAX - M_CW, 4.5) / ETA_TRAZ        # peggior salita: 4,5 m, 200 kg, con contrappesi
trips_ratio = e_up_sen / e_up_con


def esc(s):
    return html.escape(s, quote=True)


def wrap(text, width_px, size):
    maxc = max(8, int(width_px / (0.585 * size)))
    words, lines, cur = text.split(), [], ''
    for w in words:
        if len(cur) + len(w) + (1 if cur else 0) <= maxc:
            cur = (cur + ' ' + w) if cur else w
        else:
            lines.append(cur)
            cur = w
    if cur:
        lines.append(cur)
    return lines


def T(x, y, s, size=13, anchor='start', weight='normal', fill=TXT):
    o.append(f'<text x="{x}" y="{y}" font-size="{size}" text-anchor="{anchor}" '
             f'font-weight="{weight}" fill="{fill}">{esc(s)}</text>')


def PARA(x, y, text, width_px, size=13, lh=None, fill=TXT, weight='normal'):
    lh = lh or size * 1.35
    lines = wrap(text, width_px, size)
    for i, l in enumerate(lines):
        T(x, y + i * lh, l, size, 'start', weight, fill)
    return y + len(lines) * lh


def BUL(x, y, text, width_px, color, size=13.5):
    o.append(f'<circle cx="{x + 5}" cy="{y - 4}" r="3.2" fill="{color}"/>')
    return PARA(x + 16, y, text, width_px - 16, size) + 5


def R(x, y, w, h, fill='#fff', stroke='#999', sw=1.5, rx=10):
    o.append(f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="{rx}" fill="{fill}" '
             f'stroke="{stroke}" stroke-width="{sw}"/>')


def L(x1, y1, x2, y2, color=GRY, w=3, dash=None, arrow=False, both=False):
    d = f' stroke-dasharray="{dash}"' if dash else ''
    ms = f' marker-start="url(#as{color[1:]})"' if both else ''
    me = f' marker-end="url(#ar{color[1:]})"' if arrow or both else ''
    o.append(f'<line x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}" stroke="{color}" stroke-width="{w}"{d}{ms}{me}/>')


def SECTION(y, title, color=NAV):
    o.append(f'<rect x="20" y="{y}" width="8" height="30" rx="3" fill="{color}"/>')
    T(40, y + 24, title, 21, 'start', 'bold', color)


o.append(f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}" '
         'font-family="DejaVu Sans, Arial, Helvetica, sans-serif">')
o.append('<defs>')
for c in (GRY, GRN, NAV, RED, BLU, ORA, AMB):
    o.append(f'<marker id="ar{c[1:]}" markerWidth="10" markerHeight="10" refX="9" refY="5" orient="auto">'
             f'<path d="M0,0 L10,5 L0,10 z" fill="{c}"/></marker>')
    o.append(f'<marker id="as{c[1:]}" markerWidth="10" markerHeight="10" refX="1" refY="5" orient="auto-start-reverse">'
             f'<path d="M0,0 L10,5 L0,10 z" fill="{c}"/></marker>')
o.append('</defs>')
o.append(f'<rect width="{W}" height="{H}" fill="#f7f8fa"/>')

# ------------------------------------------------------------------ intestazione
R(0, 0, W, 104, '#2a3b55', '#2a3b55', 0, 0)
T(30, 46, 'ELEVANCE — Energia, batterie e contrappesi', 32, 'start', 'bold', '#fff')
T(30, 78, 'Come la macchina si alimenta, si ricarica e risparmia energia · per il dipartimento commerciale', 17, 'start', 'normal', '#dbe4f0')
T(W - 30, 46, 'Versione light · aggiornata al 07/11/2026', 16, 'end', 'bold', '#fff')
T(W - 30, 72, 'Documento descrittivo, non contrattuale. I numeri di consumo sono un ESEMPIO con ipotesi dichiarate, non misure.', 14, 'end', 'normal', '#dbe4f0')
T(W - 30, 92, 'Fonti: CP.TEC.025 Rev.5, ST.TEC.068, manuali IM.TEC, analisi dei rischi Rev.7, firmware. Dati di prodotto = documenti; «esempio» e «stima» = da confermare.', 12, 'end', 'normal', '#c3d0e2')

# ------------------------------------------------------------------ 1. percorso dell'energia
SECTION(124, '1 · Il percorso dell\'energia')
flow = [
    ('Rete domestica', ORA, '#fff6ee', '230 V monofase, 2 A, 0,5 kW'),
    ('Caricabatteria', ORA, '#fff6ee', 'Ricarica la batteria solo a macchina ferma'),
    ('Batteria 48 V', GRN, '#effaf3', '12 Ah, AGM sigillata, senza manutenzione'),
    ('Quadro e azionamento', NAV, '#eef2f8', 'Decide e comanda il motore'),
    ('Motoriduttore', ORA, '#fff6ee', 'Brushless 48 V, con doppio freno a molle'),
    ('Cinghie dentate', GRY, '#f3f4f6', 'Due cinghie, una per ogni contrappeso'),
    ('Cabina e contrappesi', PUR, '#f6f0fa', 'Cabina 160 kg, due contrappesi da 30 kg'),
]
fx, fy, fw, fh, fg = 20, 166, 308, 118, 34
for i, (t, col, fill, d) in enumerate(flow):
    x = fx + i * (fw + fg)
    R(x, fy, fw, fh, fill, col, 2)
    R(x, fy, fw, 36, col, col, 0, 10)
    T(x + 14, fy + 25, t, 17, 'start', 'bold', '#fff')
    PARA(x + 14, fy + 62, d, fw - 28, 14)
    if i < len(flow) - 1:
        L(x + fw + 3, fy + 59, x + fw + fg - 3, fy + 59, NAV, 3.5, arrow=True)
cards = [
    ('A macchina ferma', GRN,
     'La batteria alimenta l\'elettronica e il caricabatteria resta al minimo; quando la carica consumata supera una soglia, ricarica la batteria.'),
    ('In marcia', ORA,
     'Il caricabatteria è staccato e la macchina va solo a batteria. Dopo la corsa, entro circa 30 secondi, torna alla ricarica.'),
    ('Senza rete (blackout)', BLU,
     'La batteria alimenta la macchina per un numero limitato di corse; la luce di emergenza resta accesa. Alla ripresa della rete la batteria si ricarica da sola.'),
]
cw3 = (W - 40 - 40) // 3
for i, (t, col, d) in enumerate(cards):
    x = 20 + i * (cw3 + 20)
    R(x, 312, cw3, 104, '#fff', col, 2)
    o.append(f'<rect x="{x}" y="312" width="10" height="104" rx="4" fill="{col}"/>')
    T(x + 24, 340, t, 18, 'start', 'bold', col)
    PARA(x + 24, 364, d, cw3 - 44, 14)

# ------------------------------------------------------------------ 2. ciclo della batteria
SECTION(444, '2 · Il ciclo della batteria: carica, scarica e ricarica', GRN)
R(20, 484, 1260, 470, '#fff', '#c9d3cf', 1.2)
# nodo centrale e satelliti
R(420, 640, 300, 150, '#effaf3', GRN, 2.5)
T(436, 668, 'A riposo', 20, 'start', 'bold', GRN)
PARA(436, 694, 'La batteria alimenta l\'elettronica e si scarica lentamente. Il caricabatteria resta al minimo e il quadro misura quanta carica è stata usata.', 270, 13.5)

R(40, 540, 250, 150, '#fff6ee', ORA, 2.5)
T(56, 568, 'In uso (scarica)', 19, 'start', 'bold', ORA)
PARA(56, 592, 'La macchina viaggia a batteria, il caricabatteria è staccato e il quadro conta la carica consumata.', 224, 13.5)

R(960, 510, 300, 150, '#eef5ff', BLU, 2.5)
T(976, 538, 'Rabbocco (ricarica leggera)', 18, 'start', 'bold', BLU)
PARA(976, 562, 'Ricarica a 54,0 V, circa 3 A, finché la carica consumata è recuperata: circa 15 minuti per 0,7 Ah (stima).', 270, 13.5)

R(960, 740, 300, 190, '#eef5ff', BLU, 2.5)
T(976, 768, 'Carica completa', 19, 'start', 'bold', BLU)
PARA(976, 792, '58,0 V fino a quando la corrente scende sotto 1,2 A: la batteria è piena e il contatore si ricalibra. Da circa un\'ora a qualche ora, secondo quanto era scarica (stima).', 270, 13.5)

# frecce con condizioni
L(292, 640, 418, 676, ORA, 3.5, arrow=True)
T(300, 618, 'comando di corsa', 12.5, 'start', 'bold', ORA)
L(418, 700, 292, 664, ORA, 3.5, arrow=True)
T(300, 705, 'fine corsa', 12.5, 'start', 'bold', ORA)
PARA(730, 556, 'mancano circa 0,7 Ah o la tensione scende sotto 48 V', 225, 12.5, fill=BLU)
L(722, 680, 958, 600, BLU, 3.5, arrow=True)
L(958, 640, 722, 715, BLU, 3.5, arrow=True)
T(740, 740, 'ricaricata', 12.5, 'start', 'bold', BLU)
L(722, 770, 958, 830, BLU, 3.5, arrow=True)
L(958, 880, 722, 790, BLU, 3.5, arrow=True)
PARA(730, 918, 'mancano più di 2 Ah, batteria scarica o dopo 10 rabbocchi', 225, 12.5, fill=BLU)

# batteria scarica
R(40, 830, 400, 106, '#fdf1ef', RED, 2)
T(56, 858, 'Batteria scarica', 18, 'start', 'bold', RED)
PARA(56, 880, 'Se sotto carico la tensione scende a 38,4 V la macchina va fuori servizio al piano (si può solo riaprire la porta) e riparte dopo la ricarica.', 370, 13)

# pannello destro
R(1300, 484, 1080, 470, '#fff', '#c9d3cf', 1.2)
T(1320, 514, 'Come il quadro gestisce la batteria', 19, 'start', 'bold', GRN)
yy = 542
for t in [
    'Misura continuamente tensione, corrente e carica consumata con un contatore che sopravvive anche ai riavvii del quadro.',
    'Ricarica a due livelli: rabbocco leggero a 54,0 V per le piccole scariche, carica completa a 58,0 V quando serve.',
    'Ogni 10 rabbocchi esegue una carica completa, per ricalibrare il contatore di carica.',
    'La corrente di ricarica è regolata attorno a 3 A, per non stressare la batteria.',
    'Se la batteria è molto scarica passa subito alla carica completa.',
    'Durante la marcia il caricabatteria resta al minimo e la ricarica riprende a macchina ferma.',
]:
    yy = BUL(1320, yy, t, 1040, GRN)
yy += 12
T(1320, yy, 'Protezioni', 19, 'start', 'bold', RED)
yy += 28
for t in [
    'Fusibile di backup sulla batteria contro i cortocircuiti (dato dell\'analisi dei rischi).',
    'Soglia di tensione minima a 38,4 V: sotto questo valore la macchina si ferma al piano per non scaricare la batteria.',
    'Soglia di sovratensione a 63 V in marcia: apre l\'uscita del motore e blocca la macchina.',
    'Sezionatori separati per rete e batteria: la macchina è spenta del tutto solo con entrambi aperti.',
]:
    yy = BUL(1320, yy, t, 1040, RED)

# ------------------------------------------------------------------ 3. contrappesi
SECTION(984, '3 · I contrappesi: perché riducono l\'energia', PUR)
R(20, 1024, 2360, 400, '#fff', '#d9cde4', 1.2)
# schema puleggia
px = 290
o.append(f'<circle cx="{px}" cy="1090" r="36" fill="#eee" stroke="{GRY}" stroke-width="3"/>')
T(px, 1096, 'motore', 13, 'middle', 'bold', GRY)
L(px - 36, 1090, px - 36, 1230, GRY, 3)
L(px + 36, 1090, px + 36, 1230, GRY, 3)
R(px - 130, 1230, 190, 120, '#f6f0fa', PUR, 2.5)
T(px - 35, 1262, 'Cabina', 17, 'middle', 'bold', PUR)
T(px - 35, 1286, '160 kg', 15, 'middle', 'normal', PUR)
T(px - 35, 1308, '+ persone', 14, 'middle', 'normal', PUR)
R(px + 70, 1160, 70, 60, '#fff6ee', ORA, 2.5)
R(px + 70, 1226, 70, 60, '#fff6ee', ORA, 2.5)
T(px + 105, 1196, '30 kg', 14, 'middle', 'bold', ORA)
T(px + 105, 1262, '30 kg', 14, 'middle', 'bold', ORA)
L(px + 36, 1090, px + 105, 1090, GRY, 3)
L(px + 105, 1090, px + 105, 1160, GRY, 3)
T(px, 1390, 'Il motore solleva solo la differenza', 14, 'middle', 'bold', PUR)
T(px, 1408, 'fra cabina e contrappesi.', 14, 'middle', 'normal', PUR)

# tabella e barre
tx = 520
T(tx, 1058, f'Energia meccanica per una salita di {it(CORSA)} m (calcolo dalle masse, senza perdite)', 17, 'start', 'bold', PUR)
T(tx, 1080, 'Con i contrappesi (verde) e senza (grigio): stessa cabina, stesso carico.', 13, 'start', 'normal', SUB)
maxw = 340.0
maxv = max(r[3] for r in rows)
for i, (name, q, wc, ws, sav) in enumerate(rows):
    y = 1108 + i * 100
    T(tx, y + 14, name, 15, 'start', 'bold', TXT)
    o.append(f'<rect x="{tx}" y="{y + 24}" width="{maxw * wc / maxv:.0f}" height="22" rx="4" fill="{GRN}"/>')
    T(tx + maxw * wc / maxv + 8, y + 41, f'{it(wc, 2)} Wh con contrappesi', 13, 'start', 'bold', GRN)
    o.append(f'<rect x="{tx}" y="{y + 50}" width="{maxw * ws / maxv:.0f}" height="22" rx="4" fill="#9ca3af"/>')
    T(tx + maxw * ws / maxv + 8, y + 67, f'{it(ws, 2)} Wh senza contrappesi', 13, 'start', 'normal', SUB)
    T(tx + 700, y + 50, f'−{it(sav * 100, 0)}%', 28, 'start', 'bold', PUR)
    T(tx + 700, y + 70, 'energia in salita', 12.5, 'start', 'normal', SUB)

# testo benefici
bx = 1330
T(bx, 1058, 'Cosa cambia con i contrappesi', 19, 'start', 'bold', PUR)
yy = 1088
for t in [
    f'Meno energia per salire: nell\'esempio con una persona il motore deve sollevare {it(M_CAB + M_PERS - M_CW, 0)} kg invece di {it(M_CAB + M_PERS, 0)} kg (−{it(rows[1][4] * 100, 0)}%).',
    f'Meno potenza di picco: a 0,10 m/s la salita richiede circa {it(p_con, 0)} W meccanici invece di {it(p_sen, 0)} W, cioè circa {it(i_con, 1)} A dalla batteria invece di {it(i_sen, 1)} A (ipotesi di rendimento).',
    f'Contando solo il sollevamento, con una persona la stessa energia dà circa {it(trips_ratio, 2)} volte le salite possibili senza contrappesi (esclusi i consumi fissi di quadro, freno e porte).',
    'A parità di prestazioni, meno energia e meno potenza di picco permettono in generale motore e batteria più piccoli (valutazione di progetto, non quantificata qui).',
    'Il comportamento in discesa (consumo ed eventuale recupero) non è valutato in questo documento.',
]:
    yy = BUL(bx, yy, t, 1020, PUR)

# ------------------------------------------------------------------ 4/5/6 autonomia, giornata, costi
SECTION(1454, '4 · Senza rete, una giornata tipica e costi di esercizio (esempio)', BLU)
cwid = (W - 40 - 40) // 3
# 4. autonomia
x0 = 20
R(x0, 1494, cwid, 330, '#fff', BLU, 2)
T(x0 + 18, 1524, 'Senza rete: l\'autonomia', 19, 'start', 'bold', BLU)
yy = 1550
for t in [
    'Dato di prodotto (CP.TEC.025): 10 corse di emergenza garantite a batteria. Il manuale d\'uso parla di «numero limitato di corse».',
    f'Esempio peggiore: salita di 4,5 m con 200 kg, con contrappesi: circa {it(worst, 1)} Wh dalla batteria per corsa, quindi {it(worst * 10, 0)} Wh per 10 corse (rendimento ipotizzato).',
    f'Rispetto all\'energia nominale della batteria (576 Wh = 12 Ah × 48 V) sono circa il {it(worst * 10 / BAT_WH * 100, 0)}%; la capacità utile cala con l\'età.',
    'A carica insufficiente l\'impianto va fuori servizio e consente solo il completamento della corsa e l\'apertura della porta (manuale d\'uso 4.06).',
    'Luce di emergenza di cabina: almeno 5 lux per 1 ora (analisi dei rischi). Al ritorno della rete la ricarica parte da sola.',
]:
    yy = BUL(x0 + 18, yy, t, cwid - 40, BLU)

# 5. giornata tipica
x1 = 20 + cwid + 20
R(x1, 1494, cwid, 330, '#fff', GRN, 2)
T(x1 + 18, 1524, 'Una giornata tipica (esempio)', 19, 'start', 'bold', GRN)
bx0, bw0 = x1 + 18, cwid - 36
o.append(f'<rect x="{bx0}" y="1548" width="{bw0}" height="26" rx="4" fill="#e5e7eb"/>')
import random
random.seed(7)
for h_ in (7.0, 8.0, 9.5, 12.5, 13.5, 16.0, 18.0, 19.0, 21.0, 22.0):
    o.append(f'<rect x="{bx0 + bw0 * h_ / 24:.0f}" y="1548" width="8" height="26" rx="2" fill="{ORA}"/>')
for h_ in (0, 6, 12, 18, 24):
    T(bx0 + bw0 * h_ / 24, 1592, f'{h_:02d}', 11, 'middle', 'normal', SUB)
yy = 1616
for t in [
    f'10 corse al giorno ({SALITE_DIE} salite e {SALITE_DIE} discese di {it(CORSA)} m, barre arancioni): circa 300 al mese, l\'uso normale del manuale di manutenzione.',
    'Il caricabatteria resta al minimo e interviene con un rabbocco solo quando la carica consumata supera circa 0,7 Ah.',
    f'Per il resto della giornata la macchina è ferma, con consumo in stand-by di {it(STANDBY_W, 0)} W (valore da certificare).',
    f'Energia di una salita con una persona: {it(e_up_con, 1)} Wh dalla batteria con i contrappesi, {it(e_up_sen, 1)} Wh senza.',
]:
    yy = BUL(x1 + 18, yy, t, cwid - 40, GRN)

# 6. costi
x2 = 20 + 2 * (cwid + 20)
R(x2, 1494, cwid, 330, '#fff', AMB, 2)
T(x2 + 18, 1524, 'Costi di esercizio (esempio, limite inferiore)', 19, 'start', 'bold', AMB)
tbl = [('', 'con contrappesi', 'senza contrappesi'),
       ('Energia al giorno dalla rete', f'{it(day_con, 1)} Wh', f'{it(day_sen, 1)} Wh'),
       ('Energia all\'anno', f'{it(year_con, 1)} kWh', f'{it(year_sen, 1)} kWh'),
       (f'Costo all\'anno ({it(PREZZO, 2)} €/kWh)', f'{it(year_con * PREZZO, 2)} €', f'{it(year_sen * PREZZO, 2)} €')]
for i, (a, b, c) in enumerate(tbl):
    y = 1556 + i * 26
    T(x2 + 18, y, a, 13.5, 'start', 'bold' if i == 0 else 'normal', SUB if i == 0 else TXT)
    T(x2 + cwid - 190, y, b, 13.5, 'end', 'bold', GRN)
    T(x2 + cwid - 24, y, c, 13.5, 'end', 'normal', SUB)
yy = 1672
for t in [
    f'Calcolati con stand-by di {it(STANDBY_W, 0)} W (dato da certificare): ogni watt in più di consumo fisso aggiunge circa 8,8 kWh e {it(8.76 * PREZZO, 1)} € all\'anno.',
    'Non inclusi consumi di freno, azionamento, porte, luci e comunicazione: da misurare. Il valore reale può essere più alto.',
    f'Il risparmio dei contrappesi nell\'esempio è circa {it(year_sen - year_con, 1)} kWh ({it((year_sen - year_con) * PREZZO, 2)} € all\'anno): il vantaggio è soprattutto su potenza di picco e autonomia.',
]:
    yy = BUL(x2 + 18, yy, t, cwid - 40, AMB, 13)

# ------------------------------------------------------------------ 7. vita batterie + ipotesi
SECTION(1854, '5 · Vita e manutenzione delle batterie, ipotesi dell\'esempio', NAV)
R(20, 1894, 1170, 230, '#fff', GRN, 2)
T(38, 1924, 'Vita e manutenzione delle batterie', 19, 'start', 'bold', GRN)
yy = 1950
for t in [
    'Sostituzione consigliata ogni 3 anni; manutenzione della macchina ogni 6 mesi (configurazione di prodotto).',
    'Batterie AGM sigillate senza manutenzione: nessuna perdita di elettrolita.',
    'Con l\'invecchiamento la capacità cala e l\'autonomia in blackout si riduce: il manuale di manutenzione prevede la verifica degli accumulatori e della discesa in emergenza.',
    'La carica completa periodica serve a ricalibrare il contatore di carica; l\'effetto sulla vita utile non è dichiarato.',
    'Per lavorare in sicurezza si aprono entrambi i sezionatori; per mantenere la luce si può togliere solo il fusibile del motore.',
]:
    yy = BUL(38, yy, t, 1130, GRN)

R(1210, 1894, 1170, 230, '#fffaf0', AMB, 2)
T(1228, 1924, 'Ipotesi dell\'esempio (da sostituire con misure)', 19, 'start', 'bold', AMB)
yy = 1950
for t in [
    f'Corsa {it(CORSA)} m (la corsa va da 2,5 a 4,5 m), velocità 0,10 m/s, una persona da {it(M_PERS, 0)} kg, cabina {it(M_CAB, 0)} kg, contrappesi {it(M_CW, 0)} kg in tutto.',
    f'Rendimento complessivo batteria → cinghia {it(ETA_TRAZ * 100, 0)}% e rete → batteria {it(ETA_CARICA * 100, 0)}%; consumo di trazione in discesa trascurato.',
    f'{SALITE_DIE} salite e {SALITE_DIE} discese al giorno; stand-by {it(STANDBY_W, 0)} W tutto il giorno (da certificare); energia a {it(PREZZO, 2)} €/kWh.',
    'Capacità batteria 12 Ah × 48 V = 576 Wh nominali (confermata). Il recupero in discesa non è considerato.',
    'Le misure reali (Wh per corsa, stand-by, autonomia in blackout, tempi di ricarica) sostituiranno questi valori.',
]:
    yy = BUL(1228, yy, t, 1130, AMB)

T(20, 2142, 'Per il diagramma complessivo vedere diagramma_funzionale_light; dettagli tecnici e stato delle correzioni in docs/registro_issue.md.', 12.5, 'start', 'normal', SUB)

o.append('</svg>')
out = sys.argv[1] if len(sys.argv) > 1 else 'docs/diagrammi/04.diagramma_energia_light.svg'
with open(out, 'w', encoding='utf-8') as f:
    f.write('\n'.join(o))
print('scritto', out)
print(f'esempio: salita con contrappesi {e_up_con:.2f} Wh, senza {e_up_sen:.2f} Wh; giorno {day_con:.1f}/{day_sen:.1f} Wh; '
      f'anno {year_con:.1f}/{year_sen:.1f} kWh; peggiore {worst:.2f} Wh; rapporto salite {trips_ratio:.2f}')
