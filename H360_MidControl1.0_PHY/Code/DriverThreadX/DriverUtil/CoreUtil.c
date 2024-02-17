/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:11:21 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-15 13:51:20 +0800
************************************************************************************************/ 
#include "CoreUtil.h"


//辅助转换映射表
static uint8_t decToHexMapArray[16] = {'0','1','2','3','4','5','6','7','8',
                                        '9','A','B','C','D','E','F'};
//数据转换
static uint8_t CoreDecToHex(uint8_t decValue)
{
    if(decValue < 16)
    {
        return decToHexMapArray[decValue];
    }
    else
    {
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
    for(indexUtil = 0;indexUtil < count; indexUtil++)
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

/*设置默认的中断优先级分组*/
void CoreNVIC_SetGroupDefault(void)
{
    HAL_NVIC_SetPriorityGrouping(SYSTEM_DEFAULT_NVIC_GROUP);
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

//将当前Cache中的数据都写回SRAM
void CoreUtilCacheWriteThrough(void)
{
    (*(__IO uint32_t*)0XE000EF9C=1UL<<2);
}

//芯片内核复位
void CoreSystemReset(void)
{
    __NVIC_SystemReset();
}

//关中断函数,基于BASEPRI以上的中断被关闭
uint32_t CoreLockIntBasePri(void)
{
    return tx_interrupt_control(TX_INT_DISABLE);
}

//开中断函数,在上次关的时候是什么状态,打开就是什么状态
void CoreUnlockIntBasePri(uint32_t lastIntState)
{
    tx_interrupt_control(lastIntState);
}

//关中断函数系统全局中断
uint32_t CoreLockIntGlobal(void)
{
#if(CORE_LOCK_INT_GLOBAL_USER == CORE_LOCK_INT_GLOBAL_PRIMASK)
    uint32_t lastState = __get_PRIMASK();
    //1为禁止全局中断
    __set_PRIMASK(1);
    //返回关闭之前的状态
    return lastState;
#elif(CORE_LOCK_INT_GLOBAL_USER == CORE_LOCK_INT_GLOBAL_FAULTMASK)
    uint32_t lastState = __get_FAULTMASK();
    //1为禁止全局中断
    __set_FAULTMASK(1);
    //返回关闭之前的状态
    return lastState;
#endif
}

//开中断函数,系统全局中断
void CoreUnlockIntGlobal(uint32_t lastIntState)
{
#if(CORE_LOCK_INT_GLOBAL_USER == CORE_LOCK_INT_GLOBAL_PRIMASK)
    //1为禁止全局中断,0为打开全局中断
    __set_PRIMASK(lastIntState);
#elif(CORE_LOCK_INT_GLOBAL_USER == CORE_LOCK_INT_GLOBAL_FAULTMASK)
    //1为禁止全局中断,0为打开全局中断
    __set_FAULTMASK(lastIntState);
#endif
    
}

//锁定系统调度
int CoreLockSchedule(void)
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

//解锁调度
void CoreUnlockSchedule(int oldPriority)
{
    uint32_t currentPriority = 0;
    TX_THREAD* currentThreadPtr = NULL;
    //获取当前线程指针
    currentThreadPtr = tx_thread_identify();
    if(currentThreadPtr == NULL)
    {
        return;
    }
    tx_thread_preemption_change(currentThreadPtr,oldPriority, &currentPriority);
}

//设置主堆栈地址
void CoreSetMainStackPtr(uint32_t addr)
{
    asm("MSR MSP, r0");
	asm("BX r14");
}

//内核进入低功耗待机模式
void CoreEnterLowPowerMode(void)
{
    asm("WFI");
}

#define VECTOR_TABLE_LENGTH         0X400
//拷贝内核中断向量表到指定地址
void CoreCopyIntVectorTable(uint32_t srcAddr,uint32_t dstAddr)
{
    //拷贝中断向量表
    UserMemCopy((uint8_t*)dstAddr,(uint8_t*)srcAddr,VECTOR_TABLE_LENGTH);
    //设置中断向量地址
    SCB->VTOR = dstAddr;
}



