/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:30:24
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 19:24:34
**FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplMixWash.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplMixWashVersion1.h"

//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_MIX_WASH;
    moduleNo <<= 24;
    //错误标志
    uint32_t flagInsert = flag;
    flagInsert <<= 16;
    //基础错误代码
    errResult = LH_ERR_BOARD1_BASE;
    //加入模块号
    errResult |= moduleNo;
    //加入错误标志
    errResult |= flagInsert;
    //加入原始错误代码
    errResult |= errSrc;
    //返回转换完成的错误代码
    return errResult;
}


/*****************************************************清洗混匀上升的函数*********************************************************/
//清洗混匀上升模式1
static LH_ERR AppImplMixWashUpMode1(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE1,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE1;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}

//清洗混匀上升模式2
static LH_ERR AppImplMixWashUpMode2(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE2,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE2;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}

//清洗混匀上升模式3
static LH_ERR AppImplMixWashUpMode3(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE3,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE3;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}

//清洗混匀上升模式4
static LH_ERR AppImplMixWashUpMode4(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE4,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE4;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}

//清洗混匀上升模式5
static LH_ERR AppImplMixWashUpMode5(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE5,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE5;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}

//清洗混匀上升模式6
static LH_ERR AppImplMixWashUpMode6(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE6,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE6;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}

//清洗混匀上升模式7
static LH_ERR AppImplMixWashUpMode7(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE7,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE7;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}

//清洗混匀上升模式8
static LH_ERR AppImplMixWashUpMode8(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE8,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE8;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}

//清洗混匀上升模式9
static LH_ERR AppImplMixWashUpMode9(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE9,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE9;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}

//清洗混匀上升模式10
static LH_ERR AppImplMixWashUpMode10(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE10,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE10;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}   

/***************************************************清洗混匀旋转不带升降的函数*******************************************************/
//清洗混匀旋转不带升降模式1
static LH_ERR AppImplMixWashStartRotateMode1(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
        int32_t paramUtil;
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE1,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_1_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}
//清洗混匀旋转不带升降模式2
static LH_ERR AppImplMixWashStartRotateMode2(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
        int32_t paramUtil;
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE2,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_2_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}
//清洗混匀旋转不带升降模式3
static LH_ERR AppImplMixWashStartRotateMode3(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
        int32_t paramUtil;
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE3,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_3_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}
//清洗混匀旋转不带升降模式4
static LH_ERR AppImplMixWashStartRotateMode4(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
        int32_t paramUtil;
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE4,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_4_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}
//清洗混匀旋转不带升降模式5
static LH_ERR AppImplMixWashStartRotateMode5(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
        int32_t paramUtil;
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE5,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_5_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}
//清洗混匀旋转不带升降模式6
static LH_ERR AppImplMixWashStartRotateMode6(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
        int32_t paramUtil;
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE6,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_6_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}
//清洗混匀旋转不带升降模式7
static LH_ERR AppImplMixWashStartRotateMode7(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
        int32_t paramUtil;
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE7,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_7_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}
//清洗混匀旋转不带升降模式8
static LH_ERR AppImplMixWashStartRotateMode8(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
        int32_t paramUtil;
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE8,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_8_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}
//清洗混匀旋转不带升降模式9
static LH_ERR AppImplMixWashStartRotateMode9(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
        int32_t paramUtil;
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE9,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_9_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}
//清洗混匀旋转不带升降模式10
static LH_ERR AppImplMixWashStartRotateMode10(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
        int32_t paramUtil;
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE10,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_10_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}
/****************************************************清洗混匀集成混匀的函数********************************************************/
//清洗混匀集成混匀的函数模式1
static LH_ERR AppImplMixWashRotateIntegratedMode1(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE1,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE1;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE1,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_1_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机回零结构体
        StepMotorReturnZeroCmd returnZeroCmd;
    //读取升降回零修正
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_RESET_CORRECT_UPDOWN,&paramUtil);
    //升降回零结构体的配置,设置速度曲线和目标坐标
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE1;
        returnZeroCmd.timeOutSet = 5000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_DOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //一次继承混匀完成
    return LH_ERR_NONE;
    
}

