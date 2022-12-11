#define X1 PIN_PA1
#define X2 PIN_PA2
#define X3 PIN_PA4
#define X4 PIN_PA5
#define X5 PIN_PA6
#define X6 PIN_PA7

const byte AANTAL_LED_PINS = 6;
const byte LED_PINS[] = { X1, X2, X3, X4, X5, X6 };

void setup() {
  // put your setup code here, to run once:
  reset_LED_pins();
}

void loop() {
  // put your main code here, to run repeatedly:
  for (byte i = 0; i < AANTAL_LED_PINS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    pinMode(LED_PINS[(i + 1) % AANTAL_LED_PINS], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
    digitalWrite(LED_PINS[(i + 1) % AANTAL_LED_PINS], HIGH);
    delay(50);
    reset_LED_pins();
  }
    for (byte i = 0; i < AANTAL_LED_PINS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    pinMode(LED_PINS[(i + 1) % AANTAL_LED_PINS], OUTPUT);
    digitalWrite(LED_PINS[i], HIGH);
    digitalWrite(LED_PINS[(i + 1) % AANTAL_LED_PINS], LOW);
    delay(50);
    reset_LED_pins();
  }
}

void reset_LED_pins() {
  for (byte i = 0; i < AANTAL_LED_PINS; i++) {
    pinMode(LED_PINS[i], INPUT);  // Alle pinnen hoog impedant zetten
  }
}