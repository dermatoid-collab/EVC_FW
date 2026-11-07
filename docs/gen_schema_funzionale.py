#!/usr/bin/env python3
"""Genera lo schema funzionale (SVG) di Elevance / TTF."""
import html
import sys

W, H = 2300, 1560
o = []

RED = '#c0392b'     # catena sicurezze
ORA = '#d35400'     # potenza 48 V
BLU = '#2563eb'     # CAN
GRY = '#777777'     # segnali
GRN = '#2e8b57'     # monitor via UDEC.C
PUR = '#7b3fa0'     # monitor via UDEC.CX
BLK = '#111111'     # monitor TTF-Base
TXT = '#1f2937'


def esc(s):
    return html.escape(s, quote=True)


def T(x, y, s, size=11, anchor='start', weight='normal', fill=TXT, extra=''):
    o.append(f'<text x="{x}" y="{y}" font-size="{size}" text-anchor="{anchor}" '
             f'font-weight="{weight}" fill="{fill}" {extra}>{esc(s)}</text>')


def ML(x, y, lines, size=11, lh=14, **k):
    for i, l in enumerate(lines):
        T(x, y + i * lh, l, size, **k)


def R(x, y, w, h, fill='#ffffff', stroke='#555', sw=1.3, rx=6, dash=None):
    d = f' stroke-dasharray="{dash}"' if dash else ''
    o.append(f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="{rx}" '
             f'fill="{fill}" stroke="{stroke}" stroke-width="{sw}"{d}/>')


def L(x1, y1, x2, y2, color=GRY, w=1.5, dash=None, arrow=False):
    d = f' stroke-dasharray="{dash}"' if dash else ''
    m = f' marker-end="url(#a{color[1:]})"' if arrow else ''
    o.append(f'<line x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}" stroke="{color}" '
             f'stroke-width="{w}"{d}{m}/>')


def P(points, color=GRY, w=1.5, dash=None, arrow=False):
    d = f' stroke-dasharray="{dash}"' if dash else ''
    m = f' marker-end="url(#a{color[1:]})"' if arrow else ''
    pts = ' '.join(f'{x},{y}' for x, y in points)
    o.append(f'<polyline points="{pts}" fill="none" stroke="{color}" '
             f'stroke-width="{w}"{d}{m} stroke-linejoin="round"/>')


def B(x, y, w, h, title, lines=(), fill='#ffffff', stroke='#555', tsize=13,
      size=11, lh=14, tcolor=TXT, dash=None):
    R(x, y, w, h, fill, stroke, dash=dash)
    T(x + 10, y + 18, title, tsize, weight='bold', fill=tcolor)
    ML(x + 10, y + 36, lines, size, lh)


def DOT(x, y, color, r=5, hollow=False):
    if hollow:
        o.append(f'<circle cx="{x}" cy="{y}" r="{r}" fill="#fff" stroke="{color}" stroke-width="2"/>')
    else:
        o.append(f'<circle cx="{x}" cy="{y}" r="{r}" fill="{color}"/>')


def CONTACT(x, y, color=RED):
    """Contatto NC su linea orizzontale a quota y, da x a x+36."""
    L(x, y, x + 8, y, color, 2.4)
    DOT(x + 8, y, color, 2.5)
    L(x + 8, y, x + 30, y - 13, color, 2.4)
    L(x + 36, y, x + 28, y, color, 2.4)
    DOT(x + 28, y, color, 2.5)
    L(x + 28, y, x + 36, y, color, 2.4)


def NODE(x, y, name, dots, above=True, off=0):
    for i, c in enumerate(dots):
        if c is None:
            DOT(x + i * 9 - (len(dots) - 1) * 4.5, y, '#999', 5, hollow=True)
        else:
            DOT(x + i * 9 - (len(dots) - 1) * 4.5, y, c, 5)
    T(x, y - 12 - off, name, 11, 'middle', 'bold')


# ---------------------------------------------------------------- sfondo
o.append(f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}" '
         'font-family="DejaVu Sans, Arial, Helvetica, sans-serif">')
