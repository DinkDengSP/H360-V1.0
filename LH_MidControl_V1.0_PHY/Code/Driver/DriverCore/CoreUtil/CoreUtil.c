/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-07-26 13:22:46
**LastEditors: DengXiaoJun
**LastEditTime: 2021-07-27 13:20:05
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CoreUtil.h"
#include "tx_initialize.h"

//导入外部变量
extern ULONG  _tx_thread_system_state;

/*设置默认的中断优先级分组*/
void CoreNVIC_SetGroupDefault(void)
{
    HAL_NVIC_SetPriorityGrouping(SYSTEM_DEFAULT_NVIC_GROUP);
}


//关闭调度器
uint32_t CoreLockScheduler(void)
{
	uint32_t oldThread = 0;
    TX_THREAD* currentThreadPtr = NULL;
    //获取当前线程指针
    currentThreadPtr = tx_thread_identify();
    if(currentThreadPtr == NULL)
    {
        return 0;
    }
    //修改为0优先级,谁都不能抢占
    tx_thread_preemption_change(currentThreadPtr,0,&oldThread);
    //返回原来的优先级
    return oldThread;
}

//打开调度器
void CoreUnlockScheduler(uint32_t oldPreemption)
{
	uint32_t currentPriority = 0;
    TX_THREAD* currentThreadPtr = NULL;
    //获取当前线程指针
    currentThreadPtr = tx_thread_identify();
    if(currentThreadPtr == NULL)
    {
        return;
    }
    tx_thread_preemption_change(currentThreadPtr,oldPreemption, &currentPriority);
}

//关闭中断,基于BASEPRI
uint32_t CoreLockIntBasePri(void)
{
	return tx_interrupt_control(TX_INT_DISABLE);
}

//打开中断,基于BASEPRI
void CoreUnlockIntBasePri(uint32_t oldState)
{
	tx_interrupt_control(oldState);
}

//关闭中断,全局
uint32_t CoreLockIntGlobal(void)
{
	uint32_t lastState = __get_PRIMASK();
    //1为禁止全局中断
    __set_PRIMASK(1);
    //返回关闭之前的状态
    return lastState;
}

//打开中断,全局
void CoreUnlockIntGolbal(uint32_t oldState)
{
	//1为禁止全局中断,0为打开全局中断
    __set_PRIMASK(oldState);
}

//内核进入低功耗待机模式
void CoreEnterLowPowerMode(void)
{
	asm("WFI");
}

//设置主堆栈地址
void CoreSetMainStackPtr(uint32_t addr)
{
	asm("MSR MSP, r0");
	asm("BX r14");
}


//十进制转换为16进制数字
static uint8_t CoreDecToHex(uint8_t decValue)
{
    switch(decValue)
    {
        case 0:
            return '0';
        case 1:
            return '1';
        case 2:
            return '2';
        case 3:
            return '3';
        case 4:
            return '4';
        case 5:
            return '5';
        case 6:
            return '6';
        case 7:
            return '7';
        case 8:
            return '8';
        case 9:
            return '9';
        case 10:
            return 'A';
        case 11:
            return 'B';
        case 12:
            return 'C';
        case 13:
            return 'D';
        case 14:
            return 'E';
        case 15:
            return 'F';
        default:
            return '*';
    }
}

//将Byte类型数据转换为HEX类型字符串,参数指针至少四个字节
static void CoreConvertByteToHexStr(uint8_t byteValue,uint8_t* hexStrPtr)
{
    uint8_t utilByte = 0;
    hexStrPtr[0] = '0';
    hexStrPtr[1] = 'X';
    utilByte = byteValue/16;
    hexStrPtr[2] = CoreDecToHex(utilByte);
    utilByte = byteValue%16;
    hexStrPtr[3] = CoreDecToHex(utilByte);
}


//输出指定元素的数字到hex数组
void CoreConvertByteBufToHexStr(uint8_t* bufPtr,uint8_t count,uint8_t* hexStrPtr,uint16_t strBufLen,uint16_t* resultStrLen)
{
    //计算需要的缓存长度
    uint16_t needStringLen = LENGTH_BYTES_BUF_TO_HEX_STR(count);
    if(strBufLen < needStringLen)
    {
        *resultStrLen = 0;
        return;
    }
    uint16_t indexUtil = 0;
    for(indexUtil = 0 ; indexUtil < count ; indexUtil++)
    {
        //数据写入
        CoreConvertByteToHexStr(bufPtr[indexUtil],hexStrPtr + (indexUtil * 5));
        if(indexUtil != (count-1))
        {
            //空格
            hexStrPtr[(indexUtil * 5) +4] = ' ';
        }
        else
        {
            //最后一个
            hexStrPtr[(indexUtil * 5) +4] = '\r';
            hexStrPtr[(indexUtil * 5) +5] = '\n';
            hexStrPtr[(indexUtil * 5) +6] = 0;
        }
    }
    *resultStrLen = needStringLen;
    return;
}


//获取Icache是否打开
FunctionalState CoreUtilGet_ICahceState(void)
{
    uint8_t state;
    state = (((SCB->CCR)>>17)&0X01);
    return ((state == 0)?DISABLE:ENABLE);
}

//获取DCache是否打开
FunctionalState CoreUtilGet_DCahceState(void)
{
    uint8_t state;
    state = (((SCB->CCR)>>16)&0X01);
    return ((state == 0)?DISABLE:ENABLE);
}




