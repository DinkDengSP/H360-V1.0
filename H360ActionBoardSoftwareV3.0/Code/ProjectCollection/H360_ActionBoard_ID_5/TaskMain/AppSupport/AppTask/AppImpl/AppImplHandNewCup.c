/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 15:31:48
**LastEditors: DengXiaoJun
**LastEditTime: 2020-07-10 11:17:47
**FilePath: \H360_ActionBoard_ID_5\TaskMain\AppSupport\AppTask\AppImpl\AppImplHandNewCup.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplHandNewCup.h"

//转换新杯机械手错误代码的标识
typedef enum FLAG_HAND_NEW_CUP_ERR_CONVERT
{
    FLAG_HAND_NEW_CUP_ERR_RESET_BACKFRONT                   = 0X01,//前后复位错误
    FLAG_HAND_NEW_CUP_ERR_RESET_LEFTRIGHT                   = 0X02,//左右复位错误
    FLAG_HAND_NEW_CUP_ERR_RESET_UPDOWN                      = 0X03,//升降复位错误
    FLAG_HAND_NEW_CUP_ERR_RUN_BACKFRONT                     = 0X04,//前后运行错误
    FLAG_HAND_NEW_CUP_ERR_RUN_LEFTRIGHT                     = 0X05,//左右运行错误
    FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN                        = 0X06,//升降运行错误
    FLAG_HAND_NEW_CUP_ERR_RETURN_ZERO_UPDOWN                = 0X07,//升降回零错误
    FLAG_HAND_NEW_CUP_ERR_READ_LIGHT_SENSOR                 = 0X08,//读取光电传感器讯息错误
    FLAG_HAND_NEW_CUP_ERR_WRITE_ELECTRO_MAGNET              = 0X09,//写入电磁铁状态错误
    FLAG_HAND_NEW_CUP_ERR_POS_LIMIT_CATCH_NEW_TRAY          = 0X0A,//位置限制,当前位置不能抓取新杯
    FLAG_HAND_NEW_CUP_ERR_POS_LIMIT_CATCH_PLATE_REACTION    = 0X0B,//位置限制,当前位置不能执行反应盘抓杯
    FLAG_HAND_NEW_CUP_ERR_POS_LIMIT_PUT_PLATE_REACTION      = 0X0C,//位置限制,当前位置不能执行反应盘放杯
    FLAG_HAND_NEW_CUP_ERR_POS_LIMIT_PUT_GARBAGE             = 0X0D,//位置限制,当前位置不能执行垃圾桶放杯
}FLAG_HAND_NEW_CUP_ERR_CONVERT;

//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_HAND_NEW_CUP;
    moduleNo <<= 24;
    //错误标志
    uint32_t flagInsert = flag;
    flagInsert <<= 16;
    //基础错误代码
    errResult = LH_ERR_BOARD5_BASE;
    //加入模块号
    errResult |= moduleNo;
    //加入错误标志
    errResult |= flagInsert;
    //加入原始错误代码
    errResult |= errSrc;
    //返回转换完成的错误代码
    return errResult;
}

//当前机械手的平面位置
static HAND_NEW_CUP_POS currentHandNewCupPos = HAND_NEW_CUP_POS_NEW_TRAY;