o.append('<defs>')
for c in (RED, ORA, BLU, GRY, GRN, PUR, BLK):
    o.append(f'<marker id="a{c[1:]}" markerWidth="9" markerHeight="9" refX="8" refY="4.5" '
             f'orient="auto"><path d="M0,0 L9,4.5 L0,9 z" fill="{c}"/></marker>')
o.append('</defs>')
o.append(f'<rect width="{W}" height="{H}" fill="#fafafa"/>')
T(40, 34, 'ELEVANCE / TTF — Schema funzionale elettrico e catena delle sicurezze', 20, weight='bold')
T(40, 54, 'Ricostruito da schemi KiCad (TTF.pdf, SafetyIn, Output), manuale IM.TEC.158 e firmware TTF-Base. '
          'Schema funzionale, non sostituisce gli schemi elettrici.', 11, fill='#555')

# ---------------------------------------------------------------- RIGA 1: catena
CY = 210
T(40, 84, 'CATENA DELLE SICUREZZE — 24 V in serie (contatti mostrati chiusi = macchina abilitata)', 14,
  weight='bold', fill=RED)

# gruppi
R(575, 112, 640, 192, '#f2faf5', GRN, 1.2, 10, '6 4')
T(585, 130, 'COLONNA CABINA · UDEC.C (connettori P60…P66)', 11, weight='bold', fill=GRN)
R(1215, 112, 490, 192, '#f7f1fa', PUR, 1.2, 10, '6 4')
T(1225, 130, 'CIELINO · UDEC.CX (X2, X3, X15) e fondo/tetto sensibili', 11, weight='bold', fill=PUR)
R(40, 112, 530, 192, '#f4f6fa', '#7a8aa0', 1.2, 10, '6 4')
T(50, 130, 'CAPPELLO · TTF-Base (X23, X19, X17)', 11, weight='bold', fill='#3a4a60')
R(1710, 112, 240, 192, '#f4f6fa', '#7a8aa0', 1.2, 10, '6 4')
T(1720, 130, 'CAPPELLO · quadro (X15)', 11, weight='bold', fill='#3a4a60')

# linea principale
L(50, CY, 2090, CY, RED, 3)

# +24 V e fusibile
T(52, CY - 36, '+24 V', 11, weight='bold', fill=RED)
T(52, CY - 22, '(DC/DC G1)', 9, fill='#555')
R(70, CY - 11, 44, 22, '#fff', RED, 2, 3)
T(92, CY + 4, 'F1 1A', 10, 'middle', 'bold')
T(92, CY + 30, 'fusibile catena', 9, 'middle', fill='#555')
T(92, CY + 42, '(F7 in IM.158)', 9, 'middle', fill='#555')

NODE(150, CY, 'S1', [BLK])

CONTACT(190, CY)
T(208, CY + 24, 'Extracorsa', 10, 'middle')
T(208, CY + 36, 'superiore S8', 10, 'middle')
T(208, CY + 48, '(X23)', 10, 'middle')
NODE(280, CY, 'S2', [BLK])

for i, (x, n) in enumerate([(305, 'S3'), (362, 'S4'), (419, 'S5'), (476, 'S6')]):
    CONTACT(x, CY)
    T(x + 18, CY + 24, n, 10, 'middle')
T(322, CY + 40, 'cinghie cabina (X19)', 10, 'middle')
T(448, CY + 40, 'contrappesi (X17)', 10, 'middle')
T(385, CY + 56, 'ALLENTAMENTO CINGHIE', 10, 'middle', 'bold')
NODE(552, CY, 'S3', [BLK, PUR])

