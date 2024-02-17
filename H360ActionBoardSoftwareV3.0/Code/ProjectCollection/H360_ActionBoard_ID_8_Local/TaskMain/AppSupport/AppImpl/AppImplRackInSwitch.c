/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-11-08 16:11:46
**LastEditors: DengXiaoJun
**LastEditTime: 2022-01-24 17:33:53
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplRackInSwitch.h"
#include "AppImplParamUtil.h"
#include "AppImplRackWaitOut1.h"

//模块错误代码
typedef enum FLAG_RACK_IN_SWITCH_ERR_CONVERT
{
    FLAG_RACK_IN_SWITCH_ERR_READ_INPUT_INDEX                = 0x01,//读输入引脚索引错误
    FLAG_RACK_IN_SWITCH_ERR_READ_INPUT                      = 0x02,//读输入引脚错误
    FLAG_RACK_IN_SWITCH_ERR_WRITE_OUTPUT_INDEX              = 0x03,//写输出引脚索引错误
    FLAG_RACK_IN_SWITCH_ERR_WRITE_OUTPUT                    = 0x04,//写输出引脚错误
    FLAG_RACK_IN_SWITCH_ERR_STEP_MOTOR_INDEX                = 0x05,//电机索引错误
    FLAG_RACK_IN_SWITCH_ERR_STEP_MOTOR_RESET                = 0x06,//电机复位错误
    FLAG_RACK_IN_SWITCH_ERR_STEP_MOTOR_RUN_STEP             = 0x07,//电机走步数错误
    FLAG_RACK_IN_SWITCH_ERR_STEP_MOTOR_READ_POS             = 0X08,//电机读坐标错误
    FLAG_RACK_IN_SWITCH_ERR_MOTOR_SWITCH_RESET              = 0X09,//换轨电机复位错误
    FLAG_RACK_IN_SWITCH_ERR_MOTOR_SWITCH_RETURN_ZERO        = 0X0A,//换轨电机回零错误
    FLAG_RACK_IN_SWITCH_ERR_MOTOR_SWITCH_RUN_IN             = 0X0B,//换轨电机移动到入口端错误
    FLAG_RACK_IN_SWITCH_ERR_MOTOR_SWITCH_RUN_OUT            = 0X0C,//换轨电机移动到出口端错误
    FLAG_RACK_IN_SWITCH_ERR_RACK_MOVE_OUT_TIME_OUT_VALID    = 0X0D,//等待离开有效超时
    FLAG_RACK_IN_SWITCH_ERR_TRIG_STATE_SWITCH               = 0X0E,//状态不在进样入口,不能换轨
    FLAG_RACK_IN_SWITCH_ERR_TRIG_CONDITION_MOVE_OUT_FULL    = 0X0F,//换轨出口满,不满足换轨条件
}FLAG_RACK_IN_SWITCH_ERR_CONVERT;

//输入引脚配置
static const INPUT_SENSOR_PIN_CONFIG inputSensorPinConfigArray[] = {
    {INPUT_RACK_IN_SWITCH_INPUT_COME_IN         , VAILD_INPUT_RACK_IN_SWITCH_INPUT_COME_IN       },//换轨进入传感器
    {INPUT_RACK_IN_SWITCH_INPUT_MOVE_OUT        , VAILD_INPUT_RACK_IN_SWITCH_INPUT_MOVE_OUT      },//换轨离去传感器
    {INPUT_RACK_IN_SWITCH_INPUT_MOTOR_ORIGIN    , VAILD_INPUT_RACK_IN_SWITCH_INPUT_MOTOR_ORIGIN  },//换轨电机原点
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
    {RACK_IN_SWITCH_SM_MAIN_ID    ,APP_PARAM_MAIN_RACK_IN_SWITCH ,APP_PARAM_SUB_RACK_IN_SWITCH_COME_IN_POS  },//换轨电机
};
//步进电机配置表长度
#define LENGTH_STEP_MOTOR_RESET_RUN_CONFIG_ARRAY        (sizeof(moduleStepMotorResetRunConfigArray)/sizeof(moduleStepMotorResetRunConfigArray[0]))


