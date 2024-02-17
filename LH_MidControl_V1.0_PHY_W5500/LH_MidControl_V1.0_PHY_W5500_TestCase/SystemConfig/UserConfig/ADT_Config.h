/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-10-27 17:22:10 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-10-31 20:03:44 +0800
************************************************************************************************/ 
#ifndef __ADT_CONFIG_H_
#define __ADT_CONFIG_H_
#include "CoreUtil.h"
#include "UserMemManage.h"

/*-----------------------------------------------ADT_CircularPtrQueue-------------------------------------------------*/
//引用ADT_PORT的内存申请函数实现自己的内存申请函数
//最好不要用SDRAM内存,SDRAM异常会导致串口输出异常
#define ADT_CIRCULAR_PTR_QUEUE_MEM_REGION                       MEM_AXI_SRAM
//内存申请
#define ADT_CIRCULAR_PTR_QUEUE_MEM_MALLOC(size)                 UserMemMallocWhileSuccess(ADT_CIRCULAR_PTR_QUEUE_MEM_REGION,size)
//内存释放
#define ADT_CIRCULAR_PTR_QUEUE_MEM_FREE(ptr)                    UserMemFree(ptr)

//锁定系统,这个数据结构用于对串口接收的数据进行保存,所以要开关全局中断,防止优先级问题
//例如处理的线程正在对数据进行写入,此时接收中断从队列中拿数据,就会造成异常
#define ADT_CIRCULAR_PTR_QUEUE_QUEUE_LOCK(lastState)            lastState = CoreLockIntGlobal()
//解锁系统
#define ADT_CIRCULAR_PTR_QUEUE_QUEUE_UNLOCK(lastState)          CoreUnlockIntGlobal(lastState)

/*-----------------------------------------------ADT_CircularByteQueue-------------------------------------------------*/
//锁定系统
#define ADT_CIRCULAR_BYTE_QUEUE_LOCK(lastState)                 lastState = CoreLockIntBasePri()
//解锁系统
#define ADT_CIRCULAR_BYTE_QUEUE_UNLOCK(lastState)               CoreUnlockIntBasePri(lastState)


#endif

