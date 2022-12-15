void setup() {
  // put your setup code here, to run once:
  reset_LED_pins();
}

void loop() {
  // put your main code here, to run repeatedly:
}

void reset_LED_pins() {
  PORTA.DIR = 0b00000000;  // set all pins as an output
}