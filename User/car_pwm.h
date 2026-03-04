#ifndef __CAR_PWM_H__
#define __CAR_PWM_H__
#include "timer.h"
void car_pwm_init(void);
void car_pwm_proc(void);
void car_pwm_set_L(float duty);
void car_pwm_set_R(float duty);
void car_pwm_cmd(uint8_t cmd);
#endif // !__CAR_PWM_H__
