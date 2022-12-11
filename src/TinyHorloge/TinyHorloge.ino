/* PCB Horloge
 * Daan Dekoning Krekels
 * 2022-2023 AP Hogeschool
 * 
 * Gebruik bij het programmeren TinyCore en jtag2updi.
 * - https://github.com/xukangmin/TinyCore
 * - https://github.com/ElTangas/jtag2updi
*/

#include <PCF85063A.h>
PCF85063A rtc;

#include <TinyTouch.h>
TinyTouch touch;
uint8_t touchPins[2] = { 4, 8 };  //initialize touch pins PIN_PB4, PIN_PC2

// Variabele voor uur en minuten op voorhand gedefinieerd
int hour, minute;

// Pinnen gedefinieerd als bitmasks
#define X1 PIN1_bm  // 10 // PIN_PA1
#define X2 PIN2_bm  // 11 // PIN_PA2
#define X3 PIN4_bm  // 13 // PIN_PA4
#define X4 PIN5_bm  // 14 // PIN_PA5
#define X5 PIN6_bm  // 15 // PIN_PA6
#define X6 PIN7_bm  // 16 // PIN_PA7

const byte AANTAL_LED_PINS = 6;
const byte LED_PINS[] = { X1, X2, X3, X4, X5, X6 };

// Lookup table om te weten welke minuten naar welke LED moeten.
// Afronding zonder berekeningen :)
// ARRAY[x][0] moet LOW zijn
// ARRAY[x][1] moet HIGH zijn
const byte MINUTEN[61][2] = {
  // 0    1
  { X6, X2 },  // 12 Min
  { X6, X2 },  // 12 Min
  { X6, X2 },  // 12 Min
  { X4, X1 },  // 1  Min
  { X4, X1 },  // 1  Min
  { X4, X1 },  // 1  Min
  { X4, X1 },  // 1  Min
  { X4, X1 },  // 1  Min
  { X4, X2 },  // 2  Min
  { X4, X2 },  // 2  Min
  { X4, X2 },  // 2  Min
  { X4, X2 },  // 2  Min
  { X4, X2 },  // 2  Min
  { X4, X3 },  // 3  Min
  { X4, X3 },  // 3  Min
  { X4, X3 },  // 3  Min
  { X4, X3 },  // 3  Min
  { X4, X3 },  // 3  Min
  { X4, X5 },  // 4  Min
  { X4, X5 },  // 4  Min
  { X4, X5 },  // 4  Min
  { X4, X5 },  // 4  Min
  { X4, X5 },  // 4  Min
  { X4, X6 },  // 5  Min
  { X4, X6 },  // 5  Min
  { X4, X6 },  // 5  Min
  { X4, X6 },  // 5  Min
  { X4, X6 },  // 5  Min
  { X5, X1 },  // 6  Min
  { X5, X1 },  // 6  Min
  { X5, X1 },  // 6  Min
  { X5, X1 },  // 6  Min
  { X5, X1 },  // 6  Min
  { X5, X2 },  // 7  Min
  { X5, X2 },  // 7  Min
  { X5, X2 },  // 7  Min
  { X5, X2 },  // 7  Min
  { X5, X2 },  // 7  Min
  { X5, X3 },  // 8  Min
  { X5, X3 },  // 8  Min
  { X5, X3 },  // 8  Min
  { X5, X3 },  // 8  Min
  { X5, X3 },  // 8  Min
  { X5, X4 },  // 9  Min
  { X5, X4 },  // 9  Min
  { X5, X4 },  // 9  Min
  { X5, X4 },  // 9  Min
  { X5, X4 },  // 9  Min
  { X5, X6 },  // 10 Min
  { X5, X6 },  // 10 Min
  { X5, X6 },  // 10 Min
  { X5, X6 },  // 10 Min
  { X5, X6 },  // 10 Min
  { X6, X1 },  // 11 Min
  { X6, X1 },  // 11 Min
  { X6, X1 },  // 11 Min
  { X6, X1 },  // 11 Min
  { X6, X1 },  // 11 Min
  { X6, X2 },  // 12 Min
  { X6, X2 },  // 12 Min
  { X6, X2 }   // 12 Min
};

// Lookup table voor alle uren, de RTC geeft deze in 24U formaat.
// ARRAY[x][0] moet LOW zijn
// ARRAY[x][1] moet HIGH zijn
const byte UREN[24][2]{
  // 0    1
  { X3, X2 },  // 12 Uur
  { X1, X2 },  // 1  Uur
  { X1, X3 },  // 2  Uur
  { X1, X4 },  // 3  Uur
  { X1, X5 },  // 4  Uur
  { X1, X6 },  // 5  Uur
  { X2, X1 },  // 6  Uur
  { X2, X3 },  // 7  Uur
  { X2, X4 },  // 8  Uur
  { X2, X5 },  // 9  Uur
  { X2, X6 },  // 10 Uur
  { X3, X1 },  // 11 Uur
  { X3, X2 },  // 12 Uur
  { X1, X2 },  // 1  Uur
  { X1, X3 },  // 2  Uur
  { X1, X4 },  // 3  Uur
  { X1, X5 },  // 4  Uur
  { X1, X6 },  // 5  Uur
  { X2, X1 },  // 6  Uur
  { X2, X3 },  // 7  Uur
  { X2, X4 },  // 8  Uur
  { X2, X5 },  // 9  Uur
  { X2, X6 },  // 10 Uur
  { X3, X1 }   // 11 Uur
};

