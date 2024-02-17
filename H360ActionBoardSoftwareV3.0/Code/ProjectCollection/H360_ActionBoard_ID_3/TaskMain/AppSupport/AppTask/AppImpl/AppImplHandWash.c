/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 14:39:09
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-21 16:38:23
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_3\TaskMain\AppSupport\AppTask\AppImpl\AppImplHandWash.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplHandWash.h"


//清洗机械手复位(带零位偏移)
LH_ERR AppImplHandWashReset(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数初始化
    int32_t paramUtil = 0;
    //读清洗机械手版本
    errorCode = AppParamRead(INDEX_CAN_SUB_BOARD3_PARAM_HAND_WASH,APP_PARAM_SUB_INDEX_HAND_WASH_VERSION,&paramUtil);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowErrMsg(errorCode);
        return errorCode; 
    }
    //参数判断
    if(paramUtil == 0)
    {
        return AppImplHandWashResetVersion0();
    }
    else
    {
        return AppImplHandWashResetVersion1();
    }
}

//清洗机械手旋转到反应盘
LH_ERR AppImplHandWashRotateToPlateReaction(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数初始化
    int32_t paramUtil = 0;
    //读清洗机械手版本
    errorCode = AppParamRead(INDEX_CAN_SUB_BOARD3_PARAM_HAND_WASH,APP_PARAM_SUB_INDEX_HAND_WASH_VERSION,&paramUtil);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowErrMsg(errorCode);
        return errorCode; 
    }
    //参数判断
    if(paramUtil == 0)
    {
        return AppImplHandWashRotateToPlateReactionVersion0();
    }
    else
    {
        return AppImplHandWashRotateToPlateReactionVersion1();
    }
}

//清洗机械手旋转到清洗盘
LH_ERR AppImplHandWashRotateToPlateWash(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数初始化
    int32_t paramUtil = 0;
    //读清洗机械手版本
    errorCode = AppParamRead(INDEX_CAN_SUB_BOARD3_PARAM_HAND_WASH,APP_PARAM_SUB_INDEX_HAND_WASH_VERSION,&paramUtil);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowErrMsg(errorCode);
        return errorCode; 
    }
    //参数判断
    if(paramUtil == 0)
    {
        return AppImplHandWashRotateToPlateWashVersion0();
    }
    else
    {
        return AppImplHandWashRotateToPlateWashVersion1();
    }
}

//清洗机械手旋转到测量模块
LH_ERR AppImplHandWashRotateToMeasureUnit(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数初始化
    int32_t paramUtil = 0;
    //读清洗机械手版本
    errorCode = AppParamRead(INDEX_CAN_SUB_BOARD3_PARAM_HAND_WASH,APP_PARAM_SUB_INDEX_HAND_WASH_VERSION,&paramUtil);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowErrMsg(errorCode);
        return errorCode; 
    }
    //参数判断
    if(paramUtil == 0)
    {
        return AppImplHandWashRotateToMeasureUnitVersion0();
    }
    else
    {
        return AppImplHandWashRotateToMeasureUnitVersion1();
    }
}

////升降回零
//static LH_ERR AppImplHandWashUpdownReturnZero(void)
//{
//    LH_ERR errorCode = LH_ERR_NONE;
//    //升降抬起来回零
//        StepMotorReturnZeroCmd returnZeroCmd;
//        int32_t paramUtil;
//    //读取回零修正
//        AppParamRead(INDEX_CAN_SUB_BOARD3_PARAM_HAND_WASH,APP_PARAM_SUB_INDEX_HAND_WASH_UPDOWN_RESET_CORRECT,&paramUtil);
//    //填充回零参数
//        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
//        returnZeroCmd.motorIndex = HAND_WASH_STEP_MOTOR_UPDOWN_ID;
//        returnZeroCmd.correctionCoordinate = paramUtil;
//        returnZeroCmd.selectConfig = S_CURVE_HAND_WASH_UPDOWN_DEBUG;
//        returnZeroCmd.timeOutSet = 5000;
//    //电机回零,等待完成
//        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
//    //返回错误代码
//    return errorCode;
//}

//清洗机械手反应盘取杯
LH_ERR AppImplHandWashCatchCupOnPlateReaction(SENSOR_STATE* cupExistPlateReactionOld)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数初始化
    int32_t paramUtil = 0;
    //读清洗机械手版本
    errorCode = AppParamRead(INDEX_CAN_SUB_BOARD3_PARAM_HAND_WASH,APP_PARAM_SUB_INDEX_HAND_WASH_VERSION,&paramUtil);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowErrMsg(errorCode);
        return errorCode; 
    }
    //参数判断
    if(paramUtil == 0)
    {
        return AppImplHandWashCatchCupOnPlateReactionVersion0(cupExistPlateReactionOld);
    }
    else
    {
        return AppImplHandWashCatchCupOnPlateReactionVersion1(cupExistPlateReactionOld);
    }
}

