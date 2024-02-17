/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-11-08 16:11:03
**LastEditors: DengXiaoJun
**LastEditTime: 2021-12-30 16:52:57
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplRackEntry0.h"
#include "AppImplParamUtil.h"
#include "AppImplRackBarScan.h"

//模块错误代码定义
typedef enum FLAG_RACK_ENTRY0_ERR_CONVERT
{
    FLAG_RACK_ENTRY0_ERR_READ_INPUT_INDEX                = 0x01,//读输入引脚索引错误
    FLAG_RACK_ENTRY0_ERR_READ_INPUT                      = 0x02,//读输入引脚错误
    FLAG_RACK_ENTRY0_ERR_WRITE_OUTPUT_INDEX              = 0x03,//写输出引脚索引错误
    FLAG_RACK_ENTRY0_ERR_WRITE_OUTPUT                    = 0x04,//写输出引脚错误
    FLAG_RACK_ENTRY0_ERR_STEP_MOTOR_INDEX                = 0x05,//电机索引错误
    FLAG_RACK_ENTRY0_ERR_STEP_MOTOR_RESET                = 0x06,//电机复位错误
    FLAG_RACK_ENTRY0_ERR_STEP_MOTOR_RUN_STEP             = 0x07,//电机走步数错误
    FLAG_RACK_ENTRY0_ERR_STEP_MOTOR_READ_POS             = 0X08,//电机读坐标错误
    FLAG_RACK_ENTRY0_ERR_CLAMP_RESET                     = 0X09,//防卡电机复位
    FLAG_RACK_ENTRY0_ERR_CLAMP_ACTION                    = 0X0A,//防卡电机卡住工作状态
    FLAG_RACK_ENTRY0_ERR_CLAMP_RELEASE                   = 0X0B,//防卡电机释放状态
    FLAG_RACK_ENTRY0_ERR_CLAMP_RETURN_ZERO               = 0X0C,//回零失败
    FLAG_RACK_ENTRY0_ERR_CLAMP_RELEASE_TIME_OUT          = 0X0D,//等待架子离开超时
    FLAG_RACK_ENTRY0_ERR_RELEASE_TO_NEXT_NODE_TIME_OUT   = 0X0E,//释放到下一个节点超时
    FLAG_RACK_ENTRY0_ERR_RELEASE_CONDITION               = 0X0F,//不满足释放条件
}FLAG_RACK_ENTRY0_ERR_CONVERT;


//输入引脚配置
static const INPUT_SENSOR_PIN_CONFIG inputSensorPinConfigArray[] = {
    {INPUT_RACK_ENTRY0_INPUT_EXIST           , VAILD_INPUT_RACK_ENTRY0_INPUT_EXIST        },//进样检测光电
    {INPUT_RACK_ENTRY0_INPUT_MOTOR_ORIGIN    , VAILD_INPUT_RACK_ENTRY0_INPUT_MOTOR_ORIGIN },//进样防夹电机原点
};
//输入引脚配置表长度
#define LENGTH_INPUT_SENSOR_PIN_CONFIG_ARRARY          (sizeof(inputSensorPinConfigArray)/sizeof(inputSensorPinConfigArray[0])) 

