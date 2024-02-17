/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-11 14:33:08 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-11 14:39:01 +0800
************************************************************************************************/ 
#ifndef __MCU_RTT_DRIVER_CONFIG__
#define __MCU_RTT_DRIVER_CONFIG__
#include "stm32h7xx.h"

/*-----------------------------------------------------------RTT打印配置-------------------------------------------------------*/
//打印缓存数据长度
#define SEGGER_RTT_PRINTF_LENGTH                        BUFFER_SIZE_UP
//RTT使用的主通道
#define SEGGER_RTT_MAIN_CHANNEL                         0
//数据传输是锁定还是非锁定,上行模式,MCU发送到PC端
#define SEGGER_RTT_UP_MODE                              SEGGER_RTT_MODE_NO_BLOCK_SKIP
//数据传输是锁定还是非锁定,下行模式,PC发送到MCU端
#define SEGGER_RTT_DOWN_MODE                            SEGGER_RTT_MODE_NO_BLOCK_SKIP


#endif




