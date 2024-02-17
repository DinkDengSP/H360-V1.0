/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-11-17 10:44:57
**LastEditors: DengXiaoJun
**LastEditTime: 2020-11-17 13:16:19
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ADT_CircleCanRxPtrQueue.h"

//循环队列初始化
void ADT_CircleCanRxPtrQueueInit(ADT_CIRCLE_CAN_RX_PTR_QUEUE* circlePtrQueuePtr,CanRxMsg** ptrBufferInit,uint16_t bufferSize)
{
    //首先检测各个参数是否为空
    if(circlePtrQueuePtr == NULL)return;
    if(ptrBufferInit == NULL)return;
    if(bufferSize == 0)return;
    ADT_CIRCLE_CAN_RX_PTR_LOCK_THREAD();
    circlePtrQueuePtr->queueArrayBuffer = ptrBufferInit;
    circlePtrQueuePtr->frontPos = 0;
    circlePtrQueuePtr->rearPos = 0;
    circlePtrQueuePtr->maxSize = bufferSize;
    circlePtrQueuePtr->currentSize = 0;
    ADT_CIRCLE_CAN_RX_PTR_UNLOCK_THREAD();
}

//往循环队列中写入数据
void ADT_CircleCanRxPtrQueueWrite(ADT_CIRCLE_CAN_RX_PTR_QUEUE* circlePtrQueuePtr,CanRxMsg* ptr)
{
    if(circlePtrQueuePtr == NULL)
    {
        return;
    }
    if(ptr == NULL)
    {
        return;
    }
    ADT_CIRCLE_CAN_RX_PTR_LOCK_THREAD();
    if(circlePtrQueuePtr->maxSize == circlePtrQueuePtr->currentSize)
    {
        //当前数据已满.不接收
        ADT_CIRCLE_CAN_RX_PTR_UNLOCK_THREAD();
        return;
    }
    else
    {
        circlePtrQueuePtr->queueArrayBuffer[circlePtrQueuePtr->rearPos++] = ptr;
        circlePtrQueuePtr->rearPos %= circlePtrQueuePtr->maxSize;
        circlePtrQueuePtr->currentSize++;
        ADT_CIRCLE_CAN_RX_PTR_UNLOCK_THREAD();
        return;
    }
}

//从循环队列中读取数据
CanRxMsg* ADT_CircleCanRxPtrQueueGet(ADT_CIRCLE_CAN_RX_PTR_QUEUE* circlePtrQueuePtr)
{
    CanRxMsg* readPtr = NULL;
    if(circlePtrQueuePtr == NULL)
    {
        return readPtr;
    }
    ADT_CIRCLE_CAN_RX_PTR_LOCK_THREAD();
    if(0 == circlePtrQueuePtr->currentSize)
    {
        ADT_CIRCLE_CAN_RX_PTR_UNLOCK_THREAD();
        return readPtr;
    }
    else
    {
        readPtr = circlePtrQueuePtr->queueArrayBuffer[circlePtrQueuePtr->frontPos++];
        circlePtrQueuePtr->frontPos %= circlePtrQueuePtr->maxSize;
        circlePtrQueuePtr->currentSize--;
        ADT_CIRCLE_CAN_RX_PTR_UNLOCK_THREAD();

        return readPtr;
    }
}












