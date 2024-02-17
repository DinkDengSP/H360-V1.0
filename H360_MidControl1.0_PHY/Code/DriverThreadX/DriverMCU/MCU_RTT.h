/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:14:36 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-15 13:56:57 +0800
************************************************************************************************/ 
#ifndef __MCU_RTT_H_
#define __MCU_RTT_H_
#include "CoreUtil.h"

//打印缓存数据长度
#define SEGGER_RTT_PRINTF_LENGTH        BUFFER_SIZE_UP
//RTT使用的主通道
#define SEGGER_RTT_MAIN_CHANNEL         0
//数据传输是锁定还是非锁定
#define SEGGER_RTT_UP_MODE              SEGGER_RTT_MODE_NO_BLOCK_SKIP
#define SEGGER_RTT_DOWN_MODE            SEGGER_RTT_MODE_NO_BLOCK_SKIP


//RTT初始化
void MCU_RTT_Init(void);

//获取锁
void MCU_RTT_GetLock(void);

//释放锁
void MCU_RTT_ReleaseLock(void);

//RTT发送数组
void MCU_RTT_SendBuffer(uint8_t* bufferPtr,uint16_t bufferLength);

//RTT发送字符串,不能在中断中使用
int32_t MCU_RTT_SendString(uint8_t* stringPtr);

//RTT发送字符串,可以在中断中使用
int32_t MCU_RTT_SendStringInt(uint8_t* stringPtr);

//RTT打印
int MCU_RTT_Printf(const char *format, ...);


#endif

