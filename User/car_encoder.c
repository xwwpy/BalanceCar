#include "car_encoder.h"
#include "car_usart2.h"

static volatile int64_t encoder_L_count = 0;
static volatile int64_t encoder_R_count = 0;

void Motor_L_callback(void); // 左轮编码器中断回调函数
void Motor_R_callback(void); // 右轮编码器中断回调函数

static void Encoder_L_Init(void) { 
    ExitFullInitTypedef init_structure;
    init_structure.EXTI_Line = EXTI_Line14;
    init_structure.GPIO_PinSource = GPIO_PinSource14;
    init_structure.GPIO_PortSource = GPIO_PortSourceGPIOB;
    init_structure.EXTI_Mode = EXTI_Mode_Interrupt;
    init_structure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    init_structure.NVIC_IRQChannelPreemptionPriority = 0;
    init_structure.NVIC_IRQChannelSubPriority = 0;
    init_structure.callback = Motor_L_callback;
    Xww_Exit_Init(&init_structure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void Encoder_R_Init(void) { 
    // PB3 PB4 修改默认的下载代码的功能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 使能AFIO时钟
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); // 禁用JTAG功能，释放PB3和PB4
    ExitFullInitTypedef init_structure;
    init_structure.EXTI_Line = EXTI_Line3;
    init_structure.GPIO_PinSource = GPIO_PinSource3;
    init_structure.GPIO_PortSource = GPIO_PortSourceGPIOB;
    init_structure.EXTI_Mode = EXTI_Mode_Interrupt;
    init_structure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    init_structure.NVIC_IRQChannelPreemptionPriority = 0;
    init_structure.NVIC_IRQChannelSubPriority = 0;
    init_structure.callback = Motor_R_callback;
    Xww_Exit_Init(&init_structure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void car_encoder_init(void) { 
    Encoder_L_Init();
    Encoder_R_Init();
}

void car_encoder_proc(void) { 

}

int64_t car_encoder_get_L(void) { 
    return encoder_L_count;
}

int64_t car_encoder_get_R(void) { 
    return encoder_R_count;
}

void Motor_L_callback(void) { 
    uint8_t gpiob_pin_14_state = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
    uint8_t gpiob_pin_15_state = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
    if (gpiob_pin_14_state == Bit_SET && gpiob_pin_15_state == Bit_RESET) { // 对称的电机所以相反的状态对应相同的转动方向
        encoder_L_count--;
    } else if (gpiob_pin_14_state == Bit_RESET && gpiob_pin_15_state == Bit_SET) {
        encoder_L_count--;
    } else if (gpiob_pin_14_state == Bit_RESET && gpiob_pin_15_state == Bit_RESET) {
        encoder_L_count++;
    } else if (gpiob_pin_14_state == Bit_SET && gpiob_pin_15_state == Bit_SET) {
        encoder_L_count++;
    }
}

void Motor_R_callback(void) { 
    uint8_t gpiob_pin_3_state = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3);
    uint8_t gpiob_pin_4_state = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4);
    if (gpiob_pin_3_state == Bit_SET && gpiob_pin_4_state == Bit_RESET) {
        encoder_R_count++;
    } else if (gpiob_pin_3_state == Bit_RESET && gpiob_pin_4_state == Bit_SET) {
        encoder_R_count++;
    } else if (gpiob_pin_3_state == Bit_RESET && gpiob_pin_4_state == Bit_RESET) {
        encoder_R_count--;
    }
    else if (gpiob_pin_3_state == Bit_SET && gpiob_pin_4_state == Bit_SET) {
        encoder_R_count--;
    }
}
