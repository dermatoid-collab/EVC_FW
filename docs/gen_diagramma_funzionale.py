#!/usr/bin/env python3
"""Genera il diagramma funzionale completo di Elevance / TTF (SVG).

Uso:  python3 docs/gen_diagramma_funzionale.py docs/diagramma_funzionale.svg

Il diagramma è ricostruito dal firmware TTF-Base (run_mode.c, door.c, pmode.c,
vfd.c, battery.c, application.c, trips.c) e dai manuali IM.TEC.158/159/160/161.
I riquadri rossi «FW-xx», «HW-xx», … rimandano al registro docs/registro_issue.md:
quando un'issue viene risolta, aggiornare il registro e togliere il riquadro.
"""
import html
import sys

W, H = 2400, 2060
o = []

RED = '#c0392b'
ORA = '#d35400'
BLU = '#2563eb'
GRY = '#6b7280'
GRN = '#2e8b57'
PUR = '#7b3fa0'
TXT = '#1f2937'
NAV = '#2a3b55'


def esc(s):
    return html.escape(s, quote=True)


def T(x, y, s, size=11, anchor='start', weight='normal', fill=TXT):
    o.append(f'<text x="{x}" y="{y}" font-size="{size}" text-anchor="{anchor}" '
             f'font-weight="{weight}" fill="{fill}">{esc(s)}</text>')


def ML(x, y, lines, size=10.5, lh=14, **k):
    for i, l in enumerate(lines):
        T(x, y + i * lh, l, size, **k)


def R(x, y, w, h, fill='#fff', stroke='#555', sw=1.3, rx=7, dash=None):
    d = f' stroke-dasharray="{dash}"' if dash else ''
    o.append(f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="{rx}" fill="{fill}" '
             f'stroke="{stroke}" stroke-width="{sw}"{d}/>')


def PL(points, color=GRY, w=1.8, dash=None, arrow=True):
    d = f' stroke-dasharray="{dash}"' if dash else ''
    m = f' marker-end="url(#a{color[1:]})"' if arrow else ''
    pts = ' '.join(f'{x},{y}' for x, y in points)
    o.append(f'<polyline points="{pts}" fill="none" stroke="{color}" stroke-width="{w}"{d}{m} '
             'stroke-linejoin="round"/>')


def BUB(x, y, n, color=NAV):
    o.append(f'<circle cx="{x}" cy="{y}" r="10" fill="#fff" stroke="{color}" stroke-width="1.5"/>')
    T(x, y + 3.5, str(n), 10, 'middle', 'bold', color)


def TAG(x, y, s):
    w = 7.0 * len(s) + 10
    o.append(f'<rect x="{x}" y="{y - 11}" width="{w:.0f}" height="15" rx="4" fill="#fdecea" '
             f'stroke="{RED}" stroke-width="1"/>')
    T(x + 5, y, s, 9.5, 'start', 'bold', RED)
    return w


def STATE(x, y, w, h, title, lines, fill='#fff', stroke=NAV, tags=()):
    R(x, y, w, h, fill, stroke, 1.8, 10)
    T(x + 10, y + 18, title, 12.5, 'start', 'bold', stroke)
    ML(x + 10, y + 34, lines)
    tx = x + w - 6
    for t in reversed(tags):
        wd = 7.0 * len(t) + 10
        tx -= wd
        TAG(tx, y - 3, t)
        tx -= 4


def PANEL(x, y, w, h, title, color):
    R(x, y, w, h, '#ffffff', color, 1.4, 12)
    T(x + 14, y + 22, title, 14, 'start', 'bold', color)


# ------------------------------------------------------------------ preambolo
o.append(f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}" '
         'font-family="DejaVu Sans, Arial, Helvetica, sans-serif">')
o.append('<defs>')
for c in (RED, ORA, BLU, GRY, GRN, PUR, NAV):
    o.append(f'<marker id="a{c[1:]}" markerWidth="9" markerHeight="9" refX="8" refY="4.5" '
             f'orient="auto"><path d="M0,0 L9,4.5 L0,9 z" fill="{c}"/></marker>')
