/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:10:01 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 21:13:11 +0800
************************************************************************************************/ 
#ifndef __BOARD_LAN8700_H_
#define __BOARD_LAN8700_H_
#include "BoardLAN8700_Config.h"

//设备初始化
ERROR_SUB BoardLAN8700_Init(void);

//LAN8700硬件复位
ERROR_SUB BoardLAN8700_HardWareReset(void);

//读取网卡ID
ERROR_SUB BoardLAN8700_ReadID(uint32_t* idPtr);

//获取设备网线连接状态
ERROR_SUB BoardLAN8700_GetLinkState(NET_PORT_STATE* netPortState);

//从连接状态获取工作模式和速度
void BoardLAN8700_GetModeSpeed(uint32_t* speedPtr,uint32_t* deplexModePtr);



#endif


