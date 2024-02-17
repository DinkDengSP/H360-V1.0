/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 14:38:46
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-18 16:58:17
**FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_3\TaskMain\AppSupport\AppTask\AppImpl\AppImplParam.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplParam.h"

//读参数
LH_ERR AppImplReadParam(INDEX_CAN_SUB_BOARD3_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* paramReadPtr)
{
    return AppParamRead(mainIndex,subIndex,paramReadPtr);
}

//写参数
LH_ERR AppImplWriteParam(INDEX_CAN_SUB_BOARD3_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t paramWrite)
{
    return AppParamWrite(mainIndex,subIndex,paramWrite);
}

//恢复出厂设置参数
LH_ERR AppImplParamSetDefault(void)
{
    return AppParamSetDefault();
}










