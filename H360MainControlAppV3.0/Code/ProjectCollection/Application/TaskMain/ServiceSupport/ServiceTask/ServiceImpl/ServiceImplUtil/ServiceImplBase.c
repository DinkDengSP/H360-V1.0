/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 13:51:27
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-23 17:32:04
**FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplUtil\ServiceImplBase.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplBase.h"

//辅助调试信息
const uint8_t* DebugStringStart_Assist = "$$Action 1 Start : ";
const uint8_t* DebugStringEnd_Assist = "$$Action 1 End ";

//缓冲液注液调试信息
const uint8_t* DebugStringStart_BufferSolutionInject = "$$Action 2 Start : ";
const uint8_t* DebugStringEnd_BufferSolutionInject = "$$Action 2 End ";

//新杯托盘栈调试信息
const uint8_t* DebugStringStart_CupTrayStackManage = "$$Action 3 Start : ";
const uint8_t* DebugStringEnd_CupTrayStackManage = "$$Action 3 End ";

//新杯机械手调试信息
const uint8_t* DebugStringStart_HandNewCup = "$$Action 4 Start : ";
const uint8_t* DebugStringEnd_HandNewCup = "$$Action 4 End ";

//清洗机械手调试信息
const uint8_t* DebugStringStart_HandWash = "$$Action 5 Start : ";
const uint8_t* DebugStringEnd_HandWash = "$$Action 5 End ";

//液路自动化调试信息
const uint8_t* DebugStringStart_LiquidAuto = "$$Action 6 Start : ";
const uint8_t* DebugStringEnd_LiquidAuto = "$$Action 6 End ";

//测量模块调试信息
const uint8_t* DebugStringStart_MeasureModule = "$$Action 7 Start : ";
const uint8_t* DebugStringEnd_MeasureModule = "$$Action 7 End ";

//磁珠混匀调试信息
const uint8_t* DebugStringStart_MixBead = "$$Action 8 Start : ";
const uint8_t* DebugStringEnd_MixBead = "$$Action 8 End ";

//试剂混匀调试信息
const uint8_t* DebugStringStart_MixReagent = "$$Action 9 Start : ";
const uint8_t* DebugStringEnd_MixReagent = "$$Action 9 End ";

//清洗混匀调试信息
const uint8_t* DebugStringStart_MixWash = "$$Action 10 Start : ";
const uint8_t* DebugStringEnd_MixWash = "$$Action 10 End ";

//磁珠针调试信息
const uint8_t* DebugStringStart_NeedleBead = "$$Action 11 Start : ";
const uint8_t* DebugStringEnd_NeedleBead = "$$Action 11 End ";

//试剂针调试信息
const uint8_t* DebugStringStart_NeedleReagent = "$$Action 12 Start : ";
const uint8_t* DebugStringEnd_NeedleReagent = "$$Action 12 End ";

//样本针调试信息
const uint8_t* DebugStringStart_NeedleSample = "$$Action 13 Start : ";
const uint8_t* DebugStringEnd_NeedleSample = "$$Action 13 End ";

//清洗针调试信息
const uint8_t* DebugStringStart_NeedleWash = "$$Action 14 Start : ";
const uint8_t* DebugStringEnd_NeedleWash = "$$Action 14 End ";

//反应盘调试信息
const uint8_t* DebugStringStart_PlateReaction = "$$Action 15 Start : ";
const uint8_t* DebugStringEnd_PlateReaction = "$$Action 15 End ";

//试剂盘调试信息
const uint8_t* DebugStringStart_PlateReagent = "$$Action 16 Start : ";
const uint8_t* DebugStringEnd_PlateReagent = "$$Action 16 End ";

//清洗盘调试信息
const uint8_t* DebugStringStart_PlateWash = "$$Action 17 Start : ";
const uint8_t* DebugStringEnd_PlateWash = "$$Action 17 End ";

//试管架传送带调试信息
const uint8_t* DebugStringStart_SampleRackTrack = "$$Action 18 Start : ";
const uint8_t* DebugStringEnd_SampleRackTrack = "$$Action 18 End ";

//温控调试信息
const uint8_t* DebugStringStart_TemperatureControl = "$$Action 19 Start : ";
const uint8_t* DebugStringEnd_TemperatureControl = "$$Action 19 End ";

//文件系统调试信息
const uint8_t* DebugStringStart_FatFsLog = "$$Action 20 Start : ";
const uint8_t* DebugStringEnd_FatFsLog = "$$Action 20 End ";

//同步任务信息
const uint8_t* DebugStringStart_TaskSyn = "$$Action 21 Start : ";
const uint8_t* DebugStringEnd_TaskSyn = "$$Action 21 End ";

















