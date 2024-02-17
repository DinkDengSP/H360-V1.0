/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-11-08 16:12:48
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-25 16:44:49
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplRackOutSwitch.h"
#include "AppImplParamUtil.h"


//模块错误代码配置
typedef enum FLAG_RACK_OUT_SWITCH_ERR_CONVERT
{
    FLAG_RACK_OUT_SWITCH_ERR_READ_INPUT_INDEX                = 0x01,//读输入引脚索引错误
    FLAG_RACK_OUT_SWITCH_ERR_READ_INPUT                      = 0x02,//读输入引脚错误
    FLAG_RACK_OUT_SWITCH_ERR_WRITE_OUTPUT_INDEX              = 0x03,//写输出引脚索引错误
    FLAG_RACK_OUT_SWITCH_ERR_WRITE_OUTPUT                    = 0x04,//写输出引脚错误
    FLAG_RACK_OUT_SWITCH_ERR_STEP_MOTOR_INDEX                = 0x05,//电机索引错误
    FLAG_RACK_OUT_SWITCH_ERR_STEP_MOTOR_RESET                = 0x06,//电机复位错误
    FLAG_RACK_OUT_SWITCH_ERR_STEP_MOTOR_RUN_STEP             = 0x07,//电机走步数错误
    FLAG_RACK_OUT_SWITCH_ERR_STEP_MOTOR_READ_POS             = 0X08,//电机读坐标错误
    FLAG_RACK_OUT_SWITCH_ERR_MOTOR_SWITCH_RESET              = 0X09,//换轨电机复位错误
    FLAG_RACK_OUT_SWITCH_ERR_MOTOR_SWITCH_RETURN_ZERO        = 0X0A,//换轨电机回零错误
    FLAG_RACK_OUT_SWITCH_ERR_MOTOR_SWITCH_RUN_IN             = 0X0B,//换轨电机移动到入口端错误
    FLAG_RACK_OUT_SWITCH_ERR_MOTOR_SWITCH_RUN_OUT            = 0X0C,//换轨电机移动到出口端错误
    FLAG_RACK_OUT_SWITCH_ERR_RACK_MOVE_OUT_TIME_OUT_VALID    = 0X0D,//等待离开有效超时
    FLAG_RACK_OUT_SWITCH_ERR_RACK_MOVE_OUT_TIME_OUT_INVALID  = 0X0E,//等待离开无效超时
    FLAG_RACK_OUT_SWITCH_ERR_MOVE_OUT_RACK_EXIST             = 0X0F,//执行完换轨入口传感器依然检测到
    FLAG_RACK_OUT_SWITCH_ERR_TRIG_STATE_SWITCH               = 0X10,//当前位置不在入口,不能执行换轨
    FLAG_RACK_OUT_SWITCH_ERR_COME_IN_VALID                   = 0X11,//执行完换轨入口传感器依然有效
    FLAG_RACK_OUT_SWITCH_ERR_MOVE_OUT_INVALID                = 0X12,//执行完换轨出口传感器无效
}FLAG_RACK_OUT_SWITCH_ERR_CONVERT;

//输入引脚配置
static const INPUT_SENSOR_PIN_CONFIG inputSensorPinConfigArray[] = {
    {INPUT_RACK_OUT_SWITCH_INPUT_COME_IN        , VAILD_INPUT_RACK_OUT_SWITCH_INPUT_COME_IN     },//换轨进入传感器
    {INPUT_RACK_OUT_SWITCH_INPUT_MOVE_OUT       , VAILD_INPUT_RACK_OUT_SWITCH_INPUT_MOVE_OUT    },//换轨离去传感器
    {INPUT_RACK_OUT_SWITCH_INPUT_MOTOR_ORIGIN   , VAILD_INPUT_RACK_OUT_SWITCH_INPUT_MOTOR_ORIGIN},//出样换轨电机原点
};
//输入引脚配置表长度
#define LENGTH_INPUT_SENSOR_PIN_CONFIG_ARRARY          (sizeof(inputSensorPinConfigArray)/sizeof(inputSensorPinConfigArray[0])) 

