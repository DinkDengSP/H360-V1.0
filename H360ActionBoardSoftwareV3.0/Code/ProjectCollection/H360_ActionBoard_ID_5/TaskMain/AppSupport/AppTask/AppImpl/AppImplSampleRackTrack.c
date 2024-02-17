/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 15:32:38
**LastEditors: DengXiaoJun
**LastEditTime: 2020-10-13 16:39:21
**FilePath: \H360_ActionBoard_ID_5\TaskMain\AppSupport\AppTask\AppImpl\AppImplSampleRackTrack.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplSampleRackTrack.h"


//加样传送带复位
LH_ERR AppImplSampleRackTrackReset(void)
{
    int32_t paramUtil = 0;
    AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_SAMPLE_RACK_TRACK,APP_PARAM_SUB_INDEX_SAMPLE_RACK_TRACK_VERSION,&paramUtil);
    if(paramUtil == 0)
    {
        return AppImplSampleRackTrackResetVersion0();
    }
    else
    {
        return AppImplSampleRackTrackResetVersion1();
    }
}

//加样传送带移动到指定试管位
LH_ERR AppImplSampleRackTrackMove2Tube(SAMPLE_RACK_TRACK_POS rackPos,RACK_TUBE_INDEX tubeIndex,RACK_TUBE_INDEX* currentTube)
{
    int32_t paramUtil = 0;
    AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_SAMPLE_RACK_TRACK,APP_PARAM_SUB_INDEX_SAMPLE_RACK_TRACK_VERSION,&paramUtil);
    if(paramUtil == 0)
    {
        return AppImplSampleRackTrackMove2TubeVersion0(rackPos,tubeIndex,currentTube);
    }
    else
    {
        return AppImplSampleRackTrackMove2TubeVersion1(rackPos,tubeIndex,currentTube);
    }
}

//加样传送带回退一整个试管架
LH_ERR AppImplSampleRackTrackBackFullSampleRack(RACK_TUBE_INDEX* currentTube)
{
    //反向运转20个齿序号
    int32_t paramUtil = 0;
    AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_SAMPLE_RACK_TRACK,APP_PARAM_SUB_INDEX_SAMPLE_RACK_TRACK_VERSION,&paramUtil);
    if(paramUtil == 0)
    {
        return AppImplSampleRackTrackBackFullSampleRackVersion0(currentTube);
    }
    else
    {
        return AppImplSampleRackTrackBackFullSampleRackVersion1(currentTube);
    }
}

//加样传送带进行一次码盘齿差修正
LH_ERR AppImplSampleRackTrackEncoderCorrect(uint8_t encoderCorrectCount)
{
    int32_t paramUtil = 0;
    AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_SAMPLE_RACK_TRACK,APP_PARAM_SUB_INDEX_SAMPLE_RACK_TRACK_VERSION,&paramUtil);
    if(paramUtil == 0)
    {
        return AppImplSampleRackTrackEncoderCorrectVersion0(encoderCorrectCount);
    }
    else
    {
        return AppImplSampleRackTrackEncoderCorrectVersion1(encoderCorrectCount);
    }
}


//指定步进电机复位
LH_ERR AppImplSampleRackTrackStepMotorReset(INDEX_MOTOR_SAMPLE_RACK_TRACK index)
{
    int32_t paramUtil = 0;
    AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_SAMPLE_RACK_TRACK,APP_PARAM_SUB_INDEX_SAMPLE_RACK_TRACK_VERSION,&paramUtil);
    if(paramUtil == 0)
    {
        return AppImplSampleRackTrackStepMotorResetVersion0(index);
    }
    else
    {
        return AppImplSampleRackTrackStepMotorResetVersion1(index);
    }
}

//指定步进电机走指定步数
LH_ERR AppImplSampleRackTrackStepMotorRunSteps(INDEX_MOTOR_SAMPLE_RACK_TRACK index,int32_t steps,int32_t* posAfterRun)
{
    int32_t paramUtil = 0;
    AppParamRead(INDEX_CAN_SUB_BOARD5_PARAM_SAMPLE_RACK_TRACK,APP_PARAM_SUB_INDEX_SAMPLE_RACK_TRACK_VERSION,&paramUtil);
    if(paramUtil == 0)
    {
        return AppImplSampleRackTrackStepMotorRunStepsVersion0(index,steps,posAfterRun);
    }
    else
    {
        return AppImplSampleRackTrackStepMotorRunStepsVersion1(index,steps,posAfterRun);
    }
}
