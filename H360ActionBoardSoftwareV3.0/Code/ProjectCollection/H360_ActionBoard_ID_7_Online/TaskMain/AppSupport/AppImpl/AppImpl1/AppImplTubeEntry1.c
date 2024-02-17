/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-17 10:50:39
**LastEditors: DengXiaoJun
**LastEditTime: 2022-01-13 09:52:39
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplTubeEntry1.h"
#include "AppImplTubeRotate.h"

typedef enum FLAG_TUBE_ENTRY1_ERR_CONVERT
{
    FLAG_TUBE_ENTRY1_ERR_READ_INPUT_INDEX              = 0x01,//读输入引脚索引错误
    FLAG_TUBE_ENTRY1_ERR_READ_INPUT                    = 0x02,//读输入引脚错误
    FLAG_TUBE_ENTRY1_ERR_WITE_OUTPUT_INDEX             = 0x03,//写输出引脚索引错误
    FLAG_TUBE_ENTRY1_ERR_WITE_OUTPUT                   = 0x04,//写输出引脚错误
    FLAG_TUBE_ENTRY1_ERR_STEP_MOTOR_INDEX              = 0x05,//电机索引错误
    FLAG_TUBE_ENTRY1_ERR_STEP_MOTOR_RESET              = 0x06,//电机复位错误
    FLAG_TUBE_ENTRY1_ERR_STEP_MOTOR_RUN_STEP           = 0x07,//电机走步数错误
    FLAG_TUBE_ENTRY1_ERR_READ_STEP_MOTOR_COORDINATE    = 0x08,//读电机坐标错误
}FLAG_TUBE_ENTRY1_ERR_CONVERT;

//输入引脚配置
const static INPUT_SENSOR_PIN_CONFIG inputSensorPinConfigArray[] = {
    {INPUT_TUBE_ENTRY1_INTRANCE_SENSOR,              VAILD_INPUT_TUBE_ENTRY1_INTRANCE_SENSOR},//输入传感器
};
//输入引脚长度
#define LENGTH_INPUT_SENSOR_PIN_CONFIG_ARRARY          (sizeof(inputSensorPinConfigArray)/sizeof(inputSensorPinConfigArray[0])) 

//输出引脚配置
const static OUTPUT_CTRL_PIN_CONFIG outputCtrlPinConfigArrary[] = {
    {OUTPUT_TUBE_ENTRY1_RAIL_IN_OUTPUT_START,            VAILD_OUTPUT_TUBE_ENTRY1_RAIL_IN_OUTPUT_START        },//进样轨道直流电机启停
    {OUTPUT_TUBE_ENTRY1_RAIL_IN_OUTPUT_DIR,              VAILD_OUTPUT_TUBE_ENTRY1_RAIL_IN_OUTPUT_DIR          },//进样轨道直流电机方向控制
    {OUTPUT_TUBE_ENTRY1_RAIL_IN_OUTPUT_EMSTOP,           VAILD_OUTPUT_TUBE_ENTRY1_RAIL_IN_OUTPUT_EMSTOP       },//进样轨道直流电机急停
    {OUTPUT_TUBE_ENTRY1_RAIL_OUT_OUTPUT_START,           VAILD_OUTPUT_TUBE_ENTRY1_RAIL_OUT_OUTPUT_START       },//出样轨道直流电机使能
    {OUTPUT_TUBE_ENTRY1_RAIL_OUT_OUTPUT_DIR,             VAILD_OUTPUT_TUBE_ENTRY1_RAIL_OUT_OUTPUT_DIR         },//出样轨道直流电机方向控制
    {OUTPUT_TUBE_ENTRY1_RAIL_OUT_OUTPUT_EMSTOP,          VAILD_OUTPUT_TUBE_ENTRY1_RAIL_OUT_OUTPUT_EMSTOP      },//出样轨道直流电机急停
};
//输出引脚配置配置数据长度
#define LENGTH_OUTPUT_CTRL_PIN_CONFIG_ARRARY          (sizeof(outputCtrlPinConfigArrary)/sizeof(outputCtrlPinConfigArrary[0])) 

//电机配置数组
const static MODULE_STEP_MOTOR_RESET_RUN_CONFIG moduleStepMotorResetRunConfigArray[]={
    {STEP_MOTOR_INDEX_RESERVE_COMMON,        APP_PARAM_MAIN_TUBE_ENTRY,             0},//保留
};
//电机配置数组长度
#define LENGTH_MODULE_STEP_MOTOR_RESET_RUN_CONFIG_ARRARY    (sizeof(moduleStepMotorResetRunConfigArray)/(sizeof(moduleStepMotorResetRunConfigArray[0])))

