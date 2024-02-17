/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-20 14:28:58
**LastEditors: DengXiaoJun
**LastEditTime: 2020-07-03 12:43:39
**FilePath: \App\TaskMain\AppSupport\AppTask\AppImpl\AppImplStateParam.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplStateParam.h"


//自动循环事件初始化
void AppImplStateParam_EventInit(void)
{

}

//自动循环事件
void AppImplStateParam_EventRun(uint32_t periodTimeMs)
{
    
}

/*************************************轨道主控自身使用的指令*****************************************/
//设置模块数量
LH_ERR AppImplStateParamSetModuleCount(uint8_t moduleCount,CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = IPC_RailComplexSetModuleCountWhileReturn(moduleCount,can1ErrorPtr,reportLevelResultPtr);
    return errorCode;
}

//软件急停
LH_ERR AppImplStateParamEmergencyStop(CAN1_REPORT_ERR_CODE* can1ErrorPtr,CAN1_REPORT_ERR_LEVEL* reportLevelResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = IPC_AssistEmergencyStopWhileReturn(can1ErrorPtr,reportLevelResultPtr);
    return errorCode;
}

//升级记录
static BOARD_IAP_RECORD boardIapRecord;

//初始化IAP讯息
void AppImplStateParamBoardIapInit(void)
{
    boardIapRecord.iapState = BOARD_IAP_IDLE;
    boardIapRecord.boardID = 0;
    boardIapRecord.exceptRecvDataLengthBytes = 0;
    boardIapRecord.exceptRecvDataPackCount = 0;
    boardIapRecord.singlePackLengthBytesSet = 0;
    boardIapRecord.currentRecvDataLengthBytes = 0;
    boardIapRecord.currentRecvDataPackCount = 0;
}

//开始升级准备
BOARD_IAP_STATE AppImplStateParamIapPrepareUpdata(uint8_t boardID)
{
    LH_ERR errorCode = LH_ERR_NONE;
    ACTION_BOARD_CURRENT_STATE boardCurrentState = ACTION_BOARD_CURRENT_STATE_BOOT;
    boardIapRecord.boardID = boardID;
    //下位机板处理
    if(boardIapRecord.boardID == CAN1_RAIL_ACT_BOARD_ID)
    {
        //读取当前下位机的模式
        errorCode = Can2SubActionBoardReadCurrentState(CAN2_SUB_BOARD8,&boardCurrentState);
        if(errorCode != LH_ERR_NONE)
        {
            SystemDebugShowMsgError(errorCode);
            return BOARD_IAP_ERR;
        }
        //如果是APP模式,跳转到老化模式
        if(boardCurrentState == ACTION_BOARD_CURRENT_STATE_APP)
        {
            //跳转到BOOT
            Can2SubActionBoardJumpBoot(CAN2_SUB_BOARD8);
            //延时等待3S
            CoreDelayMs(3000);
            //读取当前的模式
            errorCode = Can2SubActionBoardReadCurrentState(CAN2_SUB_BOARD8,&boardCurrentState);
            if(errorCode != LH_ERR_NONE)
            {
                SystemDebugShowMsgError(errorCode);
                return BOARD_IAP_ERR;
            }
            //此时应该是BOOT模式
            if(boardCurrentState == ACTION_BOARD_CURRENT_STATE_APP)
            {
                SystemDebugShowMsgError(errorCode);
                return BOARD_IAP_ERR;
            }
            return BOARD_IAP_IDLE;
        }
    }
    else
    {
        SystemDebugShowMsgError(errorCode);
        return BOARD_IAP_ERR;
    }
    return BOARD_IAP_BUSY;
}

//写入起始讯息
BOARD_IAP_STATE AppImplStateParamBoardIapWriteStartMsg(uint8_t boardID,uint32_t exceptBytes,uint16_t exceptPackCount,uint16_t singlePackLength)
{
    boardIapRecord.boardID = boardID;
    boardIapRecord.iapState = BOARD_IAP_BUSY;
    boardIapRecord.exceptRecvDataLengthBytes = exceptBytes;
    boardIapRecord.exceptRecvDataPackCount = exceptPackCount;
    boardIapRecord.singlePackLengthBytesSet = singlePackLength;
    boardIapRecord.currentRecvDataLengthBytes = 0;
    boardIapRecord.currentRecvDataPackCount = 0;
    LH_ERR errorCode = LH_ERR_NONE;
    //数据信息写入下位机
    if(boardIapRecord.boardID == CAN1_RAIL_ACT_BOARD_ID)
    {
        //写入起始讯息
        errorCode = Can2SubActionBoardSetProgDataLength(CAN2_SUB_BOARD8,boardIapRecord.exceptRecvDataLengthBytes);
        if(errorCode != LH_ERR_NONE)
        {
            SystemDebugShowMsgError(errorCode);
            return BOARD_IAP_ERR;
        }
    }
    else
    {
        //不支持的型号
        return BOARD_IAP_ERR;
    }
    return boardIapRecord.iapState;
}