//计算机械手左右运行曲线
static uint8_t AppImplHandNewCupCalcStepMotorLeftRightCurve(HAND_NEW_CUP_POS targetPos)
{
    //目标位置和当前位置一致,用调试曲线
    if(targetPos == currentHandNewCupPos)return S_CURVE_HAND_NEW_CUP_LEFTRIGHT_DEBUG;
    //当前处于新杯区
    if(currentHandNewCupPos == HAND_NEW_CUP_POS_NEW_TRAY)
    {
        if(targetPos == HAND_NEW_CUP_POS_GARBAGE_1)
        {
            return S_CURVE_HAND_NEW_CUP_LEFTRIGHT_NEW_TRAY_TO_GARBAGE1;
        }
        else if(targetPos == HAND_NEW_CUP_POS_GARBAGE_2)
        {
            return S_CURVE_HAND_NEW_CUP_LEFTRIGHT_NEW_TRAY_TO_GARBAGE2;
        }
        else if(targetPos == HAND_NEW_CUP_POS_PLATE_REACTION)
        {
            return S_CURVE_HAND_NEW_CUP_LEFTRIGHT_NEW_TRAY_TO_PLATE_REACTION;
        }
    }
    //当前处于垃圾桶1
    else if(currentHandNewCupPos == HAND_NEW_CUP_POS_GARBAGE_1)
    {
        if(targetPos == HAND_NEW_CUP_POS_NEW_TRAY)
        {
            return S_CURVE_HAND_NEW_CUP_LEFTRIGHT_GARBAGE1_TO_NEW_TRAY;
        }
        else if(targetPos == HAND_NEW_CUP_POS_GARBAGE_2)
        {
            return S_CURVE_HAND_NEW_CUP_LEFTRIGHT_GARBAGE1_TO_GARBAGE2;
        }
        else if(targetPos == HAND_NEW_CUP_POS_PLATE_REACTION)
        {
            return S_CURVE_HAND_NEW_CUP_LEFTRIGHT_GARBAGE1_TO_PLATE_REACTION;
        }
    }
    //当前处于垃圾桶2
    else if(currentHandNewCupPos == HAND_NEW_CUP_POS_GARBAGE_2)
    {
        if(targetPos == HAND_NEW_CUP_POS_NEW_TRAY)
        {
            return S_CURVE_HAND_NEW_CUP_LEFTRIGHT_GARBAGE2_TO_NEW_TRAY;
        }
        else if(targetPos == HAND_NEW_CUP_POS_GARBAGE_1)
        {
            return S_CURVE_HAND_NEW_CUP_LEFTRIGHT_GARBAGE2_TO_NEW_GARBAGE1;
        }
        else if(targetPos == HAND_NEW_CUP_POS_PLATE_REACTION)
        {
            return S_CURVE_HAND_NEW_CUP_LEFTRIGHT_GARBAGE2_TO_PLATE_REACTION;
        }
    }
    //当前处于反应盘
    else if(currentHandNewCupPos == HAND_NEW_CUP_POS_PLATE_REACTION)
    {
        if(targetPos == HAND_NEW_CUP_POS_NEW_TRAY)
        {
            return S_CURVE_HAND_NEW_CUP_LEFTRIGHT_PLATE_REACTION_TO_NEW_TRAY;
        }
        else if(targetPos == HAND_NEW_CUP_POS_GARBAGE_1)
        {
            return S_CURVE_HAND_NEW_CUP_LEFTRIGHT_PLATE_REACTION_TO_GARBAGE1;
        }
        else if(targetPos == HAND_NEW_CUP_POS_GARBAGE_2)
        {
            return S_CURVE_HAND_NEW_CUP_LEFTRIGHT_PLATE_REACTION_TO_GARBAGE2;
        }
    }
    //未知处理
    return S_CURVE_HAND_NEW_CUP_LEFTRIGHT_DEBUG;
}

