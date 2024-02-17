#ifndef __SERVICE_TASK_CONFIG_H_
#define __SERVICE_TASK_CONFIG_H_
#include "os.h"
#include "ServiceIpcBase.h"
#include "ServiceImplInc.h"

/****************************************************CAN1协议解析任务****************************************************/
//CAN总线中断处理回调函数
void OS_Can1ReceiveDataCallFunc(CanRxMsg* rxMsg);
//任务定义
#define NAME_SERVICE_TASK_CAN1_RECV                              "Can1Recv"
//任务优先级
#define PRIO_SERVICE_TASK_CAN1_RECV                              0
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_CAN1_RECV                          8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskCan1Recv[STK_SIZE_SERVICE_TASK_CAN1_RECV];
//任务控制块
extern OS_TCB tcbServiceTaskCan1Recv;
//任务函数
void ServiceTaskFuncCan1Recv(void *p_arg);


/*******************************************************心跳任务********************************************************/
//任务定义
#define NAME_SERVICE_TASK_HEART                                  "Heart"
//任务优先级
#define PRIO_SERVICE_TASK_HEART                                  1
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_HEART                              8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskHeart[STK_SIZE_SERVICE_TASK_HEART];
//任务控制块
extern OS_TCB tcbServiceTaskHeart;
//任务函数
void ServiceTaskFuncHeart(void *p_arg);

/*******************************************************DebugIAP任务*****************************************************/
//任务定义
#define NAME_SERVICE_TASK_DEBUG_IAP                              "DebugIAP"
//任务优先级
#define PRIO_SERVICE_TASK_DEBUG_IAP                              2
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_DEBUG_IAP                          8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskDebugIAP[STK_SIZE_SERVICE_TASK_DEBUG_IAP];
//任务控制块
extern OS_TCB tcbServiceTaskDebugIAP;
//任务函数
void ServiceTaskFuncDebugIAP(void *p_arg);

/*******************************************************输入读取*********************************************************/
//任务定义
#define NAME_SERVICE_TASK_INPUT_READ                             "InputRead"
//任务优先级
#define PRIO_SERVICE_TASK_INPUT_READ                             3
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_INPUT_READ                         8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskInputRead[STK_SIZE_SERVICE_TASK_INPUT_READ];
//任务控制块
extern OS_TCB tcbServiceTaskInputRead;
//任务函数
void ServiceTaskFuncInputRead(void *p_arg);

/*******************************************************输出写入*********************************************************/
//任务定义
#define NAME_SERVICE_TASK_OUTPUT_WRITE                           "OutputWrite"
//任务优先级
#define PRIO_SERVICE_TASK_OUTPUT_WRITE                           4
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_OUTPUT_WRITE                       8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskOutputWrite[STK_SIZE_SERVICE_TASK_OUTPUT_WRITE];
//任务控制块
extern OS_TCB tcbServiceTaskOutputWrite;
//任务函数
void ServiceTaskFuncOutputWrite(void *p_arg);

/*******************************************************RS1应用**********************************************************/
//任务定义
#define NAME_SERVICE_TASK_SERIAL_USER_RS1                        "SerialUserRS1"
//任务优先级
#define PRIO_SERVICE_TASK_SERIAL_USER_RS1                        5
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_SERIAL_USER_RS1                    8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskSerialUserRS1[STK_SIZE_SERVICE_TASK_SERIAL_USER_RS1];
//任务控制块
extern OS_TCB tcbServiceTaskSerialUserRS1;
//任务函数
void ServiceTaskFuncSerialUserRS1(void *p_arg);

