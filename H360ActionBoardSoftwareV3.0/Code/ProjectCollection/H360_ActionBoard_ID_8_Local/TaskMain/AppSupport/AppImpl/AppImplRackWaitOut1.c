/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-11-08 16:12:02
**LastEditors: DengXiaoJun
**LastEditTime: 2021-12-29 17:42:28
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplRackWaitOut1.h"
#include "AppImplParamUtil.h"
#include "AppImplRackWaitOut2.h"

//模块错误号
typedef enum FLAG_RACK_WAIT_OUT1_ERR_CONVERT
{
    FLAG_RACK_WAIT_OUT1_ERR_READ_INPUT_INDEX            = 0x01,//读输入引脚索引错误
    FLAG_RACK_WAIT_OUT1_ERR_READ_INPUT                  = 0x02,//读输入引脚错误
    FLAG_RACK_WAIT_OUT1_ERR_WRITE_OUTPUT_INDEX          = 0x03,//写输出引脚索引错误
    FLAG_RACK_WAIT_OUT1_ERR_WRITE_OUTPUT                = 0x04,//写输出引脚错误
    FLAG_RACK_WAIT_OUT1_ERR_STEP_MOTOR_INDEX            = 0x05,//电机索引错误
    FLAG_RACK_WAIT_OUT1_ERR_STEP_MOTOR_RESET            = 0x06,//电机复位错误
    FLAG_RACK_WAIT_OUT1_ERR_STEP_MOTOR_RUN_STEP         = 0x07,//电机走步数错误
    FLAG_RACK_WAIT_OUT1_ERR_STEP_MOTOR_READ_POS         = 0X08,//电机读坐标错误
    FLAG_RACK_WAIT_OUT1_ERR_ROTATE_RESET                = 0X09,//电机复位失败
    FLAG_RACK_WAIT_OUT1_ERR_ROTATE_BLOCK                = 0X0A,//电机卡位失败
    FLAG_RACK_WAIT_OUT1_ERR_ROTATE_RELEASE              = 0X0B,//电机释放失败
    FLAG_RACK_WAIT_OUT1_ERR_ROTATE_RETURN_ZERO          = 0X0C,//旋转电机回零失败
    FLAG_RACK_WAIT_OUT1_ERR_RELEASE_TIME_OUT            = 0X0D,//试管架释放时间超时
    FLAG_RACK_WAIT_OUT1_ERR_RELEASE_RACK_NEXT_NODE_FULL = 0X0E,//目标节点满
    FLAG_RACK_WAIT_OUT1_ERR_RELEASE_RACK_ARRIVE_NEXT_NODE_TIME_OUT = 0X0F,//运行到目标节点超时
}FLAG_RACK_WAIT_OUT1_ERR_CONVERT;

//输入引脚配置
static const INPUT_SENSOR_PIN_CONFIG inputSensorPinConfigArray[] = {
    {INPUT_RACK_WAIT_OUT1_INPUT_EXIST           , VAILD_INPUT_RACK_WAIT_OUT1_INPUT_EXIST        },//检测试管架是否存在
    {INPUT_RACK_WAIT_OUT1_INPUT_MOTOR_ORIGIN    , VAILD_INPUT_RACK_WAIT_OUT1_INPUT_MOTOR_ORIGIN },//旋转电机原点
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
    {RACK_WAIT_OUT1_SM_MAIN_ID    ,APP_PARAM_MAIN_RACK_WAIT_OUT1 ,APP_PARAM_SUB_RACK_WAIT_OUT1_IDLE_POS  },//出样等待位1阻挡电机
};
//步进电机配置表长度
#define LENGTH_STEP_MOTOR_RESET_RUN_CONFIG_ARRAY        (sizeof(moduleStepMotorResetRunConfigArray)/sizeof(moduleStepMotorResetRunConfigArray[0]))