# cabina UDEC.C
CONTACT(595, CY)
CONTACT(642, CY)
T(613, CY + 24, 'S15', 10, 'middle')
T(660, CY + 24, 'S16', 10, 'middle')
T(637, CY + 38, 'PAR paracadute', 10, 'middle')
NODE(712, CY, 's31', [GRN])
CONTACT(735, CY)
T(753, CY + 24, 'S17', 10, 'middle')
T(753, CY + 38, 'PEC e-stop', 10, 'middle')
T(753, CY + 50, 'cabina', 10, 'middle')
NODE(808, CY, 's32', [GRN])
CONTACT(830, CY)
T(848, CY + 24, 'PET', 10, 'middle')
T(848, CY + 38, 'accostam.', 10, 'middle')
NODE(902, CY, 's33', [GRN])
CONTACT(922, CY)
CONTACT(968, CY)
T(940, CY + 24, 'COL', 10, 'middle')
T(986, CY + 24, 'COL', 10, 'middle')
T(963, CY + 38, 'blocco porta', 10, 'middle')
NODE(1040, CY, 's34', [GRN])
T(1085, CY + 24, 'TC', 10, 'middle')
T(1085, CY + 38, 'ponte P64', 10, 'middle', fill='#a0522d')
NODE(1130, CY, 's35', [GRN])
T(1170, CY + 24, 'FT', 10, 'middle')
T(1170, CY + 38, 'ponte P65/66', 10, 'middle', fill='#a0522d')
NODE(1222, CY, 'S3A', [GRN, PUR])

# CX: ponte X15
R(1243, CY - 11, 58, 22, '#fff', '#a0522d', 1.4, 3)
T(1272, CY + 4, 'ponte', 10, 'middle', fill='#a0522d')
T(1272, CY + 24, 'ponte X15', 9, 'middle', fill='#a0522d')
T(1272, CY + 36, '(contatto VFD', 9, 'middle', fill='#a0522d')
T(1272, CY + 48, 'assente)', 9, 'middle', fill='#a0522d')
NODE(1340, CY, 'S3B', [None])
T(1350, CY + 62, 'S3B non letto', 9, 'end', fill='#777')
T(1350, CY + 74, 'dalla TTF', 9, 'end', fill='#777')

# trap con bypass K1C
CONTACT(1368, CY)
CONTACT(1414, CY)
T(1386, CY - 22, 'S11', 10, 'middle')
T(1432, CY - 22, 'S12', 10, 'middle')
T(1410, CY + 66 + 44, '', 10)
P([(1356, CY), (1356, CY + 56), (1452, CY + 56), (1452, CY)], ORA, 1.8)
CONTACT(1380, CY + 56, ORA)
T(1405, CY + 74, 'K1C bypass trap', 10, 'middle', fill=ORA)
T(1405, CY + 86, '(en_byp_top)', 9, 'middle', fill=ORA)
T(1405, CY - 40, 'tetto sensibile (trap)', 10, 'middle')
NODE(1486, CY, 'S3C', [PUR])

# tray con bypass K2C
CONTACT(1516, CY)
CONTACT(1562, CY)
T(1534, CY - 22, 'S13', 10, 'middle')
T(1580, CY - 22, 'S14', 10, 'middle')
P([(1504, CY), (1504, CY + 56), (1600, CY + 56), (1600, CY)], ORA, 1.8)
CONTACT(1528, CY + 56, ORA)
T(1553, CY + 74, 'K2C bypass tray', 10, 'middle', fill=ORA)
T(1553, CY + 86, '(en_byp_bot)', 9, 'middle', fill=ORA)
T(1553, CY - 40, 'fondo sensibile (tray)', 10, 'middle')
NODE(1650, CY, 'S4', [BLK, PUR])
T(1658, CY + 44, '→ STO VFD (?)', 9, 'middle', fill='#777')

# e-stop quadro
CONTACT(1726, CY)
T(1744, CY + 24, 'S9', 10, 'middle')
T(1744, CY + 38, 'E-stop quadro', 10, 'middle')
NODE(1818, CY, 'S5 (fw: s8)', [BLK])
T(1818, CY + 24, 'H9 «OK to run»', 9, 'middle', fill='#555')

# discesa verso bobine K3/K4
P([(2090, CY), (2090, 590)], RED, 3, arrow=True)
T(2100, 260, 'S5 → bobine', 10, fill=RED, weight='bold')
T(2100, 274, 'K3 (OTR) e K4 (freno)', 10, fill=RED)

# spina emergenza
P([(150, CY), (150, 326), (1678, 326), (1678, CY)], '#e67e22', 2.2, '9 5')
DOT(150, 326, '#e67e22')
DOT(1678, 326, '#e67e22')
T(160, 320, 'SPINA DI EMERGENZA X8 (pin 1–3): ponte da S1 al nodo prima di S9 → esclude tutti i contatti tranne E-stop quadro. '
            'Pin 2–4 → BYP-S14 (PD8): firmware = bypass, 0,05 m/s',
  11, fill='#b35a00', weight='bold')

