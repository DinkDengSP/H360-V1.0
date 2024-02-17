
/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 08:52:52
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-18 16:27:47
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_INC_H_
#define __MCU_INC_H_
#include "MCU_Common.h"

#include "MCU_ID.h"
#include "MCU_MPU.h"
#include "MCU_Port.h"
#include "MCU_Random.h"
#include "MCU_SPI.h"
#include "MCU_CAN.h"
#include "MCU_ADC.h"
#include "MCU_Flash.h"
#include "MCU_Uart1Version0.h"
#include "MCU_Uart1Version1.h"
#include "MCU_Uart2.h"
#include "MCU_Uart3Version0.h"
#include "MCU_Uart3Version1.h"
#include "MCU_Uart6.h"
#include "MCU_RTC.h"
#include "SEGGER_RTT.h"

//变参宏定义,RS2,调试printf
#ifdef RUN_AT_BOOT
#define SystemPrintf(format,args...)            MCU_Uart3PrintfVersion0(format,##args)	
#endif

//动作调试口,RS1
#define ActionPrintf(format,args...)            SEGGER_RTT_printf(0,format,##args)
//#define ActionPrintf(format,args...)            MCU_Uart3PrintfVersion0(format,##args)

#endif





