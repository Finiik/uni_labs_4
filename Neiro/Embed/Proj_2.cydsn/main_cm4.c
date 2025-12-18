#include "project.h"

#define LED_ON   (uint8_t)(0u)
#define LED_OFF  (uint8_t)(1u)


#define SW_P0_4_PORT   P0_4_PORT
#define SW_P0_4_NUM    P0_4_NUM



int main(void)
{
    volatile uint32_t pinReadValue = 1ul;
    uint32_t portReadValue = 0ul;

    for (;;)
    {
   L1:
        pinReadValue = Cy_GPIO_Read(SW_P0_4_PORT, SW_P0_4_NUM);
        if (pinReadValue != 0) goto L1;

    L2:
        CyDelay(10);
        if (pinReadValue != 0) goto L2;

        Cy_GPIO_Write(LED_Blue_PORT, LED_Blue_NUM, LED_ON);
        CyDelay(200);
        Cy_GPIO_Write(LED_Blue_PORT, LED_Blue_NUM, LED_OFF);
        Cy_GPIO_Write(LED_Red_PORT, LED_Red_NUM, pinReadValue);

    L3:
        pinReadValue = Cy_GPIO_Read(SW_P0_4_PORT, SW_P0_4_NUM);
        if (pinReadValue == 0) goto L3;

    L4:
        CyDelay(10);
        if (pinReadValue == 0) goto L4;

        Cy_GPIO_Write(LED_Red_PORT, LED_Red_NUM, pinReadValue);
        Cy_GPIO_Write(LED_Blue_PORT, LED_Blue_NUM, LED_ON);
        CyDelay(200);
        Cy_GPIO_Write(LED_Blue_PORT, LED_Blue_NUM, LED_OFF);

        CyDelay(100);
    }
}
/* [] END OF FILE */