# ---------------------------------------------------------------- tabella monitoraggio
TY = 346
R(40, TY, 2040 - 40, 190, '#ffffff', '#bbb', 1.0, 8)
T(52, TY + 20, 'Monitoraggio (cosa legge il firmware)', 13, weight='bold')
DOT(430, TY + 16, BLK)
T(440, TY + 20, 'TTF-Base (opto TLP293, ingressi PD3–PD8)', 11)
DOT(780, TY + 16, GRN)
T(790, TY + 20, 'UDEC.C → CAN125', 11)
DOT(940, TY + 16, PUR)
T(950, TY + 20, 'UDEC.CX → CAN125', 11)
DOT(1110, TY + 16, '#999', hollow=True)
T(1120, TY + 20, 'non monitorato', 11)

ML(52, TY + 44, [
    'S1  → DIn.s1   → TRIP_S1_FUSE (nodo dopo F1)',
    'S2  → DIn.s2   → TRIP_S2_OVERTRAVEL',
    'S3  → DIn.s3   → TRIP_S3_BELT   (confronto con CX s3: TRIP_CHAIN_CABLE)',
    'S4  → DIn.s4   → solo confronto con CX s4 (TRIP_CHAIN_CABLE)',
    'S5  → DIn.s8   → toglie run_mode.enable; nessun trip',
    'X8  → DIn.bypass → run_mode.bypass_manutenzione',
], 11, 16)
ML(650, TY + 44, [
    's31 PAR  → TRIP_S31_OVERSPEED  (bloccante)',
    's32 PEC  → TRIP_S32_CAR_ESTOP  (non bloccante)',
    's33 PET  → door_is_closed()',
    's34 COL  → door_is_locked()',
    's35 TC / s36 FT → s36 vs CX s3a',
    '          → TRIP_TTF_CX_CABLE (*)',
], 11, 16)
ML(1180, TY + 44, [
    'S3A → confronto con s36 (cablaggio di tetto)',
    'S3C → is_trap_ok()   (tetto sensibile)',
    'S4  → is_tray_ok()   (fondo sensibile)',
    'fbk K1/K2 ↔ comando → TRIP_TRAP_FB / TRIP_TRAY_FB (TON 2 s)',
    'top/bot uzone → is_at_P1() / is_at_P0(); entrambe → TRIP_UZONE',
    'traptray.state = TRAPTRAY_FAULT → TRIP_TRAPTRAY',
], 11, 16)
T(52, TY + 160, 'Dopo 5 s dall’avvio i trip di sicurezza sono memorizzati (TRIP_SAFETY) fino al reset: pmode_request(CHARGER_NOW), '
                'pmode_relay_set(false) e run_mode.enable = 0.  (*) TRIP_TTF_CX_CABLE inattivo per timer ERR_SYNC condiviso (docs/analisi.md §2.5).',
  10.5, fill='#444')
T(52, TY + 176, 'Con la spina X8 inserita check_safety() è disattivato; restano solo E-stop quadro, assenza di fault VFD/pmode e s8.', 10.5, fill='#444')

# ---------------------------------------------------------------- RIGA 2: potenza e controllo
Y2 = 580
T(40, Y2 - 8, 'ALIMENTAZIONE, CONTROLLO E POTENZA', 14, weight='bold', fill=ORA)

# alimentazione (sinistra)
B(40, Y2 + 10, 150, 60, 'Rete 230 Vac', ['monofase 2 A, 0,5 kW'], '#fff8f0', ORA)
B(220, Y2 + 10, 170, 60, 'Sezionatore rete', ['box sezionatori (S1)'], '#fff8f0', ORA)
B(420, Y2 + 10, 190, 60, 'NPB 450', ['caricabatterie 48 V', 'solo a macchina ferma'], '#fff8f0', ORA)
L(190, Y2 + 40, 220, Y2 + 40, ORA, 2.4, arrow=True)
L(390, Y2 + 40, 420, Y2 + 40, ORA, 2.4, arrow=True)
B(40, Y2 + 110, 190, 74, 'Batteria 48 V 12 Ah', ['fusibile 100 A MIDI sul +', 'protezione polarità inversa'],
  '#fff8f0', ORA)
