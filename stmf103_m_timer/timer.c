#include "timer.h"

TimerCallBack timer1_update_callback;

u32 current_s = 0;

u16 tick_count = 0;

u32 get_current_s(void) {
    return current_s;
}

u32 get_current_ms(void) {
    return tick_count;
}

void Xww_Timer1_Init(TimerInitStructure* init_structure) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 配置nvic优先级分组
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    timer1_update_callback = init_structure->timer_update_callback;
    TIM_TimeBaseInitTypeDef timer1_init_structure;
    timer1_init_structure.TIM_ClockDivision = TIM_CKD_DIV1;
    timer1_init_structure.TIM_CounterMode = TIM_CounterMode_Up; // 上计数
    timer1_init_structure.TIM_Period = 1000 - 1; // 计数1000次
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
}


/**
 * @brief 使用对应的定时器产生pwm信号
 * 
 * @param period 0x0000 and 0xFFFF
 * @param duty 占空比
 * @param TIMx 定时器
 */
void Xww_Timer_Generate_PWM(u16 period, float duty, TIM_TypeDef* TIMx) {
    if (TIMx == TIM2) {
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
    TIM_CtrlPWMOutputs(TIMx, ENABLE); // 闭合输出总开关
    TIM_Cmd(TIMx, ENABLE); // 开启定时器总开关
}

/**
 * @brief 默认设置通道1的ccr寄存器的值
 * 
 * @param period 
 * @param duty 
 * @param TIMx 
 */
void Xww_Timer_Set_PWM_Duty(u16 period, float duty, TIM_TypeDef* TIMx) {
    TIM_SetCompare1(TIMx, (period - 1) * duty);
}

void Xww_Timer1_PWM_measure_init() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // TIM1通道1默认使用PA8
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); // 开启TIM1的时钟
    GPIO_InitTypeDef gpio_A_init_strucutre;
    gpio_A_init_strucutre.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio_A_init_strucutre.GPIO_Speed = GPIO_Speed_2MHz;
    gpio_A_init_strucutre.GPIO_Pin = GPIO_Pin_8;

    GPIO_Init(GPIOA, &gpio_A_init_strucutre);

    TIM_ARRPreloadConfig(TIM1, ENABLE); // 开启预加载

    TIM_TimeBaseInitTypeDef timer1_base_init_structure;
    timer1_base_init_structure.TIM_ClockDivision = TIM_CKD_DIV1;
    timer1_base_init_structure.TIM_CounterMode = TIM_CounterMode_Up;
    timer1_base_init_structure.TIM_Period = 65535; // 产生update的计数周期是65535 + 1 
    timer1_base_init_structure.TIM_Prescaler = 71; // 使用1MHZ的时钟周期
    timer1_base_init_structure.TIM_RepetitionCounter = 0; // 设置重复计数的次数为 0 + 1
    TIM_TimeBaseInit(TIM1, &timer1_base_init_structure);

    TIM_ICInitTypeDef timer_ic_init_structure;
    timer_ic_init_structure.TIM_Channel = TIM_Channel_1;
    timer_ic_init_structure.TIM_ICFilter = 2;
    timer_ic_init_structure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    timer_ic_init_structure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    timer_ic_init_structure.TIM_ICSelection = TIM_ICSelection_DirectTI;

    TIM_ICInit(TIM1, &timer_ic_init_structure); // 初始化输入捕获

    timer_ic_init_structure.TIM_Channel = TIM_Channel_2;
    timer_ic_init_structure.TIM_ICPolarity = TIM_ICPolarity_Falling;
    timer_ic_init_structure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
    TIM_ICInit(TIM1, &timer_ic_init_structure);

    TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
    TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);

    TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
    TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);

    NVIC_InitTypeDef nvic_init_structure;
    nvic_init_structure.NVIC_IRQChannel = TIM1_CC_IRQn;
    nvic_init_structure.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init_structure.NVIC_IRQChannelSubPriority = 0;
    nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&nvic_init_structure);

    TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1); // 选择通道1作为从模式输入
    TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset); // 选择相应的从模式
    TIM_GenerateEvent(TIM1, TIM_EventSource_Update);
    TIM_Cmd(TIM1, ENABLE);
}

void Xww_Timer1_Oc1_Init() {
    TIM_OCInitTypeDef tim1_oc_init_structure;
    tim1_oc_init_structure.TIM_OCMode = TIM_OCMode_PWM1; // 选择pwm1模式
    tim1_oc_init_structure.TIM_OutputState = TIM_OutputState_Enable; // 开启通道
    tim1_oc_init_structure.TIM_OutputNState = TIM_OutputNState_Enable; // 开启互补通道
    tim1_oc_init_structure.TIM_OCPolarity = TIM_OCPolarity_High;
    tim1_oc_init_structure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    tim1_oc_init_structure.TIM_Pulse = 0; // 设置ccr寄存器的值
    TIM_OC1Init(TIM1, &tim1_oc_init_structure);
    TIM_CtrlPWMOutputs(TIM1, ENABLE); // 开启输出通道的总开关
    TIM_CCPreloadControl(TIM1, ENABLE); // 开启预加载
    TIM_GenerateEvent(TIM1, TIM_EventSource_Update); // 手动产生update事件，来更新对应寄存器的值
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

u32 get_tick(){
    return current_s * 1000 + tick_count;
}

void TIM1_UP_IRQHandler(void) {
    tick_count++;
    if (tick_count == 1000) {
        tick_count = 0;
        current_s += 1;
    }
    if (timer1_update_callback != 0) {
        timer1_update_callback();
    }
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update); // 清除update中断标志位
}