/*******************************************************RS2调试**********************************************************/
//中断回调函数
void SystemUart3RecvCallBack(uint8_t recvDataByte);
//任务定义
#define NAME_SERVICE_TASK_SERIAL_DEBUG_RS2                       "SerialDebugRS2"
//任务优先级
#define PRIO_SERVICE_TASK_SERIAL_DEBUG_RS2                       6
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_SERIAL_DEBUG_RS2                   8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskSerialDebugRS2[STK_SIZE_SERVICE_TASK_SERIAL_DEBUG_RS2];
//任务控制块
extern OS_TCB tcbServiceTaskSerialDebugRS2;
//任务函数
void ServiceTaskFuncSerialDebugRS2(void *p_arg);

/*******************************************************步进电机1********************************************************/
//任务定义
#define NAME_SERVICE_TASK_STEP_MOTOR1                            "StepMotor1"
//任务优先级
#define PRIO_SERVICE_TASK_STEP_MOTOR1                            7
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_STEP_MOTOR1                        8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskStepMotor1[STK_SIZE_SERVICE_TASK_STEP_MOTOR1];
//任务控制块
extern OS_TCB tcbServiceTaskStepMotor1;
//任务函数
void ServiceTaskFuncServiceTaskStepMotor1(void *p_arg);

/*******************************************************步进电机2********************************************************/
//任务定义
#define NAME_SERVICE_TASK_STEP_MOTOR2                            "StepMotor2"
//任务优先级
#define PRIO_SERVICE_TASK_STEP_MOTOR2                            8
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_STEP_MOTOR2                        8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskStepMotor2[STK_SIZE_SERVICE_TASK_STEP_MOTOR2];
//任务控制块
extern OS_TCB tcbServiceTaskStepMotor2;
//任务函数
void ServiceTaskFuncServiceTaskStepMotor2(void *p_arg);

/*******************************************************步进电机3********************************************************/
//任务定义
#define NAME_SERVICE_TASK_STEP_MOTOR3                            "StepMotor3"
//任务优先级
#define PRIO_SERVICE_TASK_STEP_MOTOR3                            9
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_STEP_MOTOR3                        8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskStepMotor3[STK_SIZE_SERVICE_TASK_STEP_MOTOR3];
//任务控制块
extern OS_TCB tcbServiceTaskStepMotor3;
//任务函数
void ServiceTaskFuncServiceTaskStepMotor3(void *p_arg);

/*******************************************************步进电机4********************************************************/
//任务定义
#define NAME_SERVICE_TASK_STEP_MOTOR4                            "StepMotor4"
//任务优先级
#define PRIO_SERVICE_TASK_STEP_MOTOR4                            10
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_STEP_MOTOR4                        8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskStepMotor4[STK_SIZE_SERVICE_TASK_STEP_MOTOR4];
//任务控制块
extern OS_TCB tcbServiceTaskStepMotor4;
//任务函数
void ServiceTaskFuncServiceTaskStepMotor4(void *p_arg);

/*******************************************************步进电机5********************************************************/
//任务定义
#define NAME_SERVICE_TASK_STEP_MOTOR5                            "StepMotor5"
//任务优先级
#define PRIO_SERVICE_TASK_STEP_MOTOR5                            11
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_STEP_MOTOR5                        8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskStepMotor5[STK_SIZE_SERVICE_TASK_STEP_MOTOR5];
//任务控制块
extern OS_TCB tcbServiceTaskStepMotor5;
//任务函数
void ServiceTaskFuncServiceTaskStepMotor5(void *p_arg);

/*******************************************************步进电机6********************************************************/
//任务定义
#define NAME_SERVICE_TASK_STEP_MOTOR6                            "StepMotor6"
//任务优先级
#define PRIO_SERVICE_TASK_STEP_MOTOR6                            12
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_STEP_MOTOR6                        8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskStepMotor6[STK_SIZE_SERVICE_TASK_STEP_MOTOR6];
//任务控制块
extern OS_TCB tcbServiceTaskStepMotor6;
//任务函数
void ServiceTaskFuncServiceTaskStepMotor6(void *p_arg);

