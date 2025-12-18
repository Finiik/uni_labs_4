// Піни
#define BUTTON_PIN 4
#define LED_PIN 5

// Таймер ESP32
hw_timer_t * timer = NULL;
volatile uint32_t timerMillis = 0;  // Лічильник мілісекунд

// Стани вимірювання
volatile bool firstPress = false;
volatile uint32_t startTime = 0;
volatile uint32_t endTime = 0;

// Таймерне переривання: кожні 1 мс
void IRAM_ATTR onTimer() {
  timerMillis++;
}

// Обробка натискання кнопки
void IRAM_ATTR handleButton() {
  if (!firstPress) {
    // Перше натискання
    firstPress = true;
    startTime = timerMillis;
  } else {
    // Друге натискання
    endTime = timerMillis;
    firstPress = false;
    digitalWrite(LED_PIN, HIGH);   // Індикація завершення вимірювання
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Таймер: 80 MHz / 80 = 1 МГц → 1 tick = 1 мікросекунда
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true); // 1000 мкс = 1 мс
  timerAlarmEnable(timer);

  // Переривання по кнопці (спадаючий фронт)
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButton, FALLING);
}

void loop() {
  // Якщо вимір завершено
  if (!firstPress && endTime != 0) {
    uint32_t interval = endTime - startTime;

    Serial.print("Interval: ");
    Serial.print(interval);
    Serial.println(" ms");

    delay(1000);
    digitalWrite(LED_PIN, LOW); // Вимикаємо індикатор

    // Готуємось до нового вимірювання
    endTime = 0;
  }
}
