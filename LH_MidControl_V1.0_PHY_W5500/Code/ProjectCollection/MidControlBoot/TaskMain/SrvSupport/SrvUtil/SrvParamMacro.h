/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-08-08 14:25:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-08-09 11:47:17 +0800
************************************************************************************************/ 
#ifndef __SRV_PARAM_MACRO_H_
#define __SRV_PARAM_MACRO_H_
#include "DriverSystemInc.h"

//系统参数标记
#define SRV_PARAM_INIT_FLAG_SPEC            0XA55AAA55
//保留区域长度
#define SRV_PARAM_RESERVE_REGION_LENGTH     100
//默认IP
#define SRV_PARAM_NET_IP_DEFAULT            0X0A00A8C0

//服务层参数主序号
typedef enum INDEX_MAIN_SRV_PARAM
{
    INDEX_MAIN_NET_BASE         = 0X00,
}INDEX_MAIN_SRV_PARAM;

//网络基础讯号
typedef enum INDEX_SUB_SRV_NET_BASE
{
    INDEX_SUB_SRV_NET_BASE_IP   = 0X00,
}INDEX_SUB_SRV_NET_BASE;

#endif

