#!/usr/bin/env python3
"""Genera il diagramma funzionale «light» di Elevance per il dipartimento commerciale (SVG).

Uso:  python3 docs/gen_diagramma_light.py docs/diagramma_funzionale_light.svg

Linguaggio semplice, senza nomi di codice né riferimenti alle issue. Descrive solo ciò che
il firmware TTF-Base e i manuali (CP.TEC.025 Rev.5, IM.TEC.158-161, Analisi dei rischi Rev.7)
confermano; le funzioni non ancora complete sono nel riquadro «In sviluppo».
Quando si chiudono FW-06 e FW-12, o cambia una funzione descritta qui, aggiornare questo
diagramma (vedi DOC-10 in docs/registro_issue.md).
"""
import html
import sys

W, H = 2400, 2070
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


def R(x, y, w, h, fill='#fff', stroke='#999', sw=1.4, rx=10):
    o.append(f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="{rx}" fill="{fill}" '
             f'stroke="{stroke}" stroke-width="{sw}"/>')


def ARROW(x1, y1, x2, y2, color=GRY, w=3):
    o.append(f'<line x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}" stroke="{color}" stroke-width="{w}" '
             f'marker-end="url(#ar{color[1:]})"/>')


def SECTION(y, title, color=NAV):
    o.append(f'<rect x="20" y="{y}" width="8" height="30" rx="3" fill="{color}"/>')
    T(40, y + 24, title, 21, 'start', 'bold', color)


o.append(f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}" '
         'font-family="DejaVu Sans, Arial, Helvetica, sans-serif">')
o.append('<defs>')
for c in (GRY, GRN, NAV, RED, BLU):
    o.append(f'<marker id="ar{c[1:]}" markerWidth="10" markerHeight="10" refX="9" refY="5" orient="auto">'
             f'<path d="M0,0 L10,5 L0,10 z" fill="{c}"/></marker>')
o.append('</defs>')
o.append(f'<rect width="{W}" height="{H}" fill="#f7f8fa"/>')

# ------------------------------------------------------------------ intestazione
R(0, 0, W, 104, '#2a3b55', '#2a3b55', 0, 0)
T(30, 46, 'ELEVANCE — Come funziona', 32, 'start', 'bold', '#fff')
T(30, 78, 'Panoramica funzionale della macchina per il dipartimento commerciale', 17, 'start', 'normal', '#dbe4f0')
T(W - 30, 46, 'Versione light · aggiornata al 07/11/2026', 16, 'end', 'bold', '#fff')
T(W - 30, 72, 'Documento descrittivo, non contrattuale né certificativo.', 14, 'end', 'normal', '#dbe4f0')
T(W - 30, 92, 'Fonti: configurazione di prodotto CP.TEC.025 Rev.5, manuali IM.TEC.158-161, analisi dei rischi Rev.7, firmware del quadro.', 12, 'end', 'normal', '#c3d0e2')

# ------------------------------------------------------------------ 1. blocchi
SECTION(124, '1 · I blocchi della macchina')
blocks = [
    ('Energia', ORA, '#fff6ee',
     'Rete domestica 230 V e batterie a 48 V.',
     ['Il caricabatterie ricarica le batterie quando la macchina è ferma.',
      'La macchina funziona sempre a batteria: si muove anche in assenza di rete (10 corse di emergenza).',
      'Due sezionatori (rete e batteria) per lavorare in sicurezza.']),
    ('Comandi', GRN, '#effaf3',
     'Come l\'utente chiede il movimento.',
     ['Pulsanti in cabina.',
      'Chiamate ai piani senza fili e radiocomando.',
      'Pannello di servizio nel cappello (solo tecnici).',
      'Si comanda tenendo premuto il tasto.']),
    ('Quadro di controllo', NAV, '#eef2f8',
     'Il «cervello», nel cappello sopra la cabina.',
     ['Decide quando partire, fermarsi, aprire le porte.',
      'Controlla le sicurezze e la batteria.',
      'Mostra gli allarmi sul display e permette il reset ai tecnici.']),
    ('Trazione', ORA, '#fff6ee',
     'Ciò che muove e ferma la cabina.',
     ['Motoriduttore elettrico a 48 V e due cinghie dentate con contrappesi.',
      'Doppio freno a molle: tiene la cabina ferma anche senza corrente.',
      'Paracadute meccanico di sicurezza sulle guide.']),
    ('Cabina e porte', PUR, '#f6f0fa',
     'Dove viaggiano le persone.',
     ['Cabina tonda panoramica, senza vano murario.',
      'Porta automatica con serratura elettrica.',
      'Luce LED, pulsante di allarme e stop di emergenza, pesacarico, corrimano; sedile opzionale.']),
    ('Sicurezze', RED, '#fdf1ef',
     'Protezioni che possono fermare tutto.',
     ['Una catena di contatti in serie: se uno si apre, motore e freno perdono alimentazione.',
      'È realizzata in logica cablata, non dipende dal software.']),
    ('Servizi connessi', BLU, '#eff4fe',
     'Assistenza e monitoraggio (da configurazione di prodotto).',
     ['Telesoccorso GSM.',
      'Collegamento Sentinel per il monitoraggio da remoto.',
      'Allarme sonoro locale e combinatore telefonico per chiamare i soccorsi.']),
]
bx, by, bw, bh, gap = 20, 164, 320, 262, 20
for i, (title, col, fill, role, items) in enumerate(blocks):
    x = bx + i * (bw + gap)
    R(x, by, bw, bh, fill, col, 2)
    R(x, by, bw, 40, col, col, 0, 10)
    T(x + 14, by + 27, title, 18, 'start', 'bold', '#fff')
    yy = PARA(x + 14, by + 66, role, bw - 28, 14, fill=col, weight='bold')
    yy += 6
    for it in items:
        o.append(f'<circle cx="{x + 20}" cy="{yy - 4}" r="3.2" fill="{col}"/>')
        yy = PARA(x + 32, yy, it, bw - 46, 13) + 6
