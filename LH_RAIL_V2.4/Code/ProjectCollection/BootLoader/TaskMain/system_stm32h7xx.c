/**
  ******************************************************************************
  * @file    system_stm32h7xx.c
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M Device Peripheral Access Layer System Source File.
  *
  *   This file provides two functions and one global variable to be called from 
  *   user application:
  *      - SystemInit(): This function is called at startup just after reset and 
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32h7xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick 
  *                                  timer or configure other parameters.
  *                                     
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32h7xx_system
  * @{
  */

/** @addtogroup STM32H7xx_System_Private_Includes
  * @{
  */

#include "stm32h7xx.h"
#include <math.h>

/*外部晶振频率*/
#if !defined(HSE_VALUE)
#define HSE_VALUE ((uint32_t)24000000) /*!< Value of the External oscillator in Hz */
#endif                                 /* HSE_VALUE */

/*内部RC振荡器频率*/
#if !defined(CSI_VALUE)
#define CSI_VALUE ((uint32_t)4000000) /*!< Value of the Internal oscillator in Hz*/
#endif                                /* CSI_VALUE */

/*HSI 高速内部时钟*/
#if !defined(HSI_VALUE)
#define HSI_VALUE ((uint32_t)64000000) /*!< Value of the Internal oscillator in Hz*/
#endif                                 /* HSI_VALUE */

/**
  * @}
  */

/** @addtogroup STM32H7xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32H7xx_System_Private_Defines
  * @{
  */

/************************* Miscellaneous Configuration ************************/
/*定义系统初始化的时候是否需要使能D2域 SRAM的时钟,默认使能*/
/*!< Uncomment the following line if you need to use initialized data in D2 domain SRAM  */
#define DATA_IN_D2_SRAM

/*定义中断向量表存放位置是在FLASH里面还是SRAM里面*/
/*!< Uncomment the following line if you need to relocate your vector Table in Internal SRAM. */
/* #define VECT_TAB_SRAM */

/*定义中断向量表的偏移,主要用于Boot启动的时候*/
#define VECT_TAB_OFFSET 0x00000000UL /*!< Vector Table base offset field. This value must be a multiple of 0x200. */
/******************************************************************************/

/**
  * @}
  */

/** @addtogroup STM32H7xx_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32H7xx_System_Private_Variables
  * @{
  */
/* This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetHCLKFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency 
         Note: If you use this function to configure the system clock; then there
               is no need to call the 2 first functions listed above, since SystemCoreClock
               variable is updated automatically.
  */
/*当前内核时钟*/
uint32_t SystemCoreClock = 64000000;
/*当前D2域时钟*/
uint32_t SystemD2Clock = 64000000;
/*D1域分频到D2域的默认设定*/
const uint8_t D1CorePrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

/**
  * @}
  */

/** @addtogroup STM32H7xx_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system Initialize the FPU setting, vector table location.
  *         该函数默认被Startup调用,默认初始化完成之后,使用HSI时钟
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
#if defined(DATA_IN_D2_SRAM)
  __IO uint32_t tmpreg;
#endif /* DATA_IN_D2_SRAM */

/* FPU settings 浮点协处理器设定CP10和CP11*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
  SCB->CPACR |= ((3UL << (10 * 2)) | (3UL << (11 * 2))); /* set CP10 and CP11 Full Access */
