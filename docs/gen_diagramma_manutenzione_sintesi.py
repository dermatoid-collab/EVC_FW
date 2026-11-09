#!/usr/bin/env python3
"""Diagramma «manutenzione» sintetico e visuale per il commerciale (SVG).

Uso:  python3 docs/gen_diagramma_manutenzione_sintesi.py docs/diagrammi/07.diagramma_manutenzione_sintesi.svg

Come il 06 (senza i cuscinetti 2Z; con in più garanzia estesa, 22 procedure, ricambi originali e diagnosi guidata), con in più, su richiesta dell'utente (2026-11-08), il riquadro «costo delle parti di consumo
approssimativamente zero, escluse le batterie ogni 3 anni», presentato come STIMA DEL COSTRUTTORE per uso
residenziale (circa 300 corse al mese). Non è ancora documentato da dati di vita dei componenti: vedi DOC-23.
"""
import html
import sys

W, H = 2000, 1030
TXT, SUB = '#1f2937', '#4b5563'
GRN, AMB, RED, BLU, GRY, NAV, PUR, ORA = ('#2e8b57', '#c27c0e', '#c0392b', '#2563eb',
                                          '#6b7280', '#2a3b55', '#7b3fa0', '#d35400')
o = []


def esc(s):
    return html.escape(s, quote=True)


def T(x, y, s, size=16, anchor='start', weight='normal', fill=TXT):
    o.append(f'<text x="{x}" y="{y}" font-size="{size}" text-anchor="{anchor}" '
             f'font-weight="{weight}" fill="{fill}">{esc(s)}</text>')


def R(x, y, w, h, fill='#fff', stroke='#999', sw=2, rx=14):
    o.append(f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="{rx}" fill="{fill}" '
             f'stroke="{stroke}" stroke-width="{sw}"/>')


def SEC(y, n, title, c):
    o.append(f'<circle cx="46" cy="{y}" r="20" fill="{c}"/>')
    T(46, y + 7, str(n), 20, 'middle', 'bold', '#fff')
    T(78, y + 8, title, 24, 'start', 'bold', c)


o.append(f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}" '
         'font-family="DejaVu Sans, Arial, Helvetica, sans-serif">')
o.append(f'<rect width="{W}" height="{H}" fill="#f7f8fa"/>')
R(0, 0, W, 92, NAV, NAV, 0, 0)
T(30, 44, 'ELEVANCE — Manutenzione in una pagina', 32, 'start', 'bold', '#fff')
T(30, 72, 'Controlli programmati, batterie ogni 3 anni · sintesi visuale per il commerciale', 17, 'start', 'normal', '#dbe4f0')
T(W - 30, 40, 'Aggiornata al 08/11/2026 · non contrattuale', 16, 'end', 'bold', '#fff')
T(W - 30, 66, 'Intervalli e obblighi di manutenzione possono variare secondo le norme di ciascun Paese.', 14, 'end', 'normal', '#dbe4f0')

# 1. quattro numeri
SEC(130, 1, 'In quattro numeri', NAV)
tiles = [('6 mesi', 'controllo programmato', 'a cura di personale qualificato', GRN),
         ('3 anni', 'sostituzione batterie', 'AGM sigillate, senza manutenzione ordinaria', ORA),
         ('5 anni', 'garanzia estesa†', '12 mesi di legge più 4 anni di estensione', PUR),
         ('≈ 0 €', 'costo delle parti di consumo*', 'escluse le batterie ogni 3 anni', BLU)]
tw = 465
for i, (big, mid, small, c) in enumerate(tiles):
    x = 30 + i * (tw + 27)
    R(x, 160, tw, 190, '#fff', c, 3)
    T(x + tw / 2, 235, big, 56, 'middle', 'bold', c)
    T(x + tw / 2, 278, mid, 21, 'middle', 'bold', TXT)
    T(x + tw / 2, 312, small, 15, 'middle', 'normal', SUB)

