/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 11:06:35 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-19 20:57:06 +0800
************************************************************************************************/ 
#ifndef __SRV_PARAM_DATA_TYPE_H_
#define __SRV_PARAM_DATA_TYPE_H_
#include "SrvParamMacro.h"

//网卡地址
typedef struct SRV_PARAM_LAN8700_NET_BASE
{
    int32_t ipValue;
    int32_t reserve[SRV_PARAM_RESERVE_REGION_LENGTH];
}SRV_PARAM_LAN8700_NET_BASE;
#define LENGTH_SRV_PARAM_LAN8700_NET_BASE        (sizeof(SRV_PARAM_LAN8700_NET_BASE)/sizeof(uint8_t))

//服务状态
typedef struct SRV_PARAM
{
    uint32_t initFlag;
    SRV_PARAM_LAN8700_NET_BASE lan8700NetBase;
}SRV_PARAM;
#define LENGTH_SRV_PARAM                (sizeof(SRV_PARAM)/sizeof(uint8_t))


#endif


