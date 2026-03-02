#include "spi.h"
void Xww_SPI1_Init(FunctionalState remap_state) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 开启AFIO时钟用于配置重映射
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); // 开启SPI1的时钟
    GPIO_PinRemapConfig(GPIO_Remap_SPI1, remap_state); 
    if (remap_state == DISABLE) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        GPIO_InitTypeDef gpio_init_structure;
        gpio_init_structure.GPIO_Mode = GPIO_Mode_AF_PP;
        gpio_init_structure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
        gpio_init_structure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_Init(GPIOA, &gpio_init_structure);
        gpio_init_structure.GPIO_Pin = GPIO_Pin_6;
        gpio_init_structure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &gpio_init_structure);
    } else {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); // 不使用默认的调试接口
        GPIO_InitTypeDef gpio_init_structure;
        gpio_init_structure.GPIO_Mode = GPIO_Mode_Out_PP;
        gpio_init_structure.GPIO_Pin = GPIO_Pin_15; // 由于使用的是软NSS，所以将原本硬件NSS的IO口作为驱动从机的NSS
        gpio_init_structure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_Init(GPIOA, &gpio_init_structure);
        GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_SET); // 初始状态对应的nss为高电平
        gpio_init_structure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
        gpio_init_structure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &gpio_init_structure);
        gpio_init_structure.GPIO_Pin = GPIO_Pin_4;
        gpio_init_structure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOB, &gpio_init_structure);
    }
    Xww_SPI_Init(SPI1);
}


void Xww_SPI_Init(SPI_TypeDef* SPIx) {
    SPI_InitTypeDef spi_init_structure;
    spi_init_structure.SPI_Mode = SPI_Mode_Master; // 配置为主机模式
    spi_init_structure.SPI_DataSize = SPI_DataSize_8b; // 8bit
    spi_init_structure.SPI_CPOL = SPI_CPOL_Low; // 设置为低极性
    spi_init_structure.SPI_CPHA = SPI_CPHA_1Edge; // 设置为第一边沿
    spi_init_structure.SPI_FirstBit = SPI_FirstBit_MSB; // MSB高位优先，LSB低位优先
    spi_init_structure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // 双线全双工
    spi_init_structure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; 
    spi_init_structure.SPI_NSS = SPI_NSS_Soft; // 设置软件NSS
    SPI_Init(SPIx, &spi_init_structure);
    SPI_NSSInternalSoftwareConfig(SPIx, SPI_NSSInternalSoft_Set); // 主机模式设置为高电平
}

void Xww_SPI_MasterTransmitReceive(SPI_TypeDef* SPIx, const uint8_t *pDataTx, uint8_t* pDataRx, uint16_t size) {
    SPI_Cmd(SPIx, ENABLE);
    SPI_I2S_SendData(SPIx, pDataTx[0]); // 先将第一位数据写入到TDR寄存器中
    for (uint8_t i = 0; i < size - 1; i++) {
        while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET); // 等待TDR寄存器为空
        SPI_I2S_SendData(SPIx, pDataTx[i + 1]); // 先将第一位数据写入到TDR寄存器中
        while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET); // 等待数据接收完毕 同时这里的第i个数据也发送完毕
        pDataRx[i] = SPI_I2S_ReceiveData(SPIx); // 接收第i个数据
    }
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET); // 等待最后一个数据接收完毕 同时这里的第size个数据也发送完毕
    pDataRx[size - 1] = SPI_I2S_ReceiveData(SPIx);
    SPI_Cmd(SPIx, DISABLE);
}
