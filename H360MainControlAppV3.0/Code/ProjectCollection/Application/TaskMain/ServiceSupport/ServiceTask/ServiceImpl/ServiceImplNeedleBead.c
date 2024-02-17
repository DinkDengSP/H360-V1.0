/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 13:55:17
**LastEditors: DengXiaoJun
**LastEditTime: 2020-11-19 10:17:32
**FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplNeedleBead.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplNeedleBead.h"
#include "ServiceStatePrivate.h"
#include "ServiceIpcAssist.h"


//模块输入配置映射,前一个是系统序号,后一个是有效电平
static const MODULE_INPUT_MAP_UNIT moduleNeedleBeadInputMapArray[] = {
    {INPUT_NEEDLE_BEAD_UPDOWN_ORGIN_SENSOR    ,VALID_LEVEL_INPUT_NEEDLE_BEAD_UPDOWN_ORGIN_SENSOR    },//升降电机原点
    {INPUT_NEEDLE_BEAD_ROTATE_ORGIN_SENSOR    ,VALID_LEVEL_INPUT_NEEDLE_BEAD_ROTATE_ORGIN_SENSOR    },//旋转电机原点
    {INPUT_NEEDLE_BEAD_PUMP_ORGIN_SENSOR      ,VALID_LEVEL_INPUT_NEEDLE_BEAD_PUMP_ORGIN_SENSOR      },//注射泵原点
    {INPUT_NEEDLE_BEAD_EMERGENCY_STOP_SENSOR  ,VALID_LEVEL_INPUT_NEEDLE_BEAD_EMERGENCY_STOP_SENSOR  },//磁珠针升降防撞
    {INPUT_NEEDLE_BEAD_LIQUID_DETECT_SENSOR   ,VALID_LEVEL_INPUT_NEEDLE_BEAD_LIQUID_DETECT_SENSOR   },//磁珠针升降探液
};
//配置数组长度
#define MODULE_NEEDLE_BEAD_INPUT_MAP_ARRAY_LENGTH   (sizeof(moduleNeedleBeadInputMapArray)/sizeof(moduleNeedleBeadInputMapArray[0]))
/* 由动作层错误参数确定错误等级 */
void ServiceImplNeedleBead_GetErrorLevel(LH_ERR errorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    /* 动作层错误参数 */
    FLAG_NEEDLE_BEAD_ERR_CONVERT needleBeadErrParam;
    needleBeadErrParam = (FLAG_NEEDLE_BEAD_ERR_CONVERT)((errorCode>>16)&0XFF);
    switch (needleBeadErrParam)
    {
    /* 加样暂停级 */
        case FLAG_NEEDLE_BEAD_ERR_LIQUID_COMM:                    //磁珠针探液板通信失败
        case FLAG_NEEDLE_BEAD_ERR_CRASH_AT_PLATE_REAGENT:         //磁珠针试剂盘位置防撞
        case FLAG_NEEDLE_BEAD_ERR_CRASH_AT_CLEAN:                 //磁珠针清洗位置防撞
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            break;
    /* 急停级 */
        case FLAG_NEEDLE_BEAD_ERR_CRASH_AT_PLATE_REACTION:        //磁珠针反应盘注液位置防撞
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            break;
    default:*commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
        break;
    }
}

