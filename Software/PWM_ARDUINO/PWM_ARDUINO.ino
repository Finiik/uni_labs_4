// PWM LED fade using analogWrite() (Arduino Uno/Nano/Mega)

const int LED_PIN1 = 15;   // Use a PWM-capable pin: 3, 5, 6, 9, 10, or 11 on Uno
const int LED_PIN2 = 2; 
const int LED_PIN3 = 4; 
const int DELAY_MS = 20; // Delay between brightness steps

void setup() {
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);
}

void loop() {
  // Fade in
  for (int duty = 0; duty <= 255; duty++) {
    analogWrite(LED_PIN1, duty); // 0–255 duty cycle
    analogWrite(LED_PIN2, duty);
    analogWrite(LED_PIN3, duty);
    delay(DELAY_MS);
  }

  // Fade out
  for (int duty = 255; duty >= 0; duty--) {
    analogWrite(LED_PIN1, duty);
    analogWrite(LED_PIN2, duty);
    analogWrite(LED_PIN3, duty);
    delay(DELAY_MS);
  }
}
