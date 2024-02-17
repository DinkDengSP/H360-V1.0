/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-17 10:52:14
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-16 11:26:02
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplTubeBarScan.h"

typedef enum FLAG_TUBE_BAR_SCAN_ERR_CONVERT
{
    FLAG_TUBE_BAR_SCAN_ERR_READ_INPUT_INDEX             = 0x01,//读输入引脚索引错误
    FLAG_TUBE_BAR_SCAN_ERR_READ_INPUT                   = 0x02,//读输入引脚错误
    FLAG_TUBE_BAR_SCAN_ERR_WITE_OUTPUT_INDEX            = 0x03,//写输出引脚索引错误
    FLAG_TUBE_BAR_SCAN_ERR_WITE_OUTPUT                  = 0x04,//写输出引脚错误
    FLAG_TUBE_BAR_SCAN_ERR_STEP_MOTOR_INDEX             = 0x05,//电机索引错误
    FLAG_TUBE_BAR_SCAN_ERR_STEP_MOTOR_RESET             = 0x06,//电机复位错误
    FLAG_TUBE_BAR_SCAN_ERR_STEP_MOTOR_RUN_STEP          = 0x07,//电机走步数错误
    FLAG_TUBE_BAR_SCAN_ERR_READ_STEP_MOTOR_COORDINATE   = 0X08,//读电机坐标错误
    FLAG_TUBE_BAR_SCAN_ERR_BARSCAN_NOT_EXIST            = 0X09,//试管条码枪不存在
    FLAG_TUBE_BAR_SCAN_ERR_STEP_MOTOR_RUN_ALWAYS        = 0X0A,//电机持续运行产生的错误
    FLAG_TUBE_BAR_SCAN_ERR_STEP_MOTOR_DECELERATION_STOP = 0X0B,//电机减速停止产生的错误
}FLAG_TUBE_BAR_SCAN_ERR_CONVERT;

//输入引脚配置
const static INPUT_SENSOR_PIN_CONFIG inputSensorPinConfigArray[] = {
    {INTPUT_COMMON_INTPUT_ENABLE,              VAILD_INTPUT_COMMON_INTPUT_ENABLE},//保留
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
    {TUBE_BAR_SCAN_SM_ROTATE,        APP_PARAM_MAIN_TUBE_BAR_SCAN,             0},//扫码节点电机
};
//电机配置数组长度
#define LENGTH_MODULE_STEP_MOTOR_RESET_RUN_CONFIG_ARRARY    (sizeof(moduleStepMotorResetRunConfigArray)/(sizeof(moduleStepMotorResetRunConfigArray[0])))


