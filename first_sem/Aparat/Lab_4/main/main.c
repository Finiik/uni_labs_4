#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include <stdio.h>

#define PWM_GPIO 4
#define PWM_FREQ_HZ 100000 // 100 кГц
#define PWM_RES LEDC_TIMER_10_BIT
#define PWM_MAX_DUTY 1023

void pwm_init()
{
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = PWM_FREQ_HZ,
        .duty_resolution = PWM_RES};
    ledc_timer_config(&timer);

    ledc_channel_config_t ch = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = PWM_GPIO,
        .duty = 0,
    };
    ledc_channel_config(&ch);
}

void adc_init()
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); // GPIO0
}

void measure_adc()
{
    printf("ADC measurements:\n");
    for (int i = 0; i < 20; i++)
    {
        int raw = adc1_get_raw(ADC1_CHANNEL_0);
        printf("%d\n", raw);
        vTaskDelay(pdMS_TO_TICKS(16)); // 20 точок на період
    }
}

void app_main(void)
{
    pwm_init();
    adc_init();

    const int steps = 100;
    const float period = 1.0 / 3.0;
    const float dt = period / steps;

    while (1)
    {
        // Генерація пилки
        for (int i = 0; i < steps; i++)
        {
            int duty = (PWM_MAX_DUTY * i) / steps;
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

            vTaskDelay(pdMS_TO_TICKS(dt * 1000));
        }

        // Виконати вимір 4.10
        measure_adc();
    }
}
