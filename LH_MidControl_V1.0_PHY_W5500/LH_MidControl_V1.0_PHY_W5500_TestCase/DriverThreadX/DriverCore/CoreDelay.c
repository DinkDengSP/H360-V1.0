/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-10-27 16:50:01 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-11 14:44:06 +0800
************************************************************************************************/ 
#include "CoreDelay.h"
#include "MCU_CoreDriverConfig.h"
#include "tx_api.h"
#include "tx_initialize.h"

//调试端口地址宏定义
#define  DWT_CYCCNT                         *(volatile unsigned int *)0xE0001004
#define  DWT_CR                             *(volatile unsigned int *)0xE0001000
#define  DEM_CR                             *(volatile unsigned int *)0xE000EDFC
#define  DBGMCU_CR                          *(volatile unsigned int *)0xE0042004
#define  DWT_LAR                            *(volatile unsigned int *)0xE0001FB0
//数据宏定义        
#define  DEM_CR_TRCENA                      (1 << 24)
#define  DWT_CR_CYCCNTENA                   (1 <<  0)
//解锁DWT访问锁定       
#define  DWT_LAR_UNLOCK                     (uint32_t)0xC5ACCE55

//操作系统最小周期
#define THREAD_X_TIME_CYCLE_MIN_MS          1

//初始化DWT,无论是否使用
void CoreDWT_Init(void)
{
    //解锁DWT的访问
    DWT_LAR        |= DWT_LAR_UNLOCK;   
    //暂停DWT 
    DEM_CR         |= (unsigned int)DEM_CR_TRCENA; 
    //DWT计数器清零  
	DWT_CYCCNT     = (unsigned int)0u;
    //DWT开始计数
	DWT_CR         |= (unsigned int)DWT_CR_CYCCNTENA;
}

//使用调试接口作为系统定时接口,存在弊端就是jlink退出调试的时候可能会导致停止
#if(CORE_DELAY_MODE_USER == CORE_DELAY_MODE_DWT)
//导入系统是否启动消息
extern ULONG _tx_thread_system_state;

//延时1US的计数值
#define  DWT_DELAY_ONE_US_COUNT     ((uint32_t)(SystemCoreClock/1000000))
//单次延时微秒最大值
#define  DWT_DELAY_MAX_US_SINGLE    (uint32_t)(0XFFFFFFFF/DWT_DELAY_ONE_US_COUNT)

//核心延时函数初始化
void CoreDelayInit(void)
{
    //初始化DWT
    CoreDWT_Init();
}

//内部使用的微秒延时函数,拆开来用于控制溢出
//系统延时微妙,内部使用,32位计数器 180M下的计数最大时间大概是23S左右
//两个32位无符号数相减，获取的结果再赋值给32位无符号数依然可以正确的获取差值。
//假如A,B,C都是32位无符号数。
//如果A > B  那么A - B = C，这个很好理解，完全没有问题
//如果A < B  那么A - B = C， C的数值就是0xFFFFFFFF - B + A + 1。这一点要特别注意，正好用于本函数。
static void CoreDelayUsBase(uint32_t nus)
{
    uint32_t tCnt, tDelayCnt;
	uint32_t tStart;
    /* 刚进入时的计数器值 */
	tStart = DWT_CYCCNT;                                     
	tCnt = 0;
    /* 需要的节拍数 */ 
	tDelayCnt = nus*DWT_DELAY_ONE_US_COUNT;	 		      
    /* 求减过程中，如果发生第一次32位计数器重新计数，依然可以正确计算 */
	while(tCnt < tDelayCnt)
	{
		tCnt = DWT_CYCCNT - tStart; 	
	}
}

//核心延时微秒
void CoreDelayUs(uint32_t nus)
{
    //基础延时的单次延时最大微秒数
    uint32_t timeUsSingleMax = DWT_DELAY_MAX_US_SINGLE;
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
    //余数延时
    CoreDelayUsBase(timeUsRemain);
}




#elif(CORE_DELAY_MODE_USER == CORE_DELAY_MODE_TIMER6)
#include "MCU_Timer6.h"
//单次延时最大微秒数
#define TIMER_DELAY_MAX_US_SINGLE       60000
static uint32_t overFlowFlag = 0;
//导入系统是否启动消息
extern ULONG _tx_thread_system_state;
//溢出处理
static void CoreDelayOverFlow(void)
{
    overFlowFlag++;
}