/*******************************************************步进电机7********************************************************/
//任务定义
#define NAME_SERVICE_TASK_STEP_MOTOR7                            "StepMotor7"
//任务优先级
#define PRIO_SERVICE_TASK_STEP_MOTOR7                            13
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_STEP_MOTOR7                        8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskStepMotor7[STK_SIZE_SERVICE_TASK_STEP_MOTOR7];
//任务控制块
extern OS_TCB tcbServiceTaskStepMotor7;
//任务函数
void ServiceTaskFuncServiceTaskStepMotor7(void *p_arg);

/*******************************************************步进电机8********************************************************/
//任务定义
#define NAME_SERVICE_TASK_STEP_MOTOR8                            "StepMotor8"
//任务优先级
#define PRIO_SERVICE_TASK_STEP_MOTOR8                            14
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_STEP_MOTOR8                        8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskStepMotor8[STK_SIZE_SERVICE_TASK_STEP_MOTOR8];
//任务控制块
extern OS_TCB tcbServiceTaskStepMotor8;
//任务函数
void ServiceTaskFuncServiceTaskStepMotor8(void *p_arg);

/*******************************************************步进电机9********************************************************/
//任务定义
#define NAME_SERVICE_TASK_STEP_MOTOR9                            "StepMotor9"
//任务优先级
#define PRIO_SERVICE_TASK_STEP_MOTOR9                            15
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_STEP_MOTOR9                        8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskStepMotor9[STK_SIZE_SERVICE_TASK_STEP_MOTOR9];
//任务控制块
extern OS_TCB tcbServiceTaskStepMotor9;
//任务函数
void ServiceTaskFuncServiceTaskStepMotor9(void *p_arg);

/*******************************************************步进电机10*******************************************************/
//任务定义
#define NAME_SERVICE_TASK_STEP_MOTOR10                           "StepMotor10"
//任务优先级
#define PRIO_SERVICE_TASK_STEP_MOTOR10                           16
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_STEP_MOTOR10                       8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskStepMotor10[STK_SIZE_SERVICE_TASK_STEP_MOTOR10];
//任务控制块
extern OS_TCB tcbServiceTaskStepMotor10;
//任务函数
void ServiceTaskFuncServiceTaskStepMotor10(void *p_arg);

/*******************************************************步进电机11*******************************************************/
//任务定义
#define NAME_SERVICE_TASK_STEP_MOTOR11                           "StepMotor11"
//任务优先级
#define PRIO_SERVICE_TASK_STEP_MOTOR11                           17
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_STEP_MOTOR11                       8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskStepMotor11[STK_SIZE_SERVICE_TASK_STEP_MOTOR11];
//任务控制块
extern OS_TCB tcbServiceTaskStepMotor11;
//任务函数
void ServiceTaskFuncServiceTaskStepMotor11(void *p_arg);

/*******************************************************步进电机12*******************************************************/
//任务定义
#define NAME_SERVICE_TASK_STEP_MOTOR12                           "StepMotor12"
//任务优先级
#define PRIO_SERVICE_TASK_STEP_MOTOR12                           18
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_STEP_MOTOR12                       8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskStepMotor12[STK_SIZE_SERVICE_TASK_STEP_MOTOR12];
//任务控制块
extern OS_TCB tcbServiceTaskStepMotor12;
//任务函数
void ServiceTaskFuncServiceTaskStepMotor12(void *p_arg);

/*******************************************************服务参数读写*****************************************************/
//任务定义
#define NAME_SERVICE_TASK_PARAM                               "ServiceParam"
//任务优先级
#define PRIO_SERVICE_TASK_PARAM                               19
//任务堆栈大小
#define STK_SIZE_SERVICE_TASK_PARAM                           8 * 32
//任务堆栈
extern CPU_STK stackBufferServiceTaskParam[STK_SIZE_SERVICE_TASK_PARAM];
//任务控制块
extern OS_TCB tcbServiceTaskParam;
//任务函数
void ServiceTaskFuncServiceTaskParam(void *p_arg);


#endif






