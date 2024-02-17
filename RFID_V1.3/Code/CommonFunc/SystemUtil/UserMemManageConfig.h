/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-01-04 15:13:16
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-01-04 15:33:30
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __USER_MEM_MANAGE_CONFIG_H_
#define __USER_MEM_MANAGE_CONFIG_H_
#include "CoreUtil.h"

//内存管理分区
#define BLOCK_SIZE_MAIN			        32//内存块大小默认为32字节
#define MAX_SIZE_MAIN			        16*1024//最大管理内存
#define ALLOC_TABLE_SIZE_MAIN	        (MAX_SIZE_MAIN/BLOCK_SIZE_MAIN) //内存表大小


#endif