//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplRackWaitOut1ConvertErrorCode(FLAG_RACK_WAIT_OUT1_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_RACK_WAIT_OUT1;
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
LH_ERR AppImplRackWaitOut1ReadInput(RACK_WAIT_OUT1_INPUT inputIndex,SENSOR_STATE* sensorStatePtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(inputIndex) >= LENGTH_INPUT_SENSOR_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplRackWaitOut1ConvertErrorCode(FLAG_RACK_WAIT_OUT1_ERR_READ_INPUT_INDEX,errorCode);
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

//设置指定输出状态
LH_ERR AppImplRackWaitOut1WriteOutput(RACK_WAIT_OUT1_OUTPUT outputIndex,OUT_STATE setState)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(outputIndex) >= LENGTH_OUTPUT_CTRL_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplRackWaitOut1ConvertErrorCode(FLAG_RACK_WAIT_OUT1_ERR_WRITE_OUTPUT_INDEX,errorCode);
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

//更新系统传感器状态
static void AppImplRackWaitOut1ReflushAllSensor(WAIT_OUT1_SENSOR_SET* rackWaitOut1SensorSetPtr)
{
    AppImplRackWaitOut1ReadInput(RACK_WAIT_OUT1_INPUT_EXIST,&(rackWaitOut1SensorSetPtr->rackExistSensor));
}

//出样等待1步进电机复位
static LH_ERR AppImplRackWaitOut1MotorMainReset(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //复位指令结构体
    StepMotorResetCmd resetCmd;
    //初始化复位参数
    IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
    //获取复位修正参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_WAIT_OUT1,APP_PARAM_SUB_RACK_WAIT_OUT1_IDLE_POS,&paramUtil);
    //填充参数
    resetCmd.motorIndex = RACK_WAIT_OUT1_SM_MAIN_ID;//电机序号
    resetCmd.timeOutSet = 60000;//超时时间
    resetCmd.correctionCoordinate = paramUtil;//复位修正
    //开始复位
    errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplRackWaitOut1ConvertErrorCode(FLAG_RACK_WAIT_OUT1_ERR_ROTATE_RESET,errorCode);
        return errorCode;
    }
    return errorCode;
}

//出样等待1步进电机回零
static LH_ERR AppImplRackWaitOut1MotorMainReturnZero(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机回零结构体
    StepMotorReturnZeroCmd returnZeroCmd;
    //出样旋转
    //获取复位修正参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_WAIT_OUT1,APP_PARAM_SUB_RACK_WAIT_OUT1_IDLE_POS,&paramUtil);
    //回零结构体初始化
    IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
    //配置回零参数
    returnZeroCmd.motorIndex = RACK_WAIT_OUT1_SM_MAIN_ID;
    returnZeroCmd.correctionCoordinate = paramUtil;
    returnZeroCmd.selectConfig = S_CURVE_RACK_WAIT_OUT1_SM_MAIN_RETURN_ZERO;
    returnZeroCmd.timeOutSet = 60000;
    errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //回零失败
        errorCode = AppImplRackWaitOut1ConvertErrorCode(FLAG_RACK_WAIT_OUT1_ERR_ROTATE_RETURN_ZERO,errorCode);
        return errorCode;
    }
    return errorCode;
}

//出样等待1步进电机切换到限位位置
static LH_ERR AppImplRackWaitOut1MotorMainRunToBrake(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机走坐标结构体
    StepMotorRun2CoordationCmd runToPosCmd;
    //初始化走坐标参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_WAIT_OUT1,APP_PARAM_SUB_RACK_WAIT_OUT1_ACTION_POS,&paramUtil);
    //初始化走目标坐标结构
    IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
    //填充走坐标结构体
    runToPosCmd.motorIndex = RACK_WAIT_OUT1_SM_MAIN_ID;//电机序号
    runToPosCmd.targetCoordinate = paramUtil;//目标坐标
    runToPosCmd.selectConfig = S_CURVE_RACK_WAIT_OUT1_SM_MAIN_BRAKE;//速度曲线
    runToPosCmd.timeOutSet = 60000;
    runToPosCmd.utilStop1Enable = 0;
    //走坐标
    errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
    if(errorCode != LH_ERR_NONE)
    {
        errorCode = AppImplRackWaitOut1ConvertErrorCode(FLAG_RACK_WAIT_OUT1_ERR_ROTATE_BLOCK,errorCode);
        return errorCode;
    }
    return errorCode;
}

//出样等待1步进电机切换到释放位置
static LH_ERR AppImplRackWaitOut1MotorMainRunToRelease(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机走坐标结构体
    StepMotorRun2CoordationCmd runToPosCmd;
    //初始化走坐标参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_WAIT_OUT1,APP_PARAM_SUB_RACK_WAIT_OUT1_IDLE_POS,&paramUtil);
    //初始化走目标坐标结构
    IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
    //填充走坐标结构体
    runToPosCmd.motorIndex = RACK_WAIT_OUT1_SM_MAIN_ID;//电机序号
    runToPosCmd.targetCoordinate = paramUtil;//目标坐标
    runToPosCmd.selectConfig = S_CURVE_RACK_WAIT_OUT1_SM_MAIN_RELEASE;//速度曲线
    runToPosCmd.timeOutSet = 60000;
    runToPosCmd.utilStop1Enable = 0;
    //走坐标
    errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
    if(errorCode != LH_ERR_NONE)
    {
        errorCode = AppImplRackWaitOut1ConvertErrorCode(FLAG_RACK_WAIT_OUT1_ERR_ROTATE_RELEASE,errorCode);
        return errorCode;
    }
    return errorCode;
}