o.append('</defs>')
o.append(f'<rect width="{W}" height="{H}" fill="#f6f7f9"/>')
T(30, 34, 'ELEVANCE / TTF — Diagramma funzionale della macchina', 22, 'start', 'bold')
T(30, 54, 'Ricostruito dal firmware TTF-Base e dai manuali IM.TEC.158–161. I riquadri rossi rimandano alle issue di '
          'docs/registro_issue.md. Stato del firmware: commit di import, nessuna correzione applicata.', 11.5, fill='#555')

# =================================================================== PANNELLO 1
PANEL(20, 70, 1500, 960, '1 · Macchina a stati del movimento (run_mode.c) — scansione ciclica, 1 passo per giro di main_loop', NAV)

BW, BH = 240, 100
INIT = (50, 120)
FALL = (50, 330)
NORM = (560, 330)
UPS = (1130, 120)
DWN = (1130, 520)
STK = (1130, 790)
WAI = (760, 840)
OPN = (200, 600)
CLS = (200, 840)

STATE(*INIT, BW, BH, 'INIT_SEQ', ['door_reset · abilita TLC e uscite P71', 'vfd_stop · NPB: modo PSU, EEPROM off',
                                  'VFD: SDO timeout comunicazione', 'L2D_PARAM saltato (debug)'],
      '#f4f6fa', tags=('FW-04', 'FW-25'))
STATE(*FALL, BW, BH, 'FALL_TEST (collaudo paracadute)', ['relè ON; freno aperto da tasto 2 o IN4', 'se s8; senza tasti: vfd_halt',
                                                        'esce dopo 20 s senza tasti/chiamate'],
      '#fff7ec', ORA, tags=('FW-01', 'FW-19'))
STATE(*NORM, BW, BH, 'NORMAL (fermo)', ['traptray NO_BYPASS · vfd_stop 1 s', 'scansione chiamate P0/P1, riapri',
                                       'batteria scarica al piano → carica', 'reset da quadro/COP 3 s → reboot'],
      '#eef6ff', BLU, tags=('FW-22',))
STATE(*UPS, BW, BH, 'UP', ['traptray TRAY_BYPASS', 'wait_enable(trap ok) → brake_release', '→ go_up 0,10 m/s (0,05 con magnete',
                          'o in bypass); porta aperta → CLOSE'], '#eef9f1', GRN)
STATE(*DWN, BW, BH, 'DOWN', ['traptray NO_BYPASS', 'wait_enable(trap ∧ tray) → brake_release', '(marcia SU a 11 rpm) → go_down',
                            'tetto premuto: nessun bypass trap'], '#eef9f1', GRN, tags=('FW-02', 'FW-06'))
STATE(*STK, BW, BH, 'STUCK_UP', ['porta ferma · NO_BYPASS', 'wait_for_enable = 1', 'attende rilascio ostacolo'],
      '#fdf3f2', RED, tags=('FW-21',))
STATE(*WAI, BW, BH, 'WAIT_FOR_CALL_RELEASE', ['occupato acceso · NO_BYPASS', 'porta ferma, VFD in halt',
                                              'riparte solo dopo rilascio tasti'], '#fdf3f2', RED)
STATE(*OPN, BW, BH, 'OPEN_DOOR (al piano)', ['controllo sovraccarico: porta aperta,', 'luce + cicalino · door_loop(OPEN)',
                                            'parcheggio 20 s · sblocco subito dopo stop'], '#f4f4fb', PUR, tags=('FW-03', 'FW-12'))
STATE(*CLS, BW, BH, 'CLOSE_DOOR', ['sovraccarico → riapre · S3 assente →', 'reset porta · door_loop(CLOSE)',
                                  'riapri/PAP/chiamata al piano → OPEN'], '#f4f4fb', PUR, tags=('FW-07',))