//清洗混匀集成混匀的函数模式2
static LH_ERR AppImplMixWashRotateIntegratedMode2(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE2,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE2;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE2,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_2_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机回零结构体
        StepMotorReturnZeroCmd returnZeroCmd;
    //读取升降回零修正
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_RESET_CORRECT_UPDOWN,&paramUtil);
    //升降回零结构体的配置,设置速度曲线和目标坐标
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE2;
        returnZeroCmd.timeOutSet = 5000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_DOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }

    //一次继承混匀完成
    return LH_ERR_NONE;
}

//清洗混匀集成混匀的函数模式3
static LH_ERR AppImplMixWashRotateIntegratedMode3(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE3,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE3;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE3,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_3_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机回零结构体
        StepMotorReturnZeroCmd returnZeroCmd;
    //读取升降回零修正
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_RESET_CORRECT_UPDOWN,&paramUtil);
    //升降回零结构体的配置,设置速度曲线和目标坐标
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE3;
        returnZeroCmd.timeOutSet = 5000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_DOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }

    //一次继承混匀完成
    return LH_ERR_NONE;
}

//清洗混匀集成混匀的函数模式4
static LH_ERR AppImplMixWashRotateIntegratedMode4(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE4,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE4;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE4,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_4_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机回零结构体
        StepMotorReturnZeroCmd returnZeroCmd;
    //读取升降回零修正
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_RESET_CORRECT_UPDOWN,&paramUtil);
    //升降回零结构体的配置,设置速度曲线和目标坐标
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE4;
        returnZeroCmd.timeOutSet = 5000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_DOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }

    //一次继承混匀完成
    return LH_ERR_NONE;
}

//清洗混匀集成混匀的函数模式5
static LH_ERR AppImplMixWashRotateIntegratedMode5(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE5,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE5;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE5,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_5_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机回零结构体
        StepMotorReturnZeroCmd returnZeroCmd;
    //读取升降回零修正
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_RESET_CORRECT_UPDOWN,&paramUtil);
    //升降回零结构体的配置,设置速度曲线和目标坐标
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE5;
        returnZeroCmd.timeOutSet = 5000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_DOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }

    //一次继承混匀完成
    return LH_ERR_NONE;
}

//清洗混匀集成混匀的函数模式6
static LH_ERR AppImplMixWashRotateIntegratedMode6(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE6,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE6;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE6,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_6_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机回零结构体
        StepMotorReturnZeroCmd returnZeroCmd;
    //读取升降回零修正
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_RESET_CORRECT_UPDOWN,&paramUtil);
    //升降回零结构体的配置,设置速度曲线和目标坐标
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE6;
        returnZeroCmd.timeOutSet = 5000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_DOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    
    //一次继承混匀完成
    return LH_ERR_NONE;
}

//清洗混匀集成混匀的函数模式7
static LH_ERR AppImplMixWashRotateIntegratedMode7(void)
{   
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE7,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE7;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE7,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_7_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机回零结构体
        StepMotorReturnZeroCmd returnZeroCmd;
    //读取升降回零修正
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_RESET_CORRECT_UPDOWN,&paramUtil);
    //升降回零结构体的配置,设置速度曲线和目标坐标
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE7;
        returnZeroCmd.timeOutSet = 5000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_DOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }

    //一次继承混匀完成
    return LH_ERR_NONE;
}

//清洗混匀集成混匀的函数模式8
static LH_ERR AppImplMixWashRotateIntegratedMode8(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE8,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE8;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE8,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_8_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机回零结构体
        StepMotorReturnZeroCmd returnZeroCmd;
    //读取升降回零修正
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_RESET_CORRECT_UPDOWN,&paramUtil);
    //升降回零结构体的配置,设置速度曲线和目标坐标
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE8;
        returnZeroCmd.timeOutSet = 5000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_DOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }

    //一次继承混匀完成
    return LH_ERR_NONE;
}

