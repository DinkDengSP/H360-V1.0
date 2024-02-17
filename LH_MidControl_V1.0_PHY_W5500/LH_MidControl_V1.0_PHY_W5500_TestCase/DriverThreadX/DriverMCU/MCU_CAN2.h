/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:03:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-28 12:50:31 +0800
************************************************************************************************/ 
#ifndef __MCU_CAN2_H_
#define __MCU_CAN2_H_
#include "MCU_CAN_Universal.h"


//配置CAN TX引脚
#define MCU_CAN2_TX_PIN                         MCU_PIN_B_6
//配置CAN RX引脚
#define MCU_CAN2_RX_PIN                         MCU_PIN_B_5
//状态监控引脚
#define MCU_CAN2_VSEN_PIN                       MCU_PIN_B_4

//发送完成回调
void MCU_CAN2_TxCompleteCallBack(uint32_t BufferIndexes);
//FIFO0接收回调
void MCU_CAN2_RxFifo0CallBack(uint32_t RxFifo0ITs);
//FIFO1接收回调
void MCU_CAN2_RxFifo1CallBack(uint32_t RxFifo1ITs);
//错误状态回调
void MCU_CAN2_ErrorStatusCallBack(uint32_t ErrorStatusITs);
//错误回调
void MCU_CAN2_ErrorCallBack(void);


//获取CAN互斥锁
void MCU_CAN2_GetLock(void);

//释放CAN2互斥锁
void MCU_CAN2_ReleaseLock(void);

#if(MCU_CAN2_MODE_USER == MCU_CAN_MODE_FD_BRS)
//初始化,数据可变波特率FS CAN
void MCU_CAN2_Init(MCU_CAN_ARBITRATION_BPS arbBps,MCU_CAN_DATA_BPS dataBps,MCU_CAN_RecvPackProcFunc recvCallBackPtr);
#else
//初始化,经典CAN和FD CAN
void MCU_CAN2_Init(MCU_CAN_ARBITRATION_BPS arbBps,MCU_CAN_RecvPackProcFunc recvCallBackPtr);
#endif

//将接收到并处理完成的数据放回驱动空闲队列
void MCU_CAN2_RecvDatRelease(CAN_RECV_PACK* recvDataPtr);

//CAN总线发送数据
ERROR_SUB MCU_CAN2_SendMsg(uint32_t id,CAN_ID_TYPE idType,uint8_t dataLen,uint8_t* dataBuf);

//获取CAN2的各项信息
MCU_CAN_INFO* MCU_CAN2_GetInfo(void);

#endif