# archi (percorsi ortogonali)
PL([(290, 150), (680, 150), (680, 330)], NAV); BUB(500, 150, 1)                                 # 1
PL([(800, 355), (980, 355), (980, 150), (1130, 150)], GRN); BUB(980, 250, 2)                    # 2 NORMAL->UP
PL([(1130, 200), (1040, 200), (1040, 385), (800, 385)], GRN); BUB(1040, 290, 9)                 # 9 UP->NORMAL
PL([(800, 410), (920, 410), (920, 545), (1130, 545)], GRN); BUB(920, 480, 3)                    # 3 NORMAL->DOWN
PL([(1130, 590), (1000, 590), (1000, 428), (800, 428)], GRN); BUB(1000, 520, 9)                 # 9 DOWN->NORMAL
PL([(590, 430), (590, 650), (440, 650)], PUR); BUB(590, 540, 4)                                 # 4 NORMAL->OPEN
PL([(700, 430), (700, 905), (440, 905)], PUR); BUB(700, 700, 5)                                 # 5 NORMAL->CLOSE
PL([(440, 870), (640, 870), (640, 430)], PUR); BUB(640, 760, 15)                                # 15 CLOSE->NORMAL
PL([(380, 700), (380, 840)], PUR); BUB(380, 770, 13)                                            # 13 OPEN->CLOSE
PL([(270, 840), (270, 700)], PUR); BUB(270, 770, 14)                                            # 14 CLOSE->OPEN
PL([(560, 360), (290, 360)], ORA); BUB(425, 360, 16)                                            # 16 NORMAL->FALL
PL([(290, 405), (560, 405)], ORA); BUB(425, 405, 17)                                            # 17 FALL->NORMAL
PL([(1250, 620), (1250, 790)], RED); BUB(1250, 705, 10)                                         # 10 DOWN->STUCK
PL([(1130, 870), (1000, 870)], RED); BUB(1065, 870, 11)                                           # 11 STUCK->WAIT
PL([(780, 840), (780, 430)], NAV); BUB(780, 640, 12)                                            # 12 WAIT->NORMAL

# stub verso altri stati (UP e DOWN)
for yy, lab, col in ((140, '6 → OPEN_DOOR', PUR), (165, '7 → CLOSE_DOOR', PUR), (190, '8 → WAIT', RED)):
    PL([(1370, yy), (1400, yy)], col, 1.8)
    T(1404, yy + 4, lab, 10, fill=col, weight='bold')
for yy, lab, col in ((540, '6 → OPEN_DOOR', PUR), (565, '8 → WAIT', RED)):
    PL([(1370, yy), (1400, yy)], col, 1.8)
    T(1404, yy + 4, lab, 10, fill=col, weight='bold')

# riquadro «abilitazione»
R(1130, 250, 240, 120, '#fffdf3', '#b48a00', 1.2, 8)
T(1140, 268, 'enable (abilitation_check)', 11.5, 'start', 'bold', '#7a5a00')
ML(1140, 284, ['s8 ∧ ¬fault VFD ∧ ¬fault pmode', 'e, fuori bypass: ¬sovratensione ∧', 'UDEC.C/CX online ∧ ¬TRIP_SAFETY',
               'il VFD offline non toglie enable'], 10, 13)
TAG(1316, 250, 'FW-04')

# legenda stati
T(50, 1000, 'Colori: azzurro = fermo · verde = marcia · viola = ciclo porta · rosso = arresto/ostacolo · arancio = collaudo.  '
            'Il numero nel cerchio rimanda alla tabella delle condizioni (riquadro 7).', 10.5, fill='#555')

