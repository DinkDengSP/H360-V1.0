/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-08-08 18:57:55 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-08-09 15:42:03 +0800
************************************************************************************************/ 
#ifndef __LOW_LEVEL_SHOW_H_
#define __LOW_LEVEL_SHOW_H_

#define LOW_LEVEL_SHOW_OFF              0//关闭显示
#define LOW_LEVEL_SHOW_RTT              1//使用RTT显示

//用户选择的模式
#define LOW_LEVEL_SHOW_USER             LOW_LEVEL_SHOW_RTT
//单行显示数据长度
#define LOW_LEVEL_BUF_SHOW_SINGLE_LINE      20


#if(LOW_LEVEL_SHOW_USER == LOW_LEVEL_SHOW_OFF)
    #define LowLevelSendString(stringPtr)      asm("nop")
    #define LowLevelPrintf(format, ...)        asm("nop")
    #define LowLevelShowErrorSubCode(subCode)  asm("nop")
    #define LowLevelShowHalError(halState)     asm("nop")
    #define LowLevelShowGetLock()              asm("nop")
    #define LowLevelShowReleaseLock()          asm("nop")
#elif(LOW_LEVEL_SHOW_USER == LOW_LEVEL_SHOW_RTT)
    //包含相关文件
    #include "MCU_RTT.h"
    #define LowLevelSendString(stringPtr)      MCU_RTT_SendString(stringPtr)
    #define LowLevelPrintf(format, ...)        MCU_RTT_Printf(format, ##__VA_ARGS__)
    #define LowLevelShowErrorSubCode(subCode)  MCU_RTT_Printf("%s %d 0X%08X\r\n",__FUNCTION__,__LINE__,subCode)
    #define LowLevelShowHalError(halState)     MCU_RTT_Printf("%s,%d,HalResult: 0X%02X\r\n",__FUNCTION__,__LINE__,halState)
    #define LowLevelShowGetLock()              MCU_RTT_GetLock()
    #define LowLevelShowReleaseLock()          MCU_RTT_ReleaseLock()
#endif


//底层打印数组
void LowLevelShowArrayBuffer(uint8_t* prefixStr,uint8_t* bufferPtr,uint16_t bufferLength);


#endif

