/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:32:19 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 15:25:12 +0800
************************************************************************************************/ 
#ifndef __MCU_IIC2_H_
#define __MCU_IIC2_H_
#include "MCU_IIC_Universal.h"

//IIC总线端口定义
#define PIN_MCU_IIC2_SCL                MCU_PIN_H_4
#define PIN_MCU_IIC2_SDA                MCU_PIN_H_5


//用户选择的IIC模式
#define MCU_IIC2_MODE_USER              MCU_IIC_MODE_DMA
//是否显示IIC通讯时间讯息
#define TIME_MEASURE_MCU_IIC2           0
//失败之后的重试最大次数
#define RETRY_MAX_MCU_IIC2              10
//是否显示IIC中断讯息
#define MCU_IIC2_SHOW_INT_MSG           0

//IIC分频系数 0X00-0X0F
#define MCU_IIC2_PRESCALER              0x05
//IIC SCL数据建立时间 SCLDEL 0X00-0X0F
#define MCU_IIC2_SETUP_TIME             0X03
//IIC SCL数据保持时间 SDADEL 0X00-0X0F
#define MCU_IIC2_HOLD_TIME              0X03
//IIC SCL高电平周期 0X00-0XFF
#define MCU_IIC2_SCLH_PERIOD            0X03
//IIC SCL低电平周期 0X00-0XFF
#define MCU_IIC2_SCLL_PERIOD            0X09
//IIC时序,决定IIC通讯速度
//#define MCU_IIC2_TIMING (48/(11+1))                
#define MCU_IIC2_TIMING                  __LL_I2C_CONVERT_TIMINGS(MCU_IIC2_PRESCALER,MCU_IIC2_SETUP_TIME,MCU_IIC2_HOLD_TIME,MCU_IIC2_SCLH_PERIOD,MCU_IIC2_SCLL_PERIOD)



/*------------------------------------模拟IIC模式------------------------------------------------*/
#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_SIM)
//快速函数
#define IIC2_SCL_OUT_0()     MCU_PortWriteSingle(PIN_MCU_IIC2_SCL,GPIO_PIN_RESET)
#define IIC2_SCL_OUT_1()     MCU_PortWriteSingle(PIN_MCU_IIC2_SCL,GPIO_PIN_SET)
#define IIC2_SDA_OUT_0()     MCU_PortWriteSingle(PIN_MCU_IIC2_SDA,GPIO_PIN_RESET)
#define IIC2_SDA_OUT_1()     MCU_PortWriteSingle(PIN_MCU_IIC2_SDA,GPIO_PIN_SET)
#define IIC2_SDA_READ()      MCU_PortReadSingle(PIN_MCU_IIC2_SDA)

#endif

#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_INT)
//错误中断侦测
void MCU_IIC2_ErrorCallbackInt(void);
//取消操作完成
void MCU_IIC2_AbortCpltCallbackInt(void);
//发送完成回调
void MCU_IIC2_MemTxCpltCallbackInt(void);
//接收完成回调
void MCU_IIC2_MemRxCpltCallbackInt(void);
//发送完成回调
void MCU_IIC2_MasterTxCpltCallbackInt(void);
//接收完成回调
void MCU_IIC2_MasterRxCpltCallbackInt(void);
#endif

#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_DMA)
//DMA模式下缓冲区长度
#define IIC2_BUFFER_LENGTH_DMA_MODE     64
//DMA各项配置映射
#define IIC2_DMA_TX_CLK_ENABLE()        __HAL_RCC_DMA2_CLK_ENABLE()
#define IIC2_DMA_TX_INSTANCE            DMA2_Stream0
#define IIC2_DMA_TX_PRIORITY            DMA_PRIORITY_HIGH
#define IIC2_DMA_TX_IRQ                 DMA2_Stream0_IRQn
#define IIC2_DMA_TX_IRQ_HANDLER         DMA2_Stream0_IRQHandler

#define IIC2_DMA_RX_CLK_ENABLE()        __HAL_RCC_DMA2_CLK_ENABLE()
#define IIC2_DMA_RX_INSTANCE            DMA2_Stream1
#define IIC2_DMA_RX_PRIORITY            DMA_PRIORITY_HIGH
#define IIC2_DMA_RX_IRQ                 DMA2_Stream1_IRQn
#define IIC2_DMA_RX_IRQ_HANDLER         DMA2_Stream1_IRQHandler

//错误中断侦测
void MCU_IIC2_ErrorCallbackDMA(void);
//取消操作完成
void MCU_IIC2_AbortCpltCallbackDMA(void);
//发送完成回调
void MCU_IIC2_MemTxCpltCallbackDMA(void);
//接收完成回调
void MCU_IIC2_MemRxCpltCallbackDMA(void);
//发送完成回调
void MCU_IIC2_MasterTxCpltCallbackDMA(void);
//接收完成回调
void MCU_IIC2_MasterRxCpltCallbackDMA(void);
#endif



//加锁
void MCU_IIC2_GetLock(void);

//释放锁
void MCU_IIC2_ReleaseLock(void);

//IIC端口初始化
void MCU_IIC2_PortInit(void);

//检查设备是否准备好
HAL_StatusTypeDef MCU_IIC2_CheckDeviceReady(uint8_t deviceAddr);

//数据写入
HAL_StatusTypeDef MCU_IIC2_WriteBytesWithAddr(uint8_t deviceAddr,IIC_SUB_ADDR_SIZE subAddrSize,uint16_t memAddr,
                                                    uint8_t* dataBuffer,uint8_t dataLength,uint8_t singleByteTimeOut);

//数据读取
HAL_StatusTypeDef MCU_IIC2_ReadBytesWithAddr(uint8_t deviceAddr,IIC_SUB_ADDR_SIZE subAddrSize,uint16_t memAddr,
                                                    uint8_t* dataBuffer,uint8_t dataLength,uint8_t singleByteTimeOut);

//数据写入
HAL_StatusTypeDef MCU_IIC2_WriteBytes(uint8_t deviceAddr,uint8_t* dataBuffer,uint8_t dataLength,
                                            uint8_t singleByteTimeOut);

//数据读取
HAL_StatusTypeDef MCU_IIC2_ReadBytes(uint8_t deviceAddr,uint8_t* dataBuffer,uint8_t dataLength,
                                            uint8_t singleByteTimeOut);

#endif

