#ifndef __CAR_ENCODER_TEST_H__
#define __CAR_ENCODER_TEST_H__
#include "car_encoder.h"
#include "car_usart2.h"
#include "timer.h"
#define SHOW_INFO_INTERVAL 100 // 显示信息的时间间隔，单位毫秒
void car_encoder_test_proc(void);
#endif // !__CAR_ENCODER_TEST_H__
