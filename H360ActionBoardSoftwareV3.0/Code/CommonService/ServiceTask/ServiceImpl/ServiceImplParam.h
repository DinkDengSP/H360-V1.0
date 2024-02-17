/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-14 14:11:52
 *FilePath: \H360ActionBoardSoftwareV2.3\CommonService\ServiceImpl\ServiceImplParam.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_PARAM_H_
#define __SERVICE_IMPL_PARAM_H_
#include "CommonServiceImplBase.h"


//电机参数读取
LH_ERR ServiceImplParam_ReadMotorParam(uint8_t motorIndex,uint8_t mainIndex,uint16_t subIndex,int32_t* readResult);

//电机参数写入
LH_ERR ServiceImplParam_WriteMotorParam(uint8_t motorIndex,uint8_t mainIndex,uint16_t subIndex,int32_t writeParam);





#endif