# =================================================================== PANNELLO 2: porta
PANEL(1540, 70, 840, 450, '2 · Ciclo porta e serratura (door.c)', PUR)
S2W, S2H = 210, 72
DL = (1565, 120)
DU = (1865, 120)
DO = (2145, 120)
DK = (1565, 300)
DC = (1865, 300)
DS = (2145, 300)
STATE(*DL, S2W, S2H, 'LOCKED', ['chiusa e bloccata (ELT = 0)', 'se si muove → door_reset'], '#f4f4fb', PUR)
STATE(*DU, S2W, S2H, 'UNLOCKING', ['ELT = 1 (sblocco), lampada riapri', 'serratura libera → OPENING'], '#f4f4fb', PUR)
STATE(*DO, S2W, S2H, 'OPENING', ['operatori apri, a tempo 10 s', 'nessun fine corsa'], '#f4f4fb', PUR)
STATE(*DK, S2W, S2H, 'LOCKING', ['ELT = 0 (blocco), operatori chiudi', 'chiusa ∧ bloccata → LOCKED'], '#f4f4fb', PUR, tags=('FW-07',))
STATE(*DC, S2W, S2H, 'CLOSING', ['ELT = 1, operatori chiudi', 'accostata 0,5 s → LOCKING'], '#f4f4fb', PUR, tags=('FW-07',))
STATE(*DS, S2W, S2H, 'STOPPED', ['porta ferma non chiusa, ELT = 1', 'comando CLOSE → CLOSING'], '#f4f4fb', PUR)
PL([(1775, 150), (1865, 150)], PUR); T(1783, 143, 'OPEN', 10, fill=PUR)
PL([(2075, 150), (2145, 150)], PUR); T(2084, 143, 'libera', 10, fill=PUR)
PL([(2250, 192), (2250, 300)], PUR); T(2258, 250, '10 s', 10, fill=PUR)
PL([(2145, 340), (2075, 340)], PUR); T(2086, 333, 'CLOSE', 10, fill=PUR)
PL([(1865, 340), (1775, 340)], PUR); T(1790, 333, 'accost.', 10, fill=PUR)
PL([(1670, 300), (1670, 192)], PUR); T(1678, 250, 'chiusa+bloccata', 10, fill=PUR)
PL([(1990, 192), (1990, 300)], PUR, 1.5, '4 3'); T(1998, 250, 'CLOSE (da UNLOCKING)', 9.5, fill=PUR)
ML(1565, 396, ['Altre transizioni: STOPPED + OPEN (porta chiusa o bloccata) → UNLOCKING · CLOSING + OPEN → OPENING (retromarcia)',
               'LOCKING + OPEN → UNLOCKING · OPENING + CLOSE → CLOSING · LOCKED + OPEN → UNLOCKING.',
               'Fuori da qualsiasi zona di sblocco: door_reset (ma ELT resta alimentata se porta non chiusa: FW-11).',
               'Timeout LOCKING/UNLOCKING 5 s → STOPPED, TRIP_DOOR commentato; CLOSING non ha timeout: FW-07.',
               'Comando dall’alto: OPEN_DOOR/CLOSE_DOOR/RESET (run_mode). Richiesta riapertura forzata: open_door_request.',
               'In bypass (spina X8) door_is_closed/locked = vero e il ciclo porta non viene comandato.'], 10.5, 15)
TAG(2300, 426, 'FW-11')

# =================================================================== PANNELLO 3: alimentazione
PANEL(20, 1040, 880, 560, '3 · Alimentazione, marcia e freno (pmode.c, vfd.c)', ORA)
PW, PH = 190, 82
PCH = (45, 1085)
PTD = (330, 1085)
PDR = (610, 1085)
PWC = (610, 1230)
PTC = (45, 1230)
STATE(*PCH, PW, PH, 'ST_CHARGER', ['K3/K4 diseccitati, NPB attivo', 'ricarica (§5)'], '#fff8f0', ORA)
STATE(*PTD, PW, PH, 'ST_TO_DRIVE', ['K3/K4 eccitati', 'attende VFD pronto (≤ 5 s)'], '#fff8f0', ORA, tags=('FW-08',))
STATE(*PDR, 235, PH, 'ST_DRIVE', ['VFD pronto (DC link > 38 V, OTR ok)', 'marcia consentita'], '#fff8f0', ORA)
STATE(*PWC, 235, PH, 'ST_WAIT_TO_CHARGER', ['K3/K4 ancora eccitati: 30 s', 'dopo lo stop'], '#fff8f0', ORA, tags=('FW-14',))
STATE(*PTC, PW, PH, 'ST_TO_CHARGER', ['K3/K4 diseccitati', 'attende VFD fermo (≤ 5 s)'], '#fff8f0', ORA)
PL([(235, 1126), (330, 1126)], ORA); T(244, 1119, 'marcia', 10, fill=ORA)
PL([(520, 1126), (610, 1126)], ORA); T(525, 1119, 'VFD ok', 10, fill=ORA)
PL([(727, 1167), (727, 1230)], ORA); T(735, 1203, 'fine marcia', 10, fill=ORA)
PL([(610, 1290), (235, 1290)], ORA); T(420, 1283, '30 s o trip', 10, fill=ORA)
PL([(140, 1230), (140, 1167)], ORA); T(148, 1203, 'VFD fermo', 10, fill=ORA)
PL([(845, 1167), (880, 1167), (880, 1330), (140, 1330), (140, 1312)], RED, 1.6, '5 3')
T(420, 1346, 'TRIP_SAFETY → CHARGER_NOW (immediato)', 10, fill=RED)
ML(45, 1368, ['pmode_relay_set(s): POWER-EN PD10, OTR-EN PE1, NPB-DIS PE0, BRK-EN PC9 = s; forzato a 0 con TRIP_SAFETY.',
              'Il NPB è staccato in marcia: il VFD va solo a batteria.'], 10, 14)

