/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 14:12:35
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-11 18:42:20
**FilePath: \H360_ActionBoard_ID_2\TaskMain\AppSupport\AppTask\AppImpl\AppImplParam.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplParam.h"



//读取指定序号参数
LH_ERR AppImplParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* paramReadPtr)
{
    return AppParamRead(mainIndex,subIndex,paramReadPtr);
}


//写入指定序号参数
LH_ERR AppImplParamWrite(INDEX_CAN_SUB_BOARD2_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t paramWrite)
{
    return AppParamWrite(mainIndex,subIndex,paramWrite);
}


//初始化板上参数为默认值
LH_ERR AppImplParamSetDefault(void)
{
    return AppParamSetDefault();
}













