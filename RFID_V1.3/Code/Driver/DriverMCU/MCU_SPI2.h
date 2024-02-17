/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-11-13 14:25:41
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-01-06 10:17:12
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_SPI2_H_
#define __MCU_SPI2_H_
#include "CoreUtil.h"

/*****************************************SPI函数*******************************************/
//SPI模块的互斥信号量
extern  OS_MUTEX mutexMCU_SPI2;

//SPI 初始化
void MCU_SPI2_Init(void);

//写入数据
void MCU_SPI2_WriteByte(uint8_t writeDat);

//读出数据
uint8_t MCU_SPI2_ReadByte(void);

//读写MB85数据
uint8_t MCU_SPI2_ReadWriteByteMB85(uint8_t writeDat);

#endif




