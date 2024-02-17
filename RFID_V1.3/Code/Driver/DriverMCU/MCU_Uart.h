/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-11-13 14:25:47
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-01-04 15:31:24
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __MCU_UART_H_
#define __MCU_UART_H_
#include "CoreUtil.h"

//串口接收中断处理函数
typedef void (*MCU_UartRecvIntProcFunc)(uint8_t dat);
//串口空闲中断处理函数
typedef void (*MCU_UartIdleIntProcFunc)(uint8_t *datBuff,uint16_t datLen);
/*********************************串口相关枚举定义**************************************/
//字长
typedef enum MCU_UART_LENGTH
{
    MCU_UART_LENGTH8 = USART_WordLength_8b,
    MCU_UART_LENGTH9 = USART_WordLength_9b,
} MCU_UART_LENGTH;

//停止位
typedef enum MCU_UART_STOPBIT
{
    MCU_UART_STOPBIT1 = USART_StopBits_1,
    MCU_UART_STOPBIT1_5 = USART_StopBits_1_5,
    MCU_UART_STOPBIT2 = USART_StopBits_2,
} MCU_UART_STOPBIT;

//校验位
typedef enum MCU_UART_CHECK_MODE
{
    MCU_UART_CHECK_MODE_NONE = USART_Parity_No,
    MCU_UART_CHECK_MODE_EVEN = USART_Parity_Even,
    MCU_UART_CHECK_MODE_ODD = USART_Parity_Odd,
} MCU_UART_CHECK_MODE;

//硬件流控制
typedef enum MCU_UART_HARD_CONTROL
{
    MCU_UART_HARD_CONTROL_NONE = USART_HardwareFlowControl_None,
    MCU_UART_HARD_CONTROL_RTS = USART_HardwareFlowControl_RTS,
    MCU_UART_HARD_CONTROL_CTS = USART_HardwareFlowControl_CTS,
    MCU_UART_HARD_CONTROL_RTS_CTS = USART_HardwareFlowControl_RTS_CTS,
} MCU_UART_HARD_CONTROL;

/**************************************串口1************************************************/
//发送数据的串口缓冲区的长度
#define LENGTH_UART1_BUFFER_WITH_SEND       128
//串口用于printf的缓冲区的长度
#define LENGTH_UART1_BUFFER_WITH_FORMAT     64
//接收数据的串口缓冲区的长度
#define LENGTH_UART1_BUFFER_WITH_RECV       1200
//串口初始化
void MCU_Uart1Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode,MCU_UART_HARD_CONTROL hardWareControl, MCU_UartIdleIntProcFunc idleCallBack);
//串口发送数组
void MCU_Uart1SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);
//串口发送字符串
void MCU_Uart1SendString(uint8_t* stringStartPtr);
//串口发送字符串,带格式化
int MCU_Uart1Printf(const char *format, ...);


//是否支持RTT调试接口
#define SYSYEM_USE_RTT      1

#if(SYSYEM_USE_RTT == 1)
#define SystemPrintf(format,args...)            SEGGER_RTT_printf(0,format,##args)	
#else
#define SystemPrintf(format,args...)            MCU_Uart1Printf(format,##args)	
#endif

//是否执行真实动作,还是只进行流程测试
#define APP_DEBUG_SHOW_ENABLE               1

#if(APP_DEBUG_SHOW_ENABLE == 1)
#define AppShowStartMsg(msgStr,paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)        SystemPrintf("%s%s - %d - %d - %d - %d - %d\r\n",(const char *)msgStr, (char *)paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)
#define AppShowEndMsg(endStr,userErrorCode)                                                       SystemPrintf("%s - 0X%08X\r\n",(const char *)endStr,userErrorCode)
#define AppShowErrMsg(errorCode)                                                                  SystemPrintf("!!Action Error Func : %s , Line : %d , ErrorCode : 0x%08X\r\n",__FUNCTION__,__LINE__,errorCode)
#else
#define AppShowStartMsg(msgStr,paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)        asm("nop")
#define AppShowEndMsg(endStr,userErrorCode)                                                       asm("nop")
#define AppShowErrMsg(errorCode)                                                                  asm("nop")
#endif


#endif




