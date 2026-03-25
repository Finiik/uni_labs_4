#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

#define PWM_GPIO    4       // Пін для PWM
#define PWM_FREQ_HZ 1000    // Частота PWM 1 кГц
#define PWM_RES_BITS LEDC_TIMER_8_BIT  // Розрядність 8 біт
#define PWM_CHANNEL LEDC_CHANNEL_0
#define PWM_MODE    LEDC_LOW_SPEED_MODE

void app_main(void) {
    // 1. Налаштовуємо таймер LEDC
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = PWM_MODE,          // низькошвидкісний режим
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = PWM_RES_BITS,      // розрядність PWM
        .freq_hz          = PWM_FREQ_HZ,       // частота
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // 2. Налаштовуємо канал PWM
    ledc_channel_config_t ledc_channel = {
        .gpio_num       = PWM_GPIO,
        .speed_mode     = PWM_MODE,
        .channel        = PWM_CHANNEL,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER_0,
        .duty           = 128,    // 50% від 8-бітного (0..255)
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);

    // 3. Зміна duty у циклі
    while (1) {
        for (int duty = 0; duty <= 255; duty += 5) {
            ledc_set_duty(PWM_MODE, PWM_CHANNEL, duty);
            ledc_update_duty(PWM_MODE, PWM_CHANNEL);
            vTaskDelay(pdMS_TO_TICKS(20));
        }
        for (int duty = 255; duty >= 0; duty -= 5) {
            ledc_set_duty(PWM_MODE, PWM_CHANNEL, duty);
            ledc_update_duty(PWM_MODE, PWM_CHANNEL);
            vTaskDelay(pdMS_TO_TICKS(20));
        }
    }
}