#ifndef __CAR_MEASURE_METHOD_T_TEST_H__
#define __CAR_MEASURE_METHOD_T_TEST_H__
#include "car_encoder.h"
#include "car_usart2.h"
#include "timer.h"
#define T_MEASURE_SPEED_INTERVAL 100 // 测量的时间间隔，单位毫秒
void car_measure_method_t_test_proc(void);
void car_measure_method_t_print(void);
#endif // !__CAR_MEASURE_METHOD_T_TEST_H__
