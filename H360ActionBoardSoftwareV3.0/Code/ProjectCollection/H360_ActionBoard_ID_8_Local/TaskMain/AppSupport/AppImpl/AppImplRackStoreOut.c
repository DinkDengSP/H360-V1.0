/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-11-08 16:13:21
**LastEditors: DengXiaoJun
**LastEditTime: 2021-12-28 11:55:39
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplRackStoreOut.h"
#include "AppImplParamUtil.h"
#include "AppImplRackOutSwitch.h"


//系统错误类别配置
typedef enum FLAG_RACK_STOTE_OUT_ERR_CONVERT
{
    FLAG_RACK_STORE_OUT_ERR_READ_INPUT_INDEX                     = 0x01,//读输入引脚索引错误
    FLAG_RACK_STORE_OUT_ERR_READ_INPUT                           = 0x02,//读输入引脚错误
    FLAG_RACK_STORE_OUT_ERR_WRITE_OUTPUT_INDEX                   = 0x03,//写输出引脚索引错误
    FLAG_RACK_STORE_OUT_ERR_WRITE_OUTPUT                         = 0x04,//写输出引脚错误
    FLAG_RACK_STORE_OUT_ERR_STEP_MOTOR_INDEX                     = 0x05,//电机索引错误
    FLAG_RACK_STORE_OUT_ERR_STEP_MOTOR_RESET                     = 0x06,//电机复位错误
    FLAG_RACK_STORE_OUT_ERR_STEP_MOTOR_RUN_STEP                  = 0x07,//电机走步数错误
    FLAG_RACK_STORE_OUT_ERR_STEP_MOTOR_READ_POS                  = 0X08,//电机读坐标错误
    FLAG_RACK_STORE_OUT_ERR_PUSH_MOTOR_RESET                     = 0X09,//推进电机复位产生的错误
    FLAG_RACK_STORE_OUT_ERR_PUSH_MOTOR_RETURN_ZERO               = 0X0A,//推进电机回零产生的错误
    FLAG_RACK_STORE_OUT_ERR_PUSH_MOTOR_RUN_PREPARE               = 0X0B,//推手走到准备位置
    FLAG_RACK_STORE_OUT_ERR_PUSH_MOTOR_PUSH_ONCE                 = 0X0C,//推手推进走一次
    FLAG_RACK_STORE_OUT_ERR_PUSH_COUNT_MAX_LIMIT                 = 0X0D,//推进次数超过系统最大推进次数
    FLAG_RACK_STORE_OUT_ERR_ROTATE_MOTOR_RESET                   = 0X0E,//旋转电机复位产生的错误
    FLAG_RACK_STORE_OUT_ERR_ROTATE_MOTOR_RETURN_ZERO             = 0X0F,//旋转电机回零产生的错误
    FLAG_RACK_STORE_OUT_ERR_ROTATE_MOTOR_TO_ACTION               = 0X10,//旋转运行到工作位置
    FLAG_RACK_STORE_OUT_ERR_ROTATE_MOTOR_TO_IDLE                 = 0X11,//旋转运行到空闲位置
    FLAG_RACK_STORE_OUT_ERR_PUSH_MOTOR_PUSH_OUT_SWITCH_TO_STORE  = 0X12,//推手从换轨出口到样本架入口
    FLAG_RACK_STORE_OUT_ERR_PUSH_MOTOR_BACK_ONCE                 = 0X13,//推手回退一次
}FLAG_RACK_STOTE_OUT_ERR_CONVERT;

