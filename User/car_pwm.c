#include "car_pwm.h"

static void Motor_STBY_Init(void) { 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void Motor_L_Init(void) { 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 初始化电机方向控制的引脚
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(GPIOA, &GPIO_InitStructure);
    Xww_Timer_Generate_PWM_By_Oc1(1000, 0, TIM1); // 默认占空比0%
}

static void Motor_R_Init(void) { 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 初始化电机方向控制的引脚
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);

    Xww_Timer_Generate_PWM_By_Oc1(1000, 0, TIM4); // 默认占空比0%
}

/**
 * @brief 设置左电机占空比
 * @param duty 占空比 -1.0 ~ 1.0  负数表示反转，正数表示正转，0表示停止
 */
void car_pwm_set_L(float duty) {
    if (duty < 0.0f) {
       GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_SET); // 反转 因为是对称的所以需要相反
       GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_RESET);
       duty = -duty; // 取绝对值
    } else {
       GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_RESET); // 正转
       GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_SET);
    }
    Xww_Timer_Set_PWM_Duty_By_Oc1(1000, duty, TIM1);
}

/**
 * @brief 设置右电机占空比
 * @param duty 占空比 -1.0 ~ 1.0  负数表示反转，正数表示正转，0表示停止
 */
void car_pwm_set_R(float duty) {
    if (duty < 0.0f) {
       GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET); // 反转
       GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
       duty = -duty; // 取绝对值
    } else {
       GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET); // 正转
       GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);
    }
    Xww_Timer_Set_PWM_Duty_By_Oc1(1000, duty, TIM4);
}

void car_pwm_init(void) { 
    Motor_STBY_Init();
    Motor_L_Init();
    Motor_R_Init();
}

void car_pwm_proc(void) { 
}

/**
 * @brief 电机的休眠开关
 * 
 * @param cmd 0 休眠 1 唤醒
 */
void car_pwm_cmd(uint8_t cmd) {
    if (cmd == 0) {
        GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    } else if (cmd == 1) { 
        GPIO_SetBits(GPIOA, GPIO_Pin_1);
    }
}