//输出引脚配置
const static OUTPUT_CTRL_PIN_CONFIG outputCtrlPinConfigArrary[] = {
    {OUTPUT_COMMON_OUTPUT_ENABLE,  VAILD_OUTPUT_COMMON_OUTPUT_ENABLE },//占位符
};
//输出引脚配置配置数据长度
#define LENGTH_OUTPUT_CTRL_PIN_CONFIG_ARRARY          (sizeof(outputCtrlPinConfigArrary)/sizeof(outputCtrlPinConfigArrary[0])) 

//步进电机调试配置表
static const MODULE_STEP_MOTOR_RESET_RUN_CONFIG moduleStepMotorResetRunConfigArray[] = {
    {RACK_OUT_SWITCH_SM_MAIN_ID    ,APP_PARAM_MAIN_RACK_OUT_SWITCH ,APP_PARAM_SUB_RACK_OUT_SWITCH_COME_IN_POS  },//换轨电机
};
//步进电机配置表长度
#define LENGTH_STEP_MOTOR_RESET_RUN_CONFIG_ARRAY        (sizeof(moduleStepMotorResetRunConfigArray)/sizeof(moduleStepMotorResetRunConfigArray[0]))


//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplRackOutSwitchConvertErrorCode(FLAG_RACK_OUT_SWITCH_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_RACK_OUT_SWITCH;
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

//读取指定输入状态
LH_ERR AppImplRackOutSwitchReadInput(RACK_OUT_SWITCH_INPUT inputIndex,SENSOR_STATE* sensorStatePtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(inputIndex) >= LENGTH_INPUT_SENSOR_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplRackOutSwitchConvertErrorCode(FLAG_RACK_OUT_SWITCH_ERR_READ_INPUT_INDEX,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //读引脚电平
    pinValue = BoardInReadValue(inputSensorPinConfigArray[inputIndex].inputPinIndex);
    //有效电平判断
    if(pinValue == inputSensorPinConfigArray[inputIndex].pinVaildState)
    {
        *sensorStatePtr = SENSOR_STATE_VALID;
    }
    else
    {
        *sensorStatePtr = SENSOR_STATE_INVALID;
    }
    return LH_ERR_NONE;
}

//写入指定输出状态
LH_ERR AppImplRackOutSwitchWriteOutput(RACK_OUT_SWITCH_OUTPUT outputIndex,OUT_STATE setState)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(outputIndex) >= LENGTH_OUTPUT_CTRL_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplRackOutSwitchConvertErrorCode(FLAG_RACK_OUT_SWITCH_ERR_WRITE_OUTPUT_INDEX,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //写出引脚电平
    if(setState == OUT_STATE_ENABLE)
    {
        pinValue = outputCtrlPinConfigArrary[outputIndex].pinVaildState;
    }
    else
    {
        pinValue = (BitAction)!(outputCtrlPinConfigArrary[outputIndex].pinVaildState);
    }
    //写输出引脚
    BoardOutWrite(outputCtrlPinConfigArrary[outputIndex].outputPinIndex,pinValue);
    return LH_ERR_NONE;
}

//更新传感器讯息
static void AppImplRackOutSwitchReflushSensorSet(RACK_OUT_SWITCH_SENSOR_SET* rackOutSwitchSensorSetPtr)
{
    if(rackOutSwitchSensorSetPtr == NULL)return;
    AppImplRackOutSwitchReadInput(RACK_OUT_SWITCH_INPUT_COME_IN,&(rackOutSwitchSensorSetPtr->rackComeInState));
    AppImplRackOutSwitchReadInput(RACK_OUT_SWITCH_INPUT_MOVE_OUT,&(rackOutSwitchSensorSetPtr->rackMoveOutState));
}

//换轨电机复位
static LH_ERR AppImplRackOutSwitchMotorReset(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //复位指令结构体
    StepMotorResetCmd resetCmd;
    //初始化复位参数
    IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
    //换轨电机复位
    //获取复位修正参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_OUT_SWITCH,APP_PARAM_SUB_RACK_OUT_SWITCH_COME_IN_POS,&paramUtil);
    //填充参数
    resetCmd.motorIndex = RACK_OUT_SWITCH_SM_MAIN_ID;//电机序号
    resetCmd.timeOutSet = 60000;//超时时间
    resetCmd.correctionCoordinate = paramUtil;//复位修正
    //开始复位
    errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplRackOutSwitchConvertErrorCode(FLAG_RACK_OUT_SWITCH_ERR_MOTOR_SWITCH_RESET,errorCode);
        return errorCode;
    }
    return errorCode;
}

