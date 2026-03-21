#include "IIC.h"

ErrorStatus IIC_Send_Bytes(I2C_TypeDef* IICx, u8 address, u8 * _bytes, u8 len) {
	ErrorStatus res = SUCCESS;
	while (I2C_GetFlagStatus(IICx, I2C_FLAG_BUSY) == SET); // 等待总线空闲
	
	I2C_ClearFlag(IICx, I2C_FLAG_AF); // 清楚错误标记位
	I2C_GenerateSTART(IICx, ENABLE); // 产生起始信号
	
	while (I2C_GetFlagStatus(IICx, I2C_FLAG_SB) == RESET); // 等待起始信号发送完毕

	I2C_Send7bitAddress(IICx, address, I2C_Direction_Transmitter); // 发送地址

	while (I2C_GetFlagStatus(IICx, I2C_FLAG_ADDR) == RESET) { // 等待地址发送完毕 (注意是当接收到ack之后才会置位，nack不会置位)
		if (I2C_GetFlagStatus(IICx, I2C_FLAG_AF) == SET) { // 说明应答失败
			res = ERROR;
			goto Stop;
		}
	}

	I2C_ReadRegister(IICx, I2C_Register_SR1);
	I2C_ReadRegister(IICx, I2C_Register_SR2); // 清除addr标志位

	while (len--) { // 发送数据
		while (I2C_GetFlagStatus(IICx, I2C_FLAG_TXE) == RESET) { // (注意是当接收到ack之后才会置位，nack不会置位)
			if (I2C_GetFlagStatus(IICx, I2C_FLAG_AF) == SET) { // 说明应答失败
				res = ERROR;
				goto Stop;
			}
		}
		I2C_SendData(IICx, *_bytes); // 将数据写入到tdr中
		_bytes ++;
	}

	while (I2C_GetFlagStatus(IICx, I2C_FLAG_BTF) == RESET){ // 等待数据发送完毕
		if (I2C_GetFlagStatus(IICx, I2C_FLAG_AF) == SET) { // 说明应答失败
			res = ERROR;
			goto Stop;
		}
	}
	Stop: // 发送停止位
	I2C_GenerateSTOP(IICx, ENABLE);
	while (I2C_GetFlagStatus(IICx, I2C_FLAG_BUSY) == SET); // 等待总线空闲
	return res;
}

