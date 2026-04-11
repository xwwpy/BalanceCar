#ifndef __PID_H__
#define __PID_H__
#include "stm32f10x.h"
#include "timer.h"

typedef struct {
    float Kp; // 比例增益
    float Ki; // 积分增益
    float Kd; // 微分增益
    float target; // 设定值
    float last_error; // 上一次误差值
    float last_current; // 上一次测量值
    float integral; // 误差积分值
    uint64_t last_time; // 上一次计算时间
    uint8_t first_time_flag; // 是否第一次计算
    float max; // 输出上限
    float min; // 输出下限
} PID_TypeDef;

/**
 * @brief PID初始化
 *
 * @param pid PID结构体指针
 * @param Kp 比例增益
 * @param Ki 积分增益
 * @param Kd 微分增益
 * @param max 输出上限
 * @param min 输出下限
 */
void PID_Init(PID_TypeDef *pid, float Kp, float Ki, float Kd, float max, float min);
/**
 * @brief 计算PID输出
 *
 * @param pid PID结构体指针
 * @param current 当前值
 * @param delta 时间差，单位秒
 * @return PID输出值
 */
float PID_Calc(PID_TypeDef *pid, float current, float delta);
/**
 * @brief 设置PID目标值
 *
 * @param pid PID结构体指针
 * @param target 目标值
 */
void PID_SetTarget(PID_TypeDef *pid, float target);

/**
 * @brief 自动计算PID输出（根据当前系统时间）
 *
 * @param pid PID结构体指针
 * @param current 当前值
 * @return PID输出值
 */
float PID_Process(PID_TypeDef *pid, float current);

/**
 * @brief 重置PID控制器（清除积分项和历史误差）
 *
 * @param pid PID结构体指针
 */
void PID_Reset(PID_TypeDef *pid);

#endif // !__PID_H__
