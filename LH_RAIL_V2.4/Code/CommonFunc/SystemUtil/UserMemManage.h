/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 15:37:41
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-28 09:00:48
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
    MEM_DTCM        = 0,
    MEM_AXI_SRAM    = 1,
    MEM_D2_SRAM1    = 2,
    MEM_D2_SRAM2    = 3,
    MEM_D3_SRAM4    = 4,
    MEM_SRAM        = 5,
    MEM_REGION_MAX  = MEM_SRAM,
}USER_MEM_REGION;

//设置内存块指定值
void UserMemSet(void *s,uint8_t c,uint32_t count);

//拷贝内存块
void UserMemCopy(void *des,void *src,uint32_t n);

//内存管理初始化
void UserMemInit(USER_MEM_REGION region);

//申请内存
void* UserMemMalloc(USER_MEM_REGION region,uint32_t size);

//释放内存
void UserMemFree(USER_MEM_REGION region,void* ptr);

//重新申请内存
void* UserMemRealloc(USER_MEM_REGION region,void *ptr,uint32_t size);

//获取当前动态内存使用率
uint8_t UserMemPerUsed(USER_MEM_REGION region);

#endif