//计算机械手前后运行曲线
static uint8_t AppImplHandNewCupCalcStepMotorBackFrontCurve(HAND_NEW_CUP_POS targetPos)
{
    //目标位置和当前位置一致,用调试曲线
    if(targetPos == currentHandNewCupPos)return S_CURVE_HAND_NEW_CUP_BACKFRONT_DEBUG;
    //当前处于新杯区
    if(currentHandNewCupPos == HAND_NEW_CUP_POS_NEW_TRAY)
    {
        if(targetPos == HAND_NEW_CUP_POS_GARBAGE_1)
        {
            return S_CURVE_HAND_NEW_CUP_BACKFRONT_NEW_TRAY_TO_GARBAGE1;
        }
        else if(targetPos == HAND_NEW_CUP_POS_GARBAGE_2)
        {
            return S_CURVE_HAND_NEW_CUP_BACKFRONT_NEW_TRAY_TO_GARBAGE2;
        }
        else if(targetPos == HAND_NEW_CUP_POS_PLATE_REACTION)
        {
            return S_CURVE_HAND_NEW_CUP_BACKFRONT_NEW_TRAY_TO_PLATE_REACTION;
        }
    }
    //当前处于垃圾桶1
    else if(currentHandNewCupPos == HAND_NEW_CUP_POS_GARBAGE_1)
    {
        if(targetPos == HAND_NEW_CUP_POS_NEW_TRAY)
        {
            return S_CURVE_HAND_NEW_CUP_BACKFRONT_GARBAGE1_TO_NEW_TRAY;
        }
        else if(targetPos == HAND_NEW_CUP_POS_GARBAGE_2)
        {
            return S_CURVE_HAND_NEW_CUP_BACKFRONT_GARBAGE1_TO_GARBAGE2;
        }
        else if(targetPos == HAND_NEW_CUP_POS_PLATE_REACTION)
        {
            return S_CURVE_HAND_NEW_CUP_BACKFRONT_GARBAGE1_TO_PLATE_REACTION;
        }
    }
    //当前处于垃圾桶2
    else if(currentHandNewCupPos == HAND_NEW_CUP_POS_GARBAGE_2)
    {
        if(targetPos == HAND_NEW_CUP_POS_NEW_TRAY)
        {
            return S_CURVE_HAND_NEW_CUP_BACKFRONT_GARBAGE2_TO_NEW_TRAY;
        }
        else if(targetPos == HAND_NEW_CUP_POS_GARBAGE_1)
        {
            return S_CURVE_HAND_NEW_CUP_BACKFRONT_GARBAGE2_TO_NEW_GARBAGE1;
        }
        else if(targetPos == HAND_NEW_CUP_POS_PLATE_REACTION)
        {
            return S_CURVE_HAND_NEW_CUP_BACKFRONT_GARBAGE2_TO_PLATE_REACTION;
        }
    }
    //当前处于反应盘
    else if(currentHandNewCupPos == HAND_NEW_CUP_POS_PLATE_REACTION)
    {
        if(targetPos == HAND_NEW_CUP_POS_NEW_TRAY)
        {
            return S_CURVE_HAND_NEW_CUP_BACKFRONT_PLATE_REACTION_TO_NEW_TRAY;
        }
        else if(targetPos == HAND_NEW_CUP_POS_GARBAGE_1)
        {
            return S_CURVE_HAND_NEW_CUP_BACKFRONT_PLATE_REACTION_TO_GARBAGE1;
        }
        else if(targetPos == HAND_NEW_CUP_POS_GARBAGE_2)
        {
            return S_CURVE_HAND_NEW_CUP_BACKFRONT_PLATE_REACTION_TO_GARBAGE2;
        }
    }
    //未知处理
    return S_CURVE_HAND_NEW_CUP_BACKFRONT_DEBUG;
}

/* 读新杯机械手反应盘光电传感器 */
static LH_ERR AppImplHandNewCupReadSensorStateReact(SENSOR_STATE *sensorStateReact)
{
    LH_ERR errorCode = LH_ERR_NONE;
    /* 读反应盘旧杯传感器计数 */
    uint8_t sensorReadCount = 0;
    /* 有效信号计数 */
    uint8_t sensorStateValidCount = 0;

    //读取光电状态
        BitAction inputState;
        do
        {
            /* 信号稳定延时 */
            CoreDelayMs(TIME_MS_DELAY_READ_HAND_NEW_CUP_EXIST_SENSOR_REACT);
            /* 读信号 */
            errorCode = CanSubInputReadState(INPUT_PLATE_REACTION_NEW_CUP_LIGHT_SENSOR,&inputState);
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_READ_LIGHT_SENSOR,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            //转换光纤状态
            if(inputState == VALID_LEVEL_INPUT_PLATE_REACTION_NEW_CUP_LIGHT_SENSOR)
            {
                sensorStateValidCount++;
            }
            sensorReadCount++;
        } while (sensorReadCount < READ_HAND_NEW_CUP_EXIST_SENSOR_REACT);
        
    //转换光纤状态
        if(sensorStateValidCount >= HAND_NEW_CUP_EXIST_SENSOR_VALID_THRESHOLD_REACT)
        {
            *sensorStateReact = SENSOR_STATE_VALID;
        }
        else
        {
            *sensorStateReact = SENSOR_STATE_INVALID;
        }
    return LH_ERR_NONE;
}

//新杯机械手复位
LH_ERR AppImplHandNewCupReset(void)                
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数配置
        //参数辅助,用于系统参数读取
        int32_t paramUtil;
        //复位指令结构体
        StepMotorResetCmd resetCmd;
    //电磁铁释放
        errorCode = CanSubOutPutWriteSingle(OUTPUT_HAND_NEW_CUP_ELECTRO_MAGNET,(BitAction)(!VALID_LEVEL_OUTPUT_HAND_NEW_CUP_ELECTRO_MAGNET));
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_WRITE_ELECTRO_MAGNET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //读取升降复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_RESET_CORRECT_UP_DOWN,&paramUtil);
    //配置复位指令结构体
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
        resetCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//复位修正
        resetCmd.timeOutSet = 60000;//超时时间
    //开始复位
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RESET_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //读取左右复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_RESET_CORRECT_LEFT_RIGHT,&paramUtil);
    //配置复位指令结构体
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
        resetCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_LEFTRIGHT_ID;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//复位修正
        resetCmd.timeOutSet = 60000;//超时时间
    //开始复位
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RESET_LEFTRIGHT,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //读取前后复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_RESET_CORRECT_FRONT_BACK,&paramUtil);
    //配置复位指令结构体
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
        resetCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_BACKFRONT_ID;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//复位修正
        resetCmd.timeOutSet = 60000;//超时时间
    //开始复位
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RESET_BACKFRONT,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前位置
        currentHandNewCupPos = HAND_NEW_CUP_POS_NEW_TRAY;
    //返回完成
        return errorCode;
}