T(45, 1415, 'Sequenza di una corsa (UP o DOWN)', 12, 'start', 'bold', ORA)
steps = ['chiamata\nrilasciata=stop', 'pmode → DRIVE\n(≤ 5 s)', 'enable ↑ e\ntraptray in pos.', 'jog SU 11 rpm\n(brake_release)',
         'brk_fb attivo\n100 ms', 'velocità lavoro\n0,10 / 0,05 m/s', 'vfd_stop 1 s\n+ WAIT 30 s']
sx = 45
for i, s in enumerate(steps):
    if i == 0:
        s = 'richiesta\ndi corsa'
    R(sx, 1428, 112, 54, '#fff8f0', ORA, 1.2, 6)
    a, b = s.split('\n')
    T(sx + 56, 1450, a, 10, 'middle')
    T(sx + 56, 1464, b, 10, 'middle')
    if i < len(steps) - 1:
        PL([(sx + 112, 1455), (sx + 122, 1455)], ORA, 1.6)
    sx += 122
TAG(430, 1424, 'FW-02')
TAG(805, 1424, 'FW-14')
T(45, 1506, 'Relè di uscita S22 e comandi VFD', 12, 'start', 'bold', ORA)
TAG(300, 1506, 'HW-07')
TAG(352, 1506, 'FW-04')
ML(45, 1524, ['S22 (OUT3_1): OPEN → CLOSING → CLOSED → OPENING, ~100 ms; si apre dopo ≥ 10 s da stop/fault;',
              'apertura immediata per sovratensione (latch fino al riavvio). Non fa parte della catena di sicurezza.',
              'VFD: RPDO1 (velocità, direzione, rampe), TPDO1-3 (stato, fault, posizione), heartbeat NMT 100 ms su entrambi i bus;',
              'timeout arresto: se non si ferma entro rampa+0,2 s (≥ 0,5 s) ripete il comando, poi CHARGER_NOW + reboot VFD.'], 10, 14)

# =================================================================== PANNELLO 5: batteria
PANEL(920, 1040, 740, 560, '4 · Carica batteria 48 V (battery.c, npb.c) — solo in ST_CHARGER, tick 1 Hz', GRN)
BWD = (950, 1085)
BFL = (1300, 1085)
BBO = (1300, 1250)
STATE(*BWD, 230, 92, 'WAIT_DISCH', ['NPB a potenza minima (42 V, 1,5 A)', 'la batteria assorbe la rigenerazione'], '#eef9f1', GRN)
STATE(*BFL, 230, 92, 'FLOAT (54,0 V)', ['regolazione corrente 3,0 A (±0,1)', 'rabbocco: fine a contatore ≥ 0'], '#eef9f1', GRN)
STATE(*BBO, 230, 92, 'BOOST (58,0 V)', ['fine: I ≤ 1,2 A per 10 s', 'oppure timeout 10 h = «carica»'], '#eef9f1', GRN, tags=('FW-17',))
PL([(1180, 1110), (1300, 1110)], GRN); BUB(1240, 1110, 'a', GRN)
PL([(1300, 1150), (1180, 1150)], GRN); BUB(1240, 1150, 'd', GRN)
PL([(1100, 1177), (1100, 1280), (1300, 1280)], GRN); BUB(1100, 1230, 'b', GRN)
PL([(1300, 1320), (1010, 1320), (1010, 1177)], GRN); BUB(1150, 1320, 'c', GRN)
ML(950, 1368, ['a: Q < −0,72 Ah ∨ Vdc < 48 V → FLOAT (BOOST se già 10 cicli float)       b: Q < −2,0 Ah ∨ low_battery ∨ 10 float → BOOST',
               'c: BOOST completo → contatore carica pieno, cicli float = 0, → WAIT_DISCH      d: FLOAT con Q ≥ 0 → cicli float + 1 → WAIT_DISCH'], 10, 15)
