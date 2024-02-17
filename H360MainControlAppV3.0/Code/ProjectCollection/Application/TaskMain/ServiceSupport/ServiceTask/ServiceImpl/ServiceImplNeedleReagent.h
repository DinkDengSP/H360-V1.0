/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:07:50
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-25 09:52:06
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplNeedleReagent.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_NEEDLE_REAGENT_H_
#define __SERVICE_IMPL_NEEDLE_REAGENT_H_
#include "ServiceImplNeedleReagentConfig.h"

//试剂针防撞错误代码
#define ERROR_CODE_NEEDLE_REAGENT_EMERGENCY_STOP    0X43140504
//试剂针动作层错误参数
typedef enum FLAG_NEEDLE_REAGENT_ERR_CONVERT
{
    FLAG_NEEDLE_REAGENT_ERR_UPDOWN_RESET                 = 0X01,//试剂针升降复位错误
    FLAG_NEEDLE_REAGENT_ERR_ROTATE_RESET                 = 0X02,//试剂针旋转复位错误
    FLAG_NEEDLE_REAGENT_ERR_PUMP_RESET                   = 0X03,//试剂针注射泵复位错误
    FLAG_NEEDLE_REAGENT_ERR_LIQUID_DETECT_CHECK          = 0X04,//试剂针液面探测在线检查错误
    FLAG_NEEDLE_REAGENT_ERR_UPDOWN_RUN                   = 0X05,//试剂针升降运行失败
    FLAG_NEEDLE_REAGENT_ERR_ROTATE_RUN                   = 0X06,//试剂针旋转运行失败
    FLAG_NEEDLE_REAGENT_ERR_PUMP_RUN                     = 0X07,//试剂针注射泵运行失败
    FLAG_NEEDLE_REAGENT_ERR_UPDOWN_RETURN                = 0X08,//试剂针升降回零失败
    FLAG_NEEDLE_REAGENT_ERR_ROTATE_RETURN                = 0X09,//试剂针旋转回零失败
    FLAG_NEEDLE_REAGENT_ERR_PUMP_RETURN                  = 0X0A,//试剂针注射泵回零失败
    FLAG_NEEDLE_REAGENT_ERR_OPEN_LIQUID_CLEAN            = 0X0B,//试剂针清洗打开液路失败
    FLAG_NEEDLE_REAGENT_ERR_CLOSE_LIQUID_CLEAN           = 0X0C,//试剂针清洗关闭液路失败
    FLAG_NEEDLE_REAGENT_ERR_OPEN_LIQUID_PRIME            = 0X0D,//试剂针灌注打开液路失败
    FLAG_NEEDLE_REAGENT_ERR_CLOSE_LIQUID_PRIME           = 0X0E,//试剂针灌注关闭液路失败
    FLAG_NEEDLE_REAGENT_ERR_OPEN_LIQUID_MAINTAIN         = 0X0F,//试剂针清洗维护打开液路失败
    FLAG_NEEDLE_REAGENT_ERR_CLOSE_LIQUID_MAINTAIN        = 0X10,//试剂针清洗维护关闭液路失败
    FLAG_NEEDLE_REAGENT_ERR_RESET_LIQUID                 = 0X11,//试剂针液路单元复位
    FLAG_NEEDLE_REAGENT_ERR_POS_LIMIT                    = 0X12,//试剂针当前位置不能执行指定操作
    FLAG_NEEDLE_REAGENT_ERR_LIQUID_COMM                  = 0X13,//试剂针液面探测通讯失败
    FLAG_NEEDLE_REAGENT_ERR_UPDOWN_EMERGENCY             = 0X14,//升降防撞
    FLAG_NEEDLE_REAGENT_ERR_ROTATE_TARGET                = 0X15,//旋转目标错误
    FLAG_NEEDLE_REAGENT_ERR_PUMP_MAX                     = 0X16,//注射泵容量限制
    FLAG_NEEDLE_REAGENT_ERR_LIQUID_ADJUST_FAIL           = 0X17,//试剂针液面探测校准失败,找不到合适的值
    FLAG_NEEDLE_REAGENT_ERR_LIQUID_ADJUST_MAX_NOT_REACH  = 0X18,//试剂针液面探测校准达不到最大值
    FLAG_NEEDLE_REAGENT_ERR_LIQUID_ADJUST_MIN_NOT_REACH  = 0X19,//试剂针液面探测校准达不到最小值
    FLAG_NEEDLE_REAGENT_ERR_PUMP_NOT_ENOUGH              = 0X1A,//试剂针泵内液量不足
    FLAG_NEEDLE_REAGENT_ERR_CRASH_AT_PLATE_REACTION      = 0X1B,//试剂针反应盘防撞
    FLAG_NEEDLE_REAGENT_ERR_CRASH_AT_CLEAN               = 0X1C,//试剂针清洗位置防撞
    FLAG_NEEDLE_REAGENT_ERR_CRASH_AT_PLATE_REAGENT       = 0X1D,//试剂针试剂盘防撞
}FLAG_NEEDLE_REAGENT_ERR_CONVERT;

//自动运行状态机初始化
LH_ERR ServiceImplNeedleReagent_StateMachineInit(void);

//自动运行状态机轮转
LH_ERR ServiceImplNeedleReagent_StateMachineLoop(void);

//试剂针复位
LH_ERR ServiceImplNeedleReagent_ResetAll(uint16_t* pressureSensorValue,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//试剂针旋转到指定位置
LH_ERR ServiceImplNeedleReagent_RotatePos(NEEDLE_REAGENT_POS targetPos,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//试剂针清洗
LH_ERR ServiceImplNeedleReagent_Clean(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime,uint16_t* pressureValue,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//试剂针灌注
LH_ERR ServiceImplNeedleReagent_Prime(NEEDLE_PRIME_OPT primeOpt,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//试剂针吸液
LH_ERR ServiceImplNeedleReagent_Absorb(uint16_t absorbCountMinUL,uint8_t useLiquidRecord,uint16_t* liquidDetectPos,uint16_t* pressureValue,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//试剂针试剂余量探测
LH_ERR ServiceImplNeedleReagent_LiquidResidueCheck(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//试剂针注液
LH_ERR ServiceImplNeedleReagent_Inject(uint32_t cupLiquidUl,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//试剂针注液带混匀
LH_ERR ServiceImplNeedleReagent_InjectMix(uint32_t cupLiquidUl,uint8_t mixCount,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//试剂针管路维护
LH_ERR ServiceImplNeedleReagent_MainTain(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//试剂针探液校准
LH_ERR ServiceImplNeedleReagent_Adjust(CMD_SOURCE_TEST_FLOW cmdSource,uint16_t* needleLiquidDetectVol,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//读取模块内部指定输入的状态
LH_ERR ServiceImplNeedleReagent_InputRead(uint8_t inputIndex,SENSOR_STATE* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//写入模块指定输出的状态
LH_ERR ServiceImplNeedleReagent_OutputWrite(uint8_t outputIndex,OUT_STATE setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特定电机复位
LH_ERR ServiceImplNeedleReagent_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特动电机运行指定步数
LH_ERR ServiceImplNeedleReagent_MotorRunSteps(uint8_t motorIndex,int32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);





#endif


