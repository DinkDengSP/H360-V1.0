/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 11:07:12 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-17 14:58:46 +0800
************************************************************************************************/ 
#ifndef __SRV_STATE_MACRO_H_
#define __SRV_STATE_MACRO_H_
#include "DriverSystemHeader.h"

//服务层状态存储使用的内存空间
#define SRV_STATE_USE_MEM_REGION                            MEM_AXI_SRAM
//Socket初始化标记  
#define SRV_STATE_SOCKET_LAN8700_SOCKET_INIT_FLAG           0XAA55A55A
//Socket初始化标记  
#define SRV_STATE_SOCKET_W5500_SOCKET_INIT_FLAG             0X55AA5AA5

#endif

