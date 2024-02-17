/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 14:08:32
**LastEditors: DengXiaoJun
**LastEditTime: 2020-07-10 11:01:56
**FilePath: \H360_ActionBoard_ID_2\TaskMain\AppSupport\AppTask\AppImpl\AppImplCupTrayManage.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplCupTrayManage.h"

//转换试管架传送带错误代码的标识
typedef enum FLAG_CUP_TRAY_MANAGE_ERR_CONVERT
{
    FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT                 = 0X01,//输入读取错误
    FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT               = 0X02,//输入写入错误
    FLAG_CUP_TRAY_MANAGE_ERR_DOOR_OPEN                  = 0X03,//门开着不能执行该操作
    FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_EMPTY             = 0X04,//新杯栈已空
    FLAG_CUP_TRAY_MANAGE_ERR_EMPTY_TARY_FULL            = 0X05,//空杯栈已满
    FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_UPDOWN_RESET      = 0X06,//新杯栈复位失败
    FLAG_CUP_TRAY_MANAGE_ERR_EMPTY_TRAY_UPDOWN_RESET    = 0X07,//空杯栈复位失败
    FLAG_CUP_TRAY_MANAGE_ERR_PUSH_RESET                 = 0X08,//推手复位失败
    FLAG_CUP_TRAY_MANAGE_ERR_CLAMP_RESET                = 0X09,//夹手复位失败
    FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_UPDOWN_RUN        = 0X0A,//新杯栈运行失败
    FLAG_CUP_TRAY_MANAGE_ERR_EMPTY_TRAY_UPDOWN_RUN      = 0X0B,//空杯栈运行失败
    FLAG_CUP_TRAY_MANAGE_ERR_PUSH_RUN                   = 0X0C,//推杆运行失败
    FLAG_CUP_TRAY_MANAGE_ERR_CLAMP_RUN                  = 0X0D,//夹手运行失败
    FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_UPDOWN_RETURN     = 0X0E,//新杯栈回零失败
    FLAG_CUP_TRAY_MANAGE_ERR_EMPTY_TRAY_UPDOWN_RETURN   = 0X0F,//空杯栈回零失败
    FLAG_CUP_TRAY_MANAGE_ERR_PUSH_RETURN                = 0X10,//推杆回零失败
    FLAG_CUP_TRAY_MANAGE_ERR_CLAMP_RETURN               = 0X11,//夹手回零失败
    FLAG_CUP_TRAY_MANAGE_ERR_GARBAGE1_OVERLOAD          = 0X12,//垃圾桶1过载
    FLAG_CUP_TRAY_MANAGE_ERR_GARBAGE2_OVERLOAD          = 0X13,//垃圾桶2过载
    FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_UPLOAD_FAIL       = 0X14,//上新杯失败
    FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_RECYCLE_FAIL      = 0X15,//回收旧杯失败
    FLAG_CUP_TRAY_MANAGE_ERR_PUSH_INPLACE               = 0X16,//推手推杯盘到测试位失败
}FLAG_CUP_TRAY_MANAGE_ERR_CONVERT;

//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_CUP_TRAY_MANAGE;
    moduleNo <<= 24;
    //错误标志
    uint32_t flagInsert = flag;
    flagInsert <<= 16;
    //基础错误代码
    errResult = LH_ERR_BOARD2_BASE;
    //加入模块号
    errResult |= moduleNo;
    //加入错误标志
    errResult |= flagInsert;
    //加入原始错误代码
    errResult |= errSrc;
    //返回转换完成的错误代码
    return errResult;
}

//门当前的状态
static STACK_MANAGE_DOOR_STATE currentDoorState = STACK_MANAGE_DOOR_STATE_OPEN;
//当前新杯盘数量
static uint8_t currentNewCupTrayCount = 0;
//当前空杯盘数量
static uint8_t currentEmptyTrayCount = 0;
//垃圾桶状态
static STACK_GARBAGE stackGarbageStateSet;

//获取需要返回给主控的讯息
static LH_ERR AppImplCupTrayManageReadSensorStateUtil(STACK_MANAGE_RESULT* resultData)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //填充状态信息
        resultData->newCupTrayCount = currentNewCupTrayCount;
        resultData->emptyTrayCount = currentEmptyTrayCount;
    //读取传感器信息,新杯栈顶部托盘检测有无传感器
        BitAction inputState;
        errorCode = CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_NEW_TARY_CHECK_TOP_EXIST_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT,errorCode);
            return errorCode;
        }
        //转换成为是否有效讯息
        if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_NEW_TARY_CHECK_TOP_EXIST_SENSOR)
        {
            resultData->newTraySensorState = SENSOR_STATE_VALID;
        }
        else
        {
            resultData->newTraySensorState = SENSOR_STATE_INVALID;
        }
    //读取传感器信息,测试位托盘检测是否存在
        errorCode = CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_TEST_TARY_CHECK_EXIST_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT,errorCode);
            return errorCode;
        }
        //转换成为是否有效讯息
        if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_TEST_TARY_CHECK_EXIST_SENSOR)
        {
            resultData->testTraySensorState = SENSOR_STATE_VALID;
        }
        else
        {
            resultData->testTraySensorState = SENSOR_STATE_INVALID;
        }
    //读取传感器信息,垃圾桶1检测是否存在
        errorCode = CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_GARBAGE1_CHECK_EXIST_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT,errorCode);
            return errorCode;
        }
        //转换成为是否有效讯息
        if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_GARBAGE1_CHECK_EXIST_SENSOR)
        {
            resultData->garbage1SensorState = SENSOR_STATE_VALID;
        }
        else
        {
            resultData->garbage1SensorState = SENSOR_STATE_INVALID;
        }
    //读取传感器信息,垃圾桶2检测是否存在
        errorCode = CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_GARBAGE2_CHECK_EXIST_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT,errorCode);
            return errorCode;
        }
        //转换成为是否有效讯息
        if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_GARBAGE2_CHECK_EXIST_SENSOR)
        {
            resultData->garbage2SensorState = SENSOR_STATE_VALID;
        }
        else
        {
            resultData->garbage2SensorState = SENSOR_STATE_INVALID;
        }
    
    return errorCode;
}

//新杯栈维护上一次按键状态
static SENSOR_STATE newTrayStackKeySensorLastState;
//新杯栈维护本次按键状态
static SENSOR_STATE newTrayStackKeySensorCurrentState;

//空杯栈维护上一次按键状态
static SENSOR_STATE emptyTrayStackKeySensorLastState;
//空杯栈维护本次按键状态
static SENSOR_STATE emptyTrayStackKeySensorCurrentState;

