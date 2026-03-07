#include "car_encoder_test.h"

static uint32_t last_show_angle_time = 0;

void car_encoder_test_proc(void) {
    // 测试编码器
    uint32_t current_time = get_current_ms();
    if (current_time - last_show_angle_time > SHOW_ANGLE_INTERVAL) {
        printf("%f,%f\r\n", car_encoder_get_L_angle(), car_encoder_get_R_angle());
        last_show_angle_time = current_time;
    } else if (current_time < last_show_angle_time) { // 处理计时器溢出的情况
        last_show_angle_time = current_time;
    }
}