T(950, 1420, 'Soglie e protezioni', 12, 'start', 'bold', GRN)
TAG(1112, 1420, 'FW-16')
TAG(1164, 1420, 'FW-15')
ML(950, 1440, ['In marcia: NPB al minimo; se Vdc ≤ 38,4 V → low_battery e contatore carica azzerato a −7,5 Ah',
               '(senza filtro, scrittura LTC2959 a ogni scansione). low_battery si cancella con I ≤ 2,8 A in boost.',
               'Con low_battery al piano: fuori servizio, solo riapertura porta (run_mode_normal).',
               'Sovratensione: Vdc > 63 V in ST_DRIVE → apertura S22 + reboot VFD + TRIP_OVERVOLTAGE,',
               'latch fino al riavvio; la soglia è vicina alla tensione di boost 58 V.',
               'Contatore Ah: LTC2959 alimentato dalla batteria, sopravvive ai reset (centro scala 0x80000000).',
               'Carica: NPB 450 (Meanwell) su CAN250, polling 200 ms, comandi V/I scritti solo se cambiano.'], 10, 15)

# =================================================================== PANNELLO 4: sicurezze
PANEL(1680, 1040, 700, 560, '5 · Sicurezze, trip e reset (application.c, trips.c)', RED)
fl = [
    ('Ingressi catena S1…S5 · UDEC.C s31…s36 · UDEC.CX s3a/s3c/s4 · feedback relè', '#fdf3f2'),
    ('check_safety() — dopo 5 s dall’avvio e fuori bypass (spina X8)', '#fdf3f2'),
    ('trip memorizzati: S1 fusibile · S2 extracorsa · S3 cinghie · S31 paracadute ·', '#fdf3f2'),
    ('CHAIN_CABLE · TTF_CX_CABLE · UZONE · OTR/TRAY/TRAP_FB · TRAPTRAY  ⇒  TRIP_SAFETY', '#fdf3f2'),
    ('pmode_request(CHARGER_NOW) · relè K3/K4 a 0 · run_mode.enable = 0', '#fdf3f2'),
    ('macchine a stati: halt → WAIT_FOR_CALL_RELEASE (rilascio tasti)', '#fdf3f2'),
]
fy = 1085
for i, (s, f) in enumerate(fl):
    if i in (2, 3):
        continue
    pass
R(1700, 1080, 660, 50, '#fdf3f2', RED, 1.4, 8)
T(1712, 1100, 'Ingressi: catena S1…S5 · UDEC.C s31…s36 · UDEC.CX s3a/s3c/s4 · feedback OTR, K1/K2', 11)
T(1712, 1116, 'frame di beat e dati dalle schede (online / in_good)', 11)
PL([(2030, 1130), (2030, 1150)], RED)
R(1700, 1150, 660, 36, '#fdf3f2', RED, 1.4, 8)
T(1712, 1172, 'check_safety(): dopo 5 s dall’avvio, fuori bypass (spina X8)', 11)
TAG(2205, 1168, 'FW-05')
PL([(2030, 1186), (2030, 1206)], RED)
R(1700, 1206, 660, 74, '#fdf3f2', RED, 1.4, 8)
ML(1712, 1224, ['trip memorizzati fino al reboot: S1 fusibile · S2 extracorsa · S3 cinghie · S31 paracadute ·',
                'CHAIN_CABLE · TTF_CX_CABLE · UZONE · OTR_FB · TRAY_FB · TRAP_FB · TRAPTRAY  ⇒  TRIP_SAFETY',
                'non bloccanti (si ripristinano): S32 stop cabina · UDEC.C/CX offline · NPB'], 10.5, 15)
PL([(2030, 1280), (2030, 1300)], RED)
R(1700, 1300, 660, 54, '#fdf3f2', RED, 1.4, 8)
ML(1712, 1318, ['pmode_request(CHARGER_NOW) · pmode_relay_set(0) → K3/K4 a 0 · run_mode.enable = 0',
                'macchina a stati: halt → WAIT_FOR_CALL_RELEASE (richiede rilascio dei tasti)'], 10.5, 15)
