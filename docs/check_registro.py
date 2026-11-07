#!/usr/bin/env python3
"""Controllo di coerenza di docs/registro_issue.md.

Verifica che la tabella di riepilogo e i blocchi di dettaglio coincidano
(titolo, gravità, stato), che gli stati siano validi, che gli ID siano unici
e che gli script dei diagrammi citino solo issue esistenti e ancora aperte.

Uso:  python3 docs/check_registro.py        (esce con codice 1 se ci sono errori)
"""
import re
import sys
from collections import Counter
from pathlib import Path

DOCS = Path(__file__).resolve().parent
REG = DOCS / 'registro_issue.md'
DIAGRAMMI = [DOCS / 'gen_diagramma_funzionale.py', DOCS / 'gen_schema_funzionale.py']
STATI = {'Aperta', 'Da decidere', 'Da verificare', 'In corso', 'Risolta', 'Chiarita'}
CHIUSI = {'Risolta', 'Chiarita'}
GRAVITA = {'Alta', 'Media', 'Bassa'}
ID_RE = r'(?:FW|HW|NOR|DOC|PRV|CHI)-\d{2}'

errori, avvisi = [], []
testo = REG.read_text(encoding='utf-8')

# --- tabella
m = re.search(r'<!-- RIEPILOGO-INIZIO -->(.*?)<!-- RIEPILOGO-FINE -->', testo, re.S)
if not m:
    sys.exit('Marcatori RIEPILOGO-INIZIO/FINE non trovati')
tabella = {}
for riga in m.group(1).splitlines():
    c = [x.strip() for x in riga.strip().strip('|').split('|')]
    if len(c) == 5 and re.fullmatch(ID_RE, c[0]):
        if c[0] in tabella:
            errori.append(f'{c[0]}: duplicato nella tabella')
        tabella[c[0]] = dict(titolo=c[1], gravita=c[2], stato=c[3], area=c[4])

# --- blocchi di dettaglio
dettagli = {}
righe = testo.splitlines()
for i, riga in enumerate(righe):
    h = re.match(rf'^### ({ID_RE}) · (.+)$', riga)
    if not h:
        continue
    idn, titolo = h.group(1), h.group(2).strip()
    if idn in dettagli:
        errori.append(f'{idn}: duplicato nei dettagli')
    g = s = None
    for j in range(i + 1, min(i + 4, len(righe))):
        mm = re.match(r'\*\*Gravità:\*\* (\w+) · \*\*Stato:\*\* ([^·]+?)(?: · |$)', righe[j])
        if mm:
            g, s = mm.group(1), mm.group(2).strip()
            break
    if g is None:
        errori.append(f'{idn}: riga «Gravità/Stato» mancante')
    dettagli[idn] = dict(titolo=titolo, gravita=g, stato=s)

for idn, t in tabella.items():
    d = dettagli.get(idn)
    if d is None:
        errori.append(f'{idn}: nella tabella ma senza blocco di dettaglio')
        continue
    for campo in ('titolo', 'gravita', 'stato'):
        if t[campo] != d[campo]:
            errori.append(f'{idn}: {campo} diverso (tabella «{t[campo]}», dettaglio «{d[campo]}»)')
    if t['stato'] not in STATI:
        errori.append(f'{idn}: stato non valido «{t["stato"]}»')
    if t['gravita'] not in GRAVITA:
        errori.append(f'{idn}: gravità non valida «{t["gravita"]}»')
for idn in dettagli:
    if idn not in tabella:
        errori.append(f'{idn}: ha un blocco di dettaglio ma non è nella tabella')

# --- riferimenti nei diagrammi
for f in DIAGRAMMI:
    if not f.exists():
        continue
    usati = set(re.findall(ID_RE, f.read_text(encoding='utf-8')))
    for idn in sorted(usati):
        if idn not in tabella:
            errori.append(f'{f.name}: cita {idn}, che non esiste nel registro')
        elif tabella[idn]['stato'] in CHIUSI and not idn.startswith('CHI'):
            avvisi.append(f'{f.name}: cita {idn}, ora «{tabella[idn]["stato"]}»: togliere il riquadro?')

# --- riferimenti incrociati nel testo del registro
for idn in sorted(set(re.findall(ID_RE, testo))):
    if idn not in tabella:
        errori.append(f'registro: cita {idn}, che non esiste')

print(f'Issue: {len(tabella)}')
for chiave in ('stato', 'gravita', 'area'):
    conta = Counter(v[chiave] for v in tabella.values())
    print(f'  per {chiave}: ' + ', '.join(f'{k} {n}' for k, n in sorted(conta.items())))
aperte = [k for k, v in tabella.items() if v['stato'] not in CHIUSI]
print(f'Non chiuse: {len(aperte)}  ·  Chiuse: {len(tabella) - len(aperte)}')
for a in avvisi:
    print('AVVISO:', a)
for e in errori:
    print('ERRORE:', e)
sys.exit(1 if errori else 0)
