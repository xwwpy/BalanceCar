#include "car_button.h"

static struct Button userKeyButton = {0};

static void car_user_key_pressed_callback(void);

void car_button_init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
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
    if (car_pwm_get_cmd() == 0) {
        car_pwm_cmd(1);
    } else {
        car_pwm_cmd(0);
    }
}

