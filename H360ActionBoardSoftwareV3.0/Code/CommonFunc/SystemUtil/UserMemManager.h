/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-07-13 14:56:58
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-20 15:15:43
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#ifndef __USER_MEM_MANAGER_H_
#define __USER_MEM_MANAGER_H_
#include "stm32f4xx.h"
#include "CoreInc.h"

//内存管理代码
typedef enum SRAM_LOCATE
{
    SRAM_IN,
    SRAM_CCM,
    SRAM_IS62,
}SRAM_LOCATE;

//设置内存块指定值
void UserMemSet(void *s,uint8_t c,uint32_t count);

//拷贝内存块
void UserMemCopy(void *des,void *src,uint32_t n);

//动态内存初始化
void UserMemInit(SRAM_LOCATE locate);

//申请内存
void* UserMemMalloc(SRAM_LOCATE locate,uint32_t size);

//申请内存
void* UserMemMallocWhileSuccess(SRAM_LOCATE locate,uint32_t size);

//释放内存
void UserMemFree(SRAM_LOCATE locate,void* ptr);

//重新申请内存
void* UserMemRealloc(SRAM_LOCATE locate,void *ptr,uint32_t size);

//获取当前动态内存使用率
uint8_t UserMemPerUsed(SRAM_LOCATE locate);

#endif



