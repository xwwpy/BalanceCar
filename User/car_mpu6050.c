#include "car_mpu6050.h"

static float ax, ay, az; // 加速度计 单位g
static float tempature; // 温度计 单位摄氏度
static float gx, gy, gz; // 陀螺仪 单位°/s
static float yaw, pitch, roll; // 欧拉角 单位度
static uint64_t start_measure_time = 0;

static void reg_write(uint8_t reg, uint8_t data);
static uint8_t reg_read(uint8_t reg);
void Car_mpu6050_init(void) {
    IIC_1_Init(ENABLE); // IIC1初始化
    // 设置mpu6050的参数
    reg_write(0x6B, 0x80); // 复位
    Delay_ms(100); // 等待复位完成
    reg_write(0x6B, 0x00); // 关闭睡眠模式
    reg_write(0x1B, 0x18); // 设置陀螺仪的量程为2000度/秒
    reg_write(0x1C, 0x00); // 设置加速计的量程为2G
}

void car_mpu6050_proc(void) {
    uint64_t current_time = get_current_ms();
    if (current_time - start_measure_time > CAR_CALCAULATE_INTERVAL) {
        Car_mpu6050_update(); // 更新数据
        float yaw_g = yaw + gz * CAR_CALCAULATE_INTERVAL / 1000.0f; // 使用陀螺仪更新欧拉角 有偏移现象
        float pitch_g = pitch + gx * CAR_CALCAULATE_INTERVAL / 1000.0f;
        float roll_g = roll - gy * CAR_CALCAULATE_INTERVAL / 1000.0f;
        float yaw_a = 0.0f; // 使用加速度计更新欧拉角
        float pitch_a = atan2(ay, ax) * 180.0f / 3.1415927f;
        float roll_a = atan2(ax, az) * 180.0f / 3.1415927f;

        // 使用混合更新
        yaw = yaw_g * MPU6050_proportion_g + yaw_a * (1 - MPU6050_proportion_g);
        pitch = pitch_g * MPU6050_proportion_g + pitch_a * (1 - MPU6050_proportion_g);
        roll = roll_g * MPU6050_proportion_g + roll_a * (1 - MPU6050_proportion_g);

        start_measure_time = current_time;
    } else if (current_time < start_measure_time) { // 处理计时器溢出的情况
        start_measure_time = current_time;
    }
}

void Car_mpu6050_update(void) {
    int16_t ax_raw = (int16_t)((reg_read(0x3B) << 8) | reg_read(0x3C));
    int16_t ay_raw = (int16_t)((reg_read(0x3D) << 8) | reg_read(0x3E));
    int16_t az_raw = (int16_t)((reg_read(0x3F) << 8) | reg_read(0x40));

    ax = (float) ax_raw * 2.0f / 32768.0f; // 量程是-2g - +2g 读数从 -32768 到 +32767
    ay = (float) ay_raw * 2.0f / 32768.0f;
    az = (float) az_raw * 2.0f / 32768.0f;

    int16_t temperature_raw = (int16_t)((reg_read(0x41) << 8) + reg_read(0x42));
    
    tempature = (float) temperature_raw / 333.87f + 21.0f;

    int16_t gx_raw = (int16_t)((reg_read(0x43) << 8) + reg_read(0x44));
    int16_t gy_raw = (int16_t)((reg_read(0x45) << 8) + reg_read(0x46));
    int16_t gz_raw = (int16_t)((reg_read(0x47) << 8) + reg_read(0x48));

    gx = (float) gx_raw * 2000.0f / 32768.0f;
    gy = (float) gy_raw * 2000.0f / 32768.0f;
    gz = (float) gz_raw * 2000.0f / 32768.0f;
}

static void reg_write(uint8_t reg, uint8_t data) {
    uint8_t buff [2] = {reg, data};
    IIC_Send_Bytes(I2C1, CAR_MPU6050_IIC_ADDR, buff, 2);
}

static uint8_t reg_read(uint8_t reg) {
    uint8_t res;
    IIC_Send_Bytes(I2C1, CAR_MPU6050_IIC_ADDR, &reg, 1);
    IIC_Receive_Bytes(I2C1, CAR_MPU6050_IIC_ADDR, &res, 1);
    return res;
}

float Car_mpu6050_get_ax(void) {
    return ax;
}
float Car_mpu6050_get_ay(void) {
    return ay;
}
float Car_mpu6050_get_az(void) {
    return az;
}

float Car_mpu6050_get_gx(void) {
    return gx;
}
float Car_mpu6050_get_gy(void) {
    return gy;
}
float Car_mpu6050_get_gz(void) {
    return gz;
}

float Car_mpu6050_get_tempature(void) {
    return tempature;
}


Mpu6050Arc Car_mpu6050_getArc() {
    Mpu6050Arc arc;
    arc.pitch = pitch;
    arc.roll = roll;
    arc.yaw = yaw;
    return arc;
}