//核心延时函数初始化
void CoreDelayInit(void)
{
    //定时器一个计数1US,最大60000,最大计数值60MS
    MCU_Timer6Init((TIM6_CLK_SRC_HZ/1000000)-1,60000,CoreDelayOverFlow);
    //溢出计数清零
    overFlowFlag = 0;
    //初始化先停止计时器
    MCU_Timer6Stop();
    //初始化DWT,ThreadX任务统计要用
    CoreDWT_Init();
}

//微秒基础延时
static void CoreDelayUsBase(uint32_t nus)
{
    //计算需要等待的计数值
    uint32_t waitCount = nus;
    //清除溢出标记
    overFlowFlag = 0;
    //开始计时
    MCU_Timer6Start();
    while(MCU_Timer6GetCurrentCount() < waitCount)
    {
        if(overFlowFlag != 0)
        {
            break;
        }
    }
    //停止计数
    MCU_Timer6Stop();
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
#elif(CORE_DELAY_MODE_USER == CORE_DELAY_MODE_TIMER7)
#include "MCU_Timer7.h"
//单次延时最大微秒数
#define TIMER_DELAY_MAX_US_SINGLE       60000
static uint32_t overFlowFlag = 0;
//导入系统是否启动消息
extern ULONG _tx_thread_system_state;
//溢出处理
static void CoreDelayOverFlow(void)
{
    overFlowFlag++;
}

//核心延时函数初始化
void CoreDelayInit(void)
{
    //定时器一个计数1US,最大60000,最大计数值60MS
    MCU_Timer7Init((TIM7_CLK_SRC_HZ/1000000)-1,60000,CoreDelayOverFlow);
    //溢出计数清零
    overFlowFlag = 0;
    //初始化先停止计时器
    MCU_Timer7Stop();
    //初始化DWT,ThreadX任务统计要用
    CoreDWT_Init();
}

//微秒基础延时
static void CoreDelayUsBase(uint32_t nus)
{
    //计算需要等待的计数值
    uint32_t waitCount = nus;
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

//使用SysTick定时器作为系统延时,最好不要用这个,会妨碍操作系统的正常运行
#elif(CORE_DELAY_MODE_USER == CORE_DELAY_MODE_SYSTICK)
//导入系统是否启动消息
extern ULONG _tx_thread_system_state;

//系统延时1US的计数值
#define TIMER_DELAY_US_SINGLE           (SystemCoreClock/1000000)
//单次延时最大微秒数
#define TIMER_DELAY_MAX_US_SINGLE       (0X00FFFFFF/TIMER_DELAY_US_SINGLE)

//核心延时函数初始化
void CoreDelayInit(void)
{
    //初始化DWT
    CoreDWT_Init();
    //SYSTICK不需要初始化,因为操作系统已经初始化了
}

//微秒基础延时
static void CoreDelayUsBase(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    //LOAD的值
    uint32_t reload = SysTick->LOAD;
    //需要的节拍数
    ticks = nus * TIMER_DELAY_US_SINGLE;
    //刚进入时的计数器值
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            //这里注意一下SYSTICK是一个递减的计数器就可以了.
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            //时间超过/等于要延迟的时间,则退出.
            if (tcnt >= ticks)
                break;
        }
    }
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

/*-----------------------------------------------使用基础时间延时进行高级延时函数------------------------------------------------*/
//核心延时毫秒
void CoreDelayMs(uint32_t nms)
{
    uint32_t timeMsQuotient = nms/THREAD_X_TIME_CYCLE_MIN_MS;
    uint32_t timeMsRemain = nms%THREAD_X_TIME_CYCLE_MIN_MS;
    if(_tx_thread_system_state == TX_INITIALIZE_IS_FINISHED)
    {
        //操作系统已经初始化完成,先调用系统级别的延时
        tx_thread_sleep(timeMsQuotient);
        //除不完的余数调用底层延时
        if(timeMsRemain != 0)
        {
            CoreDelayUs(timeMsRemain*1000);
        }
    }
    else
    {
        //系统还没初始化,调用底层延时
        CoreDelayUs(nms*1000);
    }
}

//核心延时最小时间片
void CoreDelayMinTick(void)
{
    if (_tx_thread_system_state == TX_INITIALIZE_IS_FINISHED)
    {
        tx_thread_sleep(1);
    }
    else
    {
        CoreDelayUs(1000*THREAD_X_TIME_CYCLE_MIN_MS);
    }
}



/*------------------------------重载HAL库需要的几个计时函数-----------------------------*/
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
   return HAL_OK;
}

//延时函数
void HAL_Delay(uint32_t Delay)
{
	CoreDelayUs(Delay*1000);
}

//系统计时
uint32_t HAL_GetTick(void)
{
    static uint32_t ticks = 0U;
    uint32_t i;
    //获取当前操作系统时间
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








