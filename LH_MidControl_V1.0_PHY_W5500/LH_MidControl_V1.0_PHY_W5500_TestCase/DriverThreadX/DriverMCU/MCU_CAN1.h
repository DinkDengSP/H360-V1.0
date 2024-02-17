/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:03:04 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-25 13:35:22 +0800
************************************************************************************************/ 
#ifndef __MCU_CAN1_H_
#define __MCU_CAN1_H_
#include "MCU_CAN_Universal.h"

//配置CAN TX引脚
#define MCU_CAN1_TX_PIN                         MCU_PIN_A_12
//配置CAN RX引脚
#define MCU_CAN1_RX_PIN                         MCU_PIN_A_11
//状态监控引脚
#define MCU_CAN1_VSEN_PIN                       MCU_PIN_B_7

//发送完成回调
void MCU_CAN1_TxCompleteCallBack(uint32_t BufferIndexes);
//FIFO0接收回调
void MCU_CAN1_RxFifo0CallBack(uint32_t RxFifo0ITs);
//FIFO1接收回调
void MCU_CAN1_RxFifo1CallBack(uint32_t RxFifo1ITs);
//错误状态回调
void MCU_CAN1_ErrorStatusCallBack(uint32_t ErrorStatusITs);
//错误回调
void MCU_CAN1_ErrorCallBack(void);


//获取CAN互斥锁
void MCU_CAN1_GetLock(void);

//释放CAN1互斥锁
void MCU_CAN1_ReleaseLock(void);

#if(MCU_CAN1_MODE_USER == MCU_CAN_MODE_FD_BRS)
//初始化,数据可变波特率FS CAN
void MCU_CAN1_Init(MCU_CAN_ARBITRATION_BPS arbBps,MCU_CAN_DATA_BPS dataBps,MCU_CAN_RecvPackProcFunc recvCallBackPtr);
#else
//初始化,经典CAN和FD CAN
void MCU_CAN1_Init(MCU_CAN_ARBITRATION_BPS arbBps,MCU_CAN_RecvPackProcFunc recvCallBackPtr);
#endif

//将接收到并处理完成的数据放回驱动空闲队列
void MCU_CAN1_RecvDatRelease(CAN_RECV_PACK* recvDataPtr);

//CAN总线发送数据
ERROR_SUB MCU_CAN1_SendMsg(uint32_t id,CAN_ID_TYPE idType,uint8_t dataLen,uint8_t* dataBuf);

//获取CAN1的各项信息
MCU_CAN_INFO* MCU_CAN1_GetInfo(void);

#endif

