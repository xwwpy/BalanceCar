#include "car_measure_method_m_test.h"
static uint32_t start_measure_time = 0;
static float last_L_angle = 0.0f;
static float last_R_angle = 0.0f;
static uint8_t flag = 0;
void car_measure_method_m_test_proc(void) {
    if (flag == 0) {
        last_L_angle = car_encoder_get_L_angle();
        last_R_angle = car_encoder_get_R_angle();
        start_measure_time = get_current_ms();
        flag = 1;
        return;
    }
    // 测试编码器
    uint32_t current_time = get_current_ms();
    if (current_time - start_measure_time > MEASURE_SPEED_INTERVAL) {
        
        float L_angle = car_encoder_get_L_angle();
        float R_angle = car_encoder_get_R_angle();

        float L_speed = (L_angle - last_L_angle) / (MEASURE_SPEED_INTERVAL) * 1000.0f; // 计算速度，单位为度/秒
        float R_speed = (R_angle - last_R_angle) / (MEASURE_SPEED_INTERVAL) * 1000.0f; // 计算速度，单位为度/秒
        
        printf("%3.2f, %3.2f\r\n", L_speed / 360.0f, R_speed / 360.0f);

        flag = 0;
    } else if (current_time < start_measure_time) { // 处理计时器溢出的情况
        start_measure_time = current_time;
    }
}