//输出引脚配置
const static OUTPUT_CTRL_PIN_CONFIG outputCtrlPinConfigArrary[] = {
    {OUTPUT_RACK_ENTRY0_OUT_RAIL_IN_START,           VAILD_OUTPUT_RACK_ENTRY0_OUT_RAIL_IN_START           },//进样轨道直流电机启停
    {OUTPUT_RACK_ENTRY0_OUT_RAIL_IN_DIR,             VAILD_OUTPUT_RACK_ENTRY0_OUT_RAIL_IN_DIR             },//进样轨道直流电机方向控制
    {OUTPUT_RACK_ENTRY0_OUT_RAIL_IN_SPEED,           VAILD_OUTPUT_RACK_ENTRY0_OUT_RAIL_IN_SPEED           },//进样轨道直流电机速度控制
    {OUTPUT_RACK_ENTRY0_OUT_RAIL_IN_EMSTOP,          VAILD_OUTPUT_RACK_ENTRY0_OUT_RAIL_IN_EMSTOP          },//进样轨道直流电机急停
    {OUTPUT_RACK_ENTRY0_OUT_RAIL_IN_ALARM_RELEASE,   VAILD_OUTPUT_RACK_ENTRY0_OUT_RAIL_IN_ALARM_RELEASE   },//进样轨道直流电机报警解除
    {OUTPUT_RACK_ENTRY0_OUT_RAIL_OUT_START,          VAILD_OUTPUT_RACK_ENTRY0_OUT_RAIL_OUT_START          },//出样轨道直流电机启停
    {OUTPUT_RACK_ENTRY0_OUT_RAIL_OUT_DIR,            VAILD_OUTPUT_RACK_ENTRY0_OUT_RAIL_OUT_DIR            },//出样轨道直流电机方向控制
    {OUTPUT_RACK_ENTRY0_OUT_RAIL_OUT_SPEED,          VAILD_OUTPUT_RACK_ENTRY0_OUT_RAIL_OUT_SPEED          },//出样轨道直流电机速度控制
    {OUTPUT_RACK_ENTRY0_OUT_RAIL_OUT_EMSTOP,         VAILD_OUTPUT_RACK_ENTRY0_OUT_RAIL_OUT_EMSTOP         },//出样轨道直流电机急停
    {OUTPUT_RACK_ENTRY0_OUT_RAIL_OUT_ALARM_RELEASE,  VAILD_OUTPUT_RACK_ENTRY0_OUT_RAIL_OUT_ALARM_RELEASE  },//出样轨道直流电机报警解除
};
//输出引脚配置配置数据长度
#define LENGTH_OUTPUT_CTRL_PIN_CONFIG_ARRARY          (sizeof(outputCtrlPinConfigArrary)/sizeof(outputCtrlPinConfigArrary[0])) 

//步进电机调试配置表
static const MODULE_STEP_MOTOR_RESET_RUN_CONFIG moduleStepMotorResetRunConfigArray[] = {
    {RACK_ENTRY0_SM_CLAMP_ID    ,APP_PARAM_MAIN_RACK_ENTRY ,APP_PARAM_SUB_RACK_ENTRY_IDLE_POS },//进样防卡
};
//步进电机配置表长度
#define LENGTH_STEP_MOTOR_RESET_RUN_CONFIG_ARRAY        (sizeof(moduleStepMotorResetRunConfigArray)/sizeof(moduleStepMotorResetRunConfigArray[0]))


//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplRackEntry0ConvertErrorCode(FLAG_RACK_ENTRY0_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_RACK_ENTRY;
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
LH_ERR AppImplRackEntry0ReadInput(RACK_ENTRY0_INPUT inputIndex,SENSOR_STATE* sensorStatePtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(inputIndex) >= LENGTH_INPUT_SENSOR_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplRackEntry0ConvertErrorCode(FLAG_RACK_ENTRY0_ERR_READ_INPUT_INDEX,errorCode);
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
LH_ERR AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUTPUT outputIndex,OUT_STATE setState)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(outputIndex) >= LENGTH_OUTPUT_CTRL_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplRackEntry0ConvertErrorCode(FLAG_RACK_ENTRY0_ERR_WRITE_OUTPUT_INDEX,errorCode);
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

//更新节点传感器讯息
static void AppImplRackEntry0ReflushSensorSet(RACK_ENTRY_SENSOR_SET* rackEntrySensorSetPtrPtr)
{
    if(rackEntrySensorSetPtrPtr == NULL)return;
    //更新传感器讯息
    AppImplRackEntry0ReadInput(RACK_ENTRY0_INPUT_EXIST,&(rackEntrySensorSetPtrPtr->rackExistSensor));
}

//进样节点电机复位
static LH_ERR AppImplRackEntry0ClampMotorReset(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //复位指令结构体
    StepMotorResetCmd resetCmd;
    //初始化复位参数
    IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
    //获取复位修正参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_ENTRY,APP_PARAM_SUB_RACK_ENTRY_IDLE_POS,&paramUtil);
    //填充参数
    resetCmd.motorIndex = RACK_ENTRY0_SM_CLAMP_ID;//电机序号
    resetCmd.timeOutSet = 60000;//超时时间
    resetCmd.correctionCoordinate = paramUtil;//复位修正
    //开始复位
    errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplRackEntry0ConvertErrorCode(FLAG_RACK_ENTRY0_ERR_CLAMP_RESET,errorCode);
        return errorCode;
    }
    return errorCode;
}

