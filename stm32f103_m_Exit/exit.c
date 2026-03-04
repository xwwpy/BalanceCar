#include "exit.h"

EXTI_Callback exti0_callback = 0;
EXTI_Callback exti1_callback = 0;
EXTI_Callback exti2_callback = 0;
EXTI_Callback exti3_callback = 0;
EXTI_Callback exti4_callback = 0;
EXTI_Callback exti5_callback = 0;
EXTI_Callback exti6_callback = 0;
EXTI_Callback exti7_callback = 0;
EXTI_Callback exti8_callback = 0;
EXTI_Callback exti9_callback = 0;
EXTI_Callback exti10_callback = 0;
EXTI_Callback exti11_callback = 0;
EXTI_Callback exti12_callback = 0;
EXTI_Callback exti13_callback = 0;
EXTI_Callback exti14_callback = 0;
EXTI_Callback exti15_callback = 0;

void Xww_Exit_Init(ExitFullInitTypedef* init_structure) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //  开启AFIO时钟
    GPIO_InitTypeDef gpio_init_structure; // 初始化对应的gpio时钟
    gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
    switch (init_structure->GPIO_PinSource) {
        case GPIO_PinSource0:
            gpio_init_structure.GPIO_Pin = GPIO_Pin_0;
            break;
        case GPIO_PinSource1:
            gpio_init_structure.GPIO_Pin = GPIO_Pin_1;
            break;
        case GPIO_PinSource2:
            gpio_init_structure.GPIO_Pin = GPIO_Pin_2;
            break;
        case GPIO_PinSource3:
            gpio_init_structure.GPIO_Pin = GPIO_Pin_3;
            break;
        case GPIO_PinSource4:
            gpio_init_structure.GPIO_Pin = GPIO_Pin_4;
            break;
        case GPIO_PinSource5:
            gpio_init_structure.GPIO_Pin = GPIO_Pin_5;
            break;
        case GPIO_PinSource6:
            gpio_init_structure.GPIO_Pin = GPIO_Pin_6;
            break;
        case GPIO_PinSource7:
            gpio_init_structure.GPIO_Pin = GPIO_Pin_7;
            break;
        case GPIO_PinSource8:
            gpio_init_structure.GPIO_Pin = GPIO_Pin_8;
            break;
        case GPIO_PinSource9:
            gpio_init_structure.GPIO_Pin = GPIO_Pin_9;
            break;
        case GPIO_PinSource10:
            gpio_init_structure.GPIO_Pin = GPIO_Pin_10;
            break;
        case GPIO_PinSource11:
            gpio_init_structure.GPIO_Pin = GPIO_Pin_11;
            break;
        case GPIO_PinSource12:
            gpio_init_structure.GPIO_Pin = GPIO_Pin_12;
            break;
        case GPIO_PinSource13:
            gpio_init_structure.GPIO_Pin = GPIO_Pin_13;
            break;
        case GPIO_PinSource14:
            gpio_init_structure.GPIO_Pin = GPIO_Pin_14;
            break;
        case GPIO_PinSource15:
            gpio_init_structure.GPIO_Pin = GPIO_Pin_15;
            break;
        default:
            break;
    }
    switch (init_structure->EXTI_Trigger) {
        case EXTI_Trigger_Rising:
            gpio_init_structure.GPIO_Mode = GPIO_Mode_IPD; // 配置为下拉输入
            break;
        case EXTI_Trigger_Falling:
            gpio_init_structure.GPIO_Mode = GPIO_Mode_IPU; // 配置为上拉输入
            break;
        case EXTI_Trigger_Rising_Falling:
            gpio_init_structure.GPIO_Mode = GPIO_Mode_IPU; // 配置为上拉输入
            break;
        default:
            break;
    }
    switch (init_structure->GPIO_PortSource) {
        case GPIO_PortSourceGPIOA:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
            GPIO_Init(GPIOA, &gpio_init_structure);
            break;
        case GPIO_PortSourceGPIOB:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
            GPIO_Init(GPIOB, &gpio_init_structure);
            break;
        case GPIO_PortSourceGPIOC:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
            GPIO_Init(GPIOC, &gpio_init_structure);
            break;
        case GPIO_PortSourceGPIOD:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
            GPIO_Init(GPIOD, &gpio_init_structure);
            break;
        case GPIO_PortSourceGPIOE:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
            GPIO_Init(GPIOE, &gpio_init_structure);
            break;
        case GPIO_PortSourceGPIOF:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
            GPIO_Init(GPIOF, &gpio_init_structure);
            break;
        case GPIO_PortSourceGPIOG:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
            GPIO_Init(GPIOG, &gpio_init_structure);
            break;
        default:
            break;
    }
    GPIO_EXTILineConfig(init_structure->GPIO_PortSource, init_structure->GPIO_PinSource); // 配置exti的io重映射
    
    EXTI_InitTypeDef exti_init_structure;
    exti_init_structure.EXTI_Line = init_structure->EXTI_Line;
    exti_init_structure.EXTI_Mode = init_structure->EXTI_Mode;
    exti_init_structure.EXTI_Trigger = init_structure->EXTI_Trigger;
    exti_init_structure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti_init_structure); // 初始化exti的工作模式

    NVIC_InitTypeDef nvi_init_structure; // 初始化对应的exti通道的中断优先级
    nvi_init_structure.NVIC_IRQChannelPreemptionPriority = init_structure->NVIC_IRQChannelPreemptionPriority;
    nvi_init_structure.NVIC_IRQChannelSubPriority = init_structure->NVIC_IRQChannelSubPriority;
    nvi_init_structure.NVIC_IRQChannelCmd = ENABLE;
    switch (init_structure->EXTI_Line) {
        case EXTI_Line0:
            nvi_init_structure.NVIC_IRQChannel = EXTI0_IRQn;
            exti0_callback = init_structure->callback;
            break;
        case EXTI_Line1:
            nvi_init_structure.NVIC_IRQChannel = EXTI1_IRQn;
            exti1_callback = init_structure->callback;
            break;
        case EXTI_Line2:
            nvi_init_structure.NVIC_IRQChannel = EXTI2_IRQn;
            exti2_callback = init_structure->callback;
            break;
        case EXTI_Line3:
            nvi_init_structure.NVIC_IRQChannel = EXTI3_IRQn;
            exti3_callback = init_structure->callback;
            break;
        case EXTI_Line4:
            nvi_init_structure.NVIC_IRQChannel = EXTI4_IRQn;
            exti4_callback = init_structure->callback;
            break;
        case EXTI_Line5:
            nvi_init_structure.NVIC_IRQChannel = EXTI9_5_IRQn;
            exti5_callback = init_structure->callback;
            break;
        case EXTI_Line6:
            nvi_init_structure.NVIC_IRQChannel = EXTI9_5_IRQn;
            exti6_callback = init_structure->callback;
            break;
        case EXTI_Line7:
            nvi_init_structure.NVIC_IRQChannel = EXTI9_5_IRQn;
            exti7_callback = init_structure->callback;
            break;
        case EXTI_Line8:
            nvi_init_structure.NVIC_IRQChannel = EXTI9_5_IRQn;
            exti8_callback = init_structure->callback;
            break;
        case EXTI_Line9:
            nvi_init_structure.NVIC_IRQChannel = EXTI9_5_IRQn;
            exti9_callback = init_structure->callback;
            break;
        case EXTI_Line10:
            nvi_init_structure.NVIC_IRQChannel = EXTI15_10_IRQn;
            exti10_callback = init_structure->callback;
            break;
        case EXTI_Line11:
            nvi_init_structure.NVIC_IRQChannel = EXTI15_10_IRQn;
            exti11_callback = init_structure->callback;
            break;
        case EXTI_Line12:
            nvi_init_structure.NVIC_IRQChannel = EXTI15_10_IRQn;
            exti12_callback = init_structure->callback;
            break;
        case EXTI_Line13:
            nvi_init_structure.NVIC_IRQChannel = EXTI15_10_IRQn;
            exti13_callback = init_structure->callback;
            break;
        case EXTI_Line14:
            nvi_init_structure.NVIC_IRQChannel = EXTI15_10_IRQn;
            exti14_callback = init_structure->callback;
            break;
        case EXTI_Line15:
            nvi_init_structure.NVIC_IRQChannel = EXTI15_10_IRQn;
            exti15_callback = init_structure->callback;
            break;
        default:
            break;
    }

    NVIC_Init(&nvi_init_structure);
}


