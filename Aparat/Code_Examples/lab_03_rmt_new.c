#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_rx.h"
#include "driver/rmt_types.h"

static const char *TAG = "RMT_EXAMPLE";

#define PIN_TX     8
#define PIN_RX     0

// Частота RMT — допустимий діапазон
// від   333333 Гц = 1 такт = 3 мкс
// до  80000000 Гц = 1 такт = 12.5 нс
#define RMT_RESOLUTION_HZ   333333ULL

// Максимальна кількість символів, які можемо надіслати або прийняти
#define SYMBOL_BUF_LEN      64  

// Об’єкт для передачі подій з callback у задачу
static QueueHandle_t rx_evt_queue = NULL;

// Callback, що спрацьовує коли прийом завершився або частково завершився
static bool rmt_rx_done_callback(rmt_channel_handle_t rx_chan,
                                 const rmt_rx_done_event_data_t *edata,
                                 void *user_data)
{
    // Ми працюємо в контексті ISR, тож треба взаємодіяти через “FromISR” функції
    BaseType_t high_task_woken = pdFALSE;
    // Передаємо дані в чергу (копіюємо структуру)
    xQueueSendFromISR(rx_evt_queue, edata, &high_task_woken);
    return (high_task_woken == pdTRUE);
}

void app_main(void)
{
    esp_err_t err;

    // ----- Створити TX канал -----
    rmt_channel_handle_t tx_chan = NULL;
    rmt_tx_channel_config_t tx_cfg = {
        .gpio_num = PIN_TX,
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = RMT_RESOLUTION_HZ,
        .mem_block_symbols = SYMBOL_BUF_LEN,
        .trans_queue_depth = 2,
        .flags.invert_out = false,
        .flags.with_dma = false
    };
    err = rmt_new_tx_channel(&tx_cfg, &tx_chan);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "rmt_new_tx_channel failed: %d", err);
        return;
    }
    ESP_ERROR_CHECK(rmt_enable(tx_chan));

    // ----- Створити RX канал -----
    rmt_channel_handle_t rx_chan = NULL;
    rmt_rx_channel_config_t rx_cfg = {
        .gpio_num = PIN_RX,
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = RMT_RESOLUTION_HZ,
        .mem_block_symbols = SYMBOL_BUF_LEN,
        .flags.invert_in = false,
        .flags.with_dma = false,
    };
    err = rmt_new_rx_channel(&rx_cfg, &rx_chan);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "rmt_new_rx_channel failed: %d", err);
        return;
    }
    ESP_ERROR_CHECK(rmt_enable(rx_chan));

    // ----- Реєструємо callback для RX подій -----
    rx_evt_queue = xQueueCreate(4, sizeof(rmt_rx_done_event_data_t));
    rmt_rx_event_callbacks_t evcbs = {
        .on_recv_done = rmt_rx_done_callback
    };
    ESP_ERROR_CHECK(rmt_rx_register_event_callbacks(rx_chan, &evcbs, (void *)rx_evt_queue));

    // ----- Підготуємо символи, які будемо передавати -----
    // Усі тривалості задаємо у періодах RMT_RESOLUTION_HZ !!!
    // Наприклад, якщо RMT_RESOLUTION_HZ = 1MHz, a duration = 1000
    // то тривалість буде (1000 / 1000000 Hz) = 1000 мкс = 1мс
    // Допустимий діапазон кожного duration від 0 до 32767 (15 біт) !!!
    rmt_symbol_word_t tx_symbols[3];
    tx_symbols[0].duration0 = 25000;
    tx_symbols[0].level0    = 1;
    tx_symbols[0].duration1 = 25000;
    tx_symbols[0].level1    = 0;

    tx_symbols[1].duration0 = 25000;
    tx_symbols[1].level0    = 1;
    tx_symbols[1].duration1 = 25000;
    tx_symbols[1].level1    = 0;

    tx_symbols[2].duration0 = 25000;
    tx_symbols[2].level0    = 0;
    tx_symbols[2].duration1 = 25000;
    tx_symbols[2].level1    = 0;

    // ----- Налаштування передавання -----
    rmt_transmit_config_t tx_trans_cfg = {
        .loop_count = -1,  // -1 → нескінчений цикл
        .flags = {
            .eot_level = 0     // рівень сигналу в кінці передавання, якщо потрібно
        }
    };

    // Структура конфігурації енкодера
    rmt_copy_encoder_config_t copy_encoder_cfg = {};

    // Дескриптор енкодера
    rmt_encoder_handle_t encoder = NULL;

    // Створення енкодера з конфігом
    err = rmt_new_copy_encoder(&copy_encoder_cfg, &encoder);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create copy encoder: %s", esp_err_to_name(err));
        return;
    }

    ESP_LOGI(TAG, "Start transmitting...");
    ESP_ERROR_CHECK(rmt_transmit(tx_chan, encoder, tx_symbols, sizeof(tx_symbols), &tx_trans_cfg));

    rmt_del_encoder(encoder);

    // ----- Налаштування прийому -----
    rmt_receive_config_t rx_recv_cfg = {
        // Мінімальна/максимальна тривалість допустимого імпульсу (наносекунд)
        .signal_range_min_ns = 100UL,       // У наносекундах. Усе менше — шум
        .signal_range_max_ns = 95000000UL,  // У наносекундах. Усе що довше — вважати за кінець пакета
                                            // Максимум: 32767*1000000000/RMT_RESOLUTION_HZ
        .flags = {
            .en_partial_rx = 1
        }
    };

    // Буфер, куди драйвер буде записувати символи при прийомі
    static rmt_symbol_word_t rx_buf[SYMBOL_BUF_LEN];

    // Запускаємо прийом (неблокуюча функція)
    ESP_ERROR_CHECK(rmt_receive(rx_chan, rx_buf, sizeof(rx_buf), &rx_recv_cfg));
    ESP_LOGI(TAG, "Receiver started");

    // ----- Цикл обробки подій прийому -----
    while (1) {
        rmt_rx_done_event_data_t rx_data;
        if (xQueueReceive(rx_evt_queue, &rx_data, portMAX_DELAY) == pdTRUE) {
            // rx_data.received_symbols — вказівник на буфер (тобто на rx_buf)
            int nsym = rx_data.num_symbols;
            ESP_LOGI(TAG, "RX done: %d symbols, is_last = %d", nsym, rx_data.flags.is_last);

            rmt_symbol_word_t *p = rx_data.received_symbols;
            for (int i = 0; i < nsym; i++) {
                uint32_t d0 = p[i].duration0;
                uint32_t d1 = p[i].duration1;
                int l0 = p[i].level0;
                int l1 = p[i].level1;
                ESP_LOGI(TAG, "[%02d] L0=%d for %uus, L1=%d for %uus", i, l0, d0, l1, d1);
            }

            // Якщо це “останнє” завершення прийому (is_last == true),
            // можна повторно запустити прийом
            if (rx_data.flags.is_last) {
                // Перезапускаємо прийом
                ESP_ERROR_CHECK(rmt_receive(rx_chan, rx_buf, sizeof(rx_buf), &rx_recv_cfg));
            }
        }
    }
}