#endif
  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set HSION bit 打开HSI时钟,高速内部时钟使能*/
  RCC->CR |= RCC_CR_HSION;
  /* Reset CFGR register */
  /*RCC配置清零,使用HSI作为内部时钟*/
  RCC->CFGR = 0x00000000;
  /* Reset HSEON, CSSON , CSION,RC48ON, CSIKERON PLL1ON, PLL2ON and PLL3ON bits */
  /*将特定的Bit位清零,PLL 3 2 1关闭,关闭HSE和HSE安全系统,关闭HSI48M供给USB,CSI关闭,待机下不打开,基本上就是回到复位状态 */
  RCC->CR &= 0xEAF6ED7FU;
  /* Reset D1CFGR register */
  /*清除D1域的时钟配置,回归默认配置*/
  RCC->D1CFGR = 0x00000000;
  /* Reset D2CFGR register */
  /*清除D2域的时钟配置,回归默认配置*/
  RCC->D2CFGR = 0x00000000;
  /* Reset D3CFGR register */
  /*关闭D3域的时钟配置,回归默认配置*/
  RCC->D3CFGR = 0x00000000;
  /* Reset PLLCKSELR register */
  /*PLL1 2 3预分频器静止,并将HSI作为PLL输入时钟*/
  RCC->PLLCKSELR = 0x00000000;
  /* Reset PLLCFGR register */
  /*复位PLL输出,关闭PLL输出*/
  RCC->PLLCFGR = 0x00000000;
  /* Reset PLL1DIVR register */
  /*关闭PLL1分频配置,使用默认配置*/
  RCC->PLL1DIVR = 0x00000000;
  /* Reset PLL1FRACR register */
  /*关闭PLL1小数分频配置,使用默认配置*/
  RCC->PLL1FRACR = 0x00000000;

  /* Reset PLL2DIVR register */
  RCC->PLL2DIVR = 0x00000000;
  /* Reset PLL2FRACR register */
  RCC->PLL2FRACR = 0x00000000;

  /* Reset PLL3DIVR register */
  RCC->PLL3DIVR = 0x00000000;
  /* Reset PLL3FRACR register */
  RCC->PLL3FRACR = 0x00000000;

  /* Reset HSEBYP bit 不旁路HSE晶振*/
  RCC->CR &= 0xFFFBFFFFU;
  /* Disable all interrupts 关闭全部的RCC安全中断*/
  RCC->CIER = 0x00000000;

  /*这里是Y版本的一个BUG,多路总线要读取AXI SRAM会导致读崩溃。AXI_TARG7_FN_MOD 读发起能力设置为1可解决该问题,但是会影响到AXI SRAM的部分性能*/
  /* Change  the switch matrix read issuing capability to 1 for the AXI SRAM target (Target 7) */
  if ((DBGMCU->IDCODE & 0xFFFF0000U) < 0x20000000U)
  {
    /* if stm32h7 revY*/
    /* Change  the switch matrix read issuing capability to 1 for the AXI SRAM target (Target 7) */
    *((__IO uint32_t *)0x51008108) = 0x000000001U;
  }

  /*如果初始化的时候部分数据在D2域的时候,需要使能D2域的SRAM的时钟,一般默认使能即可*/
#if defined(DATA_IN_D2_SRAM)
  /* in case of initialized data in D2 SRAM , enable the D2 SRAM clock */
  RCC->AHB2ENR |= (RCC_AHB2ENR_D2SRAM1EN | RCC_AHB2ENR_D2SRAM2EN | RCC_AHB2ENR_D2SRAM3EN);
  tmpreg = RCC->AHB2ENR;
  (void)tmpreg;
#endif /* DATA_IN_D2_SRAM */

  /*设定中断向量表的位置*/
  /* Configure the Vector Table location add offset address*/
#ifdef VECT_TAB_SRAM
  SCB->VTOR = D1_AXISRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#else
  SCB->VTOR = FLASH_BANK1_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
#endif
}

/**
   * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the core clock , it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *           
  * @note   Each time the core clock changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.         
  *     
  * @note   - The system frequency computed by this function is not the real 
  *           frequency in the chip. It is calculated based on the predefined 
  *           constant and the selected clock source:
  *             
  *           - If SYSCLK source is CSI, SystemCoreClock will contain the CSI_VALUE(*)                                 
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(**)
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(***) 
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the CSI_VALUE(*),
  *             HSI_VALUE(**) or HSE_VALUE(***) multiplied/divided by the PLL factors.
  *
  *         (*) CSI_VALUE is a constant defined in stm32h7xx_hal.h file (default value
  *             4 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.        
  *         (**) HSI_VALUE is a constant defined in stm32h7xx_hal.h file (default value
  *             64 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.   
  *    
  *         (***)HSE_VALUE is a constant defined in stm32h7xx_hal.h file (default value
  *              25 MHz), user has to ensure that HSE_VALUE is same as the real
  *              frequency of the crystal used. Otherwise, this function may
  *              have wrong result.
  *                
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  * @param  None
  * @retval None更新当前系统时钟状态
  */
