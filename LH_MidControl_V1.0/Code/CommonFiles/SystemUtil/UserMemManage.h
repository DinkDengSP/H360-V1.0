/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-07-28 15:18:02
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-05 20:26:02
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __USER_MEM_MANAGE_H_
#define __USER_MEM_MANAGE_H_
#include "MemManageConfig.h"
#include "CoreUtil.h"

//系统可用于内存分配的内存空间
typedef enum USER_MEM_REGION
{
    MEM_AXI_SRAM    = 0,
    MEM_D2_SRAM1    = 1,
    MEM_D2_SRAM2    = 2,
    MEM_D3_SRAM4    = 3,
    MEM_SDRAM1      = 4,
    MEM_SDRAM2      = 5,
}USER_MEM_REGION;

//内存管理初始化
void UserMemInit(USER_MEM_REGION region);

//申请内存
void* UserMemMalloc(USER_MEM_REGION region,uint32_t size);

//申请内存直到成功
void* UserMemMallocWhileSuccess(USER_MEM_REGION region,uint32_t size);

//释放内存
void UserMemFree(USER_MEM_REGION region,void* ptr);

//获取当前动态内存使用率
uint8_t UserMemPerUsed(USER_MEM_REGION region);

#endif