# frecce flusso: Comandi → Quadro → Trazione → Cabina
for i in (1, 2, 3):
    xa = bx + i * (bw + gap) + bw
    ARROW(xa + 1, by + 20, xa + gap - 1, by + 20, NAV, 3)
T(20, 450, 'Il flusso principale è: Comandi → Quadro di controllo → Trazione → Cabina. L\'Energia alimenta tutti i blocchi; le Sicurezze '
           'possono fermare la macchina in qualsiasi momento.', 14, 'start', 'normal', SUB)

# ------------------------------------------------------------------ 2. una corsa
SECTION(480, '2 · Una corsa passo per passo')
steps = [
    ('Chiamata', 'L\'utente tiene premuto il tasto di destinazione, in cabina, al piano o dal radiocomando. Se lo rilascia, la cabina si ferma.'),
    ('Porta', 'Se la porta è aperta si richiude da sola e la serratura la blocca. Si parte solo a porta chiusa e bloccata.'),
    ('Verifiche', 'Il quadro controlla che le sicurezze siano chiuse, che non ci sia sovraccarico e che batteria e azionamento siano pronti.'),
    ('Partenza', 'Il freno si rilascia e la cabina accelera in modo graduale.'),
    ('Viaggio', 'Velocità nominale di 0,10 m/s (massima 0,15). Vicino al piano la cabina rallenta.'),
    ('Arrivo', 'La cabina si ferma da sola al piano. Il freno si richiude e tiene la posizione.'),
    ('Apertura', 'La serratura si sblocca e la porta si apre automaticamente. La luce di cabina si accende.'),
    ('Attesa', 'Dopo circa 20 secondi la porta si richiude. Se si preme il tasto del piano dove ci si trova, la porta si riapre.'),
]
sx, sy, sw, sh, sg = 17, 522, 280, 188, 18
for i, (t, d) in enumerate(steps):
    x = sx + i * (sw + sg)
    R(x, sy, sw, sh, '#fff', NAV, 1.8)
    o.append(f'<circle cx="{x + 30}" cy="{sy + 32}" r="18" fill="{NAV}"/>')
    T(x + 30, sy + 39, str(i + 1), 20, 'middle', 'bold', '#fff')
    T(x + 58, sy + 39, t, 19, 'start', 'bold', NAV)
    PARA(x + 14, sy + 78, d, sw - 28, 14)
    if i < len(steps) - 1:
        ARROW(x + sw + 1, sy + 32, x + sw + sg - 1, sy + 32, NAV, 3)
T(20, 736, 'La macchina funziona sempre ad azione mantenuta: il movimento dura finché il tasto è premuto. Dopo un arresto di protezione '
           'bisogna rilasciare e premere di nuovo il tasto per ripartire.', 14, 'start', 'normal', SUB)

