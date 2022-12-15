# PCB Horloge

### Snelle links

- [Opdrachtomschrijving](./oo1-opdrachtbeschrijving.pdf)
- [Onderzoeksdocument](./onderzoeksdocument.md)
- [PCB-ontwerp](./src/kicad-project/)
- [Testplan bestukken printplaat](testplan.md)
- [Proces softwareontwikkeling](proces-softwareontwikkeling.md)
  - [Horloge code](./src/TinyHorloge/TinyHorloge.ino)



## Project structuur

```
.
├── assets                  (figuren)
├── datasheets              (datasheets gebruikte componenten)
└── src                     (alle bronbestanden)
    ├── kicad-project       (PCB-ontwerp)
    ├── LED-test            (alle leds laten branden)
    ├── LED-volgorde-test   (leds op volgorde laten branden)
    ├── PTC-test            (capacitieve sensoren implementeren)
    ├── RTC-test            (RTC proberen aanspreken)
    └── TinyHorloge         (finale code, alles gecombineerd)
```

