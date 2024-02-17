/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 10:07:24 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-21 11:24:42 +0800
************************************************************************************************/ 
#ifndef __USER_MEM_MANAGE_H_
#define __USER_MEM_MANAGE_H_
#include "BoardDrvHeader.h"
#include "MemManageConfig.h"

//内存区域定义
typedef enum MEM_REGION
{
    MEM_AXI_SRAM    = 0X00,
    MEM_SDRAM1      = 0X01,
    MEM_SDRAM2      = 0X02,
}MEM_REGION;

//动态内存初始化
void UserMemInit(MEM_REGION locate);

//申请内存
void* UserMemMalloc(MEM_REGION locate,uint32_t size);

//释放内存
void UserMemFree(MEM_REGION locate,void* ptr);

//申请内存
void* UserMemMallocWhileSuccess(MEM_REGION locate,uint32_t size);

//获取当前动态内存使用率
uint8_t UserMemPerUsed(MEM_REGION locate);

//面向外部的内存调用宏定义
#define COMM_MemAlloc(size)     UserMemMallocWhileSuccess(MEM_SDRAM1, (size))
#define COMM_MemFree(p)         UserMemFree(MEM_SDRAM1, (p))

#define COMM_ItemAlloc(size)    UserMemMallocWhileSuccess(MEM_SDRAM2, (size))
#define COMM_ItemFree(p)        UserMemFree(MEM_SDRAM2, (p))
#define COMM_ItemPoolClean()    asm("nop");


#endif


