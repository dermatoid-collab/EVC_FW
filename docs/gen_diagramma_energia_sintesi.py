#!/usr/bin/env python3
"""Diagramma «energia» in versione sintetica e visuale (SVG).

Uso:  python3 docs/gen_diagramma_energia_sintesi.py docs/diagrammi/05.diagramma_energia_sintesi.svg

Stessi dati e stesse ipotesi di gen_diagramma_energia.py (esempio con ipotesi dichiarate, non misure).
Il recupero di energia in discesa non è dichiarato. Voce di registro: DOC-11.
"""
import html
import sys

W, H = 2000, 1466
G = 9.81
CORSA, M_CAB, M_CW, M_PERS, Q_MAX = 3.0, 160.0, 60.0, 75.0, 200.0
ETA_T, ETA_C, STANDBY_W, SALITE, PREZZO = 0.60, 0.80, 1.0, 5, 0.25
BAT_WH = 12 * 48

TXT, SUB = '#1f2937', '#4b5563'
GRN, AMB, RED, BLU, GRY, NAV, PUR, ORA = ('#2e8b57', '#c27c0e', '#c0392b', '#2563eb',
                                          '#6b7280', '#2a3b55', '#7b3fa0', '#d35400')


def wh(kg, h=CORSA):
    return kg * G * h / 3600.0


def it(x, nd=1):
    return f'{x:.{nd}f}'.replace('.', ',')


up_con = wh(M_CAB + M_PERS - M_CW) / ETA_T
up_sen = wh(M_CAB + M_PERS) / ETA_T
day_con = SALITE * up_con / ETA_C + STANDBY_W * 24
day_sen = SALITE * up_sen / ETA_C + STANDBY_W * 24
y_con, y_sen = day_con * 365 / 1000, day_sen * 365 / 1000
worst10 = 10 * wh(M_CAB + Q_MAX - M_CW, 4.5) / ETA_T
loads = [('vuota', 0.0), ('1 persona', M_PERS), ('200 kg', Q_MAX)]

o = []


def esc(s):
    return html.escape(s, quote=True)


def T(x, y, s, size=16, anchor='start', weight='normal', fill=TXT):
    o.append(f'<text x="{x}" y="{y}" font-size="{size}" text-anchor="{anchor}" '
             f'font-weight="{weight}" fill="{fill}">{esc(s)}</text>')


def R(x, y, w, h, fill='#fff', stroke='#999', sw=2, rx=14):
    o.append(f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="{rx}" fill="{fill}" '
             f'stroke="{stroke}" stroke-width="{sw}"/>')


def L(x1, y1, x2, y2, c=GRY, w=4, arrow=False, both=False):
    ms = f' marker-start="url(#s{c[1:]})"' if both else ''
    me = f' marker-end="url(#a{c[1:]})"' if arrow or both else ''
    o.append(f'<line x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}" stroke="{c}" stroke-width="{w}"{ms}{me}/>')


def SEC(y, n, title, c):
    o.append(f'<circle cx="46" cy="{y}" r="20" fill="{c}"/>')
    T(46, y + 7, str(n), 20, 'middle', 'bold', '#fff')
    T(78, y + 8, title, 24, 'start', 'bold', c)


o.append(f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}" '
         'font-family="DejaVu Sans, Arial, Helvetica, sans-serif">')
o.append('<defs>')
for c in (GRY, GRN, NAV, RED, BLU, ORA, AMB, PUR):
    o.append(f'<marker id="a{c[1:]}" markerWidth="10" markerHeight="10" refX="9" refY="5" orient="auto">'
             f'<path d="M0,0 L10,5 L0,10 z" fill="{c}"/></marker>')
    o.append(f'<marker id="s{c[1:]}" markerWidth="10" markerHeight="10" refX="1" refY="5" orient="auto-start-reverse">'
             f'<path d="M0,0 L10,5 L0,10 z" fill="{c}"/></marker>')
o.append('</defs>')
o.append(f'<rect width="{W}" height="{H}" fill="#f7f8fa"/>')

# intestazione
R(0, 0, W, 92, NAV, NAV, 0, 0)
T(30, 44, 'ELEVANCE — Energia in una pagina', 32, 'start', 'bold', '#fff')
T(30, 72, 'Batteria, ricarica e contrappesi · sintesi visuale per il commerciale', 17, 'start', 'normal', '#dbe4f0')
T(W - 30, 40, 'Aggiornata al 07/11/2026 · non contrattuale', 16, 'end', 'bold', '#fff')
T(W - 30, 66, 'I numeri contrassegnati ESEMPIO sono calcolati con ipotesi dichiarate in basso, non misure.', 14, 'end', 'normal', '#dbe4f0')

