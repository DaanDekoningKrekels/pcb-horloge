# PCB Horloge

### Snelle links

- [Opdrachtomschrijving](./oo1-opdrachtbeschrijving.pdf)
- [Onderzoeksdocument](./onderzoeksdocument.md)
- [PCB-ontwerp](./src/kicad-project/)
- [Horloge code](./src/TinyHorloge/TinyHorloge.ino)



## Project structuur

```
.
├── assets					(figuren)
├── datasheets				(datasheets gebruikte componenten)
└── src						(alle bronbestanden)
    ├── kicad-project		(PCB-ontwerp)
    ├── LED-test			(alle leds laten branden)
    ├── LED-volgorde-test	(leds op volgorde laten branden)
    ├── PTC-test			(RTC proberen aanspreken)
    ├── RTC-test			(capacitieve sensoren implementeren)
    └── TinyHorloge			(finale code, alles gecombineerd)
```