//清洗混匀集成混匀的函数模式9
static LH_ERR AppImplMixWashRotateIntegratedMode9(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE9,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE9;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE9,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_9_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机回零结构体
        StepMotorReturnZeroCmd returnZeroCmd;
    //读取升降回零修正
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_RESET_CORRECT_UPDOWN,&paramUtil);
    //升降回零结构体的配置,设置速度曲线和目标坐标
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE9;
        returnZeroCmd.timeOutSet = 5000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_DOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }

    //一次继承混匀完成
    return LH_ERR_NONE;
}

//清洗混匀集成混匀的函数模式10
static LH_ERR AppImplMixWashRotateIntegratedMode10(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取升起来的高度
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_UP_POS_MODE10,&paramUtil);
    //设置电机走位结构体
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_UP_MODE10;//速度曲线
        runToPosCmd.timeOutSet = 5000;
    //电机运行
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_UP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机持续运转结构体
        StepMotorRunAlwaysCmd runAlwaysCmd;
    //读取运转时间
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_ROTATE_TIME_MS_MODE10,&paramUtil);
    //设置电机持续运转指令
        IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
        runAlwaysCmd.motorIndex = MIX_WASH_STEP_MOTOR_ROTATE_ID;
        runAlwaysCmd.selectConfig = S_CURVE_MIX_WASH_ROTATE_MODE_10_VERSION1 ;
        runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    //电机开始持续运转
        errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //精确延时
        CoreDelayMsSensitivity(paramUtil);
    //电机停止
        errorCode = IPC_StepMotorStopDecelerationWhileReturn(runAlwaysCmd.motorIndex);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_STOP,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //电机回零结构体
        StepMotorReturnZeroCmd returnZeroCmd;
    //读取升降回零修正
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_RESET_CORRECT_UPDOWN,&paramUtil);
    //升降回零结构体的配置,设置速度曲线和目标坐标
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE10;
        returnZeroCmd.timeOutSet = 5000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_DOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //一次继承混匀完成
    return LH_ERR_NONE;
}

/***********************************************函数指针速度集中定义***************************************/
//清洗混匀上升的函数指针数组
static const AppImplMixWashUpFuncPtr AppImplMixWashUpFuncPtrArray[] = 
{
    AppImplMixWashUpMode1,
    AppImplMixWashUpMode2,
    AppImplMixWashUpMode3,
    AppImplMixWashUpMode4,
    AppImplMixWashUpMode5,
    AppImplMixWashUpMode6,
    AppImplMixWashUpMode7,
    AppImplMixWashUpMode8,
    AppImplMixWashUpMode9,
    AppImplMixWashUpMode10,
};

//清洗混匀旋转不带升降的函数指针数组
static const AppImplMixWashStartRotateFuncPtr AppImplMixWashStartRotateFuncPtrArray[] = {
    AppImplMixWashStartRotateMode1,
    AppImplMixWashStartRotateMode2,
    AppImplMixWashStartRotateMode3,
    AppImplMixWashStartRotateMode4,
    AppImplMixWashStartRotateMode5,
    AppImplMixWashStartRotateMode6,
    AppImplMixWashStartRotateMode7,
    AppImplMixWashStartRotateMode8,
    AppImplMixWashStartRotateMode9,
    AppImplMixWashStartRotateMode10,
};

//清洗混匀集成混匀的函数指针数组
static const AppImplMixWashRotateIntegratedFuncPtr AppImplMixWashRotateIntegratedFuncPtrArray[] = {
    AppImplMixWashRotateIntegratedMode1,
    AppImplMixWashRotateIntegratedMode2,
    AppImplMixWashRotateIntegratedMode3,
    AppImplMixWashRotateIntegratedMode4,
    AppImplMixWashRotateIntegratedMode5,
    AppImplMixWashRotateIntegratedMode6,
    AppImplMixWashRotateIntegratedMode7,
    AppImplMixWashRotateIntegratedMode8,
    AppImplMixWashRotateIntegratedMode9,
    AppImplMixWashRotateIntegratedMode10,
};