//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplRackInSwitchConvertErrorCode(FLAG_RACK_IN_SWITCH_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_RACK_IN_SWITCH;
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
LH_ERR AppImplRackInSwitchReadInput(RACK_IN_SWITCH_INPUT inputIndex,SENSOR_STATE* sensorStatePtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(inputIndex) >= LENGTH_INPUT_SENSOR_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplRackInSwitchConvertErrorCode(FLAG_RACK_IN_SWITCH_ERR_READ_INPUT_INDEX,errorCode);
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
LH_ERR AppImplRackInSwitchWriteOutput(RACK_IN_SWITCH_OUTPUT outputIndex,OUT_STATE setState)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(outputIndex) >= LENGTH_OUTPUT_CTRL_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplRackInSwitchConvertErrorCode(FLAG_RACK_IN_SWITCH_ERR_WRITE_OUTPUT_INDEX,errorCode);
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
static void AppImplRackInSwitchReflushSensorSet(RACK_IN_SWITCH_SENSOR_SET* rackInSwitchSensorSetPtr)
{
    if(rackInSwitchSensorSetPtr == NULL)return;
    AppImplRackInSwitchReadInput(RACK_IN_SWITCH_INPUT_COME_IN,&(rackInSwitchSensorSetPtr->rackComeInState));
    AppImplRackInSwitchReadInput(RACK_IN_SWITCH_INPUT_MOVE_OUT,&(rackInSwitchSensorSetPtr->rackMoveOutState));
}

//换轨电机复位
static LH_ERR AppImplRackInSwitchMotorReset(void)
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
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_IN_SWITCH,APP_PARAM_SUB_RACK_IN_SWITCH_COME_IN_POS,&paramUtil);
    //填充参数
    resetCmd.motorIndex = RACK_IN_SWITCH_SM_MAIN_ID;//电机序号
    resetCmd.timeOutSet = 60000;//超时时间
    resetCmd.correctionCoordinate = paramUtil;//复位修正
    //开始复位
    errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplRackInSwitchConvertErrorCode(FLAG_RACK_IN_SWITCH_ERR_MOTOR_SWITCH_RESET,errorCode);
        return errorCode;
    }
    return errorCode;
}

//换轨电机回零
static LH_ERR AppImplRackInSwitchMotorReturnZero(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机回零结构体
    StepMotorReturnZeroCmd returnZeroCmd;
    //出样旋转
    //获取复位修正参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_IN_SWITCH,APP_PARAM_SUB_RACK_IN_SWITCH_COME_IN_POS,&paramUtil);
    //回零结构体初始化
    IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
    //配置回零参数
    returnZeroCmd.motorIndex = RACK_IN_SWITCH_SM_MAIN_ID;
    returnZeroCmd.correctionCoordinate = paramUtil;
    returnZeroCmd.selectConfig = S_CURVE_RACK_IN_SWITCH_SM_MAIN_RETURN_ZERO;
    returnZeroCmd.timeOutSet = 60000;
    errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //回零失败
        errorCode = AppImplRackInSwitchConvertErrorCode(FLAG_RACK_IN_SWITCH_ERR_MOTOR_SWITCH_RETURN_ZERO,errorCode);
        return errorCode;
    }
    return errorCode;
}

//换轨电机移动到样本进入位置
static LH_ERR AppImplRackInSwitchMotorRunToComeIn(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机走坐标结构体
    StepMotorRun2CoordationCmd runToPosCmd;
    //初始化走坐标参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_IN_SWITCH,APP_PARAM_SUB_RACK_IN_SWITCH_COME_IN_POS,&paramUtil);
    //初始化走目标坐标结构
    IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
    //填充走坐标结构体
    runToPosCmd.motorIndex = RACK_IN_SWITCH_SM_MAIN_ID;//电机序号
    runToPosCmd.targetCoordinate = paramUtil;//目标坐标
    runToPosCmd.selectConfig = S_CURVE_RACK_IN_SWITCH_SM_MAIN_SWITCH;//速度曲线
    runToPosCmd.timeOutSet = 60000;
    runToPosCmd.utilStop1Enable = 0;
    //走坐标
    errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
    if(errorCode != LH_ERR_NONE)
    {
        errorCode = AppImplRackInSwitchConvertErrorCode(FLAG_RACK_IN_SWITCH_ERR_MOTOR_SWITCH_RUN_IN,errorCode);
        return errorCode;
    }
    return errorCode;
}

