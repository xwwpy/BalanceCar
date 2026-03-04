#ifndef __CAR_ENCONDER_H__
#define __CAR_ENCONDER_H__
#include "stm32f10x.h"
#include "exit.h"
void car_encoder_init(void);
void car_encoder_proc(void);
int64_t car_encoder_get_L(void);
int64_t car_encoder_get_R(void);
#endif // !__CAR_ENCONDER_H__
