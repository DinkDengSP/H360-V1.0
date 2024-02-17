/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-23 14:15:18
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-23 14:29:48
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ADT_CircleCanRxPtrQueue.h"

//循环队列初始化
void ADT_CircleCanRxPtrQueueInit(ADT_CIRCLE_CAN_RX_PTR_QUEUE* circlePtrQueuePtr,MCU_CAN_RECV_DAT** ptrBufferInit,uint16_t bufferSize)
{
    //首先检测各个参数是否为空
    if(circlePtrQueuePtr == NULL)return;
    if(ptrBufferInit == NULL)return;
    if(bufferSize == 0)return;
    ThreadLockInt();
    circlePtrQueuePtr->queueArrayBuffer = ptrBufferInit;
    circlePtrQueuePtr->frontPos = 0;
    circlePtrQueuePtr->rearPos = 0;
    circlePtrQueuePtr->maxSize = bufferSize;
    circlePtrQueuePtr->currentSize = 0;
    ThreadUnlockInt();
}

//往循环队列中写入数据
void ADT_CircleCanRxPtrQueueWrite(ADT_CIRCLE_CAN_RX_PTR_QUEUE* circlePtrQueuePtr,MCU_CAN_RECV_DAT* ptr)
{
    if(circlePtrQueuePtr == NULL)
    {
        return;
    }
    if(ptr == NULL)
    {
        return;
    }
    ThreadLockInt();
    if(circlePtrQueuePtr->maxSize == circlePtrQueuePtr->currentSize)
    {
        //当前数据已满.不接收
        ThreadUnlockInt();
        return;
    }
    else
    {
        circlePtrQueuePtr->queueArrayBuffer[circlePtrQueuePtr->rearPos++] = ptr;
        circlePtrQueuePtr->rearPos %= circlePtrQueuePtr->maxSize;
        circlePtrQueuePtr->currentSize++;
        ThreadUnlockInt();
        return;
    }
}

//从循环队列中读取数据
MCU_CAN_RECV_DAT* ADT_CircleCanRxPtrQueueGet(ADT_CIRCLE_CAN_RX_PTR_QUEUE* circlePtrQueuePtr)
{
    MCU_CAN_RECV_DAT* readPtr = NULL;
    if(circlePtrQueuePtr == NULL)
    {
        return readPtr;
    }
    ThreadLockInt();
    if(0 == circlePtrQueuePtr->currentSize)
    {
        ThreadUnlockInt();
        return readPtr;
    }
    else
    {
        readPtr = circlePtrQueuePtr->queueArrayBuffer[circlePtrQueuePtr->frontPos++];
        circlePtrQueuePtr->frontPos %= circlePtrQueuePtr->maxSize;
        circlePtrQueuePtr->currentSize--;
        ThreadUnlockInt();
        return readPtr;
    }
}

























