#include "w25q64.h"

u8 default_SPI1_init_flag = 0;
void wait_util_not_busy(GPIO_TypeDef* gpiox, u16 gpio_pin_x, SPI_TypeDef * spix){
    while (1) {
        u8 buff[1];
        buff[0] = READ_STATUS_REGISTER_1_CMD;
        GPIO_WriteBit(gpiox, gpio_pin_x, Bit_RESET); // 使能NSS = 0
        Xww_SPI_MasterTransmitReceive(spix, buff, buff, 1); // 发送读取寄存器命令
        buff[0] = 0xFF; 
        Xww_SPI_MasterTransmitReceive(spix, buff, buff, 1); // 读取第一个字节
        GPIO_WriteBit(gpiox, gpio_pin_x, Bit_SET); // 关闭NSS
        if ((buff[0] & 0x01) == 0) break; // 等待知道对应寄存器的等待标志位被设置为0
    }
}

/**
 * @brief 像指定的扇出中存储数据
 * 
 * @param address 扇区的24位地址
 * @param pdata 数据
 * @param len 数据长度最大256
 * @param gpiox Nss连接到的GPIOx
 * @param gpio_pin_x Nss对应的引脚
 * @param spix 使用哪一个片上外设
 */
void Xww_W25Q64_SaveByte(u32 address, u8* pdata, u16 len, GPIO_TypeDef* gpiox, u16 gpio_pin_x, SPI_TypeDef * spix) {
    u8 buff[300];
    // 写使能
    buff[0] = WRITE_ENABLE_CMD;
    GPIO_WriteBit(gpiox, gpio_pin_x, Bit_RESET); // 使能NSS = 0
    Xww_SPI_MasterTransmitReceive(spix, buff, buff, 1); // 发送写使能命令
    GPIO_WriteBit(gpiox, gpio_pin_x, Bit_SET); // 关闭NSS

    // 扇出擦除
    buff[0] = SECTOR_ERASE_CMD; // 扇区擦除命令加24位地址
    buff[1] = address & (u8)0xFF;
    buff[2] = (address >> 8) & (u8)0xFF;
    buff[3] = (address >> 16) & (u8)0xFF;
    GPIO_WriteBit(gpiox, gpio_pin_x, Bit_RESET); // 使能NSS = 0
    Xww_SPI_MasterTransmitReceive(spix, buff, buff, 4); // 发送擦除命令
    GPIO_WriteBit(gpiox, gpio_pin_x, Bit_SET); // 关闭NSS

    // 等待空闲
    wait_util_not_busy(gpiox, gpio_pin_x, spix); // 等待操作结束

    // 写使能
    buff[0] = WRITE_ENABLE_CMD;
    GPIO_WriteBit(gpiox, gpio_pin_x, Bit_RESET); // 使能NSS = 0
    Xww_SPI_MasterTransmitReceive(spix, buff, buff, 1); // 发送写使能命令
    GPIO_WriteBit(gpiox, gpio_pin_x, Bit_SET); // 关闭NSS

    // 写数据
    buff[0] = PAGE_PROGRAM_CMD ; // 页编程命令加24位地址
    buff[1] = address & (u8)0xFF;
    buff[2] = (address >> 8) & (u8)0xFF;
    buff[3] = (address >> 16) & (u8)0xFF;

    for (u16 i = 0; i < len; i++) {
        buff[4 + i] = pdata[i];
    }
    
    GPIO_WriteBit(gpiox, gpio_pin_x, Bit_RESET); // 使能NSS = 0
    Xww_SPI_MasterTransmitReceive(spix, buff, buff, len + 4); // 写入数据
    GPIO_WriteBit(gpiox, gpio_pin_x, Bit_SET); // 关闭NSS

    // 等待空闲
    wait_util_not_busy(gpiox, gpio_pin_x, spix); // 等待操作结束
}

void Xww_W25Q64_SaveByteDefault(u8 _byte){
    if (default_SPI1_init_flag == 0) {
        Xww_SPI1_Init(ENABLE);
        default_SPI1_init_flag = 1;
    }
    Xww_W25Q64_SaveByte(0, &_byte, 1, GPIOA, GPIO_Pin_15, SPI1);
}

void Xww_W25Q64_LoadByte(u32 address, u8* pReadData, u16 len, GPIO_TypeDef* gpiox, u16 gpio_pin_x, SPI_TypeDef * spix) {
    u8 buff[300] = {0xFF};
    buff[0] = READ_DATA_CMD;
    buff[1] = address & (u8)0xFF;
    buff[2] = (address >> 8) & (u8)0xFF;
    buff[3] = (address >> 16) & (u8)0xFF;
    GPIO_WriteBit(gpiox, gpio_pin_x, Bit_RESET); // 使能NSS = 0
    Xww_SPI_MasterTransmitReceive(spix, buff, buff, 4); // 发送读取数据命令命令
    for (u16 i = 0; i < len; i++) {
        buff[i] = 0xFF;
    }
    
    Xww_SPI_MasterTransmitReceive(spix, buff, pReadData, len); // 读取字节
    GPIO_WriteBit(gpiox, gpio_pin_x, Bit_SET); // 关闭NSS
}

u8 Xww_W25Q64_LoadByteDefault(void){
    u8 buff[1];
    if (default_SPI1_init_flag == 0) {
        Xww_SPI1_Init(ENABLE);
        default_SPI1_init_flag = 1;
    }
    Xww_W25Q64_LoadByte(0, buff, 1, GPIOA, GPIO_Pin_15, SPI1);
    return buff[0];
}