B(250, Y2 + 110, 150, 74, 'Sezionatore', ['batteria (S2)', 'non aprire in marcia'], '#fff8f0', ORA)
L(230, Y2 + 147, 250, Y2 + 147, ORA, 2.4)
P([(400, Y2 + 147), (515, Y2 + 147), (515, Y2 + 70)], ORA, 2.4)
P([(515, Y2 + 147), (640, Y2 + 147)], ORA, 2.4)
DOT(640, Y2 + 147, ORA, 9)
T(640, Y2 + 151, '48', 9, 'middle', 'bold', '#fff')
T(578, Y2 + 138, 'BUS 48 V', 11, 'middle', 'bold', ORA)
T(600, Y2 + 176, '«48» → contatti K3', 10, 'middle', fill=ORA)
B(40, Y2 + 210, 290, 90, 'DC/DC G1 48 → 24 V', ['alimenta tutto tranne il movimento:', 'TTF-Base, catena (F1 1A),', 'cabina e UDEC.C/CX (CAR24V, F6)'],
  '#fff8f0', ORA, size=10.5)
L(135, Y2 + 184, 135, Y2 + 210, ORA, 2.4)
L(330, Y2 + 232, 410, Y2 + 232, RED, 2, arrow=True)
T(418, Y2 + 236, '24 V → catena (F1)', 10.5, fill=RED)
L(330, Y2 + 256, 410, Y2 + 256, GRY, 1.6, '5 3', arrow=True)
T(418, Y2 + 260, '24 V → UDEC.C / UDEC.CX', 10.5, fill=GRY)
L(330, Y2 + 280, 410, Y2 + 280, GRY, 1.6, '5 3', arrow=True)
T(418, Y2 + 284, '24 V → TTF-Base (logica)', 10.5, fill=GRY)
T(40, Y2 + 330, 'Con macchina in marcia il NPB è disabilitato (NPB-DIS, PE0): il VFD è alimentato solo dalla batteria.', 10, fill=ORA)
T(40, Y2 + 346, 'Rete assente: la macchina funziona a batteria (10 corse di emergenza dichiarate).', 10, fill=ORA)

# TTF-Base (centro)
R(680, Y2 + 10, 700, 420, '#f7f9fc', '#2a3b55', 1.8, 10)
T(830, Y2 + 32, 'TTF-Base (cappello) — STM32F446VC, IWDG ≈ 0,5 s', 14, weight='bold', fill='#2a3b55')

B(695, Y2 + 46, 200, 150, 'Ingressi sicurezze', ['S1 PD3 · S2 PD4', 'S3 PD5 · S4 PD6', 'S5 → «S8» PD7', 'BYP-S14 PD8 (spina X8)', 'opto TLP293, 3k3 + varistore',
                                                  'apertura rilevata subito,', 'chiusura dopo 5 cicli'], '#fff', BLK, size=10.5)
B(695, Y2 + 206, 200, 108, 'Altri ingressi', ['IN1/IN2 SU/GIÙ pannello', 'IN3/IN4 sblocco sup./inf.', 'OTR-FB (PB15)', 'BRK-FB (PC6, dopo K4)'],
  '#fff', BLK, size=10.5)
B(695, Y2 + 324, 200, 96, 'HMI / servizio', ['display LCD 8×2 + tasti', 'menu · reset UP+DN 3 s', 'USB · encoder (non usato)'],
  '#fff', BLK, size=10.5)

