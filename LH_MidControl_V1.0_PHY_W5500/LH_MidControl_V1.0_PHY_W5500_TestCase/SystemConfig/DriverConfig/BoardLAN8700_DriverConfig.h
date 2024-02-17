/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-11 15:24:01 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 20:37:47 +0800
************************************************************************************************/ 
#ifndef __BOARD_LAN8700_DRIVER_CONFIG_H_
#define __BOARD_LAN8700_DRIVER_CONFIG_H_
#include "stm32h7xx.h"

/*-------------------------------------------LAN8700 配置--------------------------------------------------------------*/
//PHY地址
#define LAN8700_PHY_ADDR                        0

//芯片超时时间配置
//软件复位超时时间
#define LAN8700_SW_RESET_TIME_OUT_MS            ((uint32_t)500U)
//初始化超时时间
#define LAN8700_INIT_TIME_OUT_MS                ((uint32_t)2000U)
//PHY设备ID
#define LAN8700_PHY_ID_A                        (0X0007C0C4)
//PHY设备ID
#define LAN8700_PHY_ID_B                        (0X0007C0C3)
//自协商超时时间
#define LAN8700_AUTO_NEGO_TIME_OUT_MS           ((uint32_t)2000U)


#endif