//清洗机械手反应盘抛杯
LH_ERR AppImplHandWashPutCupOnPlateReaction(SENSOR_STATE* cupExistPlateReactionOld)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数初始化
    int32_t paramUtil = 0;
    //读清洗机械手版本
    errorCode = AppParamRead(INDEX_CAN_SUB_BOARD3_PARAM_HAND_WASH,APP_PARAM_SUB_INDEX_HAND_WASH_VERSION,&paramUtil);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowErrMsg(errorCode);
        return errorCode; 
    }
    //参数判断
    if(paramUtil == 0)
    {
        return AppImplHandWashPutCupOnPlateReactionVersion0(cupExistPlateReactionOld);
    }
    else
    {
        return AppImplHandWashPutCupOnPlateReactionVersion1(cupExistPlateReactionOld);
    }
}

//清洗机械手清洗盘取杯
LH_ERR AppImplHandWashCatchCupOnPlateWash(SENSOR_STATE* cupExistPlateWashNew)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数初始化
    int32_t paramUtil = 0;
    //读清洗机械手版本
    errorCode = AppParamRead(INDEX_CAN_SUB_BOARD3_PARAM_HAND_WASH,APP_PARAM_SUB_INDEX_HAND_WASH_VERSION,&paramUtil);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowErrMsg(errorCode);
        return errorCode; 
    }
    //参数判断
    if(paramUtil == 0)
    {
        return AppImplHandWashCatchCupOnPlateWashVersion0(cupExistPlateWashNew);
    }
    else
    {
        return AppImplHandWashCatchCupOnPlateWashVersion1(cupExistPlateWashNew);
    }
}

//清洗机械手清洗盘抛杯
LH_ERR AppImplHandWashPutCupOnPlateWash(SENSOR_STATE* cupExistPlateWashNew)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数初始化
    int32_t paramUtil = 0;
    //读清洗机械手版本
    errorCode = AppParamRead(INDEX_CAN_SUB_BOARD3_PARAM_HAND_WASH,APP_PARAM_SUB_INDEX_HAND_WASH_VERSION,&paramUtil);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowErrMsg(errorCode);
        return errorCode; 
    }
    //参数判断
    if(paramUtil == 0)
    {
        return AppImplHandWashPutCupOnPlateWashVersion0(cupExistPlateWashNew);
    }
    else
    {
        return AppImplHandWashPutCupOnPlateWashVersion1(cupExistPlateWashNew);
    }
}

//清洗机械手测量模块取杯
LH_ERR AppImplHandWashCatchCupOnMeasureUnit(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数初始化
    int32_t paramUtil = 0;
    //读清洗机械手版本
    errorCode = AppParamRead(INDEX_CAN_SUB_BOARD3_PARAM_HAND_WASH,APP_PARAM_SUB_INDEX_HAND_WASH_VERSION,&paramUtil);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowErrMsg(errorCode);
        return errorCode; 
    }
    //参数判断
    if(paramUtil == 0)
    {
        return AppImplHandWashCatchCupOnMeasureUnitVersion0();
    }
    else
    {
        return AppImplHandWashCatchCupOnMeasureUnitVersion1();
    }
}

//清洗机械手测量模块放杯
LH_ERR AppImplHandWashPutCupOnMeasureUnit(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数初始化
    int32_t paramUtil = 0;
    //读清洗机械手版本
    errorCode = AppParamRead(INDEX_CAN_SUB_BOARD3_PARAM_HAND_WASH,APP_PARAM_SUB_INDEX_HAND_WASH_VERSION,&paramUtil);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowErrMsg(errorCode);
        return errorCode; 
    }
    //参数判断
    if(paramUtil == 0)
    {
        return AppImplHandWashPutCupOnMeasureUnitVersion0();
    }
    else
    {
        return AppImplHandWashPutCupOnMeasureUnitVersion1();
    }
}

//指定电机复位
LH_ERR AppImplHandWashMotorReset(INDEX_MOTOR_HAND_WASH index)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数初始化
    int32_t paramUtil = 0;
    //读清洗机械手版本
    errorCode = AppParamRead(INDEX_CAN_SUB_BOARD3_PARAM_HAND_WASH,APP_PARAM_SUB_INDEX_HAND_WASH_VERSION,&paramUtil);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowErrMsg(errorCode);
        return errorCode; 
    }
    //参数判断
    if(paramUtil == 0)
    {
        return AppImplHandWashMotorResetVersion0(index);
    }
    else
    {
        return AppImplHandWashMotorResetVersion1(index);
    }
}

//指定电机走相应的步数
LH_ERR AppImplHandWashMotorRunSteps(INDEX_MOTOR_HAND_WASH index,int32_t steps,int32_t* posAfterRun)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数初始化
    int32_t paramUtil = 0;
    //读清洗机械手版本
    errorCode = AppParamRead(INDEX_CAN_SUB_BOARD3_PARAM_HAND_WASH,APP_PARAM_SUB_INDEX_HAND_WASH_VERSION,&paramUtil);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowErrMsg(errorCode);
        return errorCode; 
    }
    //参数判断
    if(paramUtil == 0)
    {
        return AppImplHandWashMotorRunStepsVersion0(index,steps,posAfterRun);
    }
    else
    {
        return AppImplHandWashMotorRunStepsVersion1(index,steps,posAfterRun);
    }
}