//进样节点电机回零
static LH_ERR AppImplRackEntry0ClampMotorReturnZero(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机回零结构体
    StepMotorReturnZeroCmd returnZeroCmd;
    //出样旋转
    //获取复位修正参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_ENTRY,APP_PARAM_SUB_RACK_ENTRY_IDLE_POS,&paramUtil);
    //回零结构体初始化
    IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
    //配置回零参数
    returnZeroCmd.motorIndex = RACK_ENTRY0_SM_CLAMP_ID;
    returnZeroCmd.correctionCoordinate = paramUtil;
    returnZeroCmd.selectConfig = S_CURVE_RACK_ENTRY0_SM_CLAMP_RETURN_ZERO;
    returnZeroCmd.timeOutSet = 60000;
    errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //回零失败
        errorCode = AppImplRackEntry0ConvertErrorCode(FLAG_RACK_ENTRY0_ERR_CLAMP_RETURN_ZERO,errorCode);
        return errorCode;
    }
    return errorCode;
}

//进样节点电机运行到释放位置
static LH_ERR AppImplRackEntry0ClampMotorRunToReleasePos(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机走坐标结构体
    StepMotorRun2CoordationCmd runToPosCmd;
    //初始化走坐标参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_ENTRY,APP_PARAM_SUB_RACK_ENTRY_IDLE_POS,&paramUtil);
    //初始化走目标坐标结构
    IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
    //填充走坐标结构体
    runToPosCmd.motorIndex = RACK_ENTRY0_SM_CLAMP_ID;//电机序号
    runToPosCmd.targetCoordinate = paramUtil;//目标坐标
    runToPosCmd.selectConfig = S_CURVE_RACK_ENTRY0_SM_CLAMP_IDLE;//速度曲线
    runToPosCmd.timeOutSet = 60000;
    runToPosCmd.utilStop1Enable = 0;
    //走坐标
    errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
    if(errorCode != LH_ERR_NONE)
    {
        errorCode = AppImplRackEntry0ConvertErrorCode(FLAG_RACK_ENTRY0_ERR_CLAMP_RELEASE,errorCode);
        return errorCode;
    }
    return errorCode;
}

//进样节点电机运行到限位位置
static LH_ERR AppImplRackEntry0ClampMotorRunToBlockPos(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //电机走坐标结构体
    StepMotorRun2CoordationCmd runToPosCmd;
    //初始化走坐标参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_ENTRY,APP_PARAM_SUB_RACK_ENTRY_ACTION_POS,&paramUtil);
    //初始化走目标坐标结构
    IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
    //填充走坐标结构体
    runToPosCmd.motorIndex = RACK_ENTRY0_SM_CLAMP_ID;//电机序号
    runToPosCmd.targetCoordinate = paramUtil;//目标坐标
    runToPosCmd.selectConfig = S_CURVE_RACK_ENTRY0_SM_CLAMP_ACTION;//速度曲线
    runToPosCmd.timeOutSet = 60000;
    runToPosCmd.utilStop1Enable = 0;
    //走坐标
    errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
    if(errorCode != LH_ERR_NONE)
    {
        errorCode = AppImplRackEntry0ConvertErrorCode(FLAG_RACK_ENTRY0_ERR_CLAMP_ACTION,errorCode);
        return errorCode;
    }
    return errorCode;
}

