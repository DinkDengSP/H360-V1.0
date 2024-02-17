/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-14 14:30:42
 *FilePath: \H360ActionBoardSoftwareV2.3\ProjectCollection\H360_ActionBoard_ID_8_Rail\TaskMain\ServiceSupport\ServiceIPC\ServiceIpcSerialUserRS1.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IPC_SERIAL_USER_RS1_H_
#define __SERVICE_IPC_SERIAL_USER_RS1_H_
#include "ServiceIpcBase.h"

//输入任务的数据初始化
void IPC_SerialUserRS1DataInit(void);

//输入任务的上一次执行结果,也就是有没有执行完成
STATE_IPC IPC_SerialUserRS1GetLastCommandStatusAndResult(LH_ERR* resultCode);

//发送数据
LH_ERR IPC_SerialUserRS1SendDataBufferWhileAck(uint8_t* dataBufferPtr,uint16_t dataLength);

//发送数据并延时等待返回
LH_ERR IPC_SerialUserRS1SendDataBufferDelayReadReceiveWhileAck(uint8_t* dataBufferPtr,uint16_t dataLength,uint32_t receiveDelayMs);

//读取当前串口接收到的数据
LH_ERR IPC_SerialUserRS1ReadReceiveWhileAck(void);

//读取上次得到的数据,注意,此处得到的是一个指向指针的指针,传入的是一个指针空值
//如果有数据要发出,函数会自己申请内存,返回结果,所以,应用程序在检测到dataBufferPtr不为空值的时候
//数据使用完成,需要释放内存
uint16_t  IPC_SerialUserRS1LastReceiveData(uint8_t** dataBufferPtr);



#endif





