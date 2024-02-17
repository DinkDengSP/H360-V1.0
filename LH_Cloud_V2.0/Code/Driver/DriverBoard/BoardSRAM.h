/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-10-26 14:58:56
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-10-26 15:01:07
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
//对IS61LV25616/IS62WV25616,地址线范围为A0~A17 
//对IS61LV51216/IS62WV51216,地址线范围为A0~A18
//使用第3块 10 ->8
#define BOARD_SRAM_BASE_ADDR                ((uint32_t)(0x68000000))
//1M容量
#define BOARD_SRAM_CAPACITY                 (1024*1024)

//sram初始化
void BoardSRAM_Init(void);

//SRAM自检
LH_ERR BoardSRAM_SelfCheck(void);

//SRAM读取缓冲区
void BoardSRAM_ReadBuffer(uint32_t baseAddr,uint8_t* bufferPtr,uint32_t length);

//SRAM写入缓冲区
void BoardSRAM_WriteBuffer(uint32_t baseAddr,uint8_t* bufferPtr,uint32_t length);

#endif
