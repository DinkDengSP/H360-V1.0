/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-17 10:50:58
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-16 15:46:16
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplTubeExit.h"

typedef enum FLAG_TUBE_EXIT_ERR_CONVERT
{
    FLAG_TUBE_EXIT_ERR_READ_INPUT_INDEX             = 0x01,//读输入引脚索引错误
    FLAG_TUBE_EXIT_ERR_READ_INPUT                   = 0x02,//读输入引脚错误
    FLAG_TUBE_EXIT_ERR_WITE_OUTPUT_INDEX            = 0x03,//写输出引脚索引错误
    FLAG_TUBE_EXIT_ERR_WITE_OUTPUT                  = 0x04,//写输出引脚错误
    FLAG_TUBE_EXIT_ERR_STEP_MOTOR_INDEX             = 0x05,//电机索引错误
    FLAG_TUBE_EXIT_ERR_STEP_MOTOR_RESET             = 0x06,//电机复位错误
    FLAG_TUBE_EXIT_ERR_STEP_MOTOR_RUN_STEP          = 0x07,//电机走步数错误
    FLAG_TUBE_EXIT_ERR_READ_STEP_MOTOR_COORDINATE   = 0X08,//读电机坐标错误
}FLAG_TUBE_EXIT_ERR_CONVERT;

//输入引脚配置
const static INPUT_SENSOR_PIN_CONFIG inputSensorPinConfigArray[] = {
    {INPUT_TUBE_EXIT_EXIT_SENSOR,              VAILD_INPUT_TUBE_EXIT_EXIT_SENSOR},//输出节点出口传感器
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
    {STEP_MOTOR_INDEX_RESERVE_COMMON,        APP_PARAM_MAIN_TUBE_EXIT,             0},//保留
};
//电机配置数组长度
#define LENGTH_MODULE_STEP_MOTOR_RESET_RUN_CONFIG_ARRARY    (sizeof(moduleStepMotorResetRunConfigArray)/(sizeof(moduleStepMotorResetRunConfigArray[0])))

