#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_PIN 8
#define BUTTON_PIN 9

void app_main(void)
{
    // LED як вихід
    gpio_config_t io_led = {
        .pin_bit_mask = 1ULL << LED_PIN,
        .mode = GPIO_MODE_OUTPUT};
    gpio_config(&io_led);

    // Кнопка як вхід
    gpio_config_t io_btn = {
        .pin_bit_mask = 1ULL << BUTTON_PIN,
        .mode = GPIO_MODE_INPUT};
    gpio_config(&io_btn);

    while (1)
    {
        int btn = gpio_get_level(BUTTON_PIN); // 1 = не натиснуто, 0 = натиснуто

        if (btn == 0)
        {
            gpio_set_level(LED_PIN, 1); // гаснемо LED
        }
        else
        {
            gpio_set_level(LED_PIN, 0); // світиться LED
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
