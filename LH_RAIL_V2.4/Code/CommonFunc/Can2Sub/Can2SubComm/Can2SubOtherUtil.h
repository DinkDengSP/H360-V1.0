/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-30 10:25:25
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-15 10:13:50
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_OTHER_UTIL_H_
#define __CAN2_SUB_OTHER_UTIL_H_
#include "Can2ProcMain.h"

//当前动作板的状态,是什么模式
typedef enum ACTION_BOARD_CURRENT_STATE
{
    ACTION_BOARD_CURRENT_STATE_BOOT = 0,
    ACTION_BOARD_CURRENT_STATE_APP = 1,
}ACTION_BOARD_CURRENT_STATE;

//读取下位机版本
typedef enum READ_SOFTWARE_VERSION
{
    READ_SOFTWARE_VERSION_BOOT  = 0X00,//读取boot版本
    READ_SOFTWARE_VERSION_APP   = 0X01,//读取App版本
}READ_SOFTWARE_VERSION;

//辅助动作的指令
typedef enum CAN2_SUB_UTIL_CMD_CODE
{
    //APP可以接受的指令
    CAN2_SUB_UTIL_CMD_READ_MODE     = 0x00000000,
    CAN2_SUB_UTIL_CMD_READ_VER      = 0x00000001,
    CAN2_SUB_UTIL_CMD_JMP_BOOT      = 0x00000002,
    CAN2_SUB_UTIL_CMD_JMP_APP       = 0x00000002,
    CAN2_SUB_UTIL_CMD_PROG_LENGTH   = 0x00000003,
    CAN2_SUB_UTIL_CMD_PROG_DATA     = 0x00000004,
    CAN2_SUB_UTIL_CMD_PROG_CRC_CHECK= 0x00000005,
    CAN2_SUB_UTIL_CMD_PROG_START    = 0x00000006,
}CAN2_SUB_UTIL_CMD_CODE;

//查询当前动作板处于何种模式
LH_ERR Can2SubActionBoardReadCurrentState(CAN2_SUB_BOARD boardID,ACTION_BOARD_CURRENT_STATE* state);

//查询当前动作板程序版本
LH_ERR Can2SubActionBoardReadVersion(CAN2_SUB_BOARD boardID,READ_SOFTWARE_VERSION readSoftwareVersion,uint8_t* mainVersion,uint8_t* subVersion,uint8_t* debugVersion,uint32_t* svnVersion);

//动作板跳转进入BOOT
LH_ERR Can2SubActionBoardJumpBoot(CAN2_SUB_BOARD boardID);

//动作板跳转进入APP
LH_ERR Can2SubActionBoardJumpApp(CAN2_SUB_BOARD boardID);

//通讯握手,也就是读取模式,必须处于APP模式才能叫做握手成功
LH_ERR Can2SubActionBoardHandShake(CAN2_SUB_BOARD boardID);

//传输开始编程的信息
LH_ERR Can2SubActionBoardSetProgDataLength(CAN2_SUB_BOARD boardID,uint32_t pragDataLengthBytes);

//传输编程数据
LH_ERR Can2SubActionBoardSetProgData(CAN2_SUB_BOARD boardID,uint16_t dataLength,uint8_t* dataBuffer);

//CRC校验
LH_ERR Can2SubActionBoardSetProgCRC(CAN2_SUB_BOARD boardID,uint32_t crcValue);

//开始升级
LH_ERR Can2SubActionBoardStartPorg(CAN2_SUB_BOARD boardID);



#endif



