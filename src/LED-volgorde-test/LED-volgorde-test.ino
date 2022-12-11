#define X1 PIN_PA1
#define X2 PIN_PA2
#define X3 PIN_PA4
#define X4 PIN_PA5
#define X5 PIN_PA6
#define X6 PIN_PA7

const byte AANTAL_LED_PINS = 6;
const byte LED_PINS[] = { X1, X2, X3, X4, X5, X6 };

const byte OPTIES = 30;
const byte LED_LUT[OPTIES][2] = {
  // 0    1
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
  { X3, X4 },  // 1  min
  { X3, X5 },  // 3  min
  { X3, X6 },  // W
  { X4, X1 },  // 1  Min
  { X4, X2 },  // 2  Min
  { X4, X3 },  // 3  Min
  { X4, X5 },  // 4  Min
  { X4, X6 },  // 5  Min
  { X5, X1 },  // 6  Min
  { X5, X2 },  // 7  Min
  { X5, X3 },  // 8  Min
  { X5, X4 },  // 9  Min
  { X5, X6 },  // 10 Min
  { X6, X1 },  // 11 Min
  { X6, X2 },  // 12 Min
  { X6, X3 },  // 2  min
  { X6, X4 },  // 4  min
  { X6, X5 }   // W
};

const byte MINUTEN[12][2] = {
  // 0    1
  { X6, X2 },  // 12 Min
  { X4, X1 },  // 1  Min
  { X4, X2 },  // 2  Min
  { X4, X3 },  // 3  Min
  { X4, X5 },  // 4  Min
  { X4, X6 },  // 5  Min
  { X5, X1 },  // 6  Min
  { X5, X2 },  // 7  Min
  { X5, X3 },  // 8  Min
  { X5, X4 },  // 9  Min
  { X5, X6 },  // 10 Min
  { X6, X1 }   // 11 Min
};

const byte UREN[12][2]{
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
  // put your setup code here, to run once:
  reset_LED_pins();
}

void loop() {
  // put your main code here, to run repeatedly:
  for (byte i = 0; i < OPTIES; i++) {
    pinMode(LED_LUT[i][0], OUTPUT);
    pinMode(LED_LUT[i][1], OUTPUT);
    digitalWrite(LED_LUT[i][0], LOW);
    digitalWrite(LED_LUT[i][1], HIGH);
    delay(100);
    reset_LED_pins();
  }
    for (byte i = 0; i < 12; i++) {
    pinMode(MINUTEN[i][0], OUTPUT);
    pinMode(MINUTEN[i][1], OUTPUT);
    digitalWrite(MINUTEN[i][0], LOW);
    digitalWrite(MINUTEN[i][1], HIGH);
    delay(100);
    reset_LED_pins();
  }  
  for (byte i = 0; i < 12; i++) {
    pinMode(UREN[i][0], OUTPUT);
    pinMode(UREN[i][1], OUTPUT);
    digitalWrite(UREN[i][0], LOW);
    digitalWrite(UREN[i][1], HIGH);
    delay(100);
    reset_LED_pins();
  }  
}

void reset_LED_pins() {
  for (byte i = 0; i < AANTAL_LED_PINS; i++) {
    pinMode(LED_PINS[i], INPUT);  // Alle pinnen hoog impedant zetten
  }
}