//输入引脚配置
static const INPUT_SENSOR_PIN_CONFIG inputSensorPinConfigArray[] = {
    {INPUT_RACK_STORE_OUT_INPUT_NEAR_KEY_TARY_NEAR_KEY  , VAILD_INPUT_RACK_STORE_OUT_INPUT_NEAR_KEY_TARY_NEAR_KEY   },//靠近按键托盘靠近轨道光电传感器
    {INPUT_RACK_STORE_OUT_INPUT_NEAR_KEY_TARY_NEAR_RAIL , VAILD_INPUT_RACK_STORE_OUT_INPUT_NEAR_KEY_TARY_NEAR_RAIL  },//靠近按键托盘靠近按键光电传感器
    {INPUT_RACK_STORE_OUT_INPUT_NEAR_RAIL_TARY_NEAR_KEY , VAILD_INPUT_RACK_STORE_OUT_INPUT_NEAR_RAIL_TARY_NEAR_KEY  },//靠近轨道托盘靠近按键光电传感器
    {INPUT_RACK_STORE_OUT_INPUT_NEAR_RAIL_TARY_NEAR_RAIL, VAILD_INPUT_RACK_STORE_OUT_INPUT_NEAR_RAIL_TARY_NEAR_RAIL },//靠近轨道托盘靠近轨道光电传感器
    {INPUT_RACK_STORE_OUT_INPUT_MOTOR_PUSH_ORIGIN       , VAILD_INPUT_RACK_STORE_OUT_INPUT_MOTOR_PUSH_ORIGIN        },//出样仓推进步进电机原点
    {INPUT_RACK_STORE_OUT_INPUT_MOTOR_ROTATE_ORIGIN     , VAILD_INPUT_RACK_STORE_OUT_INPUT_MOTOR_ROTATE_ORIGIN      },//出样仓旋转步进电机原点
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
    {RACK_STORE_OUT_SM_PUSH_ID    ,APP_PARAM_MAIN_RACK_STORE_OUT ,APP_PARAM_SUB_RACK_STORE_OUT_PUSH_MOTOR_RESET_CORRECT},//出样仓推手步进电机
    {RACK_STORE_OUT_SM_ROTATE_ID  ,APP_PARAM_MAIN_RACK_STORE_OUT ,APP_PARAM_SUB_RACK_STORE_OUT_ROTATE_IDLE_POS         },//出样仓旋转步进电机
};
//步进电机配置表长度
#define LENGTH_STEP_MOTOR_RESET_RUN_CONFIG_ARRAY        (sizeof(moduleStepMotorResetRunConfigArray)/sizeof(moduleStepMotorResetRunConfigArray[0]))


//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STOTE_OUT_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_RACK_STORE_OUT;
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
LH_ERR AppImplRackStoreOutReadInput(RACK_STORE_OUT_INPUT inputIndex,SENSOR_STATE* sensorStatePtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(inputIndex) >= LENGTH_INPUT_SENSOR_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_READ_INPUT_INDEX,errorCode);
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
LH_ERR AppImplRackStoreOutWriteOutput(RACK_STORE_OUT_OUTPUT outputIndex,OUT_STATE setState)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(outputIndex) >= LENGTH_OUTPUT_CTRL_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_WRITE_OUTPUT_INDEX,errorCode);
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

//更新全部传感器讯息
static void AppImplRackStoreOutReflushAllSensor(RACK_STORE_OUT_SENSOR_SET* rackStoreOutSensorSetPtr)
{
    AppImplRackStoreOutReadInput(RACK_STORE_OUT_INPUT_NEAR_KEY_TARY_NEAR_KEY,&(rackStoreOutSensorSetPtr->closeToKeyTaryCloseToKeySensor));
    AppImplRackStoreOutReadInput(RACK_STORE_OUT_INPUT_NEAR_KEY_TARY_NEAR_RAIL,&(rackStoreOutSensorSetPtr->closeToKeyTaryCloseToRailSensor));
    AppImplRackStoreOutReadInput(RACK_STORE_OUT_INPUT_NEAR_RAIL_TARY_NEAR_KEY,&(rackStoreOutSensorSetPtr->closeToRailTaryCloseToKeySensor));
    AppImplRackStoreOutReadInput(RACK_STORE_OUT_INPUT_NEAR_RAIL_TARY_NEAR_RAIL,&(rackStoreOutSensorSetPtr->closeToRailTaryCloseToRailSensor));
}

//出样推手复位
static LH_ERR AppImplRackStoreOutMotorPushReset(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //复位指令结构体
    StepMotorResetCmd resetCmd;
    //获取复位修正参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_STORE_OUT,APP_PARAM_SUB_RACK_STORE_OUT_PUSH_MOTOR_RESET_CORRECT,&paramUtil);
    //填充参数
    IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
    resetCmd.motorIndex = RACK_STORE_OUT_SM_PUSH_ID;//电机序号
    resetCmd.timeOutSet = 60000;//超时时间
    resetCmd.correctionCoordinate = paramUtil;//复位修正
    //开始复位
    errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_PUSH_MOTOR_RESET,errorCode);
        return errorCode;
    }
    return errorCode;
}

