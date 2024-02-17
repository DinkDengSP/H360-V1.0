/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-10-27 18:57:15 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-10-28 10:33:50 +0800
************************************************************************************************/ 
#ifndef __ETH_CONFIG_H_
#define __ETH_CONFIG_H_
#include "stdint.h"

//发送DMA描述符场电影
#define ETH_TX_DESC_CNT         4  
//接收DMA描述符长度
#define ETH_RX_DESC_CNT         4  

//以太网MAC地址设定
#define ETH_MAC_ADDR0           ((uint8_t)0X90)
#define ETH_MAC_ADDR1           ((uint8_t)0X05)
#define ETH_MAC_ADDR2           ((uint8_t)0X25)
#define ETH_MAC_ADDR3           ((uint8_t)0X20)
#define ETH_MAC_ADDR4           ((uint8_t)0X07)
#define ETH_MAC_ADDR5           ((uint8_t)0X11)

#endif


