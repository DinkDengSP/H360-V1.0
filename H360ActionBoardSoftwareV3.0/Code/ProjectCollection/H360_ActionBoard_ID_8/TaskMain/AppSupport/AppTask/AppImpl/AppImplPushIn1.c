/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-27 19:47:48
**FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppTask\AppImpl\AppImplPushIn1.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplPushIn1.h"

//转换进样1错误代码的标识
typedef enum FLAG_PUSH_IN_1_ERR_CONVERT
{
    FLAG_PUSH_IN_1_ERR_RESET                    = 0X01,//进样1复位失败
    FLAG_PUSH_IN_1_ERR_RUNNING                  = 0X02,//进样1运行失败
    FLAG_PUSH_IN_1_ERR_RETURN_ZERO              = 0X03,//进样1回零失败
    FLAG_PUSH_IN_1_ERR_SENSOR_READ              = 0X04,//进样1传感器读取错误
    FLAG_PUSH_IN_1_ERR_RESET_SENSOR_STATE       = 0X05,//轨道进样1复位时试管架存在传感器有效,此时不应该有效
    FLAG_PUSH_IN_1_ERR_RETURN_SENSOR_STATE      = 0X06,//轨道进样1回零时试管架存在传感器有效,此时不应该有效
    FLAG_PUSH_IN_1_ERR_PUSH_SENSOR_STATE        = 0X07,//轨道进样1试管架存在传感器有效,不能前推
}FLAG_PUSH_IN_1_ERR_CONVERT;

//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplPushIn1ConvertErrorCode(FLAG_PUSH_IN_1_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_PUSH_IN_1;
    moduleNo <<= 24;
    //错误标志
    uint32_t flagInsert = flag;
    flagInsert <<= 16;
    //基础错误代码
    errResult = LH_ERR_BOARD8_RAIL_BASE;
    //加入模块号
    errResult |= moduleNo;
    //加入错误标志
    errResult |= flagInsert;
    //加入原始错误代码
    errResult |= errSrc;
    //返回转换完成的错误代码
    return errResult;
}

