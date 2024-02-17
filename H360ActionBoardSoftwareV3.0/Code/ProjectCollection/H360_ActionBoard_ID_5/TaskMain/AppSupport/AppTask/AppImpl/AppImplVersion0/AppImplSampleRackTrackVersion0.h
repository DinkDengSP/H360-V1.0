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
#ifndef __APP_IMPL_SAMPLE_RACK_TRACK_VERSION0_H_
#define __APP_IMPL_SAMPLE_RACK_TRACK_VERSION0_H_
#include "AppImplSampleRackTrackConfigVersion0.h"

//加样传送带复位
LH_ERR AppImplSampleRackTrackResetVersion0(void);

//加样传送带移动到指定试管位
LH_ERR AppImplSampleRackTrackMove2TubeVersion0(SAMPLE_RACK_TRACK_POS rackPos,RACK_TUBE_INDEX tubeIndex,RACK_TUBE_INDEX* currentTube);

//加样传送带回退一整个试管架
LH_ERR AppImplSampleRackTrackBackFullSampleRackVersion0(RACK_TUBE_INDEX* currentTube);

//指定步进电机复位
LH_ERR AppImplSampleRackTrackStepMotorResetVersion0(INDEX_MOTOR_SAMPLE_RACK_TRACK index);

//指定步进电机走指定步数
LH_ERR AppImplSampleRackTrackStepMotorRunStepsVersion0(INDEX_MOTOR_SAMPLE_RACK_TRACK index,int32_t steps,int32_t* posAfterRun);

//加样传送带进行一次码盘齿差修正
LH_ERR AppImplSampleRackTrackEncoderCorrectVersion0(uint8_t encoderCorrectCount);


#endif