//等待试管架离开节点
static LH_ERR AppImplRackEntry0WaitRackMoveOut(SENSOR_STATE rackExistFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_ENTRY* rackEntryStatePtr = AppStateReadRackEntryState();
    RACK_ENTRY_SENSOR_SET* rackEntrySensorSetPtr = &(rackEntryStatePtr->rackEntrySensorSet);
    //没有架子的情况下,直接返回
    if(rackExistFlag == SENSOR_STATE_INVALID)
    {
        return LH_ERR_NONE;
    }
    //存在架子的情况下,在指定时间检测到多次脱离信号代表脱离成功
    uint32_t timeMsCount = 0;
    uint16_t waitSensorInvalidCount = 0;
    do
    {
        //延时
        timeMsCount += TIME_MS_WAIT_EVENT_INTERVAL;
        CoreDelayMs(TIME_MS_WAIT_EVENT_INTERVAL);
        //更新传感器数据
        AppImplRackEntry0ReflushSensorSet(rackEntrySensorSetPtr);
        //累计传感器无效的次数
        if(rackEntrySensorSetPtr->rackExistSensor == SENSOR_STATE_VALID)
        {
            waitSensorInvalidCount = 0;
        }
        else
        {
            waitSensorInvalidCount++;
        }
        if(waitSensorInvalidCount >= (TIME_MS_STABLE_RACK_ENTRY0_WAIT_RACK_MOVE_OUT/TIME_MS_WAIT_EVENT_INTERVAL))
        {
            //多次检测到传感器无效,可以离开了
            return LH_ERR_NONE;
        }
    } while (timeMsCount <= TIME_MS_MAX_RACK_ENTRY0_WAIT_RACK_MOVE_OUT);
    //查看完成结果讯息
    if(timeMsCount > TIME_MS_MAX_RACK_ENTRY0_WAIT_RACK_MOVE_OUT)
    {
        //节点释放超时
        errorCode = AppImplRackEntry0ConvertErrorCode(FLAG_RACK_ENTRY0_ERR_CLAMP_RELEASE_TIME_OUT,errorCode);
        return errorCode;
    }
    return errorCode;
}

//等待试管架到下一个节点
static LH_ERR AppImplRackEntry0ReleaseArriveNextNode(SENSOR_STATE rackExistFlag)
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
        AppImplRackBarScanReadInput(RACK_BAR_SCAN_INPUT_EXIST,&nextNodeSensorState);
        //累计传感器有效的次数
        if(nextNodeSensorState == SENSOR_STATE_VALID)
        {
            waitSensorValidCount++;
        }
        else
        {
            waitSensorValidCount = 0;
        }
        if(waitSensorValidCount >= (TIME_MS_STABLE_RACK_ENTRY0_RELEASE_ARRIVE_NEXT_NODE/TIME_MS_WAIT_EVENT_INTERVAL))
        {
            //多次检测到传感器无效,可以离开了
            return LH_ERR_NONE;
        }
    } while (timeMsCount <= TIME_MS_MAX_RACK_ENTRY0_RELEASE_ARRIVE_NEXT_NODE);
    //结果检查
    if(timeMsCount > TIME_MS_MAX_RACK_ENTRY0_RELEASE_ARRIVE_NEXT_NODE)
    {
        errorCode = AppImplRackEntry0ConvertErrorCode(FLAG_RACK_ENTRY0_ERR_RELEASE_TO_NEXT_NODE_TIME_OUT,errorCode);
        return errorCode;
    }
    return errorCode;
}

//检查释放条件
static LH_ERR AppImplRackEntry0CheckReleaseCondition(SENSOR_STATE rackExistFlag)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //没有架子的情况下,直接返回
    if(rackExistFlag == SENSOR_STATE_INVALID)
    {
        return LH_ERR_NONE;
    }
    SENSOR_STATE nextNodeSensorState = SENSOR_STATE_VALID;
    AppImplRackBarScanReadInput(RACK_BAR_SCAN_INPUT_EXIST,&nextNodeSensorState);
    if(nextNodeSensorState == SENSOR_STATE_VALID)
    {
        //传感器有效,报错不能释放
        errorCode = AppImplRackEntry0ConvertErrorCode(FLAG_RACK_ENTRY0_ERR_RELEASE_CONDITION,errorCode);
        return errorCode;
    }
    return errorCode;
}