/*******************************************************************
**description: 读取指定输入 
******************************************************************/
static LH_ERR ServiceImplNeedleBead_ModuleInputRead(INPUT_NEEDLE_BEAD_NAME name,SENSOR_STATE* state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //输入读取
    errorCode = ServiceImplModuleInputRead((uint8_t)name,MODULE_NEEDLE_BEAD_INPUT_MAP_ARRAY_LENGTH,(MODULE_INPUT_MAP_UNIT*)moduleNeedleBeadInputMapArray,state);
    if(errorCode != LH_ERR_NONE)
    {
        if(errorCode == LH_ERR_BOARD_MAIN_COMM_INPUT_PIN_INDEX)
        {
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_INPUT_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
        else
        {
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_INPUT_READ;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        }
    }
    return errorCode;
}


//模块输出配置映射
static const MODULE_OUTPUT_MAP_UNIT moduleNeedleBeadOutputMapArray[] = {
    {MODULE_BASE_OUTPUT_PIN_RESERVE    ,VALID_LEVEL_MODULE_BASE_OUTPUT_PIN_RESERVE},
};
//配置数组长度
#define MODULE_NEEDLE_BEAD_OUTPUT_MAP_UNIT_LENGTH   (sizeof(moduleNeedleBeadOutputMapArray)/sizeof(moduleNeedleBeadOutputMapArray[0]))
/*******************************************************************
**description: 写入指定输出 
******************************************************************/
static LH_ERR ServiceImplNeedleBead_ModuleOutputWrite(OUTPUT_NEEDLE_BEAD_NAME name,OUT_STATE state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //输出写入
        errorCode = ServiceImplModuleOutputWrite((uint8_t)name,state,MODULE_NEEDLE_BEAD_OUTPUT_MAP_UNIT_LENGTH,(MODULE_OUTPUT_MAP_UNIT*)moduleNeedleBeadOutputMapArray);
        if(errorCode != LH_ERR_NONE)
        {
            if(errorCode == LH_ERR_BOARD_MAIN_COMM_OUTPUT_PIN_INDEX)
            {
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_OUTPUT_INDEX;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
            else
            {
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_OUTPUT_WRITE;
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
typedef enum NEEDLE_BEAD_SAFE_CHECK_FLAG
{
    NEEDLE_BEAD_SAFE_CHECK_FLAG_PLATE_REACTION        = 0X00,//磁珠针在反应盘上方做安全检查
    NEEDLE_BEAD_SAFE_CHECK_FLAG_PLATE_REAGENT         = 0X01,//磁珠针在试剂盘上方做安全检查
}NEEDLE_BEAD_SAFE_CHECK_FLAG;
static LH_ERR ServiceImplNeedleBead_ModuleSafeCheckBeforeRun(uint8_t mode,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查反应盘状态
        if(mode == NEEDLE_BEAD_SAFE_CHECK_FLAG_PLATE_REACTION)
        {
            //检查反应盘状态
            if(PLATE_SAFE_EFFECTIVE == ServiceStateReadPlateReactionSafeState())
            {
                errorCode = LH_ERR_BOARD_MAIN_NEEDLE_BEAD_SAFE_PROTECT_PLATE_REACTION;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_PLATE_REACTION_SAFE;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                return errorCode;
            }
        }
    //检查试剂盘状态
        else if(mode == NEEDLE_BEAD_SAFE_CHECK_FLAG_PLATE_REAGENT)
        {
            //检查试剂盘状态
            if(PLATE_SAFE_EFFECTIVE == ServiceStateReadPlateReagentSafeState())
            {
                errorCode = LH_ERR_BOARD_MAIN_NEEDLE_BEAD_SAFE_PROTECT_PLATE_REAGENT;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_PLATE_REAGENT_SAFE;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                return errorCode;
            }
        }
    return LH_ERR_NONE;
}
/*******************************************************************
**description: 急停触发读取磁珠针防撞高度 
******************************************************************/
static LH_ERR ServiceImplNeedleBead_ReadNeedleEmStopUpDownPos(LH_ERR actionErrorCode)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint16_t needleEmStopPos = 0;
    //查看错误是否为防撞触发
    if((actionErrorCode & NEEDLE_EMSTOP_ERROR_CODE_MASK) == NEEDLE_EMSTOP_ERROR_CODE)
    {
        //读取防撞高度
        errorCode = Can2SubBoard2NeedleBead_ReadNeedleUpDownEmStopPos(&needleEmStopPos);
        //保存防撞高度
        ServiceStateWriteLastNeedleBeadUpDownEmStopPos(needleEmStopPos);
        //防撞触发,高度打印
        SystemPrintf("E>Ser_NB CrashPos:%d,Code:%08X\r\n",needleEmStopPos,actionErrorCode);
    }
    //返回
    return errorCode;
}

/*******************************************************************
**description: 自动运行状态机初始化 
******************************************************************/
LH_ERR ServiceImplNeedleBead_StateMachineInit(void)
{
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 自动运行状态机轮转 
******************************************************************/
LH_ERR ServiceImplNeedleBead_StateMachineLoop(void)
{
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 磁珠针复位 
******************************************************************/
LH_ERR ServiceImplNeedleBead_ResetAll(uint16_t* pressureSensorValue,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint8_t retryCount = 0;
    int32_t paramUtil = 0;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //复位
        errorCode = Can2SubBoard2NeedleBead_Reset();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_RESET;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //探液位置参数获取
        retryCount = 0;
        do
        {
            IPC_ServiceAssistReadActionParamWhileReturn(PARAM_MAIN_INDEX_NEEDLE_BEAD_LIQUID_DETECT_MAX_POS_R1,PARAM_SUB_INDEX_NEEDLE_BEAD_LIQUID_DETECT_MAX_POS_R1,
                                                    &paramUtil,commErrorLevel,commErrorCode);
            if(errorCode != LH_ERR_NONE)
            {
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_RESET;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                //错误代码打印
                ServicePrintfShowError(errorCode);
                return errorCode;
            }
            retryCount++;
        }while(retryCount < 5 && paramUtil == 0);
        //R1
        
        ServiceStateWriteNeedleBeadMaxLiquidDetectPosR1(paramUtil);
        retryCount = 0;
        do
        {
            //R2
            IPC_ServiceAssistReadActionParamWhileReturn(PARAM_MAIN_INDEX_NEEDLE_BEAD_LIQUID_DETECT_MAX_POS_R2,PARAM_SUB_INDEX_NEEDLE_BEAD_LIQUID_DETECT_MAX_POS_R2,
                                                        &paramUtil,commErrorLevel,commErrorCode);
            if(errorCode != LH_ERR_NONE)
            {
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_RESET;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                //错误代码打印
                ServicePrintfShowError(errorCode);
                return errorCode;
            }
            retryCount++;
        }while(retryCount < 5 && paramUtil == 0);
        
        ServiceStateWriteNeedleBeadMaxLiquidDetectPosR2(paramUtil);
        retryCount = 0;
        do
        {
            //BEAD
            IPC_ServiceAssistReadActionParamWhileReturn(PARAM_MAIN_INDEX_NEEDLE_BEAD_LIQUID_DETECT_MAX_POS_BEAD,PARAM_SUB_INDEX_NEEDLE_BEAD_LIQUID_DETECT_MAX_POS_BEAD,
                                                        &paramUtil,commErrorLevel,commErrorCode);
            if(errorCode != LH_ERR_NONE)
            {
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_RESET;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                //错误代码打印
                ServicePrintfShowError(errorCode);
                return errorCode;
            }
            retryCount++;
        }while(retryCount < 5 && paramUtil == 0);
        
        ServiceStateWriteNeedleBeadMaxLiquidDetectPosBead(paramUtil);
    //取消磁珠针保护
        ServiceStateWriteNeedleBeadSafeState(NEEDLE_SAFE_INVALID);
    //设置当前针位置在何处
        ServiceStateWriteNeedleBeadCurrentPos(NEEDLE_BEAD_POS_CLEAN);
    //当前泵内液量清零
        ServiceStateWriteNeedleBeadCurrentPumpMinUlCount(0);
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 磁珠针旋转到指定位置 
******************************************************************/
LH_ERR ServiceImplNeedleBead_RotatePos(NEEDLE_BEAD_POS targetPos,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //运转
        errorCode = Can2SubBoard2NeedleBead_Rotate(targetPos);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_ROTATE_POS;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //设置当前针位置在何处
        ServiceStateWriteNeedleBeadCurrentPos(targetPos);
    return errorCode;
}

/*******************************************************************
**description: 磁珠针清洗 
******************************************************************/
LH_ERR ServiceImplNeedleBead_Clean(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime,uint16_t* pressureValue,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //清洗
        errorCode = Can2SubBoard2NeedleBead_Clean(cleanOpt,cleanTime);
        if(errorCode != LH_ERR_NONE)
        {
            //磁珠针防撞触发时保存防撞高度
            ServiceImplNeedleBead_ReadNeedleEmStopUpDownPos(errorCode);
            //针动作
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_CLEAN;
            //确定错误等级
            ServiceImplNeedleBead_GetErrorLevel(errorCode,commErrorLevel);
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //堵针检测
        *pressureValue = 2500;
    //堵针检测值写入状态
        ServiceStateWriteNeedleBeadLastPressureValue(*pressureValue);
    return errorCode;
}

/*******************************************************************
**description: 磁珠针灌注 
******************************************************************/
LH_ERR ServiceImplNeedleBead_Prime(NEEDLE_PRIME_OPT primeOpt,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //灌注
        errorCode = Can2SubBoard2NeedleBead_Prime(primeOpt);
        if(errorCode != LH_ERR_NONE)
        {
            //磁珠针防撞触发时保存防撞高度
            ServiceImplNeedleBead_ReadNeedleEmStopUpDownPos(errorCode);
            //针动作
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_PRIME;
            //确定错误等级
            ServiceImplNeedleBead_GetErrorLevel(errorCode,commErrorLevel);
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    return errorCode;
}

//转换液面探测的结果
static LH_ERR ServiceImplNeedleBead_ConvertLiquidDetectResult(LIQUID_NEEDLE_LIQUID_DETECT_RESULT NeedleLiquidDetectResult,LIQUID_FUNC_RESULT liquidResultRerurn,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //没有探测到液面的情况
        if(NeedleLiquidDetectResult == 0)
        {
            if(liquidResultRerurn == LIQUID_FUNC_RESULT_OK)
            {
                //探液打开成功但是探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_LIQUID_DETECT;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_OPEN_FAIL)
            {
                //探液打开失败且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_OPEN_FAIL_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_50_V)
            {
                //探液打开基准电压5.0V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_50_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_45_50_V)
            {
                //探液打开基准电压4.5-5.0V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_45_50_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_40_45_V)
            {
                //探液打开基准电压4.0-4.5V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_40_45_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_35_40_V)
            {
                //探液打开基准电压3.5-4.0V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_35_40_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_30_35_V)
            {
                //探液打开基准电压3.0-3.5V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_30_35_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_29_30_V)
            {
                //探液打开基准电压2.9-3.0V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_29_30_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_20_21_V)
            {
                //探液打开基准电压2.0-2.1V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_20_21_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_15_20_V)
            {
                //探液打开基准电压1.5-2.0V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_15_20_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_10_15_V)
            {
                //探液打开基准电压1.0-1.5V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_10_15_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_5_10_V)
            {
                //探液打开基准电压0.5-1.0V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_5_10_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_0_5_V)
            {
                //探液打开基准电压0-0.5V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_0_5_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_0_V)
            {
                //探液打开基准电压0V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_0_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            return LH_ERR_NONE;
        }
    //探测到液面的情况
        else
        {
            //探液探到了
            if(liquidResultRerurn == LIQUID_FUNC_RESULT_OK)
            {
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_OPEN_FAIL)
            {
                //探液打开失败但是探液成功
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_OPEN_FAIL_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_50_V)
            {
                //探液成功但是基准电压5.0V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_50_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_45_50_V)
            {
                //探液成功,但是基准电压4.5-5.0V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_45_50_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_40_45_V)
            {
                //探液成功,但是基准电压4.0-4.5V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_40_45_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_35_40_V)
            {
                //探液成功,但是基准电压3.5-4.0V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_35_40_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_30_35_V)
            {
                //探液成功,但是基准电压3.0-3.5V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_30_35_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_29_30_V)
            {
                //探液成功,但是基准电压2.9-3.0V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_29_30_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_20_21_V)
            {
                //探液成功,但是基准电压2.0-2.1V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_20_21_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_15_20_V)
            {
                //探液成功,但是基准电压1.5-2.0V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_15_20_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_10_15_V)
            {
                //探液成功,但是基准电压1.0-1.5V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_10_15_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_5_10_V)
            {
                //探液成功,但是基准电压0.5-1.0V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_5_10_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_0_5_V)
            {
                //探液成功,但是基准电压0-0.5V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_0_5_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_0_V)
            {
                //探液成功,但是基准电压0V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_BASE_0_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            return LH_ERR_NONE;
        }
}

//试剂余量探测转换液面探测的结果
static LH_ERR ServiceImplNeedleBead_LiquidResidueCheckConvertLiquidDetectResult(LIQUID_NEEDLE_LIQUID_DETECT_RESULT NeedleLiquidDetectResult,LIQUID_FUNC_RESULT liquidResultRerurn,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //没有探测到液面的情况
        if(NeedleLiquidDetectResult == 0)
        {
            if(liquidResultRerurn == LIQUID_FUNC_RESULT_OK)
            {
                //探液打开成功但是探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_LIQUID_RESIDUE_CHECK_LQIUID_DETECT;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_OPEN_FAIL)
            {
                //探液打开失败且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_OPEN_FAIL_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_50_V)
            {
                //探液打开基准电压5.0V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_50_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_45_50_V)
            {
                //探液打开基准电压4.5-5.0V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_45_50_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_40_45_V)
            {
                //探液打开基准电压4.0-4.5V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_40_45_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_35_40_V)
            {
                //探液打开基准电压3.5-4.0V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_35_40_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_30_35_V)
            {
                //探液打开基准电压3.0-3.5V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_30_35_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_29_30_V)
            {
                //探液打开基准电压2.9-3.0V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_29_30_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_20_21_V)
            {
                //探液打开基准电压2.0-2.1V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_20_21_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_15_20_V)
            {
                //探液打开基准电压1.5-2.0V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_15_20_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_10_15_V)
            {
                //探液打开基准电压1.0-1.5V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_10_15_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_5_10_V)
            {
                //探液打开基准电压0.5-1.0V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_5_10_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_0_5_V)
            {
                //探液打开基准电压0-0.5V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_0_5_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_0_V)
            {
                //探液打开基准电压0V,且探液没探到
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_0_V_DETECT_FAIL;
                return LH_ERR_NONE;
            }
            return LH_ERR_NONE;
        }
    //探测到液面的情况
        else
        {
            //探液探到了
            if(liquidResultRerurn == LIQUID_FUNC_RESULT_OK)
            {
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_OPEN_FAIL)
            {
                //探液打开失败但是探液成功
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_OPEN_FAIL_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_50_V)
            {
                //探液成功但是基准电压5.0V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_50_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_45_50_V)
            {
                //探液成功,但是基准电压4.5-5.0V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_45_50_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_40_45_V)
            {
                //探液成功,但是基准电压4.0-4.5V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_40_45_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_35_40_V)
            {
                //探液成功,但是基准电压3.5-4.0V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_35_40_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_30_35_V)
            {
                //探液成功,但是基准电压3.0-3.5V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_30_35_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_29_30_V)
            {
                //探液成功,但是基准电压2.9-3.0V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_29_30_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_20_21_V)
            {
                //探液成功,但是基准电压2.0-2.1V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_20_21_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_15_20_V)
            {
                //探液成功,但是基准电压1.5-2.0V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_15_20_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_10_15_V)
            {
                //探液成功,但是基准电压1.0-1.5V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_10_15_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_5_10_V)
            {
                //探液成功,但是基准电压0.5-1.0V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_5_10_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_0_5_V)
            {
                //探液成功,但是基准电压0-0.5V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_0_5_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            else if(liquidResultRerurn == LIQUID_FUNC_RESULT_BASE_0_V)
            {
                //探液成功,但是基准电压0V
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
                *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_LIQUID_RESIDUE_BASE_0_V_DETECT_OK;
                return LH_ERR_NONE;
            }
            return LH_ERR_NONE;
        }
}

/*******************************************************************
**description: 磁珠针吸液 
******************************************************************/
LH_ERR ServiceImplNeedleBead_Absorb(uint16_t absorbCountMinUL,uint8_t useLiquidRecord,uint16_t* liquidDetectPos,uint16_t* pressureValue,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //试剂盘安全检查
        errorCode = ServiceImplNeedleBead_ModuleSafeCheckBeforeRun(NEEDLE_BEAD_SAFE_CHECK_FLAG_PLATE_REAGENT,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //获取探液高度
        uint16_t lastLiquidDetectPos = 0;
    //注射泵容量，最小微升数表示
        uint16_t pumpMinUlCount = 0;
    //下发探液段
        PLATE_REAGENT_LIQUID_DETECT_STAGE liquidDetectStage = PLATE_REAGENT_LIQUID_DETECT_STAGE_LARGE;
    //返回的探液段
        PLATE_REAGENT_LIQUID_DETECT_STAGE liquidDetectStageReturn;
    //探液板打开结果返回
        LIQUID_FUNC_RESULT liquidResultRerurn;
    //液面探测结果
        LIQUID_NEEDLE_LIQUID_DETECT_RESULT needleLiquidDetectResult;
    //液面探测时针电压返回    
        LIQUID_DETECT_NEEDLE_VOL_UNIT needleVolReturn;
    //当前试剂盘序号
        uint8_t currentPlateReagentIndex = ServiceStateReadPlateReagentIndex();
    //计算获取偏移
        LIQUID_DETECT_OFFSET liquidOffset;
        if(NEEDLE_BEAD_POS_R1 == ServiceStateReadNeedleBeadCurrentPos())
        {
            liquidOffset = LIQUID_DETECT_OFFSET_NEEDLE_BEAD_R1;
        }
        else if(NEEDLE_BEAD_POS_R2 == ServiceStateReadNeedleBeadCurrentPos())
        {
            liquidOffset = LIQUID_DETECT_OFFSET_NEEDLE_BEAD_R2;
        }
        else if(NEEDLE_BEAD_POS_BEAD == ServiceStateReadNeedleBeadCurrentPos())
        {
            liquidOffset = LIQUID_DETECT_OFFSET_NEEDLE_BEAD_BEAD;
        }
        else
        {
            errorCode = LH_ERR_BOARD_MAIN_NEEDLE_BEAD_POS_LIMIT;
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_ABSORB;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //检查试剂盘偏移是否正确
        if(liquidOffset == LIQUID_DETECT_OFFSET_NEEDLE_BEAD_R1)
        {
            //针的状态是R1,试剂盘的偏移也要是R1
            if(PLATE_REAGENT_OFFSET_NEEDLE_BEAD_R1 != ServiceStateReadPlateReagentOffset())
            {
                errorCode = LH_ERR_BOARD_MAIN_NEEDLE_BEAD_PLATE_REAGENT_OFFSET_LIMIT;
            }
        }
        if(liquidOffset == LIQUID_DETECT_OFFSET_NEEDLE_BEAD_R2)
        {
            //针的状态是R2,试剂盘的偏移也要是R2
            if(PLATE_REAGENT_OFFSET_NEEDLE_BEAD_R2 != ServiceStateReadPlateReagentOffset())
            {
                errorCode = LH_ERR_BOARD_MAIN_NEEDLE_BEAD_PLATE_REAGENT_OFFSET_LIMIT;
            }
        }
        if(liquidOffset == LIQUID_DETECT_OFFSET_NEEDLE_BEAD_BEAD)
        {
            //针的状态是磁珠位置,试剂盘的偏移也要是磁珠位
            if(PLATE_REAGENT_OFFSET_NEEDLE_BEAD_BEAD != ServiceStateReadPlateReagentOffset())
            {
                errorCode = LH_ERR_BOARD_MAIN_NEEDLE_BEAD_PLATE_REAGENT_OFFSET_LIMIT;
            }
        }
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_ABSORB;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //获取当前磁珠针下方试剂的液面高度
        if(useLiquidRecord == 0)
        {
            lastLiquidDetectPos = 0;
            liquidDetectStage = PLATE_REAGENT_LIQUID_DETECT_STAGE_LARGE;
        }
        else
        {
            lastLiquidDetectPos = (uint16_t)ServiceStateReadPlateReagentLiquidDetectPos(currentPlateReagentIndex,liquidOffset);
            liquidDetectStage = ServiceStateReadPlateReagentLiquidDetectStage(currentPlateReagentIndex,liquidOffset);
        }
    //磁珠针在试剂盘保护
        ServiceStateWriteNeedleBeadSafeState(NEEDLE_SAFE_EFFECTIVE_PLATE_REAGENT);
    //吸液一次
        errorCode = Can2SubBoard2NeedleBead_LiquidAbsorb(absorbCountMinUL,lastLiquidDetectPos,(uint8_t)liquidDetectStage,liquidDetectPos,
                                                            &pumpMinUlCount,&liquidResultRerurn,&needleLiquidDetectResult,(uint8_t*)&liquidDetectStageReturn,&needleVolReturn);
        if(errorCode != LH_ERR_NONE)
        {
            //磁珠针防撞触发时保存防撞高度
            ServiceImplNeedleBead_ReadNeedleEmStopUpDownPos(errorCode);
            //针动作
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_ABSORB;
            //确定错误等级
            ServiceImplNeedleBead_GetErrorLevel(errorCode,commErrorLevel);
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //打印针电压结果
        SystemPrintf("Ser_NB LD%s,BV:%5d,NV:%5d\r\n",(needleLiquidDetectResult ? "OK":"NG"),needleVolReturn.liquidDetectBaseVol,needleVolReturn.liquidDetectNeedleVol);
    //取消安全保护
        ServiceStateWriteNeedleBeadSafeState(NEEDLE_SAFE_INVALID);
    //数据写入缓存,液量和探液高度
        if(needleLiquidDetectResult == LIQUID_NEEDLE_LIQUID_DETECT_SUCCEED)
        {
            //写入磁珠针液面探测成功
            ServiceStateWritePlateReagentLiquidDetectPos(currentPlateReagentIndex,liquidOffset,*liquidDetectPos);
            //写入探液失败的探液高度0
            ServiceStateWritePlateReagentLiquidDetectFailedPos(currentPlateReagentIndex,liquidOffset,0);
        }else
        {
            //探液成功高度清0
            ServiceStateWritePlateReagentLiquidDetectPos(currentPlateReagentIndex,liquidOffset,0);
            //写入探液失败的探液高度
            ServiceStateWritePlateReagentLiquidDetectFailedPos(currentPlateReagentIndex,liquidOffset,*liquidDetectPos);
        }
        //探液段落发生变化,写入状态
        if(liquidDetectStage != liquidDetectStageReturn)
        {
            ServiceStateWritePlateReagentLiquidDetectStage(currentPlateReagentIndex,liquidOffset,liquidDetectStageReturn);
        }
    //保存当前泵容量
        ServiceStateWriteNeedleBeadCurrentPumpMinUlCount(pumpMinUlCount);
    //堵针检测
        *pressureValue = (uint16_t)2500;
    //堵针检测值写入状态
        ServiceStateWriteNeedleBeadLastPressureValue(*pressureValue);
    //根据液面探测打开的结果,综合进行报警,仅在正常模式下检测
        if(MODE_RELEASE == ServiceStateReadAssistRunningMode())
        {
            errorCode = ServiceImplNeedleBead_ConvertLiquidDetectResult(needleLiquidDetectResult,liquidResultRerurn,commErrorCode,commErrorLevel);
        }
        
    return errorCode;
}

/*******************************************************************
**description: 磁珠针试剂余量校准
******************************************************************/
LH_ERR ServiceImplNeedleBead_LiquidResidueCheck(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //试剂盘安全检查
        errorCode = ServiceImplNeedleBead_ModuleSafeCheckBeforeRun(NEEDLE_BEAD_SAFE_CHECK_FLAG_PLATE_REAGENT,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
        //探液返回值
        uint16_t LiquidDetectPos = 0;
        LIQUID_FUNC_RESULT liquidResultRerurn = LIQUID_FUNC_RESULT_OK;
        LIQUID_NEEDLE_LIQUID_DETECT_RESULT needleLiquidDetectResult = LIQUID_NEEDLE_LIQUID_DETECT_SUCCEED;
    //当前试剂盘序号
        uint8_t currentPlateReagentIndex = ServiceStateReadPlateReagentIndex();
    //计算获取偏移
        LIQUID_DETECT_OFFSET liquidOffset;
        if(NEEDLE_BEAD_POS_R1 == ServiceStateReadNeedleBeadCurrentPos())
        {
            liquidOffset = LIQUID_DETECT_OFFSET_NEEDLE_BEAD_R1;
        }
        else if(NEEDLE_BEAD_POS_R2 == ServiceStateReadNeedleBeadCurrentPos())
        {
            liquidOffset = LIQUID_DETECT_OFFSET_NEEDLE_BEAD_R2;
        }
        else if(NEEDLE_BEAD_POS_BEAD == ServiceStateReadNeedleBeadCurrentPos())
        {
            liquidOffset = LIQUID_DETECT_OFFSET_NEEDLE_BEAD_BEAD;
        }
        else
        {
            errorCode = LH_ERR_BOARD_MAIN_NEEDLE_BEAD_POS_LIMIT;
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_LIQUID_RESIDUE_CHECK;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //检查试剂盘偏移是否正确
        if(liquidOffset == LIQUID_DETECT_OFFSET_NEEDLE_BEAD_R1)
        {
            //针的状态是R1,试剂盘的偏移也要是R1
            if(PLATE_REAGENT_OFFSET_NEEDLE_BEAD_R1 != ServiceStateReadPlateReagentOffset())
            {
                errorCode = LH_ERR_BOARD_MAIN_NEEDLE_BEAD_PLATE_REAGENT_OFFSET_LIMIT;
            }
        }
        if(liquidOffset == LIQUID_DETECT_OFFSET_NEEDLE_BEAD_R2)
        {
            //针的状态是R2,试剂盘的偏移也要是R2
            if(PLATE_REAGENT_OFFSET_NEEDLE_BEAD_R2 != ServiceStateReadPlateReagentOffset())
            {
                errorCode = LH_ERR_BOARD_MAIN_NEEDLE_BEAD_PLATE_REAGENT_OFFSET_LIMIT;
            }
        }
        if(liquidOffset == LIQUID_DETECT_OFFSET_NEEDLE_BEAD_BEAD)
        {
            //针的状态是磁珠位置,试剂盘的偏移也要是磁珠位
            if(PLATE_REAGENT_OFFSET_NEEDLE_BEAD_BEAD != ServiceStateReadPlateReagentOffset())
            {
                errorCode = LH_ERR_BOARD_MAIN_NEEDLE_BEAD_PLATE_REAGENT_OFFSET_LIMIT;
            }
        }
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_LIQUID_RESIDUE_CHECK;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
        //磁珠针在试剂盘保护
        ServiceStateWriteNeedleBeadSafeState(NEEDLE_SAFE_EFFECTIVE_PLATE_REAGENT);
        //发送余量探测指令
        errorCode = Can2SubBoard2NeedleBead_LiquidResidueCheck(&LiquidDetectPos,&liquidResultRerurn,&needleLiquidDetectResult);
        if(errorCode != LH_ERR_NONE)
        {
            //磁珠针防撞触发时保存防撞高度
            ServiceImplNeedleBead_ReadNeedleEmStopUpDownPos(errorCode);
            //针动作
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_LIQUID_RESIDUE_CHECK;
            //确定错误等级
            ServiceImplNeedleBead_GetErrorLevel(errorCode,commErrorLevel);
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
   //取消安全保护
        ServiceStateWriteNeedleBeadSafeState(NEEDLE_SAFE_INVALID);
    //数据写入缓存,液量和探液高度
        if(needleLiquidDetectResult == LIQUID_NEEDLE_LIQUID_DETECT_SUCCEED)
        {
            //写入磁珠针液面探测成功
            ServiceStateWritePlateReagentLiquidDetectPos(currentPlateReagentIndex,liquidOffset,LiquidDetectPos);
            //写入探液失败的探液高度0
            ServiceStateWritePlateReagentLiquidDetectFailedPos(currentPlateReagentIndex,liquidOffset,0);
        }else
        {
            //探液成功高度清0
            ServiceStateWritePlateReagentLiquidDetectPos(currentPlateReagentIndex,liquidOffset,0);
            //写入探液失败的探液高度
            ServiceStateWritePlateReagentLiquidDetectFailedPos(currentPlateReagentIndex,liquidOffset,LiquidDetectPos);
        }
    //根据液面探测打开的结果,综合进行报警,仅在正常模式下检测
        if(MODE_RELEASE == ServiceStateReadAssistRunningMode())
        {
            errorCode = ServiceImplNeedleBead_LiquidResidueCheckConvertLiquidDetectResult(needleLiquidDetectResult,liquidResultRerurn,commErrorCode,commErrorLevel);
        }
        return errorCode;
}

/*******************************************************************
**description: 磁珠针注液 
******************************************************************/
LH_ERR ServiceImplNeedleBead_Inject(uint32_t cupLiquidUl,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //反应盘上安全检查
        errorCode = ServiceImplNeedleBead_ModuleSafeCheckBeforeRun(NEEDLE_BEAD_SAFE_CHECK_FLAG_PLATE_REACTION,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //磁珠针在反应盘保护
        ServiceStateWriteNeedleBeadSafeState(NEEDLE_SAFE_EFFECTIVE_PLATE_REACTION);
        SystemPrintf("NB_Inject:%d\r\n",cupLiquidUl);
    //反应盘注液,不带混匀
        errorCode = Can2SubBoard2NeedleBead_LiquidInject(cupLiquidUl,0);
        if(errorCode != LH_ERR_NONE)
        {
            //磁珠针防撞触发时保存防撞高度
            ServiceImplNeedleBead_ReadNeedleEmStopUpDownPos(errorCode);
            //针动作
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_INJECT;
            //确定错误等级
            ServiceImplNeedleBead_GetErrorLevel(errorCode,commErrorLevel);
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //取消安全保护
        ServiceStateWriteNeedleBeadSafeState(NEEDLE_SAFE_INVALID);
    return errorCode;
}

/*******************************************************************
**description: 磁珠针注液带混匀 
******************************************************************/
LH_ERR ServiceImplNeedleBead_InjectMix(uint32_t cupLiquidUl,uint8_t mixCount,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //反应盘上安全检查
        errorCode = ServiceImplNeedleBead_ModuleSafeCheckBeforeRun(NEEDLE_BEAD_SAFE_CHECK_FLAG_PLATE_REACTION,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //磁珠针在反应盘保护
        ServiceStateWriteNeedleBeadSafeState(NEEDLE_SAFE_EFFECTIVE_PLATE_REACTION);
        SystemPrintf("NBM_Inject:%d,%d\r\n",cupLiquidUl,mixCount);
    //反应盘注液,不带混匀
        errorCode = Can2SubBoard2NeedleBead_LiquidInject(cupLiquidUl,mixCount);
        if(errorCode != LH_ERR_NONE)
        {
            //磁珠针防撞触发时保存防撞高度
            ServiceImplNeedleBead_ReadNeedleEmStopUpDownPos(errorCode);
            //针动作
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_INJECT;
            //确定错误等级
            ServiceImplNeedleBead_GetErrorLevel(errorCode,commErrorLevel);
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //取消安全保护
        ServiceStateWriteNeedleBeadSafeState(NEEDLE_SAFE_INVALID);
    return errorCode;
}

/*******************************************************************
**description: 磁珠针清洗维护 
******************************************************************/
LH_ERR ServiceImplNeedleBead_MainTain(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //磁珠针管路维护
        errorCode = Can2SubBoard2NeedleBead_MaintainClean(cleanOpt,cleanTime);
        if(errorCode != LH_ERR_NONE)
        {
            //磁珠针防撞触发时保存防撞高度
            ServiceImplNeedleBead_ReadNeedleEmStopUpDownPos(errorCode);
            //针动作
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_MAINTAIN;
            //确定错误等级
            ServiceImplNeedleBead_GetErrorLevel(errorCode,commErrorLevel);
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    return errorCode;
}

/*******************************************************************
**description: 磁珠针探液校准 
******************************************************************/
LH_ERR ServiceImplNeedleBead_Adjust(CMD_SOURCE_TEST_FLOW cmdSource,uint16_t* needleLiquidDetectVol,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //磁珠针校准
        errorCode = Can2SubBoard2NeedleBead_LiquidDetectAdjust();
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_ADJUST;
            //确定错误等级
            ServiceImplNeedleBead_GetErrorLevel(errorCode,commErrorLevel);
            //主流程调用固定报注意级
            if(cmdSource == CMD_SOURCE_TEST_FLOW_MAIN)
            {
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            }
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //读取基准电压
        errorCode = Can2SubBoard2NeedleBead_LiquidDetectReadNeedleBaseVol(needleLiquidDetectVol);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_MODULE_NEEDLE_VOL;
            //确定错误等级
            ServiceImplNeedleBead_GetErrorLevel(errorCode,commErrorLevel);
            //主流程调用固定报注意级
            if(cmdSource == CMD_SOURCE_TEST_FLOW_MAIN)
            {
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            }
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //写入电压值
        ServiceStateWriteNeedleBeadAdjustNeedleVoltage(*needleLiquidDetectVol);
    return errorCode;
}

/*******************************************************************
**description: 读取模块内部指定输入的状态 
******************************************************************/
LH_ERR ServiceImplNeedleBead_InputRead(uint8_t inputIndex,SENSOR_STATE* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = ServiceImplNeedleBead_ModuleInputRead((INPUT_NEEDLE_BEAD_NAME)inputIndex,inputState,commErrorCode,commErrorLevel);
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
LH_ERR ServiceImplNeedleBead_OutputWrite(uint8_t outputIndex,OUT_STATE setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = ServiceImplNeedleBead_ModuleOutputWrite((OUTPUT_NEEDLE_BEAD_NAME)outputIndex,setState,commErrorCode,commErrorLevel);
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
LH_ERR ServiceImplNeedleBead_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //判定参数,主要是电机序号
        if(motorIndex > INDEX_MOTOR_NEEDLE_BEAD_PUMP)
        {
            errorCode = LH_ERR_BOARD_MAIN_NEEDLE_BEAD_MOTOR_INDEX;
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_STEP_MOTOR_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //指定步进电机复位带复位修正
        INDEX_MOTOR_NEEDLE_BEAD stepMotorIndex = (INDEX_MOTOR_NEEDLE_BEAD)(motorIndex);
        errorCode = Can2SubBoard2NeedleBead_StepMotorReset(stepMotorIndex);
    //读取失败,错误处理
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_STEP_MOTOR_SINGLE_RESET;
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
LH_ERR ServiceImplNeedleBead_MotorRunSteps(uint8_t motorIndex,int32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //判定参数,主要是电机序号
        if(motorIndex > INDEX_MOTOR_NEEDLE_BEAD_PUMP)
        {
            errorCode = LH_ERR_BOARD_MAIN_NEEDLE_BEAD_MOTOR_INDEX;
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_STEP_MOTOR_INDEX;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //试剂盘电机走步数
        INDEX_MOTOR_NEEDLE_BEAD stepMotorIndex = (INDEX_MOTOR_NEEDLE_BEAD)(motorIndex);
        errorCode = Can2SubBoard2NeedleBead_StepMotorRunSteps(stepMotorIndex,steps,posAfterRun);
    //读取失败,错误处理
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_NEEDLE_BEAD_STEP_MOTOR_SINGLE_RUN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //返回数据
        return errorCode;
}




























