/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 17:56:47 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:23:35 +0800
************************************************************************************************/ 
#include "MCU_Timer6.h"
#include "IntPriority.h"
#include "LowLevelShow.h"

//定时器基础模式下,定时器周期中断函数回调
#if(MCU_TIMER6_MODE_USER == MCU_TIMER6_MODE_BASE)
//定时器配置
static MCU_TIMER_BASE16_CONFIG tim6BaseConfig = {0};
//定时器对象
static TIM_HandleTypeDef timer6BaseHandler;

//底层复位
void MCU_Timer6_MspDeInit(void)
{
    //使能定时器时钟
    __HAL_RCC_TIM6_CLK_ENABLE();
    //模块复位
    __HAL_RCC_TIM6_FORCE_RESET();
    //取消复位
    __HAL_RCC_TIM6_RELEASE_RESET();
    //关闭定时器中断
    HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
}

//底层初始化
void MCU_Timer6_MspInit(void)
{
    //使能定时器时钟
    __HAL_RCC_TIM6_CLK_ENABLE();
    //根据是否配置终端回调函数决定是否打开中断
    if(tim6BaseConfig.callBackFuncPtr != NULL)
    {
        //设置中断优先级,基础模式下,中断响应级别不用那么高
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, MCU_TIMER6_BASE_PREE_PRI, MCU_TIMER6_BASE_SUB_PRI);
        //打开中断
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    }
}

//分频系数和计数值 
void MCU_Timer6Init(uint16_t freqDivParam,uint16_t countMax,MCU_TimerBaseCallBack callBackFuncPtr)
{
    HAL_StatusTypeDef halState = HAL_OK;
    TIM_ClockConfigTypeDef  timClkSrcConfig = {0};
    TIM_MasterConfigTypeDef masterTrigConfig      = {0}; 
    //选择定时器6
    timer6BaseHandler.Instance = TIM6;   
    //定时器6底层复位
    MCU_Timer6_MspDeInit();       
    //定时器复位
    HAL_TIM_Base_DeInit(&timer6BaseHandler);
    //预分频器
    timer6BaseHandler.Init.Prescaler   = freqDivParam;
    // 计数模式:向上计数
    timer6BaseHandler.Init.CounterMode = TIM_COUNTERMODE_UP; 
    // 计数周期 
    timer6BaseHandler.Init.Period      = countMax;  
    //自动重装载缓冲,打开之后对计数值的设定需要下次发生溢出事件之后才会生效
    timer6BaseHandler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    //参数记录
    tim6BaseConfig.freqDivParam = freqDivParam;
    tim6BaseConfig.countUpValue = countMax;
    tim6BaseConfig.callBackFuncPtr = callBackFuncPtr;
    //初始化定时器底层
    MCU_Timer6_MspInit();
    //初始化定时器
    halState = HAL_TIM_Base_Init(&timer6BaseHandler);    
    if(halState != HAL_OK)
    {
        //失败报错
        LowLevelShowHalError(halState);
        while(1);
    }  
    //选择内部时钟源
    timClkSrcConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    halState = HAL_TIM_ConfigClockSource(&timer6BaseHandler, &timClkSrcConfig);  
    if(halState != HAL_OK)
    {
        //失败报错
        LowLevelShowHalError(halState);
        while(1);
    }  
    //直接复位
    masterTrigConfig.MasterOutputTrigger = TIM_TRGO_RESET; 
    // 高级控制定时器才有的TRGO2
    masterTrigConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
    //禁用主/从模式
    masterTrigConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    halState = HAL_TIMEx_MasterConfigSynchronization(&timer6BaseHandler, &masterTrigConfig);    
    if(halState != HAL_OK)
    {
        //失败报错
        LowLevelShowHalError(halState);
        while(1);
    } 
    //首先关闭定时器中断
    MCU_Timer6Stop();
}

//定时器重新启动
void MCU_Timer6ReStart(uint16_t freqDivParam,uint16_t countMax)
{
    //定时器先停止
    MCU_Timer6Stop();
    //设置新的参数
    tim6BaseConfig.freqDivParam = freqDivParam;
    tim6BaseConfig.countUpValue = countMax;
    //设置新的分频系数
    __HAL_TIM_SET_PRESCALER(&timer6BaseHandler,freqDivParam);
    //设置新的重装载值
    __HAL_TIM_SET_AUTORELOAD(&timer6BaseHandler,countMax);
    //设置计数值为0
    __HAL_TIM_SET_COUNTER(&timer6BaseHandler,0);
    if(tim6BaseConfig.callBackFuncPtr != NULL)
    {
        //打开中断
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
        //启动定时器,带中断
        HAL_TIM_Base_Start_IT(&timer6BaseHandler);
    }
    else
    {
        //关闭定时器中断
        HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
        //启动定时器,不带中断
        HAL_TIM_Base_Start(&timer6BaseHandler);
    }
}