B(915, Y2 + 46, 230, 374, 'Logica (firmware)', [
    'run_mode: SU · GIÙ · APRI/CHIUDI',
    '  STUCK_UP · WAIT_FOR_CALL_RELEASE',
    '  FALL_TEST (collaudo paracadute)',
    'check_safety() → trip memorizzati',
    'abilitation_check():',
    '  s8 ∧ ¬fault VFD ∧ ¬fault pmode',
    '  ∧ ¬TRIP_SAFETY ∧ UDEC.C/CX online',
    'door_loop(): ciclo porta + serratura',
    'traptray: bypass K1/K2 via CAN',
    'pmode: carica ↔ marcia',
    '  K3 chiuso fino a 30 s dopo lo stop',
    'vfd: RPDO1 velocità/direzione',
    '  heartbeat NMT 100 ms su 2 bus',
    'battery/npb/ltc2959: carica e Ah',
], '#fff', '#2a3b55', size=10.5, lh=15)
B(1165, Y2 + 46, 200, 160, 'Uscite', ['OTR-EN PE1 → Q16B → K3', 'BRK-EN PC9 → Q16A → K4', 'VFD-PWR-EN PD10', 'NPB-DIS PE0 (stacca NPB)',
                                      'OUT3_1 → contattore S22', 'LED stato/carica'], '#fff', BLK, size=10.5)
B(1165, Y2 + 216, 200, 204, 'Comunicazione', ['CAN250 (cappello):', '  VFD 7BL00099, NPB', 'CAN125 (cabina):', '  UDEC.C, UDEC.CX,',
                                              '  operatori porta L2D', 'heartbeat TTF 100 ms', 'timeout: UDEC.C 1,5 s,', 'CX 0,5 s, VFD 0,2 s'],
  '#fff', BLU, size=10.5)

# blocco potenza (destra)
B(1480, Y2 + 10, 250, 120, 'K3 «OTR» (AHES4192)', [
    'contatti: 48 V+ e 48 V− → VFD',
    '  (K3/2, K3/1), tag «48»',
    'aus. K3/3 → OTR-FB (PB15)',
    'bobina: A1 ← S5, A2 ← MCU'], '#fff', ORA, size=10.5)
DOT(1480, Y2 + 60, ORA, 9)
T(1480, Y2 + 64, '48', 9, 'middle', 'bold', '#fff')
B(1480, Y2 + 160, 250, 170, 'VFD 7BL00099', [
    'alimentato dal 48 V via K3',
    'CAN250 · STO ← nodo S4 (?)',
    'uscita freno → K4',
    'ingresso encoder motore',
    'comm-timeout (SDO, 1 volta)',
    'uscite U/V/W → S22 → M1'], '#fff', ORA, size=10.5)
B(1480, Y2 + 360, 250, 70, 'K4 «BREN» (G2RL-2)', [
    'contatti: freno (entrambi i poli)',
    'bobina: A1 ← S5, A2 ← MCU'], '#fff', ORA, size=10.5)
B(1780, Y2 + 10, 440, 120, 'Bobine K3 / K4', [
    'S5 (catena 24 V) → A1',
    'A2 → NUD3160D → GND, comandato da OTR-EN / BRK-EN',
    'Con catena aperta K3 e K4 cadono in hardware,',
    'anche se il firmware le sta comandando'], '#fff2f0', RED, size=10.5)
B(1780, Y2 + 160, 190, 80, 'S22 contattore', ['uscita motore, da firmware', '(OUT3_1), NON in catena'], '#fff', ORA, size=10.5)
B(2010, Y2 + 160, 210, 80, 'Motore M1', ['brushless 48 V', 'trazione a cinghie'], '#fff', ORA)
B(1780, Y2 + 360, 230, 70, 'Freno M2', ['2 avvolgimenti, a molla,', 'rilascio elettrico'], '#fff', ORA, size=10.5)
B(2030, Y2 + 360, 190, 70, 'Paracadute', ['S15/S16 in catena', 'S23/S24 presenza'], '#fff', '#555', size=10.5)

