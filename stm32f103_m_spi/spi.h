#ifndef __STM32F103x_M_SPI_H__
#define __STM32F103x_M_SPI_H__
#include "stm32f10x.h"
void Xww_SPI1_Init(FunctionalState remap_state);
void Xww_SPI_MasterTransmitReceive(SPI_TypeDef* SPIx, const uint8_t *pDataTx, uint8_t* pDataRx, uint16_t size);
void Xww_SPI_Init(SPI_TypeDef* SPIx);
#endif
