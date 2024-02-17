#ifndef __APP_TASK_CONFIG_H_
#define __APP_TASK_CONFIG_H_
#include "os.h"
#include "AppImplInc.h"
#include "AppIpcBase.h"

//任务定义,步进电机测试任务
#define NAME_APP_TASK_STEPMOTOR                              "StepMotor"
//任务优先级
#define PRIO_APP_TASK_STEPMOTOR                              22
//任务堆栈大小
#define STK_SIZE_APP_TASK_STEPMOTOR                          8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskStepMotor[STK_SIZE_APP_TASK_STEPMOTOR];
//任务控制块
extern OS_TCB tcbAppTaskStepMotor;
//任务函数
void AppTaskFuncStepMotor(void *p_arg);

//任务定义,输入输出检查任务
#define NAME_APP_TASK_INOUT                              "InOut"
//任务优先级
#define PRIO_APP_TASK_INOUT                              23
//任务堆栈大小
#define STK_SIZE_APP_TASK_INOUT                          8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskInOut[STK_SIZE_APP_TASK_INOUT];
//任务控制块
extern OS_TCB tcbAppTaskInOut;
//任务函数
void AppTaskFuncInOut(void *p_arg);


//任务定义,条码枪扫描任务
#define NAME_APP_TASK_BARCODE_SCAN                              "BarCodeScan"
//任务优先级
#define PRIO_APP_TASK_BARCODE_SCAN                              24
//任务堆栈大小
#define STK_SIZE_APP_TASK_BARCODE_SCAN                          8 * 32
//任务堆栈
extern CPU_STK stackBufferAppTaskBarCodeScan[STK_SIZE_APP_TASK_BARCODE_SCAN];
//任务控制块
extern OS_TCB tcbAppTaskBarCodeScan;
//任务函数
void AppTaskFuncBarCodeScan(void *p_arg);



#endif




