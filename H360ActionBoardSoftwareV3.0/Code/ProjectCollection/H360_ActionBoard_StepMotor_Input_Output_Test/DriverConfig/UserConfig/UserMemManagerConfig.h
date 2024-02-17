/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-10 16:59:11
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_StepMotor_Input_Output_Test\DriverConfig\UserConfig\UserMemManagerConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __USER_MEM_MANAGER_CONFIG_H_
#define __USER_MEM_MANAGER_CONFIG_H_
#include "stm32f4xx.h"

//SRAM IN 内存 系统总内存为112K 主要用在系统堆和此处,可以同时被CPU和DMA访问
//内存块大小为32字节
#define SRAM_IN_BLOCK_SIZE			32 
//最大管理内存
#define SRAM_IN_MAX_SIZE			65*1024 
//内存表大小
#define SRAM_IN_ALLOC_TABLE_SIZE	(SRAM_IN_MAX_SIZE/SRAM_IN_BLOCK_SIZE) 

//SRAM CCM内存 系统共64K CCM内存,主要用作系统栈和此处
//内存块大小为32字节
#define SRAM_CCM_BLOCK_SIZE			32 
//最大管理内存,系统栈也是存放在CCM空间的
#define SRAM_CCM_MAX_SIZE			24*1024 
//内存表大小
#define SRAM_CCM_ALLOC_TABLE_SIZE	(SRAM_CCM_MAX_SIZE/SRAM_CCM_BLOCK_SIZE) 

//内存块大小为32字节,系统一共1024K 外部内存,主要用作此处
#define SRAM_IS62_BLOCK_SIZE	    32  
//最大管理内存
#define SRAM_IS62_MAX_SIZE			963*1024
//内存表大小
#define SRAM_IS62_ALLOC_TABLE_SIZE	(SRAM_IS62_MAX_SIZE/SRAM_IS62_BLOCK_SIZE) 



//动态内存管理锁定
#define USER_MEM_LOCK()                 CPU_IntDis()

//动态内存管理解锁
#define USER_MEM_UNLOCK()               CPU_IntEn()

#endif




