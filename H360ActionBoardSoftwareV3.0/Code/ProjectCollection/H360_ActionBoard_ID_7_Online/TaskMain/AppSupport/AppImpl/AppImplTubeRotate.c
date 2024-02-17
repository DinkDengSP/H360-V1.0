/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-17 10:51:14
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-16 14:21:51
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplTubeRotate.h"

typedef enum FLAG_TUBE_ROTATE_ERR_CONVERT
{
    FLAG_TUBE_ROTATE_ERR_READ_INPUT_INDEX                         = 0x01,//读输入引脚索引错误
    FLAG_TUBE_ROTATE_ERR_READ_INPUT                               = 0x02,//读输入引脚错误
    FLAG_TUBE_ROTATE_ERR_WITE_OUTPUT_INDEX                        = 0x03,//写输出引脚索引错误
    FLAG_TUBE_ROTATE_ERR_WITE_OUTPUT                              = 0x04,//写输出引脚错误
    FLAG_TUBE_ROTATE_ERR_STEP_MOTOR_INDEX                         = 0x05,//电机索引错误
    FLAG_TUBE_ROTATE_ERR_STEP_MOTOR_RESET                         = 0x06,//电机复位错误
    FLAG_TUBE_ROTATE_ERR_STEP_MOTOR_RUN_STEP                      = 0x07,//电机走步数错误
    FLAG_TUBE_ROTATE_ERR_READ_STEP_MOTOR_COORDINATE               = 0X08,//读电机坐标错误
    FLAG_TUBE_ROTATE_ERR_READ_PARAM                               = 0X09,//旋转节点读参数产生的错误
    FLAG_TUBE_ROTATE_ERR_TUBE_ROTATE_CONDITION_NOT_MET            = 0X0A,//旋转节点旋转旋转条件不满足
    FLAG_TUBE_ROTATE_ERR_RESET_TUBE_CLAMP_NOT_RELEASE             = 0X0B,//旋转节点复位，试管固定夹爪未释放
    FLAG_TUBE_ROTATE_ERR_RESET_OUTPUT_TUBE_BLOCK                  = 0X0C,//旋转节点复位，出口试管阻塞。
    FLAG_TUBE_ROTATE_ERR_RESET_CLEAR_UP_COUNT_LIMIT               = 0X0D,//旋转节点复位，清空次数超过限制
}FLAG_TUBE_ROTATE_ERR_CONVERT;

//输入引脚配置
const static INPUT_SENSOR_PIN_CONFIG inputSensorPinConfigArray[] = {
    {TUBE_ENTRY_RAIL_ONLINE_TUBE_ROTATE_SM_ORIGIN_SENSOR, VAILD_TUBE_ENTRY_RAIL_ONLINE_TUBE_ROTATE_SM_ORIGIN_SENSOR },//旋转节点电机原点传感器
    {INPUT_TUBE_ROTATE_INPUT_POS_SENSOR                 , VAILD_INPUT_TUBE_ROTATE_INPUT_POS_SENSOR                  },//旋转节点输入位传感器
    {INPUT_TUBE_ROTATE_BARSCAN_POS_SENSOR               , VAILD_INPUT_TUBE_ROTATE_BARSCAN_POS_SENSOR                },//旋转节点扫码位传感器
    {INPUT_TUBE_ROTATE_TEST_POS_SENSOR                  , VAILD_INPUT_TUBE_ROTATE_TEST_POS_SENSOR                   },//旋转节点测试位传感器
    {INPUT_TUBE_ROTATE_OUTPUT_POS_SENSOR                , VAILD_INPUT_TUBE_ROTATE_OUTPUT_POS_SENSOR                 },//旋转节点输出位传感器
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
    {TUBE_ROTATE_SM_MAIN,        APP_PARAM_MAIN_TUBE_ROTATE,           APP_PARAM_SUB_TUBE_ROTATE_INDEX_STEP_MOTOR_RESET_CORRECT_STEP},//旋转节点电机
};
//电机配置数组长度
#define LENGTH_MODULE_STEP_MOTOR_RESET_RUN_CONFIG_ARRARY    (sizeof(moduleStepMotorResetRunConfigArray)/(sizeof(moduleStepMotorResetRunConfigArray[0])))


