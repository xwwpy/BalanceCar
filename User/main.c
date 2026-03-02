#include "stm32f10x.h"                  // Device header

#include "car_bat.h"
#include "car_usart2.h"
#include "car_bat_test.h"
#include "Delay.h"


int main(void){
	 // 初始化nvic
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	car_bat_init(); // 初始化电压采集模块
	usart2_init(); // 初始化usart2
	TimerInitStructure timer1_init_structure = {0};
	Xww_Timer1_Init(&timer1_init_structure); // 初始化定时器1

	while(1) {
		car_bat_debug_print_proc(); // 定时打印电压信息
		car_bat_show_led_proc(); // 根据电压显示对应的led状态
	}
}
