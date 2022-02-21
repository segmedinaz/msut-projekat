#ifndef __ADC1_H
#define __ADC1_H

#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"

void 		initADC1(void);	
uint16_t 	getADC1(void);

#endif 