void SystemCoreClockUpdate(void)
{
  uint32_t pllp, pllsource, pllm, pllfracen, hsivalue, tmp;
  float_t fracn1, pllvco;

  /* Get SYSCLK source -------------------------------------------------------*/

  switch (RCC->CFGR & RCC_CFGR_SWS)
  {
  case RCC_CFGR_SWS_HSI: /* HSI used as system clock source */
    SystemCoreClock = (uint32_t)(HSI_VALUE >> ((RCC->CR & RCC_CR_HSIDIV) >> 3));
    break;
  case RCC_CFGR_SWS_CSI: /* CSI used as system clock  source */
    SystemCoreClock = CSI_VALUE;
    break;
  case RCC_CFGR_SWS_HSE: /* HSE used as system clock  source */
    SystemCoreClock = HSE_VALUE;
    break;
  case RCC_CFGR_SWS_PLL1: /* PLL1 used as system clock  source */
    /* PLL_VCO = (HSE_VALUE or HSI_VALUE or CSI_VALUE/ PLLM) * PLLN
    SYSCLK = PLL_VCO / PLLR
    */
    pllsource = (RCC->PLLCKSELR & RCC_PLLCKSELR_PLLSRC);
    pllm = ((RCC->PLLCKSELR & RCC_PLLCKSELR_DIVM1) >> 4);
    pllfracen = ((RCC->PLLCFGR & RCC_PLLCFGR_PLL1FRACEN) >> RCC_PLLCFGR_PLL1FRACEN_Pos);
    fracn1 = (float_t)(uint32_t)(pllfracen * ((RCC->PLL1FRACR & RCC_PLL1FRACR_FRACN1) >> 3));
    if (pllm != 0U)
    {
      switch (pllsource)
      {
      case RCC_PLLCKSELR_PLLSRC_HSI: /* HSI used as PLL clock source */
        hsivalue = (HSI_VALUE >> ((RCC->CR & RCC_CR_HSIDIV) >> 3));
        pllvco = ((float_t)hsivalue / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1 / (float_t)0x2000) + (float_t)1);
        break;

      case RCC_PLLCKSELR_PLLSRC_CSI: /* CSI used as PLL clock source */
        pllvco = ((float_t)CSI_VALUE / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1 / (float_t)0x2000) + (float_t)1);
        break;

      case RCC_PLLCKSELR_PLLSRC_HSE: /* HSE used as PLL clock source */
        pllvco = ((float_t)HSE_VALUE / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1 / (float_t)0x2000) + (float_t)1);
        break;

      default:
        pllvco = ((float_t)CSI_VALUE / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1 / (float_t)0x2000) + (float_t)1);
        break;
      }
      pllp = (((RCC->PLL1DIVR & RCC_PLL1DIVR_P1) >> 9) + 1U);
      SystemCoreClock = (uint32_t)(float_t)(pllvco / (float_t)pllp);
    }
    else
    {
      SystemCoreClock = 0U;
    }
    break;

  default:
    SystemCoreClock = CSI_VALUE;
    break;
  }

  /* Compute SystemClock frequency 内核时钟计算的D1域时钟*/
  tmp = D1CorePrescTable[(RCC->D1CFGR & RCC_D1CFGR_D1CPRE) >> RCC_D1CFGR_D1CPRE_Pos];
  /* SystemCoreClock frequency : CM7 CPU frequency 不同的分频系数对应不同的分频比例 */
  SystemCoreClock >>= tmp;

  /* SystemD2Clock frequency : AXI and AHBs Clock frequency  D1到D2的分频*/
  SystemD2Clock = (SystemCoreClock >> ((D1CorePrescTable[(RCC->D1CFGR & RCC_D1CFGR_HPRE) >> RCC_D1CFGR_HPRE_Pos]) & 0x1FU));
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
