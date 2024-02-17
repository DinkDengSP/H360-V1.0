/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-03 14:59:04
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-04 13:12:38
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_SDRAM_H_
#define __BOARD_SDRAM_H_
#include "MCU_Inc.h"

//第一块SDRAM
#define BOARD_SDRAM1_BASE_ADDR    ((uint32_t)0xC0000000)
#define BOARD_SDRAM1_CAPACITY     (32 * 1024 * 1024)
//第二块SDRAM
#define BOARD_SDRAM2_BASE_ADDR    ((uint32_t)0xD0000000)
#define BOARD_SDRAM2_CAPACITY     (32 * 1024 * 1024)

//sdram初始化
void BoardSDRAM_Init(void);

//读取指定区域指定长度内存
void BoardSDRAM_ReadBuffer(uint32_t baseAddr,uint8_t* bufferPtr,uint32_t length);

//写入指定区域指定长度数据
void BoardSDRAM_WriteBuffer(uint32_t baseAddr,uint8_t* bufferPtr,uint32_t length);

//检查SDRAM数据完整性,自检时间6S
LH_ERR BoardSDRAM_SelfCheck(void);


#endif
