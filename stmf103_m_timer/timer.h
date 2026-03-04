#ifndef __STM32F103_M_TIMER_H__
#define __STM32F103_M_TIMER_H__
#include "stm32f10x.h"
typedef void (*TimerCallBack) (void);
typedef struct {
    TimerCallBack timer_update_callback;
    uint16_t period;
} TimerInitStructure;
#include "stm32f10x.h"
void Xww_Timer1_Init(TimerInitStructure* init_structure);
u16 calculate_pwm(u16 max_val, float target_v, float source_v);
u32 get_tick(void); 
u32 get_current_s(void);
u32 get_current_ms(void);
void Xww_Timer_Generate_PWM_By_Oc1(u16 period, float duty, TIM_TypeDef* TIMx);
void Xww_Timer_Set_PWM_Duty_By_Oc1(u16 period, float duty, TIM_TypeDef* TIMx);
#endif
