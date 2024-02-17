/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:11:36 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-15 13:56:14 +0800
************************************************************************************************/ 
#include "MCU_Random.h"
#include "stdlib.h"

//RNG句柄
static RNG_HandleTypeDef RNG_Handler;  

//随机数初始化
void MCU_RandomInit(void)
{
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit = {0};
	//设置RNG时钟源，选择PLL1Q，时钟为100MHz,模块最大可接受频率为250M
	RCC_PeriphClkInit.PeriphClockSelection=RCC_PERIPHCLK_RNG;  
    //RNG时钟源选择PLL	
	RCC_PeriphClkInit.RngClockSelection=RCC_RNGCLKSOURCE_PLL;	
    //配置时钟
    if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit) != HAL_OK)
    {
        while(1);
    }
	//使能时钟
	__HAL_RCC_RNG_CLK_ENABLE();
    //设定
    RNG_Handler.Instance=RNG;  
    //复位 
    HAL_RNG_DeInit(&RNG_Handler);
    //初始化RNG
    HAL_RNG_Init(&RNG_Handler);
    //等待RNG准备就绪
    while(__HAL_RNG_GET_FLAG(&RNG_Handler,RNG_FLAG_DRDY)==RESET)
    {
        asm("nop");
    }
    //初始化随机数函数库
    srand((int32_t)MCU_RandomGetNext());
}

//得到随机数
uint32_t MCU_RandomGetNext(void)
{
    uint32_t randomnum;
    //锁定线程
    uint32_t lastIntState = CoreLockSchedule();
    //生成随机数
    HAL_RNG_GenerateRandomNumber(&RNG_Handler,&randomnum);
    //解除锁定
    CoreUnlockSchedule(lastIntState);
    //返回数据
	return randomnum;
}

//正确计算随机数的方法
int32_t MCU_RandomGetNextInteger(int32_t min, int32_t max)
{
    if (max <= min)
    {
        return 0;
    }
    //基础值
    uint32_t baseResult = MCU_RandomGetNext();
    //将取得的值在32位数据中计算到的位置计算出来,阈值在1
    //然后根据本次阈值扩张,然后取整
    uint32_t rangeBase = (uint32_t)((max - min) * (((double)baseResult) / ((double)UINT32_MAX)));
    //最后确定符号位
    return (int32_t)(min + rangeBase);    
}

//正确计算随机数的方法
uint32_t MCU_RandomGetNextUnsigned(uint32_t min, uint32_t max)
{
    if (max <= min)
    {
        return 0;
    }
    //基础值
    uint32_t baseResult = MCU_RandomGetNext();
    uint32_t rangeBase = (uint32_t)((max - min) * (((double)baseResult) / ((double)UINT32_MAX)));
    return min + rangeBase;   
}