//定时器启动
void MCU_Timer6Start(void)
{
    //定时器先停止
    MCU_Timer6Stop();
    //设置计数值为0
    __HAL_TIM_SET_COUNTER(&timer6BaseHandler,0);
    //设置参数为内部保存参数
    __HAL_TIM_SET_PRESCALER(&timer6BaseHandler,tim6BaseConfig.freqDivParam);
    __HAL_TIM_SET_AUTORELOAD(&timer6BaseHandler,tim6BaseConfig.countUpValue);
    if(tim6BaseConfig.callBackFuncPtr != NULL)
    {
        //打开中断
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
        //启动定时器,带中断
        HAL_TIM_Base_Start_IT(&timer6BaseHandler);
    }
    else
    {
        //关闭定时器中断
        HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
        //启动定时器,不带中断
        HAL_TIM_Base_Start(&timer6BaseHandler);
    }
}

//定时器停止
void MCU_Timer6Stop(void)
{
    //关闭定时器中断
    HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
    if(tim6BaseConfig.callBackFuncPtr != NULL)
    {
        //关闭定时器
        HAL_TIM_Base_Stop_IT(&timer6BaseHandler); 
    }
    else
    {
        //启动定时器,不带中断
        HAL_TIM_Base_Stop(&timer6BaseHandler);
    }
}

//获取定时器当前值
uint16_t MCU_Timer6GetCurrentCount(void)
{
    return __HAL_TIM_GET_COUNTER(&timer6BaseHandler);
}

