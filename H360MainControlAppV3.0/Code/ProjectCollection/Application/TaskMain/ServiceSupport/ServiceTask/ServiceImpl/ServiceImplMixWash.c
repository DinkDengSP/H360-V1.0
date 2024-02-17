/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 13:55:08
**LastEditors: DengXiaoJun
**LastEditTime: 2020-08-18 15:07:56
**FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplMixWash.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplMixWash.h"
#include "ServiceStatePrivate.h"


//模块输入配置映射,前一个是系统序号,后一个是有效电平
static const MODULE_INPUT_MAP_UNIT moduleMixWashInputMapArray[] = {
    {INPUT_MIX_WASH_UPDOWN_ORGIN_SENSOR       ,VALID_LEVEL_INPUT_MIX_WASH_UPDOWN_ORGIN_SENSOR       },//清洗混匀升降电机远点
    {INPUT_MIX_WASH_ROTATE_ORGIN_SENSOR       ,VALID_LEVEL_INPUT_MIX_WASH_ROTATE_ORGIN_SENSOR       },//清洗混匀旋转电机远点
};
//配置数组长度
#define MODULE_MIX_WASH_INPUT_MAP_ARRAY_LENGTH   (sizeof(moduleMixWashInputMapArray)/sizeof(moduleMixWashInputMapArray[0]))
/*******************************************************************
**description: 读取指定输入 
******************************************************************/
static LH_ERR ServiceImplMixWash_ModuleInputRead(INPUT_MIX_WASH_NAME name,SENSOR_STATE* state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //输入读取
        errorCode = ServiceImplModuleInputRead((uint8_t)name,MODULE_MIX_WASH_INPUT_MAP_ARRAY_LENGTH,(MODULE_INPUT_MAP_UNIT*)moduleMixWashInputMapArray,state);
        if(errorCode != LH_ERR_NONE)
        {
            if(errorCode == LH_ERR_BOARD_MAIN_COMM_INPUT_PIN_INDEX)
            {
                *commErrorCode = CAN1_REPORT_ERR_MIX_WASH_INPUT_INDEX;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
            else
            {
                *commErrorCode = CAN1_REPORT_ERR_MIX_WASH_INPUT_READ;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
        }
    return errorCode;
}

//模块输出配置映射
static const MODULE_OUTPUT_MAP_UNIT moduleMixWashOutputMapArray[] = {
    {MODULE_BASE_OUTPUT_PIN_RESERVE    ,VALID_LEVEL_MODULE_BASE_OUTPUT_PIN_RESERVE},//占位符输出
};
//配置数组长度
#define MODULE_MIX_WASH_OUTPUT_MAP_UNIT_LENGTH   (sizeof(moduleMixWashOutputMapArray)/sizeof(moduleMixWashOutputMapArray[0]))
/*******************************************************************
**description: 写入指定输出 
******************************************************************/
static LH_ERR ServiceImplMixWash_ModuleOutputWrite(OUTPUT_MIX_WASH_NAME name,OUT_STATE state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //输出写入
        errorCode = ServiceImplModuleOutputWrite((uint8_t)name,state,MODULE_MIX_WASH_OUTPUT_MAP_UNIT_LENGTH,(MODULE_OUTPUT_MAP_UNIT*)moduleMixWashOutputMapArray);
        if(errorCode != LH_ERR_NONE)
        {
            if(errorCode == LH_ERR_BOARD_MAIN_COMM_OUTPUT_PIN_INDEX)
            {
                *commErrorCode = CAN1_REPORT_ERR_MIX_WASH_OUTPUT_INDEX;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
            else
            {
                *commErrorCode = CAN1_REPORT_ERR_MIX_WASH_OUTPUT_WRITE;
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
static LH_ERR ServiceImplMixWash_ModuleSafeCheckBeforeRun(uint8_t mode,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //防止编译警告
        mode = mode;
    //相关的安全状态判定,检查此时清洗盘有没有处于安全状态,清洗盘处于安全状态的时候,说明清洗盘正在旋转,混匀不能上升
        if(PLATE_SAFE_EFFECTIVE == ServiceStateReadPlateWashSafeState())
        {
            //清洗盘保护状态,混匀不能上升
            errorCode = LH_ERR_BOARD_MAIN_MIX_WASH_SAFE_PLATE_WASH;
            *commErrorCode = CAN1_REPORT_ERR_MIX_WASH_MODULE_PLATE_WASH_SAFE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            return errorCode;
        }
    return errorCode;
}

/*******************************************************************
**description: 自动运行状态机初始化 
******************************************************************/
LH_ERR ServiceImplMixWash_StateMachineInit(void)
{
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 自动运行状态机轮转 
******************************************************************/
LH_ERR ServiceImplMixWash_StateMachineLoop(void)
{
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 清洗混匀复位 
******************************************************************/
LH_ERR ServiceImplMixWash_ResetAll(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //清洗混匀复位,清洗混匀复位的时候不需要考虑清洗盘是否处于安全状态
        errorCode = Can2SubBoard1MixWash_Reset();
    //读取失败,错误处理
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_MIX_WASH_MODULE_RESET;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //相关的系统状态写入,复位之后不需要保护混匀
        ServiceStateWriteMixWashSafeState(MIX_SAFE_INVALID);
    //返回数据
        return errorCode;
}

/*******************************************************************
**description: 清洗混匀上升 
******************************************************************/
LH_ERR ServiceImplMixWash_Up(MIX_MODE mode,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //判定参数
        if(mode > MIX_MODE_MAX)
        {
            errorCode = LH_ERR_BOARD_MAIN_MIX_WASH_MODE_PARAM;
            *commErrorCode = CAN1_REPORT_ERR_MIX_WASH_MODULE_MODE_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //相关的安全状态判定
        errorCode = ServiceImplMixWash_ModuleSafeCheckBeforeRun(0,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //清洗混匀升起来
        errorCode = Can2SubBoard1MixWash_Up(mode);
    //读取失败,错误处理
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_MIX_WASH_MODULE_UP;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //相关的系统状态写入,混匀升起来之后就要保护混匀,清洗盘就不能转了
        ServiceStateWriteMixWashSafeState(MIX_SAFE_EFFECTIVE);
    //返回数据
        return errorCode;
}

/*******************************************************************
**description: 清洗混匀下降 
******************************************************************/
LH_ERR ServiceImplMixWash_Down(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //清洗混匀降下去
        errorCode = Can2SubBoard1MixWash_Down();
    //读取失败,错误处理
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_MIX_WASH_MODULE_DOWN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //相关的系统状态写入,混匀降下去之后,不需要保护混匀了
        ServiceStateWriteMixWashSafeState(MIX_SAFE_INVALID);
    //返回数据
        return errorCode;
}

/*******************************************************************
**description: 清洗混匀开始混匀 
******************************************************************/
LH_ERR ServiceImplMixWash_StartRotate(MIX_MODE mode,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //判定参数
        if(mode > MIX_MODE_MAX)
        {
            errorCode = LH_ERR_BOARD_MAIN_MIX_WASH_MODE_PARAM;
            *commErrorCode = CAN1_REPORT_ERR_MIX_WASH_MODULE_MODE_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //清洗混匀旋转一次不带升降
        errorCode = Can2SubBoard1MixWash_StartRotate(mode);
    //读取失败,错误处理
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_MIX_WASH_MODULE_ROTATE_ONCE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //返回数据
        return errorCode;
}

/*******************************************************************
**description: 清洗混匀集成混匀 
******************************************************************/
LH_ERR ServiceImplMixWash_RotateIntegrate(MIX_MODE mode,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //判定参数
        if(mode > MIX_MODE_MAX)
        {
            errorCode = LH_ERR_BOARD_MAIN_MIX_WASH_MODE_PARAM;
            *commErrorCode = CAN1_REPORT_ERR_MIX_WASH_MODULE_MODE_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //相关的安全状态判定
        errorCode = ServiceImplMixWash_ModuleSafeCheckBeforeRun(0,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //混匀加入保护,混匀准备升起来运转,此时要锁住清洗盘
        ServiceStateWriteMixWashSafeState(MIX_SAFE_INVALID);
    //清洗混匀旋转,集成
        errorCode = Can2SubBoard1MixWash_RotateIntegrated(mode);
    //读取失败,错误处理
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_MIX_WASH_MODULE_ROTATE_INTEGRATED;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //相关的系统状态写入,运行完成,混匀下降来,自然就可以解除混匀安全保护状态
        ServiceStateWriteMixWashSafeState(MIX_SAFE_INVALID);
    //返回数据
        return errorCode;
}

/*******************************************************************
**description: 读取模块内部指定输入的状态 
******************************************************************/
LH_ERR ServiceImplMixWash_InputRead(uint8_t inputIndex,SENSOR_STATE* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = ServiceImplMixWash_ModuleInputRead((INPUT_MIX_WASH_NAME)inputIndex,inputState,commErrorCode,commErrorLevel);
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
LH_ERR ServiceImplMixWash_OutputWrite(uint8_t outputIndex,OUT_STATE setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = ServiceImplMixWash_ModuleOutputWrite((OUTPUT_MIX_WASH_NAME)outputIndex,setState,commErrorCode,commErrorLevel);
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
LH_ERR ServiceImplMixWash_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //判定参数
        if(motorIndex > INDEX_MOTOR_MIX_WASH_ROTATE)
        {
            errorCode = LH_ERR_BOARD_MAIN_MIX_WASH_MOTOR_INDEX;
            *commErrorCode = CAN1_REPORT_ERR_MIX_WASH_STEP_MOTOR_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //指定步进电机复位带复位修正
        INDEX_MOTOR_MIX_WASH stepMotorIndex = (INDEX_MOTOR_MIX_WASH)(motorIndex);
        errorCode = Can2SubBoard1MixWash_StepMotorReset(stepMotorIndex);
    //读取失败,错误处理
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_MIX_WASH_STEP_MOTOR_SINGLE_RESET;
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
LH_ERR ServiceImplMixWash_MotorRunSteps(uint8_t motorIndex,int32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //判定参数,主要是电机序号
        if(motorIndex > INDEX_MOTOR_MIX_WASH_ROTATE)
        {
            errorCode = LH_ERR_BOARD_MAIN_MIX_WASH_MOTOR_INDEX;
            *commErrorCode = CAN1_REPORT_ERR_MIX_WASH_STEP_MOTOR_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //清洗混匀电机走步数
        INDEX_MOTOR_MIX_WASH stepMotorIndex = (INDEX_MOTOR_MIX_WASH)(motorIndex);
        errorCode = Can2SubBoard1MixWash_StepMotorRunSteps(stepMotorIndex,steps,posAfterRun);
    //读取失败,错误处理
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_MIX_WASH_STEP_MOTOR_SINGLE_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //返回数据
        return errorCode;
}


























