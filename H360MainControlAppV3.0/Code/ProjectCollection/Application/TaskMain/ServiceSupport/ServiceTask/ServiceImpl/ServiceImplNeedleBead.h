/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 14:07:42
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-25 09:55:01
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplNeedleBead.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_NEEDLE_BEAD_H_
#define __SERVICE_IMPL_NEEDLE_BEAD_H_
#include "ServiceImplNeedleBeadConfig.h"

//磁珠针防撞错误代码
#define ERROR_CODE_NEEDLE_BEAD_EMERGENCY_STOP    0X22140504
//磁珠针动作层错误参数
typedef enum FLAG_NEEDLE_BEAD_ERR_CONVERT
{
    FLAG_NEEDLE_BEAD_ERR_UPDOWN_RESET                 = 0X01,//磁珠针升降复位错误
    FLAG_NEEDLE_BEAD_ERR_ROTATE_RESET                 = 0X02,//磁珠针旋转复位错误
    FLAG_NEEDLE_BEAD_ERR_PUMP_RESET                   = 0X03,//磁珠针注射泵复位错误
    FLAG_NEEDLE_BEAD_ERR_LIQUID_DETECT_CHECK          = 0X04,//磁珠针液面探测在线检查错误
    FLAG_NEEDLE_BEAD_ERR_UPDOWN_RUN                   = 0X05,//磁珠针升降运行失败
    FLAG_NEEDLE_BEAD_ERR_ROTATE_RUN                   = 0X06,//磁珠针旋转运行失败
    FLAG_NEEDLE_BEAD_ERR_PUMP_RUN                     = 0X07,//磁珠针注射泵运行失败
    FLAG_NEEDLE_BEAD_ERR_UPDOWN_RETURN                = 0X08,//磁珠针升降回零失败
    FLAG_NEEDLE_BEAD_ERR_ROTATE_RETURN                = 0X09,//磁珠针旋转回零失败
    FLAG_NEEDLE_BEAD_ERR_PUMP_RETURN                  = 0X0A,//磁珠针注射泵回零失败
    FLAG_NEEDLE_BEAD_ERR_OPEN_LIQUID_CLEAN            = 0X0B,//磁珠针清洗打开液路失败
    FLAG_NEEDLE_BEAD_ERR_CLOSE_LIQUID_CLEAN           = 0X0C,//磁珠针清洗关闭液路失败
    FLAG_NEEDLE_BEAD_ERR_OPEN_LIQUID_PRIME            = 0X0D,//磁珠针灌注打开液路失败
    FLAG_NEEDLE_BEAD_ERR_CLOSE_LIQUID_PRIME           = 0X0E,//磁珠针灌注关闭液路失败
    FLAG_NEEDLE_BEAD_ERR_OPEN_LIQUID_MAINTAIN         = 0X0F,//磁珠针清洗维护打开液路失败
    FLAG_NEEDLE_BEAD_ERR_CLOSE_LIQUID_MAINTAIN        = 0X10,//磁珠针清洗维护关闭液路失败
    FLAG_NEEDLE_BEAD_ERR_RESET_LIQUID                 = 0X11,//磁珠针液路单元复位
    FLAG_NEEDLE_BEAD_ERR_POS_LIMIT                    = 0X12,//磁珠针当前位置不能执行指定操作
    FLAG_NEEDLE_BEAD_ERR_LIQUID_COMM                  = 0X13,//磁珠针液面探测通讯失败
    FLAG_NEEDLE_BEAD_ERR_UPDOWN_EMERGENCY             = 0X14,//升降防撞
    FLAG_NEEDLE_BEAD_ERR_ROTATE_TARGET                = 0X15,//旋转目标错误
    FLAG_NEEDLE_BEAD_ERR_PUMP_MAX                     = 0X16,//注射泵容量限制
    FLAG_NEEDLE_BEAD_ERR_LIQUID_ADJUST_FAIL           = 0X17,//磁珠针液面探测校准失败,找不到合适的值
    FLAG_NEEDLE_BEAD_ERR_LIQUID_ADJUST_MAX_NOT_REACH  = 0X18,//磁珠针液面探测校准达不到最大值
    FLAG_NEEDLE_BEAD_ERR_LIQUID_ADJUST_MIN_NOT_REACH  = 0X19,//磁珠针液面探测校准达不到最小值
    FLAG_NEEDLE_BEAD_ERR_PUMP_NOT_ENOUGH              = 0X1A,//磁珠针泵内液量不足
    FLAG_NEEDLE_BEAD_ERR_CRASH_AT_PLATE_REACTION      = 0X1B,//磁珠针反应盘防撞
    FLAG_NEEDLE_BEAD_ERR_CRASH_AT_CLEAN               = 0X1C,//磁珠针清洗位置防撞
    FLAG_NEEDLE_BEAD_ERR_CRASH_AT_PLATE_REAGENT       = 0X1D,//磁珠针试剂盘防撞
}FLAG_NEEDLE_BEAD_ERR_CONVERT;

//自动运行状态机初始化
LH_ERR ServiceImplNeedleBead_StateMachineInit(void);

//自动运行状态机轮转
LH_ERR ServiceImplNeedleBead_StateMachineLoop(void);

//磁珠针复位
LH_ERR ServiceImplNeedleBead_ResetAll(uint16_t* pressureSensorValue,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//磁珠针旋转到指定位置
LH_ERR ServiceImplNeedleBead_RotatePos(NEEDLE_BEAD_POS targetPos,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//磁珠针清洗
LH_ERR ServiceImplNeedleBead_Clean(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime,uint16_t* pressureValue,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//磁珠针灌注
LH_ERR ServiceImplNeedleBead_Prime(NEEDLE_PRIME_OPT primeOpt,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//磁珠针吸液
LH_ERR ServiceImplNeedleBead_Absorb(uint16_t absorbCountMinUL,uint8_t useLiquidRecord,uint16_t* liquidDetectPos,uint16_t* pressureValue,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//试剂余量校准
LH_ERR ServiceImplNeedleBead_LiquidResidueCheck(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//磁珠针注液
LH_ERR ServiceImplNeedleBead_Inject(uint32_t cupLiquidUl,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//磁珠针注液带混匀
LH_ERR ServiceImplNeedleBead_InjectMix(uint32_t cupLiquidUl,uint8_t mixCount,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//磁珠针清洗维护
LH_ERR ServiceImplNeedleBead_MainTain(NEEDLE_CLEAN_OPT cleanOpt,NEEDLE_CLEAN_TIME cleanTime,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//磁珠针探液校准
LH_ERR ServiceImplNeedleBead_Adjust(CMD_SOURCE_TEST_FLOW cmdSource,uint16_t* needleLiquidDetectVol,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//读取模块内部指定输入的状态
LH_ERR ServiceImplNeedleBead_InputRead(uint8_t inputIndex,SENSOR_STATE* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//写入模块指定输出的状态
LH_ERR ServiceImplNeedleBead_OutputWrite(uint8_t outputIndex,OUT_STATE setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特定电机复位
LH_ERR ServiceImplNeedleBead_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);

//指定模块特动电机运行指定步数
LH_ERR ServiceImplNeedleBead_MotorRunSteps(uint8_t motorIndex,int32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel);




#endif


