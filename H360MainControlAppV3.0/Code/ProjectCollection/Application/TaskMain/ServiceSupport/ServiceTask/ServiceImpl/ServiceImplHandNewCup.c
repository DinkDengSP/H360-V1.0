/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 13:53:51
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 13:09:56
**FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplHandNewCup.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplHandNewCup.h"
#include "ServiceStatePrivate.h"

//模块输入配置映射,前一个是系统序号,后一个是有效电平
static const MODULE_INPUT_MAP_UNIT moduleHandNewCupInputMapArray[] = {
    {INPUT_HAND_NEW_CUP_BACKFRONT_ORGIN_SENSOR        ,VALID_LEVEL_INPUT_HAND_NEW_CUP_BACKFRONT_ORGIN_SENSOR    },//前后电机原点
    {INPUT_HAND_NEW_CUP_LEFTRIGHT_ORGIN_SENSOR        ,VALID_LEVEL_INPUT_HAND_NEW_CUP_LEFTRIGHT_ORGIN_SENSOR    },//左右电机原点
    {INPUT_HAND_NEW_CUP_UPDOWN_ORGIN_SENSOR           ,VALID_LEVEL_INPUT_HAND_NEW_CUP_UPDOWN_ORGIN_SENSOR       },//升降电机原点
};
//配置数组长度
#define MODULE_HAND_NEW_CUP_INPUT_MAP_ARRAY_LENGTH   (sizeof(moduleHandNewCupInputMapArray)/sizeof(moduleHandNewCupInputMapArray[0]))
/*******************************************************************
**description: 读取指定输入 
******************************************************************/
static LH_ERR ServiceImplHandNewCup_ModuleInputRead(INPUT_HAND_NEW_CUP_NAME name,SENSOR_STATE* state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //输入读取
    errorCode = ServiceImplModuleInputRead((uint8_t)name,MODULE_HAND_NEW_CUP_INPUT_MAP_ARRAY_LENGTH,(MODULE_INPUT_MAP_UNIT*)moduleHandNewCupInputMapArray,state);
    if(errorCode != LH_ERR_NONE)
    {
        if(errorCode == LH_ERR_BOARD_MAIN_COMM_INPUT_PIN_INDEX)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_INPUT_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        else
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_INPUT_READ;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
    }
    return errorCode;
}

