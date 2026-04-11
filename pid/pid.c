#include "pid.h"
#include "car_usart2.h"

void PID_Init(PID_TypeDef *pid, float Kp, float Ki, float Kd, float max, float min) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->target = 0.0f;
    pid->integral = 0.0f;
    pid->max = max;
    pid->min = min;
    pid->last_error = 0.0f;
    pid->last_current = 0.0f;
    pid->last_time = get_current_us();
    pid->first_time_flag = 1;
}

void PID_Reset(PID_TypeDef *pid) {
    pid->integral = 0.0f;
    pid->last_error = 0.0f;
    pid->last_current = 0.0f;
    pid->last_time = get_current_us();
    pid->first_time_flag = 1;
}

float PID_Calc(PID_TypeDef *pid, float current, float delta) {
    if (delta <= 0.0f) {
        delta = 0.001f; // 设定一个最小delta值，防止除0
    }

    float err = pid->target - current;
    float err_dev = 0.0f;
    float err_int = pid->integral;

    // 非首轮计算则进行微分和积分
    if (!pid->first_time_flag) {
        // 微分项（使用测量值变化率，避免目标值突变带来的抖动）
        // d(Error)/dt = d(Target - Current)/dt. 如果Target不变，则 = -d(Current)/dt
        err_dev = -(current - pid->last_current) / delta;
        
        // 积分项（梯形积分）
        err_int += (err + pid->last_error) * delta * 0.5f;
    } else {
        pid->first_time_flag = 0;
    }

    float COp = pid->Kp * err;
    float COi = pid->Ki * err_int;
    float COd = pid->Kd * err_dev;
    float CO = COp + COi + COd;

    // 输出限幅
    if (CO > pid->max) {
        CO = pid->max;
        // 抗饱和：如果输出已达上限，且误差仍为正，则停止增加积分项
        if (err > 0.0f) err_int = pid->integral;
    } else if (CO < pid->min) {
        CO = pid->min;
        // 抗饱和：如果输出已达下限，且误差仍为负，则停止减少积分项
        if (err < 0.0f) err_int = pid->integral;
    }

    // 更新状态
    pid->last_error = err;
    pid->last_current = current;
    pid->integral = err_int;

    // 积分项限幅（防止积分过度回退，同时也作为二级保护）
    if (pid->integral > pid->max) pid->integral = pid->max;
    if (pid->integral < pid->min) pid->integral = pid->min;

    return CO;
}

void PID_SetTarget(PID_TypeDef *pid, float target) {
    // 仅更新目标值，不重置积分项，以保持当前的动力输出（解决“先减小再变大”的问题）
    if (pid->target != target) {
        pid->target = target;
        // 更新last_error，使得下一轮积分计算时误差的变化是基于新target的，
        // 从而避免积分项在target改变瞬间产生阶跃
        pid->last_error = pid->target - pid->last_current;
    }
}

float PID_Process(PID_TypeDef *pid, float current) {
    uint64_t now = get_current_us();
    uint64_t gap = now - pid->last_time;
    float delta = gap / 1.0e6f; // 将时间差转换为秒
    float output = PID_Calc(pid, current, delta);
    pid->last_time = now;
    return output;
}