//打开按键灯
static LH_ERR AppImplCupTrayManageKeyLightEnable(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //打开按键灯1
        errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_NEW_TARY_UPDOWN_KEY_LIGHT,(BitAction)(VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_NEW_TARY_UPDOWN_KEY_LIGHT));
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
            return errorCode;
        } 
    //打开按键灯2
        errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_EMPTY_TARY_UPDOWN_KEY_LIGHT,(BitAction)(VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_EMPTY_TARY_UPDOWN_KEY_LIGHT));
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
            return errorCode;
        }  
    return errorCode;
}

//关闭按键灯
static LH_ERR AppImplCupTrayManageKeyLightDisable(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //打开按键灯1
        errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_NEW_TARY_UPDOWN_KEY_LIGHT,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_NEW_TARY_UPDOWN_KEY_LIGHT));
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
            return errorCode;
        } 
    //打开按键灯2
        errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_EMPTY_TARY_UPDOWN_KEY_LIGHT,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_EMPTY_TARY_UPDOWN_KEY_LIGHT));
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
            return errorCode;
        }  
    return errorCode;
}

//读取开门到位
static LH_ERR AppImplCupTrayManageReadDoorOpenFull(SENSOR_STATE* doorFullOpenState)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction inputState;
    //读取开门到位
        errorCode = CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_DOOR_OPEN_FULL_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT,errorCode);
            return errorCode;
        }
    //转换成为是否有效讯息
        if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_DOOR_OPEN_FULL_SENSOR)
        {
            *doorFullOpenState = SENSOR_STATE_VALID;
        }
        else
        {
            *doorFullOpenState = SENSOR_STATE_INVALID;
        }
    return errorCode;
}

//杯栈管理状态机初始化
LH_ERR AppImplCupTrayManageStateMachineInit(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化垃圾桶状态
        stackGarbageStateSet.garbage1State = STACK_GARBAGE_IDLE;
        stackGarbageStateSet.garbage2State = STACK_GARBAGE_IDLE;
        stackGarbageStateSet.garbage1RunningTimeCount = 0;
        stackGarbageStateSet.garbage2RunningTimeCount = 0;
    //垃圾桶1停止运行
        //垃圾桶1上升无效
        CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP));
        //垃圾桶1下降无效
        CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN));
    //垃圾桶2停止运行
        //垃圾桶2上升无效
        CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP));
        //垃圾桶2下降无效
        CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN));
    //延时初始化,等待各个信号稳定
        CoreDelayMs(500);
    //关闭按键灯
        errorCode = AppImplCupTrayManageKeyLightDisable();
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //读取输入
        BitAction inputState;
    //按键状态获取,新杯栈
        errorCode = CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_NEW_TARY_UPDOWN_KEY,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //转换成为是否有效讯息
        if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_NEW_TARY_UPDOWN_KEY)
        {
            newTrayStackKeySensorCurrentState = SENSOR_STATE_VALID;
        }
        else
        {
            newTrayStackKeySensorCurrentState = SENSOR_STATE_INVALID;
        }
    //按键状态获取,空杯栈
        errorCode = CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_EMPTY_TARY_UPDOWN_KEY,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //转换成为是否有效讯息
        if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_EMPTY_TARY_UPDOWN_KEY)
        {
            emptyTrayStackKeySensorCurrentState = SENSOR_STATE_VALID;
        }
        else
        {
            emptyTrayStackKeySensorCurrentState = SENSOR_STATE_INVALID;
        }
    //记录状态
        newTrayStackKeySensorLastState = newTrayStackKeySensorCurrentState;
        emptyTrayStackKeySensorLastState = emptyTrayStackKeySensorCurrentState;
    //设置垃圾桶1开始下降
        //垃圾桶1上升无效
        CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP));
        //垃圾桶1下降有效
        CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN,(BitAction)(VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN));
    //设置垃圾桶2开始下降
        //垃圾桶2上升无效
        CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP));
        //垃圾桶2下降有效
        CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN,(BitAction)(VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN));
    //设置垃圾桶1的状态,正在下降
        stackGarbageStateSet.garbage1State = STACK_GARBAGE_DOWN;
        stackGarbageStateSet.garbage1RunningTimeCount = 0;
    //设置垃圾桶2的状态,正在下降
        stackGarbageStateSet.garbage2State = STACK_GARBAGE_DOWN;
        stackGarbageStateSet.garbage2RunningTimeCount = 0;
    //返回完成
        return errorCode;
}
//杯栈管理按键更新
static LH_ERR AppImplCupTrayManageKeyStateReflush(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    SENSOR_STATE keySensorState;
    BitAction inputState;
    //按键状态获取,新杯栈
        errorCode = CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_NEW_TARY_UPDOWN_KEY,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //转换成为是否有效讯息
        if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_NEW_TARY_UPDOWN_KEY)
        {
            keySensorState = SENSOR_STATE_VALID;
        }
        else
        {
            keySensorState = SENSOR_STATE_INVALID;
        }
    //更新按键讯息
        if(keySensorState != newTrayStackKeySensorCurrentState)
        {
            newTrayStackKeySensorLastState = newTrayStackKeySensorCurrentState;
            newTrayStackKeySensorCurrentState = keySensorState;
        }
    //按键状态获取,空杯栈
        errorCode = CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_EMPTY_TARY_UPDOWN_KEY,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //转换成为是否有效讯息
        if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_EMPTY_TARY_UPDOWN_KEY)
        {
            keySensorState = SENSOR_STATE_VALID;
        }
        else
        {
            keySensorState = SENSOR_STATE_INVALID;
        }
    //更新按键讯息
        if(keySensorState != emptyTrayStackKeySensorCurrentState)
        {
            emptyTrayStackKeySensorLastState = emptyTrayStackKeySensorCurrentState;
            emptyTrayStackKeySensorCurrentState = keySensorState;
        }
    return LH_ERR_NONE;
}
//杯栈管理新杯栈按键处理
static LH_ERR AppImplCupTrayManageKeyEventNewStackProcess()
{
    LH_ERR errorCode = LH_ERR_NONE;
    //电机复位,电机走坐标,电机走位,电机回零结构体
        int32_t paramUtil;
        int32_t motorPos;
        StepMotorRun2CoordationCmd runToPosCmd;
        StepMotorReturnZeroCmd returnZeroCmd;
    //上一次按键没按下,本次按键按下,才会处理按键事件
    if((newTrayStackKeySensorLastState == SENSOR_STATE_INVALID)&&(newTrayStackKeySensorCurrentState == SENSOR_STATE_VALID))
    {
        //读取新杯盘修正
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_NEW_CUP_TRAY_STACK_RESET_CORRECT,&paramUtil);
        //读取电机坐标
        IPC_StepMotorReflushStateWhileReturn(CUP_TRAY_MANAGE_STEP_MOTOR_NEW_TRAY_UPDOWN_ID,&motorPos);
        //根据坐标决定上升下降
        if(motorPos == paramUtil)
        {
            //抬起来,读取一层的高度
            AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_SPACING_LAYER_NEW_CUP_TRAY,&paramUtil);
            paramUtil *= TRAY_COUNT_NEW_STACK_KEY;
            //空杯栈走到指定位置
            IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
            runToPosCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_NEW_TRAY_UPDOWN_ID;//电机序号
            runToPosCmd.targetCoordinate = paramUtil;//目标坐标
            runToPosCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_UP;//速度曲线
            runToPosCmd.timeOutSet = 60000;
            errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_UPDOWN_RUN,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        }
        else
        {
            //降下去
            IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
            returnZeroCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_NEW_TRAY_UPDOWN_ID;
            returnZeroCmd.correctionCoordinate = paramUtil;
            returnZeroCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_DOWN;
            returnZeroCmd.timeOutSet = 60000;
            //电机回零,等待完成
            errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_UPDOWN_RETURN,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        }
    }
    return LH_ERR_NONE;
}
//杯栈管理空杯栈按键处理
static LH_ERR AppImplCupTrayManageKeyEventEmptyStackProcess()
{
    LH_ERR errorCode = LH_ERR_NONE;
    //电机复位,电机走坐标,电机走位,电机回零结构体
        int32_t paramUtil;
        int32_t motorPos;
        StepMotorRun2CoordationCmd runToPosCmd;
        StepMotorReturnZeroCmd returnZeroCmd;
    //上一次按键没按下,本次按键按下,才会处理按键事件
    if((emptyTrayStackKeySensorLastState == SENSOR_STATE_INVALID)&&(emptyTrayStackKeySensorCurrentState == SENSOR_STATE_VALID))
    {
        //读取空杯盘修正
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_EMPTY_CUP_TRAY_STACK_RESET_CORRECT,&paramUtil);
        //读取电机坐标
        IPC_StepMotorReflushStateWhileReturn(CUP_TRAY_MANAGE_STEP_MOTOR_EMPTY_TRAY_UPDOWN_ID,&motorPos);
        //根据坐标决定上升下降
        if(motorPos == paramUtil)
        {
            //抬起来,读取一层的高度
            AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_SPACING_LAYER_EMPTY_CUP_TRAY,&paramUtil);
            paramUtil *= TRAY_COUNT_EMPTY_STACK_KEY;
            //空杯栈走到指定位置
            IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
            runToPosCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_EMPTY_TRAY_UPDOWN_ID;//电机序号
            runToPosCmd.targetCoordinate = paramUtil;//目标坐标
            runToPosCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_UP;//速度曲线
            runToPosCmd.timeOutSet = 60000;
            errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_EMPTY_TRAY_UPDOWN_RUN,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        }
        else
        {
            //降下去
            IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
            returnZeroCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_EMPTY_TRAY_UPDOWN_ID;
            returnZeroCmd.correctionCoordinate = paramUtil;
            returnZeroCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_DOWN;
            returnZeroCmd.timeOutSet = 60000;
            //电机回零,等待完成
            errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_EMPTY_TRAY_UPDOWN_RETURN,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        }
    }
    return LH_ERR_NONE;
}