//新杯机械手移动到新杯区
LH_ERR AppImplHandNewCupMoveToNewCupTray(uint8_t lineBackFront,uint8_t colLeftRight)     
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度,升降先回零
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_RESET_CORRECT_UP_DOWN,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_DEBUG;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //辅助计算的参数
    float coordinateUtil = 0.0;
    int32_t holePos1,holePos140;
    //读取新杯左右位置
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_POS_NEW_CUP_TRAY_LEFT_RIGHT_HOLE_NO_1,&holePos1);
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_POS_NEW_CUP_TRAY_LEFT_RIGHT_HOLE_NO_140,&holePos140);
        coordinateUtil = (float)(holePos140 - holePos1);
        coordinateUtil /= (HOLE_LEFT_RIGHT_MAX-1);
    //获取计算步数曲线
        uint8_t curveSelect =  AppImplHandNewCupCalcStepMotorLeftRightCurve(HAND_NEW_CUP_POS_NEW_TRAY);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_LEFTRIGHT_ID;//电机序号
        runToPosCmd.targetCoordinate = (int32_t)(holePos1 + (coordinateUtil*(colLeftRight -1)));//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileAck(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_LEFTRIGHT,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //读取新杯前后位置
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_POS_NEW_CUP_TRAY_FRONT_BACK_HOLE_NO_1,&holePos1);
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_POS_NEW_CUP_TRAY_FRONT_BACK_HOLE_NO_140,&holePos140);
        coordinateUtil = (float)(holePos140 - holePos1);
        coordinateUtil /= (HOLE_BACK_FRONT_MAX-1);
    //获取计算步数曲线
        curveSelect =  AppImplHandNewCupCalcStepMotorBackFrontCurve(HAND_NEW_CUP_POS_NEW_TRAY);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_BACKFRONT_ID;//电机序号
        runToPosCmd.targetCoordinate = (int32_t)(holePos1 + (coordinateUtil*(lineBackFront -1)));//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_BACKFRONT,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //等待左右运行完成
        errorCode = IPC_StepMotorWaitLastReturn(HAND_NEW_CUP_STEP_MOTOR_LEFTRIGHT_ID);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_LEFTRIGHT,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前位置
        currentHandNewCupPos = HAND_NEW_CUP_POS_NEW_TRAY;
    //返回完成
        return errorCode;
}

//新杯机械手移动到反应盘
LH_ERR AppImplHandNewCupMoveToPlateReaction(void)  
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度,升降先回零
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_RESET_CORRECT_UP_DOWN,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_DEBUG;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //读取反应盘前后
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_POS_FRONT_BACK_PLATE_REACTION,&paramUtil);
    //获取计算步数曲线
        uint8_t curveSelect =  AppImplHandNewCupCalcStepMotorBackFrontCurve(HAND_NEW_CUP_POS_PLATE_REACTION);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_BACKFRONT_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
    //电机运行,等待完成
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_BACKFRONT,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //读取反应盘左右
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_POS_LEFT_RIGHT_PLATE_REACTION,&paramUtil);
    //获取计算步数曲线
        curveSelect =  AppImplHandNewCupCalcStepMotorLeftRightCurve(HAND_NEW_CUP_POS_PLATE_REACTION);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_LEFTRIGHT_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
    //电机运行,不等待完成
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_LEFTRIGHT,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前位置
        currentHandNewCupPos = HAND_NEW_CUP_POS_PLATE_REACTION;
    //返回完成
        return errorCode;
}

