#ifndef __BUTTON_UTILS_H__
#define __BUTTON_UTILS_H__
#include "stm32f10x.h"

enum ButtonState {
    JUST_PRESSED,
    PRESSED,
    RELEASED,
    JUST_RELEASED,
};
struct Button {
    u16 GPIO_Pin;
    GPIO_TypeDef* gpio_port;
    enum ButtonState current_state;
    enum ButtonState last_state;
    u8 pressed_count; // 防抖计数 默认检测4次如果都为0则认为按键被按下
    u8 released_count;
    u8 pressed_value; // 按下时的值 0或1
};

void check_button(struct Button* button);

enum ButtonState get_button_state(struct Button* button);


#endif // !__BUTTON_UTILS_H__
