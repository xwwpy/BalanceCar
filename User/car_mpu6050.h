#ifndef __CAR_MPU6050_H__
#define __CAR_MPU6050_H__
#include "stm32f10x.h"
#include "IIC.h"
#include "Delay.h"
#include "timer.h"
#include "math.h"
#define CAR_MPU6050_IIC_ADDR 0xD0
#define CAR_CALCAULATE_INTERVAL 5
#define MPU6050_proportion_g 0.95238f
typedef struct {
    float pitch;
    float roll;
    float yaw;
} Mpu6050Arc;

void Car_mpu6050_init(void);
void car_mpu6050_proc(void);
void Car_mpu6050_update(void);
float Car_mpu6050_get_ax(void);
float Car_mpu6050_get_ay(void);
float Car_mpu6050_get_az(void);

float Car_mpu6050_get_gx(void);
float Car_mpu6050_get_gy(void);
float Car_mpu6050_get_gz(void);

float Car_mpu6050_get_tempature(void);
Mpu6050Arc Car_mpu6050_getArc(void);
#endif // !__CAR_MPU6050_H__