//清洗混匀模块复位,带零位偏移
LH_ERR AppImplMixWashResetVersion1(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数配置
        //参数辅助,用于系统参数读取
        int32_t paramUtil;
        //复位指令结构体
        StepMotorResetCmd resetCmd;
    //读取升降复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_RESET_CORRECT_UPDOWN,&paramUtil);
    //配置升降复制指令结构体
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
        resetCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//复位修正
        resetCmd.timeOutSet = 60000;//超时时间
    //开始复位
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_RESET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置旋转电机状态为复位状态
        errorCode = IPC_StepMotorSetStateWhileReturn(MIX_WASH_STEP_MOTOR_ROTATE_ID,STEP_MOTOR_MAIN_STATE_STOP,STEP_MOTOR_STOP_SUB_NORMAL);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_ROTATE_RESET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}


//清洗混匀升起来
LH_ERR AppImplMixWashUpVersion1(MIX_MODE mixMode)
{
    //模式修正
    if(mixMode > MIX_MODE_MAX)
    {
        mixMode = MIX_MODE_MAX;
    }
    //调用试剂的升起来的函数
    return AppImplMixWashUpFuncPtrArray[mixMode]();
}


//清洗混匀升降降下去
LH_ERR AppImplMixWashDownVersion1(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机回零结构体
    StepMotorReturnZeroCmd returnZeroCmd;
    //读取升降回零修正
        AppParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_RESET_CORRECT_UPDOWN,&paramUtil);
    //升降回零结构体的配置,设置速度曲线和目标坐标
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = MIX_WASH_STEP_MOTOR_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_MIX_WASH_UPDOWN_DOWN_MODE1;
        returnZeroCmd.timeOutSet = 5000;
    //电机回零
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplMixWashConvertErrorCode(FLAG_MIX_WASH_ERR_UPDOWN_DOWN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}


//清洗混匀旋转混匀一次,不带自动升降
LH_ERR AppImplMixWashStartRotateVersion1(MIX_MODE mixMode)
{
    //模式修正
    if(mixMode > MIX_MODE_MAX)
    {
        mixMode = MIX_MODE_MAX;
    }
    //调用实际的开始旋转函数
    return AppImplMixWashStartRotateFuncPtrArray[mixMode]();
}


//清洗混匀集成混匀一次
LH_ERR AppImplMixWashRotateIntegratedVersion1(MIX_MODE mixMode)
{
    //模式修正
    if(mixMode > MIX_MODE_MAX)
    {
        mixMode = MIX_MODE_MAX;
    }
    //调用实际的集成混匀的函数
    return AppImplMixWashRotateIntegratedFuncPtrArray[mixMode]();
}


//步进电机调试配置表
static const MODULE_STEP_MOTOR_RESET_RUN_CONFIG moduleStepMotorResetRunConfigArray[] = {
    {MIX_WASH_STEP_MOTOR_UPDOWN_ID,INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_RESET_CORRECT_UPDOWN},//清洗混匀升降
    {MIX_WASH_STEP_MOTOR_ROTATE_ID,INDEX_CAN_SUB_BOARD1_PARAM_MAIN_MIX_WASH,APP_PARAM_SUB_INDEX_MIX_WASH_RESET_CORRECT_ROTATE},//清洗混匀旋转
};

//指定步进电机复位
LH_ERR AppImplMixWashStepMotorResetVersion1(INDEX_MOTOR_MIX_WASH index)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //复位指令结构体
        StepMotorResetCmd resetCmd;
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
    //旋转电机无原点，只设置电机状态。
    if(index == INDEX_MOTOR_MIX_WASH_ROTATE)
    {
        errorCode = IPC_StepMotorSetStateWhileReturn(moduleStepMotorResetRunConfigArray[index].motorIndex,STEP_MOTOR_MAIN_STATE_STOP,STEP_MOTOR_STOP_SUB_NORMAL);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        return LH_ERR_NONE;
    }
    //读取修正
        AppParamRead((INDEX_CAN_SUB_BOARD1_PARAM_MAIN)(moduleStepMotorResetRunConfigArray[index].resetCorrectMainIndex),
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
LH_ERR AppImplMixWashStepMotorRunStepsVersion1(INDEX_MOTOR_MIX_WASH index,int32_t steps,int32_t* posAfterRun)
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























