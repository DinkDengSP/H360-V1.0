/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-14 13:52:25 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-14 14:49:41 +0800
************************************************************************************************/ 
#ifndef __MCU_SPI2_H_
#define __MCU_SPI2_H_
#include "MCU_SPI_Universal.h"
#include "MCU_Config.h"

//SPI通讯引脚定义
#define MCU_SPI2_PIN_SCK                MCU_PIN_I_1
#define MCU_SPI2_PIN_MISO               MCU_PIN_I_2
#define MCU_SPI2_PIN_MOSI               MCU_PIN_I_3

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