//出样推手回零
static LH_ERR AppImplRackStoreOutMotorPushReturnZero(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机回零结构体
    StepMotorReturnZeroCmd returnZeroCmd;
    //获取复位修正参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_STORE_OUT,APP_PARAM_SUB_RACK_STORE_OUT_PUSH_MOTOR_RESET_CORRECT,&paramUtil);
    //填充参数
    IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
    returnZeroCmd.motorIndex = RACK_STORE_OUT_SM_PUSH_ID;
    returnZeroCmd.correctionCoordinate = paramUtil;
    returnZeroCmd.selectConfig = S_CURVE_RACK_STORE_OUT_SM_PUSH_RETURN_ZERO;
    returnZeroCmd.timeOutSet = 60000;
    errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //回零失败
        errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_PUSH_MOTOR_RETURN_ZERO,errorCode);
        return errorCode;
    }
    return errorCode;
}

//出样推手运行到起始位置
static LH_ERR AppImplRackStoreOutMotorPushRunToStart(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机走坐标结构体
    StepMotorRun2CoordationCmd runToPosCmd;
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_STORE_OUT,APP_PARAM_SUB_RACK_STORE_OUT_PUSH_START_POS,&paramUtil);
    //初始化走目标坐标结构
    IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
    //填充走坐标结构体
    runToPosCmd.motorIndex = RACK_STORE_OUT_SM_PUSH_ID;//电机序号
    runToPosCmd.targetCoordinate = paramUtil;//目标坐标
    runToPosCmd.selectConfig = S_CURVE_RACK_STORE_OUT_SM_PUSH_TO_PREPARE;//速度曲线
    runToPosCmd.timeOutSet = 60000;
    runToPosCmd.utilStop1Enable = 0;
    //走坐标
    errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //运行到指定坐标失败
        errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_PUSH_MOTOR_RUN_PREPARE,errorCode);
        return errorCode;
    }
    return errorCode;
}

//出样推手推进一次试管架步数
static LH_ERR AppImplRackStoreOutMotorPushRunOnceSteps(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    StepMotorRunStepsCmd runStepCmd;
    //初始化走步数结构
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //推进轴走步数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_STORE_OUT,APP_PARAM_SUB_RACK_STORE_OUT_PUSH_ONCE_STEP,&paramUtil);
    //填充走步数结构
    runStepCmd.motorIndex = RACK_STORE_OUT_SM_PUSH_ID;
    runStepCmd.specialSteps = paramUtil;
    runStepCmd.selectConfig = S_CURVE_RACK_STORE_OUT_SM_PUSH_PUSH;
    //执行一次走步数
    errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
    if(errorCode != LH_ERR_NONE)
    {
        errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_PUSH_MOTOR_PUSH_ONCE,errorCode);
        return errorCode;
    }
    return errorCode;
}

//出样旋转复位
static LH_ERR AppImplRackStoreOutMotorRotateReset(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //参数辅助
    int32_t paramUtil;
    //复位指令结构体
    StepMotorResetCmd resetCmd;
    IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
    //出样旋转
    //获取复位修正参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_STORE_OUT,APP_PARAM_SUB_RACK_STORE_OUT_ROTATE_IDLE_POS,&paramUtil);
    //填充参数
    resetCmd.motorIndex = RACK_STORE_OUT_SM_ROTATE_ID;//电机序号
    resetCmd.timeOutSet = 60000;//超时时间
    resetCmd.correctionCoordinate = paramUtil;//复位修正
    //开始复位
    errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_ROTATE_MOTOR_RESET,errorCode);
        return errorCode;
    }
    return errorCode;
}

//出样旋转回零
static LH_ERR AppImplRackStoreOutMotorRotateReturnZero(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机回零结构体
    StepMotorReturnZeroCmd returnZeroCmd;
    //出样旋转
    //获取复位修正参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_STORE_OUT,APP_PARAM_SUB_RACK_STORE_OUT_ROTATE_IDLE_POS,&paramUtil);
    //回零结构体初始化
    IPC_StepMotorUtilReturnZeroDataStructInit(&returnZeroCmd);
    //配置回零参数
    returnZeroCmd.motorIndex = RACK_STORE_OUT_SM_ROTATE_ID;
    returnZeroCmd.correctionCoordinate = paramUtil;
    returnZeroCmd.selectConfig = S_CURVE_RACK_STORE_OUT_SM_ROTATE_RETURN_ZERO;
    returnZeroCmd.timeOutSet = 60000;
    errorCode = IPC_StepMotorReturnZeroWhileReturn(&returnZeroCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //回零失败
        errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_ROTATE_MOTOR_RETURN_ZERO,errorCode);
        return errorCode;
    }
    return errorCode;
}

