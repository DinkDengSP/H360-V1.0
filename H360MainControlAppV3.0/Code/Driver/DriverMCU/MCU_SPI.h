/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 08:54:27
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-01-19 08:53:47
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_SPI_H_
#define __MCU_SPI_H_
#include "MCU_Common.h"

//SPI速度设定
typedef enum MCU_SPI_CLK
{
    SPI_BAUDRATEPRESCALER_100M      = SPI_BAUDRATEPRESCALER_2,/* 100M */
    SPI_BAUDRATEPRESCALER_50M       = SPI_BAUDRATEPRESCALER_4,/* 50M */
    SPI_BAUDRATEPRESCALER_25M       = SPI_BAUDRATEPRESCALER_8,/* 25M */
    SPI_BAUDRATEPRESCALER_12_5M     = SPI_BAUDRATEPRESCALER_16,/* 12.5M */
    SPI_BAUDRATEPRESCALER_6_25M     = SPI_BAUDRATEPRESCALER_32,/* 6.5M */
    SPI_BAUDRATEPRESCALER_3_125M    = SPI_BAUDRATEPRESCALER_64,/* 3.125M */
    SPI_BAUDRATEPRESCALER_1_5625M   = SPI_BAUDRATEPRESCALER_128,/* 1.562M */
    SPI_BAUDRATEPRESCALER_781_25K   = SPI_BAUDRATEPRESCALER_256,/* 781.25K */
}MCU_SPI_CLK;

//SPI信号极性
typedef enum MCU_SPI_CPOL
{
    MCU_SPI_CPOL_HIGH = SPI_POLARITY_HIGH,
    MCU_SPI_CPOL_LOW = SPI_POLARITY_LOW,
}MCU_SPI_CPOL;

//SPI采样点设置
typedef enum MCU_SPI_CPHA
{
    MCU_SPI_CPHA_1EDGE = SPI_PHASE_1EDGE,
    MCU_SPI_CPHA_2EDGE = SPI_PHASE_2EDGE,
}MCU_SPI_CPHA;

//SPI1互斥信号量
extern OS_MUTEX mutexMCU_Spi1;
//SPI1初始化
void MCU_SPI1_Init(MCU_SPI_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha);
//SPI1设定传输速度
void MCU_SPI1_SetSpeed(MCU_SPI_CLK speed);
//SPI1读写数据
uint8_t MCU_SPI1_ReadWriteByte(uint8_t dat);

//SPI2互斥信号量
extern OS_MUTEX mutexMCU_Spi2;
//SPI2初始化
void MCU_SPI2_Init(MCU_SPI_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha);
//SPI2设定传输速度
void MCU_SPI2_SetSpeed(MCU_SPI_CLK speed);
//SPI2读写数据
uint8_t MCU_SPI2_ReadWriteByte(uint8_t dat);


#endif