// Overige LEDS om mee te spelen.
// ARRAY[x][0] moet LOW zijn
// ARRAY[x][1] moet HIGH zijn
const byte MISC[6][2]{
  // 0    1
  { X3, X4 },  // 1  min
  { X6, X3 },  // 2  min
  { X3, X5 },  // 3  min
  { X6, X4 },  // 4  min
  { X6, X5 },  // W
  { X3, X6 }   // W
};

// Geen delay() functie maar via millis().
// Non-blocking en eventuele interrupts blijven werken.
int period = 10000;  // 10 seconden
unsigned long time_now = 0;
bool toon_tijd = false;  // True als er op de knop wordt gedrukt.

void setup() {
  touch.begin(touchPins, sizeof(touchPins));

  //  setTime(hour, minute, sec);
  rtc.setTime(14, 17, 00);  // 24H mode, ex. 6:54:00
  //  setDate(weekday, day, month, yr);
  // rtc.setDate(6, 16, 5, 2020);  // 0 for Sunday, ex. Saturday, 16.5.2020.

  reset_LED_pins();  // Al even alle leds uitschakelen en als INPUT zetten / Hoog impedant

  // Even door alle tijden lopen
  for (byte i = 0; i < 12; i++) {
    set_hour(i);
    delay(5);
    for (byte j = 0; j < 60; j++) {
      set_minute(j);
      delay(5);
      set_hour(i);
      delay(5);
    }
  }
  for (byte i = 0; i < 6; i++) {
    set_misc(i);
    delay(100);
  }
}

void loop() {
  time_now = millis();  // Slaag actuele tijd op

  touch.touchHandle();  // Start touch controller
                        // Uitbreiding: Interrupts aan de praat krijgen.

  // Sensorwaarde vergelijken met threshold.
  // En tijd tonen of LEDS resetten
  if (touch.getValue(0) > 1000) {
    // Heel kort een lampke branden
    set_misc(0);
    delay(50);
    toon_tijd = true;  // Tijd mag dadelijk getoond worden
  } else {
    reset_LED_pins();
  }

  // Sensorwaarde vergelijken met threshold.
  // En tijd tonen of LEDS resetten
  if (touch.getValue(1) > 1000) {
    // Heel kort een lampke branden
    set_misc(3);
    delay(50);
    toon_tijd = true;  // Tijd mag dadelijk getoond worden
  } else {
    reset_LED_pins();
  }

  // Toon uren en minuten.
  if (toon_tijd) {
    // Tijd maar 1x ophalen, enkel wanneer er gedrukt wordt.
    hour = rtc.getHour();      // Uur ophalen van RTC
    minute = rtc.getMinute();  // Minuten ophalen van RTC
    // Blijven herhalen tot tijd op is
    // While loop omdat alle code die hierboven geschreven is niet van belang is wanneer de tijd wordt weergegeven
    // Als dat wel het geval is (bv. knoppen moeten nog bruikbaar zij) dan kan een if statement gebruitk worden
    //    In dat geval moet er een else toegevoegd worden waar de `time_now` wordt ingesteld en `toon_tijd` wordt uitgeschakeld
    while (millis() < time_now + period) {
      set_hour(hour);      // Uur weergeven
      delay(0.2);            // Geef CPU wat rusttijd
      set_minute(minute);  // Minuten weergeven
      delay(0.2);            // Geef CPU wat rusttijd
    }
    toon_tijd = false;  // Bij volgende loop tijd niet meer tonen
  }
  delay(1);  // Geef CPU wat rusttijd
}


/*
 * Zet alle LED pinnen als `INPUT`/Hoog Impedant
*/
void reset_LED_pins() {
  PORTA_DIR = 0b00000000;  // set all pins as an output
  // for (byte i = 0; i < AANTAL_LED_PINS; i++) {
  //   pinMode(LED_PINS[i], INPUT);  // Alle pinnen hoog impedant zetten
  // }
  // PORTA_OUT = 0b00000000;  // set all pins as an LOW
}

/*
* Zet de LED aan van het meegegeven uur.
* Gebruikt `UREN[][];`
*/
void set_hour(int hour) {
  reset_LED_pins();

  PORTA_DIR = UREN[hour][0] | UREN[hour][1];  // Pinnen als output
  PORTA_OUT = UREN[hour][1];                  // Enkel deze als HIGH

  // pinMode(UREN[hour][0], OUTPUT);
  // pinMode(UREN[hour][1], OUTPUT);
  // digitalWrite(UREN[hour][0], LOW);
  // digitalWrite(UREN[hour][1], HIGH);
}

/*
 * Zet de LED aan van de meegegeven minuut.
 * Gebruikt `MINUTEN[][];`
*/
void set_minute(int minute) {
  reset_LED_pins();

  PORTA_DIR = MINUTEN[minute][0] | MINUTEN[minute][1];  // Pinnen als output
  PORTA_OUT = MINUTEN[minute][1];                       // Enkel deze als HIGH

  //   pinMode(MINUTEN[minute][0], OUTPUT);
  //   pinMode(MINUTEN[minute][1], OUTPUT);
  //   digitalWrite(MINUTEN[minute][0], LOW);
  //   digitalWrite(MINUTEN[minute][1], HIGH);
}

/*
 * Zet de LED aan van de meegegeven overige aantal.
 * Gebruikt `MISC[][];`
*/
void set_misc(int misc) {
  reset_LED_pins();

  PORTA_DIR = MISC[misc][0] | MISC[misc][1];  // Pinnen als output
  PORTA_OUT = MISC[misc][1];                  // Enkel deze als HIGH

  // pinMode(MISC[misc][0], OUTPUT);
  // pinMode(MISC[misc][1], OUTPUT);
  // digitalWrite(MISC[misc][0], LOW);
  // digitalWrite(MISC[misc][1], HIGH);
}