static uint32_t railControlTick = 0;
static uint8_t alarmReleaseFlag = 0;
//记录上一次的状态
static RACK_ENTRY_SENSOR_SET rackEntryLastSensorSet = {SENSOR_STATE_INVALID};
//初始化功能状态机
void AppImplRackEntry0FSM_Init(void)
{
    APP_STATE_RACK_ENTRY* rackEntryStatePtr = AppStateReadRackEntryState();
    //状态初始化
    rackEntryStatePtr->rackComeInEventFlag = EVENT_FLAG_NONE;
    railControlTick = 0;
    alarmReleaseFlag = 0;
    //初始化的时候,节点处于位置状态
    rackEntryStatePtr->nodeState = RAIL_NODE_UNKNOW;
    //读取并记录传感器状态
    AppImplRackEntry0ReflushSensorSet(&(rackEntryStatePtr->rackEntrySensorSet));
    //数据拷贝
    UserMemCopy(&rackEntryLastSensorSet,&(rackEntryStatePtr->rackEntrySensorSet),sizeof(RACK_ENTRY_SENSOR_SET)/sizeof(uint8_t));
}


//运行功能状态机
void AppImplRackEntry0FSM_Run(void)
{
    railControlTick++;
    //定期进行报警解除工作
    if(railControlTick%4 == 0)
    {
        if(alarmReleaseFlag == 0)
        {
            alarmReleaseFlag = 1;
            //进样轨道
            AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_IN_ALARM_RELEASE,OUT_STATE_DISABLE);
            //出样轨道
            AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_OUT_ALARM_RELEASE,OUT_STATE_DISABLE);
        }
        else
        {
            alarmReleaseFlag = 0;
            //进样轨道
            AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_IN_ALARM_RELEASE,OUT_STATE_ENABLE);
            //出样轨道
            AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_OUT_ALARM_RELEASE,OUT_STATE_ENABLE);
        }
    }
    //如果节点处于未知状态,状态机不运行
    APP_STATE_RACK_ENTRY* rackEntryStatePtr = AppStateReadRackEntryState();
    if(rackEntryStatePtr->nodeState == RAIL_NODE_UNKNOW)
    {
        return;
    }
    //如果入口事件处于已经触发状态,状态机不运行
    if(rackEntryStatePtr->rackComeInEventFlag == EVENT_FLAG_TRIG)
    {
        return;
    }
    //检查标志变化,首先读取当前状态
    AppImplRackEntry0ReflushSensorSet(&(rackEntryStatePtr->rackEntrySensorSet));
    if(rackEntryStatePtr->rackEntrySensorSet.rackExistSensor == SENSOR_STATE_VALID)
    {
        //本次传感器状态为有效且上次传感器状态为无效的情况下,才能触发事件
        if(rackEntryLastSensorSet.rackExistSensor == SENSOR_STATE_INVALID)
        {
            rackEntryStatePtr->rackComeInEventFlag = EVENT_FLAG_TRIG;
        }
    }
    //更新检测讯息,数据拷贝
    UserMemCopy(&rackEntryLastSensorSet,&(rackEntryStatePtr->rackEntrySensorSet),sizeof(RACK_ENTRY_SENSOR_SET)/sizeof(uint8_t));
}


