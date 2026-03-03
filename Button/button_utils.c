#include "button_utils.h"

void check_button(struct Button* button) {
    if (GPIO_ReadInputDataBit(button->gpio_port, button->GPIO_Pin) == button->pressed_value) {
        if (button->pressed_count < 4) {
            button->pressed_count++;
        } else {
            button->released_count = 0;
            button->last_state = button->current_state;
            button->current_state = PRESSED;
            if (button->pressed_count <= 5) {
                button->pressed_count++; // 继续计数 5 次代表刚刚按下 6次代表按下了一段时间
            }
            
        }
    } else {
        if (button->released_count < 4) {
            button->released_count++;
        } else {
            button->pressed_count = 0;
            button->last_state = button->current_state;
            button->current_state = RELEASED;
            if (button->released_count <= 5) {
                button->released_count++; // 继续计数 5 次代表刚刚松开 6次代表松开了一段时间
            }
        }
    }
}

enum ButtonState get_button_state(struct Button* button) {
    // 读取按键状态
    if (button->current_state == PRESSED) {
        if (button->last_state == RELEASED && button->pressed_count == 5) {
            // 检测到按键按下
            return JUST_PRESSED;
        } else {
            return PRESSED;
        }
    } else {
        if (button->last_state == PRESSED && button->released_count == 5) {
            return JUST_RELEASED;
        } else {
            return RELEASED;
        }
    }
}
