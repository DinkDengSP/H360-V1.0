/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-11 14:41:20 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:20:58 +0800
************************************************************************************************/ 
#ifndef __MCU_TIMER_DRIVER_CONFIG__
#define __MCU_TIMER_DRIVER_CONFIG__
#include "MCU_ClockDriverConfig.h"

/*-----------------------------------------------------------------定时器时钟配置--------------------------------------------*/
#define TIM_CLK_APB1_HZ                                     (APB1_CLOCK_FREQ*2)
#define TIM_CLK_APB2_HZ                                     (APB2_CLOCK_FREQ*2)
#define TIM_CLK_APB4_HZ                                     (APB4_CLOCK_FREQ*2)
//各个定时器的时钟源设定
//挂载在APB2上的定时器
#define TIM1_CLK_SRC_HZ                                     TIM_CLK_APB2_HZ
#define TIM8_CLK_SRC_HZ                                     TIM_CLK_APB2_HZ
#define TIM15_CLK_SRC_HZ                                    TIM_CLK_APB2_HZ
#define TIM16_CLK_SRC_HZ                                    TIM_CLK_APB2_HZ
#define TIM17_CLK_SRC_HZ                                    TIM_CLK_APB2_HZ
#define HRTIM_CLK_SRC_HZ                                    TIM_CLK_APB2_HZ
//挂载在APB1上的定时器
#define TIM2_CLK_SRC_HZ                                     TIM_CLK_APB1_HZ
#define TIM3_CLK_SRC_HZ                                     TIM_CLK_APB1_HZ
#define TIM4_CLK_SRC_HZ                                     TIM_CLK_APB1_HZ
#define TIM5_CLK_SRC_HZ                                     TIM_CLK_APB1_HZ
#define TIM6_CLK_SRC_HZ                                     TIM_CLK_APB1_HZ
#define TIM7_CLK_SRC_HZ                                     TIM_CLK_APB1_HZ
#define TIM12_CLK_SRC_HZ                                    TIM_CLK_APB1_HZ
#define TIM13_CLK_SRC_HZ                                    TIM_CLK_APB1_HZ
#define TIM14_CLK_SRC_HZ                                    TIM_CLK_APB1_HZ
#define LPTIM1_CLK_SRC_HZ                                   TIM_CLK_APB1_HZ
//挂载在APB4上的定时器
#define LPTIM2_CLK_SRC_HZ                                   TIM_CLK_APB4_HZ
#define LPTIM3_CLK_SRC_HZ                                   TIM_CLK_APB4_HZ
#define LPTIM4_CLK_SRC_HZ                                   TIM_CLK_APB4_HZ
#define LPTIM5_CLK_SRC_HZ                                   TIM_CLK_APB4_HZ

/*------------------------------------------芯片基础定时器6------------------------------------*/
//基础定时功能
#define MCU_TIMER6_MODE_BASE                                0
//输出触发DAC功能
#define MCU_TIMER6_MODE_DAC                                 1

//选择定时器使用模式,TIMER6同时用于DAC下溢中断和TIMER6中断,在DAC模式下,不会定义中断回调
#define MCU_TIMER6_MODE_USER                                MCU_TIMER6_MODE_BASE
//是否执行TIMER6测试用例
#define MCU_TIMER6_TEST_CASE_USE                            0
//是否打印定时器中断讯息
#define MCU_TIMER6_INT_MSH_SHOW                             0
//如果系统延时用的定时器,那么就不能使用测试用例
#if((CORE_DELAY_MODE_USER == CORE_DELAY_MODE_TIMER6)&&(MCU_TIMER6_TEST_CASE_USE != 0))
#error "MCU_Timer6 Use For SysDelay,Should Not Define MCU_TIMER7_TEST_CASE_USE"
#endif
//如果系统延时用的定时器,那么就必须是基础定时模式
#if((CORE_DELAY_MODE_USER == CORE_DELAY_MODE_TIMER6)&&(MCU_TIMER6_MODE_USER != MCU_TIMER6_MODE_BASE))
#error "MCU_Timer6 Use For SysDelay,Should Define MCU_TIMER6_MODE_USER To MCU_TIMER6_MODE_BASE"
#endif

/*------------------------------------------芯片基础定时器7------------------------------------*/
//基础定时功能
#define MCU_TIMER7_MODE_BASE                                0
//输出触发DAC功能
#define MCU_TIMER7_MODE_DAC                                 1

//选择定时器使用模式,TIMER6同时用于DAC下溢中断和TIMER6中断,在DAC模式下,不会定义中断回调
#define MCU_TIMER7_MODE_USER                                MCU_TIMER7_MODE_BASE
//是否执行TIMER6测试用例
#define MCU_TIMER7_TEST_CASE_USE                            0
//是否打印定时器中断讯息
#define MCU_TIMER7_INT_MSH_SHOW                             0
//如果系统延时用的定时器,那么就不能使用测试用例
#if((CORE_DELAY_MODE_USER == CORE_DELAY_MODE_TIMER7)&&(MCU_TIMER7_TEST_CASE_USE != 0))
#error "MCU_Timer7 Use For SysDelay,Should Not Define MCU_TIMER7_TEST_CASE_USE"
#endif
//如果系统延时用的定时器,那么就必须是基础定时模式
#if((CORE_DELAY_MODE_USER == CORE_DELAY_MODE_TIMER7)&&(MCU_TIMER7_MODE_USER != MCU_TIMER7_MODE_BASE))
#error "MCU_Timer7 Use For SysDelay,Should Define MCU_TIMER7_MODE_USER To MCU_TIMER7_MODE_BASE"
#endif


#endif