//换轨电机移动到样本离开位置
static LH_ERR AppImplRackInSwitchMotorRunToMoveOut(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机走坐标结构体
    StepMotorRun2CoordationCmd runToPosCmd;
    //初始化走目标坐标结构
    IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
    //初始化走坐标参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_IN_SWITCH,APP_PARAM_SUB_RACK_IN_SWITCH_MOVE_OUT_POS,&paramUtil);
    //填充走坐标结构体
    runToPosCmd.motorIndex = RACK_IN_SWITCH_SM_MAIN_ID;//电机序号
    runToPosCmd.targetCoordinate = paramUtil;//目标坐标
    runToPosCmd.selectConfig = S_CURVE_RACK_IN_SWITCH_SM_MAIN_SWITCH;//速度曲线
    runToPosCmd.timeOutSet = 60000;
    runToPosCmd.utilStop1Enable = 0;
    //走坐标
    errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
    if(errorCode != LH_ERR_NONE)
    {
        errorCode = AppImplRackInSwitchConvertErrorCode(FLAG_RACK_IN_SWITCH_ERR_MOTOR_SWITCH_RUN_OUT,errorCode);
        return errorCode;
    }
    return errorCode;
}

//换轨电机开始移动到样本离开位置
static LH_ERR AppImplRackInSwitchMotorStartRunToMoveOut(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机走坐标结构体
    StepMotorRun2CoordationCmd runToPosCmd;
    //初始化走目标坐标结构
    IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
    //初始化走坐标参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_IN_SWITCH,APP_PARAM_SUB_RACK_IN_SWITCH_MOVE_OUT_POS,&paramUtil);
    //填充走坐标结构体
    runToPosCmd.motorIndex = RACK_IN_SWITCH_SM_MAIN_ID;//电机序号
    runToPosCmd.targetCoordinate = paramUtil;//目标坐标
    runToPosCmd.selectConfig = S_CURVE_RACK_IN_SWITCH_SM_MAIN_SWITCH;//速度曲线
    runToPosCmd.timeOutSet = 60000;
    runToPosCmd.utilStop1Enable = 0;
    //走坐标
    errorCode = IPC_StepMotorRun2CoordinateWhileAck(&runToPosCmd);
    if(errorCode != LH_ERR_NONE)
    {
        errorCode = AppImplRackInSwitchConvertErrorCode(FLAG_RACK_IN_SWITCH_ERR_MOTOR_SWITCH_RUN_OUT,errorCode);
        return errorCode;
    }
    return errorCode;
}

//等待换轨电机移动完成
static LH_ERR AppImplRackInSwitchMotorWaitRunToMoveOut(uint8_t* completeFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    STATE_IPC stateIpc = STATE_IPC_WAIT_RETURN;
    stateIpc = IPC_StepMotorGetLastCommandStatusAndResult(RACK_IN_SWITCH_SM_MAIN_ID,&errorCode);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    if(STATE_IPC_CMD_COMPLETE != stateIpc)
    {
        //未执行完成
        *completeFlag = 0;
    }
    else
    {
        //已经执行完成
        *completeFlag = 1;
    }
    return errorCode;
}

//等待换轨位置试管离开,参数是需不需要等待信号有效
static LH_ERR AppImplRackInSwitchWaitRackMoveOut(uint8_t needWaitValid)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint32_t timeMsCount = 0;
    APP_STATE_RACK_IN_SWITCH* rackInSwitchStatePtr = AppStateReadRackInSwitchState();
    RACK_IN_SWITCH_SENSOR_SET* rackInSwitchSensorSetPtr = &(rackInSwitchStatePtr->rackInSwitchSensorSet);
    uint32_t sensorMoveOutCheckCount = 0;
    if(needWaitValid == 1)
    {
        //等待连续多次检测到出口讯息
        do
        {
            AppImplRackInSwitchReflushSensorSet(rackInSwitchSensorSetPtr);
            if(rackInSwitchSensorSetPtr->rackMoveOutState == SENSOR_STATE_VALID)
            {
                sensorMoveOutCheckCount++;
            }
            else
            {
                sensorMoveOutCheckCount = 0;
            }
            //检测次数到位
            if(sensorMoveOutCheckCount >= COUNT_RACK_IN_SWITCH_MOVE_OUT_WAIT)
            {
                break;
            }
            timeMsCount += TIME_MS_WAIT_EVENT_INTERVAL;
            CoreDelayMs(TIME_MS_WAIT_EVENT_INTERVAL);
        }while(timeMsCount <= TIME_OUT_MS_RACK_IN_SWITCH_MOVE_OUT_TRIG);
        //检测超时
        if(timeMsCount > TIME_OUT_MS_RACK_IN_SWITCH_MOVE_OUT_TRIG)
        {
            errorCode = AppImplRackInSwitchConvertErrorCode(FLAG_RACK_IN_SWITCH_ERR_RACK_MOVE_OUT_TIME_OUT_VALID,errorCode);
            return errorCode;
        }
    }
    //延时等待稳定离开
    CoreDelayMs(TIME_OUT_MS_RACK_IN_SWITCH_MOVE_OUT_STABLE);
    return errorCode;
}