#define SINGLE_PACK_LENGTH      396

//写入数据讯息
BOARD_IAP_STATE AppImplStateParamBoardIapWriteData(uint8_t boardID,uint16_t packIndex,uint16_t dataLength,uint8_t* dataBuffer)
{
    if(boardIapRecord.iapState != BOARD_IAP_BUSY)
    {//状态不对
        boardIapRecord.iapState = BOARD_IAP_ERR;
        return BOARD_IAP_ERR;
    }
    if(boardIapRecord.boardID != boardID)
    {//运行时板卡ID变化
        boardIapRecord.iapState = BOARD_IAP_ERR;
        return BOARD_IAP_ERR;
    }
    if(dataLength != boardIapRecord.singlePackLengthBytesSet)
    {//单包数据长度不对
        if(packIndex != (boardIapRecord.exceptRecvDataPackCount -1))
        {
            boardIapRecord.iapState = BOARD_IAP_ERR;
            return BOARD_IAP_ERR;
        }
    }
    if(packIndex != boardIapRecord.currentRecvDataPackCount)
    {//包序号和想要的序号不一致
        boardIapRecord.iapState = BOARD_IAP_ERR;
        return BOARD_IAP_ERR;
    }
    //一次发送256字节
    LH_ERR errorCode = LH_ERR_NONE;
    uint16_t localDataLengthUtil = dataLength;
    if(boardIapRecord.boardID == CAN1_RAIL_ACT_BOARD_ID)
    {
        do
        {
            if(localDataLengthUtil >= SINGLE_PACK_LENGTH)
            {
                //写入数据
                errorCode = Can2SubActionBoardSetProgData(CAN2_SUB_BOARD8,SINGLE_PACK_LENGTH,dataBuffer+dataLength-localDataLengthUtil);
                if(errorCode != LH_ERR_NONE)
                {
                    SystemDebugShowMsgError(errorCode);
                    return BOARD_IAP_ERR;
                }
                localDataLengthUtil -= SINGLE_PACK_LENGTH;
            }
            else
            {
                //写入数据
                errorCode = Can2SubActionBoardSetProgData(CAN2_SUB_BOARD8,localDataLengthUtil,dataBuffer+dataLength-localDataLengthUtil);
                if(errorCode != LH_ERR_NONE)
                {
                    SystemDebugShowMsgError(errorCode);
                    return BOARD_IAP_ERR;
                }
                localDataLengthUtil = 0;
            }
            CoreDelayMinTick();
        }while(localDataLengthUtil != 0);
    }
    else
    {
        //不支持的型号
        return BOARD_IAP_ERR;
    }
    //计数增加
    boardIapRecord.currentRecvDataLengthBytes += dataLength;
    //包数增加
    boardIapRecord.currentRecvDataPackCount++;
    //返回状态
    return boardIapRecord.iapState;
}

//计算结果校验和
BOARD_IAP_STATE AppImplStateParamBoardIapCalcCRC32(uint8_t boardID,uint32_t crc32Result)
{
    if(boardIapRecord.iapState != BOARD_IAP_BUSY)
    {
        boardIapRecord.iapState = BOARD_IAP_ERR;
        return BOARD_IAP_ERR;
    }
    if(boardIapRecord.boardID != boardID)
    {//运行时板卡ID变化
        boardIapRecord.iapState = BOARD_IAP_ERR;
        return BOARD_IAP_ERR;
    }
    if(boardIapRecord.currentRecvDataPackCount != boardIapRecord.exceptRecvDataPackCount)
    {//包数未达到
        boardIapRecord.iapState = BOARD_IAP_ERR;
        return BOARD_IAP_ERR;
    }
    if(boardIapRecord.currentRecvDataLengthBytes != boardIapRecord.exceptRecvDataLengthBytes)
    {//总长度未达到
        boardIapRecord.iapState = BOARD_IAP_ERR;
        return BOARD_IAP_ERR;
    }
    LH_ERR errorCode = LH_ERR_NONE;
    //计算校验和
    if(boardIapRecord.boardID == CAN1_RAIL_ACT_BOARD_ID)
    {
        errorCode = Can2SubActionBoardSetProgCRC(CAN2_SUB_BOARD8,crc32Result);
        if(errorCode != LH_ERR_NONE)
        {
            SystemDebugShowMsgError(errorCode);
            return BOARD_IAP_ERR;
        }
    }
    else
    {
        //不支持的型号
        return BOARD_IAP_ERR;
    }
    //完成
    return boardIapRecord.iapState;
}

