#ifndef __CAR_BAT_H__
#define __CAR_BAT_H__
#include "stm32f10x.h"
#include "timer.h"
#define LED_SLOW_BLINK_TIME 500 // 慢闪的时间，单位为ms
#define LED_FAST_BLINK_TIME 100 // 快闪的时间，单位为ms
void car_bat_init(void);
float car_get_current_v(void);
void car_bat_show_led_proc(void); // 显示电压的led的切片函数
#endif // !___CAR_BAT_H__
