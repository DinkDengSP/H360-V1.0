/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-11-13 13:37:16
**LastEditors: DengXiaoJun
**LastEditTime: 2020-11-16 14:15:15
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CoreUtil.h"


void WFI_SET(void)
{
	__asm("WFI");
}

//关闭所有中断(但是不包括fault和NMI中断)
void INTX_DISABLE(void)
{
	__asm("CPSID   I");
	__asm("BX      LR");
}

//开启所有中断
void INTX_ENABLE(void)
{
	__asm("CPSIE   I");
	__asm("BX      LR");
}

//设置栈顶地址
//addr:栈顶地址
//set Main Stack value
void MSR_MSP(uint32_t addr)
{
	__asm("MSR MSP, r0");
	__asm("BX r14");
}

//锁定线程,关闭中断
inline void ThreadLockInt(void)
{
	//获取当前控制寄存器的状态,中断状态下为0,非中断下返回非0
	if(0 != __get_CONTROL())
	{
		//关闭全部中断
		INTX_DISABLE();
	}
}

//解锁线程,解锁中断
inline void ThreadUnlockInt(void)
{
	//获取当前控制寄存器的状态,中断状态下为0,非中断下返回非0
	if(0 != __get_CONTROL())
	{
		//打开全部中断
		INTX_ENABLE();
	}
}

