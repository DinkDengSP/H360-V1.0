/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-11 14:31:13 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 20:00:56 +0800
************************************************************************************************/ 
#ifndef __MCU_CLOCK_DRIVER_CONFIG__
#define __MCU_CLOCK_DRIVER_CONFIG__
#include "MCU_CoreDriverConfig.h"

/*---------------------------------------内核时钟选择-----------------------------------------------------------*/
//常规400M运行
#define CORE_CLOCK_MODE_400M                            0
//超频480M运行          
#define CORE_CLOCK_MODE_480M                            1
//用户使用的内核时钟模式            
#define CORE_CLOCK_MODE_USER                            CORE_CLOCK_MODE_400M
//根据定义的时钟定义各个总线的频率
#if(CORE_CLOCK_MODE_USER == CORE_CLOCK_MODE_480M)
#define AHB_CLOCK_FREQ                                  240000000
#define APB1_CLOCK_FREQ                                 120000000
#define APB2_CLOCK_FREQ                                 120000000
#define APB4_CLOCK_FREQ                                 120000000
#else
#define AHB_CLOCK_FREQ                                  200000000
#define APB1_CLOCK_FREQ                                 100000000
#define APB2_CLOCK_FREQ                                 100000000
#define APB4_CLOCK_FREQ                                 100000000
#endif

//将HSE分频输入锁相环,HSE Frequency(Hz)    = 24000000
#define CORE_MAIN_PLL_M                                 6
//根据不同的目标时钟选择不同锁相环倍频系数
#if(CORE_CLOCK_MODE_USER ==   CORE_CLOCK_MODE_480M)
#define CORE_MAIN_PLL_N                                 240
#elif(CORE_CLOCK_MODE_USER == CORE_CLOCK_MODE_400M)
#define CORE_MAIN_PLL_N                                 200
#endif
//锁相环输入时钟分频到系统内核
#define CORE_MAIN_PLL_P                                 2  
//锁相环时钟输入到PLLQ,内核的四分之一,供给SDMMC1,SAI1,SAI4A,SAI4B,USB
//SPI123,FDCAN,QSPI,FMC,SPDIF,SAI2_SAIA,SAI2_SAIB
#define CORE_MAIN_PLL_Q                                 8  
//锁相环时钟输入到PLLR,与内核相等,这个时钟只能供给TRACE
#define CORE_MAIN_PLL_R                                 2  

//PLL2时钟设置,PLL2M,HSE Frequency(Hz)= 24000000,设置输出4M
#define CLK_PLL2_M_PARAM                                6
//锁相环倍频系数,倍频200,输出800M
#define CLK_PLL2_N_PARAM                                200
//PLL2P,400M,输出供给CLK_OUT2,LPTIM2,LPTIM234,ADC,SAI4AB,SPI123,LPTIM1,SAI1,SAI2
#define CLK_PLL2_P_PARAM                                2
//PLL2Q,400M.输出供给FDCAN,SPI6,LPUART1,UART234578,SPI45,USART16
#define CLK_PLL2_Q_PARAM                                2
//PLL2R,200M,输出供给SDMMC1,QSPI,FMC,SPDIF
#define CLK_PLL2_R_PARAM                                4

//将HSE分频输入锁相环,HSE Frequency(Hz)= 24000000,设置输出4M
#define CLK_PLL3_M_PARAM                                5  
//将输入锁相环的时钟倍频,4*60=240MHZ                   
#define CLK_PLL3_N_PARAM                                60 
//PLL3P,120M,输出供给SAI4AB SPI123,SAI1,SAI2,SAI3                    
#define CLK_PLL3_P_PARAM                                2
//PLL3Q,48M,输出供给,USB,SPI6,LPUART1,USART16,UART234578,SPI45                   
#define CLK_PLL3_Q_PARAM                                5  
//PLL3R,48M,输出供给LTDC,IIC1234,LPTIM12345,ADC,SPDIF                   
#define CLK_PLL3_R_PARAM                                5

#endif



