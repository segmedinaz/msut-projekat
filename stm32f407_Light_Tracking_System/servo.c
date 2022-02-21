#include <servo.h>

void SetServoPosition(uint32_t angle){
	uint32_t range = SERVO_MAX_ANGLE - SERVO_MIN_ANGLE;
	uint32_t unit = range / 180;

	uint32_t step = unit * angle;

	if(step == 0)
		step = SERVO_MIN_ANGLE;

	if(step > SERVO_MAX_ANGLE)
		step = SERVO_MAX_ANGLE;

	PositionServoPulse(step);
}

/*
 * Positions the servo by re-configuring PWM and changing the pulse value to be the value passed in
 * for the Futaba S3003 im using this should be between 20 and 2500
 */
void PositionServoPulse(uint32_t pulse){
	  TIM_OCInitTypeDef  TIM_OCInitStructure;
	/* Configure timer for PWM - channel 1*/
	  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = pulse;
	  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	  //notice the number 1 in TIM_OC1Init
	  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
}

/**
 * Sets up PWM and the PB6 pin ready for use with an initial value of ServoMinAngle
 */
void SetupPWM(uint32_t period, uint16_t PrescalerValue){
  SetupPins();

	/*Structures used in the configuration*/
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  /* Setup timer defaults */
  TIM_TimeBaseStructure.TIM_Period = period;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* Configure timer for PWM - channel 1*/
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  //notice the number 1 in TIM_OC1Init
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);


  TIM_ARRPreloadConfig(TIM4, ENABLE);

  /* Start timer*/
  TIM_Cmd(TIM4, ENABLE);

}


/**
 * Sets up the PB6 pin for PWM
 */
void SetupPins(){
	/* Enable TIM4 Clock */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);


	  //Enabled GPIOB we are going to use PB6 which is linked to TIM4_CH1 according to the
	  //documentation
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	  GPIO_InitTypeDef GPIO_InitStructure;
	  //initalise pin 6 B - relating to timer 4 channel 1
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);

	  //link the pins up to the timer - AF stands for alternate function
	  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
}

