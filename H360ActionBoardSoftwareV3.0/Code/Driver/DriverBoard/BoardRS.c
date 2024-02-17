/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-07-13 14:56:59
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-11 19:48:09
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardRS.h"
#include "SEGGER_RTT.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

//串口初始化
void BoardRS_Init(BOARD_RS_PORT rsPort,uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, 
                                MCU_UART_HARD_CONTROL hardWareControl, MCU_UartRecvIntProcFunc rxCallBack)
{
    if(rsPort == BOARD_RS_PORT_1)
    {
        MCU_Uart1Init(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
    }
    else
    {
        MCU_Uart3Init(baud,length,stopBit,checkMode,hardWareControl,rxCallBack);
    }
    
}

//串口发送数组
void BoardRS_SendBuffer(BOARD_RS_PORT rsPort,uint8_t* bufferStartPtr,uint16_t sendLength)
{
    if(rsPort == BOARD_RS_PORT_1)
    {
        MCU_Uart1SendBuffer(bufferStartPtr,sendLength);
    }
    else
    {
        MCU_Uart3SendBuffer(bufferStartPtr,sendLength);
    }
}

//串口发送字符串
void BoardRS_SendString(BOARD_RS_PORT rsPort,uint8_t* stringStartPtr)
{
    if(rsPort == BOARD_RS_PORT_1)
    {
        MCU_Uart1SendString(stringStartPtr);
    }
    else
    {
        MCU_Uart3SendString(stringStartPtr);
    }
}

#pragma pack(4)
//用于printf的缓冲区
static uint8_t blockBufferWithSeggerPrintf[BUFFER_SIZE_UP] = {0};
#pragma pack ()
//互斥量
static OS_MUTEX mutexSegger;

//RTT初始化
void BoardRTT_Init(void)
{
    //配置调试库RTT
    OS_ERR err;
    //配置RTT调试端口
    SEGGER_RTT_Init();
    /* 配置通道0，上行配置*/
    SEGGER_RTT_ConfigUpBuffer(0, "RTTUP", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
    /* 配置通道0，下行配置*/	
    SEGGER_RTT_ConfigDownBuffer(0, "RTTDOWN", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
    //创建互斥信号量
    OSMutexCreate((OS_MUTEX*)&mutexSegger,(CPU_CHAR*)"mutexSegger",(OS_ERR*)&err);
}

//串口发送字符串,带格式化
int BoardRTT_Printf(const char *format, ...)
{
    OS_ERR err;
    //转换
    va_list arg;
    int rv;
    //申请互斥量
    if(OSRunning)
    {
        OSMutexPend(&mutexSegger,0,OS_OPT_PEND_BLOCKING,0,&err);
    }
    //锁定调度器
    OSSchedLock(&err);
    //启动可变参数解析
    va_start(arg, format);
    //将数据转换成带打印缓存
    rv = vsnprintf((char *)blockBufferWithSeggerPrintf, BUFFER_SIZE_UP, format, arg);
    va_end(arg);
    //解锁调度器
    OSSchedUnlock(&err);
    //将数据发送出去
    if ((rv > 0) && (rv <= BUFFER_SIZE_UP))
    {
        //RV大于0代表转换成功,且RV在指定范围内部LENGTH_UART6_BUFFER_WITH_FORMAT
        SEGGER_RTT_Write(0,blockBufferWithSeggerPrintf, (uint16_t)rv);
    }
    //释放互斥信号量
    if(OSRunning)
    {
        OSMutexPost(&mutexSegger,OS_OPT_POST_NO_SCHED,&err);
    }
    return rv;
}














