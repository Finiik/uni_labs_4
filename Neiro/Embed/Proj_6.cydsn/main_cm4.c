#include "project.h"
#include <stdio.h>

#define MAX_COUNT 65535u

void HandleError(void);
void ISR_SW_2(void);

int count = 0u;
uint32_t printCount = false;
char buffer[50];

int main(void)
{
    int16_t adc_Code, adc_V;

    // Initialize UART status
    cy_en_scb_uart_status_t uart_status;

    // Initialize UART (Config & Context copied from generated source)
    uart_status = Cy_SCB_UART_Init(UART_HW, &UART_config, &UART_context);
    if (uart_status != CY_SCB_UART_SUCCESS)
    {
        Cy_GPIO_Clr(LED_Blue_PORT, LED_Blue_NUM);
        HandleError();
    }

    Cy_SCB_UART_Enable(UART_HW);

    // ADC Init & Start
    ADC_Init();
    ADC_Start();
    ADC_StartConvert();

    // Interrupt for SW2
    Cy_SysInt_Init(&SysInt_SW_cfg, ISR_SW_2);
    NVIC_ClearPendingIRQ(SysInt_SW_cfg.intrSrc);
    NVIC_EnableIRQ((IRQn_Type)SysInt_SW_cfg.intrSrc);

    __enable_irq(); // Enable global interrupts

    // UART Screen clear & intro
    Cy_SCB_UART_PutString(UART_HW, "\033[2J\033[H");
    Cy_SCB_UART_PutString(UART_HW, " -- LR_6. --\r\n");
    Cy_SCB_UART_PutString(UART_HW, " -- Work with SAR ADC in PSoC 6. --\r\n");

    for (;;)
    {
        if (printCount)
        {
            printCount = false;
            sprintf(buffer,
                    "Number of times switch SW2 has been pressed = %d\r\n",
                    count);
            Cy_SCB_UART_PutString(UART_HW, buffer);
        }

        ADC_IsEndConversion(1);
        adc_Code = ADC_GetResult16(0);
        adc_V = ADC_CountsTo_mVolts(0, adc_Code);

        sprintf(buffer,
                " --- ADC Code = %d --- ADC Value = %d (mV) --- \r\n",
                adc_Code, adc_V);
        Cy_SCB_UART_PutString(UART_HW, buffer);

        CyDelay(200);
    }
}

void ISR_SW_2(void)
{
    Cy_GPIO_ClearInterrupt(Pin_SW_2_PORT, Pin_SW_2_NUM);
    NVIC_ClearPendingIRQ(SysInt_SW_cfg.intrSrc);

    count++;
    if (count == MAX_COUNT)
    {
        count = 0u;
    }

    printCount = true;
}

void HandleError(void)
{
    __disable_irq(); // Disable interrupts
    while (1u)
    {
        // Trap CPU here
    }
}