//新杯机械手移动到垃圾桶
LH_ERR AppImplHandNewCupMoveToGarbage(HAND_NEW_CUP_GARBAGE_NO garbageNo)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度,升降先回零
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_RESET_CORRECT_UP_DOWN,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_DEBUG;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //速度曲线
        uint8_t curveSelect = 0;
    //读取垃圾桶左右
        if(garbageNo == HAND_NEW_CUP_GARBAGE_NO_1)
        {
            AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_POS_LEFT_RIGHT_GARBAGE_1,&paramUtil);
            curveSelect = AppImplHandNewCupCalcStepMotorLeftRightCurve(HAND_NEW_CUP_POS_GARBAGE_1);
        }
        else
        {
            AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_POS_LEFT_RIGHT_GARBAGE_2,&paramUtil);
            curveSelect = AppImplHandNewCupCalcStepMotorLeftRightCurve(HAND_NEW_CUP_POS_GARBAGE_2);
        }
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_LEFTRIGHT_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
    //电机运行,不等待完成
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_LEFTRIGHT,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //读取垃圾桶前后
        if(garbageNo == HAND_NEW_CUP_GARBAGE_NO_1)
        {
            AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_POS_FRONT_BACK_GARBAGE_1,&paramUtil);
            curveSelect = AppImplHandNewCupCalcStepMotorBackFrontCurve(HAND_NEW_CUP_POS_GARBAGE_1);
        }
        else
        {
            AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_POS_FRONT_BACK_GARBAGE_2,&paramUtil);
            curveSelect = AppImplHandNewCupCalcStepMotorBackFrontCurve(HAND_NEW_CUP_POS_GARBAGE_2);
        }
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_BACKFRONT_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
    //电机运行,等待完成
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_BACKFRONT,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前位置
        if(garbageNo == HAND_NEW_CUP_GARBAGE_NO_1)
        {
            currentHandNewCupPos = HAND_NEW_CUP_POS_GARBAGE_1;
        }
        else
        {
            currentHandNewCupPos = HAND_NEW_CUP_POS_GARBAGE_2;
        }
    //返回完成
        return errorCode;
}        
//新杯机械手新杯区取杯
LH_ERR AppImplHandNewCupCatchCupNewCupTray(void)   
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先检查当前位置对不对
        if(currentHandNewCupPos != HAND_NEW_CUP_POS_NEW_TRAY)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_POS_LIMIT_CATCH_NEW_TRAY,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机走坐标结构体
        int32_t paramUtil;
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度,升降先回零
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_RESET_CORRECT_UP_DOWN,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_DEBUG;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //获取抓取位置
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_POS_NEW_CUP_TRAY_UP_DOWN_CATCH_CUP,&paramUtil);
    //升降先快速运行到变速点
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil - OFFSET_SPEED_SWITCH_UPDOWN_NEW_TRAY_CATCH_DOWN;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_CATCH_NEW_TRAY_DOWN_FAST;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //如果配置了切换点
    #if(OFFSET_SPEED_SWITCH_UPDOWN_NEW_TRAY_CATCH_DOWN != 0)
        //如果配置了切换速度延时
        #if(TIME_MS_HAND_NEW_CUP_UPDOWN_SPEED_SWITCH != 0)
            CoreDelayMsSensitivity(TIME_MS_HAND_NEW_CUP_UPDOWN_SPEED_SWITCH);
        #endif
        //切换速度运转到目标位置
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_CATCH_NEW_TRAY_DOWN_SLOW;//速度曲线
        runToPosCmd.timeOutSet = 5000;
        //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    #endif
    //延时取杯等待稳定时间
        CoreDelayMsSensitivity(TIME_MS_HAND_NEW_CUP_CATCH_CUP_NEW_TRAY_STABLE);
    //如果配置了切换点,慢速抬起到切换点
    #if(OFFSET_SPEED_SWITCH_UPDOWN_NEW_TRAY_CATCH_UP != 0)
        //切换速度运转到目标位置
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil-OFFSET_SPEED_SWITCH_UPDOWN_NEW_TRAY_CATCH_UP;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_CATCH_NEW_TRAY_UP_SLOW;//速度曲线
        runToPosCmd.timeOutSet = 5000;
        //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //如果配置了切换速度延时
        #if(TIME_MS_HAND_NEW_CUP_UPDOWN_SPEED_SWITCH != 0)
            CoreDelayMsSensitivity(TIME_MS_HAND_NEW_CUP_UPDOWN_SPEED_SWITCH);
        #endif
    #endif
    //升降抬起来回零
        StepMotorReturnZeroCmd returnZeroCmd;
    //读取升降回零修正
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_RESET_CORRECT_UP_DOWN,&paramUtil);
    //升降回零结构体的配置,设置速度曲线和目标坐标
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_CATCH_NEW_TRAY_UP_FAST;
        returnZeroCmd.timeOutSet = 5000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RETURN_ZERO_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return errorCode;
}
//新杯机械手反应盘取杯
LH_ERR AppImplHandNewCupCatchCupPlateReaction(SENSOR_STATE* cupExistPlateReactionNew)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先检查当前位置对不对
        if(currentHandNewCupPos != HAND_NEW_CUP_POS_PLATE_REACTION)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_POS_LIMIT_CATCH_PLATE_REACTION,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机走坐标结构体
        int32_t paramUtil;
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度,升降先回零
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_RESET_CORRECT_UP_DOWN,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_DEBUG;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //获取抓取位置
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_POS_PLATE_REACTION_UP_DOWN_CATCH_CUP,&paramUtil);
    //升降先快速运行到变速点
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil - OFFSET_SPEED_SWITCH_UPDOWN_PLATE_REACTION_CATCH_DOWN;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_CATCH_PLATE_REACTION_DOWN_FAST;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //如果配置了切换点
    #if(OFFSET_SPEED_SWITCH_UPDOWN_PLATE_REACTION_CATCH_DOWN != 0)
        //如果配置了切换速度延时
        #if(TIME_MS_HAND_NEW_CUP_UPDOWN_SPEED_SWITCH != 0)
            CoreDelayMsSensitivity(TIME_MS_HAND_NEW_CUP_UPDOWN_SPEED_SWITCH);
        #endif
        //切换速度运转到目标位置
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_CATCH_PLATE_REACTION_DOWN_SLOW;//速度曲线
        runToPosCmd.timeOutSet = 5000;
        //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    #endif
    //延时取杯等待稳定时间
        CoreDelayMsSensitivity(TIME_MS_HAND_NEW_CUP_CATCH_CUP_PLATE_REACTION_STABLE);
    //如果配置了切换点,慢速抬起到切换点
    #if(OFFSET_SPEED_SWITCH_UPDOWN_PLATE_REACTION_CATCH_UP != 0)
        //切换速度运转到目标位置
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil-OFFSET_SPEED_SWITCH_UPDOWN_PLATE_REACTION_CATCH_UP;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_CATCH_PLATE_REACTION_UP_SLOW;//速度曲线
        runToPosCmd.timeOutSet = 5000;
        //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //如果配置了切换速度延时
        #if(TIME_MS_HAND_NEW_CUP_UPDOWN_SPEED_SWITCH != 0)
            CoreDelayMsSensitivity(TIME_MS_HAND_NEW_CUP_UPDOWN_SPEED_SWITCH);
        #endif
    #endif
    //升降抬起来回零
        StepMotorReturnZeroCmd returnZeroCmd;
    //读取升降回零修正
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_RESET_CORRECT_UP_DOWN,&paramUtil);
    //升降回零结构体的配置,设置速度曲线和目标坐标
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_CATCH_PLATE_REACTION_UP_FAST;
        returnZeroCmd.timeOutSet = 5000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RETURN_ZERO_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //延时等待光电传感器稳定
        CoreDelayMsSensitivity(TIME_MS_PLATE_REACTION_LIGHT_SENSOR_STABLE);
    //读取光电传感器信息
        errorCode = AppImplHandNewCupReadSensorStateReact(cupExistPlateReactionNew);
    return errorCode;
}