//出样旋转运行到工作位置
static LH_ERR AppImplRackStoreOutMotorRotateRunToAction(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机走坐标结构体
    StepMotorRun2CoordationCmd runToPosCmd;
    //读取参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_STORE_OUT,APP_PARAM_SUB_RACK_STORE_OUT_ROTATE_ACTION_POS,&paramUtil);
    //初始化走目标坐标结构
    IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
    //填充走坐标结构体
    runToPosCmd.motorIndex = RACK_STORE_OUT_SM_ROTATE_ID;//电机序号
    runToPosCmd.targetCoordinate = paramUtil;//目标坐标
    runToPosCmd.selectConfig = S_CURVE_RACK_STORE_OUT_SM_ROTATE_TO_ACTION;//速度曲线
    runToPosCmd.timeOutSet = 60000;
    runToPosCmd.utilStop1Enable = 0;
    //走坐标
    errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //运行到指定坐标失败
        errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_ROTATE_MOTOR_TO_ACTION,errorCode);
        return errorCode;
    }
    return errorCode;
}

//出样旋转运行到空闲位置
static LH_ERR AppImplRackStoreOutMotorRotateRunToIdle(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //电机走坐标结构体
    StepMotorRun2CoordationCmd runToPosCmd;
    //读取参数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_STORE_OUT,APP_PARAM_SUB_RACK_STORE_OUT_ROTATE_IDLE_POS,&paramUtil);
    //初始化走目标坐标结构
    IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
    //填充走坐标结构体
    runToPosCmd.motorIndex = RACK_STORE_OUT_SM_ROTATE_ID;//电机序号
    runToPosCmd.targetCoordinate = paramUtil;//目标坐标
    runToPosCmd.selectConfig = S_CURVE_RACK_STORE_OUT_SM_ROTATE_TO_IDLE;//速度曲线
    runToPosCmd.timeOutSet = 60000;
    runToPosCmd.utilStop1Enable = 0;
    //走坐标
    errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //运行到指定坐标失败
        errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_ROTATE_MOTOR_TO_IDLE,errorCode);
        return errorCode;
    }
    return errorCode;
}


//初始化功能状态机
void AppImplRackStoreOutFSM_Init(void)
{
    APP_STATE_RACK_STORE_OUT* rackStoreOutStatePtr = AppStateReadRackStoreOutState();
    //更新传感器讯息
    AppImplRackStoreOutReflushAllSensor(&(rackStoreOutStatePtr->rackStoreOutSensorSet));
    //推进次数清空
    rackStoreOutStatePtr->currentPushCount = 0;
    //推手旋转空闲状态
    rackStoreOutStatePtr->rotateState = PUSH_ROTATE_STATE_UNKNOW;
}

//运行功能状态机
void AppImplRackStoreOutFSM_Run(void)
{
    APP_STATE_RACK_STORE_OUT* rackStoreOutStatePtr = AppStateReadRackStoreOutState();
    //更新传感器讯息
    AppImplRackStoreOutReflushAllSensor(&(rackStoreOutStatePtr->rackStoreOutSensorSet));
}

//指定步进电机复位
LH_ERR AppImplRackStoreOutStepMotorReset(RACK_STORE_OUT_SM smIndex,int32_t* currentPos)
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
        errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_STEP_MOTOR_INDEX,errorCode);
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
        errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_STEP_MOTOR_RESET,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //读取结果
    errorCode = IPC_StepMotorReflushStateWhileReturn(resetCmd.motorIndex,currentPos);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_STEP_MOTOR_READ_POS,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    return LH_ERR_NONE;
}