PL([(2030, 1354), (2030, 1374)], RED)
R(1700, 1374, 660, 70, '#fff', NAV, 1.4, 8)
ML(1712, 1392, ['Reset: UP+DN al quadro (3 s) o pc0+pc1 in cabina (3 s) → vfd_reset → +100 ms → NVIC_SystemReset',
                'IWDG ≈ 0,5 s (rifornito a ogni giro di main_loop). Con la catena aperta K3/K4 cadono in hardware',
                'indipendentemente dal firmware.'], 10.5, 15)
T(1700, 1470, 'Eccezioni e rilievi', 12, 'start', 'bold', RED)
TAG(1830, 1470, 'FW-13')
TAG(1882, 1470, 'FW-04')
TAG(1934, 1470, 'FW-05')
TAG(1986, 1470, 'HW-01')
ML(1700, 1490, ['• Il manuale IM.TEC.160 2.03.15 prevede il ripristino automatico dopo il corto sulla catena (FW-13).',
                '• Il VFD offline non è un trip né toglie enable (FW-04).',
                '• Controllo cablaggio di tetto inattivo per timer condiviso (FW-05).',
                '• Spina X8: nessun controllo, 0,05 m/s, a uomo presente (HW-01).'], 10.5, 15)

# =================================================================== PANNELLO 6: modi
PANEL(1540, 540, 840, 490, '6 · Modi di funzionamento', NAV)
modes = [
    ('Normale', GRN, (), ['Comandi a uomo presente da cabina, piani, radiocomando.', 'Arresto automatico al piano (zona di sblocco).',
                      'Velocità 0,10 m/s, 0,05 m/s con magnete di piano.', 'Controlli di sicurezza attivi (check_safety).']),
    ('Emergenza / spina X8', ORA, ('FW-09', 'HW-01'), ['Spina nel quadro: esclude catena tranne e-stop quadro.', 'Velocità 0,05 m/s, solo pulsanti del pannello (IM.158);',
                                   'nel firmware anche UDEC.C e radiocomando (FW-09).', 'Porte non comandate; paracadute ripristinabile.']),
    ('Collaudo paracadute', RED, ('FW-01',), ['Menu del quadro, a cabina al piano alto, a pieno carico.', 'Apre il freno a mano (tasto 2 / IN4), prova di caduta.',
                                  'Poi: sicurezze aperte, ripristino con spina X8 (SU).', 'Oggi parte da qualsiasi voce di menu (FW-01).']),
    ('Manutenzione dal quadro', GRY, ('FW-12',), ['Prevista da IM.158/160 (solo al piano basso,', 'cicalino se fuori piano). Non implementata:', 'voce MANUALE del menu senza funzione (FW-12).']),
    ('Batteria scarica / rete assente', BLU, (), ['Rete assente: va a batteria (10 corse di emergenza).', 'Batteria scarica: ferma al piano, solo riapertura porta.',
                                              'Ripresa a carica minima (I ≤ 2,8 A in boost).']),
]
my = 584
for name, col, tgs, lines in modes:
    h = 24 + 15 * len(lines)
    R(1560, my - 18, 800, h, '#fff', col, 1.4, 8)
    T(1572, my, name, 12.5, 'start', 'bold', col)
    tx = 1572 + 8.2 * len(name) + 14
    for tg in tgs:
        tx += TAG(tx, my, tg) + 4
    ML(1572, my + 16, lines, 10.5, 15)
    my += h + 10