//出样等待1传感器集合讯息
static WAIT_OUT1_SENSOR_SET lastRackWaitOut1SensorSet = {SENSOR_STATE_VALID};
//初始化功能状态机
void AppImplRackWaitOut1FSM_Init(void)
{
    APP_STATE_RACK_WAIT_OUT1* rackWaitOut1StatePtr = AppStateReadRackWaitOut1State();
    //状态初始化
    rackWaitOut1StatePtr->rackComeInEventFlag = EVENT_FLAG_NONE;
    //初始化的时候,节点处于未知状态
    rackWaitOut1StatePtr->nodeState = RAIL_NODE_UNKNOW;
    //读取并记录传感器状态
    AppImplRackWaitOut1ReflushAllSensor(&(rackWaitOut1StatePtr->rackWaitOut1SensorSet));
    //数据拷贝
    UserMemCopy(&lastRackWaitOut1SensorSet,&(rackWaitOut1StatePtr->rackWaitOut1SensorSet),sizeof(WAIT_OUT1_SENSOR_SET)/sizeof(uint8_t));
}

//运行功能状态机
void AppImplRackWaitOut1FSM_Run(void)
{
    //如果节点处于未知状态,状态机不运行
    APP_STATE_RACK_WAIT_OUT1* rackWaitOut1StatePtr = AppStateReadRackWaitOut1State();
    if(rackWaitOut1StatePtr->nodeState == RAIL_NODE_UNKNOW)
    {
        return;
    }
    //如果入口事件处于已经触发状态,状态机不运行
    if(rackWaitOut1StatePtr->rackComeInEventFlag == EVENT_FLAG_TRIG)
    {
        return;
    }
    //检查标志变化,首先读取当前状态
    AppImplRackWaitOut1ReflushAllSensor(&(rackWaitOut1StatePtr->rackWaitOut1SensorSet));
    if(rackWaitOut1StatePtr->rackWaitOut1SensorSet.rackExistSensor == SENSOR_STATE_VALID)
    {
        //本次传感器状态为有效且上次传感器状态为无效的情况下,才能触发事件
        if(lastRackWaitOut1SensorSet.rackExistSensor == SENSOR_STATE_INVALID)
        {
            rackWaitOut1StatePtr->rackComeInEventFlag = EVENT_FLAG_TRIG;
        }
    }
    //更新检测讯息,数据拷贝
    UserMemCopy(&lastRackWaitOut1SensorSet,&(rackWaitOut1StatePtr->rackWaitOut1SensorSet),sizeof(WAIT_OUT1_SENSOR_SET)/sizeof(uint8_t));
}