//垃圾桶1状态机循环
static void AppImplCupTrayManageStateMachineGarbage1Proc()
{
    //垃圾桶过载IO状态
    BitAction inputState;
    //垃圾桶1状态处理
    if((STACK_GARBAGE_UP == stackGarbageStateSet.garbage1State)||(STACK_GARBAGE_DOWN == stackGarbageStateSet.garbage1State))
    {
        //运行时间增加
        stackGarbageStateSet.garbage1RunningTimeCount++;
        //检测过载
        CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_GARBAGE1_OVERLOAD_SENSOR,&inputState);
        //检测过载状态
        if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_GARBAGE1_OVERLOAD_SENSOR)
        {
            //垃圾桶1过载
            stackGarbageStateSet.garbage1State = STACK_GARBAGE_OVERLOAD;
            stackGarbageStateSet.garbage1RunningTimeCount = 0;
            //停止运行,垃圾桶1上升无效
            CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP));
            //垃圾桶1下降无效
            CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN));
        }
        else
        {
            //没有过载
            if(stackGarbageStateSet.garbage1RunningTimeCount >= TIME_COUNT_CUP_TRAY_MANAGE_GARBAGE_UPDOWN)
            {
                //运行时间到位,垃圾桶1空闲
                stackGarbageStateSet.garbage1State = STACK_GARBAGE_IDLE;
                stackGarbageStateSet.garbage1RunningTimeCount = 0;
                //停止运行,垃圾桶1上升无效
                CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP));
                //垃圾桶1下降无效
                CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN));
            }
        }
    }
}

//垃圾桶2状态机循环
static void AppImplCupTrayManageStateMachineGarbage2Proc()
{
    //垃圾桶过载IO状态
    BitAction inputState;
    //垃圾桶2状态处理
    if((STACK_GARBAGE_UP == stackGarbageStateSet.garbage2State)||(STACK_GARBAGE_DOWN == stackGarbageStateSet.garbage2State))
    {
        //运行时间增加
        stackGarbageStateSet.garbage2RunningTimeCount++;
        //检测过载
        CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_GARBAGE2_OVERLOAD_SENSOR,&inputState);
        //检测过载状态
        if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_GARBAGE2_OVERLOAD_SENSOR)
        {
            //垃圾桶2过载
            stackGarbageStateSet.garbage2State = STACK_GARBAGE_OVERLOAD;
            stackGarbageStateSet.garbage2RunningTimeCount = 0;
            //停止运行,垃圾桶2上升无效
            CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP));
            //垃圾桶2下降无效
            CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN));
        }
        else
        {
            //没有过载
            if(stackGarbageStateSet.garbage2RunningTimeCount >= TIME_COUNT_CUP_TRAY_MANAGE_GARBAGE_UPDOWN)
            {
                //运行时间到位,垃圾桶2空闲
                stackGarbageStateSet.garbage2State = STACK_GARBAGE_IDLE;
                stackGarbageStateSet.garbage2RunningTimeCount = 0;
                //停止运行,垃圾桶2上升无效
                CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP));
                //垃圾桶2下降无效
                CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN));
            }
        }
    }
}

