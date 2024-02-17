/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-17 10:51:38
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-16 13:27:54
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplTubeClamp.h"

typedef enum FLAG_TUBE_CLAMP_ERR_CONVERT
{
    FLAG_TUBE_CLAMP_ERR_READ_INPUT_INDEX            = 0x01,//读输入引脚索引错误
    FLAG_TUBE_CLAMP_ERR_READ_INPUT                  = 0x02,//读输入引脚错误
    FLAG_TUBE_CLAMP_ERR_WITE_OUTPUT_INDEX           = 0x03,//写输出引脚索引错误
    FLAG_TUBE_CLAMP_ERR_WITE_OUTPUT                 = 0x04,//写输出引脚错误
    FLAG_TUBE_CLAMP_ERR_STEP_MOTOR_INDEX            = 0x05,//电机索引错误
    FLAG_TUBE_CLAMP_ERR_STEP_MOTOR_RESET            = 0x06,//电机复位错误
    FLAG_TUBE_CLAMP_ERR_STEP_MOTOR_RUN_STEP         = 0x07,//电机走步数错误
    FLAG_TUBE_CLAMP_ERR_READ_STEP_MOTOR_COORDINATE  = 0X08,//读电机坐标错误
    FLAG_TUBE_CLAMP_ERR_READ_PARAM                  = 0X09,//读参数产生的错误
    FLAG_TUBE_CLAMP_ERR_STEP_MOTOR_RUN_COORDIATE    = 0X0A,//电机走坐标的错误
    FLAG_TUBE_CLAMP_ERR_STEP_MOTOR_RETURN_ZERO      = 0X0B,//电机回零的错误
}FLAG_TUBE_CLAMP_ERR_CONVERT;

//输入引脚配置
const static INPUT_SENSOR_PIN_CONFIG inputSensorPinConfigArray[] = {
    {INPUT_RAIL_ONLINE_TUBE_CLAMP_SM_UPDOWN_ORIGIN_SENSOR,              VAILD_INPUT_RAIL_ONLINE_TUBE_CLAMP_SM_UPDOWN_ORIGIN_SENSOR},//电机原点传感器
};
//输入引脚长度
#define LENGTH_INPUT_SENSOR_PIN_CONFIG_ARRARY          (sizeof(inputSensorPinConfigArray)/sizeof(inputSensorPinConfigArray[0])) 

//输出引脚配置
const static OUTPUT_CTRL_PIN_CONFIG outputCtrlPinConfigArrary[] = {
    {OUTPUT_COMMON_OUTPUT_ENABLE,           VAILD_OUTPUT_COMMON_OUTPUT_ENABLE },//保留
};
//输出引脚配置配置数据长度
#define LENGTH_OUTPUT_CTRL_PIN_CONFIG_ARRARY          (sizeof(outputCtrlPinConfigArrary)/sizeof(outputCtrlPinConfigArrary[0])) 


//电机配置数组
const static MODULE_STEP_MOTOR_RESET_RUN_CONFIG moduleStepMotorResetRunConfigArray[]={
    {TUBE_CLAMP_SM_MAIN,        APP_PARAM_MAIN_TUBE_CLAMP,           APP_PARAM_SUB_TUBE_CLAMP_INDEX_UPDOWN_STEP_MOTOR_RESET_CORRECT_STEP},//测试试管固定电机
};
//电机配置数组长度
#define LENGTH_MODULE_STEP_MOTOR_RESET_RUN_CONFIG_ARRARY    (sizeof(moduleStepMotorResetRunConfigArray)/(sizeof(moduleStepMotorResetRunConfigArray[0])))


//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplTubeClampConvertErrorCode(FLAG_TUBE_CLAMP_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_TUBE_CLAMP;
    moduleNo <<= 24;
    //错误标志
    uint32_t flagInsert = flag;
    flagInsert <<= 16;
    //基础错误代码
    errResult = LH_ERR_BOARD7_BASE;
    //加入模块号
    errResult |= moduleNo;
    //加入错误标志
    errResult |= flagInsert;
    //加入原始错误代码
    errResult |= errSrc;
    //返回转换完成的错误代码
    return errResult;
}


