#include "car_encoder.h"
#include "car_usart2.h"

static volatile int64_t encoder_L_count = 0;
static volatile int64_t encoder_R_count = 0;

static volatile int8_t direction_l = 1; // 电机转动方向 用于T测速方法 1 正转 -1 反转
static volatile int8_t direction_r = 1; // 电机转动方向 用于T测速方法 1 正转 -1 反转

static volatile uint64_t t0_l, t0_r;
static volatile uint64_t t1_l, t1_r;

static volatile uint32_t timeout = 100000; // 用于M测速方法的超时处理 100ms

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
    init_structure.NVIC_IRQChannelPreemptionPriority = 1;
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

float car_encoder_get_L_angle(void) { 
    return encoder_L_count / 449.0f * 360.0f;
}

float car_encoder_get_R_angle(void) { 
    return encoder_R_count / 449.0f * 360.0f;
}

/**
 * @brief 返回单位时间内的转速，单位为度/秒
 * 
 * @return float 
 */
float car_encoder_get_L_speed(void) {
     __disable_irq();
    int8_t direction_l_tmp = direction_l;
    uint64_t t0_l_tmp = t0_l;
    uint64_t t1_l_tmp = t1_l;
    __enable_irq();
    if (get_current_us() - t0_l_tmp > timeout) {
        return 0.0f;
    }
    return direction_l_tmp / ((t0_l_tmp - t1_l_tmp) * 1e-6f) * 360.0f / 449.0f;
}

/**
 * @brief 返回单位时间内的转速，单位为度/秒
 * 
 * @return float 
 */
float car_encoder_get_R_speed(void) {
    __disable_irq(); // 关闭中断，防止在读取过程中被中断打断导致数据不一致
    int8_t direction_r_tmp = direction_r;
    uint64_t t0_r_tmp = t0_r;
    uint64_t t1_r_tmp = t1_r;
    __enable_irq(); // 重新开启中断
    if (get_current_us() - t0_r_tmp > timeout) {
        return 0.0f;
    }
    return direction_r_tmp / ((t0_r_tmp - t1_r_tmp) * 1e-6f) * 360.0f / 449.0f;
}

void Motor_L_callback(void) {
    t1_l = t0_l;
    t0_l = get_current_us();
    uint8_t gpiob_pin_14_state = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
    uint8_t gpiob_pin_15_state = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
    if (gpiob_pin_14_state == Bit_SET && gpiob_pin_15_state == Bit_RESET) { // 对称的电机所以相反的状态对应相同的转动方向
        encoder_L_count--;
        direction_l = -1;
    } else if (gpiob_pin_14_state == Bit_RESET && gpiob_pin_15_state == Bit_SET) {
        encoder_L_count--;
        direction_l = -1;
    } else if (gpiob_pin_14_state == Bit_RESET && gpiob_pin_15_state == Bit_RESET) {
        encoder_L_count++;
        direction_l = 1;
    } else if (gpiob_pin_14_state == Bit_SET && gpiob_pin_15_state == Bit_SET) {
        encoder_L_count++;
        direction_l = 1;
    }
}

void Motor_R_callback(void) { 
    t1_r = t0_r;
    t0_r = get_current_us();
    uint8_t gpiob_pin_3_state = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3);
    uint8_t gpiob_pin_4_state = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4);
    if (gpiob_pin_3_state == Bit_SET && gpiob_pin_4_state == Bit_RESET) {
        encoder_R_count++;
        direction_r = 1;
    } else if (gpiob_pin_3_state == Bit_RESET && gpiob_pin_4_state == Bit_SET) {
        encoder_R_count++;
        direction_r = 1;
    } else if (gpiob_pin_3_state == Bit_RESET && gpiob_pin_4_state == Bit_RESET) {
        encoder_R_count--;
        direction_r = -1;
    } else if (gpiob_pin_3_state == Bit_SET && gpiob_pin_4_state == Bit_SET) {
        encoder_R_count--;
        direction_r = -1;
    }
}
