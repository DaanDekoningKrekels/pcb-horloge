#include <PCF85063A.h>
PCF85063A rtc;

// #include "Touch/src/TinyTouch.h"
#include <TinyTouch.h>

TinyTouch touch;

uint8_t touchPins[2] = { 4, 8 };  //initialize touch pins PIN_PB4, PIN_PC2

int hour, minute;

#define X1 10  // PIN_PA1
#define X2 11  // PIN_PA2
#define X3 13  // PIN_PA4
#define X4 14  // PIN_PA5
#define X5 15  // PIN_PA6
#define X6 16  // PIN_PA7

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

void setup() {
  touch.begin(touchPins, sizeof(touchPins));
  //bool format = rtc.changeTimeFormat();
  //  setTime(hour, minute, sec);
  // rtc.changeTimeFormat();
  rtc.setTime(16, 14, 00);  // 24H mode, ex. 6:54:00
  //  setDate(weekday, day, month, yr);
  // rtc.setDate(6, 16, 5, 2020);  // 0 for Sunday, ex. Saturday, 16.5.2020.
  reset_LED_pins();
}

void loop() {
  touch.touchHandle();

  if (touch.getValue(0) > 1000) {
    set_misc(0);
  } else {
    reset_LED_pins();
  }

    if (touch.getValue(1) > 1000) {
    set_misc(1);
  } else {
    reset_LED_pins();
  }

  hour = rtc.getHour();
  set_hour(hour);
  // delay(50);
  reset_LED_pins();

  minute = rtc.getMinute();
  set_minute(minute);

  // delay(50);
  reset_LED_pins();
}

void reset_LED_pins() {
  for (byte i = 0; i < AANTAL_LED_PINS; i++) {
    pinMode(LED_PINS[i], INPUT);  // Alle pinnen hoog impedant zetten
  }
  // PORTA.DIR = 0b00000000;  // set all pins as an output
}

void set_hour(int hour) {
  pinMode(UREN[hour][0], OUTPUT);
  pinMode(UREN[hour][1], OUTPUT);
  digitalWrite(UREN[hour][0], LOW);
  digitalWrite(UREN[hour][1], HIGH);
}

void set_minute(int minute) {
  pinMode(MINUTEN[minute][0], OUTPUT);
  pinMode(MINUTEN[minute][1], OUTPUT);
  digitalWrite(MINUTEN[minute][0], LOW);
  digitalWrite(MINUTEN[minute][1], HIGH);
}

void set_misc(int misc) {
  pinMode(MISC[misc][0], OUTPUT);
  pinMode(MISC[misc][1], OUTPUT);
  digitalWrite(MISC[misc][0], LOW);
  digitalWrite(MISC[misc][1], HIGH);
}