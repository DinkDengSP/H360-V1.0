/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 14:05:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 18:57:36 +0800
************************************************************************************************/ 
#include "MCU_WDG.h"
#include "LowLevelShow.h"
#include "IntPriority.h"

//独立看门狗句柄
static IWDG_HandleTypeDef IWDG_Handle;

//最长喂狗时间
void MCU_IWDG_Init(uint16_t timeMsMaxFeed)
{
    //使能LSI
    __HAL_RCC_LSI_ENABLE();
    //等待LSI Ready
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) == 0U);
    //检查独立看门狗复位标志位
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDG1RST) != RESET)
	{
        LowLevelSendString("Last Reset Src IWDG\r\n");
		//清除复位标志位
		__HAL_RCC_CLEAR_RESET_FLAGS();
	}
    //初始化IWDG
    IWDG_Handle.Instance = IWDG1;
	//LSI使用34KHZ,分频32,一次计数1MS
	IWDG_Handle.Init.Prescaler = IWDG_PRESCALER_32;
	// 设置重装载寄存器值
	IWDG_Handle.Init.Reload = timeMsMaxFeed;
	//设置要与向下计数器进行比较的窗口值
	IWDG_Handle.Init.Window = IWDG_WINDOW_DISABLE;
	// 初始化IWDG
	HAL_IWDG_Init(&IWDG_Handle);	
	// 启动 IWDG
	__HAL_IWDG_START(&IWDG_Handle);
    //设置调试模式看门狗冻结
    __HAL_DBGMCU_FREEZE_IWDG1();
}

//喂狗
void MCU_IWDG_Feed(void)
{
    //重新装载计数值
    HAL_IWDG_Refresh(&IWDG_Handle);
}


//窗口看门狗句柄
static WWDG_HandleTypeDef WWDG_Handle;

/* WWDG 配置函数
 * upperLimit ：递减计时器的值， 取值范围为：0x7f~0x40，超出范围会直接复位
 * lowerLimit ：窗口值，取值范围为：0x7f~0x40
 * prv：预分频器值，取值基础是PCLK1/4096 = 29KHZ,依据这个计算计数值1对应的MS数
 * WWDG_PRESCALER_1  ==> 34US
 * WWDG_PRESCALER_2  ==> 68US
 * WWDG_PRESCALER_4  ==> 136US
 * WWDG_PRESCALER_8  ==> 273US
 * WWDG_PRESCALER_16 ==> 546US
 * WWDG_PRESCALER_32 ==> 1.09ms
 * WWDG_PRESCALER_64 ==> 2.18ms
 * WWDG_PRESCALER_128 ==> 4.36ms
 *      
 * 例：upperLimit = 127(0x7f，tr的最大值)  
 *    lowerLimit = 80（0x50, 0x40为最小wr最小值）  
 *	  apbDiv = WWDG_PRESCALER_32
*/
//窗口看门狗初始化
void MCU_WWDG_Init(void)
{
    //检查独立看门狗复位标志位
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDG1RST) != RESET)
	{
        LowLevelSendString("Last Reset Src WWDG\r\n");
		//清除复位标志位
		__HAL_RCC_CLEAR_RESET_FLAGS();
	}
    // 开启 WWDG 时钟
	__HAL_RCC_WWDG1_CLK_ENABLE();
	// 配置WWDG中断优先级
	HAL_NVIC_SetPriority(WWDG_IRQn,MCU_WWDG_PREE_PRI,MCU_WWDG_SUB_PRI);
	HAL_NVIC_EnableIRQ(WWDG_IRQn);
	// 配置WWDG句柄即寄存器基地址
	WWDG_Handle.Instance = WWDG1;
	// 设置预分频器值
	WWDG_Handle.Init.Prescaler = WWDG_PRESCALER_128;
	// 设置上窗口值
	WWDG_Handle.Init.Window = 80;	
	// 设置计数器的值
	WWDG_Handle.Init.Counter = 127;
	// 使能提前唤醒中断
	WWDG_Handle.Init.EWIMode = WWDG_EWI_ENABLE;
	// 初始化WWDG
	HAL_WWDG_Init(&WWDG_Handle);
    //调试模式看门狗冻结
    __HAL_DBGMCU_FREEZE_WWDG1();
}

//窗口看门狗喂狗
void MCU_WWDG_Feed(void)
{
    //窗口值
    uint8_t wwdg_wr = WWDG1->CFR & 0X7F;
    //计数器当前值
    uint8_t wwdg_tr = WWDG1->CR & 0X7F;
    if((wwdg_tr <= wwdg_wr)&&(wwdg_tr >= 0X40))
    {
        //喂狗，刷新递减计数器的值，设置成最大WDG_CNT=0X7F
	    HAL_WWDG_Refresh(&WWDG_Handle);
    }
}

//提前唤醒中断
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef* hwwdg)
{
    //喂狗
	HAL_WWDG_Refresh(hwwdg);
}

//中断处理
void WWDG_IRQHandler(void)
{
    HAL_WWDG_IRQHandler(&WWDG_Handle);
}

