#include "car_usart2.h"

volatile u8 receive_buff[50] = {0};
volatile u8 receive_idx = 0;
void send_string(char* str, u8 len) {
	while (len--) {
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // 等待发送缓冲区空
		USART_SendData(USART2, *str);
		str++;
	}
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); // 等待发送完成
}

void usart2_init(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef gpioa_init_structure;
	gpioa_init_structure.GPIO_Pin = GPIO_Pin_2;
	gpioa_init_structure.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioa_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioa_init_structure);

	gpioa_init_structure.GPIO_Pin = GPIO_Pin_3;
	gpioa_init_structure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &gpioa_init_structure);

	USART_InitTypeDef usart2_init_structure;
	usart2_init_structure.USART_BaudRate = 115200; 
	usart2_init_structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart2_init_structure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart2_init_structure.USART_StopBits = USART_StopBits_1;
	usart2_init_structure.USART_Parity = USART_Parity_No;
	usart2_init_structure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &usart2_init_structure);
	GPIO_PinRemapConfig(GPIO_Remap_USART2, DISABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); // 开启IDLE中断

	NVIC_InitTypeDef nvic_init_structure;
	nvic_init_structure.NVIC_IRQChannel = USART2_IRQn;
	nvic_init_structure.NVIC_IRQChannelPreemptionPriority = 2;
	nvic_init_structure.NVIC_IRQChannelSubPriority = 0;
	nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_init_structure);

	USART_Cmd(USART2, ENABLE);
}

int fputc(int ch, FILE * file) {
	char tmp = (char) ch;
	send_string(&tmp, 1);
	return ch;
}

void USART2_IRQHandler(void) {
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) {
		receive_buff[receive_idx] = USART_ReceiveData(USART2);
		receive_idx ++;
	}
	if (USART_GetITStatus(USART2, USART_IT_IDLE) == SET) { // 接收到IDLE中断 说明数据接收完毕
		USART_ClearITPendingBit(USART2, USART_IT_IDLE); // 清除IDLE中断 
		send_string((char *) receive_buff, receive_idx);
		receive_idx = 0;
	}
}
