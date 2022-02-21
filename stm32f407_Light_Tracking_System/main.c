#include "stm32f4xx.h"
#include "adc.h"
#include "usart.h"
#include "delay.h"
#include "servo.h"



int main(void)
{
	uint32_t utmp32;
	uint8_t curr_angle = 90;
	uint32_t prescalar = (uint16_t) ( ( (SystemCoreClock / 2) / 1000000) - 1);
	
	initSYSTIMER();
	initUSART2(USART2_BAUDRATE_115200);
	initADC1();
	
	SetServoPosition(curr_angle);

	SetupPWM(20000, prescalar);
	
	while(1)
	{ 
		utmp32 = getADC1();
		
		printUSART2("adc val: %d\n",utmp32);
		
		//delay_ms(1000);
		
		
		if (utmp32 < 100) {
			if (curr_angle < 170) {
				curr_angle++;
			}
		}
		
		if (utmp32 > 3500) {
			if (curr_angle > 0) {
				curr_angle--;
			}
		}
		
		SetServoPosition(curr_angle);
		delay_ms(100);
	}
}




