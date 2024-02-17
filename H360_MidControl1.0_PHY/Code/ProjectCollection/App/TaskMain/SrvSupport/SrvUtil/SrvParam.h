/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 11:06:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-17 15:07:36 +0800
************************************************************************************************/ 
#ifndef __SRV_PARAM_H_
#define __SRV_PARAM_H_
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