//读取输入
LH_ERR AppImplTubeClampReadInput(RAIL_ONLINE_TUBE_CLAMP_INPUT_INDEX readIndex,SENSOR_STATE *sensorValuePtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(readIndex) >= LENGTH_INPUT_SENSOR_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode =AppImplTubeClampConvertErrorCode(FLAG_TUBE_CLAMP_ERR_READ_INPUT_INDEX,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //读引脚电平
    pinValue = BoardInReadValue(inputSensorPinConfigArray[readIndex].inputPinIndex);
    //有效电平判断
    if(pinValue == inputSensorPinConfigArray[readIndex].pinVaildState)
    {
        *sensorValuePtr = SENSOR_STATE_VALID;
    }
    else
    {
        *sensorValuePtr = SENSOR_STATE_INVALID;
    }
    return LH_ERR_NONE;
}

//输出写出
LH_ERR AppImplTubeClampWriteOutput(RAIL_ONLINE_TUBE_CLAMP_OUTPUT_INDEX WriteIndex,OUT_STATE outState)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(WriteIndex) >= LENGTH_OUTPUT_CTRL_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplTubeClampConvertErrorCode(FLAG_TUBE_CLAMP_ERR_WITE_OUTPUT_INDEX,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //写出引脚电平
    if(outState == OUT_STATE_ENABLE)
    {
        pinValue = outputCtrlPinConfigArrary[WriteIndex].pinVaildState;
    }
    else
    {
        pinValue = (BitAction)!(outputCtrlPinConfigArrary[WriteIndex].pinVaildState);
    }
    //写输出引脚
    BoardOutWrite(outputCtrlPinConfigArrary[WriteIndex].outputPinIndex,pinValue);
    return LH_ERR_NONE;
}

//电机复位
LH_ERR AppImplTubeClampStepMotorReset(RAIL_ONLINE_TUBE_CLAMP_SM_INDEX motorIndex,int32_t *posAfterResetPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //修正
    int32_t paramUtil = 0;
    //复位指令结构体
        StepMotorResetCmd resetCmd;
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
    //电机索引判断
    if((uint8_t)(motorIndex) >= LENGTH_MODULE_STEP_MOTOR_RESET_RUN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode =AppImplTubeClampConvertErrorCode(FLAG_TUBE_CLAMP_ERR_STEP_MOTOR_INDEX,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //读电机复位修正
    if(moduleStepMotorResetRunConfigArray[motorIndex].resetCorrectSubIndex == 0)
    {
        //无复位修正参数
        paramUtil = 0;
    }
    else 
    {
        AppParamReadParam((APP_PARAM_MAIN_INDEX)(moduleStepMotorResetRunConfigArray[motorIndex].resetCorrectMainIndex),moduleStepMotorResetRunConfigArray[motorIndex].resetCorrectSubIndex,&paramUtil);
    }
    //填充参数
        resetCmd.motorIndex = moduleStepMotorResetRunConfigArray[motorIndex].motorIndex;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//复位修正
        resetCmd.timeOutSet = 60000;//超时时间
    //开始复位
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码转换
            errorCode =AppImplTubeClampConvertErrorCode(FLAG_TUBE_CLAMP_ERR_STEP_MOTOR_RESET,errorCode);
            AppShowLogError(errorCode);
            return errorCode;
        }
    //读取坐标
        errorCode = IPC_StepMotorReflushStateWhileReturn(resetCmd.motorIndex,posAfterResetPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码转换
            errorCode =AppImplTubeClampConvertErrorCode(FLAG_TUBE_CLAMP_ERR_READ_STEP_MOTOR_COORDINATE,errorCode);
            AppShowLogError(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}

//电机走步数
LH_ERR AppImplTubeClampStepMotorRunSteps(RAIL_ONLINE_TUBE_CLAMP_SM_INDEX motorIndex,int32_t stepToRun,int32_t *posAfterRunPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //电机索引判断
    if((uint8_t)(motorIndex) >= LENGTH_MODULE_STEP_MOTOR_RESET_RUN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode =AppImplTubeClampConvertErrorCode(FLAG_TUBE_CLAMP_ERR_STEP_MOTOR_INDEX,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //填充参数
        runStepCmd.motorIndex = moduleStepMotorResetRunConfigArray[motorIndex].motorIndex;
        runStepCmd.specialSteps = stepToRun;
        runStepCmd.selectConfig = SM_CURVE_0;//默认曲线0作为调试曲线
    //开始走步数
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码转换
            errorCode =AppImplTubeClampConvertErrorCode(FLAG_TUBE_CLAMP_ERR_STEP_MOTOR_RUN_STEP,errorCode);
            AppShowLogError(errorCode);
            return errorCode;
        }
    //读取坐标
        errorCode = IPC_StepMotorReflushStateWhileReturn(runStepCmd.motorIndex,posAfterRunPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码转换
            errorCode =AppImplTubeClampConvertErrorCode(FLAG_TUBE_CLAMP_ERR_READ_STEP_MOTOR_COORDINATE,errorCode);
            AppShowLogError(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}

//初始化试管机构状态机
void AppImplTubeClampFSM_Init(void)
{
    APP_STATE_TUBE_CLAMP* tubeClampStatePtr = AppStateGetTubeClampStatePtr();
    tubeClampStatePtr->tubeClampState = TUBE_CLAMP_UNKNOW;
}

//试管扶正机构运转
void AppImplTubeClampFSM_Run(void)
{
    asm("nop");
}

//试管扶正机构复位
LH_ERR AppImplTubeClampReset(APP_STATE_TUBE_CLAMP** tubeClampStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_TUBE_CLAMP* tubeClampStatePtr = AppStateGetTubeClampStatePtr();
    *tubeClampStatePtrPtr = tubeClampStatePtr;
    //机构复位
    int32_t paramUtil = 0;
    //复位指令结构体
    StepMotorResetCmd resetCmd;
    IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
    //读复位修正
    errorCode = AppParamReadParam(APP_PARAM_MAIN_TUBE_CLAMP,APP_PARAM_SUB_TUBE_CLAMP_INDEX_UPDOWN_STEP_MOTOR_RESET_CORRECT_STEP,&paramUtil);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode =AppImplTubeClampConvertErrorCode(FLAG_TUBE_CLAMP_ERR_READ_PARAM,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //升降电机复位
    resetCmd.motorIndex = TUBE_CLAMP_SM_MAIN;//电机序号
    resetCmd.correctionCoordinate = paramUtil;//复位修正
    resetCmd.timeOutSet = 60000;//超时时间
    //开始复位
    errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode =AppImplTubeClampConvertErrorCode(FLAG_TUBE_CLAMP_ERR_STEP_MOTOR_RESET,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }    
    //设置扶正机构状态为释放
    tubeClampStatePtr->tubeClampState = TUBE_CLAMP_RELEASE;
    return LH_ERR_NONE;
}

//试管扶正机构抓住
LH_ERR AppImplTubeClampCatch(APP_STATE_TUBE_CLAMP** tubeClampStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_TUBE_CLAMP* tubeClampStatePtr = AppStateGetTubeClampStatePtr();
    *tubeClampStatePtrPtr = tubeClampStatePtr;
    int32_t paramUtil = 0;
    StepMotorRun2CoordationCmd runToPosCmd;
    IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
    //读取下降坐标
    errorCode = AppParamReadParam(APP_PARAM_MAIN_TUBE_CLAMP,APP_PARAM_SUB_TUBE_CLAMP_INDEX_UPDOWN_STEP_MOTOR_DOWN_POS,&paramUtil);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode =AppImplTubeClampConvertErrorCode(FLAG_TUBE_CLAMP_ERR_READ_PARAM,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //扶正机构走固定试管的坐标
    runToPosCmd.motorIndex = TUBE_CLAMP_SM_MAIN;
    runToPosCmd.targetCoordinate = paramUtil;
    runToPosCmd.selectConfig = S_CURVE_TUBE_CLAMP_SM_MAIN_DOWN;
    errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode =AppImplTubeClampConvertErrorCode(FLAG_TUBE_CLAMP_ERR_STEP_MOTOR_RUN_COORDIATE,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //设置扶正机构状态为夹紧
    tubeClampStatePtr->tubeClampState = TUBE_CLAMP_CATCH;
    return LH_ERR_NONE;
}

//试管扶正机构释放
LH_ERR AppImplTubeClampRelease(APP_STATE_TUBE_CLAMP** tubeClampStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_TUBE_CLAMP* tubeClampStatePtr = AppStateGetTubeClampStatePtr();
    *tubeClampStatePtrPtr = tubeClampStatePtr;
    int32_t paramUtil = 0;
    StepMotorReturnZeroCmd returnZeroCmd;
    IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
    //读取复位修正
    errorCode = AppParamReadParam(APP_PARAM_MAIN_TUBE_CLAMP,APP_PARAM_SUB_TUBE_CLAMP_INDEX_UPDOWN_STEP_MOTOR_RESET_CORRECT_STEP,&paramUtil);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode =AppImplTubeClampConvertErrorCode(FLAG_TUBE_CLAMP_ERR_READ_PARAM,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //扶正机构回零
    returnZeroCmd.motorIndex = TUBE_CLAMP_SM_MAIN;
    returnZeroCmd.selectConfig = S_CURVE_TUBE_CLAMP_SM_MAIN_UP;
    returnZeroCmd.correctionCoordinate = paramUtil;
    errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode =AppImplTubeClampConvertErrorCode(FLAG_TUBE_CLAMP_ERR_STEP_MOTOR_RETURN_ZERO,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //设置扶正机构状态为释放
    tubeClampStatePtr->tubeClampState = TUBE_CLAMP_RELEASE;
    return LH_ERR_NONE;
}

//读传感器事件状态
LH_ERR AppImplTubeClampReadState(APP_STATE_TUBE_CLAMP** tubeClampStatePtrPtr)
{
    APP_STATE_TUBE_CLAMP* tubeClampStatePtr = AppStateGetTubeClampStatePtr();
    *tubeClampStatePtrPtr = tubeClampStatePtr;
    return LH_ERR_NONE;
}

//清除节点事件状态
LH_ERR AppImplTubeClampClearState(void)
{
    return LH_ERR_NONE;
}



