/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-14 13:59:38
 *FilePath: \H360ActionBoardSoftwareV2.3\CommonService\ServiceIPC\ServiceIpcInput.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_INPUT_H_
#define __SERVICE_IPC_INPUT_H_
#include "CommonServiceIpcBase.h"

/*******************************************************初始化************************************************************/
//IPC调用基础结构初始化
void IPC_InputDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//输入任务的上一次执行结果,也就是有没有执行完成
STATE_IPC IPC_InputGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_InputWaitLastReturn(void);

/******************************************只要求ACK不要求返回的指令**********************************************/
//读取指定引脚状态
LH_ERR IPC_InputReadPinValueWhileAck(BOARD_IN_PIN pinIndex);

//系统急停
LH_ERR IPC_InputAllStepMotorEmergencyWhileAck(void);

/******************************************发出指令并等待返回的指令***********************************************/
//读取指定引脚状态
LH_ERR IPC_InputReadPinValueWhileReturn(BOARD_IN_PIN pinIndex,BitAction* bitValue);

//系统急停
LH_ERR IPC_InputAllStepMotorEmergencyWhileReturn(void);

/*******************************************结果数据读取**************************************************/
//读取上次引脚读取的结果
BitAction  IPC_InputLastReadPinValueResult(void);


#endif




