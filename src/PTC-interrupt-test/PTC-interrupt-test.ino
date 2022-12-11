// In slaap gaan lukt nog niet
// Interrupts wel
// https://github.com/xukangmin/TinyCore/blob/master/avr/cores/tiny/ATtiny_DFP/include/avr/iotn1616.h#L1623
// Datasheet p94 | 11.5.1 Control A Register CTRLA





#include <avr/sleep.h>  //Needed for sleep_mode
#include <avr/power.h>  //Needed for powering down perihperals such as the ADC/TWI and Timers
#include <TinyTouch.h>
// #include "src/Touch/TinyTouch.h"
TinyTouch touch;
uint8_t touchPins[2] = { 4, 8 };  //initialize touch pins PIN_PB4, PIN_PC2
/* Defines the interrupt priority for the PTC. Set low priority to PTC interrupt for applications having interrupt time
 * constraints. Range: 0 to 2 Default: 2 (Lowest Priority)
 */
#define DEF_PTC_INTERRUPT_PRIORITY 0

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

const byte MISC[6][2]{
  // 0    1
  { X3, X4 },  // 1  min
  { X6, X3 },  // 2  min
  { X3, X5 },  // 3  min
  { X6, X4 },  // 4  min
  { X6, X5 },  // W
  { X3, X6 }   // W
};

bool blijven_hangen = false;

void setup() {
  touch.begin(touchPins, sizeof(touchPins));

  reset_LED_pins();

  sei();
  // Selects which sleep mode to enter when the Sleep Enable (SEN) bit is written to ‘1’
  // and the SLEEP extruction is executed.
  // SLPCTRL_CTRLA = SLEEP_MODE_IDLE;
  sleep_enable();
  set_sleep_mode(SLEEP_MODE_IDLE);
}

ISR(RTC_CNT_vect) {
  /* Insert your RTC Compare interrupt handling code */
  touch.touch_timer_handler();

  /* Compare interrupt flag has to be cleared manually */
  RTC.INTFLAGS = RTC_CMP_bm;

  touch.touchHandle();
  if (touch.getValue(0) > 1000) {
    blijven_hangen = true;
    // SLPCTRL_CTRLA = 0b00000000;
    sleep_disable();
  }
  if (touch.getValue(1) > 1000) {
    blijven_hangen = true;
    // SLPCTRL_CTRLA = 0b00000000;
    sleep_disable();
  }
}

void loop() {
  while (blijven_hangen) {
    set_misc(2);
    delay(100);
    blijven_hangen = false;
  }

  // touch.touchHandle();

  // Even door alle tijden lopen
  for (byte i = 0; i < 12; i++) {
    if (blijven_hangen)
      return;
    set_hour(i);
    delay(5);
    for (byte j = 0; j < 60; j++) {
      if (blijven_hangen)
        return;
      set_minute(j);
      delay(5);
      set_hour(i);
      delay(5);
    }
  }
  // SLPCTRL_CTRLA = 0b00000001;
  sleep_enable();

  sleep_cpu();
  // delay(1);
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
