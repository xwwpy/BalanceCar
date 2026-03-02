#ifndef __STM32F103x_M_IIC_H__ // stm32f103 m 系列IIC样板代码
#define __STM32F103x_M_IIC_H__
#include "stm32f10x.h"                  // Device header

void IIC_1_Init(FunctionalState remap_status);
ErrorStatus IIC_Receive_Bytes(I2C_TypeDef* IICx, u8 address, u8* target_buf, u8 len);
ErrorStatus IIC_Send_Bytes(I2C_TypeDef* IICx, u8 address, u8 * _bytes, u8 len);
#endif