//新杯机械手反应盘放杯
LH_ERR AppImplHandNewCupPutCupPlateReaction(SENSOR_STATE* cupExistPlateReactionNew)  
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先检查当前位置对不对
        if(currentHandNewCupPos != HAND_NEW_CUP_POS_PLATE_REACTION)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_POS_LIMIT_CATCH_PLATE_REACTION,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机走坐标结构体
        int32_t paramUtil;
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度,升降先回零
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_RESET_CORRECT_UP_DOWN,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_DEBUG;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //合上电磁铁
        errorCode = CanSubOutPutWriteSingle(OUTPUT_HAND_NEW_CUP_ELECTRO_MAGNET,VALID_LEVEL_OUTPUT_HAND_NEW_CUP_ELECTRO_MAGNET);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_WRITE_ELECTRO_MAGNET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }    
    //延时等待电磁铁稳定
        CoreDelayMsSensitivity(TIME_MS_PLATE_REACTION_ELECTRO_MAGNET_STABLE);
    //获取放杯位置
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_POS_PLATE_REACTION_UP_DOWN_PUT_CUP,&paramUtil);
    //升降先快速运行到变速点
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil - OFFSET_SPEED_SWITCH_UPDOWN_PLATE_REACTION_PUT_DOWN;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_PUT_PLATE_REACTION_DOWN_FAST;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //如果配置了切换点
    #if(OFFSET_SPEED_SWITCH_UPDOWN_PLATE_REACTION_PUT_DOWN != 0)
        //如果配置了切换速度延时
        #if(TIME_MS_HAND_NEW_CUP_UPDOWN_SPEED_SWITCH != 0)
            CoreDelayMsSensitivity(TIME_MS_HAND_NEW_CUP_UPDOWN_SPEED_SWITCH);
        #endif
        //切换速度运转到目标位置
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_PUT_PLATE_REACTION_DOWN_SLOW;//速度曲线
        runToPosCmd.timeOutSet = 5000;
        //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    #endif
    //延时放杯等待稳定时间
        CoreDelayMsSensitivity(TIME_MS_HAND_NEW_CUP_PUT_CUP_PLATE_REACTION_STABLE);
    //如果配置了切换点,慢速抬起到切换点
    #if(OFFSET_SPEED_SWITCH_UPDOWN_PLATE_REACTION_PUT_UP != 0)
        //切换速度运转到目标位置
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil-OFFSET_SPEED_SWITCH_UPDOWN_PLATE_REACTION_PUT_UP;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_PUT_PLATE_REACTION_UP_SLOW;//速度曲线
        runToPosCmd.timeOutSet = 5000;
        //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //如果配置了切换速度延时
        #if(TIME_MS_HAND_NEW_CUP_UPDOWN_SPEED_SWITCH != 0)
            CoreDelayMsSensitivity(TIME_MS_HAND_NEW_CUP_UPDOWN_SPEED_SWITCH);
        #endif
    #endif
    //升降抬起来回零
        StepMotorReturnZeroCmd returnZeroCmd;
    //读取升降回零修正
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_RESET_CORRECT_UP_DOWN,&paramUtil);
    //升降回零结构体的配置,设置速度曲线和目标坐标
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_PUT_PLATE_REACTION_UP_FAST;
        returnZeroCmd.timeOutSet = 5000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RETURN_ZERO_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //打开电磁铁
        errorCode = CanSubOutPutWriteSingle(OUTPUT_HAND_NEW_CUP_ELECTRO_MAGNET,(BitAction)(!VALID_LEVEL_OUTPUT_HAND_NEW_CUP_ELECTRO_MAGNET));
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_WRITE_ELECTRO_MAGNET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //延时等待光电传感器稳定
        CoreDelayMsSensitivity(TIME_MS_PLATE_REACTION_LIGHT_SENSOR_STABLE);
    //读取光电传感器信息
        errorCode = AppImplHandNewCupReadSensorStateReact(cupExistPlateReactionNew);
    return errorCode;
}

