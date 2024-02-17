/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 09:08:52
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-19 17:09:28
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_SRAM_H_
#define __BOARD_SRAM_H_
#include "MCU_Inc.h"

/**ISWV51216 512*16/2 1M字节*/
//对IS61LV25616/SRAMWV25616,地址线范围为A0~A17 
//对IS61LV51216/SRAMWV51216,地址线范围为A0~A18
//使用第四块 11 ->C
#define BOARD_SRAM_BASE_ADDR    ((uint32_t)(0x6C000000))
//1M容量
#define BOARD_SRAM_CAPACITY     (1024*1024)

LH_ERR BoardSRAM_Init(void);

LH_ERR BoardSRAM_ReadBuffer(uint32_t baseAddr,uint8_t* bufferPtr,uint32_t length);

LH_ERR BoardSRAM_WriteBuffer(uint32_t baseAddr,uint8_t* bufferPtr,uint32_t length);

LH_ERR BoardSRAM_SelfCheck(void);


#endif
