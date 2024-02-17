/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-09 14:43:22
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-12 13:52:54
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CoreDelay.h"
#include "os.h"

//us延时倍乘数
static uint32_t coreDelayUsTimeBase=0;	
//ms延时倍乘数,在os下,代表每个节拍的ms数						
static uint16_t coreDelayMsTimeBase=0;		
//微妙延时的最大限制
#define MAX_PARAM_US_DELAY		40000

//延时函数初始化
//SYSTICK的时钟固定为AHB时钟的1/8
//SYSCLK:系统时钟频率
void CoreDelayInit(void)
{
	uint32_t reload;
	//SysTick频率为HCLK
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	//微秒时间基数
	coreDelayUsTimeBase = (uint32_t)(SystemCoreClock/1000000);
	//每秒钟的计数次数 单位为K							    
	reload=coreDelayUsTimeBase;	
	//根据delay_ostickspersec设定溢出时间				           
	reload*=1000000/OSCfg_TickRate_Hz;	
	//reload为24位寄存器,最大值:16777216,在400M下,约合41.9ms左右	
	//代表OS可以延时的最少单位										
	coreDelayMsTimeBase = 1000/OSCfg_TickRate_Hz;	
	//开启SYSTICK中断		   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;
	//每1/OS_TICKS_PER_SEC 秒中断一次
	SysTick->LOAD=reload; 		
	//开启SYSTICK				
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; 
}

//延时指定时间
void CoreDelayMs(uint32_t nms)
{
	//如果OS已经在跑了,并且不是在中断里面(中断里面不能任务调度)
	if(OSRunning&&OSIntNestingCtr==0)	    
	{		 
		OS_ERR err; 
		//延时的时间大于OS的最少时间周期 
		if(nms>=coreDelayMsTimeBase)						
		{ 
			//UCOSIII延时采用周期模式
			OSTimeDly(nms/coreDelayMsTimeBase,OS_OPT_TIME_PERIODIC,&err);
		}
		//OS已经无法提供这么小的延时了,采用普通方式延时    
		nms %= coreDelayMsTimeBase;						
	}
	//普通方式延时
	CoreDelayUs((uint32_t)(nms*1000));				
}

//延时最小时钟单元
void CoreDelayMinTick(void)
{
	CoreDelayMs(coreDelayMsTimeBase);
}

//延时指定时间
void CoreDelayUs(uint16_t nus)
{
	OS_ERR err;
	//不能超过限制最大值
	if(nus > MAX_PARAM_US_DELAY)
	{
		nus = MAX_PARAM_US_DELAY;
	}
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	//LOAD的值
	uint32_t reload=SysTick->LOAD;	
	//需要的节拍数 				    	 
	ticks=nus*coreDelayUsTimeBase; 	
	//UCOSIII的方式,禁止调度，防止打断us延时					 
	OSSchedLock(&err);	
	//刚进入时的计数器值					
	told=SysTick->VAL;        				
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)
			{
				tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			}
			else 
			{
				tcnt+=reload-tnow+told;	
			}    
			told=tnow;
			//时间超过/等于要延迟的时间,则退出.
			if(tcnt>=ticks)break;			
		}  
	};
	//UCOSIII的方式,恢复调度
	OSSchedUnlock(&err);					
}

/*SysTick定时器中断函数*/
void CoreSystickIRQ_Handler(void)
{
	CPU_SR_ALLOC();
    //HAL库的延时函数
    HAL_IncTick();
    //OS开始跑了,才执行正常的调度处理
    if(OSRunning==1)					
	{
		CPU_CRITICAL_ENTER();
		OSIntEnter();//进入中断
		CPU_CRITICAL_EXIT();
		OSTimeTick();//调用ucos的时钟服务程序               
		OSIntExit();//触发任务切换软中断
	}
}
