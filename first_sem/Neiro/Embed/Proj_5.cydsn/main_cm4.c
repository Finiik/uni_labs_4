// LR_9_1
#include "project.h"

#define LED_ON  (0)
#define LED_OFF (1)

void UartInit(void);
void TimerInit(void);
void Isr_Timer(void);

bool LEDupdateFlag = false;

int main(void)
{
    UartInit();
    __enable_irq();

    Cy_SCB_UART_PutString(UART_HW, "\x1b[2J\x1b[;H");
    Cy_SCB_UART_PutString(UART_HW, "*** Example Project UART ***\r\n\n");
    Cy_SCB_UART_PutString(UART_HW, " --- Lab_Work_9 ---\r\n\n");
    Cy_SCB_UART_PutString(UART_HW, "Press Enter key to start blinking the LED\r\n\n");

    while (Cy_SCB_UART_Get(UART_HW) != '\r');

    TimerInit();
    Cy_SCB_UART_PutString(UART_HW, "Observe the LED blinking on the kit!!!\r\n");

    for (;;)
    {
        if (LEDupdateFlag)
        {
            LEDupdateFlag = false;
            Cy_GPIO_Inv(LED_Green_0_PORT, LED_Green_0_NUM);
        }
    }
}

void UartInit(void)
{
    Cy_SCB_UART_Init(UART_HW, &UART_config, &UART_context);
    Cy_SCB_UART_Enable(UART_HW);
}

void TimerInit(void)
{
    Cy_TCPWM_Counter_Init(Timer_HW, Timer_CNT_NUM, &Timer_config);
    Cy_TCPWM_Counter_Enable(Timer_HW, Timer_CNT_NUM);
    Cy_TCPWM_TriggerStart(Timer_HW, Timer_CNT_MASK);

    Cy_SysInt_Init(&SysInt_Timer_cfg, Isr_Timer);
    NVIC_EnableIRQ((IRQn_Type)SysInt_Timer_cfg.intrSrc);
}

void Isr_Timer(void)
{
    Cy_TCPWM_ClearInterrupt(Timer_HW, Timer_CNT_NUM, CY_TCPWM_INT_ON_TC);
    NVIC_ClearPendingIRQ(SysInt_Timer_cfg.intrSrc);
    LEDupdateFlag = true;
}
