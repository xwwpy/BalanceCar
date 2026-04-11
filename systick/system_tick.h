#ifndef __CAR_SYSTEM_TICK_H__
#define __CAR_SYSTEM_TICK_H__
#include "stm32f10x.h"
#include "car_usart2.h"

void system_tick_init(void);

uint64_t get_tick(void);

uint64_t get_tick_us(void);

#endif
