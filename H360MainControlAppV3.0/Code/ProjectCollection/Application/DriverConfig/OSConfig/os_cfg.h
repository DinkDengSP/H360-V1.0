/*
*********************************************************************************************************
*                                              uC/OS-III
*                                        The Real-Time Kernel
*
*                    Copyright 2009-2020 Silicon Laboratories Inc. www.silabs.com
*
*                                 SPDX-License-Identifier: APACHE-2.0
*
*               This software is subject to an open source license and is distributed by
*                Silicon Laboratories Inc. pursuant to the terms of the Apache License,
*                    Version 2.0 available at www.apache.org/licenses/LICENSE-2.0.
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                          CONFIGURATION FILE
*
* Filename : os_cfg.h
* Version  : V3.08.00
*********************************************************************************************************
*/

#ifndef OS_CFG_H
#define OS_CFG_H

//操作系统Hook回调
#define OS_CFG_APP_HOOKS_EN                        0u           /* Enable (1) or Disable (0) application specific hooks                  */
//运行时参数检查
#define OS_CFG_ARG_CHK_EN                          1u           /* Enable (1) or Disable (0) argument checking                           */
//中断中调用函数检查
#define OS_CFG_CALLED_FROM_ISR_CHK_EN              1u           /* Enable (1) or Disable (0) check for called from ISR                   */
//系统调试功能
#define OS_CFG_DBG_EN                              1u           /* Enable (1) or Disable (0) debug code/variables                        */
//系统Tick定时器使能
#define OS_CFG_TICK_EN                             1u           /* Enable (1) or Disable (0) the kernel tick                             */
//动态时钟源使能
#define OS_CFG_DYN_TICK_EN                         0u           /* Enable (1) or Disable (0) the Dynamic Tick                            */
//不支持的操作系统调用检查
#define OS_CFG_INVALID_OS_CALLS_CHK_EN             1u           /* Enable (1) or Disable (0) checks for invalid kernel calls             */
//对象类型检查
#define OS_CFG_OBJ_TYPE_CHK_EN                     1u           /* Enable (1) or Disable (0) object type checking                        */
//系统时间帧测量
#define OS_CFG_TS_EN                               1u           /* Enable (1) or Disable (0) time stamping                               */
//最大任务优先级
#define OS_CFG_PRIO_MAX                           32u           /* Defines the maximum number of task priorities (see OS_PRIO data type) */
//调度时间测量
#define OS_CFG_SCHED_LOCK_TIME_MEAS_EN             1u           /* Include code to measure scheduler lock time                           */
//同级优先级轮转
#define OS_CFG_SCHED_ROUND_ROBIN_EN                1u           /* Include code for Round-Robin scheduling                               */
//最小堆栈大小
#define OS_CFG_STK_SIZE_MIN                       128u           /* Minimum allowable task stack size                                     */
//系统事件标志
#define OS_CFG_FLAG_EN                             1u           /* Enable (1) or Disable (0) code generation for EVENT FLAGS             */
#define OS_CFG_FLAG_DEL_EN                         1u           /*     Include code for OSFlagDel()                                      */
#define OS_CFG_FLAG_MODE_CLR_EN                    1u           /*     Include code for Wait on Clear EVENT FLAGS                        */
#define OS_CFG_FLAG_PEND_ABORT_EN                  1u           /*     Include code for OSFlagPendAbort()                                */
//系统内存管理
#define OS_CFG_MEM_EN                              1u           /* Enable (1) or Disable (0) code generation for the MEMORY MANAGER      */
//互斥信号量
#define OS_CFG_MUTEX_EN                            1u           /* Enable (1) or Disable (0) code generation for MUTEX                   */
#define OS_CFG_MUTEX_DEL_EN                        1u           /*     Include code for OSMutexDel()                                     */
#define OS_CFG_MUTEX_PEND_ABORT_EN                 1u           /*     Include code for OSMutexPendAbort()                               */
//消息队列
#define OS_CFG_Q_EN                                1u           /* Enable (1) or Disable (0) code generation for QUEUES                  */
#define OS_CFG_Q_DEL_EN                            1u           /*     Include code for OSQDel()                                         */
#define OS_CFG_Q_FLUSH_EN                          1u           /*     Include code for OSQFlush()                                       */
#define OS_CFG_Q_PEND_ABORT_EN                     1u           /*     Include code for OSQPendAbort()                                   */
//多值信号量
#define OS_CFG_SEM_EN                              1u           /* Enable (1) or Disable (0) code generation for SEMAPHORES              */
#define OS_CFG_SEM_DEL_EN                          1u           /*     Include code for OSSemDel()                                       */
#define OS_CFG_SEM_PEND_ABORT_EN                   1u           /*     Include code for OSSemPendAbort()                                 */
#define OS_CFG_SEM_SET_EN                          1u           /*     Include code for OSSemSet()                                       */
//状态统计任务
#define OS_CFG_STAT_TASK_EN                        1u           /* Enable (1) or Disable (0) the statistics task                         */
#define OS_CFG_STAT_TASK_STK_CHK_EN                1u           /*     Check task stacks from the statistic task                         */
//运行时优先级修改
#define OS_CFG_TASK_CHANGE_PRIO_EN                 1u           /* Include code for OSTaskChangePrio()                                   */
//删除任务
#define OS_CFG_TASK_DEL_EN                         1u           /* Include code for OSTaskDel()                                          */
//空闲任务使能
#define OS_CFG_TASK_IDLE_EN                        1u           /* Include the idle task                                                 */
//任务TCB配置
#define OS_CFG_TASK_PROFILE_EN                     1u           /* Include variables in OS_TCB for profiling                             */
//任务消息使能
#define OS_CFG_TASK_Q_EN                           1u           /* Include code for OSTaskQXXXX()                                        */
//强制解除等待指定消息
#define OS_CFG_TASK_Q_PEND_ABORT_EN                1u           /* Include code for OSTaskQPendAbort()                                   */
//任务寄存器的尺寸
#define OS_CFG_TASK_REG_TBL_SIZE                   1u           /* Number of task specific registers                                     */
//任务堆栈红区,红区堆栈是保留的,不能被中断和信号所侵占,函数可以在不移动栈指针的情况下使用它存取一些临时数据——于是两个移动rsp的指令就被节省下来了
#define OS_CFG_TASK_STK_REDZONE_EN                 1u           /* Enable (1) or Disable (0) stack redzone                               */
//堆栈红区深度
#define OS_CFG_TASK_STK_REDZONE_DEPTH              8u           /* Depth of the stack redzone                                            */
//信号量强制获取,取消挂起
#define OS_CFG_TASK_SEM_PEND_ABORT_EN              1u           /* Include code for OSTaskSemPendAbort()                                 */
//任务挂起使能
#define OS_CFG_TASK_SUSPEND_EN                     1u           /* Include code for OSTaskSuspend() and OSTaskResume()                   */
//任务存储空间大小
#define OS_CFG_TLS_TBL_SIZE                        0u           /* Include code for Task Local Storage (TLS) registers                   */
//延时函数
#define OS_CFG_TIME_DLY_HMSM_EN                    1u           /* Include code for OSTimeDlyHMSM()                                      */
#define OS_CFG_TIME_DLY_RESUME_EN                  1u           /* Include code for OSTimeDlyResume()                                    */
//定时器
#define OS_CFG_TMR_EN                              1u           /* Enable (1) or Disable (0) code generation for TIMERS                  */
#define OS_CFG_TMR_DEL_EN                          1u           /* Enable (1) or Disable (0) code generation for OSTmrDel()              */
//系统指令跟踪
#define OS_CFG_TRACE_EN                            0u           /* Enable (1) or Disable (0) uC/OS-III Trace instrumentation             */
#define OS_CFG_TRACE_API_ENTER_EN                  0u           /* Enable (1) or Disable (0) uC/OS-III Trace API enter instrumentation   */
#define OS_CFG_TRACE_API_EXIT_EN                   0u           /* Enable (1) or Disable (0) uC/OS-III Trace API exit  instrumentation   */

#endif
