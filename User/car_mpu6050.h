#ifndef __CAR_MPU6050_H__
#define __CAR_MPU6050_H__
#include "stm32f10x.h"
#include "IIC.h"
#include "Delay.h"
#define CAR_MPU6050_IIC_ADDR 0xD0
void Car_mpu6050_init(void);
void Car_mpu6050_update(void);
float Car_mpu6050_get_ax(void);
float Car_mpu6050_get_ay(void);
float Car_mpu6050_get_az(void);

float Car_mpu6050_get_gx(void);
float Car_mpu6050_get_gy(void);
float Car_mpu6050_get_gz(void);

float Car_mpu6050_get_tempature(void);
#endif // !__CAR_MPU6050_H__
