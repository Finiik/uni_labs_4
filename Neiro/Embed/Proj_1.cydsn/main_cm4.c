#include "project.h"

#define LED_ON   (uint8_t)(0u)
#define LED_OFF  (uint8_t)(1u)

#define LED_RED_PORT    P0_3_PORT
#define LED_RED_NUM     P0_3_NUM

#define LED_BLUE_PORT   P11_1_PORT
#define LED_BLUE_NUM    P11_1_NUM

int main(void)
{
    __enable_irq();
    unsigned char n1 = 5, ij;
    const uint32_t T1 = 750;   
    const uint32_t T2 = 1250;  

    for(;;)
    {
       for (ij = 0; ij<n1; ij++){
        Cy_GPIO_Write(LED_RED_PORT, LED_RED_NUM, LED_ON);
        Cy_GPIO_Write(LED_BLUE_PORT, LED_BLUE_NUM, LED_OFF);
        CyDelay(T1);
        

        Cy_GPIO_Write(LED_BLUE_PORT, LED_BLUE_NUM, LED_ON);
        Cy_GPIO_Write(LED_RED_PORT, LED_RED_NUM, LED_OFF);
        CyDelay(T1);
        Cy_GPIO_Write(LED_BLUE_PORT, LED_BLUE_NUM, LED_OFF);
        Cy_GPIO_Write(LED_RED_PORT, LED_RED_NUM, LED_ON);
        CyDelay(T1);
        Cy_GPIO_Write(LED_BLUE_PORT, LED_BLUE_NUM, LED_ON);
        Cy_GPIO_Write(LED_RED_PORT, LED_RED_NUM, LED_OFF);
        CyDelay(T1);
        Cy_GPIO_Write(LED_RED_PORT, LED_RED_NUM, LED_OFF);
        Cy_GPIO_Write(LED_BLUE_PORT, LED_BLUE_NUM, LED_OFF);
        CyDelay(T1);
        Cy_GPIO_Write(LED_RED_PORT, LED_RED_NUM, LED_ON);
        Cy_GPIO_Write(LED_BLUE_PORT, LED_BLUE_NUM, LED_ON);
        CyDelay(T2);
        Cy_GPIO_Write(LED_BLUE_PORT, LED_BLUE_NUM, LED_OFF);
        Cy_GPIO_Write(LED_RED_PORT, LED_RED_NUM, LED_ON);
        CyDelay(T2);
        Cy_GPIO_Write(LED_BLUE_PORT, LED_BLUE_NUM, LED_ON);
        Cy_GPIO_Write(LED_RED_PORT, LED_RED_NUM, LED_OFF);
        CyDelay(T2);
        Cy_GPIO_Write(LED_BLUE_PORT, LED_BLUE_NUM, LED_OFF);
        Cy_GPIO_Write(LED_RED_PORT, LED_RED_NUM, LED_OFF);
        CyDelay(T2);
    }
        
       
    }
}
