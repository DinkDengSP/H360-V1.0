/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-19 16:51:25
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-15 10:32:07
**FilePath: \App\TaskMain\AppSupport\AppTask\AppTaskStateUpLoad.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppTaskConfig.h"


//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferTaskAppStateUpLoad[STK_SIZE_TASK_APP_STATE_UPLOAD]@".dtcm_ram";
#pragma pack()

//任务控制块
OS_TCB tcbTaskAppStateUpLoad@".dtcm_ram";

//自动运行程序的执行周期,单位毫秒
#define STATE_UPLOAD_AUTO_FUNC_RUNNING_PERIOD_MS            500
//系统循环自动程序初始化
static void AppTaskFuncStateUpLoad_Auto_Init(void);
#if(STATE_UPLOAD_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
//系统还处理一些循环自动化程序
static void AppTaskFuncStateUpLoad_Auto_Proc(void);
#endif


//任务函数
void TaskFuncAppStateUpLoad(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //状态机初始化
    AppTaskFuncStateUpLoad_Auto_Init();
    #if(STATE_UPLOAD_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
    //上一次的外部信号检测发送的时间
	OS_TICK lastStateCheckTick = OSTimeGet(&err);
    //更新定时时钟
    OS_TICK currentStateCheckTick = OSTimeGet(&err);
    #endif
    while(1)
    {
        #if(STATE_UPLOAD_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
        //更新定时时钟
        currentStateCheckTick = OSTimeGet(&err);
		if(currentStateCheckTick - lastStateCheckTick >= (STATE_UPLOAD_AUTO_FUNC_RUNNING_PERIOD_MS))
		{
			//上次发送时间和本次发送时间差了STATE_PARAM_AUTO_FUNC_RUNNING_PERIOD_MS个时钟
			lastStateCheckTick = currentStateCheckTick;
			//模块定时执行任务
			AppTaskFuncStateUpLoad_Auto_Proc();
		}
        //如果当前时间小于上一次时间,说明计时器溢出,进行修正
		if(currentStateCheckTick < lastStateCheckTick)
		{
			lastStateCheckTick = currentStateCheckTick;
		}
        #endif 
        /* 除了定时任务其他不做 */
        CoreDelayMs(STATE_UPLOAD_AUTO_FUNC_RUNNING_PERIOD_MS);
    }
}

/************************************************自动运行程序************************************************/
//系统循环自动程序初始化
static void AppTaskFuncStateUpLoad_Auto_Init(void)
{
    AppImplStateUpLoad_EventInit();
}
#if(STATE_UPLOAD_AUTO_FUNC_RUNNING_PERIOD_MS!= 0)
//系统还处理一些循环自动化程序
static void AppTaskFuncStateUpLoad_Auto_Proc(void)
{
    
    AppImplStateUpLoad_EventRun();
      
}
#endif