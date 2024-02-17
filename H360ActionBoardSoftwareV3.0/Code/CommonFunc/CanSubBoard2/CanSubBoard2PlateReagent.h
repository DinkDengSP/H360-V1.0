/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:13:49
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-01 23:58:17
 *FilePath: \Code\CommonFunc\CanSubBoard2\CanSubBoard2PlateReagent.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SUB_BOARD2_PLATE_REAGENT_H_
#define __CAN_SUB_BOARD2_PLATE_REAGENT_H_
#include "CanMasterMain.h"
#include "CanSubBase.h"

//试剂盘模块电机
typedef enum INDEX_MOTOR_PLATE_REAGENT
{
    INDEX_MOTOR_PLATE_REAGENT_ROTATE  = 0X00,     
}INDEX_MOTOR_PLATE_REAGENT;

//试剂盘的偏移信息
typedef enum PLATE_REAGENT_OFFSET
{
    PLATE_REAGENT_OFFSET_NONE               = 0X00,//试剂盘无偏移
    PLATE_REAGENT_OFFSET_NEEDLE_REAGENT_R1  = 0X01,//试剂盘旋转到对准试剂针R1位置
    PLATE_REAGENT_OFFSET_NEEDLE_REAGENT_R2  = 0X02,//试剂盘旋转到对准试剂针R2位置
    PLATE_REAGENT_OFFSET_NEEDLE_BEAD_R1     = 0X03,//试剂盘旋转到对阵磁珠针R1位置
    PLATE_REAGENT_OFFSET_NEEDLE_BEAD_R2     = 0X04,//试剂盘旋转到对阵磁珠针R2位置
    PLATE_REAGENT_OFFSET_NEEDLE_BEAD_BEAD   = 0X05,//试剂盘旋转到对阵磁珠针BEAD位置
    PLATE_REAGENT_OFFSET_QR_SCAN            = 0X06,//试剂盘旋转到扫码位置
    PLATE_REAGENT_OFFSET_QR_RFID            = 0X07,//试剂盘旋转到RFID扫码位置
}PLATE_REAGENT_OFFSET;

/* 扫码偏移 */
typedef enum PLATE_REAGENT_SCAN_OFFSET
{
    PLATE_REAGENT_SCAN_OFFSET_NONE          = 0,//无偏移
    PLATE_REAGENT_SCAN_OFFSET_START         = 1,//试剂盘移动扫码起始点
    PLATE_REAGENT_SCAN_OFFSET_END           = 2,//试剂盘移动扫码结束点
}PLATE_REAGENT_SCAN_OFFSET;

//试剂盘复位
LH_ERR CanSubBoard2PlateReagent_Reset(uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset);

//试剂盘运行到指定序号杯位,带偏移
LH_ERR CanSubBoard2PlateReagent_Run2CupWithOffset(uint8_t targetCupIndex,PLATE_REAGENT_OFFSET targetOffset,uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset);

//试剂盘运行指定数量个杯位,基于当前位置
LH_ERR CanSubBoard2PlateReagent_RunAnyCups(uint16_t cups,uint8_t* currentCupIndex,PLATE_REAGENT_OFFSET* currentOffset);

//指定步进电机复位
LH_ERR CanSubBoard2PlateReagent_StepMotorReset(INDEX_MOTOR_PLATE_REAGENT index);

//指定步进电机走步数
LH_ERR CanSubBoard2PlateReagent_StepMotorRunSteps(INDEX_MOTOR_PLATE_REAGENT index,int32_t steps,int32_t* posAfterRun);




#endif