//指定步进电机复位
LH_ERR AppImplRackEntry0StepMotorReset(RACK_ENTRY0_SM smIndex,int32_t* currentPos)
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
        errorCode = AppImplRackEntry0ConvertErrorCode(FLAG_RACK_ENTRY0_ERR_STEP_MOTOR_INDEX,errorCode);
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
        errorCode = AppImplRackEntry0ConvertErrorCode(FLAG_RACK_ENTRY0_ERR_STEP_MOTOR_RESET,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //读取结果
    errorCode = IPC_StepMotorReflushStateWhileReturn(resetCmd.motorIndex,currentPos);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplRackEntry0ConvertErrorCode(FLAG_RACK_ENTRY0_ERR_STEP_MOTOR_READ_POS,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    return LH_ERR_NONE;
}

//指定步进电机走位
LH_ERR AppImplRackEntry0StepMotorRunSteps(RACK_ENTRY0_SM smIndex,int32_t runSteps,int32_t* currentPos)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //索引判断
    if((uint8_t)(smIndex) >= LENGTH_STEP_MOTOR_RESET_RUN_CONFIG_ARRAY)
    {
        //错误代码转换
        errorCode = AppImplRackEntry0ConvertErrorCode(FLAG_RACK_ENTRY0_ERR_STEP_MOTOR_INDEX,errorCode);
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
        errorCode = AppImplRackEntry0ConvertErrorCode(FLAG_RACK_ENTRY0_ERR_STEP_MOTOR_RUN_STEP,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //读取结果
    errorCode = IPC_StepMotorReflushStateWhileReturn(runStepCmd.motorIndex,currentPos);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplRackEntry0ConvertErrorCode(FLAG_RACK_ENTRY0_ERR_STEP_MOTOR_READ_POS,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    return LH_ERR_NONE;
}

//节点复位
LH_ERR AppImplRackEntry0ModuleReset(APP_STATE_RACK_ENTRY** rackEntryStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_ENTRY* rackEntryStatePtr = AppStateReadRackEntryState();
    *rackEntryStatePtrPtr = rackEntryStatePtr;
    //节点复位
    errorCode = AppImplRackEntry0ClampMotorReset();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //运行到释放位置
    errorCode = AppImplRackEntry0ClampMotorRunToReleasePos();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器讯息
    AppImplRackEntry0ReflushSensorSet(&(rackEntryStatePtr->rackEntrySensorSet));
    //事件清除
    rackEntryStatePtr->rackComeInEventFlag = EVENT_FLAG_NONE;
    //状态切换为释放
    rackEntryStatePtr->nodeState = RAIL_NODE_RELEASE;
    return errorCode;
}

//节点卡住试管架
LH_ERR AppImplRackEntry0SwitchStateBlock(APP_STATE_RACK_ENTRY** rackEntryStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_ENTRY* rackEntryStatePtr = AppStateReadRackEntryState();
    *rackEntryStatePtrPtr = rackEntryStatePtr;
    //检查当前位置是限位位置还是释放位置
    if(rackEntryStatePtr->nodeState != RAIL_NODE_BLOCK)
    {
        //电机先回零
        errorCode = AppImplRackEntry0ClampMotorReturnZero();
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    }
    //运行到卡住位置
    errorCode = AppImplRackEntry0ClampMotorRunToBlockPos();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器状态
    AppImplRackEntry0ReflushSensorSet(&(rackEntryStatePtr->rackEntrySensorSet));
    //状态切换为卡住状态
    rackEntryStatePtr->nodeState = RAIL_NODE_BLOCK;
    return errorCode;
}

//节点释放试管架
LH_ERR AppImplRackEntry0SwitchStateRelease(APP_STATE_RACK_ENTRY** rackEntryStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_ENTRY* rackEntryStatePtr = AppStateReadRackEntryState();
    *rackEntryStatePtrPtr = rackEntryStatePtr;
    //检查当前位置是限位位置还是释放位置
    if(rackEntryStatePtr->nodeState != RAIL_NODE_RELEASE)
    {
        //电机先回零
        errorCode = AppImplRackEntry0ClampMotorReturnZero();
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    }
    //运行到释放位置
    errorCode = AppImplRackEntry0ClampMotorRunToReleasePos();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器状态
    AppImplRackEntry0ReflushSensorSet(&(rackEntryStatePtr->rackEntrySensorSet));
    //状态切换为释放状态
    rackEntryStatePtr->nodeState = RAIL_NODE_RELEASE;
    return errorCode;
}

//节点放行
LH_ERR AppImplRackEntry0ReleaseRack(APP_STATE_RACK_ENTRY** rackEntryStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_ENTRY* rackEntryStatePtr = AppStateReadRackEntryState();
    *rackEntryStatePtrPtr = rackEntryStatePtr;
    //更新传感器状态
    AppImplRackEntry0ReflushSensorSet(&(rackEntryStatePtr->rackEntrySensorSet));
    //试管架存在标记
    SENSOR_STATE rackExistFlag = rackEntryStatePtr->rackEntrySensorSet.rackExistSensor;
    //检查释放条件
    errorCode = AppImplRackEntry0CheckReleaseCondition(rackExistFlag);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //运行到释放位置
    errorCode = AppImplRackEntry0SwitchStateRelease(rackEntryStatePtrPtr);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待试管架离开
    errorCode = AppImplRackEntry0WaitRackMoveOut(rackExistFlag);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //重新锁定
    errorCode = AppImplRackEntry0SwitchStateBlock(rackEntryStatePtrPtr);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //等待运行到下一节点
    errorCode = AppImplRackEntry0ReleaseArriveNextNode(rackExistFlag);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器状态
    AppImplRackEntry0ReflushSensorSet(&(rackEntryStatePtr->rackEntrySensorSet));
    //清除事件
    rackEntryStatePtr->rackComeInEventFlag = EVENT_FLAG_NONE;
    //状态切换为卡住状态
    rackEntryStatePtr->nodeState = RAIL_NODE_BLOCK;
    return LH_ERR_NONE;
}

//轨道启动
LH_ERR AppImplRackEntry0RailStart(uint8_t railInDir,uint8_t railOutDir,APP_STATE_RACK_ENTRY** rackEntryStatePtrPtr)
{
    APP_STATE_RACK_ENTRY* rackEntryStatePtr = AppStateReadRackEntryState();
    *rackEntryStatePtrPtr = rackEntryStatePtr;
    //进样轨道
    AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_IN_ALARM_RELEASE,OUT_STATE_ENABLE);
    CoreDelayMs(10);
    AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_IN_EMSTOP,OUT_STATE_DISABLE);
    CoreDelayMs(10);
    AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_IN_START,OUT_STATE_ENABLE);
    CoreDelayMs(10);
    if(railInDir == 0)
    {
        AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_IN_DIR,OUT_STATE_DISABLE);
    }
    else
    {
        AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_IN_DIR,OUT_STATE_ENABLE);
    }
    CoreDelayMs(10);
    AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_IN_SPEED,OUT_STATE_ENABLE);

    //出样轨道
    AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_OUT_ALARM_RELEASE,OUT_STATE_ENABLE);
    CoreDelayMs(10);
    AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_OUT_EMSTOP,OUT_STATE_DISABLE);
    CoreDelayMs(10);
    AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_OUT_START,OUT_STATE_ENABLE);
    CoreDelayMs(10);
    if(railOutDir == 0)
    {
        AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_OUT_DIR,OUT_STATE_DISABLE);
    }
    else
    {
        AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_OUT_DIR,OUT_STATE_ENABLE);
    }
    CoreDelayMs(10);
    AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_OUT_SPEED,OUT_STATE_ENABLE);
    
    //更新传感器状态
    AppImplRackEntry0ReflushSensorSet(&(rackEntryStatePtr->rackEntrySensorSet));
    return LH_ERR_NONE;
}

