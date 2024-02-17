/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-11-13 13:37:16
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-11-16 14:34:22
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CORE_UTIL_H_
#define __CORE_UTIL_H_
#include "CoreDelay.h"
#include "ErrorCode.h"
#include "SEGGER_RTT.h"

void WFI_SET(void);

//关闭所有中断(但是不包括fault和NMI中断)
void INTX_DISABLE(void);

//开启所有中断
void INTX_ENABLE(void);

//设置栈顶地址
//addr:栈顶地址
//set Main Stack value
void MSR_MSP(uint32_t addr);

//锁定线程,关闭中断
void ThreadLockInt(void);

//解锁线程,解锁中断
void ThreadUnlockInt(void);


/**定义系统中断优先级 数值越小,优先级越高*/
typedef enum INT_PRI
{
    INT_PRI_0 = 0,
    INT_PRI_1 = 1,
    INT_PRI_2 = 2,
    INT_PRI_3 = 3,
} INT_PRI;

/*空引用定义*/
#ifndef NULL
#define NULL 0
#endif



#endif




