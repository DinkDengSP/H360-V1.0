/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-14 13:48:27 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-14 15:48:38 +0800
************************************************************************************************/ 
#ifndef __BOARD_CONFIG_H_
#define __BOARD_CONFIG_H_
#include "ParamAddrConfig.h"

/*------------------------------------MB85RS2MT 配置---------------------------------------*/
//铁电存储器的存储空间大小,也是最大地址
#define MB85RS2MT_SIZE                                  0x40000
//铁电存储器的芯片ID
#define MB85RS2MT_ID                                    0X00087F04
//读取ID的重试次数
#define MB85RS2MT_READ_ID_RETRY_MAX                     10  
//SPI通讯速率
#define MB85RS2MT_SPI_CLK                               SPI_123_BAUD_RATE_12_5M
//SPI CPOL
#define MB85RS2MT_SPI_CPOL                              MCU_SPI_CPOL_HIGH
//SPI CPHA
#define MB85RS2MT_SPI_CPHA                              MCU_SPI_CPHA_2EDGE

/*------------------------------------W5500 配置---------------------------------------*/
//SPI通讯速率
#define W5500_SPI_CLK                                   SPI_45_BAUD_RATE_25M
//SPI CPOL
#define W5500_SPI_CPOL                                  MCU_SPI_CPOL_HIGH
//SPI CPHA
#define W5500_SPI_CPHA                                  MCU_SPI_CPHA_2EDGE
//版本代码
#define W5500_VERSION_CODE                              0X04
//W5500数据发送超时时间
#define W5500_SEND_TIME_OUT_MAX                         200
//W5500使用的内存空间
#define W5500_USE_MEM_REGION                            MEM_AXI_SRAM
//W5500 MAC地址定义
#define W5500_MAC_ADDR_0                                0X90
#define W5500_MAC_ADDR_1                                0X05
#define W5500_MAC_ADDR_2                                0X25
#define W5500_MAC_ADDR_3                                0X20
#define W5500_MAC_ADDR_4                                0X07
#define W5500_MAC_ADDR_5                                0X11
//W5500 子网掩码配置
#define W5500_SUB_NET_MASK1                             255
#define W5500_SUB_NET_MASK2                             255
#define W5500_SUB_NET_MASK3                             255
#define W5500_SUB_NET_MASK4                             0
//IP地址
#define W5500_IP_ADDR0                                  192  
#define W5500_IP_ADDR1                                  168
#define W5500_IP_ADDR2                                  207
#define W5500_IP_ADDR3                                  11
//缓存长度定义
#define W5500_SOCKET_BUF_SIZE_0KB                       0
#define W5500_SOCKET_BUF_SIZE_1KB                       1
#define W5500_SOCKET_BUF_SIZE_2KB                       2
#define W5500_SOCKET_BUF_SIZE_4KB                       4
#define W5500_SOCKET_BUF_SIZE_8KB                       8
#define W5500_SOCKET_BUF_SIZE_16KB                      16
//应用中发送缓存定义
#define W5500_SOCKET_0_TX_BUF_SIZE                      W5500_SOCKET_BUF_SIZE_16KB
#define W5500_SOCKET_1_TX_BUF_SIZE                      W5500_SOCKET_BUF_SIZE_0KB
#define W5500_SOCKET_2_TX_BUF_SIZE                      W5500_SOCKET_BUF_SIZE_0KB
#define W5500_SOCKET_3_TX_BUF_SIZE                      W5500_SOCKET_BUF_SIZE_0KB
#define W5500_SOCKET_4_TX_BUF_SIZE                      W5500_SOCKET_BUF_SIZE_0KB
#define W5500_SOCKET_5_TX_BUF_SIZE                      W5500_SOCKET_BUF_SIZE_0KB
#define W5500_SOCKET_6_TX_BUF_SIZE                      W5500_SOCKET_BUF_SIZE_0KB
#define W5500_SOCKET_7_TX_BUF_SIZE                      W5500_SOCKET_BUF_SIZE_0KB
//应用中接收缓存定义
#define W5500_SOCKET_0_RX_BUF_SIZE                      W5500_SOCKET_BUF_SIZE_16KB
#define W5500_SOCKET_1_RX_BUF_SIZE                      W5500_SOCKET_BUF_SIZE_0KB
#define W5500_SOCKET_2_RX_BUF_SIZE                      W5500_SOCKET_BUF_SIZE_0KB
#define W5500_SOCKET_3_RX_BUF_SIZE                      W5500_SOCKET_BUF_SIZE_0KB
#define W5500_SOCKET_4_RX_BUF_SIZE                      W5500_SOCKET_BUF_SIZE_0KB
#define W5500_SOCKET_5_RX_BUF_SIZE                      W5500_SOCKET_BUF_SIZE_0KB
#define W5500_SOCKET_6_RX_BUF_SIZE                      W5500_SOCKET_BUF_SIZE_0KB
#define W5500_SOCKET_7_RX_BUF_SIZE                      W5500_SOCKET_BUF_SIZE_0KB

#endif