# frecce riga 2
P([(1365, Y2 + 90), (1420, Y2 + 90), (1420, Y2 + 40), (1480, Y2 + 40)], GRY, 1.6, '5 3', arrow=True)   # OTR-EN -> K3
T(1372, Y2 + 82, 'OTR-EN', 9, fill=GRY)
P([(1365, Y2 + 120), (1440, Y2 + 120), (1440, Y2 + 395), (1480, Y2 + 395)], GRY, 1.6, '5 3', arrow=True)  # BRK-EN -> K4
T(1372, Y2 + 114, 'BRK-EN', 9, fill=GRY)
P([(1365, Y2 + 260), (1480, Y2 + 260)], BLU, 2.4, arrow=True)
T(1372, Y2 + 254, 'CAN250', 10, fill=BLU, weight='bold')
L(1520, Y2 + 130, 1520, Y2 + 160, ORA, 2.6, arrow=True)         # K3 -> VFD
T(1528, Y2 + 150, '48 V±', 10, fill=ORA)
L(1520, Y2 + 330, 1520, Y2 + 360, ORA, 2.6, arrow=True)         # VFD -> K4
T(1528, Y2 + 350, 'uscita freno', 10, fill=ORA)
L(1730, Y2 + 200, 1780, Y2 + 200, ORA, 2.6, arrow=True)         # VFD -> S22
L(1970, Y2 + 200, 2010, Y2 + 200, ORA, 2.6, arrow=True)         # S22 -> M1
L(1730, Y2 + 395, 1780, Y2 + 395, ORA, 2.6, arrow=True)         # K4 -> M2
P([(1750, Y2 + 130), (1750, Y2 + 150)], RED, 0)
L(1780, Y2 + 70, 1730, Y2 + 70, RED, 2.4, arrow=True)           # bobine -> K3 (azione)
T(1738, Y2 + 62, 'A1', 9, fill=RED)
P([(1760, Y2 + 130), (1760, Y2 + 345), (1730, Y2 + 345), (1730, Y2 + 372)], RED, 1.6, '5 3')   # S5 -> K4
P([(1760, Y2 + 372), (1730, Y2 + 372)], RED, 1.6, arrow=True)
T(1766, Y2 + 345, 'S5', 9, fill=RED)

# ingresso sicurezze alla TTF
L(760, 542, 760, Y2 + 46, BLK, 2, arrow=True)
T(768, 556, 'S1…S5, BYP-S14', 10, fill=BLK)

# ---------------------------------------------------------------- RIGA 3: cabina
Y3 = 1060
T(40, Y3 - 12, 'CABINA E COMANDI DI PIANO', 14, weight='bold', fill=GRN)
P([(1000, Y2 + 430), (1000, Y3 + 20)], BLU, 2.6)
P([(1000, Y3 + 20), (1560, Y3 + 20)], BLU, 2.6)
T(1010, Y3 + 14, 'CAN125 — cavi catena nel montante destro', 11, fill=BLU, weight='bold')
for x in (1000, 1560):
    L(x, Y3 + 20, x, Y3 + 50, BLU, 2.4, arrow=True)

B(700, Y3 + 50, 560, 300, 'UDEC.C (colonna cabina)', [
    'Ingressi:', '  sb_pc0/pc1 chiamate P0/P1 · sb_pc5 riapri porta',
    '  sb_pc6/pc7 radiocomando (TLC) · pc0+pc1 3 s = reset',
    '  ftc1 magnete di piano (rallenta a 0,05 m/s)',
    '  ftc3 sovraccarico (pesacarico) · monitor s31…s36',
    'Uscite:', '  op1…op3 apri/chiudi (operatori porta)',
    '  hl_ovl + hl_buz sovraccarico · hl_occ occupato',
    '  hl_pc0/pc1 luminose · hl_emg',
    'Contatti in catena: PAR, PEC, PET, COL, TC, FT',
    'Stop cabina (fungo S17), allarme → cicalino su tetto',
], '#f2faf5', GRN, size=10.5, lh=16)

B(1300, Y3 + 50, 560, 300, 'UDEC.CX (cielino)', [
    'Ingressi (CAN → TTF):', '  s3, s3a, s3c (trap), s4 (tray)',
    '  fbk_byp_top / fbk_byp_bot (contatti K1B/K2B)',
    '  top_uzone / bot_uzone (zone di sblocco)',
    'Uscite (da TTF via CAN125):',
    '  en_byp_top → Q2B → K1 · en_byp_bot → Q2A → K2',
    '  elt → elettroserratura U4 (PWM 100 % poi 30 %)',
    'Altro: celle di carico, luce cabina LED, 1-Wire',
    'Se manca il battito di TTF (0,5 s):',
    '  bypass e serratura diseccitati',
    'Filtro ingressi 20 cicli (CX); TTF: 5 cicli solo in salita',
], '#f7f1fa', PUR, size=10.5, lh=16)