//杯栈管理状态机循环
LH_ERR AppImplCupTrayManageStateMachineLoop(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    SENSOR_STATE doorOpenFullState;
    //垃圾桶1状态处理
    AppImplCupTrayManageStateMachineGarbage1Proc();
    //垃圾桶2状态处理
    AppImplCupTrayManageStateMachineGarbage2Proc();
    //维护按键处理
    if(currentDoorState == STACK_MANAGE_DOOR_STATE_OPEN)
    {
        //读取开门到位
            errorCode = AppImplCupTrayManageReadDoorOpenFull(&doorOpenFullState);
            if(errorCode != LH_ERR_NONE)
            {
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        //开门没到位,就不需要循环了
            if(doorOpenFullState != SENSOR_STATE_VALID)
            {
                //关闭按键灯
                errorCode = AppImplCupTrayManageKeyLightDisable();
                if(errorCode != LH_ERR_NONE)
                {
                    AppShowErrMsg(errorCode);
                    return errorCode;
                }
                //门没开到位,直接完成
                return LH_ERR_NONE;
            }
        //运行到这里,门开到位了,打开按键灯
            errorCode = AppImplCupTrayManageKeyLightEnable();
            if(errorCode != LH_ERR_NONE)
            {
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        //执行按键检测
            errorCode = AppImplCupTrayManageKeyStateReflush();
            if(errorCode != LH_ERR_NONE)
            {
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        //新杯栈按键处理
            errorCode = AppImplCupTrayManageKeyEventNewStackProcess();
            if(errorCode != LH_ERR_NONE)
            {
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        //空杯栈按键处理
            errorCode = AppImplCupTrayManageKeyEventEmptyStackProcess();
            if(errorCode != LH_ERR_NONE)
            {
                AppShowErrMsg(errorCode);
                return errorCode;
            }
    }
    else
    {
        //关闭按键灯
            errorCode = AppImplCupTrayManageKeyLightDisable();
            if(errorCode != LH_ERR_NONE)
            {
                AppShowErrMsg(errorCode);
                return errorCode;
            }
    }
    return errorCode;
}

//设置杯栈门状态
LH_ERR AppImplCupTrayManageSetDoorState(STACK_MANAGE_DOOR_STATE doorState)
{
    currentDoorState = doorState;
    return LH_ERR_NONE;
}

//新杯盘栈初始化
LH_ERR AppImplCupTrayManageNewStackInit(STACK_MANAGE_RESULT* resultData)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //电机复位,电机走坐标,电机走位,电机回零结构体
        int32_t paramUtil;
        StepMotorRun2CoordationCmd runToPosCmd;
        StepMotorResetCmd resetCmd;
        StepMotorReturnZeroCmd returnZeroCmd;
        StepMotorRunStepsCmd runStepCmd;
    //读取新杯栈升降复位修正参数
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_NEW_CUP_TRAY_STACK_RESET_CORRECT,&paramUtil);
    //新杯栈升降复位
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
        resetCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_NEW_TRAY_UPDOWN_ID;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//零位修正
        resetCmd.timeOutSet = 60000;//超时时间
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_UPDOWN_RESET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //读取顶部检测位传感器讯息
        BitAction inputState;
        errorCode = CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_NEW_TARY_CHECK_TOP_EXIST_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT,errorCode);
            return errorCode;
        }
        //转换成为是否有效讯息
        if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_NEW_TARY_CHECK_TOP_EXIST_SENSOR)
        {
            //顶部传感器有效,返回,因为传感器被遮挡,无法有效初始化
            currentNewCupTrayCount = 0;
            //更新要返回的讯息
            errorCode = AppImplCupTrayManageReadSensorStateUtil(resultData);
            if(errorCode != LH_ERR_NONE)
            {
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            //返回
            return errorCode;
        }
    //此时顶部传感器无效,可以正常初始化,首先走到顶部到检测位的偏移,读取偏移坐标
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_OFFSET_TOP_TO_SENSOR_WHEN_NEW_CUP_TRAY_FULL,&paramUtil);
    //电机走坐标参数设置
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_NEW_TRAY_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_CHECK_COUNT;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_UPDOWN_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前杯栈数量为满
        currentNewCupTrayCount = NEW_TRAY_STACK_MAX_COUNT;
    //开始循环检测杯栈光电
        do
        {
            //读取检测位光电
            errorCode = CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_NEW_TARY_CHECK_TOP_EXIST_SENSOR,&inputState);
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT,errorCode);
                return errorCode;
            }
            if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_NEW_TARY_CHECK_TOP_EXIST_SENSOR)
            {
                //光电触发,推出循环
                break;
            }
            //光电未触发,杯栈数量减少
            currentNewCupTrayCount -= 1;
            if(currentNewCupTrayCount == 0)
            {
                //杯栈数量为0,推出
                break;
            }
            //升降向上走一层
            AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_SPACING_LAYER_NEW_CUP_TRAY,&paramUtil);
            //填充参数
            IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
            runStepCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_NEW_TRAY_UPDOWN_ID;   
            runStepCmd.specialSteps = paramUtil;
            runStepCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_CHECK_COUNT;
            //开始走步数
            errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_UPDOWN_RUN,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        }while(1);
    //新杯栈回零
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_NEW_CUP_TRAY_STACK_RESET_CORRECT,&paramUtil);
    //电机回零参数设置
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_NEW_TRAY_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_DOWN;
        returnZeroCmd.timeOutSet = 60000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_UPDOWN_RETURN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //获取要返回给主控的讯息
        errorCode = AppImplCupTrayManageReadSensorStateUtil(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return errorCode;
}


//空杯盘栈初始化
LH_ERR AppImplCupTrayManageEmptyStackInit(STACK_MANAGE_RESULT* resultData)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //电机复位,电机走坐标,电机走位,电机回零结构体
        int32_t paramUtil;
        StepMotorRun2CoordationCmd runToPosCmd;
        StepMotorResetCmd resetCmd;
        StepMotorReturnZeroCmd returnZeroCmd;
        StepMotorRunStepsCmd runStepCmd;
    //读取空杯栈升降复位修正参数
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_EMPTY_CUP_TRAY_STACK_RESET_CORRECT,&paramUtil);
    //空杯栈升降复位
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
        resetCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_EMPTY_TRAY_UPDOWN_ID;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//零位修正
        resetCmd.timeOutSet = 60000;//超时时间
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_EMPTY_TRAY_UPDOWN_RESET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //首先走到顶部到检测位的偏移,读取偏移坐标
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_OFFSET_TOP_TO_SENSOR_WHEN_EMPTY_CUP_TRAY_FULL,&paramUtil);
    //电机走坐标参数设置
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_EMPTY_TRAY_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_CHECK_COUNT;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_EMPTY_TRAY_UPDOWN_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前杯栈数量为满
        currentEmptyTrayCount = EMPTY_TRAY_STACK_MAX_COUNT;
        BitAction inputState;
    //开始循环检测杯栈光电
        do
        {
            //读取检测位光电
            errorCode = CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_EMPTY_TARY_CHECK_EXIST_SENSOR,&inputState);
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT,errorCode);
                return errorCode;
            }
            if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_EMPTY_TARY_CHECK_EXIST_SENSOR)
            {
                //光电触发,推出循环
                break;
            }
            //光电未触发,杯栈数量减少
            currentEmptyTrayCount -= 1;
            if(currentEmptyTrayCount == 0)
            {
                //杯栈数量为0,推出
                break;
            }
            //升降向上走一层
            AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_SPACING_LAYER_EMPTY_CUP_TRAY,&paramUtil);
            //填充参数
            IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
            runStepCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_EMPTY_TRAY_UPDOWN_ID;   
            runStepCmd.specialSteps = paramUtil;
            runStepCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_CHECK_COUNT;
            //开始走步数
            errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_EMPTY_TRAY_UPDOWN_RUN,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        }while(1);
    //空杯栈回零
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_EMPTY_CUP_TRAY_STACK_RESET_CORRECT,&paramUtil);
    //电机回零参数设置
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_EMPTY_TRAY_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_DOWN;
        returnZeroCmd.timeOutSet = 60000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_EMPTY_TRAY_UPDOWN_RETURN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //获取要返回给主控的讯息
        errorCode = AppImplCupTrayManageReadSensorStateUtil(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return errorCode;
}