//换轨电机回零
static LH_ERR AppImplRackOutSwitchMotorReturnZero(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机回零结构体
    StepMotorReturnZeroCmd returnZeroCmd;
    //出样旋转
    //获取复位修正参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_OUT_SWITCH,APP_PARAM_SUB_RACK_OUT_SWITCH_COME_IN_POS,&paramUtil);
    //回零结构体初始化
    IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
    //配置回零参数
    returnZeroCmd.motorIndex = RACK_OUT_SWITCH_SM_MAIN_ID;
    returnZeroCmd.correctionCoordinate = paramUtil;
    returnZeroCmd.selectConfig = S_CURVE_RACK_OUT_SWITCH_SM_MAIN_RETURN_ZERO;
    returnZeroCmd.timeOutSet = 60000;
    errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //回零失败
        errorCode = AppImplRackOutSwitchConvertErrorCode(FLAG_RACK_OUT_SWITCH_ERR_MOTOR_SWITCH_RETURN_ZERO,errorCode);
        return errorCode;
    }
    return errorCode;
}

//换轨电机移动到样本进入位置
static LH_ERR AppImplRackOutSwitchMotorRunToComeIn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机走坐标结构体
    StepMotorRun2CoordationCmd runToPosCmd;
    //初始化走坐标参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_OUT_SWITCH,APP_PARAM_SUB_RACK_OUT_SWITCH_COME_IN_POS,&paramUtil);
    //初始化走目标坐标结构
    IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
    //填充走坐标结构体
    runToPosCmd.motorIndex = RACK_OUT_SWITCH_SM_MAIN_ID;//电机序号
    runToPosCmd.targetCoordinate = paramUtil;//目标坐标
    runToPosCmd.selectConfig = S_CURVE_RACK_OUT_SWITCH_SM_MAIN_SWITCH;//速度曲线
    runToPosCmd.timeOutSet = 60000;
    runToPosCmd.utilStop1Enable = 0;
    //走坐标
    errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
    if(errorCode != LH_ERR_NONE)
    {
        errorCode = AppImplRackOutSwitchConvertErrorCode(FLAG_RACK_OUT_SWITCH_ERR_MOTOR_SWITCH_RUN_IN,errorCode);
        return errorCode;
    }
    return errorCode;
}

//换轨电机移动到样本离开位置
static LH_ERR AppImplRackOutSwitchMotorRunToMoveOut(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机走坐标结构体
    StepMotorRun2CoordationCmd runToPosCmd;
    //初始化走目标坐标结构
    IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
    //初始化走坐标参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_OUT_SWITCH,APP_PARAM_SUB_RACK_OUT_SWITCH_MOVE_OUT_POS,&paramUtil);
    //填充走坐标结构体
    runToPosCmd.motorIndex = RACK_OUT_SWITCH_SM_MAIN_ID;//电机序号
    runToPosCmd.targetCoordinate = paramUtil;//目标坐标
    runToPosCmd.selectConfig = S_CURVE_RACK_OUT_SWITCH_SM_MAIN_SWITCH;//速度曲线
    runToPosCmd.timeOutSet = 60000;
    runToPosCmd.utilStop1Enable = 0;
    //走坐标
    errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
    if(errorCode != LH_ERR_NONE)
    {
        errorCode = AppImplRackOutSwitchConvertErrorCode(FLAG_RACK_OUT_SWITCH_ERR_MOTOR_SWITCH_RUN_OUT,errorCode);
        return errorCode;
    }
    return errorCode;
}

