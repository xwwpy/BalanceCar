#include "timer.h"

TimerCallBack timer1_update_callback;

uint64_t current_s = 0;

u16 tick_count = 0;

uint8_t timer1_init_flag = 0;
uint8_t timer4_init_flag = 0;

uint64_t get_current_s(void) {
    return current_s;
}

uint64_t get_current_ms(void) {
    return current_s * 1000 + tick_count;
}

uint64_t get_current_us(void) {
    return current_s * 1000000 + tick_count * 1000 + TIM_GetCounter(TIM1);
}

void Xww_Timer1_Init(TimerInitStructure* init_structure) {
    if (timer1_init_flag == 1) {
        return;
    }
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    timer1_update_callback = init_structure->timer_update_callback;
    TIM_TimeBaseInitTypeDef timer1_init_structure;
    timer1_init_structure.TIM_ClockDivision = TIM_CKD_DIV1;
    timer1_init_structure.TIM_CounterMode = TIM_CounterMode_Up; // 上计数
    timer1_init_structure.TIM_Period = init_structure->period - 1; // 计数1000次
    timer1_init_structure.TIM_Prescaler = 72 - 1; // 分频72
    timer1_init_structure.TIM_RepetitionCounter = 0; // 重复一次
    TIM_TimeBaseInit(TIM1, &timer1_init_structure);
    // 开启ARR寄存器预加载
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_GenerateEvent(TIM1, TIM_EventSource_Update); // 手动产生update事件

    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); // 开启update事件

    NVIC_InitTypeDef nvic_init_structure;
    nvic_init_structure.NVIC_IRQChannel = TIM1_UP_IRQn;
    nvic_init_structure.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init_structure.NVIC_IRQChannelSubPriority = 0;
    nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init_structure);

    TIM_Cmd(TIM1, ENABLE);
    timer1_init_flag = 1;
}


/**
 * @brief 使用对应的定时器产生pwm信号
 * 
 * @param period 0x0000 and 0xFFFF
 * @param duty 占空比
 * @param TIMx 定时器
 */
void Xww_Timer_Generate_PWM_By_Oc1(u16 period, float duty, TIM_TypeDef* TIMx) {
    if (TIMx == TIM1) {
        TimerInitStructure init_structure;
        init_structure.timer_update_callback = 0;
        init_structure.period = period;
        Xww_Timer1_Init(&init_structure);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // TIM1_CH1 PA8
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        TIM_CtrlPWMOutputs(TIMx, ENABLE); // 闭合输出总开关 高级定时器的专用功能
    } else if (TIMx == TIM2) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
        TIM_TimeBaseInitTypeDef tim_base_init_structure;
        tim_base_init_structure.TIM_ClockDivision = TIM_CKD_DIV1;
        tim_base_init_structure.TIM_Prescaler = 71; // APB1总线最大频率为36MHZ 加上倍频 正好是72MHZ
        tim_base_init_structure.TIM_CounterMode = TIM_CounterMode_Up; // 上计数模式
        tim_base_init_structure.TIM_Period = period - 1; // 定时周期为period
        TIM_TimeBaseInit(TIM2, &tim_base_init_structure);

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

        GPIO_InitTypeDef gpio_A_init_structure;
        gpio_A_init_structure.GPIO_Pin = GPIO_Pin_0; // 通道一默认使用PA0
        gpio_A_init_structure.GPIO_Mode = GPIO_Mode_AF_PP;
        gpio_A_init_structure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_Init(GPIOA, &gpio_A_init_structure);
    } else if (TIMx == TIM4) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
        TIM_TimeBaseInitTypeDef tim_base_init_structure;
        tim_base_init_structure.TIM_ClockDivision = TIM_CKD_DIV1;
        tim_base_init_structure.TIM_CounterMode = TIM_CounterMode_Up;
        tim_base_init_structure.TIM_Period = period - 1; // 定时周期为period
        tim_base_init_structure.TIM_Prescaler = 72 - 1;
        TIM_TimeBaseInit(TIM4, &tim_base_init_structure);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // TIM1_CH1 PA8
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; // 通道一默认使用PB6
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    TIM_ARRPreloadConfig(TIMx, ENABLE);
    TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
    TIM_OCInitTypeDef tim_oc_init_structure;
    tim_oc_init_structure.TIM_OCMode = TIM_OCMode_PWM1;
    tim_oc_init_structure.TIM_OCPolarity = TIM_OCPolarity_High;
    tim_oc_init_structure.TIM_OutputState = TIM_OutputState_Enable;
    tim_oc_init_structure.TIM_Pulse = (period - 1) * duty; // 根据占空比设置对应比较寄存器的值
    TIM_OC1Init(TIMx, &tim_oc_init_structure); // 默认使用一通道
    TIM_GenerateEvent(TIMx, TIM_EventSource_Update); // 更新对应寄存器的值
    TIM_Cmd(TIMx, ENABLE); // 开启定时器总开关
}

/**
 * @brief 默认设置通道1的ccr寄存器的值
 * 
 * @param period 
 * @param duty 
 * @param TIMx 
 */
void Xww_Timer_Set_PWM_Duty_By_Oc1(u16 period, float duty, TIM_TypeDef* TIMx) {
    TIM_SetCompare1(TIMx, (period - 1) * duty);
}

/**
 * @brief 根据设定的最大计数值，以及目标的等效电压和高电平的电压计算对应的比较寄存器的值
 * 
 * @param max_val 
 * @param target_v 
 * @param source_v 
 * @return u8 
 */
u16 calculate_pwm(u16 max_val, float target_v, float source_v) {
    return (u16)(max_val * (target_v / source_v));
}

uint64_t get_tick(){
    return current_s * 1000 + tick_count;
}

void TIM1_UP_IRQHandler(void) {
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update); // 清除update中断标志位
    tick_count++;
    if (tick_count == 1000) {
        tick_count = 0;
        current_s += 1;
    }
    if (timer1_update_callback != 0) {
        timer1_update_callback();
    }
}
