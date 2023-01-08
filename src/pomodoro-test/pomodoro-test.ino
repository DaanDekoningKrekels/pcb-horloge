/* PCB Horloge
 * Daan Dekoning Krekels
 * 2022-2023 AP Hogeschool
 * 
 * Gebruik bij het programmeren TinyCore en jtag2updi.
 * - https://github.com/xukangmin/TinyCore
 * - https://github.com/ElTangas/jtag2updi
 * Bibliotheken:
 * - RTC PCF85063A https://github.com/SolderedElectronics/PCF85063A-Arduino-Library 
*/

#include <PCF85063A.h>
PCF85063A rtc;

#include <TinyTouch.h>
TinyTouch touch;
const int RTC_INT_PIN = 6;        // Interrupt pin voor RTC PC0
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
int period = 10000;           // 10 seconden
unsigned long time_now = 0;   // Gebruikt om te controlleren hoe lang het uur al zichtbaar is
bool toon_tijd = false;       // True als er op de knop wordt gedrukt.
bool tijd_opgehaald = false;  // True als de tijd al van de RTC is opgevraagd

/* POMODERO */
bool pomodoro_setup = false;         // True als de pomodero timer is ingesteld via RTC
bool pomodoro_active = false;        // True als pomodero in gebruik is, tijd kan niet meer worden weergegeven
bool pomodero_interrupt_fl = false;  // True wanneer de RTC een interrupt liet afgaan
byte pomodoro_value = 0;             // Hoe lang de initiele timer tijd is
byte pomodoro_new_value = 0;         // Hoe lang de nieuwe timer tijd is
#define POMODERO_INTERVAL 5          // Per hoeveel seconden er kan gekozen worden

/* DEBOUNCE */
#define SHORT_PRESS 50           // aantal ms om een korte druk te registreren
bool pressed_0 = false;          // True als er gedrukt is op linkse knop
unsigned long press_time_0 = 0;  // Aantal millis() op het moment dat er links gedrukt is
bool pressed_1 = false;          // True als er gedrukt is op rechtse knop
unsigned long press_time_1 = 0;  // Aantal millis() op het moment dat er rechts gedrukt is