# ------------------------------------------------------------------ 3. stati
SECTION(766, '3 · Gli stati della macchina')
states = [
    ('A riposo', GRN, 'Cabina ferma al piano, freno chiuso. Le batterie si ricaricano quando serve. Luce spenta dopo l\'uso.',
     'Esce con la pressione di un tasto di chiamata.'),
    ('In viaggio', GRN, 'Salita o discesa a uomo presente, con rallentamento vicino al piano.',
     'Si ferma al piano o quando si rilascia il tasto.'),
    ('Porta in manovra', GRN, 'Chiusura con blocco, oppure sblocco e apertura. Segnalazione di riapertura accesa.',
     'Termina a porta chiusa e bloccata o aperta.'),
    ('Sovraccarico', AMB, 'Si accendono lampada e segnale acustico; la porta resta aperta e la macchina non risponde ai comandi.',
     'Si riprende da soli togliendo il carico in eccesso.'),
    ('Fermata per ostacolo', AMB, 'Il fondo sensibile ferma la discesa e il tetto sensibile ferma la salita. Il fondo consente di risalire per liberare l\'ostacolo.',
     'Si libera l\'ostacolo e si preme di nuovo il tasto.'),
    ('Attesa rilascio tasto', AMB, 'Dopo un arresto di protezione la macchina aspetta che l\'utente lasci il tasto, per evitare ripartenze involontarie.',
     'Si rilascia e si preme di nuovo.'),
    ('Stop di emergenza premuto', AMB, 'Il fungo rosso in cabina ferma tutto e resta bloccato. La macchina non risponde ai comandi.',
     'Si sblocca ruotando il fungo e si preme di nuovo il tasto.'),
    ('Mancanza di rete', BLU, 'La macchina continua a funzionare a batteria, con luce e allarme: 10 corse di emergenza garantite.',
     'Torna normale al ritorno della rete.'),
    ('Batteria scarica', AMB, 'La macchina va fuori servizio al piano e consente solo la riapertura della porta per uscire.',
     'Riprende quando la batteria si è ricaricata a sufficienza.'),
    ('Blocco di sicurezza', RED, 'Si è aperta una sicurezza (paracadute, cinghie, finecorsa superiore, fusibile). La macchina resta ferma e sul display compare l\'errore.',
     'Reset e verifica da parte di un tecnico.'),
    ('Emergenza con spina', GRY, 'Solo per tecnici: una spina nel quadro esclude le sicurezze e permette movimenti lenti a uomo presente (circa 0,05 m/s).',
     'Si toglie la spina e si riporta la macchina in servizio.'),
    ('Collaudo paracadute', GRY, 'Solo per tecnici: prova di caduta a pieno carico dal piano alto, da menu del quadro.',
     'Poi la macchina è bloccata finché non viene ripristinata con la spina.'),
]
cx0, cy0, cw, ch, cg = 20, 808, 575, 132, 20
for i, (t, col, what, how) in enumerate(states):
    r, c = divmod(i, 4)
    x, y = cx0 + c * (cw + cg), cy0 + r * (ch + 14)
    R(x, y, cw, ch, '#fff', col, 2)
    o.append(f'<rect x="{x}" y="{y}" width="10" height="{ch}" rx="4" fill="{col}"/>')
    T(x + 24, y + 28, t, 18, 'start', 'bold', col)
    yy = PARA(x + 24, y + 54, what, cw - 44, 13.5)
    PARA(x + 24, yy + 8, 'Come si riparte: ' + how, cw - 44, 13, fill=SUB)

