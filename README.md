# EVC_FW

Progetto hardware e firmware del sistema di controllo di un ascensore domestico a trazione (TTF) di LiftingItalia s.r.l. Quattro schede basate su STM32F446, collegate in CAN; il motore è comandato da un VFD.

## Struttura

| Cartella | Contenuto |
|---|---|
| `TTF-Base/` | Scheda master (STM32F446VC, PCB a 4 strati): schemi e PCB KiCad, Gerber/PDF in `fabs/`, firmware in `Firmware/TTF-Base/`. |
| `TTF-CX/` | Scheda di tetto cabina (STM32F446RC, PCB a 4 strati): ingressi di sicurezza S3/S4, bypass trap/tray, zone di sblocco, cella di carico, illuminazione. Firmware in `Firmware/TTF-CX/`. |
| `UDEC.CX/` | Scheda collegata alla cabina (STM32F446VC, PCB a 4 strati): audio, pulsantiera, interfaccia VFD, cella di carico. Firmware in `CubeMX/`, simulazioni in `LTSpice/`. |
| `Schema Macchina/` | Schema elettrico generale dell'impianto (cabina, cappello, servizi); PDF in `fabs/TTF.pdf`. |
| `F510.05.9019 Scheda Consenso Paracadute/` | PCB a 1 strato della scheda di consenso paracadute e modelli FreeCAD del sensore. |

## Firmware TTF-Base

Super-loop con timer software e macchina a stati, in C (STM32CubeIDE/HAL). Moduli principali in `TTF-Base/Firmware/TTF-Base/Core/`:

- `run_mode.c`: macchina a stati del movimento (`NORMAL`, `UP`, `DOWN`, `OPEN_DOOR`, `CLOSE_DOOR`, `STUCK_UP`, `FALL_TEST`, `WAIT_FOR_CALL_RELEASE`) e sequenza di abilitazione.
- `door.c`: ciclo porte (apertura, chiusura, blocco, sblocco).
- `pmode.c`, `battery.c`, `npb.c`, `ltc2959.c`: commutazione caricabatteria/VFD, gestione e misura della batteria.
- `vfd.c`: comando dell'azionamento (CANopen).
- `trips.c`, `trip_list.h`: gestione degli errori (inattivo, attivo, memorizzato).
- `udec_c.c`, `udec_cx.c`, `l2d.c`, `can.c`: comunicazione CAN con le altre schede.
- `HMI.c`, `HMI_menu.c`, `lcd_hd44780_i2c.c`: display LCD e menu.

Le modifiche recenti sono descritte in `TTF-Base/Firmware/TTF-Base/Changelog`.

## Documentazione

- `docs/analisi.md`: analisi del progetto, revisione del codice di sicurezza, catena delle sicurezze e confronto con i manuali.
- `docs/schema_funzionale.svg` (e `.png`): schema funzionale elettrico con la catena delle sicurezze. Si rigenera con `python3 docs/gen_schema_funzionale.py docs/schema_funzionale.svg`.

## Strumenti

- Schemi e PCB: KiCad.
- Firmware: STM32CubeIDE, progetti configurati con file `.ioc` (CubeMX).
- Simulazioni: LTspice (`UDEC.CX/LTSpice/`).
- Modelli meccanici: FreeCAD.

## Note

- `Drivers/` e `Middlewares/` (HAL e USB di ST) sono inclusi nel repo, così il firmware compila senza rigenerare il codice.
- Cache, file di lock, backup e output di build sono esclusi dal `.gitignore`.
- Il sistema ha funzioni di sicurezza (paracadute, catena di sicurezze, bypass, extracorsa): ogni modifica va validata secondo le norme di prodotto.
