#include "car_measure_method_t_test.h"

static uint64_t start_measure_time = 0;

void car_measure_method_t_test_proc(void) {
    // 测试编码器
    uint64_t current_time = get_current_ms();
    if (current_time - start_measure_time > T_MEASURE_SPEED_INTERVAL) {
        car_measure_method_t_print();
        start_measure_time = current_time;
    } else if (current_time < start_measure_time) { // 处理计时器溢出的情况
        start_measure_time = current_time;
    }
}


void car_measure_method_t_print(void) {
    float L_speed = car_encoder_get_L_speed();
    float R_speed = car_encoder_get_R_speed();
    printf("%.2f, %.2f\r\n", L_speed, R_speed);
}
