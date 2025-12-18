#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

#define LEDC_OUTPUT_IO    (4)   // GPIO where you want the PWM signal
#define LEDC_CHANNEL      LEDC_CHANNEL_0
#define LEDC_TIMER        LEDC_TIMER_0
#define LEDC_MODE         LEDC_HIGH_SPEED_MODE
#define LEDC_DUTY_RES     LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_FREQUENCY    (5000)            // Frequency in Hz. Set to 5 kHz
#define LEDC_DUTY         (4095)            // 50% duty (2^13 / 2)

void app_main(void)
{
    // 1. Configure timer
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = LEDC_FREQUENCY,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // 2. Configure channel
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO,
        .duty           = 0, // start with 0% duty
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);

    // 3. Fade service (optional)
    ledc_fade_func_install(0);

    while (1) {
        // Increase duty cycle gradually
        for (int duty = 0; duty <= LEDC_DUTY; duty += 256) {
            ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
            ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
            vTaskDelay(pdMS_TO_TICKS(50));
        }
        // Decrease duty cycle gradually
        for (int duty = LEDC_DUTY; duty >= 0; duty -= 256) {
            ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
            ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}