//轨道停止
LH_ERR AppImplRackEntry0RailStop(APP_STATE_RACK_ENTRY** rackEntryStatePtrPtr)
{
    APP_STATE_RACK_ENTRY* rackEntryStatePtr = AppStateReadRackEntryState();
    *rackEntryStatePtrPtr = rackEntryStatePtr;
    //进样轨道
    AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_IN_ALARM_RELEASE,OUT_STATE_ENABLE);
    CoreDelayMs(10);
    AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_IN_START,OUT_STATE_DISABLE);

    //出样轨道
    AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_OUT_ALARM_RELEASE,OUT_STATE_ENABLE);
    CoreDelayMs(10);
    AppImplRackEntry0WriteOutput(RACK_ENTRY0_OUT_RAIL_OUT_START,OUT_STATE_DISABLE);

    //更新传感器状态
    AppImplRackEntry0ReflushSensorSet(&(rackEntryStatePtr->rackEntrySensorSet));
    
    return LH_ERR_NONE;
}

//获取节点全部状态
LH_ERR AppImplRackEntry0ReadState(APP_STATE_RACK_ENTRY** rackEntryStatePtrPtr)
{
    APP_STATE_RACK_ENTRY* rackEntryStatePtr = AppStateReadRackEntryState();
    *rackEntryStatePtrPtr = rackEntryStatePtr;
    //更新传感器状态
    AppImplRackEntry0ReflushSensorSet(&(rackEntryStatePtr->rackEntrySensorSet));
    return LH_ERR_NONE;
}

//清除节点记录状态
LH_ERR AppImplRackEntry0ClearState(void)
{
    APP_STATE_RACK_ENTRY* rackEntryStatePtr = AppStateReadRackEntryState();
    rackEntryStatePtr->rackComeInEventFlag = EVENT_FLAG_NONE;
    return LH_ERR_NONE;
}



