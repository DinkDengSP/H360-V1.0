/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-01-14 14:05:09
**FilePath: \H360ActionBoardSoftwareV2.3\CommonService\ServiceIPC\ServiceIpcOutput.h
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_OUTPUT_H_
#define __SERVICE_IPC_OUTPUT_H_
#include "CommonServiceIpcBase.h"

/*******************************************************初始化************************************************************/
//IPC调用基础结构初始化
void IPC_OutputDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//输出任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_OutputGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_OutputWaitLastReturn(void);


/******************************************只要求ACK不要求返回的指令**********************************************/
//输出设置指定引脚状态
LH_ERR IPC_OutputWritePinValueWhileAck(BOARD_OUT_PIN pinIndex,BitAction value);


/******************************************发出指令并等待返回的指令***********************************************/
//输出设置指定引脚状态
LH_ERR IPC_OutputWritePinValueWhileReturn(BOARD_OUT_PIN pinIndex,BitAction value);

#endif