//指定步进电机走位
LH_ERR AppImplRackStoreOutStepMotorRunSteps(RACK_STORE_OUT_SM smIndex,int32_t runSteps,int32_t* currentPos)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //索引判断
    if((uint8_t)(smIndex) >= LENGTH_STEP_MOTOR_RESET_RUN_CONFIG_ARRAY)
    {
        //错误代码转换
        errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_STEP_MOTOR_INDEX,errorCode);
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
        errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_STEP_MOTOR_RUN_STEP,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //读取结果
    errorCode = IPC_StepMotorReflushStateWhileReturn(runStepCmd.motorIndex,currentPos);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_STEP_MOTOR_READ_POS,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    return LH_ERR_NONE;
}

//出样仓复位
LH_ERR AppImplRackStoreOutModuleReset(APP_STATE_RACK_STORE_OUT** rackStoreOutStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_STORE_OUT* rackStoreOutStatePtr = AppStateReadRackStoreOutState();
    *rackStoreOutStatePtrPtr = rackStoreOutStatePtr;
    //旋转复位
    errorCode = AppImplRackStoreOutMotorRotateReset();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //旋转运行到空闲状态
    errorCode = AppImplRackStoreOutMotorRotateRunToIdle();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //推进复位
    errorCode = AppImplRackStoreOutMotorPushReset();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器讯息
    AppImplRackStoreOutReflushAllSensor(&(rackStoreOutStatePtr->rackStoreOutSensorSet));
    //推进次数清空
    rackStoreOutStatePtr->currentPushCount = 0;
    //推手旋转空闲状态
    rackStoreOutStatePtr->rotateState = PUSH_ROTATE_STATE_IDLE;
    return errorCode;
}

//出样仓回零
LH_ERR AppImplRackStoreOutReturnZero(APP_STATE_RACK_STORE_OUT** rackStoreOutStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_STORE_OUT* rackStoreOutStatePtr = AppStateReadRackStoreOutState();
    *rackStoreOutStatePtrPtr = rackStoreOutStatePtr;
    //旋转回零
    errorCode = AppImplRackStoreOutMotorRotateReturnZero();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //旋转运行到空闲状态
    errorCode = AppImplRackStoreOutMotorRotateRunToIdle();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //推手旋转空闲状态
    rackStoreOutStatePtr->rotateState = PUSH_ROTATE_STATE_IDLE;
    //推手回零
    errorCode = AppImplRackStoreOutMotorPushReturnZero();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //推进次数清空
    rackStoreOutStatePtr->currentPushCount = 0;
    //更新传感器讯息
    AppImplRackStoreOutReflushAllSensor(&(rackStoreOutStatePtr->rackStoreOutSensorSet));
    return errorCode;
}

//出样旋转到工作位置
LH_ERR AppImplRackStoreOutRotateSwitchToAction(APP_STATE_RACK_STORE_OUT** rackStoreOutStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_STORE_OUT* rackStoreOutStatePtr = AppStateReadRackStoreOutState();
    *rackStoreOutStatePtrPtr = rackStoreOutStatePtr;
    if(rackStoreOutStatePtr->rotateState != PUSH_ROTATE_STATE_ACTION)
    {
        //旋转回零
        errorCode = AppImplRackStoreOutMotorRotateReturnZero();
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    }
    //旋转运行到工作位置
    errorCode = AppImplRackStoreOutMotorRotateRunToAction();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器讯息
    AppImplRackStoreOutReflushAllSensor(&(rackStoreOutStatePtr->rackStoreOutSensorSet));
    //状态修改为工作状态
    rackStoreOutStatePtr->rotateState = PUSH_ROTATE_STATE_ACTION;
    return errorCode;
}

//出样旋转到空闲位置
LH_ERR AppImplRackStoreOutRotateSwitchToIdle(APP_STATE_RACK_STORE_OUT** rackStoreOutStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_STORE_OUT* rackStoreOutStatePtr = AppStateReadRackStoreOutState();
    *rackStoreOutStatePtrPtr = rackStoreOutStatePtr;
    if(rackStoreOutStatePtr->rotateState != PUSH_ROTATE_STATE_IDLE)
    {
        //旋转回零
        errorCode = AppImplRackStoreOutMotorRotateReturnZero();
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    }
    //推手运行到空闲状态
    AppImplRackStoreOutMotorRotateRunToIdle();
    //更新传感器讯息
    AppImplRackStoreOutReflushAllSensor(&(rackStoreOutStatePtr->rackStoreOutSensorSet));
    //状态修改为空闲状态
    rackStoreOutStatePtr->rotateState = PUSH_ROTATE_STATE_IDLE;
    return errorCode;
}