//上一次的信号状态
static RACK_OUT_SWITCH_SENSOR_SET lastRackOutSwitchSensorSet = {SENSOR_STATE_VALID};
//初始化功能状态机
void AppImplRackOutSwitchFSM_Init(void)
{
    APP_STATE_RACK_OUT_SWITCH* rackOutSwitchStatePtr = AppStateReadRackOutSwitchState();
    //状态初始化
    rackOutSwitchStatePtr->rackComeInEventFlag = EVENT_FLAG_NONE;
    //初始化的时候,节点处于位置状态
    rackOutSwitchStatePtr->railSwitchState = RAIL_SWITCH_STATE_UNKNOW;
    //读取并记录传感器状态
    AppImplRackOutSwitchReflushSensorSet(&(rackOutSwitchStatePtr->rackOutSwitchSensorSet));
    //数据拷贝
    UserMemCopy(&lastRackOutSwitchSensorSet,&(rackOutSwitchStatePtr->rackOutSwitchSensorSet),sizeof(RACK_OUT_SWITCH_SENSOR_SET)/sizeof(uint8_t));
}

//运行功能状态机
void AppImplRackOutSwitchFSM_Run(void)
{
    //如果节点处于未知状态,状态机不运行
    APP_STATE_RACK_OUT_SWITCH* rackOutSwitchStatePtr = AppStateReadRackOutSwitchState();
    if(rackOutSwitchStatePtr->railSwitchState == RAIL_SWITCH_STATE_UNKNOW)
    {
        return;
    }
    //如果入口事件处于已经触发状态,状态机不运行
    if(rackOutSwitchStatePtr->rackComeInEventFlag == EVENT_FLAG_TRIG)
    {
        return;
    }
    //检查标志变化,首先读取当前状态
    AppImplRackOutSwitchReflushSensorSet(&(rackOutSwitchStatePtr->rackOutSwitchSensorSet));
    if(rackOutSwitchStatePtr->rackOutSwitchSensorSet.rackComeInState == SENSOR_STATE_VALID)
    {
        //本次传感器状态为有效且上次传感器状态为无效的情况下,才能触发事件
        if(lastRackOutSwitchSensorSet.rackComeInState == SENSOR_STATE_INVALID)
        {
            rackOutSwitchStatePtr->rackComeInEventFlag = EVENT_FLAG_TRIG;
        }
    }
    //更新检测讯息,数据拷贝
    UserMemCopy(&lastRackOutSwitchSensorSet,&(rackOutSwitchStatePtr->rackOutSwitchSensorSet),sizeof(RACK_OUT_SWITCH_SENSOR_SET)/sizeof(uint8_t));
}