//推手初始化
LH_ERR AppImplCupTrayManagePushInit(STACK_MANAGE_RESULT* resultData)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //步进电机复位
        StepMotorResetCmd resetCmd;
        int32_t paramUtil;
    //读取复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_PUSH_RESET_CORRECT,&paramUtil);
    //推手复位
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
        resetCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_PUSH_ID;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//零位修正
        resetCmd.timeOutSet = 60000;//超时时间
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_PUSH_RESET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //获取要返回给主控的讯息
        errorCode = AppImplCupTrayManageReadSensorStateUtil(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return errorCode;
}


//夹手初始化
LH_ERR AppImplCupTrayManageClampInit(STACK_MANAGE_RESULT* resultData)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //步进电机复位
        StepMotorResetCmd resetCmd;
        int32_t paramUtil;
    //读取复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_CLAMP_RESET_CORRECT,&paramUtil);
    //夹手复位
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
        resetCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_CLAMP_ID;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//零位修正
        resetCmd.timeOutSet = 60000;//超时时间
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_CLAMP_RESET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //获取要返回给主控的讯息
        errorCode = AppImplCupTrayManageReadSensorStateUtil(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return errorCode;
}


//垃圾桶下降
LH_ERR AppImplCupTrayManageGarbageDownUnlock(GARBAGE_SELECT garbageNo,STACK_MANAGE_RESULT* resultData)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //记录一下上一次垃圾桶1和垃圾桶2的状态
        STACK_GARBAGE_STATE lastGarbage1State = stackGarbageStateSet.garbage1State;
        STACK_GARBAGE_STATE lastGarbage2State = stackGarbageStateSet.garbage2State;
    //垃圾桶1切换到下降模式
        if((garbageNo&GARBAGE_SELECT_1) != 0)
        {
            //停止运行,垃圾桶1上升无效
            errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP));
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            //垃圾桶1下降无效
            errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN));
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            //垃圾桶1上升无效
            errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP));
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            //垃圾桶1下降有效
            errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN,(BitAction)(VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN));
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            //设置状态为垃圾桶下降
            stackGarbageStateSet.garbage1State = STACK_GARBAGE_DOWN;
            stackGarbageStateSet.garbage1RunningTimeCount = 0;
        }
    //垃圾桶2切换到下降模式
        if((garbageNo&GARBAGE_SELECT_2) != 0)
        {
            //停止运行,垃圾桶2上升无效
            errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP));
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            //垃圾桶2下降无效
            errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN));
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            //垃圾桶2上升无效
            errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP));
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            //垃圾桶2下降有效
            errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN,(BitAction)(VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN));
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            //设置状态为垃圾桶下降
            stackGarbageStateSet.garbage2State = STACK_GARBAGE_DOWN;
            stackGarbageStateSet.garbage2RunningTimeCount = 0;
        }
    //根据过载信号决定是否返回过载错误讯息
        if(((garbageNo&GARBAGE_SELECT_1) != 0)&&(lastGarbage1State == STACK_GARBAGE_OVERLOAD))
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_GARBAGE1_OVERLOAD,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        if(((garbageNo&GARBAGE_SELECT_2) != 0)&&(lastGarbage2State == STACK_GARBAGE_OVERLOAD))
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_GARBAGE2_OVERLOAD,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //获取要返回给主控的讯息
        errorCode = AppImplCupTrayManageReadSensorStateUtil(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return errorCode;
}


//垃圾桶上升
LH_ERR AppImplCupTrayManageGarbegeUpLock(GARBAGE_SELECT garbageNo,STACK_MANAGE_RESULT* resultData)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //记录一下上一次垃圾桶1和垃圾桶2的状态
        STACK_GARBAGE_STATE lastGarbage1State = stackGarbageStateSet.garbage1State;
        STACK_GARBAGE_STATE lastGarbage2State = stackGarbageStateSet.garbage2State;
    //垃圾桶1切入上升状态
        if((garbageNo&GARBAGE_SELECT_1) != 0)
        {
            //停止运行,垃圾桶1上升无效
            errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP));
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            //垃圾桶1下降无效
            errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN));
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            //垃圾桶1上升有效
            errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP,(BitAction)(VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_UP));
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            //垃圾桶1下降无效
            errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE1_DOWN));
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        }
    //垃圾桶2切入上升状态
        if((garbageNo&GARBAGE_SELECT_2) != 0)
        {
            //停止运行,垃圾桶2上升无效
            errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP));
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            //垃圾桶2下降无效
            errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN));
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            //垃圾桶2上升有效
            errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP,(BitAction)(VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_UP));
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            //垃圾桶2下降无效
            errorCode = CanSubOutPutWriteSingle(OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN,(BitAction)(!VALID_LEVEL_OUTPUT_CUP_TRAY_MANAGE_GARBAGE2_DOWN));
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_WRITE_OUTPUT,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        }
    //根据过载信号决定是否返回过载错误讯息
        if(((garbageNo&GARBAGE_SELECT_1) != 0)&&(lastGarbage1State == STACK_GARBAGE_OVERLOAD))
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_GARBAGE1_OVERLOAD,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        if(((garbageNo&GARBAGE_SELECT_2) != 0)&&(lastGarbage2State == STACK_GARBAGE_OVERLOAD))
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_GARBAGE2_OVERLOAD,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //获取要返回给主控的讯息
        errorCode = AppImplCupTrayManageReadSensorStateUtil(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return errorCode;
}