//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplTubeExitConvertErrorCode(FLAG_TUBE_EXIT_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_TUBE_EXIT;
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
LH_ERR AppImplTubeExitReadInput(RAIL_ONLINE_TUBE_EXIT_INPUT_INDEX readIndex,SENSOR_STATE *sensorValuePtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(readIndex) >= LENGTH_INPUT_SENSOR_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplTubeExitConvertErrorCode(FLAG_TUBE_EXIT_ERR_READ_INPUT_INDEX,errorCode);
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
LH_ERR AppImplTubeExitWriteOutput(RAIL_ONLINE_TUBE_EXIT_OUTPUT_INDEX WriteIndex,OUT_STATE outState)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(WriteIndex) >= LENGTH_OUTPUT_CTRL_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplTubeExitConvertErrorCode(FLAG_TUBE_EXIT_ERR_WITE_OUTPUT_INDEX,errorCode);
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
LH_ERR AppImplTubeExitStepMotorReset(RAIL_ONLINE_TUBE_EXIT_SM_INDEX motorIndex,int32_t *posAfterResetPtr)
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
        errorCode = AppImplTubeExitConvertErrorCode(FLAG_TUBE_EXIT_ERR_STEP_MOTOR_INDEX,errorCode);
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
        //读取复位修正
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
            errorCode = AppImplTubeExitConvertErrorCode(FLAG_TUBE_EXIT_ERR_STEP_MOTOR_RESET,errorCode);
            AppShowLogError(errorCode);
            return errorCode;
        }
    //读取坐标
        errorCode = IPC_StepMotorReflushStateWhileReturn(resetCmd.motorIndex,posAfterResetPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码转换
            errorCode = AppImplTubeExitConvertErrorCode(FLAG_TUBE_EXIT_ERR_READ_STEP_MOTOR_COORDINATE,errorCode);
            AppShowLogError(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}

//电机走步数
LH_ERR AppImplTubeExitStepMotorRunSteps(RAIL_ONLINE_TUBE_EXIT_SM_INDEX motorIndex,int32_t stepToRun,int32_t *posAfterRunPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //电机索引判断
    if((uint8_t)(motorIndex) >= LENGTH_MODULE_STEP_MOTOR_RESET_RUN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplTubeExitConvertErrorCode(FLAG_TUBE_EXIT_ERR_STEP_MOTOR_INDEX,errorCode);
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
            errorCode = AppImplTubeExitConvertErrorCode(FLAG_TUBE_EXIT_ERR_STEP_MOTOR_RUN_STEP,errorCode);
            AppShowLogError(errorCode);
            return errorCode;
        }
    //读取坐标
        errorCode = IPC_StepMotorReflushStateWhileReturn(runStepCmd.motorIndex,posAfterRunPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码转换
            errorCode = AppImplTubeExitConvertErrorCode(FLAG_TUBE_EXIT_ERR_READ_STEP_MOTOR_COORDINATE,errorCode);
            AppShowLogError(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}

//更新系统传感器讯息
static LH_ERR AppImplTubeExitReflushAllSensor(TUBE_EXIT_SENSOR_SET* tubeExistSensorSetPtr)
{
    AppImplTubeExitReadInput(RAIL_ONLINE_TUBE_EXIT_INPUT_INDEX_EXIT_SENSOR,&(tubeExistSensorSetPtr->tubeExitState));
    return LH_ERR_NONE;
}

static TUBE_EXIT_SENSOR_SET lastTubeExistSensorSet = {SENSOR_STATE_INVALID};
//初始化输出检测状态机
void AppImplTubeExitFSM_Init(void)
{
    APP_STATE_TUBE_EXIT* tubeExistStatePtr = AppStateGetTubeExitStatePtr();
    //更新传感器讯息
    AppImplTubeExitReflushAllSensor(&(tubeExistStatePtr->tubeExtiSensorSet));
    //清除事件
    tubeExistStatePtr->tubExitEventFlag = EVENT_FLAG_NONE;
    //数据拷贝
    UserMemCopy(&lastTubeExistSensorSet,&(tubeExistStatePtr->tubeExtiSensorSet),sizeof(TUBE_EXIT_SENSOR_SET)/sizeof(uint8_t));
}

//运行输出检测状态机
void AppImplTubeExitFSM_Run(void)
{
    APP_STATE_TUBE_EXIT* tubeExistStatePtr = AppStateGetTubeExitStatePtr();
    //更新传感器讯息
    AppImplTubeExitReflushAllSensor(&(tubeExistStatePtr->tubeExtiSensorSet));
    if(tubeExistStatePtr->tubExitEventFlag == EVENT_FLAG_TRIG)
    {
        UserMemCopy(&lastTubeExistSensorSet,&(tubeExistStatePtr->tubeExtiSensorSet),sizeof(TUBE_EXIT_SENSOR_SET)/sizeof(uint8_t));
        return ;
    }
    if((lastTubeExistSensorSet.tubeExitState == SENSOR_STATE_INVALID)&&(tubeExistStatePtr->tubeExtiSensorSet.tubeExitState == SENSOR_STATE_VALID))
    {
        tubeExistStatePtr->tubExitEventFlag = EVENT_FLAG_TRIG;
    }
    UserMemCopy(&lastTubeExistSensorSet,&(tubeExistStatePtr->tubeExtiSensorSet),sizeof(TUBE_EXIT_SENSOR_SET)/sizeof(uint8_t));
}

//输出节点复位
LH_ERR AppImplTubeExitReset(APP_STATE_TUBE_EXIT** tubeExistStatePtrPtr)
{
    APP_STATE_TUBE_EXIT* tubeExistStatePtr = AppStateGetTubeExitStatePtr();
    *tubeExistStatePtrPtr = tubeExistStatePtr;
    //更新传感器讯息
    AppImplTubeExitReflushAllSensor(&(tubeExistStatePtr->tubeExtiSensorSet));
    //清除事件
    tubeExistStatePtr->tubExitEventFlag = EVENT_FLAG_NONE;
    return LH_ERR_NONE;
}

//读传感器节点信息
LH_ERR AppImplTubeExitReadState(APP_STATE_TUBE_EXIT** tubeExistStatePtrPtr)
{
    APP_STATE_TUBE_EXIT* tubeExistStatePtr = AppStateGetTubeExitStatePtr();
    *tubeExistStatePtrPtr = tubeExistStatePtr;
    //更新传感器讯息
    AppImplTubeExitReflushAllSensor(&(tubeExistStatePtr->tubeExtiSensorSet));
    return LH_ERR_NONE;
}

//清除输出节点状态
LH_ERR AppImplTubeExitClearState(void)
{
    APP_STATE_TUBE_EXIT* tubeExistStatePtr = AppStateGetTubeExitStatePtr();
    //清除事件
    tubeExistStatePtr->tubExitEventFlag = EVENT_FLAG_NONE;
    return LH_ERR_NONE;
}


