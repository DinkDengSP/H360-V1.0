/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:05:31 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 20:29:08 +0800
************************************************************************************************/ 
#ifndef __MCU_IIC2_H_
#define __MCU_IIC2_H_
#include "MCU_IIC_Universal.h"

//IIC总线端口定义
#define PIN_MCU_IIC2_SCL                MCU_PIN_H_4
#define PIN_MCU_IIC2_SDA                MCU_PIN_H_5
/*------------------------------------模拟IIC模式------------------------------------------------*/
#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_SIM)
//快速函数
#define IIC2_SCL_OUT_0()                MCU_PinWriteSingle(PIN_MCU_IIC2_SCL,GPIO_PIN_RESET)
#define IIC2_SCL_OUT_1()                MCU_PinWriteSingle(PIN_MCU_IIC2_SCL,GPIO_PIN_SET)
#define IIC2_SDA_OUT_0()                MCU_PinWriteSingle(PIN_MCU_IIC2_SDA,GPIO_PIN_RESET)
#define IIC2_SDA_OUT_1()                MCU_PinWriteSingle(PIN_MCU_IIC2_SDA,GPIO_PIN_SET)
#define IIC2_SDA_READ()                 MCU_PinReadSingle(PIN_MCU_IIC2_SDA)
#endif

/*------------------------------------中断IIC模式------------------------------------------------*/
#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_INT)
//错误中断侦测回调方法
void MCU_IIC2_ErrorCallbackInt(void);
//取消操作完成回调方法
void MCU_IIC2_AbortCpltCallbackInt(void);
//发送完成回调回调方法
void MCU_IIC2_MemTxCpltCallbackInt(void);
//接收完成回调回调方法
void MCU_IIC2_MemRxCpltCallbackInt(void);
//发送完成回调回调方法
void MCU_IIC2_MasterTxCpltCallbackInt(void);
//接收完成回调回调方法
void MCU_IIC2_MasterRxCpltCallbackInt(void);
#endif

/*------------------------------------DMA IIC模式------------------------------------------------*/
#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_DMA)
//错误中断侦测回调方法
void MCU_IIC2_ErrorCallbackDMA(void);
//取消操作完成回调方法
void MCU_IIC2_AbortCpltCallbackDMA(void);
//发送完成回调回调方法
void MCU_IIC2_MemTxCpltCallbackDMA(void);
//接收完成回调回调方法
void MCU_IIC2_MemRxCpltCallbackDMA(void);
//发送完成回调回调方法
void MCU_IIC2_MasterTxCpltCallbackDMA(void);
//接收完成回调回调方法
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