//进样换轨触发一次的条件
static LH_ERR AppImplRackInSwitchCheckTrigOnceCondition(uint8_t rackExistFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    SENSOR_STATE rackInSwitchMoveOutSensorState = SENSOR_STATE_VALID;
    //不存在试管架,直接返回
    if(rackExistFlag == SENSOR_STATE_INVALID)
    {
        return LH_ERR_NONE;
    }
    //检查出口状态
    AppImplRackInSwitchReadInput(RACK_IN_SWITCH_INPUT_MOVE_OUT,&rackInSwitchMoveOutSensorState);
    if(rackInSwitchMoveOutSensorState == SENSOR_STATE_VALID)
    {
        //出口满,报错
        errorCode = AppImplRackInSwitchConvertErrorCode(FLAG_RACK_IN_SWITCH_ERR_TRIG_CONDITION_MOVE_OUT_FULL,errorCode);
        return errorCode;
    }
    return errorCode;
}

//上一次的信号状态
static RACK_IN_SWITCH_SENSOR_SET lastRackInSwitchSensorSet = {SENSOR_STATE_VALID};
//初始化功能状态机
void AppImplRackInSwitchFSM_Init(void)
{
    APP_STATE_RACK_IN_SWITCH* rackInSwitchStatePtr = AppStateReadRackInSwitchState();
    //状态初始化
    rackInSwitchStatePtr->rackComeInEventFlag = EVENT_FLAG_NONE;
    //初始化的时候,节点处于位置状态
    rackInSwitchStatePtr->railSwitchState = RAIL_SWITCH_STATE_UNKNOW;
    //读取并记录传感器状态
    AppImplRackInSwitchReflushSensorSet(&(rackInSwitchStatePtr->rackInSwitchSensorSet));
    //数据拷贝
    UserMemCopy(&lastRackInSwitchSensorSet,&(rackInSwitchStatePtr->rackInSwitchSensorSet),sizeof(RACK_IN_SWITCH_SENSOR_SET)/sizeof(uint8_t));
}

//运行功能状态机
void AppImplRackInSwitchFSM_Run(void)
{
    //如果节点处于未知状态,状态机不运行
    APP_STATE_RACK_IN_SWITCH* rackInSwitchStatePtr = AppStateReadRackInSwitchState();
    if(rackInSwitchStatePtr->railSwitchState == RAIL_SWITCH_STATE_UNKNOW)
    {
        return;
    }
    //如果入口事件处于已经触发状态,状态机不运行
    if(rackInSwitchStatePtr->rackComeInEventFlag == EVENT_FLAG_TRIG)
    {
        return;
    }
    //检查标志变化,首先读取当前状态
    AppImplRackInSwitchReflushSensorSet(&(rackInSwitchStatePtr->rackInSwitchSensorSet));
    if(rackInSwitchStatePtr->rackInSwitchSensorSet.rackComeInState == SENSOR_STATE_VALID)
    {
        //本次传感器状态为有效且上次传感器状态为无效的情况下,才能触发事件
        if(lastRackInSwitchSensorSet.rackComeInState == SENSOR_STATE_INVALID)
        {
            rackInSwitchStatePtr->rackComeInEventFlag = EVENT_FLAG_TRIG;
        }
    }
    //更新检测讯息,数据拷贝
    UserMemCopy(&lastRackInSwitchSensorSet,&(rackInSwitchStatePtr->rackInSwitchSensorSet),sizeof(RACK_IN_SWITCH_SENSOR_SET)/sizeof(uint8_t));
}

