#include "project.h"

/* Макроси для налаштування проекту */
#define DELAY_SHORT      (500u)   /* msec */
#define DELAY_LONG       (1000u)  /* msec */
#define LED_BLINK_COUNT  (6u)
#define LED_ON           (0u)
#define LED_OFF          (1u)

/* Глобальні змінні */
volatile bool interrupt_flag = false;

/* 
 * Функція: Isr_switch()
 * Виконується, коли спрацьовує переривання GPIO.
 */
void Isr_switch(void)
{
    /* Очищення переривання по виводу SW_P0_4 */
    Cy_GPIO_ClearInterrupt(SW_P0_4_PORT, SW_P0_4_NUM);
    NVIC_ClearPendingIRQ(SysInt_Port0_cfg.intrSrc);

    /* Встановити прапорець переривання */
    interrupt_flag = true;
}

int main(void)
{
    uint32_t count = 0;
    uint32_t delayMs = DELAY_LONG;

    /* Дозвіл глобальних переривань */
    __enable_irq();

    /* Ініціалізація та дозвіл переривань GPIO */
    Cy_SysInt_Init(&SysInt_Port0_cfg, Isr_switch);
    NVIC_ClearPendingIRQ(SysInt_Port0_cfg.intrSrc);
    NVIC_EnableIRQ(SysInt_Port0_cfg.intrSrc);

    for (;;)
    {
        /* Зміна статусу переривання */
        if (interrupt_flag)
        {
            interrupt_flag = false;

            /* Зміна частоти блимання LED */
            if (delayMs == DELAY_LONG)
            {
                delayMs = DELAY_SHORT;
            }
            else
            {
                delayMs = DELAY_LONG;
            }
        }

        /* Цикл блимання світлодіодом LED LED_BLINK_COUNT разів */
        for (count = 0; count < LED_BLINK_COUNT; count++)
        {
            Cy_GPIO_Write(LED_Red_PORT, LED_Red_NUM, LED_ON);
            Cy_SysLib_Delay(delayMs);
            Cy_GPIO_Write(LED_Red_PORT, LED_Red_NUM, LED_OFF);
            Cy_SysLib_Delay(delayMs);
        }

        /* Перехід у режим ″глибокого сну″ */
        Cy_SysPm_DeepSleep(CY_SYSPM_WAIT_FOR_INTERRUPT);
    }
}
