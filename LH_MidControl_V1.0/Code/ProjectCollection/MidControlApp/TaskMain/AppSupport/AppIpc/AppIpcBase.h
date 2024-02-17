/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-17 21:42:01
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-18 18:52:33
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_BASE_H_
#define __APP_IPC_BASE_H_
#include "BoardInc.h"
#include "TcpBaseCmd.h"

//系统内部指令处理的阶段定义
//系统动作调用API,分为三种 
//1. 发送动作并等待ACK(ACK无错误代表动作开始执行)  
//2. 发送动作并等待结果(无错误代表动作执行完成)  
//3. 读取上一次动作的结果
//如果调用了发送动作并等待ACK,那么同一个模块在下次发送动作之前必须读取上一次的动作结果,否则后一次调用动作报错.
typedef enum STATE_IPC
{
    STATE_IPC_IDLE          = 0, //空闲
    STATE_IPC_WAIT_ACK      = 1, //等待ACK
    STATE_IPC_WAIT_RETURN   = 2, //等待结果
    STATE_IPC_CMD_COMPLETE  = 3, //执行完成
    STATE_IPC_OTHER_CALL    = 4,//IPC指令在被其他线程调用
}STATE_IPC;

//辅助任务
typedef struct IPC_DATA_BASE
{
    //指令数据域操作互斥信号量
    TX_MUTEX dataMutex;
    //IPC ACK的信号量组
    TX_EVENT_FLAGS_GROUP* eventGroupFlagsAckPtr;
    //IPC 完成的信号量组
    TX_EVENT_FLAGS_GROUP* eventGroupFlagsCompletePtr;
    //目标消息队列指针
    TX_QUEUE* targetQueuePtr;
    //当前映射通道的事件标志位信息，表示1的左移位数
    uint8_t eventLeftShiftOffset;
    //指令错误参数
    LH_ERR errorParam;
    //返回的错误结果
    TCP_ERR errorCode;
    //返回的错误级别
    TCP_ERR_LEVEL errorLevel;
}IPC_DATA_BASE;

//IPC通讯，ACK事件标志组
extern TX_EVENT_FLAGS_GROUP ipcDataEventGroupAck;
//IPC通讯，Complete事件标志组
extern TX_EVENT_FLAGS_GROUP ipcDataEventGroupComplete;

//初始化APP IPC的基本数据结构
void IPC_AppBaseDataInit(void);

//IPC调用基础结构初始化
void IPC_DataInit(IPC_DATA_BASE* ipcDataPtr,uint8_t* mutexNamePtr,TX_QUEUE* targetQueuePtr,TX_EVENT_FLAGS_GROUP* ackFlagsGroupPtr,TX_EVENT_FLAGS_GROUP* completeFlagsGroupPtr,
                    uint8_t eventFlagLeftShiftOffset);

//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_BaseGetLastCommandStateAndResult(IPC_DATA_BASE* ipcDataPtr,LH_ERR* errorParam,TCP_ERR_LEVEL* errorLevel,TCP_ERR* errorCode);

//等待执行完成,阻塞
LH_ERR IPC_BaseWaitLastReturn(IPC_DATA_BASE* ipcDataPtr,TCP_ERR_LEVEL* errorLevel,TCP_ERR* errorCode);

//动作API发送任务给动作API,等待ACK不等待返回
LH_ERR IPC_BaseSendCommandWhileAck(IPC_DATA_BASE* ipcDataPtr,uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5,uint8_t* paramBuffer,
                                        uint16_t paramBufferLength);

//动作API发送任务给动作API,等待返回
LH_ERR IPC_BaseSendCommandWhileReturn(IPC_DATA_BASE* ipcDataPtr,uint16_t cmdIndex,int32_t param1,int32_t param2,int32_t param3,int32_t param4,int32_t param5,uint8_t* paramBuffer,
                                        uint16_t paramBufferLength,TCP_ERR_LEVEL* errorLevel,TCP_ERR* errorCode);



#endif