void setup() {
  touch.begin(touchPins, sizeof(touchPins));

  //  setTime(hour, minute, sec);
  rtc.setTime(10, 54, 00);  // 24H mode, ex. 6:54:00
  //  setDate(weekday, day, month, yr);
  // rtc.setDate(6, 16, 5, 2020);  // 0 for Sunday, ex. Saturday, 16.5.2020.
  pinMode(RTC_INT_PIN, INPUT_PULLUP);  // Interrupt pin voor RTC met pullup

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

ISR(RTC_CNT_vect) {

  /* Insert your RTC Compare interrupt handling code */
  TinyTouch::touch_timer_handler();

  /* Compare interrupt flag has to be cleared manually */
  RTC.INTFLAGS = RTC_CMP_bm;
}


void RTC_INTERRUPT() {
  detachInterrupt(RTC_INT_PIN);  // disables interrupt op RTC interrupt pin
  pomodero_interrupt_fl = true;  // Vlag voor de rest van de code
}

void loop() {
  if (!toon_tijd) {
    time_now = millis();  // Slaag actuele tijd op
  }
  touch.touchHandle();  // Start touch controller
                        // Uitbreiding: Interrupts aan de praat krijgen.

  // Sensorwaarde vergelijken met threshold.
  if (touch.getValue(0) > 1000) {
    press_time_0 = millis();  // Waarden instellen voor debounce
    pressed_0 = true;         // Er is gedrukt
  }

  // Pomodero, en tijd tonen of LEDS resetten
  if (millis() - press_time_0 > SHORT_PRESS && pressed_0) {  // Ontdenderen
    // Heel kort een lampke branden
    set_misc(0);
    delay(0.5);

    // Enkel als we in pomodoro modus zijn
    // En pomodoro nog niet op 0 staat
    // Anders gaan we in pomodero modus
    if (pomodoro_active && pomodoro_new_value != 0) {
      pomodoro_new_value -= POMODERO_INTERVAL;  // Timer naar beneden doen
    } else {
      pomodoro_active = true;                  // Pomodero ingeschakeled
      pomodoro_new_value = POMODERO_INTERVAL;  // Direct al een timer verschillend van 0

      toon_tijd = false;  // We willen de tijd ook niet meer tonen als deze al actief was
    }
    pressed_0 = false;  // Debounce terug resetten
  } else {
    reset_LED_pins();
  }


  // Sensorwaarde vergelijken met threshold.
  if (touch.getValue(1) > 1000) {
    press_time_1 = millis();  // Waarde instellen voor debounce
    pressed_1 = true;         // Er is gedrukt
  }

  // Pomodero, en tijd tonen of LEDS resetten
  if (millis() - press_time_1 > SHORT_PRESS && pressed_1) {  // Ontdeneren
    // Heel kort een lampke branden
    set_misc(3);
    delay(0.5);

    // Enkel als we in pomodero modus zijn
    // en als we de variabele niet gaan overflowen
    // Anders tijd weergeven
    if (pomodoro_active && pomodoro_new_value != 254) {
      pomodoro_new_value += POMODERO_INTERVAL;  // Interval verhogen
    } else {
      toon_tijd = true;  // Tijd mag dadelijk getoond worden
    }
    pressed_1 = false;  // Debounce resetten
  } else {
    reset_LED_pins();
  }

  /* POMODERO MODUS
   * - Eerst controle of de interrupt is afgegaan
   * - Controle of er een nieuwe tijd is ingesteld en setup RTC timer
   * - Juiste LED inschakelen voor timer
   * - Als timer afgelopen is lampjes laten flikkeren
 */
  if (pomodoro_active) {
    set_misc(5);  // Een van de extra ledjes laten branden ter aanwijzing

    // Timer is afgegaan
    if (pomodero_interrupt_fl) {
      pomodoro_new_value -= POMODERO_INTERVAL;  // Het standaard interval wordt afgetrokken van de totale ingestelde tijd
      pomodero_interrupt_fl = false;            // Interruptvlag uitschakelen
    }

    // Als de setup nog niet is uitgevoerd of
    // als er een nieuwe timerwaarde is ingesteld
    if (!pomodoro_setup || pomodoro_value != pomodoro_new_value) {
      pomodoro_value = pomodoro_new_value;  // Nieuwe timerwaarde registreren
      if (pomodoro_new_value != 0) {        // Als we nog niet klaar zijn

        /*   source_clock
         *       PCF85063A::TIMER_CLOCK_4096HZ     -> clk = 4096Hz -> min timer = 244uS -> MAX timer = 62.256mS        
         *       PCF85063A::TIMER_CLOCK_64HZ       -> clk = 64Hz   -> min timer = 15.625mS -> MAX timer = 3.984s
         *       PCF85063A::TIMER_CLOCK_1HZ        -> clk = 1Hz    -> min timer = 1s -> MAX timer = 255s
         *       PCF85063A::TIMER_CLOCK_1PER60HZ   -> clk = 1/60Hz -> min timer = 60s -> MAX timer = 4h15min  
         *   value
         *       coundowntime in seconds
         *   int_enable
         *       true = enable interrupt; false = disable interrupt
         *   int_pulse
         *       true = interrupt generate a pulse; false = interrupt follows timer flag
         * 
         *   Het standaardinterval wordt gebruikt om telkens bij een interrupt de leds correct te zetten om de vooruitgang weer te geven
         *   Standaard interval is 5 seconden dus iedere 5 seconden zal er een interrupt ingesteld worden en afgaan als we nog niet op 0 zitten 
         */
        rtc.timerSet(PCF85063A::TIMER_CLOCK_1HZ, POMODERO_INTERVAL, true, true);  // Standaardinterval instellen met interrupt ingeschakeld
        attachInterrupt(RTC_INT_PIN, RTC_INTERRUPT, LOW); // Interrupt inschakelen voor PC0
        pomodoro_setup = true; // De setup is bij deze voltooid
      }
    }
    set_minute(pomodoro_value); // LED corresponderend bij het aantal resterende minuten/seocnden laten branden

    // Als de timer afgeopen is
    if (pomodoro_value == 0) {
      // Heel snel alle leds afgaan
      for (byte h = 0; h < 10; h++) {
        for (byte i = 0; i < 12; i++) {
          set_hour(i);
          delay(0.1);
          for (byte j = 0; j < 60; j++) {
            set_minute(j);
            delay(0.1);
            set_hour(i);
            delay(0.1);
          }
        }
      }
      pomodoro_active = false; // Pomodoro is klaar
    }
  }


  // Toon uren en minuten.
  if (toon_tijd) {
    if (!tijd_opgehaald) {
      // Tijd maar 1x ophalen, enkel wanneer er gedrukt wordt.
      hour = rtc.getHour();      // Uur ophalen van RTC
      minute = rtc.getMinute();  // Minuten ophalen van RTC
      tijd_opgehaald = true;
    }
    // Blijven herhalen tot tijd op is
    // While loop omdat alle code die hierboven geschreven is niet van belang is wanneer de tijd wordt weergegeven
    // Als dat wel het geval is (bv. knoppen moeten nog bruikbaar zij) dan kan een if statement gebruitk worden
    //    In dat geval moet er een else toegevoegd worden waar de `time_now` wordt ingesteld en `toon_tijd` wordt uitgeschakeld
    if (millis() < time_now + period) {
      set_hour(hour);      // Uur weergeven
      delay(0.1);          // Geef CPU wat rusttijd
      set_minute(minute);  // Minuten weergeven
      delay(0.1);          // Geef CPU wat rusttijd
    } else {
      tijd_opgehaald = false;
      toon_tijd = false;  // Bij volgende loop tijd niet meer tonen
    }
  }
  delay(0.1);  // Geef CPU wat rusttijd
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
