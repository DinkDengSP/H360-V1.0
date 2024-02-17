/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-14 17:09:21 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-15 11:18:34 +0800
************************************************************************************************/ 
#include "SystemTaskConfig.h"

//系统时钟初始化
extern void CoreClockInit(void);

int main(void)
{
    //中断向量表拷贝,并且重新设置
    CoreCopyIntVectorTable(VECTOR_TABLE_ADDR_SRC,D1_DTCMRAM_BASE);
    //STM32H7xx HAL 库初始化，此时系统用的还是H7自带的64MHz，HSI时钟:
    //调用函数HAL_InitTick，初始化滴答时钟中断1ms。
    //设置NVIV优先级分组为4。
	HAL_Init();
    //内核时钟初始化
    CoreClockInit();
    //MPU配置
    MCU_MPU_ConfigInit();
    //Cache使能
    MCU_MPU_CacheEnable();
    //延时系统初始化
    CoreDelayInit();
    /* 内核开启前关闭HAL的时间基准,也就是临时关闭Systick定时器 */
	HAL_SuspendTick();
    /* 进入ThreadX内核 */
    tx_kernel_enter();
    //正常运行不到这里
	while(1){asm("nop");}; 
}


//创建启动任务
void  tx_application_define(void *first_unused_memory)
{
	/**************创建启动任务*********************/
    tx_thread_create(&tcbSystemTaskStart,               /* 任务控制块地址 */   
                     NAME_SYSTEM_TASK_START,            /* 任务名 */
                     FuncTaskStartEntry,                /* 启动任务函数地址 */
                     0,                                 /* 传递给任务的参数 */
                     &stackBufferSystemTaskStart[0],    /* 堆栈基地址 */
                     STK_SIZE_SYSTEM_TASK_START,        /* 堆栈空间大小 */  
                     PRIO_SYSTEM_TASK_START,            /* 任务优先级*/
                     PRIO_SYSTEM_TASK_START,            /* 任务抢占阀值 */
                     TX_NO_TIME_SLICE,                  /* 不开启时间片 */
                     TX_AUTO_START);                    /* 创建后立即启动 */
}