//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplTubeRotateConvertErrorCode(FLAG_TUBE_ROTATE_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_TUBE_ROTATE;
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
LH_ERR AppImplTubeRotateReadInput(RAIL_ONLINE_TUBE_ROTATE_INPUT_INDEX readIndex,SENSOR_STATE *sensorValuePtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(readIndex) >= LENGTH_INPUT_SENSOR_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplTubeRotateConvertErrorCode(FLAG_TUBE_ROTATE_ERR_READ_INPUT_INDEX,errorCode);
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
LH_ERR AppImplTubeRotateWriteOutput(RAIL_ONLINE_TUBE_ROTATE_OUTPUT_INDEX WriteIndex,OUT_STATE outState)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(WriteIndex) >= LENGTH_OUTPUT_CTRL_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplTubeRotateConvertErrorCode(FLAG_TUBE_ROTATE_ERR_WITE_OUTPUT_INDEX,errorCode);
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
LH_ERR AppImplTubeRotateStepMotorReset(RAIL_ONLINE_TUBE_ROTATE_SM_INDEX motorIndex,int32_t *posAfterResetPtr)
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
        errorCode = AppImplTubeRotateConvertErrorCode(FLAG_TUBE_ROTATE_ERR_STEP_MOTOR_INDEX,errorCode);
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
        errorCode = AppImplTubeRotateConvertErrorCode(FLAG_TUBE_ROTATE_ERR_STEP_MOTOR_RESET,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //读取坐标
    errorCode = IPC_StepMotorReflushStateWhileReturn(resetCmd.motorIndex,posAfterResetPtr);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplTubeRotateConvertErrorCode(FLAG_TUBE_ROTATE_ERR_READ_STEP_MOTOR_COORDINATE,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    return LH_ERR_NONE;
}

//电机走步数
LH_ERR AppImplTubeRotateStepMotorRunSteps(RAIL_ONLINE_TUBE_ROTATE_SM_INDEX motorIndex,int32_t stepToRun,int32_t *posAfterRunPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //电机索引判断
    if((uint8_t)(motorIndex) >= LENGTH_MODULE_STEP_MOTOR_RESET_RUN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplTubeRotateConvertErrorCode(FLAG_TUBE_ROTATE_ERR_STEP_MOTOR_INDEX,errorCode);
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
        errorCode = AppImplTubeRotateConvertErrorCode(FLAG_TUBE_ROTATE_ERR_STEP_MOTOR_RUN_STEP,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //读取坐标
    errorCode = IPC_StepMotorReflushStateWhileReturn(runStepCmd.motorIndex,posAfterRunPtr);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplTubeRotateConvertErrorCode(FLAG_TUBE_ROTATE_ERR_READ_STEP_MOTOR_COORDINATE,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    return LH_ERR_NONE;
}

//初始化转盘状态机
void AppImplTubeRotateFSM_Init(void)
{
    asm("nop");
}

//运行转盘检测状态机
void AppImplTubeRotateFSM_Run(void)
{
    asm("nop");
}

//查询转盘传感器状态
static LH_ERR AppImplTubeRotateReflushAllSensor(TUBE_ROTATE_SENSOR_SET* tubeRotateSensorSetPtr)
{
    //读取四个传感器状态，
    //入口传感器
    AppImplTubeRotateReadInput(RAIL_ONLINE_TUBE_ROTATE_INPUT_INDEX_INPUT_POS_SENSOR,&(tubeRotateSensorSetPtr->inputState));
    //扫码旋转
    AppImplTubeRotateReadInput(RAIL_ONLINE_TUBE_ROTATE_INPUT_INDEX_BARSCAN_POS_SENSOR,&(tubeRotateSensorSetPtr->barScanState));
    //测试位
    AppImplTubeRotateReadInput(RAIL_ONLINE_TUBE_ROTATE_INPUT_INDEX_TEST_POS_SENSOR,&(tubeRotateSensorSetPtr->testState));
    //出口
    AppImplTubeRotateReadInput(RAIL_ONLINE_TUBE_ROTATE_INPUT_INDEX_OUTPUT_POS_SENSOR,&(tubeRotateSensorSetPtr->outputState));
    return LH_ERR_NONE;
}

//检查转盘是否满足旋转条件,不满足旋转条件则不能旋转
static LH_ERR AppImplTubeRotateQueryActCondition(CONDITION_RESULT* conditionResultPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    SENSOR_STATE outputstate = SENSOR_STATE_INVALID;
    //检查旋转条件，读取出口传感器状态
    errorCode = AppImplTubeRotateReadInput(RAIL_ONLINE_TUBE_ROTATE_INPUT_INDEX_OUTPUT_POS_SENSOR,&outputstate);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    //检查是否有效
    if(outputstate == SENSOR_STATE_VALID)
    {
       *conditionResultPtr = CONDITION_FAIL;
       return LH_ERR_NONE;
    }
    //检查测试试管固定机构的状态
    if(AppStatetTubeClampReadTubeClampState() != TUBE_CLAMP_RELEASE)
    {
        *conditionResultPtr = CONDITION_FAIL;
        return LH_ERR_NONE;
    }
    //到此满足
     *conditionResultPtr = CONDITION_OK;
    //返回
    return LH_ERR_NONE;
}

//转盘复位
LH_ERR AppImplTubeRotateReset(APP_STATE_TUBE_ROTATE** tubeRotateStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_TUBE_ROTATE* tubeRotateSensorSetPtr = AppStateGetTubeRotateStatePtr();
    *tubeRotateStatePtrPtr = tubeRotateSensorSetPtr;
    //参数初始化
    //连续多次检测到出口堵塞的信号
    uint32_t countOutputBlock = 0;
    //连续多次检测不到任何信号的计数器
    uint32_t countClearUpSensorInvalid = 0;
    //清空计数
    uint32_t countClearUp = 0;
    int32_t paramUtil = 0;
    //步进电机复位结构体
    StepMotorResetCmd resetCmd;
    IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
    //步进电机走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //首先取消步进电机急停
    IPC_StepMotorSetStateWhileReturn(TUBE_ROTATE_SM_MAIN,STEP_MOTOR_MAIN_STATE_STOP,STEP_MOTOR_STOP_SUB_NORMAL);
    //检查测试试管固定机构的状态
    if(AppStatetTubeClampReadTubeClampState() != TUBE_CLAMP_RELEASE)
    {
        //错误代码转换,条件不满足
        errorCode = AppImplTubeRotateConvertErrorCode(FLAG_TUBE_ROTATE_ERR_RESET_TUBE_CLAMP_NOT_RELEASE,errorCode);
        AppShowLogError(errorCode);
        return errorCode; 
    }
    //正向旋转直到盘子上没有试管
    while(1)
    {
        //电机正方向走寻找原点
        runStepCmd.motorIndex = TUBE_ROTATE_SM_MAIN;
        runStepCmd.specialSteps = STEPS_TUBE_ROTATE_TUBE_ROTATE_ONCE/8;
        runStepCmd.selectConfig = S_CURVE_TUBE_ROTATE_MAIN_FIND_ORIGIN;
        //电机走固定坐标
        errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码转换
            errorCode = AppImplTubeRotateConvertErrorCode(FLAG_TUBE_ROTATE_ERR_STEP_MOTOR_RUN_STEP,errorCode);
            AppShowLogError(errorCode);
            return errorCode; 
        }
        //读取传感器讯号
        AppImplTubeRotateReflushAllSensor(&(tubeRotateSensorSetPtr->tubeRotateSensorSet));
        //检查输入信号是否全空
        if((tubeRotateSensorSetPtr->tubeRotateSensorSet.inputState == SENSOR_STATE_INVALID)&
            (tubeRotateSensorSetPtr->tubeRotateSensorSet.barScanState == SENSOR_STATE_INVALID)&
            (tubeRotateSensorSetPtr->tubeRotateSensorSet.testState == SENSOR_STATE_INVALID)&
            (tubeRotateSensorSetPtr->tubeRotateSensorSet.outputState == SENSOR_STATE_INVALID))
        {
            countClearUpSensorInvalid++;
        }
        else
        {
            countClearUpSensorInvalid = 0;
        }
        //多次检测到信号不存在表示清空
        if(countClearUpSensorInvalid >= COUNT_ROTATE_SENSOR_INVALID_CLEARUP)
        {
            break;
        }
        //检测出口信号是否堵塞
        if(tubeRotateSensorSetPtr->tubeRotateSensorSet.outputState == SENSOR_STATE_VALID)
        {
            countOutputBlock++;
        }
        else
        {
            countOutputBlock = 0;
        }
        //多次检测到出口信号堵塞
        if(countOutputBlock >= COUNT_ROTATE_SENSOR_BLOCK)
        {
            errorCode = AppImplTubeRotateConvertErrorCode(FLAG_TUBE_ROTATE_ERR_RESET_OUTPUT_TUBE_BLOCK,errorCode);
            AppShowLogError(errorCode);
            return errorCode; 
        }
        //检测清空旋转次数是否超过限制
        countClearUp++;
        if(countClearUp >= COUNT_MAX_ROTATE_CLEARUP)
        {
            errorCode = AppImplTubeRotateConvertErrorCode(FLAG_TUBE_ROTATE_ERR_RESET_CLEAR_UP_COUNT_LIMIT,errorCode);
            AppShowLogError(errorCode);
            return errorCode; 
        }
    }
    //读取复位修正
    errorCode = AppParamReadParam(APP_PARAM_MAIN_TUBE_ROTATE,APP_PARAM_SUB_TUBE_ROTATE_INDEX_STEP_MOTOR_RESET_CORRECT_STEP,&paramUtil);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplTubeRotateConvertErrorCode(FLAG_TUBE_ROTATE_ERR_READ_PARAM,errorCode);
        AppShowLogError(errorCode);
        return errorCode;  
    }
    //填充参数
    resetCmd.motorIndex =TUBE_ROTATE_SM_MAIN;//电机序号
    resetCmd.correctionCoordinate = paramUtil;//复位修正
    resetCmd.timeOutSet = 60000;//超时时间
    //开始复位
    errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplTubeRotateConvertErrorCode(FLAG_TUBE_ROTATE_ERR_STEP_MOTOR_RESET,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //读取四个传感器状态
    AppImplTubeRotateReflushAllSensor(&(tubeRotateSensorSetPtr->tubeRotateSensorSet));
    //返回
    return LH_ERR_NONE;
}

//转盘旋转一格
LH_ERR AppImplTubeRotateOnce(APP_STATE_TUBE_ROTATE** tubeRotateStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_TUBE_ROTATE* tubeRotateSensorSetPtr = AppStateGetTubeRotateStatePtr();
    *tubeRotateStatePtrPtr = tubeRotateSensorSetPtr;
    //参数初始化
    CONDITION_RESULT RoateConditionFlag = CONDITION_FAIL;
    StepMotorRunStepsCmd runStepCmd;
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //检查旋转条件,不满足报错
    errorCode = AppImplTubeRotateQueryActCondition(&RoateConditionFlag);
    if(errorCode != LH_ERR_NONE)
    {
        return errorCode;
    }
    if(RoateConditionFlag == CONDITION_FAIL)
    {
        //错误代码转换,条件不满足
        errorCode = AppImplTubeRotateConvertErrorCode(FLAG_TUBE_ROTATE_ERR_TUBE_ROTATE_CONDITION_NOT_MET,errorCode);
        AppShowLogError(errorCode);
        return errorCode; 
    }
    //参数填充
    runStepCmd.motorIndex = TUBE_ROTATE_SM_MAIN;
    runStepCmd.selectConfig = S_CURVE_TUBE_ROTATE_MAIN_NORMAL;
    runStepCmd.specialSteps = STEPS_TUBE_ROTATE_TUBE_ROTATE_ONCE;
    //电机走固定坐标
    errorCode = IPC_StepMotorRunStepsWhileReturn(&runStepCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplTubeRotateConvertErrorCode(FLAG_TUBE_ROTATE_ERR_STEP_MOTOR_RUN_STEP,errorCode);
        AppShowLogError(errorCode);
        return errorCode; 
    }
    //延时指定时间,等待稳定
    CoreDelayMs(TIME_MS_WAIT_PLATE_STABLE);
    //返回传感器状态
    AppImplTubeRotateReflushAllSensor(&(tubeRotateSensorSetPtr->tubeRotateSensorSet));
    return LH_ERR_NONE;
}

//读传感器事件信息
LH_ERR AppImplTubeRotateReadState(APP_STATE_TUBE_ROTATE** tubeRotateStatePtrPtr)
{
    APP_STATE_TUBE_ROTATE* tubeRotateSensorSetPtr = AppStateGetTubeRotateStatePtr();
    *tubeRotateStatePtrPtr = tubeRotateSensorSetPtr;
    //更新传感器状态
    AppImplTubeRotateReflushAllSensor(&(tubeRotateSensorSetPtr->tubeRotateSensorSet));
    return LH_ERR_NONE;
}

//清除事件信息
LH_ERR AppImplTubeRotateClearState(void)
{
    return LH_ERR_NONE;
}






















