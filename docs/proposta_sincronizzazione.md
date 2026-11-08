# Proposta di sincronizzazione TortoiseGit → GitHub (periodo di test)

Data: 2026-11-08 · Stato: **proposta, nulla è stato attivato** · Valida solo per il primo periodo di test.

## 1. Obiettivo e principi

- Portare su GitHub (`dermatoid-collab/EVC_FW`) il lavoro del tecnico che usa TortoiseGit sulla rete aziendale, **solo in un senso**: da TortoiseGit a GitHub. Niente torna indietro nel repo aziendale.
- Il firmware non viene modificato dall'assistente: l'assistente importa e confronta soltanto ciò che il tecnico ha già committato.
- Ogni aggiornamento è confrontato e riepilogato prima di essere applicato al ramo di lavoro con la documentazione.

## 2. Premessa tecnica

- Il repo GitHub è stato creato importando uno zip (commit iniziale `180b04b`): la sua storia è **indipendente** da quella del repo aziendale. Un push sullo stesso ramo sarebbe rifiutato.
- Il ramo `claude/wizardly-pascal-d8clis` contiene la documentazione (`docs/`, registro, diagrammi, `CLAUDE.md`, `README.md`). Il tecnico **non deve fare push su quel ramo**.
- Gli invii del tecnico vanno su un ramo nuovo, per esempio `rete-aziendale`.

## 3. Opzioni per portare il codice su GitHub

### A. Manuale con zip dei file (consigliata per cominciare)

1. Committare tutto in TortoiseGit (l'export prende solo il commit scelto).
2. Clic destro sulla radice del repo → TortoiseGit → Export… → revisione `HEAD` (o il ramo da inviare) → salvare come ZIP, oppure esportare in una cartella e comprimerla.
3. Comunicare l'hash del commit e il nome del ramo (dal log).
4. Lo zip deve avere la struttura intera del progetto (`TTF-Base/`, `TTF-CX/`, `UDEC.CX/`, `Schema Macchina/`, `F510.05.9019 Scheda Consenso Paracadute/`) e non contenere cartelle di compilazione.

### B. Manuale con la storia dei commit (bundle)

Non zippare `.git` (può contenere indirizzi aziendali e credenziali). Creare un bundle: TortoiseGit → Create Bundle…, oppure `git bundle create ../ttf-test.bundle --all`. Il bundle conserva autori e messaggi dei commit.

### C. Automatica con push programmato

Sul PC del tecnico:

1. Aggiungere il remote `github`: Settings → Git → Remote → Add, URL `https://github.com/dermatoid-collab/EVC_FW.git`. Il remote `origin` non cambia.
2. Creare un token fine-grained su GitHub limitato al solo repo `EVC_FW`, con permesso Contents: Read and write, da usare come password e da memorizzare nel gestore di credenziali.
3. Provare un push manuale: TortoiseGit → Push → remote `github`, ramo locale di lavoro, ramo remoto `rete-aziendale`, **senza** Force.
4. Pianificare l'invio con Utilità di pianificazione di Windows, ogni giorno all'orario scelto. Programma `git.exe`, argomenti:

   `-C "C:\percorso\del\repo" push github <ramo-del-tecnico>:rete-aziendale`

Requisiti da provare sul PC del tecnico: accesso a `github.com` sulla porta 443 (eventuale proxy in TortoiseGit → Settings → Network), PC acceso all'orario scelto, token valido (controllarne la scadenza). Viene inviato solo il lavoro già committato.

**Da evitare:** `push --mirror` e qualsiasi push forzato, perché cancellerebbero il ramo con la documentazione.

## 4. Lato assistente

- **Aggiornamento su richiesta (A o B):** estrae lo zip o il bundle in una cartella separata, confronta con GitHub escludendo `docs/`, `CLAUDE.md`, `README.md` e `.gitignore`, presenta il riepilogo (file nuovi, modificati, cancellati), applica solo dopo conferma su un ramo `import-aziendale` che contiene soltanto il codice del tecnico, poi lo riversa nel ramo di lavoro.
- **Controllo programmato (con l'opzione C):** un'attività pianificata, per esempio ogni giorno alle 7:00, controlla se `rete-aziendale` è cambiato e invia per email il riepilogo. Nel periodo di test **solo riepilogo**: l'aggiornamento del ramo di lavoro e del registro avviene dopo conferma. Non è ancora stato creato.
- Il registro viene aggiornato nei punti toccati dalle modifiche (per esempio le righe di codice citate); una voce passa a «Risolta» solo con la prova, non sulla base del solo commit.

## 5. Cautele

- **Riservatezza:** il codice aziendale finisce in un repo GitHub personale; decidere chi ha accesso prima di cominciare.
- **Token:** limitarlo al solo repo e annotarne la scadenza.
- **Fine righe:** se i file risultano tutti modificati solo per CRLF/LF, vengono normalizzati e segnalati.
- **Cancellazioni:** i file mancanti nell'invio sono elencati prima di essere tolti.
- **Durata:** procedura temporanea di test; alla fine si sceglie se riunire le due storie (il tecnico porta nel suo repo `docs/`, `CLAUDE.md`, `README.md`, `.gitignore`) o mantenere il flusso a senso unico.

## 6. Decisioni da prendere

1. Opzione di partenza: A (zip dei file), B (bundle) o C (push automatico).
2. Nome del ramo su GitHub per il codice aziendale (proposto: `rete-aziendale`).
3. Se attivare il controllo programmato, con quale frequenza, e se solo riepilogo.
4. Chi può vedere il repo GitHub, dato che contiene codice aziendale.