//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplTubeBarScanConvertErrorCode(FLAG_TUBE_BAR_SCAN_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_TUBE_BAR_SACN;
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
LH_ERR AppImplTubeBarScanReadInput(RAIL_ONLINE_TUBE_BAR_SCAN_INPUT_INDEX readIndex,SENSOR_STATE *sensorValuePtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(readIndex) >= LENGTH_INPUT_SENSOR_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplTubeBarScanConvertErrorCode(FLAG_TUBE_BAR_SCAN_ERR_READ_INPUT_INDEX,errorCode);
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
LH_ERR AppImplTubeBarScanWriteOutput(RAIL_ONLINE_TUBE_BAR_SCAN_OUTPUT_INDEX WriteIndex,OUT_STATE outState)
{
    LH_ERR errorCode = LH_ERR_NONE;
    BitAction pinValue = Bit_RESET;
    //索引判断
    if((uint8_t)(WriteIndex) >= LENGTH_OUTPUT_CTRL_PIN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplTubeBarScanConvertErrorCode(FLAG_TUBE_BAR_SCAN_ERR_WITE_OUTPUT_INDEX,errorCode);
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
LH_ERR AppImplTubeBarScanStepMotorReset(RAIL_ONLINE_TUBE_BAR_SCAN_SM_INDEX motorIndex,int32_t *posAfterResetPtr)
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
        errorCode = AppImplTubeBarScanConvertErrorCode(FLAG_TUBE_BAR_SCAN_ERR_STEP_MOTOR_INDEX,errorCode);
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
            errorCode = AppImplTubeBarScanConvertErrorCode(FLAG_TUBE_BAR_SCAN_ERR_STEP_MOTOR_RESET,errorCode);
            AppShowLogError(errorCode);
            return errorCode;
        }
    //读取坐标
        errorCode = IPC_StepMotorReflushStateWhileReturn(resetCmd.motorIndex,posAfterResetPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码转换
            errorCode = AppImplTubeBarScanConvertErrorCode(FLAG_TUBE_BAR_SCAN_ERR_READ_STEP_MOTOR_COORDINATE,errorCode);
            AppShowLogError(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}

//电机走步数
LH_ERR AppImplTubeBarScanStepMotorRunSteps(RAIL_ONLINE_TUBE_BAR_SCAN_SM_INDEX motorIndex,int32_t stepToRun,int32_t *posAfterRunPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    IPC_StepMotorUtilRunStepsCmdDataStructInit(&runStepCmd);
    //电机索引判断
    if((uint8_t)(motorIndex) >= LENGTH_MODULE_STEP_MOTOR_RESET_RUN_CONFIG_ARRARY)
    {
        //错误代码转换
        errorCode = AppImplTubeBarScanConvertErrorCode(FLAG_TUBE_BAR_SCAN_ERR_STEP_MOTOR_INDEX,errorCode);
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
            errorCode = AppImplTubeBarScanConvertErrorCode(FLAG_TUBE_BAR_SCAN_ERR_STEP_MOTOR_RUN_STEP,errorCode);
            AppShowLogError(errorCode);
            return errorCode;
        }
    //读取坐标
        errorCode = IPC_StepMotorReflushStateWhileReturn(runStepCmd.motorIndex,posAfterRunPtr);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码转换
            errorCode = AppImplTubeBarScanConvertErrorCode(FLAG_TUBE_BAR_SCAN_ERR_READ_STEP_MOTOR_COORDINATE,errorCode);
            AppShowLogError(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}


//初始化条码状态机
void AppImplTubeBarScanFSM_Init(void)
{
    asm("nop");
}

//运行条码扫描状态机
void AppImplTubeBarScanFSM_Run(void)
{
    asm("nop");
}

//条码扫描复位
LH_ERR AppImplTubeBarScanReset(APP_STATE_TUBE_BAR_SCAN** tubeBarScanStatePtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    APP_STATE_TUBE_BAR_SCAN* tubeBarScanStatePtr = AppStateGetTubeBarScanStatePtr();
    *tubeBarScanStatePtrPtr = tubeBarScanStatePtr;
    //初始化电机复位参数
    StepMotorResetCmd resetCmd;
    IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
    //填充参数
    resetCmd.motorIndex = TUBE_BAR_SCAN_SM_ROTATE;//电机序号
    resetCmd.correctionCoordinate = 0;//复位修正
    resetCmd.timeOutSet = 60000;//超时时间
    //开始复位
    errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplTubeBarScanConvertErrorCode(FLAG_TUBE_BAR_SCAN_ERR_STEP_MOTOR_RESET,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //检查条码枪是否存在
    errorCode = BoardBarScanCR100M_CheckExist();
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplTubeBarScanConvertErrorCode(FLAG_TUBE_BAR_SCAN_ERR_BARSCAN_NOT_EXIST,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //条码枪关闭
    BoardBarScanCR100M_Close();
    return LH_ERR_NONE;
}

//执行一次扫码,并返回结果
LH_ERR AppImplTubeBarScanTrigOnce(APP_STATE_TUBE_BAR_SCAN** tubeBarScanStatePtrPtr,uint16_t* barcodeLengthPtr,uint8_t** barcodeBufferPtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    OS_ERR osErr = OS_ERR_NONE;
    APP_STATE_TUBE_BAR_SCAN* tubeBarScanStatePtr = AppStateGetTubeBarScanStatePtr();
    *tubeBarScanStatePtrPtr = tubeBarScanStatePtr;
    //参数初始化
    uint32_t startTime = 0;
    //读取扫码数据长度
    *barcodeLengthPtr = 0;
    *barcodeBufferPtrPtr = NULL;
    StepMotorRunAlwaysCmd runAlwaysCmd;
    IPC_StepMotorUtilRunAlwaysDataStructInit(&runAlwaysCmd);
    //打开条码枪，单次扫码
    BoardBarScanCR100M_OpenOnce();
    //记录开始时间
    startTime = OSTimeGet(&osErr);
    //旋转电机持续运行
    runAlwaysCmd.motorIndex = TUBE_BAR_SCAN_SM_ROTATE;
    runAlwaysCmd.selectConfig = S_CURVE_TUBE_BAR_SCAN_ROTATE_SCAN;
    runAlwaysCmd.dir = BOARD_CPLD_MOTOR_DIR_CW;
    errorCode = IPC_StepMotorRunAlwaysWhileReturn(&runAlwaysCmd);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplTubeBarScanConvertErrorCode(FLAG_TUBE_BAR_SCAN_ERR_STEP_MOTOR_RUN_ALWAYS,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //持续等待扫码结果
    do
    {
        if(BoardBarScanCR100M_IsBarScanFinish())
        {
            break;
        }
        CoreDelayMsSensitivity(10);
    }while ((OSTimeGet(&osErr) - startTime) < TIME_BAR_SCAN_ONCE_MAX_DURTION_TIME_MS);
    //扫码成功，复制扫码数据
    if(BoardBarScanCR100M_IsBarScanFinish())
    {
        //获取扫码数据长度
        *barcodeLengthPtr = BoardBarScanCR100M_ReadLastRecvDataLength();
        //内存申请
        *barcodeBufferPtrPtr = UserMemMallocWhileSuccess(SRAM_IN,*barcodeLengthPtr);
        //读扫码结果
        BoardBarScanCR100M_ReadLastRecvData(*barcodeBufferPtrPtr,*barcodeLengthPtr);
    }
    //关闭条码枪
    BoardBarScanCR100M_Close();
    //旋转电机停止
    errorCode = IPC_StepMotorStopDecelerationWhileReturn(TUBE_BAR_SCAN_SM_ROTATE);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码转换
        errorCode = AppImplTubeBarScanConvertErrorCode(FLAG_TUBE_BAR_SCAN_ERR_STEP_MOTOR_DECELERATION_STOP,errorCode);
        AppShowLogError(errorCode);
        return errorCode;
    }
    //返回
    return LH_ERR_NONE;
}

//单次打开条码枪
LH_ERR AppImplTubeBarScanScannerOpenOnce(uint8_t openTimeSecond,APP_STATE_TUBE_BAR_SCAN** tubeBarScanStatePtrPtr,uint16_t* barcodeLengthPtr,uint8_t** barcodeBufferPtrPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    OS_ERR osErr = OS_ERR_NONE;
    APP_STATE_TUBE_BAR_SCAN* tubeBarScanStatePtr = AppStateGetTubeBarScanStatePtr();
    *tubeBarScanStatePtrPtr = tubeBarScanStatePtr;
    uint32_t startTime = 0;
    *barcodeLengthPtr = 0;
    *barcodeBufferPtrPtr = NULL;
    //打开条码枪，单次扫码
    errorCode = BoardBarScanCR100M_OpenOnce();
    //记录开始时间
    startTime = OSTimeGet(&osErr);
    //持续等待扫码结果
    do
    {
        if(BoardBarScanCR100M_IsBarScanFinish())
        {
            break;
        }
        CoreDelayMs(10);
    }while ((OSTimeGet(&osErr) - startTime) < openTimeSecond*1000);
    //获取数据长度
    if(BoardBarScanCR100M_IsBarScanFinish() != 0)
    {
        //扫码成功，获取长度
        *barcodeLengthPtr = BoardBarScanCR100M_ReadLastRecvDataLength();
        //申请条码空间
        *barcodeBufferPtrPtr = UserMemMallocWhileSuccess(SRAM_IN,*barcodeLengthPtr);
        //数据清空
        UserMemSet(*barcodeBufferPtrPtr,0,*barcodeLengthPtr);
        //读扫码结果
        BoardBarScanCR100M_ReadLastRecvData(*barcodeBufferPtrPtr,*barcodeLengthPtr);
    }
    //关闭条码枪
    BoardBarScanCR100M_Close();
    return errorCode;
}

//持续打开条码枪
LH_ERR AppImplTubeBarScanScannerOpenAlways(APP_STATE_TUBE_BAR_SCAN** tubeBarScanStatePtrPtr)
{
    APP_STATE_TUBE_BAR_SCAN* tubeBarScanStatePtr = AppStateGetTubeBarScanStatePtr();
    *tubeBarScanStatePtrPtr = tubeBarScanStatePtr;
    BoardBarScanCR100M_OpenContinue();
    return LH_ERR_NONE;
}

//关闭条码枪
LH_ERR AppImplTubeBarScanScannerClose(APP_STATE_TUBE_BAR_SCAN** tubeBarScanStatePtrPtr)
{
    APP_STATE_TUBE_BAR_SCAN* tubeBarScanStatePtr = AppStateGetTubeBarScanStatePtr();
    *tubeBarScanStatePtrPtr = tubeBarScanStatePtr;
    BoardBarScanCR100M_Close(); 
    return LH_ERR_NONE;
}

//读缓存状态
LH_ERR AppImplTubeBarScanReadState(APP_STATE_TUBE_BAR_SCAN** tubeBarScanStatePtrPtr)
{
    APP_STATE_TUBE_BAR_SCAN* tubeBarScanStatePtr = AppStateGetTubeBarScanStatePtr();
    *tubeBarScanStatePtrPtr = tubeBarScanStatePtr;
     return LH_ERR_NONE;
}

//清除缓存状态
LH_ERR AppImplTubeBarScanClearState(void)
{
    asm("nop");
    return LH_ERR_NONE;
}