//杯栈模块整体初始化
LH_ERR AppImplCupTrayManageInitAll(STACK_MANAGE_RESULT* resultData)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //推手初始化
        errorCode = AppImplCupTrayManagePushInit(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //夹手初始化
        errorCode = AppImplCupTrayManageClampInit(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //新杯栈初始化
        errorCode = AppImplCupTrayManageNewStackInit(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //空杯栈初始化
        errorCode = AppImplCupTrayManageEmptyStackInit(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //夹手夹紧
        errorCode = AppImplCupTrayManageClampClose(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //获取要返回给主控的讯息
        errorCode = AppImplCupTrayManageReadSensorStateUtil(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return errorCode;
}


//夹手释放
LH_ERR AppImplCupTrayManageClampOpen(STACK_MANAGE_RESULT* resultData)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //电机走坐标结构体
        int32_t paramUtil;
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取夹手打开位置
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_POS_CLAMP_OPEN,&paramUtil);
    //夹手打开,走到打开位置
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_CLAMP_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_CLAMP_OPEN;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_CLAMP_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //获取要返回给主控的讯息
        errorCode = AppImplCupTrayManageReadSensorStateUtil(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return errorCode;
}


//夹手夹紧
LH_ERR AppImplCupTrayManageClampClose(STACK_MANAGE_RESULT* resultData)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //电机走坐标结构体
        int32_t paramUtil;
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取夹手关闭位置
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_POS_CLAMP_CLOSE,&paramUtil);
    //夹手关闭,走到关闭位置
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_CLAMP_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_CLAMP_CLOSE;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_CLAMP_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //获取要返回给主控的讯息
        errorCode = AppImplCupTrayManageReadSensorStateUtil(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return errorCode;
}


//新杯盘栈上传一个新盘
LH_ERR AppImplCupTrayManageUploadNewTray(STACK_MANAGE_RESULT* resultData)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction inputState;
    //电机走坐标,电机走位,电机回零结构体
        int32_t paramUtilOffset;
        int32_t paramUtil;
        StepMotorRun2CoordationCmd runToPosCmd;
        StepMotorReturnZeroCmd returnZeroCmd;
        StepMotorRunStepsCmd runStepCmd;
    //首先检测顶部传感器状态,如果有效,直接返回
        errorCode = CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_NEW_TARY_CHECK_TOP_EXIST_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //有效直接返回
        if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_NEW_TARY_CHECK_TOP_EXIST_SENSOR)
        {
            errorCode = AppImplCupTrayManageReadSensorStateUtil(resultData);
            if(errorCode != LH_ERR_NONE)
            {
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            return errorCode;
        }
    //检测新盘数量,新盘数量为0,返回报错
        if(currentNewCupTrayCount == 0)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_EMPTY,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //首先回零
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_NEW_CUP_TRAY_STACK_RESET_CORRECT,&paramUtil);
    //电机回零参数设置
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_NEW_TRAY_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_DOWN;
        returnZeroCmd.timeOutSet = 60000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_UPDOWN_RETURN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //读取到平台偏移
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_OFFSET_TOP_TO_PALTFORM_WHEN_NEW_CUP_TRAY_FULL,&paramUtilOffset);
    //读取一格的偏移
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_SPACING_LAYER_NEW_CUP_TRAY,&paramUtil);
    //计算目标坐标,是到平台的偏移加上检测架子层数补偿
        paramUtil = paramUtilOffset + paramUtil*(NEW_TRAY_STACK_MAX_COUNT - currentNewCupTrayCount);
    //走到目标坐标
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_NEW_TRAY_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_UP;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_UPDOWN_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //延时等待盘子稳定
        CoreDelayMsSensitivity(TIMS_MS_WAIT_NEW_TRAY_STACK_STABLE);
    //反向运行一定步数,脱离
        IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
        runStepCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_NEW_TRAY_UPDOWN_ID;   
        runStepCmd.specialSteps = (int32_t)((0-paramUtilOffset)/2);
        runStepCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_ADJUST;
        //开始走步数
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_UPDOWN_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //新杯栈回零
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_NEW_CUP_TRAY_STACK_RESET_CORRECT,&paramUtil);
    //电机回零参数设置
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_NEW_TRAY_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_DOWN;
        returnZeroCmd.timeOutSet = 60000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_UPDOWN_RETURN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //查看顶部传感器状态决定是否将新盘减一,如果有效,说明被子上去了,状态-1
        errorCode = CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_NEW_TARY_CHECK_TOP_EXIST_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT,errorCode);
            return errorCode;
        }
        //转换成为是否有效讯息
        if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_NEW_TARY_CHECK_TOP_EXIST_SENSOR)
        {
            currentNewCupTrayCount -= 1;
        }
        else
        {
            //上传失败
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_UPLOAD_FAIL,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //获取要返回给主控的讯息
        errorCode = AppImplCupTrayManageReadSensorStateUtil(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return errorCode;
}


//空杯盘栈从测试区回收一个空盘
LH_ERR AppImplCupTrayManageRecycleEmptyTray(STACK_MANAGE_RESULT* resultData)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction inputState;
    //电机走坐标,电机走位,电机回零结构体
        int32_t paramUtilOffset;
        int32_t paramUtil;
        StepMotorRun2CoordationCmd runToPosCmd;
        StepMotorReturnZeroCmd returnZeroCmd;
    //回收前测试位在位信号和回收后测试位在位信号,回收前有效回收后无效(回收成功计数),回收前有效,回收后有效(回收失败)
    //回收前无效回收后无效(成功但是不计数)
        SENSOR_STATE testInPlaceSensorStateBeforeRecycle;
        SENSOR_STATE testInPlaceSensorStateAfterRecycle;
    //如果空杯栈满了,就不能继续回收
        if(currentEmptyTrayCount == EMPTY_TRAY_STACK_MAX_COUNT)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_EMPTY_TARY_FULL,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //空杯栈回零
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_EMPTY_CUP_TRAY_STACK_RESET_CORRECT,&paramUtil);
    //电机回零参数设置
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_EMPTY_TRAY_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_DOWN;
        returnZeroCmd.timeOutSet = 60000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_EMPTY_TRAY_UPDOWN_RETURN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //读取测试位光电信号
        errorCode = CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_TEST_TARY_CHECK_EXIST_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT,errorCode);
            return errorCode;
        }
        //转换成为是否有效讯息
        if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_TEST_TARY_CHECK_EXIST_SENSOR)
        {
            testInPlaceSensorStateBeforeRecycle = SENSOR_STATE_VALID;
        }
        else
        {
            testInPlaceSensorStateBeforeRecycle = SENSOR_STATE_INVALID;
        }
    //获取空杯栈到承接平台的偏移以及一层的高度
    //读取到平台偏移
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_OFFSET_TOP_TO_PALTFORM_WHEN_EMPTY_CUP_TRAY_FULL,&paramUtilOffset);
    //读取一格的偏移
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_SPACING_LAYER_EMPTY_CUP_TRAY,&paramUtil);
    //计算目标坐标,是到平台的偏移加上检测架子层数补偿
        paramUtil = paramUtilOffset + paramUtil*(EMPTY_TRAY_STACK_MAX_COUNT - currentEmptyTrayCount);
    //升降运行到承接位置
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_EMPTY_TRAY_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_UP;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_EMPTY_TRAY_UPDOWN_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //夹手打开
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_POS_CLAMP_OPEN,&paramUtil);
    //夹手打开,走到打开位置
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_CLAMP_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_CLAMP_OPEN;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_CLAMP_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //延时等待盘子掉下去
        CoreDelayMsSensitivity(TIMS_MS_WAIT_EMPTY_TRAY_STACK_STABLE);
    //空杯栈回零
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_EMPTY_CUP_TRAY_STACK_RESET_CORRECT,&paramUtil);
    //电机回零参数设置
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_EMPTY_TRAY_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_DOWN;
        returnZeroCmd.timeOutSet = 60000;
    //电机回零,不等待完成
        errorCode = IPC_StepMotorReturnZeroWhileAck(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_EMPTY_TRAY_UPDOWN_RETURN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //延时等待空杯盘电机下降
        CoreDelayMsSensitivity(TIMS_MS_WAIT_EMPTY_TRAY_STACK_DOWN);
        //读取夹手复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_CLAMP_RESET_CORRECT,&paramUtil);
        //夹手回零
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_CLAMP_ID;//电机序号
        returnZeroCmd.correctionCoordinate = paramUtil;//零位修正
        returnZeroCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_CLAMP_RETURN_ZERO;
        returnZeroCmd.timeOutSet = 60000;//超时时间
        errorCode = IPC_StepMotorReturnZeroWhileAck(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_CLAMP_RESET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        /* 等待夹手复位完成 */
        while(IPC_StepMotorGetLastCommandStatusAndResult(CUP_TRAY_MANAGE_STEP_MOTOR_CLAMP_ID,&errorCode) == STATE_IPC_WAIT_RETURN)
        {
            CoreDelayMinTick();
        }
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_CLAMP_RESET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        /* 等待空杯栈回零完成 */
        while(IPC_StepMotorGetLastCommandStatusAndResult(CUP_TRAY_MANAGE_STEP_MOTOR_EMPTY_TRAY_UPDOWN_ID,&errorCode) == STATE_IPC_WAIT_RETURN)
        {
            CoreDelayMinTick();
        }
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_EMPTY_TRAY_UPDOWN_RETURN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }

    //读取此时检测位置光电讯号
        //读取测试位光电信号
        errorCode = CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_TEST_TARY_CHECK_EXIST_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT,errorCode);
            return errorCode;
        }
        //转换成为是否有效讯息
        if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_TEST_TARY_CHECK_EXIST_SENSOR)
        {
            testInPlaceSensorStateAfterRecycle = SENSOR_STATE_VALID;
        }
        else
        {
            testInPlaceSensorStateAfterRecycle = SENSOR_STATE_INVALID;
        }
    //根据两次的信号判定继续下去
        if(testInPlaceSensorStateBeforeRecycle == SENSOR_STATE_VALID)
        {
            //承接之前有架子,承接之后没架子,接成功了
            if(testInPlaceSensorStateAfterRecycle == SENSOR_STATE_INVALID)
            {
                currentEmptyTrayCount += 1;
            }
            else
            {
                //承接失败,报错
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_RECYCLE_FAIL,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
        }
        else
        {
            //承接之前就没什么东西,所以正常完成不用管
        }
    //获取要返回给主控的讯息
        errorCode = AppImplCupTrayManageReadSensorStateUtil(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return errorCode;
}


//推手推一个新盘到测试位
LH_ERR AppImplCupTrayManagePushNewTrayToTest(STACK_MANAGE_RESULT* resultData)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //电机复位,电机走坐标,电机走位,电机回零结构体
        int32_t paramUtil;
        StepMotorRun2CoordationCmd runToPosCmd;
        StepMotorResetCmd resetCmd;
        StepMotorReturnZeroCmd returnZeroCmd;
        StepMotorRunStepsCmd runStepCmd;
    //首先夹手复位
    //读取复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_CLAMP_RESET_CORRECT,&paramUtil);
    //夹手复位
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
        resetCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_CLAMP_ID;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//零位修正
        resetCmd.timeOutSet = 60000;//超时时间
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_CLAMP_RESET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //推杆回零
    //读取复位修正
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_PUSH_RESET_CORRECT,&paramUtil);
    //电机回零参数设置
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_PUSH_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_PUSH_RETURN;
        returnZeroCmd.timeOutSet = 60000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_PUSH_RETURN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //推杆推架子相关的传感器状态,当推完架子之后,如果测试区有杯盘就不用管了
    //如果测试区没有杯盘,需要查看推之前新杯栈顶点是否有杯盘,新盘有的话,说明没推到位要报错,新盘没的话,就无所谓了
        SENSOR_STATE testInPlaceSensorStateAfter;
        SENSOR_STATE newTrayTopSensorStateBefore;
        BitAction inputState;
    //读取新杯盘顶点传感器讯息
        errorCode = CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_NEW_TARY_CHECK_TOP_EXIST_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_NEW_TARY_CHECK_TOP_EXIST_SENSOR)
        {
            newTrayTopSensorStateBefore = SENSOR_STATE_VALID;
        }
        else
        {
            newTrayTopSensorStateBefore = SENSOR_STATE_INVALID;
        }
    //推手前推
    //读取前推坐标
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_POS_PUSH_TRAY_TO_TEST,&paramUtil);
    //电机走坐标参数设置
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_PUSH_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_PUSH_FAST;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_PUSH_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //读取测试到位传感器
    //读取传感器信息,测试位托盘检测是否存在
        errorCode = CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_TEST_TARY_CHECK_EXIST_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //如果有盘子,二次推进
        if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_TEST_TARY_CHECK_EXIST_SENSOR)
        {
            //读取补充推进步数
            // AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_OFFSET_PUSH_TRAY_TIGHT_SECOND,&paramUtil);
            //填充参数
            IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
            runStepCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_PUSH_ID;   
            runStepCmd.specialSteps = CUP_TRAY_STACK_MANAGE_OFFSET_PUSH_TRAY_TIGHT_SECOND;
            runStepCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_PUSH_SLOW;//二次推进
            //开始走步数
            errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
            if(errorCode != LH_ERR_NONE)
            {
                errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_PUSH_RUN,errorCode);
                AppShowErrMsg(errorCode);
                return errorCode;
            }
            //延时等待二次推进稳定
            CoreDelayMsSensitivity(TIMS_MS_WAIT_PUSH_TRAY_STABLE);
        }       
    //夹手夹紧
    //读取夹手关闭位置
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_POS_CLAMP_CLOSE,&paramUtil);
    //夹手关闭,走到关闭位置
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_CLAMP_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_CLAMP_CLOSE;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_CLAMP_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //推手回零
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_PUSH_RESET_CORRECT,&paramUtil);
    //电机回零参数设置
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_PUSH_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_PUSH_RETURN;
        returnZeroCmd.timeOutSet = 60000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_PUSH_RETURN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //读取测试到位传感器状态
        errorCode = CanSubInputReadState(INPUT_CUP_TRAY_MANAGE_TEST_TARY_CHECK_EXIST_SENSOR,&inputState);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_READ_INPUT,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        if(inputState == VALID_LEVEL_INPUT_CUP_TRAY_MANAGE_TEST_TARY_CHECK_EXIST_SENSOR)
        {
            testInPlaceSensorStateAfter = SENSOR_STATE_VALID;
        }
        else
        {
            testInPlaceSensorStateAfter = SENSOR_STATE_INVALID;
        }
    //判定推手状态
        if((testInPlaceSensorStateAfter == SENSOR_STATE_INVALID)&&(newTrayTopSensorStateBefore == SENSOR_STATE_VALID))
        {
            //推完测试位置没架子,但是推之前新杯盘顶点有架子,说明架子没退到位
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_PUSH_INPLACE,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //获取要返回给主控的讯息
        errorCode = AppImplCupTrayManageReadSensorStateUtil(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return errorCode;
}


//新杯盘栈上升指定层(门开到位状态)
LH_ERR AppImplCupTrayManageNewStackUpSpecialLayerWithDoorOpenState(uint8_t layers,STACK_MANAGE_RESULT* resultData)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //辅助参数
        int32_t paramUtil;
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取一层的高度
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_SPACING_LAYER_NEW_CUP_TRAY,&paramUtil);
        paramUtil *= layers;
    //空杯栈走到指定位置
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_NEW_TRAY_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_UP;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_UPDOWN_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //获取要返回给主控的讯息
        errorCode = AppImplCupTrayManageReadSensorStateUtil(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return errorCode;
}


