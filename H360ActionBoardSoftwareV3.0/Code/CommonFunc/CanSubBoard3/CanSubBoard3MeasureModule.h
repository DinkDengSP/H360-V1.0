/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:14:00
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-29 19:22:40
 *FilePath: \H360_ActionBoard_ID_3d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard3\CanSubBoard3MeasureModule.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SUB_BOARD3_MEASURE_MODULE_H_
#define __CAN_SUB_BOARD3_MEASURE_MODULE_H_
#include "CanMasterMain.h"
#include "CanSubBase.h"

//测量模块电机
typedef enum INDEX_MOTOR_MEASURE_MODULE
{
    INDEX_MOTOR_MEASURE_MODULE_DOOR     = 0X00,//上门
    INDEX_MOTOR_MEASURE_MODULE_WINDOW   = 0X01,//侧门
    INDEX_MOTOR_MEASURE_MODULE_PUMP     = 0X02,//注射泵
}INDEX_MOTOR_MEASURE_MODULE;

//瓶号选择
typedef enum BOTTLE_SELECT_MEASURE_MODULE
{
    BOTTLE_SELECT_MEASURE_MODULE_A = 0X01,//A瓶
    BOTTLE_SELECT_MEASURE_MODULE_B = 0X02,//B瓶
}BOTTLE_SELECT_MEASURE_MODULE;

//测量模块复位
LH_ERR CanSubBoard3MeasureModule_Reset(void);

//测量上门全开
LH_ERR CanSubBoard3MeasureModule_DoorOpenFull(void);

//测量上门半开
LH_ERR CanSubBoard3MeasureModule_DoorOpenHalf(void);

//测量上门关闭
LH_ERR CanSubBoard3MeasureModule_DoorClose(void);

//测量侧门打开
LH_ERR CanSubBoard3MeasureModule_WindowOpen(void);

//测量侧门关闭
LH_ERR CanSubBoard3MeasureModule_WindowClose(void);

//激发液灌注
LH_ERR CanSubBoard3MeasureModule_PrimeOnce(BOTTLE_SELECT_MEASURE_MODULE bottle);

//激发液注液
LH_ERR CanSubBoard3MeasureModule_InjectOnce(BOTTLE_SELECT_MEASURE_MODULE bottle);

//测量模块清洗维护
LH_ERR CanSubBoard3MeasureModule_InjectMaintain(BOTTLE_SELECT_MEASURE_MODULE bottle);

//测量模块暗计数
LH_ERR CanSubBoard3MeasureModule_ReadDark(uint32_t* darkValue);

//测量模块读本底
LH_ERR CanSubBoard3MeasureModule_ReadBackGround(uint32_t* backGroundValue);

//测量模块注液测光
LH_ERR CanSubBoard3MeasureModule_ReadLightWithInject(BOTTLE_SELECT_MEASURE_MODULE bottle,uint32_t* lightValue);

//测量模块集成单次测量
LH_ERR CanSubBoard3MeasureModule_ReadLightComplex(BOTTLE_SELECT_MEASURE_MODULE bottle,FunctionalState enableInject,uint16_t* backGroundValue,uint32_t* lightValue);

//测量模块读光子变化率
LH_ERR CanSubBoard3MeasureModule_ReadLightChangeRate(BOTTLE_SELECT_MEASURE_MODULE bottle,uint8_t setChangeCount,uint8_t* resultPointCount,uint32_t** resultArrayPtr);

//指定步进电机复位
LH_ERR CanSubBoard3MeasureModule_StepMotorReset(INDEX_MOTOR_MEASURE_MODULE index);

//指定步进电机走指定步数
LH_ERR CanSubBoard3MeasureModule_StepMotorRunSteps(INDEX_MOTOR_MEASURE_MODULE index,int32_t steps,int32_t* posAfterRun);





#endif

