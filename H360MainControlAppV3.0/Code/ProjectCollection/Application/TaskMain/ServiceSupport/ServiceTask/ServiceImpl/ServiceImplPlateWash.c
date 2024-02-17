/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 13:58:44
**LastEditors: DengXiaoJun
**LastEditTime: 2020-08-18 14:45:55
**FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplPlateWash.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplPlateWash.h"
#include "ServiceStatePrivate.h"

//模块输入配置映射,前一个是系统序号,后一个是有效电平
static const MODULE_INPUT_MAP_UNIT modulePlateWashInputMapArray[] = {
    {INPUT_PLATE_WASH_ROTATE_ORGIN_SENSOR       ,VALID_LEVEL_INPUT_PLATE_WASH_ROTATE_ORGIN_SENSOR },//旋转电机原点
    {INPUT_PLATE_WASH_NEW_CUP_SENSOR            ,VALID_LEVEL_INPUT_PLATE_WASH_NEW_CUP_SENSOR      },//新杯检测光纤
};
//配置数组长度
#define MODULE_PLATE_WASH_INPUT_MAP_ARRAY_LENGTH   (sizeof(modulePlateWashInputMapArray)/sizeof(modulePlateWashInputMapArray[0]))
/*******************************************************************
**description: 读取指定输入 
******************************************************************/
static LH_ERR ServiceImplPlateWash_ModuleInputRead(INPUT_PLATE_WASH_NAME name,SENSOR_STATE* state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //输入读取
        errorCode = ServiceImplModuleInputRead((uint8_t)name,MODULE_PLATE_WASH_INPUT_MAP_ARRAY_LENGTH,(MODULE_INPUT_MAP_UNIT*)modulePlateWashInputMapArray,state);
        if(errorCode != LH_ERR_NONE)
        {
            if(errorCode == LH_ERR_BOARD_MAIN_COMM_INPUT_PIN_INDEX)
            {
                *commErrorCode = CAN1_REPORT_ERR_PLATE_WASH_INPUT_INDEX;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
            else
            {
                *commErrorCode = CAN1_REPORT_ERR_PLATE_WASH_INPUT_READ;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
        }
    return errorCode;
}

//模块输出配置映射
static const MODULE_OUTPUT_MAP_UNIT modulePlateWashOutputMapArray[] = {
    {MODULE_BASE_OUTPUT_PIN_RESERVE    ,VALID_LEVEL_MODULE_BASE_OUTPUT_PIN_RESERVE},
};
//配置数组长度
#define MODULE_PLATE_WASH_OUTPUT_MAP_UNIT_LENGTH   (sizeof(modulePlateWashOutputMapArray)/sizeof(modulePlateWashOutputMapArray[0]))
/*******************************************************************
**description: 写入指定输出 
******************************************************************/
static LH_ERR ServiceImplPlateWash_ModuleOutputWrite(OUTPUT_PLATE_WASH_NAME name,OUT_STATE state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //输出写入
        errorCode = ServiceImplModuleOutputWrite((uint8_t)name,state,MODULE_PLATE_WASH_OUTPUT_MAP_UNIT_LENGTH,(MODULE_OUTPUT_MAP_UNIT*)modulePlateWashOutputMapArray);
        if(errorCode != LH_ERR_NONE)
        {
            if(errorCode == LH_ERR_BOARD_MAIN_COMM_OUTPUT_PIN_INDEX)
            {
                *commErrorCode = CAN1_REPORT_ERR_PLATE_WASH_OUTPUT_INDEX;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
            else
            {
                *commErrorCode = CAN1_REPORT_ERR_PLATE_WASH_OUTPUT_WRITE;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
            return errorCode;
        }
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 模块运行前的检查,检查当前模块可不可以运动
**param: mode 检查模式
**return: 
**Author: DengXiaoJun
**LastEditors: DengXiaoJun
**LastEditTime: 
**LastModifyRecord: 
******************************************************************/
static LH_ERR ServiceImplPlateWash_ModuleSafeCheckBeforeRun(uint8_t mode,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查清洗混匀状态
        if(MIX_SAFE_EFFECTIVE == ServiceStateReadMixWashSafeState())
        {
            errorCode = LH_ERR_BOARD_MAIN_PLATE_WASH_SAFE_PROTECT;
            *commErrorCode = CAN1_REPORT_ERR_PLATE_WASH_MODULE_MIX_WASH_SAFE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            return errorCode;
        }
    //检查清洗针
        if(NEEDLE_SAFE_EFFECTIVE_PLATE_WASH == ServiceStateReadNeedleWashSafeState())
        {
            errorCode = LH_ERR_BOARD_MAIN_PLATE_WASH_SAFE_PROTECT;
            *commErrorCode = CAN1_REPORT_ERR_PLATE_WASH_MODULE_NEEDLE_WASH_SAFE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            return errorCode;
        }
    //检查缓冲液注液
        if(MODULE_SAFE_EFFECTIVE == ServiceStateReadBufferSolutionInjectSafeState())
        {
            errorCode = LH_ERR_BOARD_MAIN_PLATE_WASH_SAFE_PROTECT;
            *commErrorCode = CAN1_REPORT_ERR_PLATE_WASH_MODULE_BUFFER_INJECT_SAFE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            return errorCode;
        }
    //检查清洗机械手
        if(HAND_SAFE_EFFECTIVE_WASH == ServiceStateReadHandWashSafeState())
        {
            errorCode = LH_ERR_BOARD_MAIN_PLATE_WASH_SAFE_PROTECT;
            *commErrorCode = CAN1_REPORT_ERR_PLATE_WASH_MODULE_HAND_WASH_SAFE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            return errorCode;
        }
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 自动运行状态机初始化 
******************************************************************/
LH_ERR ServiceImplPlateWash_StateMachineInit(void)
{
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 自动运行状态机轮转 
******************************************************************/
LH_ERR ServiceImplPlateWash_StateMachineLoop(void)
{
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 清洗盘复位 
******************************************************************/
LH_ERR ServiceImplPlateWash_ResetAll(uint8_t* plateWashIndex,SENSOR_STATE* cupPlateWashExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //安全状态判定
        errorCode = ServiceImplPlateWash_ModuleSafeCheckBeforeRun(0,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //写入清洗盘保护
        ServiceStateWritePlateWashSafeState(PLATE_SAFE_EFFECTIVE);
    //复位
        errorCode = Can2SubBoard1PlateWash_Reset(cupPlateWashExist,plateWashIndex);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_PLATE_WASH_RESET;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //设置状态
        ServiceStateWritePlateWashIndex(*plateWashIndex);
        ServiceStateWritePlateWashCupExist(*cupPlateWashExist);
    //关闭清洗盘保护
        ServiceStateWritePlateWashSafeState(PLATE_SAFE_INVALID);
    return errorCode;
}

/*******************************************************************
**description: 清洗盘移动指定数量个杯位 
******************************************************************/
LH_ERR ServiceImplPlateWash_MoveAnyCups(uint8_t cups,uint8_t* plateWashIndex,SENSOR_STATE* cupPlateWashExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //安全状态判定
        errorCode = ServiceImplPlateWash_ModuleSafeCheckBeforeRun(0,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //写入清洗盘保护
        ServiceStateWritePlateWashSafeState(PLATE_SAFE_EFFECTIVE);
    //走孔位数量
        errorCode = Can2SubBoard1PlateWash_RunAnyCups(cups,cupPlateWashExist,plateWashIndex);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_PLATE_WASH_RUN_ANY_CUP;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //设置状态
        ServiceStateWritePlateWashIndex(*plateWashIndex);
        ServiceStateWritePlateWashCupExist(*cupPlateWashExist);
    //关闭清洗盘保护
        ServiceStateWritePlateWashSafeState(PLATE_SAFE_INVALID);
    return errorCode;
}

/*******************************************************************
**description: 清洗盘移动到指定序号的位置 
******************************************************************/
LH_ERR ServiceImplPlateWash_Move2Index(uint8_t targetCupIndex,uint8_t* plateWashIndex,SENSOR_STATE* cupPlateWashExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //安全状态判定
        errorCode = ServiceImplPlateWash_ModuleSafeCheckBeforeRun(0,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //写入清洗盘保护
        ServiceStateWritePlateWashSafeState(PLATE_SAFE_EFFECTIVE);
    //走孔位数量
        errorCode = Can2SubBoard1PlateWash_Run2CupWithIndex(targetCupIndex,cupPlateWashExist,plateWashIndex);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_PLATE_WASH_RUN_TO_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //设置状态
        ServiceStateWritePlateWashIndex(*plateWashIndex);
        ServiceStateWritePlateWashCupExist(*cupPlateWashExist);
    //关闭清洗盘保护
        ServiceStateWritePlateWashSafeState(PLATE_SAFE_INVALID);
    return errorCode;
}

/*******************************************************************
**description: 读取模块内部指定输入的状态 
******************************************************************/
LH_ERR ServiceImplPlateWash_InputRead(uint8_t inputIndex,SENSOR_STATE* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = ServiceImplPlateWash_ModuleInputRead((INPUT_PLATE_WASH_NAME)inputIndex,inputState,commErrorCode,commErrorLevel);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
    }
    return errorCode;
}

/*******************************************************************
**description: 写入模块指定输出的状态 
******************************************************************/
LH_ERR ServiceImplPlateWash_OutputWrite(uint8_t outputIndex,OUT_STATE setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = ServiceImplPlateWash_ModuleOutputWrite((OUTPUT_PLATE_WASH_NAME)outputIndex,setState,commErrorCode,commErrorLevel);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
    }
    return errorCode;
}

/*******************************************************************
**description: 指定模块特定电机复位 
******************************************************************/
LH_ERR ServiceImplPlateWash_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //判定参数,主要是电机序号
        if(motorIndex > INDEX_MOTOR_PLATE_WASH_ROTATE)
        {
            errorCode = LH_ERR_BOARD_MAIN_PLATE_WASH_MOTOR_INDEX;
            *commErrorCode = CAN1_REPORT_ERR_PLATE_WASH_STEP_MOTOR_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //指定步进电机复位带复位修正
        INDEX_MOTOR_PLATE_WASH stepMotorIndex = (INDEX_MOTOR_PLATE_WASH)(motorIndex);
        errorCode = Can2SubBoard1PlateWash_StepMotorReset(stepMotorIndex);
    //读取失败,错误处理
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_PLATE_WASH_STEP_MOTOR_SINGLE_RESET;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //返回数据
        return errorCode;
}

/*******************************************************************
**description: 指定模块特动电机运行指定步数 
******************************************************************/
LH_ERR ServiceImplPlateWash_MotorRunSteps(uint8_t motorIndex,int32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //判定参数,主要是电机序号
        if(motorIndex > INDEX_MOTOR_PLATE_WASH_ROTATE)
        {
            errorCode = LH_ERR_BOARD_MAIN_PLATE_WASH_MOTOR_INDEX;
            *commErrorCode = CAN1_REPORT_ERR_PLATE_WASH_STEP_MOTOR_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //清洗盘电机走步数
        INDEX_MOTOR_PLATE_WASH stepMotorIndex = (INDEX_MOTOR_PLATE_WASH)(motorIndex);
        errorCode = Can2SubBoard1PlateWash_StepMotorRunSteps(stepMotorIndex,steps,posAfterRun);
    //读取失败,错误处理
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_PLATE_WASH_STEP_MOTOR_SINGLE_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //返回数据
        return errorCode;
}




























