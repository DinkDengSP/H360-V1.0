/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-09-04 09:05:22
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-10-27 15:41:07
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __EC20_PORT_H_
#define __EC20_PORT_H_
#include "EC20_Type.h"

//等待EC20的指令返回
#define EC20_WAIT_RESPONSE(timeMs)                      CoreDelayMs(timeMs)
//初始化EC20通讯端口
#define EC20_INIT_COMM_PORT(initBaud,callback)          MCU_Uart2Init(initBaud,MCU_UART_LENGTH8,MCU_UART_STOPBIT1,MCU_UART_CHECK_MODE_NONE,MCU_UART_HARD_CONTROL_NONE,callback);
//修改EC20通讯波特率
#define EC20_CHANGE_BAUD(baud)                          MCU_Uart2ChangeBaud(baud)

//初始化EC20复位端口
#define EC20_INIT_RST_PORT(initLevel)                   MCU_PortInit(MCU_PIN_E_3, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);MCU_PortWriteSingle(MCU_PIN_E_3, initLevel)
//设置EC20复位端口的电平
#define EC20_RST_PORT_SET(level)                        MCU_PortWriteSingle(MCU_PIN_E_3, level)
//端口有效信号
#define VALID_LEVEL_EC20_RST_PORT                       Bit_SET

//初始化EC20电源端口
#define EC20_INIT_POWER_PORT(initLevel)                 MCU_PortInit(MCU_PIN_E_2, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Fast_Speed);MCU_PortWriteSingle(MCU_PIN_E_2, initLevel)
//设置EC20电源端口的电平
#define EC20_POWER_PORT_SET(level)                      MCU_PortWriteSingle(MCU_PIN_E_2, level)
//端口有效信号
#define VALID_LEVEL_EC20_POWER_PORT                     Bit_RESET

//EC20数据发送
#define EC20_SEND_BUFFER(buffer,length)                 MCU_Uart2SendBuffer(buffer,length)  
//EC20发送字符串
#define EC20_SEND_STR(str)                              MCU_Uart2SendString(str)      
//EC20格式化数据发送
#define EC20_Printf(format,args...)                     MCU_Uart2Printf(format,##args)


#endif

