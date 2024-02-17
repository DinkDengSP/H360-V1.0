/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-17 21:47:00
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-20 11:49:21
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_BASE_H_
#define __APP_IMPL_BASE_H_
#include "BoardInc.h"
#include "AppParam.h"
#include "AppState.h"
#include "SrvTaskConfig.h"
#include "TcpApp.h"
#include "AppImplWarningCode.h"

#define IsNeedLocalTrack        1    //生化本机轨道是否使用1.使用本机轨道
#define IsNeedOnlineTrack       AppImplCheckRailExist()    //是否使用联机轨道


//应用层CAN消息处理映射
typedef void (*AppImplModuleCanMsgProcFunc)(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength, APP_STATE* pstStatus);
//CAN消息的指令和映射的处理函数,免疫
typedef struct MAP_CAN_MSG_CMD_FUNC_UNIT
{
    CAN_CMD command;
    AppImplModuleCanMsgProcFunc canMsgProcFuncPtr;
}MAP_CAN_MSG_CMD_FUNC_UNIT;

//应用层CAN消息处理映射，轨道
typedef struct MAP_CAN_RAIL_MSG_CMD_FUNC_UNIT
{
    CAN_TRACK_CMD command;
    AppImplModuleCanMsgProcFunc canMsgProcFuncPtr;
}MAP_CAN_RAIL_MSG_CMD_FUNC_UNIT;

//应用层CAN消息处理映射
typedef void (*AppImplModuleBioCanMsgProcFunc)(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//CAN消息的指令和映射的处理函数
typedef struct MAP_CAN_BIO_MSG_CMD_FUNC_UNIT
{
    CAN_CMD command;
    AppImplModuleBioCanMsgProcFunc canMsgProcFuncPtr;
}MAP_CAN_BIO_MSG_CMD_FUNC_UNIT;

//复位后的小流程
extern void AppImplImmCanMsgProcCmd_ActionAfterReset(APP_STATE * pstStatus, uint8_t u8Module, uint8_t u8Result,APP_MODULE_STATE eState);
//判断轨道是否存在
extern uint8_t AppImplCheckRailExist();

#endif