void EXTI0_IRQHandler(void) {
    EXTI_ClearITPendingBit(EXTI_Line0);
    if (exti0_callback != 0) {
        exti0_callback();
    }
}
void EXTI1_IRQHandler(void) {
    EXTI_ClearITPendingBit(EXTI_Line1);
    if (exti1_callback != 0) {
        exti1_callback();
    }
}
void EXTI2_IRQHandler(void) {
    EXTI_ClearITPendingBit(EXTI_Line2);
    if (exti2_callback != 0) {
        exti2_callback();
    }
}
void EXTI3_IRQHandler(void) {
    EXTI_ClearITPendingBit(EXTI_Line3);
    if (exti3_callback != 0) {
        exti3_callback();
    }
}
void EXTI4_IRQHandler(void) {
    EXTI_ClearITPendingBit(EXTI_Line4);
    if (exti4_callback != 0) {
        exti4_callback();
    }
}
void EXTI9_5_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line5) == SET) {
        EXTI_ClearITPendingBit(EXTI_Line5);
        if (exti5_callback != 0) {
            exti5_callback();
        }
    }
    else if (EXTI_GetITStatus(EXTI_Line6) == SET) {
        EXTI_ClearITPendingBit(EXTI_Line6);
        if (exti6_callback != 0) {
            exti6_callback();
        }
    }
    else if (EXTI_GetITStatus(EXTI_Line7) == SET) {
        EXTI_ClearITPendingBit(EXTI_Line7);
        if (exti7_callback != 0) {
            exti7_callback();
        }
    }
    else if (EXTI_GetITStatus(EXTI_Line8) == SET) {
        EXTI_ClearITPendingBit(EXTI_Line8);
        if (exti8_callback != 0) {
            exti8_callback();
        }
    }
    else if (EXTI_GetITStatus(EXTI_Line9) == SET) {
        EXTI_ClearITPendingBit(EXTI_Line9);
        if (exti9_callback != 0) {
            exti9_callback();
        }
    }
}
void EXTI15_10_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line10) == SET) {
        EXTI_ClearITPendingBit(EXTI_Line10);
        if (exti10_callback != 0) {
            exti10_callback();
        }
    }
    else if (EXTI_GetITStatus(EXTI_Line11) == SET) {
        EXTI_ClearITPendingBit(EXTI_Line11);
        if (exti11_callback != 0) {
            exti11_callback();
        }
    }
    else if (EXTI_GetITStatus(EXTI_Line12) == SET) {
        EXTI_ClearITPendingBit(EXTI_Line12);
        if (exti12_callback != 0) {
            exti12_callback();
        }
    }
    else if (EXTI_GetITStatus(EXTI_Line13) == SET) {
        EXTI_ClearITPendingBit(EXTI_Line13);
        if (exti13_callback != 0) {
            exti13_callback();
        }
    }
    else if (EXTI_GetITStatus(EXTI_Line14) == SET) {
        EXTI_ClearITPendingBit(EXTI_Line14);
        if (exti14_callback != 0) {
            exti14_callback();
        }
    }
    else if (EXTI_GetITStatus(EXTI_Line15) == SET) {
        EXTI_ClearITPendingBit(EXTI_Line15);
        if (exti15_callback != 0) {
            exti15_callback();
        }
    }
}
