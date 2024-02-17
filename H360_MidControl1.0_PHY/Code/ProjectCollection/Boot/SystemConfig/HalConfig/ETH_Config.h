/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-08-15 18:30:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 13:58:23 +0800
************************************************************************************************/ 
#ifndef __ETH_CONFIG_H_
#define __ETH_CONFIG_H_

//收发描述符长度
#define ETH_TX_DESC_CNT         4  /* number of Ethernet Tx DMA descriptors */
#define ETH_RX_DESC_CNT         4  /* number of Ethernet Rx DMA descriptors */

//MAC的MAC地址
#define ETH_MAC_ADDR0           ((uint8_t)0x20)
#define ETH_MAC_ADDR1           ((uint8_t)0x07)
#define ETH_MAC_ADDR2           ((uint8_t)0x11)
#define ETH_MAC_ADDR3           ((uint8_t)0x90)
#define ETH_MAC_ADDR4           ((uint8_t)0x05)
#define ETH_MAC_ADDR5           ((uint8_t)0x25)

#endif



