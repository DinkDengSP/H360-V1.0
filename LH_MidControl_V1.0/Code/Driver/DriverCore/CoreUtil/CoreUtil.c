/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-07-26 13:22:46
**LastEditors: DengXiaoJun
**LastEditTime: 2021-07-27 13:20:05
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CoreUtil.h"
#include "tx_initialize.h"

//导入外部变量
extern ULONG  _tx_thread_system_state;

/*设置默认的中断优先级分组*/
void CoreNVIC_SetGroupDefault(void)
{
    HAL_NVIC_SetPriorityGrouping(SYSTEM_DEFAULT_NVIC_GROUP);
}

//进入睡眠模式
void WFI_SET(void)
{
	asm("WFI");
}

//关闭所有中断(但是不包括fault和NMI中断)
void INTX_DISABLE(void)
{
	__set_PRIMASK(1);
}

//开启所有中断
void INTX_ENABLE(void)
{
	__set_PRIMASK(0);
}

//禁用操作系统调度器
uint32_t CoreLockScheduler(void)
{
	uint32_t oldIntState = 0;
	oldIntState = tx_interrupt_control(TX_INT_DISABLE);
    return oldIntState;
}

//打开操作系统调度器
void CoreUnlockScheduler(uint32_t preemption)
{
	tx_interrupt_control(preemption);
}

//设置栈顶地址
//addr:栈顶地址
//set Main Stack value
void MSR_MSP(uint32_t addr)
{
	asm("MSR MSP, r0");
	asm("BX r14");
}

//获取Icache是否打开
FunctionalState CoreUtilGet_ICahceState(void)
{
    uint8_t state;
    state = (((SCB->CCR)>>17)&0X01);
    return ((state == 0)?DISABLE:ENABLE);
}

//获取DCache是否打开
FunctionalState CoreUtilGet_DCahceState(void)
{
    uint8_t state;
    state = (((SCB->CCR)>>16)&0X01);
    return ((state == 0)?DISABLE:ENABLE);
}

//锁定线程,关闭中断
inline void ThreadLockWithOutInt(int* intControlValue)
{
	//获取当前控制寄存器的状态,中断状态下为0,非中断下返回非0
    //只有在非中断的情况下,才能真的关闭中断,主要用于中断和非中断同时处理数据结构的时候
    //非中断中将中断关闭掉,防止中断中进行抢断
	if(0 != __get_CONTROL())
	{
		*intControlValue = tx_interrupt_control(TX_INT_DISABLE);
	}
}

//解锁线程,解锁中断
inline void ThreadUnlockWithOutInt(int lastControlValue)
{
	//获取当前控制寄存器的状态,中断状态下为0,非中断下返回非0
    //只有在非中断的情况下,才能真的打开中断,主要用于中断和非中断同时处理数据结构的时候
    //非中断中将中断关闭掉,防止中断中进行抢断
	if(0 != __get_CONTROL())
	{
		tx_interrupt_control(lastControlValue);
	}
}


//设置内存块指定值
void UserMemSet(void *s,uint8_t c,uint32_t count)
{
    uint8_t *xs = s; 
    if(count == 0)
        return;
    while(count--)
		*xs++=c;
}

//拷贝内存块
void UserMemCopy(void *des,void *src,uint32_t n)
{
    uint8_t *xdes=des;
	uint8_t *xsrc=src;
    if(n == 0)
        return;
    while(n--)
		*xdes++=*xsrc++; 
}