//模块输出配置映射
static const MODULE_OUTPUT_MAP_UNIT moduleHandNewCupOutputMapArray[] = {
    {OUTPUT_HAND_NEW_CUP_ELECTRO_MAGNET    ,VALID_LEVEL_OUTPUT_HAND_NEW_CUP_ELECTRO_MAGNET},//反应杯放杯电磁铁
};
//配置数组长度
#define MODULE_HAND_NEW_CUP_OUTPUT_MAP_UNIT_LENGTH   (sizeof(moduleHandNewCupOutputMapArray)/sizeof(moduleHandNewCupOutputMapArray[0]))
/*******************************************************************
**description: 写入指定输出 
******************************************************************/
static LH_ERR ServiceImplHandNewCup_ModuleOutputWrite(OUTPUT_HAND_NEW_CUP_NAME name,OUT_STATE state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //输出写入
    errorCode = ServiceImplModuleOutputWrite((uint8_t)name,state,MODULE_HAND_NEW_CUP_OUTPUT_MAP_UNIT_LENGTH,(MODULE_OUTPUT_MAP_UNIT*)moduleHandNewCupOutputMapArray);
    if(errorCode != LH_ERR_NONE)
    {
        if(errorCode == LH_ERR_BOARD_MAIN_COMM_OUTPUT_PIN_INDEX)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_OUTPUT_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        else
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_OUTPUT_WRITE;
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
typedef enum HAND_NEW_CUP_SAFE_CHECK_FLAG
{
    HAND_NEW_CUP_SAFE_CHECK_FLAG_NEW_STACK      = 0X00,//机械手在新杯区上方做安全检查
    HAND_NEW_CUP_SAFE_CHECK_FLAG_PLATE_REACTION = 0X01,//机械手在反应盘上方做安全检查
}HAND_NEW_CUP_SAFE_CHECK_FLAG;

static LH_ERR ServiceImplHandNewCup_ModuleSafeCheckBeforeRun(HAND_NEW_CUP_SAFE_CHECK_FLAG mode,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //新杯栈上方安全检测
        if(mode == HAND_NEW_CUP_SAFE_CHECK_FLAG_NEW_STACK)
        {
            //检查新杯栈状态
            if(MODULE_SAFE_EFFECTIVE == ServiceStateReadCupTrayStackManageSafeState())
            {
                errorCode = LH_ERR_BOARD_MAIN_HAND_NEW_CUP_SAFE_PROTECT;
                *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_MODULE_NEW_TRAY_STACK_SAFE;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                return errorCode;
            }
        }
    //反应盘上方安全检测
        else if(mode == HAND_NEW_CUP_SAFE_CHECK_FLAG_PLATE_REACTION)
        {
            //检查反应盘状态
            if(PLATE_SAFE_EFFECTIVE == ServiceStateReadPlateReactionSafeState())
            {
                errorCode = LH_ERR_BOARD_MAIN_HAND_NEW_CUP_SAFE_PROTECT;
                *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_MODULE_PLATE_REACTION_SAFE;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                return errorCode;
            }
        }
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 自动运行状态机初始化 
******************************************************************/
LH_ERR ServiceImplHandNewCup_StateMachineInit(void)
{
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 自动运行状态机轮转 
******************************************************************/
LH_ERR ServiceImplHandNewCup_StateMachineLoop(void)
{
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 新杯机械手复位 
******************************************************************/
LH_ERR ServiceImplHandNewCup_ResetAll(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //复位
        errorCode = Can2SubBoard5HandNewCup_Reset();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_MODULE_RESET;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //取消新杯机械手保护
        ServiceStateWriteHandNewCupSafeState(HAND_SAFE_INVALID);
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 新杯机械手移动到新杯盘 
******************************************************************/
LH_ERR ServiceImplHandNewCup_Move2NewStack(uint8_t leftRight,uint8_t backFront,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查左右参数
        if((leftRight == 0)||(leftRight > 10))
        {
            errorCode = LH_ERR_BOARD_MAIN_HAND_NEW_CUP_PARAM;
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_LEFTRIGHT;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //检查前后参数
        if((backFront == 0)||(backFront > 14))
        {
            errorCode = LH_ERR_BOARD_MAIN_HAND_NEW_CUP_PARAM;
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_BACKFRONT;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //移动到新杯区
        errorCode = Can2SubBoard5HandNewCup_MoveToNewCupTray(backFront,leftRight);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    return errorCode;
}

/*******************************************************************
**description: 新杯机械手移动到反应盘 
******************************************************************/
LH_ERR ServiceImplHandNewCup_Move2PlateReaction(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //移动到反应盘
        errorCode = Can2SubBoard5HandNewCup_MoveToPlateReaction();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    return errorCode;
}

/*******************************************************************
**description: 新杯机械手移动到垃圾桶 
******************************************************************/
LH_ERR ServiceImplHandNewCup_Move2Garbage(GARBAGE_SELECT garbageNo,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //转换垃圾桶序号
        HAND_NEW_CUP_GARBAGE_NO garbageSubNo;
    //垃圾桶序号检查
        if(garbageNo == GARBAGE_SELECT_1)
        {
            //垃圾桶1
            garbageSubNo = HAND_NEW_CUP_GARBAGE_NO_1;
        }
        else if(garbageNo == GARBAGE_SELECT_2)
        {
            //垃圾桶2
            garbageSubNo = HAND_NEW_CUP_GARBAGE_NO_2;
        }
        else
        {
            //垃圾桶参数错误
            errorCode = LH_ERR_BOARD_MAIN_HAND_NEW_CUP_PARAM;
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_GARBAGE_NO;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //移动到指定垃圾桶位置
        errorCode = Can2SubBoard5HandNewCup_MoveToGarbage(garbageSubNo);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 新杯机械手从新杯盘取杯 
******************************************************************/
LH_ERR ServiceImplHandNewCup_CatchCupFromNewStack(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查机械手在新杯栈上方的安全状态
        errorCode = ServiceImplHandNewCup_ModuleSafeCheckBeforeRun(HAND_NEW_CUP_SAFE_CHECK_FLAG_NEW_STACK,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //机械手在新杯栈上方安全保护
        ServiceStateWriteHandNewCupSafeState(HAND_SAFE_EFFECTIVE_NEW_STACK);
    //从新杯栈取杯
        errorCode = Can2SubBoard5HandNewCup_CatchCupNewCupTray();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //机械手取消安全保护
        ServiceStateWriteHandNewCupSafeState(HAND_SAFE_INVALID);
    return errorCode;
}

/*******************************************************************
**description: 新杯机械手从反应盘取杯 
******************************************************************/
LH_ERR ServiceImplHandNewCup_CatchCupFromPlateReaction(SENSOR_STATE* newCupExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查机械手在反应盘的安全状态
        errorCode = ServiceImplHandNewCup_ModuleSafeCheckBeforeRun(HAND_NEW_CUP_SAFE_CHECK_FLAG_PLATE_REACTION,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //机械手在反应盘上方安全保护
        ServiceStateWriteHandNewCupSafeState(HAND_SAFE_EFFECTIVE_PLATE_REACTION);
    //反应盘取杯
        errorCode = Can2SubBoard5HandNewCup_CatchCupPlateReaction(newCupExist);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //机械手取消安全保护
        ServiceStateWriteHandNewCupSafeState(HAND_SAFE_INVALID);
    //更新反应盘上的杯子状态
        ServiceStateWritePlateReactionNewCupExist(*newCupExist);
    //非老化模式,识别杯状态信号
        if((FLAG_HAND_SHAKE_TRIG == ServiceStateReadAssistHandShakeFlag())&&(MODE_RELEASE == ServiceStateReadAssistRunningMode()))
        {        
            //取杯完成,反应盘还有杯,说明杯丢失
            if((*newCupExist) == SENSOR_STATE_VALID)
            {
                //抓杯之后反应盘还有杯
                errorCode = LH_ERR_BOARD_MAIN_HAND_NEW_CUP_LOSS_CUP;
                *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_LOSS_CUP;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                //错误代码打印
                ServicePrintfShowError(errorCode);
                return errorCode;
            }
        }
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 新杯机械手往反应盘放杯 
******************************************************************/
LH_ERR ServiceImplHandNewCup_PutCupToPlateReaction(SENSOR_STATE* newCupExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查机械手在反应盘的安全状态
        errorCode = ServiceImplHandNewCup_ModuleSafeCheckBeforeRun(HAND_NEW_CUP_SAFE_CHECK_FLAG_PLATE_REACTION,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //非老化模式,识别杯状态信号
        if((FLAG_HAND_SHAKE_TRIG == ServiceStateReadAssistHandShakeFlag())&&(MODE_RELEASE == ServiceStateReadAssistRunningMode()))
        {
            //检测反应盘新杯位当前状态
            SENSOR_STATE newCupExistCheck = SENSOR_STATE_VALID;
            newCupExistCheck = ServiceStateReadPlateReactionNewCupExist();
            if(newCupExistCheck == SENSOR_STATE_VALID)
            {
                //新杯机械手放杯前检测到当前位置有杯
                errorCode = LH_ERR_BOARD_MAIN_HAND_NEW_CUP_PUT_REACTION_CUP_EXIST;
                *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_PUT_CUP_REACTION_EXIST;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                //错误代码打印
                ServicePrintfShowError(errorCode);
                return errorCode;
            }
        }
    //机械手在反应盘上方安全保护
        ServiceStateWriteHandNewCupSafeState(HAND_SAFE_EFFECTIVE_PLATE_REACTION);
    //反应盘放杯
        errorCode = Can2SubBoard5HandNewCup_PutCupPlateReaction(newCupExist);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //机械手取消安全保护
        ServiceStateWriteHandNewCupSafeState(HAND_SAFE_INVALID);
    //更新反应盘上的杯子状态
        ServiceStateWritePlateReactionNewCupExist(*newCupExist);
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 新杯机械手往垃圾桶放杯 
******************************************************************/
LH_ERR ServiceImplHandNewCup_PutCupToGarbage(GARBAGE_SELECT garbageNo,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //转换垃圾桶序号
        HAND_NEW_CUP_GARBAGE_NO garbageSubNo;
        SENSOR_STATE garbage1Exist,garbage2Exist;
        ServiceStateReadCupTrayStackManageGarbageExistSensor(&garbage1Exist,&garbage2Exist);
    //垃圾桶序号检查
        if(garbageNo == GARBAGE_SELECT_1)
        {
            //垃圾桶1
            garbageSubNo = HAND_NEW_CUP_GARBAGE_NO_1;
            //检查垃圾桶1是否存在
            if(garbage1Exist == SENSOR_STATE_INVALID)
            {
                errorCode = LH_ERR_BOARD_MAIN_HAND_NEW_CUP_GARBAGE_LOSS;
                //垃圾桶1不存在,报错
                *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_GARBAGE1_LOSS;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                //错误代码打印
                ServicePrintfShowError(errorCode);
                return errorCode;
            }
        }
        else if(garbageNo == GARBAGE_SELECT_2)
        {
            //垃圾桶2
            garbageSubNo = HAND_NEW_CUP_GARBAGE_NO_2;
            //检查垃圾桶2是否存在
            if(garbage2Exist == SENSOR_STATE_INVALID)
            {
                errorCode = LH_ERR_BOARD_MAIN_HAND_NEW_CUP_GARBAGE_LOSS;
                //垃圾桶2不存在,报错
                *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_GARBAGE2_LOSS;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                //错误代码打印
                ServicePrintfShowError(errorCode);
                return errorCode;
            }
        }
        else
        {
            errorCode = LH_ERR_BOARD_MAIN_HAND_NEW_CUP_PARAM;
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_GARBAGE_NO;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //往指定垃圾桶丢杯
        errorCode = Can2SubBoard5HandNewCup_PutCupGarbage(garbageSubNo);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 读取模块内部指定输入的状态 
******************************************************************/
LH_ERR ServiceImplHandNewCup_InputRead(uint8_t inputIndex,SENSOR_STATE* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = ServiceImplHandNewCup_ModuleInputRead((INPUT_HAND_NEW_CUP_NAME)inputIndex,inputState,commErrorCode,commErrorLevel);
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
LH_ERR ServiceImplHandNewCup_OutputWrite(uint8_t outputIndex,OUT_STATE setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = ServiceImplHandNewCup_ModuleOutputWrite((OUTPUT_HAND_NEW_CUP_NAME)outputIndex,setState,commErrorCode,commErrorLevel);
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
LH_ERR ServiceImplHandNewCup_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //判定参数,主要是电机序号
        if(motorIndex > INDEX_MOTOR_HAND_NEW_CUP_UPDOWN)
        {
            errorCode = LH_ERR_BOARD_MAIN_HAND_NEW_CUP_MOTOR_INDEX;
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_STEP_MOTOR_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //指定步进电机复位带复位修正
        INDEX_MOTOR_HAND_NEW_CUP stepMotorIndex = (INDEX_MOTOR_HAND_NEW_CUP)(motorIndex);
        errorCode = Can2SubBoard5HandNewCup_StepMotorReset(stepMotorIndex);
    //读取失败,错误处理
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_STEP_MOTOR_SINGLE_RESET;
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
LH_ERR ServiceImplHandNewCup_MotorRunSteps(uint8_t motorIndex,int32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //判定参数,主要是电机序号
        if(motorIndex > INDEX_MOTOR_HAND_NEW_CUP_UPDOWN)
        {
            errorCode = LH_ERR_BOARD_MAIN_HAND_NEW_CUP_MOTOR_INDEX;
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_STEP_MOTOR_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //试剂盘电机走步数
        INDEX_MOTOR_HAND_NEW_CUP stepMotorIndex = (INDEX_MOTOR_HAND_NEW_CUP)(motorIndex);
        errorCode = Can2SubBoard5HandNewCup_StepMotorRunSteps(stepMotorIndex,steps,posAfterRun);
    //读取失败,错误处理
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_NEW_CUP_STEP_MOTOR_SINGLE_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //返回数据
        return errorCode;
}













