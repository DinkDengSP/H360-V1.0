/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-24 12:19:17
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-10-27 15:52:49
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_1\DriverConfig\CoreConfig\CoreNVIC_Prio.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CORE_NVIC_PRIO_H_
#define __CORE_NVIC_PRIO_H_
#include "stm32f4xx.h"

//因为系统的中断四位,所以,慎重起见,将中断感知级别设置为7 ,也就是抢占优先级1,子优先级3
//只有在这之上的中断,才能调用系统函数

//系统内部NVIC中断的优先级设定
/*****************************不会调用系统内核函数的中断优先级************************************/
//窗口看门狗 最高优先级
#define MCU_WDG_WINDOW_PREE_PRI             INT_PRE_PRI_0
#define MCU_WDG_WINDOW_SUB_PRI              INT_SUB_PRI_0

//中断抢占优先级,用户外部中断
#define USER_EXT_INT_PREE_PRI               INT_PRE_PRI_0
//中断子优先级,用户外部中断
#define USER_EXT_INT_SUB_PRI                INT_SUB_PRI_0

/************************************会调用系统内核函数的中断优先级********************************/
//中断优先级RX CAN
#define CAN1_RX_INT_PREE_PRI                INT_PRE_PRI_2
#define CAN1_RX_INT_SUB_PRI                 INT_SUB_PRI_0

//中断优先级 TX
#define CAN1_TX_INT_PREE_PRI                INT_PRE_PRI_2
#define CAN1_TX_INT_SUB_PRI                 INT_SUB_PRI_1

//中断抢占优先级,串口2
#define MCU_UART2_PREE_PRI                  INT_PRE_PRI_3
#define MCU_UART2_SUB_PRI                   INT_SUB_PRI_0

//中断抢占优先级,串口1
#define MCU_UART1_PREE_PRI                  INT_PRE_PRI_3
#define MCU_UART1_SUB_PRI                   INT_SUB_PRI_1

//中断抢占优先级,串口3
#define MCU_UART3_PREE_PRI                  INT_PRE_PRI_3
#define MCU_UART3_SUB_PRI                   INT_SUB_PRI_2

//中断抢占优先级,串口6
#define MCU_UART6_PREE_PRI                  INT_PRE_PRI_3
#define MCU_UART6_SUB_PRI                   INT_SUB_PRI_3




#endif






