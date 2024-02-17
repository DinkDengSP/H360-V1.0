/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-14 13:54:47 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-14 13:59:14 +0800
************************************************************************************************/ 
#ifndef __MCU_CONFIG_H_
#define __MCU_CONFIG_H_
#include "stm32h7xx_hal.h"
#include "SEGGER_RTT.h"

/*------------------------------------------芯片SPI模式------------------------------------*/
//查询模式
#define MCU_SPI_MODE_POOL                           0
//中断模式
#define MCU_SPI_MODE_INT                            1
//DMA模式
#define MCU_SPI_MODE_DMA                            2


/*------------------------------------------SPI2------------------------------------*/
//选择的SPI通讯模式
#define MCU_SPI2_MODE_USER                          MCU_SPI_MODE_DMA
//中断模式下,等待中断的时候是否开启操作系统调度,不开启快,开启了慢但是对系统有好处
#define MCU_SPI2_INT_ENABLE_SCHEDULE                1
//取消传输最大超时时间
#define MCU_SPI2_ABORT_WAIT_TIME_MAX                1000
//是否执行中断打印
#define MCU_SPI2_ENABLE_INT_SHOW                    0

//DMA收发缓冲区长度
#define LENGTH_BYTES_SPI2_DMA_BUF                   4096
//DMA各项配置映射
#define SPI2_DMA_TX_CLK_ENABLE()                    __HAL_RCC_DMA2_CLK_ENABLE()
#define SPI2_DMA_TX_INSTANCE                        DMA2_Stream0
#define SPI2_DMA_TX_PRIORITY                        DMA_PRIORITY_MEDIUM
#define SPI2_DMA_TX_IRQ                             DMA2_Stream0_IRQn
#define SPI2_DMA_TX_IRQ_HANDLER                     DMA2_Stream0_IRQHandler

#define SPI2_DMA_RX_CLK_ENABLE()                    __HAL_RCC_DMA2_CLK_ENABLE()
#define SPI2_DMA_RX_INSTANCE                        DMA2_Stream1
#define SPI2_DMA_RX_PRIORITY                        DMA_PRIORITY_MEDIUM
#define SPI2_DMA_RX_IRQ                             DMA2_Stream1_IRQn
#define SPI2_DMA_RX_IRQ_HANDLER                     DMA2_Stream1_IRQHandler

/*------------------------------------------SPI5------------------------------------*/
//选择的SPI通讯模式
#define MCU_SPI5_MODE_USER                          MCU_SPI_MODE_DMA
//中断模式下,等待中断的时候是否开启操作系统调度,不开启快,开启了慢但是对系统有好处
#define MCU_SPI5_INT_ENABLE_SCHEDULE                1
//取消传输最大超时时间
#define MCU_SPI5_ABORT_WAIT_TIME_MAX                1000
//是否执行中断打印
#define MCU_SPI5_ENABLE_INT_SHOW                    0

//DMA收发缓冲区长度
#define LENGTH_BYTES_SPI5_DMA_BUF                   4096
//DMA各项配置映射
#define SPI5_DMA_TX_CLK_ENABLE()                    __HAL_RCC_DMA2_CLK_ENABLE()
#define SPI5_DMA_TX_INSTANCE                        DMA2_Stream2
#define SPI5_DMA_TX_PRIORITY                        DMA_PRIORITY_MEDIUM
#define SPI5_DMA_TX_IRQ                             DMA2_Stream2_IRQn
#define SPI5_DMA_TX_IRQ_HANDLER                     DMA2_Stream2_IRQHandler

#define SPI5_DMA_RX_CLK_ENABLE()                    __HAL_RCC_DMA2_CLK_ENABLE()
#define SPI5_DMA_RX_INSTANCE                        DMA2_Stream3
#define SPI5_DMA_RX_PRIORITY                        DMA_PRIORITY_MEDIUM
#define SPI5_DMA_RX_IRQ                             DMA2_Stream3_IRQn
#define SPI5_DMA_RX_IRQ_HANDLER                     DMA2_Stream3_IRQHandler

#endif



