/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 15:33:32
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-29 11:27:58
 *FilePath: \H360_ActionBoard_ID_5\TaskMain\AppSupport\AppTask\AppImpl\AppImplSampleRackTrack.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_SAMPLE_RACK_TRACK_VERSION1_H_
#define __APP_IMPL_SAMPLE_RACK_TRACK_VERSION1_H_
#include "AppImplSampleRackTrackConfigVersion1.h"

//加样传送带复位
LH_ERR AppImplSampleRackTrackResetVersion1(void);

//加样传送带移动到指定试管位
LH_ERR AppImplSampleRackTrackMove2TubeVersion1(SAMPLE_RACK_TRACK_POS rackPos,RACK_TUBE_INDEX tubeIndex,RACK_TUBE_INDEX* currentTube);

//加样传送带回退一整个试管架
LH_ERR AppImplSampleRackTrackBackFullSampleRackVersion1(RACK_TUBE_INDEX* currentTube);

//指定步进电机复位
LH_ERR AppImplSampleRackTrackStepMotorResetVersion1(INDEX_MOTOR_SAMPLE_RACK_TRACK index);

//指定步进电机走指定步数
LH_ERR AppImplSampleRackTrackStepMotorRunStepsVersion1(INDEX_MOTOR_SAMPLE_RACK_TRACK index,int32_t steps,int32_t* posAfterRun);

//加样传送带进行一次码盘齿差修正
LH_ERR AppImplSampleRackTrackEncoderCorrectVersion1(uint8_t encoderCorrectCount);


#endif

