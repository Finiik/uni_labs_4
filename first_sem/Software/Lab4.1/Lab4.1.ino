// --- Піни ---
const int LED1_PIN = 5;    // Перенесено, бо GPIO18 зайнятий кнопкою
const int LED2_PIN = 19;
const int LED3_PIN = 21;

const int BUTTON_PIN = 18; // Кнопка → GPIO18

// Лічильник натискань
volatile uint8_t pressCount = 0;

// Для антидребезгу
volatile unsigned long lastInterruptTime = 0;

void IRAM_ATTR handleButton() {
  unsigned long currentTime = millis();

  // Антидребезг (100 мс)
  if (currentTime - lastInterruptTime > 100) {
    pressCount++;
    if (pressCount > 4) pressCount = 1;  // Циклічність
  }

  lastInterruptTime = currentTime;
}

void setup() {
  // Налаштування світлодіодів
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  // Кнопка з внутрішнім підтягуванням
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Переривання по спадаючому фронту (натискання)
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButton, FALLING);
}

void loop() {

  switch (pressCount) {

    case 1:
      // Увімкнути LED1
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, LOW);
      digitalWrite(LED3_PIN, LOW);
      break;

    case 2:
      // Увімкнути LED1 і LED2
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, HIGH);
      digitalWrite(LED3_PIN, LOW);
      break;

    case 3:
      // Увімкнути LED1, LED2, LED3
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, HIGH);
      digitalWrite(LED3_PIN, HIGH);
      break;

    case 4:
      // Вимкнути всі
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, LOW);
      digitalWrite(LED3_PIN, LOW);
      break;
  }

  // Головний цикл не блокується
}