//新杯盘栈回零(门开到位状态)
LH_ERR AppImplCupTrayManageNewStackReturnZero(STACK_MANAGE_RESULT* resultData)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //新杯栈回零结构体
        StepMotorReturnZeroCmd returnZeroCmd;
        int32_t paramUtil;
    //读取新杯栈回零修正
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_NEW_CUP_TRAY_STACK_RESET_CORRECT,&paramUtil);
    //新杯栈回零
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_NEW_TRAY_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_NEW_TRAY_DOWN;
        returnZeroCmd.timeOutSet = 60000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_NEW_TRAY_UPDOWN_RETURN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //获取要返回给主控的讯息
        errorCode = AppImplCupTrayManageReadSensorStateUtil(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return errorCode;
}


//空杯盘上升指定层(门开到位状态)
LH_ERR AppImplCupTrayManageEmptyStackUpSpecialLayerWithDoorOpenState(uint8_t layers,STACK_MANAGE_RESULT* resultData)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //辅助参数
        int32_t paramUtil;
        StepMotorRun2CoordationCmd runToPosCmd;
    //读取一层的高度
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_SPACING_LAYER_EMPTY_CUP_TRAY,&paramUtil);
        paramUtil *= layers;
    //空杯栈走到指定位置
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_EMPTY_TRAY_UPDOWN_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_UP;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_EMPTY_TRAY_UPDOWN_RUN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //获取要返回给主控的讯息
        errorCode = AppImplCupTrayManageReadSensorStateUtil(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return errorCode;
}


