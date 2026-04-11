#ifndef __MATH_UTILS_H__
#define __MATH_UTILS_H__
#include "stm32f10x.h"

uint32_t Xww_pow(uint8_t base, uint8_t exp);
uint32_t Xww_abs(int32_t val);
float qsin(float x);
float qcos(float x);
float qtan(float x);
float qasin(float x);
float qacos(float x);
float qatan(float x);
float qatan2(float y, float x);
#endif
