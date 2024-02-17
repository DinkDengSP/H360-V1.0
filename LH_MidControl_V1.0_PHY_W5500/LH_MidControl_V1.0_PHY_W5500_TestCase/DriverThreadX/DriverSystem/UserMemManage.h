/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:46:54 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:14:32 +0800
************************************************************************************************/ 
#ifndef __USER_MEM_MANAGE_H_
#define __USER_MEM_MANAGE_H_
#include "DriverHeaderBoard.h"
#include "MemManageDriverConfig.h"

//内存区域定义
typedef enum MEM_REGION
{
    MEM_AXI_SRAM = 0X00,
    MEM_D2_SRAM1 = 0X01,
    MEM_D3_SRAM4 = 0X02,
    MEM_SDRAM1   = 0X03,
    MEM_SDRAM2   = 0X04,
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


#endif
