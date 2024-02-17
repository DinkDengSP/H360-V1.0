/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:34:20
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-12 11:47:16
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplParam.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_PARAM_H_
#define __APP_IMPL_PARAM_H_
#include "AppImplBase.h"


//读取指定序号参数
LH_ERR AppImplParamRead(INDEX_CAN_SUB_BOARD1_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* paramReadPtr);

//写入指定序号参数
LH_ERR AppImplParamWrite(INDEX_CAN_SUB_BOARD1_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t paramWrite);

//初始化板上参数为默认值
LH_ERR AppImplParamSetDefault(void);




#endif