# 2. tre anni
SEC(402, 2, 'I primi tre anni', GRN)
R(30, 430, 1940, 150, '#fff', '#d5dbe3', 1.5)
x0, x1, yl = 130, 1830, 495
o.append(f'<line x1="{x0}" y1="{yl}" x2="{x1}" y2="{yl}" stroke="{GRY}" stroke-width="6"/>')
for m in range(0, 37, 6):
    x = x0 + (x1 - x0) * m / 36
    c = NAV if m == 0 else GRN
    o.append(f'<circle cx="{x}" cy="{yl}" r="16" fill="{c}"/>')
    T(x, yl + 6, '✓', 20, 'middle', 'bold', '#fff')
    T(x, yl + 46, 'Messa in servizio' if m == 0 else f'{m} mesi', 15, 'middle', 'bold', c)
o.append(f'<rect x="{x1-30}" y="{yl-62}" width="120" height="38" rx="10" fill="#fff6ee" stroke="{ORA}" stroke-width="3"/>')
T(x1 + 30, yl - 36, 'Batterie', 17, 'middle', 'bold', ORA)
o.append(f'<line x1="{x1}" y1="{yl-24}" x2="{x1}" y2="{yl-16}" stroke="{ORA}" stroke-width="3"/>')

# 3. cosa si controlla
SEC(630, 3, 'Cosa si controlla: 22 procedure nel piano di controllo', BLU)
groups = [('Movimento', 'fermate, velocità, sovraccarico'), ('Porte e cabina', 'serrature, luci, bordi sensibili, segnaletica'),
          ('Emergenza', 'alimentazione di emergenza, batterie'), ('Sicurezze', 'paracadute, finecorsa, sensori cinghie'),
          ('Trazione', 'cinghie, pulegge, rotelle, guide'), ('Impianto elettrico', 'messa a terra e linee')]
gw, gh = 625, 90
for k, (t, d) in enumerate(groups):
    x, y = 30 + (k % 3) * (gw + 32), 660 + (k // 3) * (gh + 14)
    R(x, y, gw, gh, '#fff', '#c7d6ee', 2, 12)
    o.append(f'<circle cx="{x+34}" cy="{y+gh/2}" r="15" fill="{BLU}"/>')
    T(x + 34, y + gh / 2 + 6, '✓', 18, 'middle', 'bold', '#fff')
    T(x + 66, y + 38, t, 20, 'start', 'bold', TXT)
    T(x + 66, y + 66, d, 15, 'start', 'normal', SUB)

# 4. dove, chi, diagnosi
boxes = [('Aree di intervento', 'Vano motore in testata e sopra la pedana di carico (cabina al piano 0).'),
         ('Personale qualificato e ricambi originali', 'Controlli semplici a cura dell\'utente: vedi manuale d\'uso.'),
         ('Diagnosi guidata', 'Il quadro memorizza gli eventi e li mostra sul display.')]
for k, (t, d) in enumerate(boxes):
    x = 30 + k * 657
    R(x, 880, 625, 70, '#fff', PUR, 2.5, 10)
    T(x + 22, 906, t, 17, 'start', 'bold', PUR)
    T(x + 22, 932, d, 14.5, 'start', 'normal', TXT)
T(30, 982, '* Stima del costruttore per uso residenziale (circa 300 corse al mese).', 13.5, 'start', 'bold', SUB)
T(30, 1002, '† Garanzia estesa alle condizioni e con le esclusioni previste dal manuale di manutenzione.', 13.5, 'start', 'bold', SUB)
T(30, 1020, 'Fonti: CP.TEC.025 Rev.5, istruzioni di manutenzione IM.TEC.159, analisi dei rischi Rev.7, ufficio tecnico LiftingItalia.', 12.5, 'start', 'normal', SUB)

o.append('</svg>')
out = sys.argv[1] if len(sys.argv) > 1 else 'docs/diagrammi/07.diagramma_manutenzione_sintesi.svg'
with open(out, 'w', encoding='utf-8') as f:
    f.write('\n'.join(o))
print('scritto', out)