//定时器中断
void TIM6_DAC_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用定时器中断处理函数
    HAL_TIM_IRQHandler(&timer6BaseHandler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

//回调函数实现
void MCU_Timer6_PeriodElapsedCallback(void)
{
    //调用回调函数
    if(tim6BaseConfig.callBackFuncPtr != NULL)
    {
        tim6BaseConfig.callBackFuncPtr();
    }
#if(MCU_TIMER6_INT_MSH_SHOW == 1)
    LowLevelSendStringInt("MCU_Timer6_PeriodElapsedCallback\r\n");
#endif
}

#endif

//DAC模式,只是输出,不带中断
#if(MCU_TIMER6_MODE_USER == MCU_TIMER6_MODE_DAC)
//定时器配置
MCU_TIMER_BASE16_CONFIG tim6BaseConfig = {0};
//定时器对象
static TIM_HandleTypeDef timer6BaseHandler;
//底层复位
void MCU_Timer6_MspDeInit(void)
{
    //使能定时器时钟
    __HAL_RCC_TIM6_CLK_ENABLE();
    //模块复位
    __HAL_RCC_TIM6_FORCE_RESET();
    //取消复位
    __HAL_RCC_TIM6_RELEASE_RESET();
}

//底层初始化
void MCU_Timer6_MspInit(void)
{
    //使能定时器时钟
    __HAL_RCC_TIM6_CLK_ENABLE();
}

//分频系数和计数值 
void MCU_Timer6Init(uint16_t freqDivParam,uint16_t countMax)
{
    HAL_StatusTypeDef halState = HAL_OK;
    TIM_ClockConfigTypeDef  timClkSrcConfig = {0};
    TIM_MasterConfigTypeDef masterTrigConfig      = {0}; 
    //选择定时器6
    timer6BaseHandler.Instance = TIM6;   
    //定时器6底层复位
    MCU_Timer6_MspDeInit();       
    //定时器复位
    HAL_TIM_Base_DeInit(&timer6BaseHandler);
    //预分频器
    timer6BaseHandler.Init.Prescaler   = freqDivParam;
    // 计数模式:向上计数
    timer6BaseHandler.Init.CounterMode = TIM_COUNTERMODE_UP; 
    // 计数周期 
    timer6BaseHandler.Init.Period      = countMax;  
    //自动重装载缓冲,打开之后对计数值的设定需要下次发生溢出事件之后才会生效
    timer6BaseHandler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    //参数记录
    tim6BaseConfig.freqDivParam = freqDivParam;
    tim6BaseConfig.countUpValue = countMax;
    tim6BaseConfig.callBackFuncPtr = NULL;
    //初始化定时器底层
    MCU_Timer6_MspInit();
    //初始化定时器
    halState = HAL_TIM_Base_Init(&timer6BaseHandler);    
    if(halState != HAL_OK)
    {
        //失败报错
        LowLevelShowHalError(halState);
        while(1);
    }  
    //选择内部时钟源
    timClkSrcConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    halState = HAL_TIM_ConfigClockSource(&timer6BaseHandler, &timClkSrcConfig);  
    if(halState != HAL_OK)
    {
        //失败报错
        LowLevelShowHalError(halState);
        while(1);
    }  
    //DAC触发模式,触发输出
    masterTrigConfig.MasterOutputTrigger = TIM_TRGO_UPDATE; 
    // 高级控制定时器才有的TRGO2
    masterTrigConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
    //禁用主/从模式
    masterTrigConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    halState = HAL_TIMEx_MasterConfigSynchronization(&timer6BaseHandler, &masterTrigConfig);    
    if(halState != HAL_OK)
    {
        //失败报错
        LowLevelShowHalError(halState);
        while(1);
    } 
    //首先关闭定时器中断
    MCU_Timer6Stop();
}

//定时器重新启动
void MCU_Timer6ReStart(uint16_t freqDivParam,uint16_t countMax)
{
    tim6BaseConfig.freqDivParam = freqDivParam;
    tim6BaseConfig.countUpValue = countMax;
    //关闭中断
    __HAL_TIM_DISABLE_IT(&timer6BaseHandler,TIM_IT_UPDATE);
    //关闭定时器,不带中断
    HAL_TIM_Base_Stop(&timer6BaseHandler);
    //设置新的分频系数
    __HAL_TIM_SET_PRESCALER(&timer6BaseHandler,freqDivParam);
    //设置新的重装载值
    __HAL_TIM_SET_AUTORELOAD(&timer6BaseHandler,countMax);
    //设置计数值为0
    __HAL_TIM_SET_COUNTER(&timer6BaseHandler,0);
    //启动定时器,不带中断
    HAL_TIM_Base_Start(&timer6BaseHandler);
}

//定时器启动
void MCU_Timer6Start(void)
{
    //关闭中断
    __HAL_TIM_DISABLE_IT(&timer6BaseHandler,TIM_IT_UPDATE);
    //关闭定时器,不带中断
    HAL_TIM_Base_Stop(&timer6BaseHandler);
    //设置计数值为0
    __HAL_TIM_SET_COUNTER(&timer6BaseHandler,0);
    //设置参数为内部保存参数
    __HAL_TIM_SET_PRESCALER(&timer6BaseHandler,tim6BaseConfig.freqDivParam);
    __HAL_TIM_SET_AUTORELOAD(&timer6BaseHandler,tim6BaseConfig.countUpValue);
    //启动定时器,不带中断
    HAL_TIM_Base_Start(&timer6BaseHandler);
}

//定时器停止
void MCU_Timer6Stop(void)
{
    //关闭中断
    __HAL_TIM_DISABLE_IT(&timer6BaseHandler,TIM_IT_UPDATE);
    //关闭定时器,不带中断
    HAL_TIM_Base_Stop(&timer6BaseHandler);
}

//获取定时器当前值
uint16_t MCU_Timer6GetCurrentCount(void)
{
    return __HAL_TIM_GET_COUNTER(&timer6BaseHandler);
}

#endif



//使用测试用例且定时器为基础模式下的测试用例
#if((MCU_TIMER6_TEST_CASE_USE != 0)&&(MCU_TIMER6_MODE_USER == MCU_TIMER6_MODE_BASE))
static uint32_t timer6IntTrigFlag = 0;
static uint32_t timer6IntTrigTick = 0;

//TIMER6测试用例中断回调函数
static void MCU_Timer6_TestCaseCallBack(void)
{
    timer6IntTrigFlag++;
    timer6IntTrigTick = tx_time_get();
}

//测试用例初始化
void MCU_Timer6_TestCaseInit(void)
{
    timer6IntTrigFlag = 0;
    timer6IntTrigTick = 0;
    //初始化定时器6,默认1S一次中断
    MCU_Timer6Init(TIM6_CLK_SRC_HZ/10000,10000,MCU_Timer6_TestCaseCallBack);
    //启动定时器
    MCU_Timer6Start();
}

#include "SrvTaskConfig.h"
//测试用例周期运转
void MCU_Timer6_TestCaseRunPeriod(void)
{
    if((timer6IntTrigFlag != 0)&&(timer6IntTrigTick != 0))
    {
        SystemPrintf("Timer6 BaseModeTestBaseIntCallBack,IntCount: %d,IntTick: %d\r\n",timer6IntTrigFlag,timer6IntTrigTick);
        //清空数据
        timer6IntTrigFlag = 0;
        timer6IntTrigTick = 0;
    }
}
#endif

