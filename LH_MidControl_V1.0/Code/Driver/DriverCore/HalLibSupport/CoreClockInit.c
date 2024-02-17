/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-03-29 15:06:57 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-03-29 15:37:33 +0800
************************************************************************************************/ 
#include "stm32h7xx_hal.h"

//系统主PLL锁相环初始化,该时钟主要供给总线矩阵和系统内核
//PLL_P = 400MHZ PLL_Q = 100MHZ PLL_R = 400MHZ
static void SystemMainPLL_ClockInit(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    //配置外部时钟的驱动能力，在LSE启动前
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);	
    //初始化CPU AHB APB时钟
    //使能HSE，并选择HSE作为PLL时钟源
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
    RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    /*************************************PLL1*****************************************/
    //pllm:PLL1 预分频系数(进PLL 之前的分频),取值范围:2~63.  24/6=4M
    RCC_OscInitStruct.PLL.PLLM = 6;
    //plln:PLL1 倍频系数(PLL 倍频),取值范围:4~512.  4*200 = 800M
    RCC_OscInitStruct.PLL.PLLN = 200;
    //pllp:PLL1 的p分频系数(PLL 之后的分频),分频后作为系统时钟,取值范围:2~128 800/2 = 400M
    RCC_OscInitStruct.PLL.PLLP = 2;
    //pllr:PLL1 的r分频系数(PLL 之后的分频),分频后作为系统时钟,取值范围:2~128 800/2 = 400M
    RCC_OscInitStruct.PLL.PLLR = 2;
    //pllq:PLL1 的q 分频系数(PLL 之后的分频),取值范围:1~128. 800/8 = 100M
    //FDCAN使用PLL1Q,最大支持100MHZ时钟
    RCC_OscInitStruct.PLL.PLLQ = 8;		
    //开始初始化PLL锁相环
    //设置调压器输出电压级别,级别数值越小工作频率越高，所以如果我们要配置 H7 的主频为 400MHz，那么我们必须
    //配置调压器输出电压级别VOS 为级别0
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    /*************************************PLL2*****************************************/
    //等待时钟初始化完成
    while (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        asm("nop");
    }
}


//系统内核时钟和总线矩阵时钟初始化
// system Clock 配置如下: 
//     System Clock source  = PLL (HSE)
//     SYSCLK(Hz)           = 400000000 (CPU Clock)
//     AHB Prescaler        = 2
//     HCLK(Hz)             = 200000000 (AXI and AHBs Clock)
//     D1 APB3 Prescaler    = 2 (APB3 Clock  100MHz)
//     D2 APB1 Prescaler    = 2 (APB1 Clock  100MHz)
//     D2 APB2 Prescaler    = 2 (APB2 Clock  100MHz)
//     D3 APB4 Prescaler    = 2 (APB4 Clock  100MHz)
//     HSE Frequency(Hz)    = 20000000
//     VDD(V)               = 3.3
//     Flash Latency(WS)    = 4
static void SystemCoreBusClockInit(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    //选择PLL的输出作为系统时钟
    //配置RCC_CLOCKTYPE_SYSCLK系统时钟
    //配置RCC_CLOCKTYPE_HCLK 时钟，对应AHB1，AHB2，AHB3和AHB4总线
    //配置RCC_CLOCKTYPE_PCLK1时钟，对应APB1总线
    //配置RCC_CLOCKTYPE_PCLK2时钟，对应APB2总线
    //配置RCC_CLOCKTYPE_D1PCLK1时钟，对应APB3总线
    //配置RCC_CLOCKTYPE_D3PCLK1时钟，对应APB4总线  
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                                |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
    //系统时钟就是锁相环时钟,从PLL_P出来 400MHZ
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    //HCLK为系统时钟2分频 200MHZ
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    //APB1 2 3 4均从AHB二分瓶 100MHZ
    //定时器的时钟都是在D2域进行控制，而且当 D2PPRE1或 D2PPRE2 的分频系数不为1的时候，
    //定时器的时钟频率为rcc_pclk1或rcc_pclk2的2倍,也就是200MHZ定时器
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
    /* 此函数会更新SystemCoreClock，并重新配置HAL_InitTick,同时设置Flash访问周期为4 */
    while (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        asm("nop");
    }
}

//系统时钟初始化
// System Clock 配置
static void SystemMainClockInit(void)
{
    /* 锁住SCU(Supply configuration update) */
	MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);
    //设置电源级别,0标识可以提升到480M,但是需要芯片本身支持
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    //等待电源稳定
    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {};
    //系统主锁相环初始化,PLL_P输出400MHZ
    SystemMainPLL_ClockInit();
    //系统内核时钟与总线矩阵时钟初始化,确定各个分频系数
    SystemCoreBusClockInit();
    //使用IO的高速模式，要使能IO补偿，即调用下面三个函数 
    //使能CSI clock
    //使能SYSCFG clock
    //使能I/O补偿单元， 设置SYSCFG_CCCSR寄存器的bit0
	__HAL_RCC_CSI_ENABLE() ;
    //系统配置时钟使能,该单元负责补偿
	__HAL_RCC_SYSCFG_CLK_ENABLE();
    //使能IO补偿单元
	HAL_EnableCompensationCell();
    /* AXI SRAM的时钟是上电自动使能的，而D2域的SRAM1，SRAM2和SRAM3要单独使能 */	
    __HAL_RCC_D2SRAM1_CLK_ENABLE();
    __HAL_RCC_D2SRAM2_CLK_ENABLE();
    __HAL_RCC_D2SRAM3_CLK_ENABLE();
}

//初始化FMC时钟,使用PLL2R作为时钟,因为SDRAM时钟最大100M,所以必须得整出一个FMC
//时钟为100MHZ的整数倍,也就是200M.主PLL生成的480M时钟不适用,会存在带宽损失
static void SystemFMC_ClockInit(void)
{
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit = {0};
    //初始化FMC端口,使用PLL2的PLL2R
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FMC;
    RCC_PeriphClkInit.FmcClockSelection = RCC_FMCCLKSOURCE_PLL2;
    //PLL2时钟配置
    //24/6=4MHZ
    RCC_PeriphClkInit.PLL2.PLL2M = 6;
    //4*120 = 480MHZ
    RCC_PeriphClkInit.PLL2.PLL2N = 120;
    //480/2=240MHZ
    RCC_PeriphClkInit.PLL2.PLL2P = 2;
    //480/2=240MHZ
    RCC_PeriphClkInit.PLL2.PLL2Q = 2;
    //480/2=240MHZ
    RCC_PeriphClkInit.PLL2.PLL2R = 2;
    //开始初始化PLL锁相环
    //设置调压器输出电压级别,级别数值越小工作频率越高，所以如果我们要配置 H7 的主频为 480MHz，那么我们必须
    //配置调压器输出电压级别VOS 为级别1
    RCC_PeriphClkInit.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_2;
    RCC_PeriphClkInit.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;	
    if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit) != HAL_OK)
    {
        while(1);
    }
    //使能FMC输出
    __FMC_CLK_ENABLE();
}

//主时钟初始化
void SystemClockInit(void)
{
    //主时钟初始化
    SystemMainClockInit();
    //FMC时钟初始化
    SystemFMC_ClockInit();
}
