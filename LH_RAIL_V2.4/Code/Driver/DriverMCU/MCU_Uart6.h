/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-07-31 15:02:48
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-11-02 17:29:20
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_UART6_H_
#define __MCU_UART6_H_
#include "MCU_Common.h"

//发送数据的串口缓冲区的长度
#define LENGTH_UART6_BUFFER_WITH_SEND       1024
//串口用于printf的缓冲区的长度
#define LENGTH_UART6_BUFFER_WITH_FORMAT     512

//串口初始化
void MCU_Uart6Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, MCU_UartRecvIntProcFunc rxCallBack);

//串口发送数组
void MCU_Uart6SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);

//串口发送字符串
void MCU_Uart6SendString(uint8_t* stringStartPtr);

//串口发送字符串,带格式化
int MCU_Uart6Printf(const char *format, ...);

//串口回调函数,接收完成中断
void MCU_UART6_RxCpltCallback(UART_HandleTypeDef *UartHandle);

//串口回调函数,发送完成中断
void MCU_UART6_TxCpltCallback(UART_HandleTypeDef *UartHandle);

#endif
