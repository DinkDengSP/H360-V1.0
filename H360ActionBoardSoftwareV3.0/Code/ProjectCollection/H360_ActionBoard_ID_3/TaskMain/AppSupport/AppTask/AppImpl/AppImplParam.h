/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 14:39:51
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-07 11:42:50
 *FilePath: \H360_ActionBoard_ID_3\TaskMain\AppSupport\AppTask\AppImpl\AppImplParam.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_PARAM_H_
#define __APP_IMPL_PARAM_H_
#include "AppImplBase.h"

//读参数
LH_ERR AppImplReadParam(INDEX_CAN_SUB_BOARD3_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* paramReadPtr);

//写参数
LH_ERR AppImplWriteParam(INDEX_CAN_SUB_BOARD3_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t paramWrite);

//恢复出厂设置参数
LH_ERR AppImplParamSetDefault(void);



#endif




