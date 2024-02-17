/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-11 14:38:41 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-25 11:33:39 +0800
************************************************************************************************/ 
#ifndef __MCU_IIC_DRIVER_CONFIG__
#define __MCU_IIC_DRIVER_CONFIG__
#include "stm32h7xx.h"


/*-----------------------------------------------------------------芯片IIC----------------------------------------------------*/
//不同的MCU IIC模式
#define MCU_IIC_MODE_SIM                                    0X00//模拟IIC
#define MCU_IIC_MODE_POOL                                   0X01//查询IIC
#define MCU_IIC_MODE_INT                                    0X02//中断IIC
#define MCU_IIC_MODE_DMA                                    0X03//DMA IIC

/*-----------------------------------------------------------------IIC2------------------------------------------------------*/
//用户选择的IIC模式
#define MCU_IIC2_MODE_USER                                  MCU_IIC_MODE_DMA
//是否显示IIC通讯时间讯息       
#define TIME_MEASURE_MCU_IIC2                               0
//IIC等待设备ready的超时时间
#define IIC2_TIME_OUT_MS                                    50
//失败之后的重试最大次数        
#define RETRY_MAX_MCU_IIC2                                  10
//是否显示IIC中断讯息       
#define MCU_IIC2_SHOW_INT_MSG                               0

//IIC分频系数 0X00-0X0F
#define MCU_IIC2_PRESCALER                                  0x05
//IIC SCL数据建立时间 SCLDEL 0X00-0X0                   
#define MCU_IIC2_SETUP_TIME                                 0X03
//IIC SCL数据保持时间 SDADEL 0X00-0X0                   
#define MCU_IIC2_HOLD_TIME                                  0X03
//IIC SCL高电平周期 0X00-0XFF                   
#define MCU_IIC2_SCLH_PERIOD                                0X03
//IIC SCL低电平周期 0X00-0XFF                   
#define MCU_IIC2_SCLL_PERIOD                                0X09
//IIC时序,决定IIC通讯速度,以上使用H7中文手册1817页,时钟48MHZ下,400KHZ的通讯速率                
#define MCU_IIC2_TIMING                                     __LL_I2C_CONVERT_TIMINGS(MCU_IIC2_PRESCALER,MCU_IIC2_SETUP_TIME,MCU_IIC2_HOLD_TIME,MCU_IIC2_SCLH_PERIOD,MCU_IIC2_SCLL_PERIOD)

//DMA模式下缓冲区长度
#define IIC2_BUFFER_LENGTH_DMA_MODE                         512
//DMA各项配置映射
#define IIC2_DMA_TX_CLK_ENABLE()                            __HAL_RCC_DMA1_CLK_ENABLE()
#define IIC2_DMA_TX_INSTANCE                                DMA1_Stream4
#define IIC2_DMA_TX_PRIORITY                                DMA_PRIORITY_LOW
#define IIC2_DMA_TX_IRQ                                     DMA1_Stream4_IRQn
#define IIC2_DMA_TX_IRQ_HANDLER                             DMA1_Stream4_IRQHandler

#define IIC2_DMA_RX_CLK_ENABLE()                            __HAL_RCC_DMA1_CLK_ENABLE()
#define IIC2_DMA_RX_INSTANCE                                DMA1_Stream5
#define IIC2_DMA_RX_PRIORITY                                DMA_PRIORITY_MEDIUM
#define IIC2_DMA_RX_IRQ                                     DMA1_Stream5_IRQn
#define IIC2_DMA_RX_IRQ_HANDLER                             DMA1_Stream5_IRQHandler



#endif