# =================================================================== RIQUADRO 7: tabella condizioni
R(20, 1620, 1500, 420, '#fff', NAV, 1.4, 12)
T(34, 1642, '7 · Condizioni delle transizioni del riquadro 1', 14, 'start', 'bold', NAV)
cond = [
    ('1', 'fine sequenza di inizializzazione → NORMAL'),
    ('2', 'chiamata P1 ∧ porta chiusa e bloccata ∧ trap ok → UP'),
    ('3', 'chiamata P0 ∧ porta chiusa e bloccata ∧ tray ok → DOWN'),
    ('4', 'al piano (zona di sblocco) ∧ ¬bypass ∧ (chiamata dello stesso piano ∨ tasto riapri ∨ batteria scarica con tasto) → OPEN_DOOR'),
    ('5', 'chiamata ∧ porta non chiusa/bloccata (o al piano con porta aperta, senza PAP) → CLOSE_DOOR'),
    ('6', 'UP/DOWN: arrivo alla zona di sblocco del piano → vfd_stop (1 s) → OPEN_DOOR; l’apertura può cominciare prima dello stop (FW-03)'),
    ('7', 'UP: porta non chiusa o non bloccata → CLOSE_DOOR'),
    ('8', 'UP/DOWN: trap premuto o enable perso → vfd_halt → WAIT_FOR_CALL_RELEASE'),
    ('9', 'UP/DOWN: chiamata rilasciata → vfd_stop → NORMAL'),
    ('10', 'DOWN: tray non ok (fondo premuto) → vfd_halt → STUCK_UP'),
    ('11', 'STUCK_UP: traptray in posizione ∧ trap ok → WAIT_FOR_CALL_RELEASE'),
    ('12', 'WAIT_FOR_CALL_RELEASE: nessuna chiamata → NORMAL'),
    ('13', 'OPEN_DOOR: parcheggio 20 s con porta aperta, o chiamata per l’altro piano ∧ ¬sovraccarico → CLOSE_DOOR'),
    ('14', 'CLOSE_DOOR: sovraccarico, tasto riapri/PAP, chiamata al piano corrente → OPEN_DOOR'),
    ('15', 'CLOSE_DOOR: porta chiusa e bloccata, o S3 assente (reset porta) → NORMAL'),
    ('16', 'NORMAL: OK nel menu del quadro → FALL_TEST (indipendentemente dalla voce scelta: FW-01)'),
    ('17', 'FALL_TEST: 20 s senza tasti né chiamate → NORMAL'),
]
cy = 1664
for n, s in cond:
    BUB(46, cy - 4, n)
    T(64, cy, s, 10.5)
    cy += 20.5

# =================================================================== RIQUADRO 8: comunicazioni e tempi
PANEL(1540, 1620, 840, 420, '8 · Comunicazioni, heartbeat e tempi', BLU)
ML(1560, 1665, [
    'CAN250 (cappello): VFD 7BL00099 (CANopen), NPB 450 (frame estesi, polling 200 ms).',
    'CAN125 (cabina): UDEC.C, UDEC.CX, operatori porta L2D (frame estesi UDEC_CAN_ID).',
    'Heartbeat TTF → entrambi i bus ogni 100 ms; il CX stacca bypass e serratura se manca per 0,5 s.',
    'Timeout offline: UDEC.C 1,5 s · UDEC.CX 0,5 s · VFD 0,2 s · L2D 2 s.',
    'Il frame CAN_STATUS di UDEC.C è trattato come status word (FW-10).',
    'Frame estesi sconosciuti vengono ritrasmessi su CAN125; DLC non controllato (FW-23).',
    '',
    'Tempi del ciclo macchina (application.h):',
    '  parcheggio a porta aperta 20 s · manovra porta 10 s · sblocco/blocco 5 s (timer) / 1,5 s (ritardo)',
    '  accostamento→blocco 0,5 s · rilascio freno 100 ms · luce occupato +10 s',
    '  avvio: controlli di sicurezza dopo 5 s · reset da quadro/COP 3 s',
    '  commutazione carica↔marcia ≤ 5 s ciascuna · attesa prima dello spegnimento VFD 30 s',
    '  S22: contatti 200 ms · apertura dopo 10 s da stop · relè OTR 200 ms · feedback bypass 2 s',
], 10.5, 15)
TAG(1560 + 5.6 * 66 + 12, 1725, 'FW-10')
TAG(1560 + 5.6 * 90 + 12, 1740, 'FW-23')
T(1560, 1905, 'Legenda riquadri rossi', 11, 'start', 'bold')
ML(1560, 1923, ['FW = firmware · HW = hardware/architettura · NOR = normativa/rischi · DOC = documentazione ·',
                'PRV = verifica in prova. Dettagli, proposte e stato: docs/registro_issue.md.'], 10.5, 15)
T(1560, 1975, 'Incertezze: punti marcati «(?)» negli schemi; Analisi dei rischi Rev.5 non letta (NOR-01).', 10.5, fill='#7a4b00')

o.append('</svg>')
out = sys.argv[1] if len(sys.argv) > 1 else 'diagramma_funzionale.svg'
with open(out, 'w', encoding='utf-8') as f:
    f.write('\n'.join(o))
print('scritto', out)