ErrorStatus IIC_Receive_Bytes(I2C_TypeDef* IICx, u8 address, u8* target_buf, u8 len) {
	ErrorStatus res = SUCCESS;
	while (I2C_GetFlagStatus(IICx, I2C_FLAG_BUSY) == SET); // 等待总线空闲
	I2C_ClearFlag(IICx, I2C_FLAG_AF); // 清楚错误标记位
	I2C_GenerateSTART(IICx, ENABLE); // 产生起始信号
	while (I2C_GetFlagStatus(IICx, I2C_FLAG_SB) == RESET); // 等待起始信号发送完成
	I2C_Send7bitAddress(IICx, address, I2C_Direction_Receiver); // 发送读取数据的地址

	while (I2C_GetFlagStatus(IICx, I2C_FLAG_ADDR) == RESET) {
		if (I2C_GetFlagStatus(IICx, I2C_FLAG_AF) == SET) { // 没有应答
			res = ERROR;
			goto Stop;
		}	
	}

	if (len == 1) {
		I2C_AcknowledgeConfig(IICx, DISABLE); // 提前准备数据到达之后需要发送ack还是nack
		I2C_ReadRegister(IICx, I2C_Register_SR1);
		I2C_ReadRegister(IICx, I2C_Register_SR2); // 清除addr标志位
		while (I2C_GetFlagStatus(IICx, I2C_FLAG_RXNE) == RESET);//等待发送寄存器非空
		I2C_GenerateSTOP(IICx, ENABLE);  // 发送停止信号
		*target_buf = I2C_ReceiveData(IICx);
		while (I2C_GetFlagStatus(IICx, I2C_FLAG_BUSY) == SET);
		return res;
	} else if (len == 2) {
		I2C_AcknowledgeConfig(IICx, ENABLE); // 提前准备数据到达之后需要发送ack还是nack
		I2C_ReadRegister(IICx, I2C_Register_SR1);
		I2C_ReadRegister(IICx, I2C_Register_SR2); // 清除addr标志位
		while (I2C_GetFlagStatus(IICx, I2C_FLAG_RXNE) == RESET);//等待发送寄存器非空
		*target_buf = I2C_ReceiveData(IICx);
		target_buf ++;
		I2C_AcknowledgeConfig(IICx, DISABLE); // 提前准备数据到达之后需要发送ack还是nack
		while (I2C_GetFlagStatus(IICx, I2C_FLAG_RXNE) == RESET);//等待发送寄存器非空
		I2C_GenerateSTOP(IICx, ENABLE);  // 发送停止信号
		*target_buf = I2C_ReceiveData(IICx);
		while (I2C_GetFlagStatus(IICx, I2C_FLAG_BUSY) == SET);
		return res;
	} else {
		I2C_AcknowledgeConfig(IICx, ENABLE); // 提前准备第一个数据到达之后需要发送ack还是nack
		I2C_ReadRegister(IICx, I2C_Register_SR1);
		I2C_ReadRegister(IICx, I2C_Register_SR2); // 清除addr标志位
		while (len) {
			I2C_AcknowledgeConfig(IICx, ENABLE); // 提前准备数据到达之后需要发送ack还是nack
			if (len == 2) {
				while (I2C_GetFlagStatus(IICx, I2C_FLAG_RXNE) == RESET); //等待倒数第二个数据到达
				*target_buf = I2C_ReceiveData(IICx);
				target_buf ++;
				I2C_AcknowledgeConfig(IICx, DISABLE); // 提前准备数据到达之后需要发送ack还是nack
				while (I2C_GetFlagStatus(IICx, I2C_FLAG_RXNE) == RESET);//等待发送寄存器非空
				I2C_GenerateSTOP(IICx, ENABLE);  // 发送停止信号
				*target_buf = I2C_ReceiveData(IICx);
				while (I2C_GetFlagStatus(IICx, I2C_FLAG_BUSY) == SET);
				return res;
			}
			while (I2C_GetFlagStatus(IICx, I2C_FLAG_RXNE) == RESET);//等待发送寄存器非空
			*target_buf = I2C_ReceiveData(IICx);
			target_buf ++;
			len --;
		}
	}
	

	Stop:
	I2C_GenerateSTOP(IICx, ENABLE);
	while (I2C_GetFlagStatus(IICx, I2C_FLAG_BUSY) == SET); // 等待总线空闲
	return res;
}

void IIC_1_Init(FunctionalState remap_status) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 开启AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE); // 复位I2C1外设
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
	GPIO_PinRemapConfig(GPIO_Remap_I2C1, remap_status); // 配置引脚重映射
	GPIO_InitTypeDef gpiob_init_structure;
	gpiob_init_structure.GPIO_Mode = GPIO_Mode_AF_OD; // 复用开漏模式
	gpiob_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
	if (remap_status == ENABLE) {	
		gpiob_init_structure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; // 重映射的引脚
	} else {
		gpiob_init_structure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // 默认引脚
	}
	GPIO_Init(GPIOB, &gpiob_init_structure);
	I2C_InitTypeDef i2c1_init_structure;
	i2c1_init_structure.I2C_ClockSpeed = 400000; // 最高400khz
	i2c1_init_structure.I2C_DutyCycle = I2C_DutyCycle_2;
	i2c1_init_structure.I2C_Mode = I2C_Mode_I2C;
	I2C_Init(I2C1, &i2c1_init_structure);
	I2C_Cmd(I2C1, ENABLE);
}
