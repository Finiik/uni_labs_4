// --- Конфігурація ШІМ ---

const int PWM_FREQ = 1000;        // 1 кГц
const int PWM_RESOLUTION = 8;    // 10 біт → 1024 рівні яскравості
const int MAX_DUTY = (1 << PWM_RESOLUTION) - 1;

// Піни LED
const int LED1_PIN = 18;
const int LED2_PIN = 19;
const int LED3_PIN = 21;

// Канали ШІМ
const int LED1_CH = 0;
const int LED2_CH = 1;
const int LED3_CH = 2;

// Час на засвічування / загасання: 4 сек
// Кожен крок = 4000 мс / 1023 ≈ 3.9 мс
const float STEP_DELAY_MS = 4000.0 / MAX_DUTY;

void setup() {
  // Налаштування каналів
  ledcSetup(LED1_CH, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(LED2_CH, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(LED3_CH, PWM_FREQ, PWM_RESOLUTION);

  // Прив’язка пінів
  ledcAttachPin(LED1_PIN, LED1_CH);
  ledcAttachPin(LED2_PIN, LED2_CH);
  ledcAttachPin(LED3_PIN, LED3_CH);
}

void loop() {

  // --- Плавне засвічування всіх трьох LED одночасно ---
  for (int duty = 0; duty <= MAX_DUTY; duty++) {
    ledcWrite(LED1_CH, duty);
    ledcWrite(LED2_CH, duty);
    ledcWrite(LED3_CH, duty);
    delay(STEP_DELAY_MS);
  }

  // --- Плавне загасання всіх трьох LED одночасно ---
  for (int duty = MAX_DUTY; duty >= 0; duty--) {
    ledcWrite(LED1_CH, duty);
    ledcWrite(LED2_CH, duty);
    ledcWrite(LED3_CH, duty);
    delay(STEP_DELAY_MS);
  }
}
