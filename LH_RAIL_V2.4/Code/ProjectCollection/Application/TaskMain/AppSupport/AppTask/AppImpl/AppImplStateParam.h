/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-20 14:29:07
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-07-03 09:05:53
 *FilePath: \App\TaskMain\AppSupport\AppTask\AppImpl\AppImplStateParam.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_STATE_PARAM_H_
#define __APP_IMPL_STATE_PARAM_H_
#include "AppImplBase.h"

//自动循环事件初始化
void AppImplStateParam_EventInit(void);

//自动循环事件
void AppImplStateParam_EventRun(uint32_t periodTimeMs);


/*************************************轨道主控自身使用的指令*****************************************/
//设置模块数量
LH_ERR AppImplStateParamSetModuleCount(uint8_t moduleCount,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);

//软件急停
LH_ERR AppImplStateParamEmergencyStop(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr);


/************************************************在线升级相关功能*********************************************/
//IAP的状态
typedef enum BOARD_IAP_STATE
{
    BOARD_IAP_IDLE = 0X00,
    BOARD_IAP_BUSY = 0X01,
    BOARD_IAP_ERR  = 0X02,
}BOARD_IAP_STATE;

typedef struct BOARD_IAP_RECORD
{
    uint8_t boardID;
    BOARD_IAP_STATE iapState;
    uint32_t exceptRecvDataLengthBytes;//期望接收的数据长度
    uint16_t exceptRecvDataPackCount;//期望接收的数据总包数
    uint16_t singlePackLengthBytesSet;//期望接收的每一包数据的有效数据长度
    uint32_t currentRecvDataLengthBytes;//当前已经接收到的数据长度
    uint16_t currentRecvDataPackCount;//当前已经接收到的数据包数
}BOARD_IAP_RECORD;

//初始化IAP讯息
void AppImplStateParamBoardIapInit(void);

//开始升级准备
BOARD_IAP_STATE AppImplStateParamIapPrepareUpdata(uint8_t boardID);

//写入起始讯息
BOARD_IAP_STATE AppImplStateParamBoardIapWriteStartMsg(uint8_t boardID,uint32_t exceptBytes,uint16_t exceptPackCount,uint16_t singlePackLength);

//写入数据讯息
BOARD_IAP_STATE AppImplStateParamBoardIapWriteData(uint8_t boardID,uint16_t packIndex,uint16_t dataLength,uint8_t* dataBuffer);

//计算结果校验和
BOARD_IAP_STATE AppImplStateParamBoardIapCalcCRC32(uint8_t boardID,uint32_t crc32Result);

//开始将数据写入Flash
BOARD_IAP_STATE AppImplStateParamIapStartUpdata(uint8_t boardID);

//开始升级准备
BOARD_IAP_STATE AppImplStateParamIapProgramJump(uint8_t boardID,uint8_t *bootOrApp);

#endif




