#ifndef __W25Q64_H__
#define __W25Q64_H__
#define WRITE_ENABLE_CMD 0x06 // 写使能命令
#define SECTOR_ERASE_CMD 0x20 // 扇区擦除命令
#define READ_STATUS_REGISTER_1_CMD 0x05 // 读取状态寄存器1命令
#define READ_STATUS_REGISTER_2_CMD 0x35 // 读取状态寄存器1命令
#define READ_STATUS_REGISTER_3_CMD 0x15 // 读取状态寄存器1命令
#define PAGE_PROGRAM_CMD 0x02 // 页编程命令
#define READ_DATA_CMD 0x03 // 读取数据指令
#include "stm32f10x.h"
#include "spi.h"
void Xww_W25Q64_SaveByte(u32 address, u8* pdata, u16 len, GPIO_TypeDef* gpiox, u16 gpio_pin_x, SPI_TypeDef * spix);
void Xww_W25Q64_SaveByteDefault(u8 _byte);
void Xww_W25Q64_LoadByte(u32 address, u8* pReadData, u16 len, GPIO_TypeDef* gpiox, u16 gpio_pin_x, SPI_TypeDef * spix);
u8 Xww_W25Q64_LoadByteDefault(void);
#endif
