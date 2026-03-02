#include "car_usart2.h"

u8 send_buff[100] = {0};
u8 receive_buff[100] = {0};
u8 receive_idx = 0;
u8 end_idx = 0;
void send_string(char* str, u8 len) {
	while (len--) {
		send_buff[end_idx] = *str;
		end_idx += 1;
		str++;
	}
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE); // 开启发送中断
}

u8 get_current_send_data() {
	u8 res = send_buff[0];

	for (u8 i = 0; i < end_idx - 1; i++) {
		send_buff[i] = send_buff[i + 1];
	}

	end_idx--;
	return res;
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
	usart2_init_structure.USART_BaudRate = 9600; 
	usart2_init_structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart2_init_structure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart2_init_structure.USART_StopBits = USART_StopBits_1;
	usart2_init_structure.USART_Parity = USART_Parity_No;
	usart2_init_structure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &usart2_init_structure);
	GPIO_PinRemapConfig(GPIO_Remap_USART2, DISABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 

	NVIC_InitTypeDef nvic_init_structure;
	nvic_init_structure.NVIC_IRQChannel = USART2_IRQn;
	nvic_init_structure.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_init_structure.NVIC_IRQChannelSubPriority = 0;
	nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_init_structure);

	USART_Cmd(USART2, ENABLE);
}

int fputc(int ch, FILE * file) {
	char tmp = (char) ch;
	send_string(&tmp, 1);
	while (USART_GetFlagStatus(USART2, USART_IT_TXE));
	return ch;
}

void USART2_IRQHandler(void) {
	if (USART_GetITStatus(USART2, USART_IT_TXE) == SET) { // 如果发送寄存器为空
		if (end_idx == 0) {
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
			return;
		}
		USART_SendData(USART2, get_current_send_data());
		USART_ClearITPendingBit(USART2, USART_IT_TXE);
	}

	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) {
		receive_buff[receive_idx] = USART_ReceiveData(USART2);
		receive_idx ++;
		if (receive_idx > 2 && receive_buff[receive_idx - 2] == '\r' && receive_buff[receive_idx - 1] == '\n') { // 说明接收数据结束
			send_string((char *) receive_buff, receive_idx - 2);
			receive_idx = 0;
		}
	}
}
