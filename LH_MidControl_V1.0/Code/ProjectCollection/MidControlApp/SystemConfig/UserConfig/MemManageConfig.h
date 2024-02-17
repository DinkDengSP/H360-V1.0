/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-07-27 18:27:23
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-05 20:25:39
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MEM_MANAGE_CONFIG_H_
#define __MEM_MANAGE_CONFIG_H_
#include "stm32h7xx.h"

//D1_AXI_SRAM 最大容量512KB
#define BLOCK_SIZE_AXI_SRAM			    32//内存块大小默认为32字节
#define MAX_SIZE_AXI_SRAM			    450*1024//最大管理内存
#define ALLOC_TABLE_SIZE_AXI_SRAM	    (MAX_SIZE_AXI_SRAM/BLOCK_SIZE_AXI_SRAM) //内存表大小

//D2_SRAM1 最大容量128KB
#define BLOCK_SIZE_D2_SRAM1			    32//内存块大小默认为32字节
#define MAX_SIZE_D2_SRAM1			    105*1024//最大管理内存
#define ALLOC_TABLE_SIZE_D2_SRAM1	    (MAX_SIZE_D2_SRAM1/BLOCK_SIZE_D2_SRAM1) //内存表大小

//D2_SRAM2 最大容量128KB
#define BLOCK_SIZE_D2_SRAM2			    32//内存块大小默认为32字节
#define MAX_SIZE_D2_SRAM2			    120*1024//最大管理内存
#define ALLOC_TABLE_SIZE_D2_SRAM2	    (MAX_SIZE_D2_SRAM2/BLOCK_SIZE_D2_SRAM2) //内存表大小

//D3_SRAM4 最大容量64KB
#define BLOCK_SIZE_D3_SRAM4			    32//内存块大小默认为32字节
#define MAX_SIZE_D3_SRAM4			    60*1024//最大管理内存
#define ALLOC_TABLE_SIZE_D3_SRAM4	    (MAX_SIZE_D3_SRAM4/BLOCK_SIZE_D3_SRAM4) //内存表大小

//SDRAM1内存
#define MAX_SIZE_SDRAM1			        32*1024*1024//最大管理内存

//SDRAM2内存
#define MAX_SIZE_SDRAM2			        16*1024*1024//最大管理内存

#define MAX_TEST_ITEM                   50000//最大测试数
#endif