//空杯盘栈回零(门开到位状态)
LH_ERR AppImplCupTrayManageEmptyStackReturnZero(STACK_MANAGE_RESULT* resultData)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //空杯栈回零结构体
        StepMotorReturnZeroCmd returnZeroCmd;
        int32_t paramUtil;
    //读取空杯栈回零修正
        AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_EMPTY_CUP_TRAY_STACK_RESET_CORRECT,&paramUtil);
    //新杯栈回零
        IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
        returnZeroCmd.motorIndex = CUP_TRAY_MANAGE_STEP_MOTOR_EMPTY_TRAY_UPDOWN_ID;
        returnZeroCmd.correctionCoordinate = paramUtil;
        returnZeroCmd.selectConfig = S_CURVE_CUP_TRAY_MANAGE_EMPTY_TRAY_DOWN;
        returnZeroCmd.timeOutSet = 60000;
    //电机回零,等待完成
        errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplCupTrayManageConvertErrorCode(FLAG_CUP_TRAY_MANAGE_ERR_EMPTY_TRAY_UPDOWN_RETURN,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //获取要返回给主控的讯息
        errorCode = AppImplCupTrayManageReadSensorStateUtil(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    return errorCode;
}


//读取传感器讯息
LH_ERR AppImplCupTrayManageReadSensorState(STACK_MANAGE_RESULT* resultData)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //获取要返回给主控的讯息
        errorCode = AppImplCupTrayManageReadSensorStateUtil(resultData);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
        }
    return errorCode;
}





//步进电机调试配置表
static const MODULE_STEP_MOTOR_RESET_RUN_CONFIG moduleStepMotorResetRunConfigArray[] = {
    {CUP_TRAY_MANAGE_STEP_MOTOR_NEW_TRAY_UPDOWN_ID      ,INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_NEW_CUP_TRAY_STACK_RESET_CORRECT},
    {CUP_TRAY_MANAGE_STEP_MOTOR_EMPTY_TRAY_UPDOWN_ID    ,INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_EMPTY_CUP_TRAY_STACK_RESET_CORRECT},
    {CUP_TRAY_MANAGE_STEP_MOTOR_PUSH_ID                 ,INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_PUSH_RESET_CORRECT},
    {CUP_TRAY_MANAGE_STEP_MOTOR_CLAMP_ID                ,INDEX_CAN_SUB_BOARD2_PARAM_MAIN_CUP_TRAY_STACK_MANAGE,APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_CLAMP_RESET_CORRECT},
};

//指定步进电机复位
LH_ERR AppImplCupTrayManageStepMotorReset(INDEX_MOTOR_CUP_TRAY_STACK_MANAGE index)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //复位指令结构体
        StepMotorResetCmd resetCmd;
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
    //读取修正
        AppParamRead((INDEX_CAN_SUB_BOARD2_PARAM_MAIN)(moduleStepMotorResetRunConfigArray[index].resetCorrectMainIndex),
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
LH_ERR AppImplCupTrayManageStepMotorRunSteps(INDEX_MOTOR_CUP_TRAY_STACK_MANAGE index,int32_t steps,int32_t* posAfterRun)
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