//新杯机械手垃圾桶放杯
LH_ERR AppImplHandNewCupPutCupGarbage(HAND_NEW_CUP_GARBAGE_NO garbageNo)        
{
    LH_ERR errorCode = LH_ERR_NONE;
    //首先检查当前位置对不对
        if((garbageNo == HAND_NEW_CUP_GARBAGE_NO_1)&&(currentHandNewCupPos != HAND_NEW_CUP_POS_GARBAGE_1))
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_POS_LIMIT_PUT_GARBAGE,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        if((garbageNo == HAND_NEW_CUP_GARBAGE_NO_2)&&(currentHandNewCupPos != HAND_NEW_CUP_POS_GARBAGE_2))
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_POS_LIMIT_PUT_GARBAGE,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机走坐标结构体
        int32_t paramUtil;
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度,升降先回零
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_RESET_CORRECT_UP_DOWN,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_DEBUG;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //获取放杯位置
        if(garbageNo == HAND_NEW_CUP_GARBAGE_NO_1)
        {
            AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_POS_UP_DOWN_PUT_CUP_GARBAGE_1,&paramUtil);            
        }
        else
        {
            AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_POS_UP_DOWN_PUT_CUP_GARBAGE_2,&paramUtil);
        }
    //升降先快速运行到变速点
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil - OFFSET_SPEED_SWITCH_UPDOWN_GARBAGE_PUT_DOWN;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_PUT_GARBAGE_DOWN_FAST;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //如果配置了切换点
    #if(OFFSET_SPEED_SWITCH_UPDOWN_GARBAGE_PUT_DOWN != 0)
        //如果配置了切换速度延时
        #if(TIME_MS_HAND_NEW_CUP_UPDOWN_SPEED_SWITCH != 0)
            CoreDelayMsSensitivity(TIME_MS_HAND_NEW_CUP_UPDOWN_SPEED_SWITCH);
        #endif
        //切换速度运转到目标位置
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_PUT_GARBAGE_DOWN_SLOW;//速度曲线
        runToPosCmd.timeOutSet = 5000;
        //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    #endif
    //延时放杯等待稳定时间
        CoreDelayMsSensitivity(TIME_MS_HAND_NEW_CUP_PUT_CUP_GARBAGE_STABLE);
    //如果配置了切换点,慢速抬起到切换点
    #if(OFFSET_SPEED_SWITCH_UPDOWN_GARBAGE_PUT_UP != 0)
        //切换速度运转到目标位置
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil-OFFSET_SPEED_SWITCH_UPDOWN_GARBAGE_PUT_UP;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_PUT_GARBAGE_UP_SLOW;//速度曲线
        runToPosCmd.timeOutSet = 5000;
        //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RUN_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //如果配置了切换速度延时
        #if(TIME_MS_HAND_NEW_CUP_UPDOWN_SPEED_SWITCH != 0)
            CoreDelayMsSensitivity(TIME_MS_HAND_NEW_CUP_UPDOWN_SPEED_SWITCH);
        #endif
    #endif
    //升降抬起来回零
        StepMotorReturnZeroCmd returnZeroCmd;
    //读取升降回零修正
        AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_RESET_CORRECT_UP_DOWN,&paramUtil);
    //升降回零结构体的配置,设置速度曲线和目标坐标
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_HAND_NEW_CUP_UPDOWN_PUT_GARBAGE_UP_FAST;
        returnZeroCmd.timeOutSet = 5000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplHandNewCupConvertErrorCode(FLAG_HAND_NEW_CUP_ERR_RETURN_ZERO_UPDOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return errorCode;
}