# 1. percorso con icone
SEC(130, 1, 'Da dove arriva l\'energia', NAV)
bx = [60, 560, 1060, 1560]
labels = [('Rete di casa', '230 V · 2 A · 0,5 kW', ORA), ('Batteria', '48 V · 12 Ah · senza manutenzione', GRN),
          ('Motore a 48 V', 'Brushless con doppio freno', NAV), ('Cabina + contrappesi', '160 kg + 2 × 30 kg', PUR)]
for i, (x, (t, d, c)) in enumerate(zip(bx, labels)):
    R(x, 165, 380, 170, '#fff', c, 2.5)
    T(x + 190, 296, t, 22, 'middle', 'bold', c)
    T(x + 190, 322, d, 14, 'middle', 'normal', SUB)
    if i < 3:
        L(x + 392, 250, bx[i + 1] - 12, 250, NAV, 5, arrow=True)
# icone
x = bx[0]  # presa
o.append(f'<rect x="{x+150}" y="190" width="80" height="60" rx="10" fill="{ORA}"/>')
o.append(f'<rect x="{x+166}" y="170" width="10" height="26" fill="{GRY}"/><rect x="{x+204}" y="170" width="10" height="26" fill="{GRY}"/>')
o.append(f'<path d="M{x+190},250 L{x+190},270 L{x+240},270" stroke="{GRY}" stroke-width="6" fill="none"/>')
x = bx[1]  # batteria
o.append(f'<rect x="{x+130}" y="185" width="120" height="75" rx="8" fill="#fff" stroke="{GRN}" stroke-width="5"/>')
o.append(f'<rect x="{x+250}" y="210" width="12" height="26" rx="3" fill="{GRN}"/>')
o.append(f'<rect x="{x+138}" y="193" width="100" height="59" rx="4" fill="{GRN}"/>')
T(x + 190, 235, '48 V', 20, 'middle', 'bold', '#fff')
x = bx[2]  # motore
o.append(f'<circle cx="{x+190}" cy="225" r="42" fill="#fff" stroke="{NAV}" stroke-width="5"/>')
for k in range(8):
    import math
    a = k * math.pi / 4
    o.append(f'<line x1="{x+190+math.cos(a)*42:.0f}" y1="{225+math.sin(a)*42:.0f}" x2="{x+190+math.cos(a)*54:.0f}" y2="{225+math.sin(a)*54:.0f}" stroke="{NAV}" stroke-width="8"/>')
