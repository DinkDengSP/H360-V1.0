/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-08-08 13:51:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-08-08 13:51:30 +0800
************************************************************************************************/ 
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