//出样仓运行到推进起点
LH_ERR AppImplRackStoreOutRunToPrepare(APP_STATE_RACK_STORE_OUT** rackStoreOutStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_STORE_OUT* rackStoreOutStatePtr = AppStateReadRackStoreOutState();
    *rackStoreOutStatePtrPtr = rackStoreOutStatePtr;
    //旋转空闲
    errorCode = AppImplRackStoreOutRotateSwitchToIdle(rackStoreOutStatePtrPtr);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //推手运行到开始位置
    errorCode = AppImplRackStoreOutMotorPushRunToStart();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //推进次数清空
    rackStoreOutStatePtr->currentPushCount = 0;
    //更新传感器讯息
    AppImplRackStoreOutReflushAllSensor(&(rackStoreOutStatePtr->rackStoreOutSensorSet));
    return errorCode;
}

//出样仓推进一次
LH_ERR AppImplRackStoreOutPushOnce(APP_STATE_RACK_STORE_OUT** rackStoreOutStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_STORE_OUT* rackStoreOutStatePtr = AppStateReadRackStoreOutState();
    *rackStoreOutStatePtrPtr = rackStoreOutStatePtr;
    //旋转到工作位置
    errorCode = AppImplRackStoreOutRotateSwitchToAction(rackStoreOutStatePtrPtr);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //推手推进一次
    errorCode = AppImplRackStoreOutMotorPushRunOnceSteps();
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //更新传感器讯息
    AppImplRackStoreOutReflushAllSensor(&(rackStoreOutStatePtr->rackStoreOutSensorSet));
    //推进次数增加
    rackStoreOutStatePtr->currentPushCount++;
    return errorCode;
}

//出样仓回退一次
LH_ERR AppImplRackStoreOutBackOnce(APP_STATE_RACK_STORE_OUT** rackStoreOutStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    APP_STATE_RACK_STORE_OUT* rackStoreOutStatePtr = AppStateReadRackStoreOutState();
    *rackStoreOutStatePtrPtr = rackStoreOutStatePtr;
    StepMotorRunStepsCmd runStepCmd;
    if(rackStoreOutStatePtr->currentPushCount > 0)
    {
        //初始化走步数结构
        IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
        //推进轴走步数
        AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_STORE_OUT,APP_PARAM_SUB_RACK_STORE_OUT_PUSH_ONCE_STEP,&paramUtil);
        //填充走步数结构
        runStepCmd.motorIndex = RACK_STORE_OUT_SM_PUSH_ID;
        runStepCmd.specialSteps = 0 - paramUtil;
        runStepCmd.selectConfig = S_CURVE_RACK_STORE_OUT_SM_PUSH_PUSH;
        //执行一次走步数
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_PUSH_MOTOR_BACK_ONCE,errorCode);
            return errorCode;
        }
        //计数-1
        rackStoreOutStatePtr->currentPushCount -= 1;
    }
    //更新传感器讯息
    AppImplRackStoreOutReflushAllSensor(&(rackStoreOutStatePtr->rackStoreOutSensorSet));
    return errorCode;
}

