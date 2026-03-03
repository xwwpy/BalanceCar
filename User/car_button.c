#include "car_button.h"

static struct Button userKeyButton = {0};

static void car_user_key_pressed_callback(void);

void car_button_init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    userKeyButton.GPIO_Pin = GPIO_Pin_11;
    userKeyButton.gpio_port = GPIOA;
    userKeyButton.last_state = RELEASED;
    userKeyButton.current_state = RELEASED;
    userKeyButton.pressed_count = 4;
    userKeyButton.released_count = 4;
    userKeyButton.pressed_value = 0;
}

void car_button_proc(void) {
    check_button(&userKeyButton);
    if (get_button_state(&userKeyButton) == JUST_PRESSED) {
        car_user_key_pressed_callback();
    }
}

static void car_user_key_pressed_callback(void) {
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == RESET) {
        GPIO_SetBits(GPIOA, GPIO_Pin_1);
    } else {
        GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    }
}

