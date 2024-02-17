/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:02:06 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 20:08:40 +0800
************************************************************************************************/ 
#ifndef __MCU_UART1_H_
#define __MCU_UART1_H_
#include "MCU_UART_Universal.h"

//获取互斥锁
void MCU_UART1_GetLock(void);

//释放互斥锁
void MCU_UART1_ReleaseLock(void);

//串口初始化
void MCU_UART1_Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl, 
                        MCU_UartRecvIntProcFunc rxCallBack);

//串口发送数组
void MCU_UART1_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);

//串口发送字符串,常规模式下发送
void MCU_UART1_SendString(uint8_t* stringStartPtr);

//串口发送字符串,中断中发送
void MCU_UART1_SendStringInt(uint8_t* stringStartPtr);

//串口打印,带格式化
void MCU_UART1_Printf(const char *format, ...);

//串口接收到的数据释放
void MCU_UART1_RecvDatRelease(UART_RECV_PACK* uartRecvPackPtr);

//串口回调函数,接收完成中断
void MCU_UART1_RxCpltCallBack(void);

//串口回调函数,发送完成中断
void MCU_UART1_TxCpltCallBack(void);

//串口回调函数,错误中断
void MCU_UART1_ErrorCallBack(void);

//串口回调函数,接收事件中断
void MCU_UART1_RxEventCallBack(uint16_t dataLength);


#endif