//开始将数据写入Flash,直到升级完成
BOARD_IAP_STATE AppImplStateParamIapStartUpdata(uint8_t boardID)
{
    ACTION_BOARD_CURRENT_STATE boardCurrentState = ACTION_BOARD_CURRENT_STATE_BOOT;
    if(boardIapRecord.iapState != BOARD_IAP_BUSY)
    {
        boardIapRecord.iapState = BOARD_IAP_ERR;
        return BOARD_IAP_ERR;
    }
    if(boardIapRecord.boardID != boardID)
    {//运行时板卡ID变化
        boardIapRecord.iapState = BOARD_IAP_ERR;
        return BOARD_IAP_ERR;
    }
    if(boardIapRecord.currentRecvDataPackCount != boardIapRecord.exceptRecvDataPackCount)
    {//包数未达到
        boardIapRecord.iapState = BOARD_IAP_ERR;
        return BOARD_IAP_ERR;
    }
    if(boardIapRecord.currentRecvDataLengthBytes != boardIapRecord.exceptRecvDataLengthBytes)
    {//总长度未达到
        boardIapRecord.iapState = BOARD_IAP_ERR;
        return BOARD_IAP_ERR;
    }
    LH_ERR errorCode = LH_ERR_NONE;
    //开始升级
    if(boardIapRecord.boardID == CAN1_RAIL_ACT_BOARD_ID)
    {
        //开始升级
        errorCode = Can2SubActionBoardStartPorg(CAN2_SUB_BOARD8);
        if(errorCode != LH_ERR_NONE)
        {
            SystemDebugShowMsgError(errorCode);
            return BOARD_IAP_ERR;
        }
		//延时等待
		CoreDelayMs(1000);
        //跳转APP
        Can2SubActionBoardJumpApp(CAN2_SUB_BOARD8);
        //延时时间
        CoreDelayMs(3000);
        //读取当前的模式
        errorCode = Can2SubActionBoardReadCurrentState(CAN2_SUB_BOARD8,&boardCurrentState);
        if(errorCode != LH_ERR_NONE)
        {
            SystemDebugShowMsgError(errorCode);
            return BOARD_IAP_ERR;
        }
        //此时应该是APP模式
        if(boardCurrentState == ACTION_BOARD_CURRENT_STATE_BOOT)
        {
            SystemDebugShowMsgError(errorCode);
            return BOARD_IAP_ERR;
        }
        boardIapRecord.iapState = BOARD_IAP_IDLE;
    }
    else
    {
        //不支持的型号
        return BOARD_IAP_ERR;
    }
    return boardIapRecord.iapState;
}

//程序跳转
BOARD_IAP_STATE AppImplStateParamIapProgramJump(uint8_t boardID,uint8_t *bootOrApp)
{
    LH_ERR errorCode = LH_ERR_NONE;
    ACTION_BOARD_CURRENT_STATE boardLastState = ACTION_BOARD_CURRENT_STATE_BOOT;
    ACTION_BOARD_CURRENT_STATE boardNewState = ACTION_BOARD_CURRENT_STATE_BOOT;
    //下位机板处理
    if(boardID == CAN1_RAIL_ACT_BOARD_ID)
    {
        //读取当前下位机的模式
        errorCode = Can2SubActionBoardReadCurrentState(CAN2_SUB_BOARD8,&boardLastState);
        if(errorCode != LH_ERR_NONE)
        {
            SystemDebugShowMsgError(errorCode);
            return BOARD_IAP_ERR;
        }
        //App-->Boot
        if(boardLastState == ACTION_BOARD_CURRENT_STATE_APP)
        {
            //跳转到BOOT
            Can2SubActionBoardJumpBoot(CAN2_SUB_BOARD8);
            //延时等待3S
            CoreDelayMs(3000);
            //读取当前的模式
            errorCode = Can2SubActionBoardReadCurrentState(CAN2_SUB_BOARD8,&boardNewState);
            if(errorCode != LH_ERR_NONE)
            {
                SystemDebugShowMsgError(errorCode);
                return BOARD_IAP_ERR;
            }
            //此时应该是BOOT模式
            if(boardNewState == ACTION_BOARD_CURRENT_STATE_APP)
            {
                SystemDebugShowMsgError(errorCode);
                return BOARD_IAP_ERR;
            }
            *bootOrApp = 0;
            return BOARD_IAP_IDLE;
        }
        //Boot-->App
        else
        {
            //跳转到App
            Can2SubActionBoardJumpApp(CAN2_SUB_BOARD8);
            //延时等待3S
            CoreDelayMs(3000);
            //读取当前的模式
            errorCode = Can2SubActionBoardReadCurrentState(CAN2_SUB_BOARD8,&boardNewState);
            if(errorCode != LH_ERR_NONE)
            {
                SystemDebugShowMsgError(errorCode);
                return BOARD_IAP_ERR;
            }
            //此时应该是BOOT模式
            if(boardNewState == ACTION_BOARD_CURRENT_STATE_BOOT)
            {
                SystemDebugShowMsgError(errorCode);
                return BOARD_IAP_ERR;
            }
            *bootOrApp = 1;
            return BOARD_IAP_IDLE;
        }
    }
    else
    {
        SystemDebugShowMsgError(errorCode);
        return BOARD_IAP_ERR;
    }
}

