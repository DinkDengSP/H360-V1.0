/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-07-26 13:23:01
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-19 10:12:34
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CoreDelay.h"
#include "tx_api.h"
#include "tx_initialize.h"

//系统最小延时毫秒
//系统周期定义
#define CYCLE_THREAD_X_HZ                       1000
#define MX_MIN_THREAD_X                         (1000/CYCLE_THREAD_X_HZ)

//导入外部变量
extern ULONG  _tx_thread_system_state;

#if(CORE_DELAY_MODE_USER == CORE_DELAY_MODE_DWT)

//DWT相关宏定义
#define  DWT_CYCCNT  *(volatile unsigned int *)0xE0001004
#define  DWT_CR      *(volatile unsigned int *)0xE0001000
#define  DEM_CR      *(volatile unsigned int *)0xE000EDFC
#define  DBGMCU_CR   *(volatile unsigned int *)0xE0042004
#define  DWT_LAR     *(volatile unsigned int *)0xE0001FB0

//DWT相关数据定义
#define  DEM_CR_TRCENA               (1 << 24)
#define  DWT_CR_CYCCNTENA            (1 <<  0)
#define  DWT_LAR_UNLOCK              (uint32_t)0xC5ACCE55

//微秒帧延时最大时间
#define TIME_DELAY_US_STAMP_MAX(systemClock)     (((uint32_t)((0XFFFFFFFF)/systemClock))*1000000)

//延时函数初始化
void CoreDelayInit(void)
{
    //DWT解锁
    DWT_LAR        |= DWT_LAR_UNLOCK;    
    //暂停DWT
    DEM_CR         |= (unsigned int)DEM_CR_TRCENA;
    //DWT计数清零   
	DWT_CYCCNT      = (unsigned int)0u;
    //启动DWT计数
	DWT_CR         |= (unsigned int)DWT_CR_CYCCNTENA;
}

//延时指定微秒,只使用DWT延时
//两个32位无符号数相减，获取的结果再赋值给32位无符号数依然可以正确的获取差值。
//假如A,B,C都是32位无符号数。
//如果A > B  那么A - B = C，这个很好理解，完全没有问题
//如果A < B  那么A - B = C， C的数值就是0xFFFFFFFF - B + A + 1。这一点要特别注意，正好用于本函数。
//主频480M的情况下(H7最大主频),单次的最大延时时间是 8S
static void CoreDelayUsStamp(uint32_t delayCountUs)
{
    uint32_t tCnt, tDelayCnt;
	uint32_t tStart;
    /* 刚进入时的计数器值 */
	tStart = DWT_CYCCNT;    
    //延时计数器                                 
	tCnt = 0;
    /* 需要的节拍数 */ 	
	tDelayCnt = delayCountUs * (SystemCoreClock / 1000000);	 	      
    //等待指定节拍计数
	while(tCnt < tDelayCnt)
	{
        /* 求减过程中，如果发生第一次32位计数器重新计数，依然可以正确计算 */
		tCnt = DWT_CYCCNT - tStart; 	
	}
}


//延时指定微妙,带自动溢出切片
void CoreDelayUs(uint32_t nus)
{
    uint32_t timeStampMax = TIME_DELAY_US_STAMP_MAX(SystemCoreClock);
    //最大帧几帧
    uint32_t stampCount = nus/timeStampMax;
    //剩余微妙时间
    uint32_t stampDiv = nus%timeStampMax;
    //最大帧延时
    if(stampCount != 0)
    {
        do
        {
            CoreDelayUsStamp(timeStampMax);
            stampCount--;
        } while (stampCount > 0);
    }
    //剩余延时
    if(stampDiv != 0)
    {
        CoreDelayUsStamp(stampDiv);
    }
}

#elif(CORE_DELAY_MODE_USER == CORE_DELAY_MODE_TIMER)

#include "MCU_Timer7.h"
//单次延时最大微秒数
#define TIMER_DELAY_MAX_US_SINGLE       5000
static uint32_t overFlowFlag = 0;
//溢出处理
static void CoreDelayOverFlow(void)
{
    overFlowFlag++;
}

//核心延时函数初始化
void CoreDelayInit(void)
{
    //主频400M,定时器频率200M
    //分频19,一个周期0.1US,计数值60000,溢出时间6MS
    MCU_Timer7Init(19,60000,TIM_TRGO_RESET,CoreDelayOverFlow);
    //初始化先停止计时器
    MCU_Timer7Stop();
}

//单次延时
static void CoreDelayUsBase(uint32_t nus)
{
    //计算需要等待的计数值
    uint32_t waitCount = nus*10;
    //清除溢出标记
    overFlowFlag = 0;
    //开始计时
    MCU_Timer7Start();
    while(MCU_Timer7GetCurrentCount() < waitCount)
    {
        if(overFlowFlag != 0)
        {
            break;
        }
    }
    //停止计数
    MCU_Timer7Stop();
    return;
}

//核心延时微秒
void CoreDelayUs(uint32_t nus)
{
    //基础延时的单次延时最大微秒数
    uint32_t timeUsSingleMax = TIMER_DELAY_MAX_US_SINGLE;
    //延时商
    uint32_t timeUsQuotient = (nus/timeUsSingleMax);
    //延时余数
    uint32_t timeUsRemain = (nus%timeUsSingleMax);
    //基础延时
    while(timeUsQuotient != 0)
    {
        CoreDelayUsBase(timeUsSingleMax);
        timeUsQuotient--;
    }
    if(timeUsRemain != 0)
    {
        //余数延时
        CoreDelayUsBase(timeUsRemain);
    }
}

#endif


//延时指定毫秒
void CoreDelayMs(uint32_t nms)
{
    uint32_t msStamp = nms/MX_MIN_THREAD_X;
    uint32_t nmsDiv = nms%MX_MIN_THREAD_X;
    if (_tx_thread_system_state == TX_INITIALIZE_IS_FINISHED)
    {
        //系统已经初始化完成
        if(msStamp > 0)
        {
            //先调用系统延时
            tx_thread_sleep(msStamp);
        }
        if(nmsDiv > 0)
        {
            //后调用自身延时
            CoreDelayUs(nmsDiv*1000);
        }
    }
    else
    {
        //系统还没有初始化
        CoreDelayUs(nms*1000);
    }
}

//延时操作系统最小周期
void CoreDelayMinTick(void)
{
    CoreDelayMs(2);
}

/*
*********************************************************************************************************
*	函 数 名: HAL_Delay
*	功能说明: 重定向毫秒延迟函数。替换HAL中的函数。因为HAL中的缺省函数依赖于Systick中断，如果在USB、SD卡
*             中断中有延迟函数，则会锁死。也可以通过函数HAL_NVIC_SetPriority提升Systick中断
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void HAL_Delay(uint32_t Delay)
{
	CoreDelayMs(Delay);
}

HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority)
{
   return HAL_OK;
}

uint32_t HAL_GetTick (void)
{
    static uint32_t ticks = 0U;
    uint32_t i;
    if (_tx_thread_system_state == TX_INITIALIZE_IS_FINISHED)
    {
        return ((uint32_t)_tx_time_get());
    }
    /* 如果ThreadX还没有运行，采用下面方式 */
    for (i = (SystemCoreClock >> 14U); i > 0U; i--)
    {
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    }
    return ++ticks;
}
