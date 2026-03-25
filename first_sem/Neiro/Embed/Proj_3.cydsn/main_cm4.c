#include "project.h"

int main(void)
{
    for (;;)
    {
        // PWM 1
        Cy_TCPWM_PWM_Init(PWM_1_HW, PWM_1_CNT_NUM, &PWM_1_config);
        Cy_TCPWM_PWM_Enable(PWM_1_HW, PWM_1_CNT_NUM);
        Cy_TCPWM_TriggerStart(PWM_1_HW, PWM_1_CNT_MASK);
        Cy_TCPWM_Counter_SetCounter(PWM_1_HW, PWM_1_CNT_NUM, 1199);
        CyDelay(1200);
        Cy_TCPWM_PWM_Disable(PWM_1_HW, PWM_1_CNT_NUM);
        Cy_TCPWM_TriggerStopOrKill(PWM_1_HW, PWM_1_CNT_MASK);
        Cy_TCPWM_PWM_DeInit(PWM_1_HW, PWM_1_CNT_NUM, &PWM_1_config);

        // PWM 2
        Cy_TCPWM_PWM_Init(PWM_2_HW, PWM_2_CNT_NUM, &PWM_2_config);
        Cy_TCPWM_PWM_Enable(PWM_2_HW, PWM_2_CNT_NUM);
        Cy_TCPWM_TriggerStart(PWM_2_HW, PWM_2_CNT_MASK);
        Cy_TCPWM_Counter_SetCounter(PWM_2_HW, PWM_2_CNT_NUM, 1199);
        CyDelay(1200);
        Cy_TCPWM_PWM_Disable(PWM_2_HW, PWM_2_CNT_NUM);
        Cy_TCPWM_TriggerStopOrKill(PWM_2_HW, PWM_2_CNT_MASK);
        Cy_TCPWM_PWM_DeInit(PWM_2_HW, PWM_2_CNT_NUM, &PWM_2_config);

        // PWM 3
        Cy_TCPWM_PWM_Init(PWM_3_HW, PWM_3_CNT_NUM, &PWM_3_config);
        Cy_TCPWM_PWM_Enable(PWM_3_HW, PWM_3_CNT_NUM);
        Cy_TCPWM_TriggerStart(PWM_3_HW, PWM_3_CNT_MASK);
        Cy_TCPWM_Counter_SetCounter(PWM_3_HW, PWM_3_CNT_NUM, 1199);
        CyDelay(1200);
        Cy_TCPWM_PWM_Disable(PWM_3_HW, PWM_3_CNT_NUM);
        Cy_TCPWM_TriggerStopOrKill(PWM_3_HW, PWM_3_CNT_MASK);
        Cy_TCPWM_PWM_DeInit(PWM_3_HW, PWM_3_CNT_NUM, &PWM_3_config);
    }
}