//指定步进电机复位
LH_ERR AppImplRackOutSwitchStepMotorReset(RACK_OUT_SWITCH_SM smIndex,int32_t* currentPos)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //复位指令结构体
    StepMotorResetCmd resetCmd;
    IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
    //索引判断
    if((uint8_t)(smIndex) >= LENGTH_STEP_MOTOR_RESET_RUN_CONFIG_ARRAY)
    {
        //错误代码转换
        errorCode = AppImplRackOutSwitchConvertErrorCode(FLAG_RACK_OUT_SWITCH_ERR_STEP_MOTOR_INDEX,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //当子参数为0的情况下,表示不存在复位修正参数,因为子参数都是从1开始的
    if(moduleStepMotorResetRunConfigArray[smIndex].resetCorrectSubIndex != 0)
    {
        //读取修正
        AppImplParamUtilReadParam(moduleStepMotorResetRunConfigArray[smIndex].resetCorrectMainIndex,
                                    moduleStepMotorResetRunConfigArray[smIndex].resetCorrectSubIndex,
                                    &paramUtil);
    }
    else
    {
        paramUtil = 0;
    }
    //填充参数
    resetCmd.motorIndex = moduleStepMotorResetRunConfigArray[smIndex].motorIndex;//电机序号
    resetCmd.timeOutSet = 60000;//超时时间
    resetCmd.correctionCoordinate = paramUtil;//复位修正
    //开始复位
    errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplRackOutSwitchConvertErrorCode(FLAG_RACK_OUT_SWITCH_ERR_STEP_MOTOR_RESET,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //读取结果
    errorCode = IPC_StepMotorReflushStateWhileReturn(resetCmd.motorIndex,currentPos);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplRackOutSwitchConvertErrorCode(FLAG_RACK_OUT_SWITCH_ERR_STEP_MOTOR_READ_POS,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    return LH_ERR_NONE;
}

//指定步进电机走位
LH_ERR AppImplRackOutSwitchStepMotorRunSteps(RACK_OUT_SWITCH_SM smIndex,int32_t runSteps,int32_t* currentPos)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //索引判断
    if((uint8_t)(smIndex) >= LENGTH_STEP_MOTOR_RESET_RUN_CONFIG_ARRAY)
    {
        //错误代码转换
        errorCode = AppImplRackOutSwitchConvertErrorCode(FLAG_RACK_OUT_SWITCH_ERR_STEP_MOTOR_INDEX,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //填充参数
    runStepCmd.motorIndex = moduleStepMotorResetRunConfigArray[smIndex].motorIndex;
    runStepCmd.specialSteps = runSteps;
    runStepCmd.selectConfig = SM_CURVE_0;//默认曲线0作为调试曲线
    //开始走步数
    errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplRackOutSwitchConvertErrorCode(FLAG_RACK_OUT_SWITCH_ERR_STEP_MOTOR_RUN_STEP,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //读取结果
    errorCode = IPC_StepMotorReflushStateWhileReturn(runStepCmd.motorIndex,currentPos);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplRackOutSwitchConvertErrorCode(FLAG_RACK_OUT_SWITCH_ERR_STEP_MOTOR_READ_POS,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    return LH_ERR_NONE;
}


//节点复位
LH_ERR AppImplRackOutSwitchModuleReset(APP_STATE_RACK_OUT_SWITCH** rackOutSwitchStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_OUT_SWITCH* rackOutSwitchStatePtr = AppStateReadRackOutSwitchState();
    *rackOutSwitchStatePtrPtr = rackOutSwitchStatePtr;
    //电机复位
    errorCode = AppImplRackOutSwitchMotorReset();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //切换到入口位置
    errorCode = AppImplRackOutSwitchMotorRunToComeIn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器讯息
    AppImplRackOutSwitchReflushSensorSet(&(rackOutSwitchStatePtr->rackOutSwitchSensorSet));
    //状态初始化
    rackOutSwitchStatePtr->rackComeInEventFlag = EVENT_FLAG_NONE;
    rackOutSwitchStatePtr->railSwitchState = RAIL_SWITCH_STATE_IN;
    return errorCode;
}

//节点切换到试管架进入位置
LH_ERR AppImplRackOutSwitchToRackComeInPos(APP_STATE_RACK_OUT_SWITCH** rackOutSwitchStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_OUT_SWITCH* rackOutSwitchStatePtr = AppStateReadRackOutSwitchState();
    *rackOutSwitchStatePtrPtr = rackOutSwitchStatePtr;
    //当前是否在入口位置
    if(rackOutSwitchStatePtr->railSwitchState != RAIL_SWITCH_STATE_IN)
    {
        errorCode = AppImplRackOutSwitchMotorReturnZero();
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    }
    //运行到入口位置
    errorCode = AppImplRackOutSwitchMotorRunToComeIn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器讯息
    AppImplRackOutSwitchReflushSensorSet(&(rackOutSwitchStatePtr->rackOutSwitchSensorSet));
    //状态更新
    rackOutSwitchStatePtr->railSwitchState = RAIL_SWITCH_STATE_IN;
    return errorCode;
}

//节点切换到试管架离开位置
LH_ERR AppImplRackOutSwitchToRackMoveOutPos(APP_STATE_RACK_OUT_SWITCH** rackOutSwitchStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_OUT_SWITCH* rackOutSwitchStatePtr = AppStateReadRackOutSwitchState();
    *rackOutSwitchStatePtrPtr = rackOutSwitchStatePtr;
    //当前是否在入口位置
    if(rackOutSwitchStatePtr->railSwitchState != RAIL_SWITCH_STATE_OUT)
    {
        errorCode = AppImplRackOutSwitchMotorReturnZero();
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    }
    //运行到入口位置
    errorCode = AppImplRackOutSwitchMotorRunToMoveOut();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器讯息
    AppImplRackOutSwitchReflushSensorSet(&(rackOutSwitchStatePtr->rackOutSwitchSensorSet));
    //状态更新
    rackOutSwitchStatePtr->railSwitchState = RAIL_SWITCH_STATE_OUT;
    return errorCode;
}

//节点执行一次试管架换轨
LH_ERR AppImplRackOutSwitchTrigOnce(APP_STATE_RACK_OUT_SWITCH** rackOutSwitchStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_OUT_SWITCH* rackOutSwitchStatePtr = AppStateReadRackOutSwitchState();
    *rackOutSwitchStatePtrPtr = rackOutSwitchStatePtr;
    uint8_t needcaptureSensorMsg = 0;
    if(rackOutSwitchStatePtr->railSwitchState != RAIL_SWITCH_STATE_IN)
    {
        //不在试管入口,不能执行换轨
        errorCode = AppImplRackOutSwitchConvertErrorCode(FLAG_RACK_OUT_SWITCH_ERR_TRIG_STATE_SWITCH,errorCode);
        return errorCode;
    }
    //更新传感器讯息
    AppImplRackOutSwitchReflushSensorSet(&(rackOutSwitchStatePtr->rackOutSwitchSensorSet));
    if(rackOutSwitchStatePtr->rackOutSwitchSensorSet.rackComeInState == SENSOR_STATE_VALID)
    {
        //如果入口有架子,需要执行一次换轨
        needcaptureSensorMsg = 1;
    }
    //切换到出口位置
    errorCode = AppImplRackOutSwitchToRackMoveOutPos(rackOutSwitchStatePtrPtr);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //查看出口脱离传感器状态
    AppImplRackOutSwitchReflushSensorSet(&(rackOutSwitchStatePtr->rackOutSwitchSensorSet));
    if(needcaptureSensorMsg == 1)
    {
        //存在试管架的情况下,运动完成后如果入口还有试管架或者出口没检测到试管架,说明推失败了
        if(rackOutSwitchStatePtr->rackOutSwitchSensorSet.rackComeInState == SENSOR_STATE_VALID)
        {
            errorCode = AppImplRackOutSwitchConvertErrorCode(FLAG_RACK_OUT_SWITCH_ERR_COME_IN_VALID,errorCode);
            return errorCode;
        }
        if(rackOutSwitchStatePtr->rackOutSwitchSensorSet.rackMoveOutState == SENSOR_STATE_INVALID)
        {
            errorCode = AppImplRackOutSwitchConvertErrorCode(FLAG_RACK_OUT_SWITCH_ERR_MOVE_OUT_INVALID,errorCode);
            return errorCode;
        }
    }
    //运行到入口位置
    errorCode = AppImplRackOutSwitchToRackComeInPos(rackOutSwitchStatePtrPtr);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器讯息
    AppImplRackOutSwitchReflushSensorSet(&(rackOutSwitchStatePtr->rackOutSwitchSensorSet));
    if(rackOutSwitchStatePtr->rackOutSwitchSensorSet.rackComeInState == SENSOR_STATE_VALID)
    {
        errorCode = AppImplRackOutSwitchConvertErrorCode(FLAG_RACK_OUT_SWITCH_ERR_MOVE_OUT_RACK_EXIST,errorCode);
        return errorCode;
    }
    //状态更新
    rackOutSwitchStatePtr->railSwitchState = RAIL_SWITCH_STATE_IN;
    //清除事件
    rackOutSwitchStatePtr->rackComeInEventFlag = EVENT_FLAG_NONE;
    return errorCode;
}

//获取节点全部讯息
LH_ERR AppImplRackOutSwitchReadState(APP_STATE_RACK_OUT_SWITCH** rackOutSwitchStatePtrPtr)
{
    APP_STATE_RACK_OUT_SWITCH* rackOutSwitchStatePtr = AppStateReadRackOutSwitchState();
    *rackOutSwitchStatePtrPtr = rackOutSwitchStatePtr;
    //更新传感器讯息
    AppImplRackOutSwitchReflushSensorSet(&(rackOutSwitchStatePtr->rackOutSwitchSensorSet));
    return LH_ERR_NONE;
}

//清除节点全部讯息
LH_ERR AppImplRackOutSwitchClearState(void)
{
    APP_STATE_RACK_OUT_SWITCH* rackOutSwitchStatePtr = AppStateReadRackOutSwitchState();
    rackOutSwitchStatePtr->rackComeInEventFlag = EVENT_FLAG_NONE;
    return LH_ERR_NONE;
}


