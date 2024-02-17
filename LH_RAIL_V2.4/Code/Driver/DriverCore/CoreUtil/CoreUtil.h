/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-08 17:13:31
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-23 14:59:19
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CORE_UTIL_H_
#define __CORE_UTIL_H_
#include "CoreDelay.h"

/*主板中断分组设计,可选值 :
 * NVIC_PRIORITYGROUP_0
 * NVIC_PRIORITYGROUP_1
 * NVIC_PRIORITYGROUP_2
 * NVIC_PRIORITYGROUP_3
 * NVIC_PRIORITYGROUP_4
 * */
#define SYSTEM_DEFAULT_NVIC_GROUP       NVIC_PRIORITYGROUP_2

/**定义系统中断抢占优先级 数值越小,优先级越高*/
typedef enum NVIC_INT_PRE_PRI
{
    NVIC_INT_PRE_PRI_0 = 0,
    NVIC_INT_PRE_PRI_1 = 1,
    NVIC_INT_PRE_PRI_2 = 2,
    NVIC_INT_PRE_PRI_3 = 3,
} NVIC_INT_PRE_PRI;

/**定义系统中断子优先级 数值越小,优先级越高*/
typedef enum NVIC_INT_SUB_PRI
{
    NVIC_INT_SUB_PRI_0 = 0,
    NVIC_INT_SUB_PRI_1 = 1,
    NVIC_INT_SUB_PRI_2 = 2,
    NVIC_INT_SUB_PRI_3 = 3,
} NVIC_INT_SUB_PRI;

/*设置默认的中断优先级分组*/
void CoreNVIC_SetGroupDefault(void);

//设置进入低功耗睡眠模式
void WFI_SET(void);

//关闭所有中断(但是不包括fault和NMI中断)
void INTX_DISABLE(void);

//开启所有中断
void INTX_ENABLE(void);

//设置栈顶地址
//addr:栈顶地址
//set Main Stack value
void MSR_MSP(uint32_t addr);

//获取Icache是否打开
FunctionalState CoreUtilGet_ICahceState(void);

//获取DCache是否打开
FunctionalState CoreUtilGet_DCahceState(void);

//锁定线程,关闭中断
void ThreadLockInt(void);

//解锁线程,解锁中断
void ThreadUnlockInt(void);

//将当前Cache中的数据都写回SRAM
#define CacheWriteThrough()     (*(__IO uint32_t*)0XE000EF9C=1UL<<2)



#endif




