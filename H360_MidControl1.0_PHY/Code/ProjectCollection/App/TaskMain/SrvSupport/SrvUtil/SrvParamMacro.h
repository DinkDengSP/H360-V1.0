/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 11:06:42 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-19 20:57:21 +0800
************************************************************************************************/ 
#ifndef __SRV_PARAM_MACRO_H_
#define __SRV_PARAM_MACRO_H_
#include "DriverSystemHeader.h"

//系统参数标记
#define SRV_PARAM_INIT_FLAG_SPEC                0XA55AAA54
//保留区域长度
#define SRV_PARAM_RESERVE_REGION_LENGTH         100
//默认IP
#define SRV_PARAM_NET_LAN8700_IP_DEFAULT        0X7A1FA8C0
//默认IP
#define SRV_PARAM_NET_W5500_IP_DEFAULT          0X7B1FA8C0

//服务层参数主序号
typedef enum INDEX_MAIN_SRV_PARAM
{
    INDEX_MAIN_NET_BASE_LAN8700         = 0X00,
}INDEX_MAIN_SRV_PARAM;

//网络基础讯号
typedef enum INDEX_SUB_SRV_LAN8700_NET_BASE
{
    INDEX_SUB_SRV_LAN8700_NET_BASE_IP   = 0X00,
}INDEX_SUB_SRV_LAN8700_NET_BASE;



#endif

