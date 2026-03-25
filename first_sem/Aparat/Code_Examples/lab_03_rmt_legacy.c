/*  
  Rui Santos & Sara Santos - Random Nerd Tutorials
  https://RandomNerdTutorials.com/esp-idf-esp32-gpio-outputs/
*/

#include "driver/rmt.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define PWM_GPIO    8
#define RMT_CHANNEL RMT_CHANNEL_0
#define RMT_RX_CHANNEL RMT_CHANNEL_2
#define RMT_CLK_DIV 240  // 1 такт = 3 мкс

static const char *TAG = "RMT_RX";

void app_main(void) {
    // 1. Конфігурація RMT
    rmt_config_t cfg = {
        .rmt_mode = RMT_MODE_TX,
        .channel = RMT_CHANNEL,
        .gpio_num = PWM_GPIO,
        .clk_div = RMT_CLK_DIV,
        .mem_block_num = 1,
        .tx_config.loop_en = true,
        .tx_config.carrier_en = false
    };
    rmt_config(&cfg);
    rmt_driver_install(RMT_CHANNEL, 0, 0);

    // 2. Створюємо масив імпульсів
    rmt_item32_t items[32];

    // Щоб бачити Азбуку Морзе на світлоді - установіть 20000 або 30000
    // Але RX не працюватиме коректно
    // Щоб працював RX - робимо у сто разів швидше, тобто установіть 300
    #define DURATION 300

    // Видаємо сиґнал SOS: три точки, три тире, три точки, довга павза
    // .Три точка
    items[0] = (rmt_item32_t){.level0=0,.duration0=DURATION, .level1=0,.duration1=DURATION};
    items[1] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION};
    items[2] = (rmt_item32_t){.level0=0,.duration0=DURATION, .level1=0,.duration1=DURATION};
    items[3] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION};
    items[4] = (rmt_item32_t){.level0=0,.duration0=DURATION, .level1=0,.duration1=DURATION};

    // Пауза між літерами
    items[5] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION}; // пауза
    items[6] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION};
    items[7] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION}; // пауза

    // .Три тире
    items[8] = (rmt_item32_t){.level0=0,.duration0=DURATION, .level1=0,.duration1=DURATION};
    items[9] = (rmt_item32_t){.level0=0,.duration0=DURATION, .level1=0,.duration1=DURATION};
    items[10] = (rmt_item32_t){.level0=0,.duration0=DURATION, .level1=0,.duration1=DURATION};
    items[11] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION};
    items[12] = (rmt_item32_t){.level0=0,.duration0=DURATION, .level1=0,.duration1=DURATION};
    items[13] = (rmt_item32_t){.level0=0,.duration0=DURATION, .level1=0,.duration1=DURATION};
    items[14] = (rmt_item32_t){.level0=0,.duration0=DURATION, .level1=0,.duration1=DURATION};
    items[15] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION};
    items[16] = (rmt_item32_t){.level0=0,.duration0=DURATION, .level1=0,.duration1=DURATION};
    items[17] = (rmt_item32_t){.level0=0,.duration0=DURATION, .level1=0,.duration1=DURATION};
    items[18] = (rmt_item32_t){.level0=0,.duration0=DURATION, .level1=0,.duration1=DURATION};

    // Пауза між літерами
    items[19] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION}; // пауза
    items[20] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION};
    items[21] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION}; // пауза

    // .Три точка
    items[22] = (rmt_item32_t){.level0=0,.duration0=DURATION, .level1=0,.duration1=DURATION};
    items[23] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION};
    items[24] = (rmt_item32_t){.level0=0,.duration0=DURATION, .level1=0,.duration1=DURATION};
    items[25] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION};
    items[26] = (rmt_item32_t){.level0=0,.duration0=DURATION, .level1=0,.duration1=DURATION};

    // Пауза між словами
    items[27] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION}; // пауза
    items[28] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION};
    items[29] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION}; // пауза
    items[30] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION}; // пауза
    items[31] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION};
    items[32] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION}; // пауза
    items[33] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION}; // пауза
    items[34] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION}; // пауза
    items[35] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION};
    items[36] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION}; // пауза
    items[37] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION}; // пауза
    items[38] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION};
    items[39] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION}; // пауза
    items[40] = (rmt_item32_t){.level0=1,.duration0=DURATION, .level1=1,.duration1=DURATION}; // пауза

    // 3. Відправляємо на RMT
    rmt_write_items(RMT_CHANNEL, items, 39, true); // loop=true


    // 1. Налаштування RMT для прийому
    rmt_config_t rmt_rx_config2 = {
        .rmt_mode = RMT_MODE_RX,
        .channel = RMT_RX_CHANNEL,
        .gpio_num = 0,
        .clk_div = RMT_CLK_DIV,
        .mem_block_num = 1,
        .rx_config = {
            .filter_en = false,        // Без фільтра
            .idle_threshold = 5000    // 10ms тиша = кінець пакету
        }
    };
    ESP_ERROR_CHECK(rmt_config(&rmt_rx_config2));
    ESP_ERROR_CHECK(rmt_driver_install(RMT_RX_CHANNEL, 1024, 0));

    // 2. Увімкнення прийому
    RingbufHandle_t rb = NULL;
    ESP_ERROR_CHECK(rmt_get_ringbuf_handle(RMT_RX_CHANNEL, &rb));
    ESP_ERROR_CHECK(rmt_rx_start(RMT_RX_CHANNEL, true));

    // 3. Обробка отриманих даних
    while (rb) {
        size_t length = 0;
        rmt_item32_t *items = (rmt_item32_t *)xRingbufferReceive(rb, &length, pdMS_TO_TICKS(1000));
        if (items) {
            int num_items = length / sizeof(rmt_item32_t);
            ESP_LOGI(TAG, "Отримано %d імпульсів", num_items);
            for (int i = 0; i < num_items; i++) {
                uint32_t high_us = items[i].duration0;
                uint32_t low_us  = items[i].duration1;
                int level0 = items[i].level0;
                int level1 = items[i].level1;
                ESP_LOGI(TAG, "[%02d] Level: %d for %dus, then %d for %dus", i, level0, high_us, level1, low_us);
            }
            // Звільнення пам’яті буфера
            vRingbufferReturnItem(rb, (void *)items);
        }
    }
}