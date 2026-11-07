# Regole di lavoro per questo repository

Progetto Elevance / TTF (LiftingItalia): hardware KiCad e firmware STM32 (TTF-Base, TTF-CX, UDEC.CX). Lingua di lavoro: italiano.

## Documento base

`docs/registro_issue.md` è il documento base del lavoro: contiene tutte le osservazioni, le proposte di correzione, le verifiche e lo stato di ciascuna.

- **Ogni osservazione o proposta** nuova (da lettura di codice, schemi, manuali, prove o conversazione) va registrata lì con un ID nuovo, sia nella tabella sia nel blocco di dettaglio. Mai riutilizzare un ID.
- **Quando un'issue cambia stato** si aggiornano riga della tabella, blocco di dettaglio (campo Stato e riga Storia, con data e commit) e la cronologia in fondo.
- **Diagrammi**: i riquadri rossi in `docs/diagrammi/02.diagramma_funzionale.svg` rimandano agli ID. Quando un'issue è risolta o chiarita si toglie il riquadro dallo script (`docs/gen_diagramma_funzionale.py`), si rigenerano SVG e PNG e si aggiorna il testo del diagramma se il comportamento è cambiato. Lo stesso per `docs/diagrammi/01.schema_funzionale.svg` (`docs/gen_schema_funzionale.py`). La versione commerciale `docs/diagrammi/03.diagramma_funzionale_light.svg` (`docs/gen_diagramma_light.py`) non porta riquadri ma va riallineata alle funzioni correnti (DOC-10): niente nomi di codice né ID di issue, solo ciò che è verificato; il resto va in «In sviluppo».
- **Prima di ogni commit** che tocca registro o diagrammi: `python3 docs/check_registro.py` (deve uscire senza errori).
- Le analisi narrative (`docs/analisi.md`) non si aggiornano issue per issue.

## Modifiche al firmware

Non modificare il firmware senza richiesta esplicita dell'utente. Procedura: proposta nel registro → approvazione → modifica → verifica → stato aggiornato (Risolta o In corso, con commit e prova).

## Convenzioni

- Riferimenti a codice come `file:riga` relativi a `TTF-Base/Firmware/TTF-Base/Core/` salvo diversa indicazione.
- Distinguere sempre ciò che è verificato dal codice o dai documenti da ciò che è dedotto o da verificare.
- Le fonti esterne (manuali e note su Notion, analisi dei rischi sul server aziendale) vanno citate per titolo o codice documento.
