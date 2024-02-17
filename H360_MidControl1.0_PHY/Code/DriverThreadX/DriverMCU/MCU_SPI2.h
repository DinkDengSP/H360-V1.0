/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:28:34 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 19:13:56 +0800
************************************************************************************************/ 
#ifndef __MCU_SPI2_H_
#define __MCU_SPI2_H_
#include "MCU_SPI_Universal.h"

//选择的SPI通讯模式
#define MCU_SPI2_MODE_USER              MCU_SPI_MODE_DMA
//SPI通讯引脚定义
#define MCU_SPI2_PIN_SCK                MCU_PIN_I_1
#define MCU_SPI2_PIN_MISO               MCU_PIN_I_2
#define MCU_SPI2_PIN_MOSI               MCU_PIN_I_3
//中断模式下,等待中断的时候是否开启操作系统调度,不开启快,开启了慢但是对系统有好处
#define MCU_SPI2_INT_ENABLE_SCHEDULE    1
//取消传输最大超时时间
#define MCU_SPI2_ABORT_WAIT_TIME_MAX    1000
//是否执行中断打印
#define MCU_SPI2_ENABLE_INT_SHOW        0


//SPI2初始化
void MCU_SPI2_Init(MCU_SPI_123_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha);
//SPI2 通讯参数重新设置
void MCU_SPI2_ResetParam(MCU_SPI_123_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha);
//锁定SPI2
void MCU_SPI2_GetLock(void);
//解锁SPI
void MCU_SPI2_ReleaseLock(void);
//SPI2设定传输速度
void MCU_SPI2_SetSpeed(MCU_SPI_123_CLK speed);
//SPI2读写数据
LH_ERR MCU_SPI2_ReadWriteBuffer(uint8_t* writeDataPtr,uint8_t* readDataPtr,uint16_t dataLength);

/*------------------------中断模式下的扩展函数------------------------*/
#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_INT)
//收发完成中断回调函数
void MCU_SPI2_TxRxCpltCallback_Int(void);
//发送中断回调函数
void MCU_SPI2_TxCpltCallback_Int(void);
//接收中断回调函数
void MCU_SPI2_RxCpltCallback_Int(void);
//错误回调函数
void MCU_SPI2_ErrorCallback_Int(void);
//取消传输标记
void MCU_SPI2_AbortCpltCallback_IT(void);
#endif

/*------------------------DMA模式下的扩展函数------------------------*/
#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_DMA)
//DMA收发缓冲区长度
#define LENGTH_BYTES_SPI2_DMA_BUF   512
//DMA各项配置映射
#define SPI2_DMA_TX_CLK_ENABLE()        __HAL_RCC_DMA1_CLK_ENABLE()
#define SPI2_DMA_TX_INSTANCE            DMA1_Stream4
#define SPI2_DMA_TX_PRIORITY            DMA_PRIORITY_MEDIUM
#define SPI2_DMA_TX_IRQ                 DMA1_Stream4_IRQn
#define SPI2_DMA_TX_IRQ_HANDLER         DMA1_Stream4_IRQHandler

#define SPI2_DMA_RX_CLK_ENABLE()        __HAL_RCC_DMA1_CLK_ENABLE()
#define SPI2_DMA_RX_INSTANCE            DMA1_Stream5
#define SPI2_DMA_RX_PRIORITY            DMA_PRIORITY_MEDIUM
#define SPI2_DMA_RX_IRQ                 DMA1_Stream5_IRQn
#define SPI2_DMA_RX_IRQ_HANDLER         DMA1_Stream5_IRQHandler

//收发完成中断回调函数
void MCU_SPI2_TxRxCpltCallback_DMA(void);
//发送中断回调函数
void MCU_SPI2_TxCpltCallback_DMA(void);
//接收中断回调函数
void MCU_SPI2_RxCpltCallback_DMA(void);
//错误回调函数
void MCU_SPI2_ErrorCallback_DMA(void);
//取消传输标记
void MCU_SPI2_AbortCpltCallback_DMA(void);

#endif


#endif


