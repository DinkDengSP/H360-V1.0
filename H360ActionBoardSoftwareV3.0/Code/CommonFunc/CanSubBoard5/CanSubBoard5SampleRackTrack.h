/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 16:17:05
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-29 11:57:18
 *FilePath: \H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard5\CanSubBoard5SampleRackTrack.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SUB_BOARD5_SAMPLE_RACK_TRACK_H_
#define __CAN_SUB_BOARD5_SAMPLE_RACK_TRACK_H_
#include "CanMasterMain.h"
#include "CanSubBase.h"

//加样传送带
typedef enum INDEX_MOTOR_SAMPLE_RACK_TRACK
{
    INDEX_MOTOR_SAMPLE_RACK_TRACK_MOVE  = 0X00,//加样传送带移动 
}INDEX_MOTOR_SAMPLE_RACK_TRACK;

//试管架的运动目标
typedef enum SAMPLE_RACK_TRACK_POS
{
    SAMPLE_RACK_TRACK_POS_CURRENT    = 0X00,//当前试管架
    SAMPLE_RACK_TRACK_POS_NEXT       = 0X01,//下一个试管架
}SAMPLE_RACK_TRACK_POS;

//当前第几个试管对着样本针
typedef enum RACK_TUBE_INDEX
{
    RACK_TUBE_INDEX_1 = 0x00,
    RACK_TUBE_INDEX_2 = 0x01,
    RACK_TUBE_INDEX_3 = 0x02,
    RACK_TUBE_INDEX_4 = 0x03,
    RACK_TUBE_INDEX_5 = 0x04,
}RACK_TUBE_INDEX;

//加样传送带复位
LH_ERR CanSubBoard5SampleRackTrack_Reset(void);

//加样传送带移动到指定试管位
LH_ERR CanSubBoard5SampleRackTrack_Move2Tube(SAMPLE_RACK_TRACK_POS rackPos,RACK_TUBE_INDEX tubeIndex,RACK_TUBE_INDEX* currentTube);

//加样传送带回退一整个试管架
LH_ERR CanSubBoard5SampleRackTrack_BackFullSampleRack(RACK_TUBE_INDEX* currentTube);

//指定步进电机复位
LH_ERR CanSubBoard5SampleRackTrack_StepMotorReset(INDEX_MOTOR_SAMPLE_RACK_TRACK index);

//指定步进电机走指定步数
LH_ERR CanSubBoard5SampleRackTrack_StepMotorRunSteps(INDEX_MOTOR_SAMPLE_RACK_TRACK index,int32_t steps,int32_t* posAfterRun);

//加样传送带齿差修正
LH_ERR CanSubBoard5SampleRackTrack_EncoderCorrect(uint8_t correctCount);


#endif

