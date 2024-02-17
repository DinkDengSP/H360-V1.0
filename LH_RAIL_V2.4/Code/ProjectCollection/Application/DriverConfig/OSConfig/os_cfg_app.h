/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-09 19:20:44
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-22 16:45:33
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
/*
*********************************************************************************************************
*
*                               OS CONFIGURATION (APPLICATION SPECIFICS)
*
* Filename : os_cfg_app.h
* Version  : V3.08.00
*********************************************************************************************************
*/

#ifndef OS_CFG_APP_H
#define OS_CFG_APP_H

/*
**************************************************************************************************************************
*                                                      CONSTANTS
**************************************************************************************************************************
*/
                                                                /* ------------------ MISCELLANEOUS ------------------- */
                                                                /* Stack size of ISR stack (number of CPU_STK elements) */
//中断任务空间
#define  OS_CFG_ISR_STK_SIZE                                512u
//系统消息池
#define  OS_CFG_MSG_POOL_SIZE                               1920u
//堆栈报警量
#define  OS_CFG_TASK_STK_LIMIT_PCT_EMPTY                    10u
//空闲任务堆栈
#define  OS_CFG_IDLE_TASK_STK_SIZE                          128u
//统计任务堆栈
#define  OS_CFG_STAT_TASK_PRIO                              ((OS_PRIO)(OS_CFG_PRIO_MAX-2u))
//统计任务运行频率
#define  OS_CFG_STAT_TASK_RATE_HZ                           10u
//统计任务堆栈
#define  OS_CFG_STAT_TASK_STK_SIZE                          256u
//系统运行周期
#define  OS_CFG_TICK_RATE_HZ                                1000u
//定时运行任务优先级
#define  OS_CFG_TMR_TASK_PRIO                               ((OS_PRIO)(OS_CFG_PRIO_MAX-3u))
//定时运行任务的堆栈
#define  OS_CFG_TMR_TASK_STK_SIZE                           512u
//定时运行任务的运行周期
#define  OS_CFG_TMR_TASK_RATE_HZ                            100u


#endif
