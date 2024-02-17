/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 13:53:58
**LastEditors: DengXiaoJun
**LastEditTime: 2020-08-18 15:30:49
**FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplHandWash.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplHandWash.h"
#include "ServiceStatePrivate.h"

//模块输入配置映射,前一个是系统序号,后一个是有效电平
static const MODULE_INPUT_MAP_UNIT moduleHandWashInputMapArray[] = {
    {INPUT_HAND_WASH_ROTATE_ORGIN_SENSOR   ,VALID_LEVEL_INPUT_HAND_WASH_ROTATE_ORGIN_SENSOR},//旋转电机原点
    {INPUT_HAND_WASH_UPDOWN_ORGIN_SENSOR   ,VALID_LEVEL_INPUT_HAND_WASH_UPDOWN_ORGIN_SENSOR},//升降电机原点
    {INPUT_HAND_WASH_ALARM_UTIL_SENSOR     ,VALID_LEVEL_INPUT_HAND_WASH_ALARM_UTIL_SENSOR  },//升降防撞信号
};
//配置数组长度
#define MODULE_HAND_WASH_INPUT_MAP_ARRAY_LENGTH   (sizeof(moduleHandWashInputMapArray)/sizeof(moduleHandWashInputMapArray[0]))
/*******************************************************************
**description: 读取指定输入 
******************************************************************/
static LH_ERR ServiceImplHandWash_ModuleInputRead(INPUT_HAND_WASH_NAME name,SENSOR_STATE* state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //输入读取
        errorCode = ServiceImplModuleInputRead((uint8_t)name,MODULE_HAND_WASH_INPUT_MAP_ARRAY_LENGTH,(MODULE_INPUT_MAP_UNIT*)moduleHandWashInputMapArray,state);
        if(errorCode != LH_ERR_NONE)
        {
            if(errorCode == LH_ERR_BOARD_MAIN_COMM_INPUT_PIN_INDEX)
            {
                *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_INPUT_INDEX;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
            else
            {
                *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_INPUT_READ;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
        }
    return errorCode;
}


//模块输出配置映射
static const MODULE_OUTPUT_MAP_UNIT moduleHandWashOutputMapArray[] = {
    {OUTPUT_HAND_WASH_ELECTRO_MAGNET_PLATE_REACTION     ,VALID_LEVEL_OUTPUT_HAND_WASH_ELECTRO_MAGNET_PLATE_REACTION },//反应杯旧杯电磁铁
    {OUTPUT_HAND_WASH_ELECTRO_MAGNET_PLATE_WASH         ,VALID_LEVEL_OUTPUT_HAND_WASH_ELECTRO_MAGNET_PLATE_WASH     },//清洗盘放杯电磁铁
};
//配置数组长度
#define MODULE_HAND_WASH_OUTPUT_MAP_UNIT_LENGTH   (sizeof(moduleHandWashOutputMapArray)/sizeof(moduleHandWashOutputMapArray[0]))
/*******************************************************************
**description: 写入指定输出 
******************************************************************/
static LH_ERR ServiceImplHandWash_ModuleOutputWrite(OUTPUT_HAND_WASH_NAME name,OUT_STATE state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //输出写入
        errorCode = ServiceImplModuleOutputWrite((uint8_t)name,state,MODULE_HAND_WASH_OUTPUT_MAP_UNIT_LENGTH,(MODULE_OUTPUT_MAP_UNIT*)moduleHandWashOutputMapArray);
        if(errorCode != LH_ERR_NONE)
        {
            if(errorCode == LH_ERR_BOARD_MAIN_COMM_OUTPUT_PIN_INDEX)
            {
                *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_OUTPUT_INDEX;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
            else
            {
                *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_OUTPUT_WRITE;
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
typedef enum HAND_WASH_SAFE_CHECK_FLAG
{
    HAND_WASH_SAFE_CHECK_FLAG_PLATE_REACTION        = 0X00,//机械手在反应盘上方做安全检查
    HAND_WASH_SAFE_CHECK_FLAG_PLATE_WASH            = 0X01,//机械手在清洗盘上方做安全检查
    HAND_WASH_SAFE_CHECK_FLAG_CATCH_MEASURE_UNIT    = 0X02,//机械手在测量单元上方做安全检查,抓杯检查
    HAND_WASH_SAFE_CHECK_FLAG_PUT_MEASURE_UNIT      = 0X03,//机械手在测量单元上方做安全检查,放杯检查
}HAND_WASH_SAFE_CHECK_FLAG;

static LH_ERR ServiceImplHandWash_ModuleSafeCheckBeforeRun(uint8_t mode,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //机械手在反应盘上方做安全检查
        if(mode == HAND_WASH_SAFE_CHECK_FLAG_PLATE_REACTION)
        {
            //检查反应盘状态
            if(PLATE_SAFE_EFFECTIVE == ServiceStateReadPlateReactionSafeState())
            {
                errorCode = LH_ERR_BOARD_MAIN_HAND_WASH_SAFE_PROTECT;
                *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_MODULE_PLATE_REACTION_SAFE;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                return errorCode;
            }
        }
    //机械手在清洗盘上方做安全检查
        else if(mode == HAND_WASH_SAFE_CHECK_FLAG_PLATE_WASH)
        {
            //检查清洗盘状态
            if(PLATE_SAFE_EFFECTIVE == ServiceStateReadPlateWashSafeState())
            {
                errorCode = LH_ERR_BOARD_MAIN_HAND_WASH_SAFE_PROTECT;
                *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_MODULE_PLATE_WASH_SAFE;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                return errorCode;
            }
        }
    //机械手在测量单元上方做安全检查,抓杯检查
        else if(mode == HAND_WASH_SAFE_CHECK_FLAG_CATCH_MEASURE_UNIT)
        {
            //测量室上门必须全开
            if(MEASURE_MODULE_DOOR_OPEN_FULL != ServiceStateReadMeasureModuleDoorState())
            {
                errorCode = LH_ERR_BOARD_MAIN_HAND_WASH_SAFE_PROTECT;
                *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_MODULE_MEASURE_UNIT_SAFE;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                return errorCode;
            }
        }
    //机械手在测量单元上方做安全检查,放杯检查
        else if(mode == HAND_WASH_SAFE_CHECK_FLAG_PUT_MEASURE_UNIT)
        {
            //测量室上门必须半开
            if(MEASURE_MODULE_DOOR_OPEN_HALF != ServiceStateReadMeasureModuleDoorState())
            {
                errorCode = LH_ERR_BOARD_MAIN_HAND_WASH_SAFE_PROTECT;
                *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_MODULE_MEASURE_UNIT_SAFE;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                return errorCode;
            }
        }
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 自动运行状态机初始化 
******************************************************************/
LH_ERR ServiceImplHandWash_StateMachineInit(void)
{
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 自动运行状态机轮转 
******************************************************************/
LH_ERR ServiceImplHandWash_StateMachineLoop(void)
{
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 清洗机械手复位 
******************************************************************/
LH_ERR ServiceImplHandWash_ResetAll(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //复位
        errorCode = Can2SubBoard3HandWash_Reset();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_MODULE_RESET;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //取消清洗机械手保护
        ServiceStateWriteHandWashSafeState(HAND_SAFE_INVALID);
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 清洗机械手旋转到清洗盘 
******************************************************************/
LH_ERR ServiceImplHandWash_RotatePlateWash(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //旋转到清洗盘
        errorCode = Can2SubBoard3HandWash_RotatePlateWash();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_MODULE_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //取消清洗机械手保护
        ServiceStateWriteHandWashSafeState(HAND_SAFE_INVALID);
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 清洗机械手旋转到反应盘 
******************************************************************/
LH_ERR ServiceImplHandWash_RotatePlateReaction(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //旋转到反应盘
        errorCode = Can2SubBoard3HandWash_RotatePlateReaction();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_MODULE_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //取消清洗机械手保护
        ServiceStateWriteHandWashSafeState(HAND_SAFE_INVALID);
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 清洗机械手旋转到测量模块 
******************************************************************/
LH_ERR ServiceImplHandWash_RotateMeasureModule(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //旋转到测量模块
        errorCode = Can2SubBoard3HandWash_RotateMeasureModule();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_MODULE_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //取消清洗机械手保护
        ServiceStateWriteHandWashSafeState(HAND_SAFE_INVALID);
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 清洗机械手从反应盘抓杯 
******************************************************************/
LH_ERR ServiceImplHandWash_CatchPlateReaction(SENSOR_STATE* plateReactionCupExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查清洗机械手在反应盘上方的安全许可
        errorCode = ServiceImplHandWash_ModuleSafeCheckBeforeRun(HAND_WASH_SAFE_CHECK_FLAG_PLATE_REACTION,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //清洗机械手在反应盘上方安全保护
        ServiceStateWriteHandWashSafeState(HAND_SAFE_EFFECTIVE_PLATE_REACTION);
    //反应盘取杯
        errorCode = Can2SubBoard3HandWash_CatchCupPlateReaction(plateReactionCupExist);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_MODULE_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //更新反应盘上的杯子状态
        ServiceStateWritePlateReactionOldCupExist(*plateReactionCupExist);
    //取消安全保护
        ServiceStateWriteHandWashSafeState(HAND_SAFE_INVALID);
    //非老化模式,识别杯状态信号
        if((FLAG_HAND_SHAKE_TRIG == ServiceStateReadAssistHandShakeFlag())&&(MODE_RELEASE == ServiceStateReadAssistRunningMode()))
        {
            //取杯完成,反应盘还有杯,说明杯丢失
            if((*plateReactionCupExist) == SENSOR_STATE_VALID)
            {
                //抓杯之后反应盘还有杯
                errorCode = LH_ERR_BOARD_MAIN_HAND_WASH_LOSS_CUP;
                *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_LOSS_CUP_PLATE_REACTION;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                //错误代码打印
                ServicePrintfShowError(errorCode);
                return errorCode;
            }
        }
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 清洗机械手往反应盘放杯 
******************************************************************/
LH_ERR ServiceImplHandWash_PutPlateReaction(SENSOR_STATE* plateReactionCupExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //非老化模式,识别杯状态信号
        if((FLAG_HAND_SHAKE_TRIG == ServiceStateReadAssistHandShakeFlag())&&(MODE_RELEASE == ServiceStateReadAssistRunningMode()))
        {
            //检测反应盘旧杯位当前状态
            SENSOR_STATE plateReactionCupExistCheck = SENSOR_STATE_VALID;
            plateReactionCupExistCheck = ServiceStateReadPlateReactionOldCupExist();
            if(plateReactionCupExistCheck == SENSOR_STATE_VALID)
            {
                //清洗机械手放杯前检测到当前位置有杯
                errorCode = LH_ERR_BOARD_MAIN_HAND_WASH_PUT_REACTION_CUP_EXIST;
                *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_PUT_REACTION_CUP_EXIST;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                //错误代码打印
                ServicePrintfShowError(errorCode);
                return errorCode;
            }
        }
    //检查清洗机械手在反应盘上方的安全许可
        errorCode = ServiceImplHandWash_ModuleSafeCheckBeforeRun(HAND_WASH_SAFE_CHECK_FLAG_PLATE_REACTION,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //清洗机械手在反应盘上方安全保护
        ServiceStateWriteHandWashSafeState(HAND_SAFE_EFFECTIVE_PLATE_REACTION);
    //反应盘取杯
        errorCode = Can2SubBoard3HandWash_PutCupPlateReaction(plateReactionCupExist);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_MODULE_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //更新反应盘上的杯子状态
        ServiceStateWritePlateReactionOldCupExist(*plateReactionCupExist);
    //取消安全保护
        ServiceStateWriteHandWashSafeState(HAND_SAFE_INVALID);
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 清洗机械手从清洗盘抓杯 
******************************************************************/
LH_ERR ServiceImplHandWash_CatchPlateWash(SENSOR_STATE* plateWashCupExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查清洗机械手在清洗盘上方的安全许可
        errorCode = ServiceImplHandWash_ModuleSafeCheckBeforeRun(HAND_WASH_SAFE_CHECK_FLAG_PLATE_WASH,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //清洗机械手在清洗盘上方安全保护
        ServiceStateWriteHandWashSafeState(HAND_SAFE_EFFECTIVE_WASH);
    //清洗盘取杯
        errorCode = Can2SubBoard3HandWash_CatchCupPlateWash(plateWashCupExist);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_MODULE_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //更新清洗盘上的杯子状态
        ServiceStateWritePlateWashCupExist(*plateWashCupExist);
    //取消安全保护
        ServiceStateWriteHandWashSafeState(HAND_SAFE_INVALID);
    //非老化模式,识别杯状态信号
        if((FLAG_HAND_SHAKE_TRIG == ServiceStateReadAssistHandShakeFlag())&&(MODE_RELEASE == ServiceStateReadAssistRunningMode()))
        {
            //取杯完成,反应盘还有杯,说明杯丢失
            if((*plateWashCupExist) == SENSOR_STATE_VALID)
            {
                //抓杯之后反应盘还有杯
                errorCode = LH_ERR_BOARD_MAIN_HAND_WASH_LOSS_CUP;
                *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_LOSS_CUP_PLATE_WASH;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                //错误代码打印
                ServicePrintfShowError(errorCode);
                return errorCode;
            }
        }
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 清洗机械手往清洗盘放杯 
******************************************************************/
LH_ERR ServiceImplHandWash_PutPlateWash(SENSOR_STATE* plateWashCupExist,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //非老化模式,识别杯状态信号
        if((FLAG_HAND_SHAKE_TRIG == ServiceStateReadAssistHandShakeFlag())&&(MODE_RELEASE == ServiceStateReadAssistRunningMode()))
        {
            //检测清洗盘新杯位当前状态
            SENSOR_STATE plateWashCupExistCheck = SENSOR_STATE_VALID;
            plateWashCupExistCheck = ServiceStateReadPlateWashCupExist();
            if(plateWashCupExistCheck == SENSOR_STATE_VALID)
            {
                //清洗机械手放杯前检测到当前位置有杯
                errorCode = LH_ERR_BOARD_MAIN_HAND_WASH_PUT_WASH_CUP_EXIST;
                *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_PUT_WASH_CUP_EXIST;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                //错误代码打印
                ServicePrintfShowError(errorCode);
                return errorCode;
            }
        }
    //检查清洗机械手在清洗盘上方的安全许可
        errorCode = ServiceImplHandWash_ModuleSafeCheckBeforeRun(HAND_WASH_SAFE_CHECK_FLAG_PLATE_WASH,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //清洗机械手在清洗盘上方安全保护
        ServiceStateWriteHandWashSafeState(HAND_SAFE_EFFECTIVE_WASH);
    //清洗盘取杯
        errorCode = Can2SubBoard3HandWash_PutCupPlateWash(plateWashCupExist);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_MODULE_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //更新清洗盘上的杯子状态
        ServiceStateWritePlateWashCupExist(*plateWashCupExist);
    //取消安全保护
        ServiceStateWriteHandWashSafeState(HAND_SAFE_INVALID);
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 清洗机械手去测量模块抓杯 
******************************************************************/
LH_ERR ServiceImplHandWash_CatchMeasureModule(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查清洗机械手在测量模块上方抓杯的安全许可
        errorCode = ServiceImplHandWash_ModuleSafeCheckBeforeRun(HAND_WASH_SAFE_CHECK_FLAG_CATCH_MEASURE_UNIT,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //清洗机械手在测量模块上方安全保护
        ServiceStateWriteHandWashSafeState(HAND_SAFE_EFFECTIVE_MEASURE_MODULE);
    //测量模块抓杯
        errorCode = Can2SubBoard3HandWash_CatchCupMeasureModule();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_MODULE_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //取消安全保护
        ServiceStateWriteHandWashSafeState(HAND_SAFE_INVALID);
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 清洗机械手往测量模块放杯 
******************************************************************/
LH_ERR ServiceImplHandWash_PutMeasureModule(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查清洗机械手在测量模块上方放杯的安全许可
        errorCode = ServiceImplHandWash_ModuleSafeCheckBeforeRun(HAND_WASH_SAFE_CHECK_FLAG_PUT_MEASURE_UNIT,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //清洗机械手在测量模块上方安全保护
        ServiceStateWriteHandWashSafeState(HAND_SAFE_EFFECTIVE_MEASURE_MODULE);
    //测量模块放杯
        errorCode = Can2SubBoard3HandWash_PutCupMeasureModule();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_MODULE_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //取消安全保护
        ServiceStateWriteHandWashSafeState(HAND_SAFE_INVALID);
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 读取模块内部指定输入的状态 
******************************************************************/
LH_ERR ServiceImplHandWash_InputRead(uint8_t inputIndex,SENSOR_STATE* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = ServiceImplHandWash_ModuleInputRead((INPUT_HAND_WASH_NAME)inputIndex,inputState,commErrorCode,commErrorLevel);
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
LH_ERR ServiceImplHandWash_OutputWrite(uint8_t outputIndex,OUT_STATE setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = ServiceImplHandWash_ModuleOutputWrite((OUTPUT_HAND_WASH_NAME)outputIndex,setState,commErrorCode,commErrorLevel);
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
LH_ERR ServiceImplHandWash_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //判定参数,主要是电机序号
        if(motorIndex > INDEX_MOTOR_HAND_WASH_UPDOWN)
        {
            errorCode = LH_ERR_BOARD_MAIN_HAND_WASH_MOTOR_INDEX;
            *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_STEP_MOTOR_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //指定步进电机复位带复位修正
        INDEX_MOTOR_HAND_WASH stepMotorIndex = (INDEX_MOTOR_HAND_WASH)(motorIndex);
        errorCode = Can2SubBoard3HandWash_StepMotorReset(stepMotorIndex);
    //读取失败,错误处理
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_STEP_MOTOR_SINGLE_RESET;
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
LH_ERR ServiceImplHandWash_MotorRunSteps(uint8_t motorIndex,int32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //判定参数,主要是电机序号
        if(motorIndex > INDEX_MOTOR_HAND_WASH_UPDOWN)
        {
            errorCode = LH_ERR_BOARD_MAIN_HAND_WASH_MOTOR_INDEX;
            *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_STEP_MOTOR_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //试剂盘电机走步数
        INDEX_MOTOR_HAND_WASH stepMotorIndex = (INDEX_MOTOR_HAND_WASH)(motorIndex);
        errorCode = Can2SubBoard3HandWash_StepMotorRunSteps(stepMotorIndex,steps,posAfterRun);
    //读取失败,错误处理
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_HAND_WASH_STEP_MOTOR_SINGLE_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //返回数据
        return errorCode;
}
