//进样1复位
LH_ERR AppImplPushIn1Reset(RAIL_RESET_CORRECT_FLAG correctFlag)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        BitAction rackExistSensorState = Bit_RESET;
        int32_t paramUtil = 0;
        StepMotorResetCmd resetCmd;
    //首先复位
        if(correctFlag == RAIL_RESET_CORRECT_OFF)
        {
            paramUtil = 0;
        }
        else
        {
            AppParamReadParam(APP_PARAM_MAIN_INDEX_PUSH_IN1,APP_PARAM_INDEX_SUB_PUSH_IN_RESET_CORRECTION,&paramUtil);
        }
    //开始复位
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
        resetCmd.motorIndex = PUSH_IN_1_STEP_MOTOR_ID;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//复位修正
        resetCmd.timeOutSet = 55000;//超时时间
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPushIn1ConvertErrorCode(FLAG_PUSH_IN_1_ERR_RESET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //读取传感器
        errorCode = IPC_InputReadPinValueWhileReturn(INPUT_PUSHIN1_RACK_EXIST,&rackExistSensorState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPushIn1ConvertErrorCode(FLAG_PUSH_IN_1_ERR_SENSOR_READ,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //检查传感器状态
        if(rackExistSensorState == VALID_LEVEL_PUSHIN1_RACK_EXIST)
        {
            errorCode = AppImplPushIn1ConvertErrorCode(FLAG_PUSH_IN_1_ERR_RESET_SENSOR_STATE,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //正常完成
        return LH_ERR_NONE;
}

//进样1推进一次
LH_ERR AppImplPushIn1PushOnce(SENSOR_STATE_PUSHIN_EXIST* rackExistSensorStatePtr)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        LH_ERR errorCodeUtil = LH_ERR_NONE;
        BitAction rackExistSensorState = Bit_RESET;
        //回零结构体
        StepMotorReturnZeroCmd returnZeroCmd;
        //电机走步数结构体
        StepMotorRun2CoordationCmd runToPosCmd;
        int32_t paramUtil = 0;
    //首先进样轨道回零
        AppParamReadParam(APP_PARAM_MAIN_INDEX_PUSH_IN1,APP_PARAM_INDEX_SUB_PUSH_IN_RESET_CORRECTION,&paramUtil);
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = PUSH_IN_1_STEP_MOTOR_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = CURVE_PUSHIN1_RETURN_ZERO;
        returnZeroCmd.timeOutSet = 55000;
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            //回零失败
            errorCode = AppImplPushIn1ConvertErrorCode(FLAG_PUSH_IN_1_ERR_RETURN_ZERO,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //检查试管架存在状态,推架子之前不能有效,否则推不了架子
        errorCode = IPC_InputReadPinValueWhileReturn(INPUT_PUSHIN1_RACK_EXIST,&rackExistSensorState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPushIn1ConvertErrorCode(FLAG_PUSH_IN_1_ERR_SENSOR_READ,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        if(rackExistSensorState == VALID_LEVEL_PUSHIN1_RACK_EXIST)
        {
            errorCode = AppImplPushIn1ConvertErrorCode(FLAG_PUSH_IN_1_ERR_PUSH_SENSOR_STATE,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //读取坐标,走到指定坐标,如果急停,说明撞到了架子,如果不急停,说明没碰到架子
        AppParamReadParam(APP_PARAM_MAIN_INDEX_PUSH_IN1,APP_PARAM_INDEX_SUB_PUSH_IN_MAX_POS,&paramUtil);
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = PUSH_IN_1_STEP_MOTOR_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = CURVE_PUSHIN1_PUSH;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
    //既不是急停也不是正常错误代码,说明系统错误
        if((errorCode != LH_ERR_SERVICE_SM_EMERGENCY_SIGNAL)&&(errorCode != LH_ERR_NONE))
        {
            //回零
                AppParamReadParam(APP_PARAM_MAIN_INDEX_PUSH_IN1,APP_PARAM_INDEX_SUB_PUSH_IN_RESET_CORRECTION,&paramUtil);
                IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
                returnZeroCmd.motorIndex = PUSH_IN_1_STEP_MOTOR_ID;
                returnZeroCmd.correctionCoordinate = paramUtil;
                returnZeroCmd.selectConfig = CURVE_PUSHIN1_RETURN_ZERO;
                returnZeroCmd.timeOutSet = 55000;
                errorCodeUtil = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
                if(errorCodeUtil != LH_ERR_NONE)
                {
                    //回零失败
                    errorCodeUtil = AppImplPushIn1ConvertErrorCode(FLAG_PUSH_IN_1_ERR_RETURN_ZERO,errorCodeUtil);
                    AppShowErrMsg(errorCodeUtil);
                    return errorCodeUtil;
                }
            //回零完成,上报错误代码
                errorCode = AppImplPushIn1ConvertErrorCode(FLAG_PUSH_IN_1_ERR_RUNNING,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
        }
    //急停,调用回零
        if(errorCode == LH_ERR_SERVICE_SM_EMERGENCY_SIGNAL)
        {
            //设置状态为触发
                *rackExistSensorStatePtr = SENSOR_STATE_PUSHIN_EXIST_TRIG;
            //回零
                AppParamReadParam(APP_PARAM_MAIN_INDEX_PUSH_IN1,APP_PARAM_INDEX_SUB_PUSH_IN_RESET_CORRECTION,&paramUtil);
                IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
                returnZeroCmd.motorIndex = PUSH_IN_1_STEP_MOTOR_ID;
                returnZeroCmd.correctionCoordinate = paramUtil;
                returnZeroCmd.selectConfig = CURVE_PUSHIN1_RETURN_ZERO;
                returnZeroCmd.timeOutSet = 55000;
                errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
                if(errorCode != LH_ERR_NONE)
                {
                    //回零失败
                    errorCode = AppImplPushIn1ConvertErrorCode(FLAG_PUSH_IN_1_ERR_RETURN_ZERO,errorCode);
                    AppShowErrMsg(errorCode);
                    return errorCode;
                }
        }
    //不是急停,调用回到复位修正
        else if(errorCode == LH_ERR_NONE)
        {
            //设置状态为未触发
                *rackExistSensorStatePtr = SENSOR_STATE_PUSHIN_EXIST_NONE;
            //没有触发也直接回零
                AppParamReadParam(APP_PARAM_MAIN_INDEX_PUSH_IN1,APP_PARAM_INDEX_SUB_PUSH_IN_RESET_CORRECTION,&paramUtil);
                IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
                returnZeroCmd.motorIndex = PUSH_IN_1_STEP_MOTOR_ID;
                returnZeroCmd.correctionCoordinate = paramUtil;
                returnZeroCmd.selectConfig = CURVE_PUSHIN1_RETURN_ZERO;
                returnZeroCmd.timeOutSet = 55000;
                errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
                if(errorCode != LH_ERR_NONE)
                {
                    //回零失败
                    errorCode = AppImplPushIn1ConvertErrorCode(FLAG_PUSH_IN_1_ERR_RETURN_ZERO,errorCode);
                    AppShowErrMsg(errorCode);
                    return errorCode;
                }
        }
    //回到零位了,检查此时试管架存在状态,此时应该为无效,读取传感器
        errorCode = IPC_InputReadPinValueWhileReturn(INPUT_PUSHIN1_RACK_EXIST,&rackExistSensorState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplPushIn1ConvertErrorCode(FLAG_PUSH_IN_1_ERR_SENSOR_READ,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //检查传感器状态
        if(rackExistSensorState == VALID_LEVEL_PUSHIN1_RACK_EXIST)
        {
            errorCode = AppImplPushIn1ConvertErrorCode(FLAG_PUSH_IN_1_ERR_RETURN_SENSOR_STATE,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //完成
    return LH_ERR_NONE;
}

//进样1推进多次直到传感器触发
LH_ERR AppImplPushIn1PushMultiWhileSensorTrig(uint16_t retryMax,SENSOR_STATE_PUSHIN_EXIST* rackExistSensorStatePtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint16_t retryCount = 0;
    for(retryCount = 0; retryCount < retryMax;retryCount++)
    {
        //调用单次推架子
        errorCode = AppImplPushIn1PushOnce(rackExistSensorStatePtr);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
        //如果推到架子有效,即可停止
        if(*rackExistSensorStatePtr == SENSOR_STATE_PUSHIN_EXIST_TRIG)
        {
            return LH_ERR_NONE;
        }
    }
    //循环多次依然触发不了传感器
    *rackExistSensorStatePtr = SENSOR_STATE_PUSHIN_EXIST_NONE;
    return LH_ERR_NONE;
}




