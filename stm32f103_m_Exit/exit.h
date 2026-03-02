#ifndef __STM32F103_M_EXIT_H__
#define __STM32F103_M_EXIT_H__
#include "stm32f10x.h"
typedef void (*EXTI_Callback)(void);
typedef struct {
    uint8_t GPIO_PortSource; // 选择当前通道映射的对应片上外设 selects the GPIO port to be used as source for EXTI lines. This parameter can be GPIO_PortSourceGPIOx where x can be (A..G).
    uint8_t GPIO_PinSource; // 哪一个IO口 GPIO_PinSourcex where x can be (0..15)
    uint8_t NVIC_IRQChannelPreemptionPriority;
    uint8_t NVIC_IRQChannelSubPriority;
    uint32_t EXTI_Line;
    EXTIMode_TypeDef EXTI_Mode;
    EXTITrigger_TypeDef EXTI_Trigger;
    EXTI_Callback callback;
} ExitFullInitTypedef;
void Xww_Exit_Init(ExitFullInitTypedef* init_structure);
#endif
