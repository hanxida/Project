#ifndef _GENERALTIME_H
#define _GENERALTIME_H


#include "stm32f10x.h"


/************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4��5************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������Ĭ��ʹ��TIM3

#define            GENERAL_TIM                   TIM3
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM3
#define            GENERAL_TIM_Period            39999
#define            GENERAL_TIM_Prescaler         35
// TIM3 ����Ƚ�ͨ��1
#define            GENERAL_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH1_PORT          GPIOA
#define            GENERAL_TIM_CH1_PIN           GPIO_Pin_6

// TIM3 ����Ƚ�ͨ��2
#define            GENERAL_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH2_PORT          GPIOA
#define            GENERAL_TIM_CH2_PIN           GPIO_Pin_7

/*��ת�Ƕ�ռ�ձ���ֵ�궨��*/
#define            ZeroDegree                    1000   
#define            Forty_FiveDegree              2000   
#define            NinetyDegree                  3000   
#define            Onehundred_ThirtyfiveDegree   4000   
#define            Onehundred_EightyDegree       5000   

void GENERAL_TIM_Init(void);

//void DegreeControlOne(uint16_t CCR1_Val);
//void DegreeControlTwo(uint16_t CCR1_Val);

#endif	/* __BSP_GENERALTIME_H */



