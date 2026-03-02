#include "car_bat.h"
static volatile float v = 0.0f;
static uint32_t led_last_time = 0;
static uint8_t led_flag = 0; // 快慢闪的标志，0: 亮，1: 灭

static void TIM2_TRGO_Init(void);
static void ADC1_Init(void);
static void car_bat_show_led_init(void);
void car_bat_init() {
    // 初始化时基单元
    TIM2_TRGO_Init();
    // 初始化adc
    ADC1_Init();    
    // 初始化led
    car_bat_show_led_init();
}

float car_get_current_v() {
    return v;
}


void car_bat_show_led_init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // led1-3 分别对应 pa4-pa6
    GPIO_InitTypeDef gpioa_init_structure;
    gpioa_init_structure.GPIO_Mode = GPIO_Mode_Out_PP;
    gpioa_init_structure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    gpioa_init_structure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &gpioa_init_structure);
}

void car_bat_show_led_proc(void) {
    if (v > 7.9f) { // 电压大于7.9v 电量100%
        GPIO_WriteBit(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6, Bit_SET);
    } else if (v > 7.4f) { // 电压大于7.4v 电量75%
        GPIO_WriteBit(GPIOA, GPIO_Pin_4 | GPIO_Pin_5, Bit_SET);
        GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
    } else if (v > 7.0f) { // 电压大于7.0v 电量50%
        GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
        GPIO_WriteBit(GPIOA, GPIO_Pin_5 | GPIO_Pin_6, Bit_RESET);
    } else if (v > 6.5f) { // 电量20% 慢闪
        uint32_t current_time;
        current_time = get_current_ms();
        if (current_time - led_last_time > LED_SLOW_BLINK_TIME) { 
            if (led_flag == 0) {
                GPIO_WriteBit(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6, Bit_SET);
                led_flag = 1;
            } else { // 灭
                GPIO_WriteBit(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6, Bit_RESET);
                led_flag = 0;
            }
            led_last_time = current_time;
        } else if (current_time < led_last_time) { // 处理计时器溢出的情况
            led_last_time = current_time;
        }
    } else { // 电压小于6.5v 快闪
        uint32_t current_time;
        current_time = get_current_ms();
        if (current_time - led_last_time > LED_FAST_BLINK_TIME) { 
            if (led_flag == 0) {
                GPIO_WriteBit(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6, Bit_SET);
                led_flag = 1;
            } else { // 灭
                GPIO_WriteBit(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6, Bit_RESET);
                led_flag = 0;
            }
            led_last_time = current_time;
        } else if (current_time < led_last_time) { // 处理计时器溢出的情况
            led_last_time = current_time;
        }
    }
}

void ADC1_2_IRQHandler(void) {
    if (ADC_GetFlagStatus(ADC1, ADC_FLAG_JEOC) == SET) { // 采集完毕
        ADC_ClearFlag(ADC1, ADC_FLAG_JEOC);
        u16 val = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1);
        v = ((float) val / 4095.0f) * 8.4f; // 得到真实的电压
    }   
}


void ADC1_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    

    GPIO_InitTypeDef gpiob_init_structure; // 初始化引脚为模拟输入模式
    gpiob_init_structure.GPIO_Mode = GPIO_Mode_AIN;
    gpiob_init_structure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOB, &gpiob_init_structure);

    ADC_InitTypeDef adc1_init_structure;
    adc1_init_structure.ADC_ContinuousConvMode = DISABLE;
    adc1_init_structure.ADC_DataAlign = ADC_DataAlign_Right;
    adc1_init_structure.ADC_Mode = ADC_Mode_Independent;
    adc1_init_structure.ADC_ScanConvMode = DISABLE;
    ADC_InjectedSequencerLengthConfig(ADC1, 1);
    ADC_InjectedChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_7Cycles5);
    ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T2_TRGO);
    ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE);
    ADC_Init(ADC1, &adc1_init_structure);

    ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE); // 开启中断开关
    
    NVIC_InitTypeDef adc1_jeoc_nvic_init_structure;
    adc1_jeoc_nvic_init_structure.NVIC_IRQChannel = ADC1_2_IRQn;
    adc1_jeoc_nvic_init_structure.NVIC_IRQChannelPreemptionPriority = 0;
    adc1_jeoc_nvic_init_structure.NVIC_IRQChannelSubPriority = 0;
    adc1_jeoc_nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&adc1_jeoc_nvic_init_structure);

    ADC_Cmd(ADC1, ENABLE);
}


void TIM2_TRGO_Init(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 设置为6分频--12MHZ
    TIM_TimeBaseInitTypeDef timer2_init_structure;
    timer2_init_structure.TIM_ClockDivision = TIM_CKD_DIV1; // 不分频
    timer2_init_structure.TIM_CounterMode = TIM_CounterMode_Up; // 上计数模式
    timer2_init_structure.TIM_Period = 9999; // update事件的周期为10ms
    timer2_init_structure.TIM_Prescaler = 71;
    TIM_ARRPreloadConfig(TIM2, ENABLE); // 开启预加载
    TIM_TimeBaseInit(TIM2, &timer2_init_structure);
    TIM_GenerateEvent(TIM2, TIM_EventSource_Update); // 产生update事件
    TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable); // 开启主模式
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update); // 设置触发方式为update
    TIM_Cmd(TIM2, ENABLE); // 开启时钟
}