//指定步进电机复位
LH_ERR AppImplRackWaitOut1StepMotorReset(RACK_WAIT_OUT1_SM smIndex,int32_t* currentPos)
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
        errorCode = AppImplRackWaitOut1ConvertErrorCode(FLAG_RACK_WAIT_OUT1_ERR_STEP_MOTOR_INDEX,errorCode);
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
        errorCode = AppImplRackWaitOut1ConvertErrorCode(FLAG_RACK_WAIT_OUT1_ERR_STEP_MOTOR_RESET,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //读取结果
    errorCode = IPC_StepMotorReflushStateWhileReturn(resetCmd.motorIndex,currentPos);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplRackWaitOut1ConvertErrorCode(FLAG_RACK_WAIT_OUT1_ERR_STEP_MOTOR_READ_POS,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    return LH_ERR_NONE;
}

//指定步进电机走位
LH_ERR AppImplRackWaitOut1StepMotorRunSteps(RACK_WAIT_OUT1_SM smIndex,int32_t runSteps,int32_t* currentPos)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //索引判断
    if((uint8_t)(smIndex) >= LENGTH_STEP_MOTOR_RESET_RUN_CONFIG_ARRAY)
    {
        //错误代码转换
        errorCode = AppImplRackWaitOut1ConvertErrorCode(FLAG_RACK_WAIT_OUT1_ERR_STEP_MOTOR_INDEX,errorCode);
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
        errorCode = AppImplRackWaitOut1ConvertErrorCode(FLAG_RACK_WAIT_OUT1_ERR_STEP_MOTOR_RUN_STEP,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //读取结果
    errorCode = IPC_StepMotorReflushStateWhileReturn(runStepCmd.motorIndex,currentPos);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplRackWaitOut1ConvertErrorCode(FLAG_RACK_WAIT_OUT1_ERR_STEP_MOTOR_READ_POS,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    return LH_ERR_NONE;
}

//模块节点复位
LH_ERR AppImplRackWaitOut1ModuleReset(APP_STATE_RACK_WAIT_OUT1** rackWaitOut1StatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_WAIT_OUT1* rackWaitOut1StatePtr = AppStateReadRackWaitOut1State();
    *rackWaitOut1StatePtrPtr = rackWaitOut1StatePtr;
    WAIT_OUT1_SENSOR_SET* rackWaitOut1SensorSetPtr = &(rackWaitOut1StatePtr->rackWaitOut1SensorSet);
    //首先电机复位
    errorCode = AppImplRackWaitOut1MotorMainReset();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //然后切换到释放位置
    errorCode = AppImplRackWaitOut1MotorMainRunToRelease();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器讯息
    AppImplRackWaitOut1ReflushAllSensor(rackWaitOut1SensorSetPtr);
    //设置状态和清空事件
    rackWaitOut1StatePtr->nodeState = RAIL_NODE_RELEASE;
    rackWaitOut1StatePtr->rackComeInEventFlag = EVENT_FLAG_NONE;
    //回传结果
    return errorCode;
}

//切换到卡住状态
LH_ERR AppImplRackWaitOut1SwitchStateBrake(APP_STATE_RACK_WAIT_OUT1** rackWaitOut1StatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_WAIT_OUT1* rackWaitOut1StatePtr = AppStateReadRackWaitOut1State();
    *rackWaitOut1StatePtrPtr = rackWaitOut1StatePtr;
    //如果当前状态不处于卡住状态
    if(rackWaitOut1StatePtr->nodeState != RAIL_NODE_BLOCK)
    {
        //电机回零
        errorCode = AppImplRackWaitOut1MotorMainReturnZero();
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    }
    //电机运行到卡住位置
    errorCode = AppImplRackWaitOut1MotorMainRunToBrake();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器状态
    AppImplRackWaitOut1ReflushAllSensor(&(rackWaitOut1StatePtr->rackWaitOut1SensorSet));
    //更新节点状态
    rackWaitOut1StatePtr->nodeState = RAIL_NODE_BLOCK;
    return errorCode;
}

//切换到释放状态
LH_ERR AppImplRackWaitOut1SwitchStateRelease(APP_STATE_RACK_WAIT_OUT1** rackWaitOut1StatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_WAIT_OUT1* rackWaitOut1StatePtr = AppStateReadRackWaitOut1State();
    *rackWaitOut1StatePtrPtr = rackWaitOut1StatePtr;
    //如果当前状态不处于释放状态
    if(rackWaitOut1StatePtr->nodeState != RAIL_NODE_RELEASE)
    {
        //电机回零
        errorCode = AppImplRackWaitOut1MotorMainReturnZero();
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    }
    //电机运行到释放位置
    errorCode = AppImplRackWaitOut1MotorMainRunToRelease();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器状态
    AppImplRackWaitOut1ReflushAllSensor(&(rackWaitOut1StatePtr->rackWaitOut1SensorSet));
    //更新节点状态
    rackWaitOut1StatePtr->nodeState = RAIL_NODE_RELEASE;
    return errorCode;
}

//释放条件检查
static LH_ERR AppImplRackWaitOut1CheckReleaseRackCondition(SENSOR_STATE rackExistFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    if(rackExistFlag == SENSOR_STATE_INVALID)
    {
        return LH_ERR_NONE;
    }
    //读取目标节点讯息
    SENSOR_STATE nextNodeFullSensorState = SENSOR_STATE_VALID;
    AppImplRackWaitOut2ReadInput(RACK_WAIT_OUT2_INPUT_EXIST,&nextNodeFullSensorState);
    if(nextNodeFullSensorState == SENSOR_STATE_VALID)
    {
        //目标节点满
        errorCode = AppImplRackWaitOut1ConvertErrorCode(FLAG_RACK_WAIT_OUT1_ERR_RELEASE_RACK_NEXT_NODE_FULL,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    return LH_ERR_NONE;
}

//等待试管架运行到目标节点
static LH_ERR AppImplRackWaitOut1WaitArriveNextNode(uint8_t rackExistFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //没有架子的情况下,直接返回
    if(rackExistFlag == SENSOR_STATE_INVALID)
    {
        return LH_ERR_NONE;
    }
    uint32_t timeMsCount = 0;
    uint16_t waitSensorValidCount = 0;
    SENSOR_STATE nextNodeSensorState = SENSOR_STATE_VALID;
    do
    {
        //延时
        timeMsCount += TIME_MS_WAIT_EVENT_INTERVAL;
        CoreDelayMs(TIME_MS_WAIT_EVENT_INTERVAL);
        //更新传感器数据
        AppImplRackWaitOut2ReadInput(RACK_WAIT_OUT2_INPUT_EXIST,&nextNodeSensorState);
        //累计传感器有效的次数
        if(nextNodeSensorState == SENSOR_STATE_VALID)
        {
            waitSensorValidCount++;
        }
        else
        {
            waitSensorValidCount = 0;
        }
        if(waitSensorValidCount >= (TIME_STABLE_MS_RACK_WAIT_OUT1_ARRIVE_NEXT_NODE/TIME_MS_WAIT_EVENT_INTERVAL))
        {
            //多次检测到传感器无效,可以离开了
            return LH_ERR_NONE;
        }
    } while (timeMsCount <= TIME_OUT_MS_RACK_WAIT_OUT1_ARRIVE_NEXT_NODE);
    //结果检查
    if(timeMsCount > TIME_OUT_MS_RACK_WAIT_OUT1_ARRIVE_NEXT_NODE)
    {
        errorCode = AppImplRackWaitOut1ConvertErrorCode(FLAG_RACK_WAIT_OUT1_ERR_RELEASE_RACK_ARRIVE_NEXT_NODE_TIME_OUT,errorCode);
        return errorCode;
    }
    return errorCode;
}

//执行一次释放
LH_ERR AppImplRackWaitOut1ReleaseRackOnce(APP_STATE_RACK_WAIT_OUT1** rackWaitOut1StatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_WAIT_OUT1* rackWaitOut1StatePtr = AppStateReadRackWaitOut1State();
    *rackWaitOut1StatePtrPtr = rackWaitOut1StatePtr;
    //更新传感器状态
    AppImplRackWaitOut1ReflushAllSensor(&(rackWaitOut1StatePtr->rackWaitOut1SensorSet));
    SENSOR_STATE rackExistSensorState = rackWaitOut1StatePtr->rackWaitOut1SensorSet.rackExistSensor;
    //检查释放条件
    errorCode = AppImplRackWaitOut1CheckReleaseRackCondition(rackExistSensorState);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //首先切换到释放状态
    errorCode = AppImplRackWaitOut1SwitchStateRelease(rackWaitOut1StatePtrPtr);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待试管运行到目标节点
    errorCode = AppImplRackWaitOut1WaitArriveNextNode(rackExistSensorState);
    if(errorCode != LH_ERR_NONE)
    {
        //即使报错,也要卡住
        AppImplRackWaitOut1SwitchStateBrake(rackWaitOut1StatePtrPtr);
        return errorCode;
    }
    //切换到卡住状态
    errorCode = AppImplRackWaitOut1SwitchStateBrake(rackWaitOut1StatePtrPtr);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器状态
    AppImplRackWaitOut1ReflushAllSensor(&(rackWaitOut1StatePtr->rackWaitOut1SensorSet));
    //更新节点状态
    rackWaitOut1StatePtr->rackComeInEventFlag = EVENT_FLAG_NONE;
    rackWaitOut1StatePtr->nodeState = RAIL_NODE_BLOCK;
    return errorCode;
}

//获取节点缓存讯息
LH_ERR AppImplRackWaitOut1ReadState(APP_STATE_RACK_WAIT_OUT1** rackWaitOut1StatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_WAIT_OUT1* rackWaitOut1StatePtr = AppStateReadRackWaitOut1State();
    *rackWaitOut1StatePtrPtr = rackWaitOut1StatePtr;
    //更新传感器状态
    AppImplRackWaitOut1ReflushAllSensor(&(rackWaitOut1StatePtr->rackWaitOut1SensorSet));
    return errorCode;
}

//清除节点缓存讯息
LH_ERR AppImplRackWaitOut1ClearState(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_WAIT_OUT1* rackWaitOut1StatePtr = AppStateReadRackWaitOut1State();
    rackWaitOut1StatePtr->rackComeInEventFlag = EVENT_FLAG_NONE;
    //更新传感器状态
    AppImplRackWaitOut1ReflushAllSensor(&(rackWaitOut1StatePtr->rackWaitOut1SensorSet));
    return errorCode;
}