//步进电机调试配置表
static const MODULE_STEP_MOTOR_RESET_RUN_CONFIG moduleStepMotorResetRunConfigArray[] = {
    {HAND_NEW_CUP_STEP_MOTOR_BACKFRONT_ID,INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_RESET_CORRECT_FRONT_BACK },//新杯机械手前后
    {HAND_NEW_CUP_STEP_MOTOR_LEFTRIGHT_ID,INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_RESET_CORRECT_LEFT_RIGHT },//新杯机械手左右
    {HAND_NEW_CUP_STEP_MOTOR_UPDOWN_ID,INDEX_CAN_SUB_BOARD5_PARAM_HAND_NEW_CUP,APP_PARAM_SUB_INDEX_HAND_NEW_CUP_RESET_CORRECT_UP_DOWN       },//新杯机械手升降
};

//指定步进电机复位
LH_ERR AppImplHandNewCupStepMotorReset(INDEX_MOTOR_HAND_NEW_CUP index)       
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //复位指令结构体
        StepMotorResetCmd resetCmd;
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
    //读取修正
        AppParamRead((INDEX_CAN_SUB_BOARD5_PARAM_MAIN)(moduleStepMotorResetRunConfigArray[index].resetCorrectMainIndex),
                        moduleStepMotorResetRunConfigArray[index].resetCorrectSubIndex,
                        &paramUtil);
    //填充参数
        resetCmd.motorIndex = moduleStepMotorResetRunConfigArray[index].motorIndex;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//复位修正
        resetCmd.timeOutSet = 60000;//超时时间
    //开始复位
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }

    return LH_ERR_NONE;
}

//指定步进电机走指定步数
LH_ERR AppImplHandNewCupStepMotorRunSteps(INDEX_MOTOR_HAND_NEW_CUP index,int32_t steps,int32_t* posAfterRun)    
{
    LH_ERR errorCode = LH_ERR_NONE;
    //走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //填充参数
        runStepCmd.motorIndex = moduleStepMotorResetRunConfigArray[index].motorIndex;
        runStepCmd.specialSteps = steps;
        runStepCmd.selectConfig = SM_CURVE_0;//默认曲线0作为调试曲线
    //开始走步数
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //读取结果
        errorCode = IPC_StepMotorReflushStateWhileReturn(runStepCmd.motorIndex,posAfterRun);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}