B(40, Y3 + 50, 620, 150, 'Sensori di corsa e di zona', [
    'S7 zona di sblocco piano superiore (cappello, X16 TOP-ZONE)',
    '    → UDEC.CX top_uzone: sblocco porta e fermata al P1',
    'S10 zona di sblocco piano inferiore (P0) → bot_uzone',
    'S18 livello di piano (magnete) → UDEC.C ftc1',
    'Celle di carico → pesacarico → ftc3 (percorso da verificare)',
    'Extracorsa superiore S8 e allentamenti S3–S6: in catena',
], '#fff', '#555', size=10.5, lh=16)
B(40, Y3 + 220, 620, 130, 'Porta di cabina', [
    'Operatori porta (2 ante, bassa energia) ← UDEC.C op1…op3',
    'Elettroserratura U4 ← CX elt (diseccitata = bloccata, a molla)',
    '  contatti catenaccio (COL) e accostamento (PET) → catena',
    'Chiave di emergenza EN 81-20: sezionatori aperti (IM.159 §12)',
], '#fff', '#555', size=10.5, lh=16)

B(1890, Y3 + 50, 330, 300, 'Modi di funzionamento', [
    'Normale: a uomo presente da cabina,',
    '  piani e radiocomando; arresto al piano',
    'Emergenza: spina X8, 0,05 m/s, comandi',
    '  del pannello (nel fw anche UDEC.C)',
    'Collaudo paracadute: menu del quadro,',
    '  freno aperto a mano dal quadro',
    'Batteria scarica: ferma al piano,',
    '  solo riapertura porta',
    'Reset trip: UP+DN 3 s (riavvio)',
    'Rete assente: batteria, 10 corse',
], '#fff', '#555', size=10.5, lh=16)

# ---------------------------------------------------------------- legenda
LY = 1440
R(40, LY, 2210 - 40, 100, '#fff', '#bbb', 1.0, 8)
T(52, LY + 20, 'Legenda', 12, weight='bold')
L(52, LY + 38, 112, LY + 38, RED, 3); T(120, LY + 42, 'catena sicurezze 24 V', 10.5)
L(52, LY + 58, 112, LY + 58, ORA, 3); T(120, LY + 62, 'potenza 48 V / bypass', 10.5)
L(52, LY + 78, 112, LY + 78, BLU, 3); T(120, LY + 82, 'bus CAN', 10.5)
L(320, LY + 38, 380, LY + 38, GRY, 1.6, '5 3'); T(388, LY + 42, 'segnale di comando / controllo', 10.5)
L(320, LY + 58, 380, LY + 58, '#e67e22', 2.2, '9 5'); T(388, LY + 62, 'spina di emergenza', 10.5)
DOT(330, LY + 78, BLK); T(342, LY + 82, 'monitor su TTF-Base', 10.5)
DOT(560, LY + 78, GRN); T(572, LY + 82, 'su UDEC.C', 10.5)
DOT(680, LY + 78, PUR); T(692, LY + 82, 'su UDEC.CX', 10.5)
T(820, LY + 42, 'Incertezze:  S3B non letto da TTF · STO VFD sul nodo S4 (dal disegno) · percorso celle di carico → ftc3 ·', 10.5, fill='#7a4b00')
T(820, LY + 58, 'relè K1/K2/K3/K4 non dichiarati a contatti guidati sugli schemi, la scheda Notion «Bypass trap/tray» dice di sì (HW-03) ·', 10.5, fill='#7a4b00')
T(820, LY + 74, 'Bus CAN confermati dal codice: CAN250 = VFD e NPB; CAN125 = UDEC.C, UDEC.CX e operatori porta L2D. Issue: docs/registro_issue.md.', 10.5, fill='#7a4b00')
T(820, LY + 90, 'Analisi dei rischi Rev.7 letta (voci nel registro).  Sorgente: docs/gen_schema_funzionale.py.', 10.5, fill='#7a4b00')

o.append('</svg>')

out = sys.argv[1]
with open(out, 'w', encoding='utf-8') as f:
    f.write('\n'.join(o))
print('scritto', out, len('\n'.join(o)))