//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplTubeEntryConvertErrorCode1(FLAG_TUBE_ENTRY1_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_TUBE_ENTRY;
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
LH_ERR AppImplTubeEntryReadInput1(RAIL_ONLINE_TUBE_ENTRY1_INPUT_INDEX readIndex,SENSOR_STATE *sensorValuePtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(readIndex) >= LENGTH_INPUT_SENSOR_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplTubeEntryConvertErrorCode1(FLAG_TUBE_ENTRY1_ERR_READ_INPUT_INDEX,errorCode);
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
LH_ERR AppImplTubeEntryWriteOutput1(RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX WriteIndex,OUT_STATE outState)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(WriteIndex) >= LENGTH_OUTPUT_CTRL_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplTubeEntryConvertErrorCode1(FLAG_TUBE_ENTRY1_ERR_WITE_OUTPUT_INDEX,errorCode);
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
LH_ERR AppImplTubeEntryStepMotorReset1(RAIL_ONLINE_TUBE_ENTRY1_SM_INDEX motorIndex,int32_t *posAfterResetPtr)
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
        errorCode = AppImplTubeEntryConvertErrorCode1(FLAG_TUBE_ENTRY1_ERR_STEP_MOTOR_INDEX,errorCode);
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
        errorCode = AppImplTubeEntryConvertErrorCode1(FLAG_TUBE_ENTRY1_ERR_STEP_MOTOR_RESET,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //读取坐标
    errorCode = IPC_StepMotorReflushStateWhileReturn(resetCmd.motorIndex,posAfterResetPtr);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplTubeEntryConvertErrorCode1(FLAG_TUBE_ENTRY1_ERR_READ_STEP_MOTOR_COORDINATE,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    return LH_ERR_NONE;
}

//电机走步数
LH_ERR AppImplTubeEntryStepMotorRunSteps1(RAIL_ONLINE_TUBE_ENTRY1_SM_INDEX motorIndex,int32_t stepToRun,int32_t *posAfterRunPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //电机索引判断
    if((uint8_t)(motorIndex) >= LENGTH_MODULE_STEP_MOTOR_RESET_RUN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplTubeEntryConvertErrorCode1(FLAG_TUBE_ENTRY1_ERR_STEP_MOTOR_INDEX,errorCode);
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
        errorCode = AppImplTubeEntryConvertErrorCode1(FLAG_TUBE_ENTRY1_ERR_STEP_MOTOR_RUN_STEP,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //读取坐标
    errorCode = IPC_StepMotorReflushStateWhileReturn(runStepCmd.motorIndex,posAfterRunPtr);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplTubeEntryConvertErrorCode1(FLAG_TUBE_ENTRY1_ERR_READ_STEP_MOTOR_COORDINATE,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    return LH_ERR_NONE;
}

//更新全部传感器讯息
static void AppImplTubeEntryReflushAllSensor1(TUBE_ENTRY_SENSOR_SET* tubeEntrySensorSetPtr)
{
    AppImplTubeEntryReadInput1(RAIL_ONLINE_TUBE_ENTRY1_INPUT_INDEX_INTRANCE_SENSOR,&(tubeEntrySensorSetPtr->intranceState));
}

//上一次的传感器状态
static TUBE_ENTRY_SENSOR_SET lastTubeEntrySensorSet = {SENSOR_STATE_INVALID};

//初始化输入检测状态机
void AppImplTubeEntryFSM_Init1(void)
{
    APP_STATE_TUBE_ENTRY* tubeEntryStatePtr = AppStateGetTubeEntryStatePtr();
    TUBE_ENTRY_SENSOR_SET* tubeEntrySensorSetPtr = &(tubeEntryStatePtr->tubeEntrySensorSet);
    //更新传感器讯息
    AppImplTubeEntryReflushAllSensor1(tubeEntrySensorSetPtr);
    //入口事件清空
    tubeEntryStatePtr->tubeEntryEventFlag = EVENT_FLAG_NONE;
    //赋值上一次和这一次保持一致
    UserMemCopy(&lastTubeEntrySensorSet,tubeEntrySensorSetPtr,sizeof(TUBE_ENTRY_SENSOR_SET)/sizeof(uint8_t));
    //轨道停止
    AppImplTubeEntryRailStop1(&tubeEntryStatePtr);
    //轨道启动
    AppImplTubeEntryRailStart1(&tubeEntryStatePtr);
}

//运行输入检测状态机
void AppImplTubeEntryFSM_Run1(void)
{
    APP_STATE_TUBE_ENTRY* tubeEntryStatePtr = AppStateGetTubeEntryStatePtr();
    TUBE_ENTRY_SENSOR_SET* tubeEntrySensorSetPtr = &(tubeEntryStatePtr->tubeEntrySensorSet);
    //更新传感器讯息
    AppImplTubeEntryReflushAllSensor1(tubeEntrySensorSetPtr);
    //如果已经触发
    if(tubeEntryStatePtr->tubeEntryEventFlag == EVENT_FLAG_TRIG)
    {
        return;
    }
    //触发
    if((tubeEntrySensorSetPtr->intranceState == SENSOR_STATE_VALID)&&(lastTubeEntrySensorSet.intranceState == SENSOR_STATE_INVALID))
    {
        tubeEntryStatePtr->tubeEntryEventFlag = EVENT_FLAG_TRIG;
    }
    //状态拷贝
    UserMemCopy(&lastTubeEntrySensorSet,tubeEntrySensorSetPtr,sizeof(TUBE_ENTRY_SENSOR_SET)/sizeof(uint8_t));
}

//输入节点复位
LH_ERR AppImplTubeEntryReset1(APP_STATE_TUBE_ENTRY** tubeEntryStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_TUBE_ENTRY* tubeEntryStatePtr = AppStateGetTubeEntryStatePtr();
    *tubeEntryStatePtrPtr = tubeEntryStatePtr;
    //轨道停止
    errorCode = AppImplTubeEntryRailStop1(tubeEntryStatePtrPtr);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowLogError(errorCode);
        return errorCode; 
    }
    //更新传感器状态
    AppImplTubeEntryReflushAllSensor1(&(tubeEntryStatePtr->tubeEntrySensorSet));
    //入口事件清除
    tubeEntryStatePtr->tubeEntryEventFlag = EVENT_FLAG_NONE;
    //轨道启动
    errorCode = AppImplTubeEntryRailStart1(tubeEntryStatePtrPtr);
    if(errorCode != LH_ERR_NONE)
    {
        AppShowLogError(errorCode);
        return errorCode; 
    }
    return errorCode;
}

//轨道启动运转
LH_ERR AppImplTubeEntryRailStart1(APP_STATE_TUBE_ENTRY** tubeEntryStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_TUBE_ENTRY* tubeEntryStatePtr = AppStateGetTubeEntryStatePtr();
    *tubeEntryStatePtrPtr = tubeEntryStatePtr;
    //输入轨道启动
    //解除急停
    errorCode = AppImplTubeEntryWriteOutput1(RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_IN_OUTPUT_EMSTOP,OUT_STATE_DISABLE);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    CoreDelayMs(10);
    //轨道输入电机使能 
    errorCode = AppImplTubeEntryWriteOutput1(RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_IN_OUTPUT_START,OUT_STATE_ENABLE);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    CoreDelayMs(10);
    //轨道输入电机方向写入 
    errorCode = AppImplTubeEntryWriteOutput1(RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_IN_OUTPUT_DIR,OUT_STATE_ENABLE);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //输出轨道启动
    //解除急停
    errorCode = AppImplTubeEntryWriteOutput1(RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_OUT_OUTPUT_EMSTOP,OUT_STATE_DISABLE);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    CoreDelayMs(10);
    //轨道输出电机使能 
    errorCode = AppImplTubeEntryWriteOutput1(RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_OUT_OUTPUT_START,OUT_STATE_ENABLE);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    CoreDelayMs(10);
    //轨道输出电机方向写入 
    errorCode = AppImplTubeEntryWriteOutput1(RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_OUT_OUTPUT_DIR,OUT_STATE_ENABLE);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    CoreDelayMs(10);
    //更新传感器讯息
    AppImplTubeEntryReflushAllSensor1(&(tubeEntryStatePtr->tubeEntrySensorSet));
    return errorCode;
}

//轨道停止运转
LH_ERR AppImplTubeEntryRailStop1(APP_STATE_TUBE_ENTRY** tubeEntryStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_TUBE_ENTRY* tubeEntryStatePtr = AppStateGetTubeEntryStatePtr();
    *tubeEntryStatePtrPtr = tubeEntryStatePtr;
    //进管轨道停止
    //启动信号关闭
    errorCode = AppImplTubeEntryWriteOutput1(RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_IN_OUTPUT_START,OUT_STATE_DISABLE);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //出管轨道停止
    //轨道急停
    errorCode = AppImplTubeEntryWriteOutput1(RAIL_ONLINE_TUBE_ENTRY1_OUTPUT_INDEX_RAIL_OUT_OUTPUT_START,OUT_STATE_DISABLE);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器讯息
    AppImplTubeEntryReflushAllSensor1(&(tubeEntryStatePtr->tubeEntrySensorSet));
    return errorCode;
}

//读传感器节点信息
LH_ERR AppImplTubeEntryReadState1(APP_STATE_TUBE_ENTRY** tubeEntryStatePtrPtr)
{
    APP_STATE_TUBE_ENTRY* tubeEntryStatePtr = AppStateGetTubeEntryStatePtr();
    *tubeEntryStatePtrPtr = tubeEntryStatePtr;
    //更新传感器讯息
    AppImplTubeEntryReflushAllSensor1(&(tubeEntryStatePtr->tubeEntrySensorSet));
    return LH_ERR_NONE;
}

//读取转盘入口信息,是否存在底座
LH_ERR AppImplTubeEntryReadRotateInput1(APP_STATE_TUBE_ROTATE** tubeRotateStatePtrPtr)
{
    return AppImplTubeRotateReadState(tubeRotateStatePtrPtr);
}

//清除节点状态
LH_ERR AppImplTubeEntryClearState1(void)
{
    APP_STATE_TUBE_ENTRY* tubeEntryStatePtr = AppStateGetTubeEntryStatePtr();
    //更新传感器讯息
    AppImplTubeEntryReflushAllSensor1(&(tubeEntryStatePtr->tubeEntrySensorSet));
    //清除事件
    tubeEntryStatePtr->tubeEntryEventFlag = EVENT_FLAG_NONE;
    return LH_ERR_NONE;
}

