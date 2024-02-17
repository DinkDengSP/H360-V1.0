
/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 09:30:41
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-01-06 09:02:46
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MEM_MANAGE_CONFIG_H_
#define __MEM_MANAGE_CONFIG_H_
#include "stm32h7xx.h"


//DTCM 最大容量128KB
#define BLOCK_SIZE_DTCM			        32//内存块大小默认为32字节
#define MAX_SIZE_DTCM			        35*1024//最大管理内存
#define ALLOC_TABLE_SIZE_DTCM	        (MAX_SIZE_DTCM/BLOCK_SIZE_DTCM) //内存表大小

//D1_AXI_SRAM 最大容量512KB
#define BLOCK_SIZE_AXI_SRAM			    32//内存块大小默认为32字节
#define MAX_SIZE_AXI_SRAM			    400*1024//最大管理内存
#define ALLOC_TABLE_SIZE_AXI_SRAM	    (MAX_SIZE_AXI_SRAM/BLOCK_SIZE_AXI_SRAM) //内存表大小

//D2_SRAM1 最大容量128KB
#define BLOCK_SIZE_D2_SRAM1			    32//内存块大小默认为32字节
#define MAX_SIZE_D2_SRAM1			    120*1024//最大管理内存
#define ALLOC_TABLE_SIZE_D2_SRAM1	    (MAX_SIZE_D2_SRAM1/BLOCK_SIZE_D2_SRAM1) //内存表大小

//D2_SRAM2 最大容量128KB
#define BLOCK_SIZE_D2_SRAM2			    32//内存块大小默认为32字节
#define MAX_SIZE_D2_SRAM2			    120*1024//最大管理内存
#define ALLOC_TABLE_SIZE_D2_SRAM2	    (MAX_SIZE_D2_SRAM2/BLOCK_SIZE_D2_SRAM2) //内存表大小

//D3_SRAM4 最大容量64KB
#define BLOCK_SIZE_D3_SRAM4			    32//内存块大小默认为32字节
#define MAX_SIZE_D3_SRAM4			    60*1024//最大管理内存
#define ALLOC_TABLE_SIZE_D3_SRAM4	    (MAX_SIZE_D3_SRAM4/BLOCK_SIZE_D3_SRAM4) //内存表大小

//MEN_IS62 1MB
#define BLOCK_SIZE_IS62			        32//内存块大小默认为32字节
#define MAX_SIZE_IS62			        963*1024//最大管理内存
#define ALLOC_TABLE_SIZE_IS62	        (MAX_SIZE_IS62/BLOCK_SIZE_IS62) //内存表大小





#endif





