/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-08-08 14:24:40 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-08-09 11:48:30 +0800
************************************************************************************************/ 
#ifndef __SRV_PARAM_H_
#define __SRV_PARAM_H_
#include "SrvParamDataType.h"
#include "SrvParamDefault.h"

//服务层参数初始化
void SrvParamInit(void);

//写入参数
LH_ERR SrvParamWriteSingle(INDEX_MAIN_SRV_PARAM mainIndex,uint8_t subIndex,int32_t paramWrite);

//读取参数
LH_ERR SrvParamReadSingle(INDEX_MAIN_SRV_PARAM mainIndex,uint8_t subIndex,int32_t* paramRead);

//设置系统IP为默认IP
void SrvParamSetNetIpDefault(void);

#endif