//将试管架从出样换轨出口推到出样仓入口
LH_ERR AppImplRackStoreOutOutSwitchToRackStore(APP_STATE_RACK_STORE_OUT** rackStoreOutStatePtrPtr,APP_STATE_RACK_OUT_SWITCH** rackOutSwitchStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_RACK_STORE_OUT* rackStoreOutStatePtr = AppStateReadRackStoreOutState();
    *rackStoreOutStatePtrPtr = rackStoreOutStatePtr;
    //推手推进一次
    StepMotorRunStepsCmd runStepCmd;
    //初始化走步数结构
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    int32_t paramUtil;
    //旋转到工作位置
    errorCode = AppImplRackStoreOutRotateSwitchToAction(rackStoreOutStatePtrPtr);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //推进轴走步数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_STORE_OUT,APP_PARAM_SUB_RACK_STORE_OUT_PUSH_STEP_SWITCH_TO_STORE,&paramUtil);
    //填充走步数结构
    runStepCmd.motorIndex = RACK_STORE_OUT_SM_PUSH_ID;
    runStepCmd.specialSteps = paramUtil;
    runStepCmd.selectConfig = S_CURVE_RACK_STORE_OUT_SM_PUSH_PUSH;
    //执行一次走步数
    errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
    if(errorCode != LH_ERR_NONE)
    {
        errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_PUSH_MOTOR_PUSH_OUT_SWITCH_TO_STORE,errorCode);
        return errorCode;
    }
    //回退一次
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //推进轴走步数
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_STORE_OUT,APP_PARAM_SUB_RACK_STORE_OUT_PUSH_ONCE_STEP,&paramUtil);
    //填充走步数结构
    runStepCmd.motorIndex = RACK_STORE_OUT_SM_PUSH_ID;
    runStepCmd.specialSteps = 0 - paramUtil;
    runStepCmd.selectConfig = S_CURVE_RACK_STORE_OUT_SM_PUSH_PUSH;
    //执行一次走步数
    errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
    if(errorCode != LH_ERR_NONE)
    {
        errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_PUSH_MOTOR_BACK_ONCE,errorCode);
        return errorCode;
    }
    //回零
    errorCode = AppImplRackStoreOutReturnZero(rackStoreOutStatePtrPtr);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //运行到准备位置
    StepMotorRun2CoordationCmd runToPosCmd;
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_STORE_OUT,APP_PARAM_SUB_RACK_STORE_OUT_PUSH_START_POS,&paramUtil);
    //初始化走目标坐标结构
    IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
    //填充走坐标结构体
    runToPosCmd.motorIndex = RACK_STORE_OUT_SM_PUSH_ID;//电机序号
    runToPosCmd.targetCoordinate = paramUtil;//目标坐标
    runToPosCmd.selectConfig = S_CURVE_RACK_STORE_OUT_SM_PUSH_TO_PREPARE;//速度曲线
    runToPosCmd.timeOutSet = 60000;
    runToPosCmd.utilStop1Enable = 0;
    //走坐标
    errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //运行到指定坐标失败
        errorCode = AppImplRackStoreOutConvertErrorCode(FLAG_RACK_STORE_OUT_ERR_PUSH_MOTOR_RUN_PREPARE,errorCode);
        return errorCode;
    }
    //更新出口换轨信息
    AppImplRackOutSwitchReadState(rackOutSwitchStatePtrPtr);
    return errorCode;
}

//获取出样仓缓存讯息
LH_ERR AppImplRackStoreOutReadState(APP_STATE_RACK_STORE_OUT** rackStoreOutStatePtrPtr,APP_STATE_RACK_OUT_SWITCH** rackOutSwitchStatePtrPtr)
{
    APP_STATE_RACK_STORE_OUT* rackStoreOutStatePtr = AppStateReadRackStoreOutState();
    *rackStoreOutStatePtrPtr = rackStoreOutStatePtr;
    //更新传感器讯息
    AppImplRackStoreOutReflushAllSensor(&(rackStoreOutStatePtr->rackStoreOutSensorSet));
    AppImplRackOutSwitchReadState(rackOutSwitchStatePtrPtr);
    //回传无错误
    return LH_ERR_NONE;
}

//清除出样仓缓存讯息
LH_ERR AppImplRackStoreOutClearState(void)
{
    APP_STATE_RACK_STORE_OUT* rackStoreOutStatePtr = AppStateReadRackStoreOutState();
    //更新传感器讯息
    AppImplRackStoreOutReflushAllSensor(&(rackStoreOutStatePtr->rackStoreOutSensorSet));
    //回传无错误
    return LH_ERR_NONE;
}

//获取最大推进次数
LH_ERR AppImplRackStoreOutGetPushMax(uint8_t* pushCountMaxTaryNearKey,uint8_t* pushCountMaxTaryNearRail)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t paramUtil;
    //检查推进上限
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_STORE_OUT,APP_PARAM_SUB_RACK_STORE_OUT_PUSH_COUNT_MAX_TARY_NEAR_KEY,&paramUtil);
    //开始推进
    *pushCountMaxTaryNearKey = (uint8_t)(paramUtil);
    AppImplParamUtilReadParam(APP_PARAM_MAIN_RACK_STORE_OUT,APP_PARAM_SUB_RACK_STORE_OUT_PUSH_COUNT_MAX_TARY_NEAR_RAIL,&paramUtil);
    *pushCountMaxTaryNearRail = (uint8_t)(paramUtil);
    return errorCode;
}


