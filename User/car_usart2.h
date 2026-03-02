#ifndef __CAR_USART_2_H__
#define __CAR_USART_2_H__
#include "stm32f10x.h"
#include "stdio.h"
void usart2_init(void);
void send_string(char* str, u8 len);
#endif // !__CAR_USART_2_H__
