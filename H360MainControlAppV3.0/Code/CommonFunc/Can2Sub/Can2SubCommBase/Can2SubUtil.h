/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-02 01:21:57
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-15 13:18:19
 *FilePath: \Code\CommonFunc\Can2SubCommBase\Can2SubUtil.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_UTIL_H_
#define __CAN2_SUB_UTIL_H_
#include "Can2ProcMain.h"
#include "Can2SubBase.h"

//当前动作板的状态,是什么模式
typedef enum ACTION_BOARD_CURRENT_STATE
{
    ACTION_BOARD_CURRENT_STATE_BOOT = 0,
    ACTION_BOARD_CURRENT_STATE_APP = 1,
}ACTION_BOARD_CURRENT_STATE;

//握手
LH_ERR Can2SubUtil_HandShake(CAN2_SUB_BOARD boardID,MODE_RUNNING mode);

//获取软件版本
LH_ERR Can2SubUtil_ReadSoftwareVersion(CAN2_SUB_BOARD boardID,READ_SOFTWARE_VERSION readSoftwareVersion,uint8_t* mainVersion,uint8_t* subVersion,uint8_t* debugVersion,uint32_t* svnVersion);

//查询当前动作板处于何种模式
LH_ERR Can2SubActionBoardReadCurrentState(CAN2_SUB_BOARD boardID,ACTION_BOARD_CURRENT_STATE* state);

//动作板跳转进入BOOT
LH_ERR Can2SubActionBoardJumpBoot(CAN2_SUB_BOARD boardID);

//动作板跳转进入APP
LH_ERR Can2SubActionBoardJumpApp(CAN2_SUB_BOARD boardID);

//传输开始编程的信息
LH_ERR Can2SubActionBoardSetProgDataLength(CAN2_SUB_BOARD boardID,uint32_t pragDataLengthBytes);

//传输编程数据
LH_ERR Can2SubActionBoardSetProgData(CAN2_SUB_BOARD boardID,uint16_t dataLength,uint8_t* dataBuffer);

//CRC校验
LH_ERR Can2SubActionBoardSetProgCRC(CAN2_SUB_BOARD boardID,uint32_t crcValue);

//开始升级
LH_ERR Can2SubActionBoardStartPorg(CAN2_SUB_BOARD boardID);


#endif




