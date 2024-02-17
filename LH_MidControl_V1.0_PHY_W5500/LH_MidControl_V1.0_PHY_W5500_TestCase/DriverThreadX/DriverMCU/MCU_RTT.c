/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:55:04 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:24:46 +0800
************************************************************************************************/ 
#include "MCU_RTT.h"
#include "MCU_RTT_DriverConfig.h"
#include "tx_mutex.h"

//用于printf的缓冲区
static uint8_t blockBufferWithRTT_Printf[SEGGER_RTT_PRINTF_LENGTH] = {0};
//互斥量
static TX_MUTEX mutexRTT;

//RTT初始化
void MCU_RTT_Init(void)
{
    //配置调试库RTT
    if(mutexRTT.tx_mutex_id == TX_MUTEX_ID)
    {
        //已经初始化过了
        return;
    }
    //创建互斥量,启用优先级继承
    tx_mutex_create(&mutexRTT,"mutexRTT",TX_INHERIT);
    //配置RTT调试端口
    SEGGER_RTT_Init();
    /* 配置通道0，上行配置*/
    SEGGER_RTT_ConfigUpBuffer(SEGGER_RTT_MAIN_CHANNEL, "RTTUP", NULL, 0, SEGGER_RTT_UP_MODE);
    /* 配置通道0，下行配置*/	
    SEGGER_RTT_ConfigDownBuffer(SEGGER_RTT_MAIN_CHANNEL, "RTTDOWN", NULL, 0, SEGGER_RTT_DOWN_MODE);
}

//获取锁
void MCU_RTT_GetLock(void)
{
    //申请互斥量
    tx_mutex_get(&mutexRTT,TX_WAIT_FOREVER);
}

//释放锁
void MCU_RTT_ReleaseLock(void)
{
    //释放互斥量
    tx_mutex_put(&mutexRTT);
}

//RTT发送数组
void MCU_RTT_SendBuffer(uint8_t* bufferPtr,uint16_t bufferLength)
{
    //申请互斥量
    tx_mutex_get(&mutexRTT,TX_WAIT_FOREVER);
    //数据输出
    SEGGER_RTT_Write(SEGGER_RTT_MAIN_CHANNEL,bufferPtr, (uint16_t)bufferLength);
    //释放互斥量
    tx_mutex_put(&mutexRTT);
}

//RTT发送字符串
int32_t MCU_RTT_SendString(uint8_t* stringPtr)
{
    int32_t strLength = strlen((char const*)stringPtr);
    //申请互斥量
    tx_mutex_get(&mutexRTT,TX_WAIT_FOREVER);
    //数据输出
    SEGGER_RTT_Write(0,stringPtr, (uint16_t)strLength);
    //释放互斥量
    tx_mutex_put(&mutexRTT);
    return strLength;
}

//RTT发送字符串,可以在中断中使用
int MCU_RTT_SendStringInt(uint8_t* stringPtr)
{
    int32_t strLength = strlen((char const*)stringPtr);
    //数据输出
    SEGGER_RTT_Write(0,stringPtr, (uint16_t)strLength);
    return strLength;
}

//RTT打印
int MCU_RTT_Printf(const char *format, ...)
{
    //转换
    va_list arg;
    int rv;
    //申请互斥量
    tx_mutex_get(&mutexRTT,TX_WAIT_FOREVER);
    //关闭调度
    int oldState = CoreLockSchedule();
    //启动可变参数解析
    va_start(arg, format);
    //将数据转换成带打印缓存
    rv = vsnprintf((char *)blockBufferWithRTT_Printf, SEGGER_RTT_PRINTF_LENGTH, format, arg);
    va_end(arg);
    //解锁调度
    CoreUnlockSchedule(oldState);
    //将数据发送出去
    if ((rv > 0) && (rv <= SEGGER_RTT_PRINTF_LENGTH))
    {
        //RV大于0代表转换成功,且RV在指定范围内部LENGTH_UART6_BUFFER_WITH_FORMAT
        SEGGER_RTT_Write(0,blockBufferWithRTT_Printf, (uint16_t)rv);
    }
    else
    {
        //sprintf转换失败
        MCU_RTT_SendString("ShowStringLength Too Length,No EnoughBuffer To Sprintf\r\n");
    }
    //释放互斥量
    tx_mutex_put(&mutexRTT);
    return rv;
}