//指定步进电机复位
LH_ERR AppImplRackInSwitchStepMotorReset(RACK_IN_SWITCH_SM smIndex,int32_t* currentPos)
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
        errorCode = AppImplRackInSwitchConvertErrorCode(FLAG_RACK_IN_SWITCH_ERR_STEP_MOTOR_INDEX,errorCode);
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
        errorCode = AppImplRackInSwitchConvertErrorCode(FLAG_RACK_IN_SWITCH_ERR_STEP_MOTOR_RESET,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //读取结果
    errorCode = IPC_StepMotorReflushStateWhileReturn(resetCmd.motorIndex,currentPos);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplRackInSwitchConvertErrorCode(FLAG_RACK_IN_SWITCH_ERR_STEP_MOTOR_READ_POS,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    return LH_ERR_NONE;
}

//指定步进电机走位
LH_ERR AppImplRackInSwitchStepMotorRunSteps(RACK_IN_SWITCH_SM smIndex,int32_t runSteps,int32_t* currentPos)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //索引判断
    if((uint8_t)(smIndex) >= LENGTH_STEP_MOTOR_RESET_RUN_CONFIG_ARRAY)
    {
        //错误代码转换
        errorCode = AppImplRackInSwitchConvertErrorCode(FLAG_RACK_IN_SWITCH_ERR_STEP_MOTOR_INDEX,errorCode);
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
        errorCode = AppImplRackInSwitchConvertErrorCode(FLAG_RACK_IN_SWITCH_ERR_STEP_MOTOR_RUN_STEP,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //读取结果
    errorCode = IPC_StepMotorReflushStateWhileReturn(runStepCmd.motorIndex,currentPos);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplRackInSwitchConvertErrorCode(FLAG_RACK_IN_SWITCH_ERR_STEP_MOTOR_READ_POS,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    return LH_ERR_NONE;
}

//节点复位
LH_ERR AppImplRackInSwitchModuleReset(APP_STATE_RACK_IN_SWITCH** rackInSwitchStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_IN_SWITCH* rackInSwitchStatePtr = AppStateReadRackInSwitchState();
    *rackInSwitchStatePtrPtr = rackInSwitchStatePtr;
    //电机复位
    errorCode = AppImplRackInSwitchMotorReset();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器讯息
    AppImplRackInSwitchReflushSensorSet(&(rackInSwitchStatePtr->rackInSwitchSensorSet));
    //状态初始化
    rackInSwitchStatePtr->rackComeInEventFlag = EVENT_FLAG_NONE;
    rackInSwitchStatePtr->railSwitchState = RAIL_SWITCH_STATE_IN;
    return errorCode;
}

//节点切换到试管架进入位置
LH_ERR AppImplRackInSwitchToRackComeInPos(APP_STATE_RACK_IN_SWITCH** rackInSwitchStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_IN_SWITCH* rackInSwitchStatePtr = AppStateReadRackInSwitchState();
    *rackInSwitchStatePtrPtr = rackInSwitchStatePtr;
    //当前是否在入口位置
    if(rackInSwitchStatePtr->railSwitchState != RAIL_SWITCH_STATE_IN)
    {
        errorCode = AppImplRackInSwitchMotorReturnZero();
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    }
    //运行到入口位置
    errorCode = AppImplRackInSwitchMotorRunToComeIn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器讯息
    AppImplRackInSwitchReflushSensorSet(&(rackInSwitchStatePtr->rackInSwitchSensorSet));
    //状态更新
    rackInSwitchStatePtr->railSwitchState = RAIL_SWITCH_STATE_IN;
    return errorCode;
}

//节点切换到试管架离开位置
LH_ERR AppImplRackInSwitchToRackMoveOutPos(APP_STATE_RACK_IN_SWITCH** rackInSwitchStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_IN_SWITCH* rackInSwitchStatePtr = AppStateReadRackInSwitchState();
    *rackInSwitchStatePtrPtr = rackInSwitchStatePtr;
    //当前是否在入口位置
    if(rackInSwitchStatePtr->railSwitchState != RAIL_SWITCH_STATE_OUT)
    {
        errorCode = AppImplRackInSwitchMotorReturnZero();
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    }
    //运行到离开位置
    errorCode = AppImplRackInSwitchMotorRunToMoveOut();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器讯息
    AppImplRackInSwitchReflushSensorSet(&(rackInSwitchStatePtr->rackInSwitchSensorSet));
    //状态更新
    rackInSwitchStatePtr->railSwitchState = RAIL_SWITCH_STATE_OUT;
    return errorCode;
}

//节点执行一次试管架换轨
LH_ERR AppImplRackInSwitchTrigOnce(APP_STATE_RACK_IN_SWITCH** rackInSwitchStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_IN_SWITCH* rackInSwitchStatePtr = AppStateReadRackInSwitchState();
    *rackInSwitchStatePtrPtr = rackInSwitchStatePtr;
    if(rackInSwitchStatePtr->railSwitchState != RAIL_SWITCH_STATE_IN)
    {
        //不在试管入口,不能执行换轨
        errorCode = AppImplRackInSwitchConvertErrorCode(FLAG_RACK_IN_SWITCH_ERR_TRIG_STATE_SWITCH,errorCode);
        return errorCode;
    }
    //是否需要采集传感器讯息
    uint8_t needcaptureSensorMsg = 0;
    SENSOR_STATE rackExistSensorState = SENSOR_STATE_VALID;
    //更新传感器讯息
    AppImplRackInSwitchReflushSensorSet(&(rackInSwitchStatePtr->rackInSwitchSensorSet));
    if(rackInSwitchStatePtr->rackInSwitchSensorSet.rackComeInState == SENSOR_STATE_VALID)
    {
        //如果入口有架子,需要执行一次换轨
        needcaptureSensorMsg = 1;
    }
    //试管架存在信息
    rackExistSensorState = rackInSwitchStatePtr->rackInSwitchSensorSet.rackComeInState;
    //检查释放条件
    errorCode = AppImplRackInSwitchCheckTrigOnceCondition(rackExistSensorState);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //开始切换到出口位置
    errorCode = AppImplRackInSwitchMotorStartRunToMoveOut();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //是否需要等待传感器有效
    uint8_t waitOutNeedWaitValid = 1;
    //循环等待运行完成
    uint8_t completeFlag = 0;
    do
    {
        errorCode = AppImplRackInSwitchMotorWaitRunToMoveOut(&completeFlag);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
        //延迟等待
        CoreDelayMs(TIME_MS_WAIT_EVENT_INTERVAL);
        //更新传感器讯息
        AppImplRackInSwitchReflushSensorSet(&(rackInSwitchStatePtr->rackInSwitchSensorSet));
        //如果出口有效,不需要下一阶段等待有效了
        if(rackInSwitchStatePtr->rackInSwitchSensorSet.rackMoveOutState == SENSOR_STATE_VALID)
        {
            //循环等待的过程中,采集传感器有效讯息,如果采集到了,就设置不再需要采集传感器讯息
            //没采集到,就还要采集传感器讯息直到有效
            waitOutNeedWaitValid = 0;
        }
    }while(completeFlag == 0);
    //需要采集传感器讯息
    if(needcaptureSensorMsg == 1)
    {
        //需要采集离开讯息
        errorCode = AppImplRackInSwitchWaitRackMoveOut(waitOutNeedWaitValid);
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    }
    //换轨完成.再切换回到入口
    //运行到入口位置
    errorCode = AppImplRackInSwitchMotorRunToComeIn();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器讯息
    AppImplRackInSwitchReflushSensorSet(&(rackInSwitchStatePtr->rackInSwitchSensorSet));
    //状态更新
    rackInSwitchStatePtr->railSwitchState = RAIL_SWITCH_STATE_IN;
    //清除事件
    rackInSwitchStatePtr->rackComeInEventFlag = EVENT_FLAG_NONE;
    return errorCode;
}

//获取节点全部讯息
LH_ERR AppImplRackInSwitchReadState(APP_STATE_RACK_IN_SWITCH** rackInSwitchStatePtrPtr)
{
    APP_STATE_RACK_IN_SWITCH* rackInSwitchStatePtr = AppStateReadRackInSwitchState();
    *rackInSwitchStatePtrPtr = rackInSwitchStatePtr;
    //更新传感器讯息
    AppImplRackInSwitchReflushSensorSet(&(rackInSwitchStatePtr->rackInSwitchSensorSet));
    return LH_ERR_NONE;
}

//清除节点全部讯息
LH_ERR AppImplRackInSwitchClearState(void)
{
    APP_STATE_RACK_IN_SWITCH* rackInSwitchStatePtr = AppStateReadRackInSwitchState();
    rackInSwitchStatePtr->rackComeInEventFlag = EVENT_FLAG_NONE;
    return LH_ERR_NONE;
}


