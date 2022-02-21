#ifndef __RCC_H
#define __RCC_H

#include "stm32f4xx.h"

#define RCC_AHB1Periph_GPIOB             ((uint32_t)0x00000002)
#define RCC_APB1Periph_TIM4              ((uint32_t)0x00000004)

void        RCC_TIMCLKPresConfig(uint32_t RCC_TIMCLKPrescaler);
void        RCC_AHB1PeriphClockCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState);
void        RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);
FlagStatus  RCC_GetFlagStatus(uint8_t RCC_FLAG);



#endif /* __STM32F4xx_RCC_H */

