#ifndef __SERVO_H
#define __SERVO_H

#include "gpio.h"
#include "rcc.h"
#include "tim.h"


#define SERVO_MIN_ANGLE 20    //PWM value for 0 degree
#define SERVO_MAX_ANGLE 3200  //PWM value for 180 degree

void SetupPWM(uint32_t period, uint16_t PrescalerValue);
void SetupPins();
void SetServoPosition(uint32_t angle);
void PositionServoPulse(uint32_t pulse);

#endif