o.append(f'<circle cx="{x+190}" cy="225" r="12" fill="{NAV}"/>')
x = bx[3]  # cabina e contrappesi
o.append(f'<line x1="{x+130}" y1="176" x2="{x+250}" y2="176" stroke="{GRY}" stroke-width="5"/>')
o.append(f'<line x1="{x+150}" y1="176" x2="{x+150}" y2="200" stroke="{GRY}" stroke-width="4"/><line x1="{x+230}" y1="176" x2="{x+230}" y2="200" stroke="{GRY}" stroke-width="4"/>')
o.append(f'<rect x="{x+120}" y="200" width="60" height="62" rx="14" fill="#f6f0fa" stroke="{PUR}" stroke-width="4"/>')
o.append(f'<rect x="{x+210}" y="200" width="40" height="22" rx="4" fill="#fff6ee" stroke="{ORA}" stroke-width="3"/>')
o.append(f'<rect x="{x+210}" y="226" width="40" height="22" rx="4" fill="#fff6ee" stroke="{ORA}" stroke-width="3"/>')
T(560 + 190, 150, '', 1)
# nota sotto
T(W // 2, 362, 'Se manca la rete la macchina continua a batteria: 10 corse di emergenza (dato di prodotto) e luce di emergenza in cabina.', 17, 'middle', 'normal', SUB)

# 2. ciclo batteria
SEC(412, 2, 'Il ciclo della batteria', GRN)
R(30, 440, 1060, 440, '#fff', '#d5dbe3', 1.5)
cx, cy = 560, 620
o.append(f'<circle cx="{cx}" cy="{cy}" r="90" fill="#effaf3" stroke="{GRN}" stroke-width="4"/>')
T(cx, cy - 6, 'A riposo', 21, 'middle', 'bold', GRN)
T(cx, cy + 20, 'il quadro conta', 14, 'middle', 'normal', SUB)
T(cx, cy + 38, 'la carica usata', 14, 'middle', 'normal', SUB)
sats = [(180, 560, 'In uso', 'scarica: va a batteria,\ncaricabatteria staccato', ORA, '#fff6ee'),
        (880, 540, 'Rabbocco', '54 V · circa 3 A\n10-15 min (stima)', BLU, '#eef5ff'),
        (700, 805, 'Carica completa', '58 V fino a corrente < 1,2 A\n4-8 ore (stima)', BLU, '#eef5ff')]
for sx, sy, t, d, c, f in sats:
    R(sx - 140, sy - 52, 280, 104, f, c, 3)
    T(sx, sy - 14, t, 21, 'middle', 'bold', c)
    for k, ln in enumerate(d.split('\n')):
        T(sx, sy + 14 + 21 * k, ln, 14, 'middle', 'normal', TXT)
L(cx - 90, cy - 10, 324, 585, ORA, 4, both=True)
L(cx + 80, cy - 40, 745, 570, BLU, 4, both=True)
L(cx + 30, cy + 86, 650, 752, BLU, 4, both=True)
T(330, 628, 'ogni corsa', 14, 'start', 'bold', ORA)
T(745, 628, 'mancano circa 0,7 Ah', 14, 'start', 'bold', BLU)
T(850, 790, 'mancano più di 2 Ah', 14, 'start', 'bold', BLU)
T(850, 808, 'o dopo 10 rabbocchi', 14, 'start', 'bold', BLU)
R(60, 790, 340, 70, '#fdf1ef', RED, 2.5)
T(230, 818, 'Sotto 38,4 V sotto carico:', 15, 'middle', 'bold', RED)
T(230, 842, 'fuori servizio al piano, riparte ricaricata', 14, 'middle', 'normal', TXT)

# misuratore
gx, gy, gh = 1110, 510, 330
T(gx + 190, 478, 'Quando si ricarica', 19, 'middle', 'bold', GRN)
segs = [(0, 70, '#bfe6cc', 'Piena: nessuna ricarica'),
        (70, 120, '#cfe0ff', 'Mancano 0,7 Ah: rabbocco'),
        (190, 90, '#8fb4ff', 'Mancano 2 Ah: carica completa'),
        (280, 50, '#f2b8b0', 'Batteria scarica')]
for off, h, col, lab in segs:
    o.append(f'<rect x="{gx}" y="{gy+off}" width="90" height="{h}" fill="{col}" stroke="#fff" stroke-width="2"/>')
    T(gx + 105, gy + off + h / 2 + 5, lab, 15, 'start', 'normal', TXT)
o.append(f'<rect x="{gx}" y="{gy}" width="90" height="{gh}" rx="8" fill="none" stroke="{NAV}" stroke-width="4"/>')
T(gx + 45, gy + gh + 26, 'scala non proporzionale', 13, 'middle', 'normal', SUB)
T(1560, 560, '', 1)
R(1500, 440, 470, 440, '#fff', '#d5dbe3', 1.5)
T(1520, 480, 'In breve', 20, 'start', 'bold', GRN)
brief = ['Il caricabatteria resta al minimo finché non serve:',
         'la ricarica parte solo dopo diverse corse.',
         '',
         'Ogni 10 rabbocchi una carica completa',
         'ricalibra il contatore di carica.',
         '',
         'Rete e batteria hanno sezionatori separati:',
         'spenta del tutto solo con entrambi aperti.',
         '',
         'Protezione a 63 V in marcia: apre l\'uscita',
         'del motore e blocca la macchina.']
for k, ln in enumerate(brief):
    T(1520, 520 + k * 28, ln, 15, 'start', 'normal', TXT)

# 3. contrappesi
SEC(930, 3, 'I contrappesi riducono l\'energia di sollevamento (calcolo dalle masse, senza perdite)', PUR)
R(30, 960, 1180, 266, '#fff', '#d9cde4', 1.5)
maxv = wh(M_CAB + Q_MAX)
for i, (name, q) in enumerate(loads):
    x0 = 70 + i * 380
    wc, ws = wh(M_CAB + q - M_CW), wh(M_CAB + q)
    sav = 1 - wc / ws
    T(x0 + 150, 996, f'Cabina {name}', 17, 'middle', 'bold', TXT)
    hh = 90.0
    o.append(f'<rect x="{x0+20}" y="{1170-hh*ws/maxv:.0f}" width="100" height="{hh*ws/maxv:.0f}" rx="6" fill="#9ca3af"/>')
    o.append(f'<rect x="{x0+160}" y="{1170-hh*wc/maxv:.0f}" width="100" height="{hh*wc/maxv:.0f}" rx="6" fill="{GRN}"/>')
    T(x0 + 70, 1190, 'senza', 13, 'middle', 'normal', SUB)
    T(x0 + 210, 1190, 'con contrappesi', 13, 'middle', 'bold', GRN)
    T(x0 + 70, 1162 - hh * ws / maxv - 6, f'{it(ws, 1)} Wh', 14, 'middle', 'normal', SUB)
    T(x0 + 210, 1162 - hh * wc / maxv - 6, f'{it(wc, 1)} Wh', 14, 'middle', 'bold', GRN)
    T(x0 + 150, 1046, f'−{it(sav * 100, 0)}%', 34, 'middle', 'bold', PUR)
T(70, 1216, f'Una salita di {it(CORSA)} m. Il motore solleva solo la differenza fra cabina e contrappesi. Discesa non valutata.', 13, 'start', 'normal', SUB)

# autonomia
R(1230, 960, 740, 266, '#fff', BLU, 2)
T(1250, 996, 'Senza rete: 10 corse di emergenza', 20, 'start', 'bold', BLU)
o.append(f'<rect x="1260" y="1030" width="400" height="70" rx="10" fill="#fff" stroke="{GRN}" stroke-width="5"/>')
o.append(f'<rect x="1660" y="1051" width="14" height="28" rx="3" fill="{GRN}"/>')
fw = 392 * worst10 / BAT_WH
o.append(f'<rect x="1264" y="1034" width="{fw:.0f}" height="62" rx="6" fill="{ORA}"/>')
T(1260 + 200, 1070, '576 Wh nominali (12 Ah × 48 V)', 17, 'middle', 'bold', TXT)
T(1700, 1062, f'{it(worst10 / BAT_WH * 100, 0)}%', 36, 'start', 'bold', ORA)
T(1700, 1086, 'ESEMPIO (sul nominale)', 13, 'start', 'bold', ORA)
T(1250, 1128, f'Caso peggiore (4,5 m, 200 kg): circa {it(worst10, 0)} Wh per 10 corse (rendimento ipotizzato).', 15, 'start', 'normal', TXT)
T(1250, 1152, 'A carica insufficiente: fuori servizio, solo fine corsa e apertura porta.', 15, 'start', 'normal', TXT)
T(1250, 1176, 'Luce di emergenza: almeno 5 lux per 1 ora (analisi dei rischi).', 15, 'start', 'normal', TXT)
T(1250, 1200, 'La capacità utile cala con l\'età.', 15, 'start', 'normal', TXT)

# 4. numeri chiave
SEC(1262, 4, 'In bolletta e in manutenzione', AMB)
tiles = [(f'{it(y_con, 0)} kWh/anno', f'ESEMPIO · con contrappesi ({it(y_sen, 0)} senza)', ORA),
         (f'≈ {it(y_con * PREZZO, 1)} €/anno', f'ESEMPIO · limite inferiore a {it(PREZZO, 2)} €/kWh', ORA),
         ('+2,2 €/anno', 'per ogni watt in più di stand-by', AMB),
         ('3 anni', 'sostituzione batterie consigliata', GRN),
         ('6 mesi', 'manutenzione della macchina', GRN)]
tw = 372
for i, (big, small, c) in enumerate(tiles):
    x = 30 + i * (tw + 20)
    R(x, 1284, tw, 92, '#fff', c, 2.5)
    T(x + tw / 2, 1328, big, 30, 'middle', 'bold', c)
    T(x + tw / 2, 1358, small, 13.5, 'middle', 'normal', SUB)

R(30, 1396, 1940, 54, '#fffaf0', AMB, 1.5)
T(48, 1419, f'Ipotesi dell\'esempio: corsa {it(CORSA)} m a 0,10 m/s, una persona da {it(M_PERS, 0)} kg, rendimenti {it(ETA_T * 100, 0)}% (batteria→cinghia) e {it(ETA_C * 100, 0)}% (rete→batteria), {SALITE} salite + {SALITE} discese al giorno (circa 300 corse/mese), stand-by {it(STANDBY_W, 0)} W (da certificare), {it(PREZZO, 2)} €/kWh.', 13.5, 'start', 'normal', TXT)
T(48, 1440, 'Non inclusi: consumi di freno, azionamento, porte, luci e comunicazione; recupero in discesa. Fonti: CP.TEC.025 Rev.5, ST.TEC.068, manuali IM.TEC, analisi dei rischi Rev.7, firmware.', 13.5, 'start', 'normal', SUB)
o.append('</svg>')
out = sys.argv[1] if len(sys.argv) > 1 else 'docs/diagrammi/05.diagramma_energia_sintesi.svg'
svg = '\n'.join(o)
with open(out, 'w', encoding='utf-8') as f:
    f.write(svg)
print('scritto', out, f'worst10={worst10:.1f} Wh, anno {y_con:.1f}/{y_sen:.1f} kWh')
