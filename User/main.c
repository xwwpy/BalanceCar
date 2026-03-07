#include "stm32f10x.h"                  // Device header

#include "car_bat.h"
#include "car_usart2.h"
#include "car_encoder.h"
#include "car_button.h"
#include "car_pwm.h"
#include "Delay.h"

#include "car_encoder_test.h"
#include "car_measure_method_m_test.h"

int main(void){
	 // 初始化nvic
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	car_pwm_init(); // 初始化pwm 这里初始化了timer1(同时用于计时)和timer4
	car_bat_init(); // 初始化电压采集模块
	usart2_init(); // 初始化usart2
	car_button_init(); // 初始化按键
	car_encoder_init(); // 初始化编码器
	car_pwm_set_L(0.99); // 设置左电机占空比为10% 正转
	car_pwm_set_R(0.99); // 设置右电机占空比为10% 反转
	car_pwm_cmd(0); // 休眠电机
	while(1) {
		car_bat_show_led_proc(); // 根据电压显示对应的led状态
		car_button_proc(); // 处理按键事件
		// car_encoder_test_proc(); // 测试编码器
		car_measure_method_m_test_proc(); // 测试测量速度方法M
	}
}
