#!/usr/bin/env python3
"""Diagramma «manutenzione» per il commerciale (SVG), solo affermazioni confermate.

Uso:  python3 docs/gen_diagramma_manutenzione.py docs/diagrammi/06.diagramma_manutenzione.svg

Contenuto ammesso (voci DOC-13, DOC-23, NOR-07 del registro):
- controllo semestrale programmato (CP.TEC.025 Rev.5, IM.TEC.159);
- batterie AGM sigillate, sostituzione ogni 3 anni (CP.TEC.025, analisi dei rischi Rev.7);
- cuscinetti schermati 2Z e già ingrassati (conferma dell'utente 2026-11-08);
- elenco dei controlli del piano (IM.TEC.159 §10.06, senza periodicità per voce);
- due aree di intervento (IM.TEC.159 §10.02); personale qualificato (IM.TEC.159).
Esclusi finché non documentati: vita utile e intervalli di sostituzione di cinghie, rotelle, cerniere, freni;
lubrificazione delle pulegge; claim «nessuna parte di usura»; obblighi di legge per Paese.
"""
import html
import sys

W, H = 2000, 1150
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
T(30, 44, 'ELEVANCE — Manutenzione programmata', 32, 'start', 'bold', '#fff')
T(30, 72, 'Cosa si fa, ogni quanto e dove · scheda per il commerciale', 17, 'start', 'normal', '#dbe4f0')
T(W - 30, 40, 'Aggiornata al 08/11/2026 · non contrattuale', 16, 'end', 'bold', '#fff')
T(W - 30, 66, 'Intervalli e obblighi di manutenzione possono variare secondo le norme di ciascun Paese.', 14, 'end', 'normal', '#dbe4f0')

# 1. tre punti fermi
SEC(130, 1, 'Tre punti fermi', NAV)
cards = [
    ('Ogni 6 mesi', 'Controllo programmato', 'Una visita semestrale a cura di personale qualificato, secondo il piano di controllo.', GRN),
    ('Ogni 3 anni', 'Sostituzione delle batterie', 'Batterie AGM sigillate, senza manutenzione ordinaria: si sostituiscono ogni 3 anni.', ORA),
    ('Cuscinetti 2Z', 'Schermati e già ingrassati', 'I cuscinetti delle pulegge sono schermati e pre-ingrassati.', PUR),
]
cw = 630
for i, (big, mid, small, c) in enumerate(cards):
    x = 30 + i * (cw + 25)
    R(x, 160, cw, 200, '#fff', c, 3)
    T(x + 30, 222, big, 40, 'start', 'bold', c)
    T(x + 30, 262, mid, 22, 'start', 'bold', TXT)
    # testo su due righe
    words = small.split()
    line, lines = '', []
    for w_ in words:
        if len(line) + len(w_) + 1 <= 52:
            line = (line + ' ' + w_).strip()
        else:
            lines.append(line)
            line = w_
    lines.append(line)
    for k, ln in enumerate(lines):
        T(x + 30, 298 + 24 * k, ln, 15, 'start', 'normal', SUB)

# 2. linea del tempo
SEC(412, 2, 'I primi tre anni: controlli e batterie', GRN)
R(30, 440, 1940, 190, '#fff', '#d5dbe3', 1.5)
x0, x1, yl = 130, 1830, 535
o.append(f'<line x1="{x0}" y1="{yl}" x2="{x1}" y2="{yl}" stroke="{GRY}" stroke-width="6"/>')
for k, m in enumerate(range(0, 37, 6)):
    x = x0 + (x1 - x0) * m / 36
    if m == 0:
        o.append(f'<circle cx="{x}" cy="{yl}" r="16" fill="{NAV}"/>')
        T(x, yl + 6, '✓', 20, 'middle', 'bold', '#fff')
        T(x, yl + 50, 'Messa in servizio', 15, 'middle', 'bold', NAV)
        T(x, yl + 70, 'con i controlli iniziali', 13, 'middle', 'normal', SUB)
    else:
        o.append(f'<circle cx="{x}" cy="{yl}" r="16" fill="{GRN}"/>')
        T(x, yl + 6, '✓', 20, 'middle', 'bold', '#fff')
        T(x, yl + 50, f'{m} mesi', 15, 'middle', 'bold', GRN)
        T(x, yl + 70, 'controllo', 13, 'middle', 'normal', SUB)
xb = x1
o.append(f'<rect x="{xb-30}" y="{yl-90}" width="120" height="44" rx="10" fill="#fff6ee" stroke="{ORA}" stroke-width="3"/>')
T(xb + 30, yl - 62, 'Batterie', 17, 'middle', 'bold', ORA)
o.append(f'<line x1="{xb}" y1="{yl-46}" x2="{xb}" y2="{yl-18}" stroke="{ORA}" stroke-width="3"/>')

# 3. cosa comprende il controllo
SEC(682, 3, 'Cosa comprende il piano di controllo', BLU)
items = ['Movimento e fermate ai piani', 'Alimentazione di emergenza: luce, allarme, discesa', 'Fondo sensibile e tetto sensibile',
         'Serrature delle porte', 'Finecorsa e sensori di allentamento cinghie', 'Messa a terra e linee elettriche',
         'Illuminazione di cabina', 'Dispositivo di sovraccarico', 'Velocità di accelerazione e decelerazione',
         'Paracadute di cabina e contrappesi', 'Cinghie, capocinghia e pulegge di trazione', 'Rotelle di scorrimento e guide',
         'Batterie', 'Segnaletica di sicurezza']
cols, tw, th = 4, 470, 54
for k, s in enumerate(items):
    cx_, cy_ = 30 + (k % cols) * (tw + 20), 712 + (k // cols) * (th + 12)
    R(cx_, cy_, tw, th, '#fff', '#c7d6ee', 2, 10)
    o.append(f'<circle cx="{cx_+28}" cy="{cy_+th/2}" r="11" fill="{BLU}"/>')
    T(cx_ + 28, cy_ + th / 2 + 5, '✓', 15, 'middle', 'bold', '#fff')
    T(cx_ + 52, cy_ + th / 2 + 5, s, 15.5, 'start', 'normal', TXT)

# 4. dove e chi
SEC(1000, 4, 'Dove e chi', PUR)
R(30, 1028, 950, 90, '#fff', PUR, 2.5)
T(55, 1062, 'Aree di intervento', 19, 'start', 'bold', PUR)
T(55, 1090, 'Il vano motore in testata e la zona sopra la pedana di carico, con la cabina al piano 0.', 15, 'start', 'normal', TXT)
R(1000, 1028, 970, 90, '#fff', PUR, 2.5)
T(1025, 1062, 'A cura di personale qualificato', 19, 'start', 'bold', PUR)
T(1025, 1090, 'Con ricambi originali. Il manuale d\'uso indica anche i semplici controlli a cura dell\'utente.', 15, 'start', 'normal', TXT)
T(30, 1140, 'Fonti: CP.TEC.025 Rev.5, istruzioni di manutenzione IM.TEC.159, analisi dei rischi Rev.7, ufficio tecnico LiftingItalia (cuscinetti).', 12.5, 'start', 'normal', SUB)

o.append('</svg>')
out = sys.argv[1] if len(sys.argv) > 1 else 'docs/diagrammi/06.diagramma_manutenzione.svg'
with open(out, 'w', encoding='utf-8') as f:
    f.write('\n'.join(o))
print('scritto', out)