# ------------------------------------------------------------------ 4. sicurezze
SECTION(1270, '4 · Le sicurezze: cosa protegge chi', RED)
safeties = [
    ('Comando a uomo presente', 'La cabina si muove solo finché si tiene premuto il tasto; al rilascio si ferma.'),
    ('Stop di emergenza in cabina', 'Ferma la macchina e toglie l\'alimentazione agli attuatori; resta bloccato fino allo sblocco volontario.'),
    ('Fondo sensibile', 'Se tocca un ostacolo sotto la pedana ferma la discesa; permette di risalire per liberarlo.'),
    ('Tetto sensibile', 'Se rileva un\'ostruzione in alto ferma la salita.'),
    ('Sensori di allentamento cinghie', 'Quattro sensori fermano la macchina se una cinghia perde tensione; coprono anche l\'intervento del paracadute del contrappeso.'),
    ('Paracadute di cabina', 'Dispositivo meccanico brevettato che blocca la cabina sulle guide in caso di velocità eccessiva; un contatto ferma anche la macchina.'),
    ('Finecorsa superiore', 'Impedisce di superare il piano alto.'),
    ('Blocco delle porte', 'La macchina si muove solo a porte chiuse e bloccate; la porta si sblocca soltanto in prossimità del piano.'),
    ('Controllo del carico', 'Se la cabina è sovraccarica non parte e avvisa con luce e suono.'),
    ('Doppio freno a molle', 'Tiene la cabina ferma anche in assenza di energia e si chiude quando la catena delle sicurezze si apre.'),
    ('Funzionamento a batteria', 'In assenza di rete la macchina continua a funzionare a batteria e consente di completare la corsa e uscire.'),
    ('Sezionatori lucchettabili', 'Rete e batteria si sezionano separatamente per lavorare in sicurezza.'),
]
kx, ky, kw, kh, kg = 20, 1312, 775, 82, 12
for i, (t, d) in enumerate(safeties):
    c, r = divmod(i, 4)
    x, y = kx + c * (kw + 20), ky + r * (kh + kg)
    R(x, y, kw, kh, '#fff', RED, 1.6)
    T(x + 16, y + 28, t, 16.5, 'start', 'bold', RED)
    PARA(x + 16, y + 50, d, kw - 32, 13.5)

# ------------------------------------------------------------------ 5. situazioni, dati, sviluppo
SECTION(1720, '5 · Dati di prodotto e funzioni in sviluppo', NAV)
dy = 1766
R(20, dy, 1170, 250, '#fff', NAV, 1.6)
T(36, dy + 28, 'Dati di prodotto (configurazione CP.TEC.025 Rev.5)', 17, 'start', 'bold', NAV)
rows = [
    ('Tipo', 'Homelift a pianta tonda, senza vano, 2 fermate'),
    ('Portata', '200 kg, 2 persone'),
    ('Velocità', '0,10 m/s nominale, 0,15 m/s massima'),
    ('Corsa', 'Da 2500 a 4500 mm, senza fossa'),
    ('Cabina', 'Ø interno 1000 mm, altezza 2000 mm, policarbonato panoramico'),
    ('Trazione', 'Motoriduttore brushless 48 Vdc, cinghie dentate e contrappesi'),
    ('Alimentazione', '230 Vac monofase 2 A (0,5 kW); batterie 48 V; 10 corse di emergenza'),
    ('Uso', '10 corse all\'ora garantite'),
    ('Installazione e garanzia', '2 installatori, 2 giorni; manutenzione semestrale; garanzia 5 anni'),
]
for i, (k, v) in enumerate(rows):
    yy = dy + 54 + i * 21
    T(36, yy, k, 13.5, 'start', 'bold', SUB)
    T(240, yy, v, 13.5)

R(1210, dy, 1170, 250, '#fffaf0', AMB, 1.8)
T(1226, dy + 28, 'In sviluppo o da confermare', 17, 'start', 'bold', AMB)
dev = [
    'Modalità manutenzione dal quadro di servizio, consentita solo con la cabina al piano basso e con segnale acustico se è fuori piano: prevista.',
    'Ripartenza in discesa dopo l\'intervento del tetto sensibile: in definizione.',
    'Funzioni di servizio previste dall\'analisi dei rischi (tempo limite in emergenza, controllo della presenza di persone con sicurezze escluse): in sviluppo.',
    'Sensore a infrarossi sulla porta di cabina: sviluppo futuro. Display touch: opzione.',
    'Consumo in stand-by di 1 W: da certificare. Certificazioni e conformità: vedere il fascicolo tecnico.',
]
yy = dy + 54
for d in dev:
    o.append(f'<circle cx="{1236}" cy="{yy - 4}" r="3.2" fill="{AMB}"/>')
    yy = PARA(1250, yy, d, 1110, 13.5) + 6

# ------------------------------------------------------------------ legenda stati
T(20, 2038, 'Colori degli stati: verde = funzionamento normale · giallo = attenzione o attesa · blu = alimentazione di emergenza · '
            'rosso = blocco di sicurezza · grigio = solo tecnici.', 13, 'start', 'normal', SUB)
T(20, 2058, 'Per dettagli tecnici, anomalie note e stato delle correzioni: docs/registro_issue.md e docs/diagramma_funzionale.svg (versione completa).',
  12.5, 'start', 'normal', SUB)

o.append('</svg>')
out = sys.argv[1] if len(sys.argv) > 1 else 'diagramma_funzionale_light.svg'
with open(out, 'w', encoding='utf-8') as f:
    f.write('\n'.join(o))
print('scritto', out)
