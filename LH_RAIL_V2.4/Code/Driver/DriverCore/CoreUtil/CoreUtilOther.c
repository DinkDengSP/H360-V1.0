/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-08 17:13:31
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-12 10:08:00
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CoreUtil.h"

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
	asm("CPSID   I");
	asm("BX      LR");
}

//开启所有中断
void INTX_ENABLE(void)
{
	asm("CPSIE   I");
	asm("BX      LR");
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
inline void ThreadLockInt(void)
{
	//获取当前控制寄存器的状态,中断状态下为0,非中断下返回非0
	if(0 == __get_CONTROL())
	{
		//关闭全部中断
		INTX_DISABLE();
	}
}

//解锁线程,解锁中断
inline void ThreadUnlockInt(void)
{
	//获取当前控制寄存器的状态,中断状态下为0,非中断下返回非0
	if(0 == __get_CONTROL())
	{
		//打开全部中断
		INTX_ENABLE();
	}
}
