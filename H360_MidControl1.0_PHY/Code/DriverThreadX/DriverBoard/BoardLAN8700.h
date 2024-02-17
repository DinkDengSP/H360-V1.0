/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 10:05:24 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 14:33:01 +0800
************************************************************************************************/ 
#ifndef __BOARD_LAN8700_H_
#define __BOARD_LAN8700_H_
#include "BoardLAN8700_Config.h"

//PHY地址
#define LAN8700_PHY_ADDR                    0

//芯片超时时间配置
//软件复位超时时间
#define LAN8700_SW_RESET_TIME_OUT_MS        ((uint32_t)500U)
//初始化超时时间
#define LAN8700_INIT_TIME_OUT_MS            ((uint32_t)2000U)
//PHY设备ID
#define LAN8700_PHY_ID_A                    (0X0007C0C4)
//PHY设备ID
#define LAN8700_PHY_ID_B                    (0X0007C0C3)
//自协商超时时间
#define LAN8700_AUTO_NEGO_TIME_OUT_MS       ((uint32_t)2000U)


//设备初始化
LH_ERR Board_LAN8700_Init(void);

//LAN8700硬件复位
LH_ERR Board_LAN8700_HardWareReset(void);

//读取网卡ID
LH_ERR Board_LAN8700_ReadID(uint32_t* idPtr);

//获取设备网线连接状态
LH_ERR Board_LAN8700_GetLinkState(NET_PORT_STATE* netPortState);

//从连接状态获取工作模式和速度
void Board_LAN8700_GetModeSpeed(uint32_t* speedPtr,uint32_t* deplexModePtr);



#endif



