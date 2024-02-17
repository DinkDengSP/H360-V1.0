/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 15:39:14
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-29 11:51:47
 *FilePath: \H360_ActionBoard_ID_5\TaskMain\AppSupport\AppIpc\AppIpcSampleRackTrack.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_SAMPLE_RACK_TRACK_H_
#define __APP_IPC_SAMPLE_RACK_TRACK_H_
#include "AppIpcBase.h"

//初始化IPC调用结构
void IPC_SampleRackTrackDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_SampleRackTrackGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_SampleRackTrackWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//加样传送带复位,带零位偏移,不等待完成
LH_ERR IPC_SampleRackTrackResetWhileAck(void);
//加样传送带移动到指定试管位,不等待完成
LH_ERR IPC_SampleRackTrackMove2TubeWhileAck(SAMPLE_RACK_TRACK_POS rackPos,RACK_TUBE_INDEX tubeIndex);
//加样传送带回退一整个试管架,不等待完成
LH_ERR IPC_SampleRackTrackBackFullSampleRackWhileAck(void);
//加样传送带码盘齿差修正,不等待完成
LH_ERR IPC_SampleRackTrackEncoderCorrectWhileAck(uint8_t correctCount);

/****************************************************发出指令并等待完成********************************************************/
//加样传送带复位,带零位偏移,等待完成
LH_ERR IPC_SampleRackTrackResetWhileReturn(void);
//加样传送带移动到指定试管位,等待完成
LH_ERR IPC_SampleRackTrackMove2TubeWhileReturn(SAMPLE_RACK_TRACK_POS rackPos,RACK_TUBE_INDEX tubeIndex);
//加样传送带回退一整个试管架,等待完成
LH_ERR IPC_SampleRackTrackBackFullSampleRackWhileReturn(void);
//加样传送带码盘齿差修正,等待完成
LH_ERR IPC_SampleRackTrackEncoderCorrectWhileReturn(uint8_t correctCount);




#endif

