/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:55:11
**LastEditors: DengXiaoJun
**LastEditTime: 2021-09-02 17:03:27
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_3\TaskMain\AppSupport\AppUtil\AppStateDefault.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppStateDefault.h"

//默认状态-入口节点
const APP_STATE_TUBE_ENTRY stateTubeEntryDefault = {
    {SENSOR_STATE_INVALID},
    EVENT_FLAG_NONE,//进管事件
};

//默认状态-转盘节点
const APP_STATE_TUBE_ROTATE stateTubeRotateDefault = {
    {SENSOR_STATE_VALID,SENSOR_STATE_VALID,SENSOR_STATE_VALID,SENSOR_STATE_VALID,},
    0,
};

//默认状态-扫码节点
const APP_STATE_TUBE_BAR_SCAN stateTubeBarScanDefault = {
    0,
};

//默认状态-测试试管夹紧
const APP_STATE_TUBE_CLAMP stateTubeClampDefault = {
    TUBE_CLAMP_UNKNOW,//试管固定状态
};

//默认状态-输出节点
const APP_STATE_TUBE_EXIT stateTubeExitDefault = {
    {SENSOR_STATE_VALID},
    EVENT_FLAG_NONE,//出管事件
};


















