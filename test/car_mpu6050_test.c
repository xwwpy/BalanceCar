#include "car_mpu6050_test.h"
static uint64_t start_measure_time = 0;

void MPU6050_Test_proc(void) {
    uint64_t current_time = get_current_ms();
    if (current_time - start_measure_time > MPU6050_TEST_INTERVAL) {
        float ax = Car_mpu6050_get_ax();
        float ay = Car_mpu6050_get_ay();
        float az = Car_mpu6050_get_az();
        float tempature = Car_mpu6050_get_tempature();
        float gx = Car_mpu6050_get_gx();
        float gy = Car_mpu6050_get_gy();
        float gz = Car_mpu6050_get_gz();
        Mpu6050Arc arc = Car_mpu6050_getArc();
        printf("%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", ax, ay, az, tempature, gx, gy, gz, arc.yaw, arc.pitch, arc.roll);
        start_measure_time = current_time;
    } else if (current_time < start_measure_time) { // 处理计时器溢出的情况
        start_measure_time = current_time;
    }
}

void MPU6050_Test_YPR_proc(void) {
    uint64_t current_time = get_current_ms();
    if (current_time - start_measure_time > MPU6050_TEST_INTERVAL) {
        Mpu6050Arc arc = Car_mpu6050_getArc();
        printf("%f,%f,%f\n", arc.yaw, arc.pitch, arc.roll);
        start_measure_time = current_time;
    } else if (current_time < start_measure_time) { // 处理计时器溢出的情况
        start_measure_time = current_time;
    }
}
