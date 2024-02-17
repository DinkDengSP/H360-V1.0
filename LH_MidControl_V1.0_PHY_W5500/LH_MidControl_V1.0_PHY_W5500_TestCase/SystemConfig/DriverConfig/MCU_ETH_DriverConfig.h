/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-11 14:40:15 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 20:39:55 +0800
************************************************************************************************/ 
#ifndef __MCU_ETH_DRIVER_CONFIG__
#define __MCU_ETH_DRIVER_CONFIG__
#include "stm32h7xx.h"

/*------------------------------------------------------------------芯片内置MAC配置-----------------------------------------*/
#define MCU_ETH_MODE_MII                                    0
#define MCU_ETH_MODE_RMII                                   1
//用户选择的以太网模式
#define MCU_ETH_MODE_USER                                   MCU_ETH_MODE_MII 

//双工模式不支持
#define PHY_DUPLEX_MODE_ETH_INVALID                         0XFFFFFFFF
//速度不支持        
#define PHY_SPEED_ETH_INVALID                               0XFFFFFFFF
//MCU动态内存匹配对象       
#define MCU_ETH_MEM_REGION                                  MEM_AXI_SRAM
//是否需要CACHE更新机制     
#define MCU_ETH_ENABLE_CACHE_INVALID_STRATEGY               0
//ETH单包数据发送超时时间       
#define TIME_OUT_MS_ETH_SEND                                100
//是否显示错误中断讯息      
#define MCU_ETH_INT_SHOW_ERR                                0

#endif



