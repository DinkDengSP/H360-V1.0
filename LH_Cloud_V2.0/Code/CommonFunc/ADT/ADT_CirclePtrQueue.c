/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-05-11 11:56:50
**LastEditors: DengXiaoJun
**LastEditTime: 2020-10-26 15:46:33
**FilePath: \H360_ActionBoard_ID_3d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\SystemUtil\ADT_CirclePtrQueue.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ADT_CirclePtrQueue.h"
#include "os.h"
#include "CoreUtil.h"

//循环队列初始化
void ADT_CirclePtrQueueInit(ADT_CIRCLE_PTR_QUEUE* circlePtrQueuePtr,CanRxMsg** ptrBufferInit,uint16_t bufferSize)
{
    //首先检测各个参数是否为空
    if(circlePtrQueuePtr == NULL)return;
    if(ptrBufferInit == NULL)return;
    if(bufferSize == 0)return;
    ADT_CIRCLE_BYTE_PTR_LOCK_THREAD();
    circlePtrQueuePtr->queueArrayBuffer = ptrBufferInit;
    circlePtrQueuePtr->frontPos = 0;
    circlePtrQueuePtr->rearPos = 0;
    circlePtrQueuePtr->maxSize = bufferSize;
    circlePtrQueuePtr->currentSize = 0;
    ADT_CIRCLE_BYTE_PTR_UNLOCK_THREAD();
}

//往循环队列中写入数据
void ADT_CirclePtrQueueWrite(ADT_CIRCLE_PTR_QUEUE* circlePtrQueuePtr,CanRxMsg* ptr)
{
    ADT_CIRCLE_BYTE_PTR_LOCK_THREAD();
    if(circlePtrQueuePtr == NULL)
    {
        ADT_CIRCLE_BYTE_PTR_UNLOCK_THREAD();
        return;
    }
    if(ptr == NULL)
    {
        ADT_CIRCLE_BYTE_PTR_UNLOCK_THREAD();
        return;
    }
    if(circlePtrQueuePtr->maxSize == circlePtrQueuePtr->currentSize)
    {
        //当前数据已满.不接收
        ADT_CIRCLE_BYTE_PTR_UNLOCK_THREAD();
        return;
    }
    else
    {
        circlePtrQueuePtr->queueArrayBuffer[circlePtrQueuePtr->rearPos++] = ptr;
        circlePtrQueuePtr->rearPos %= circlePtrQueuePtr->maxSize;
        circlePtrQueuePtr->currentSize++;
        ADT_CIRCLE_BYTE_PTR_UNLOCK_THREAD();
        return;
    }
}

//从循环队列中读取数据
CanRxMsg* ADT_CirclePtrQueueGet(ADT_CIRCLE_PTR_QUEUE* circlePtrQueuePtr)
{
    CanRxMsg* readPtr = NULL;
    ADT_CIRCLE_BYTE_PTR_LOCK_THREAD();
    if(circlePtrQueuePtr == NULL)
    {
        ADT_CIRCLE_BYTE_PTR_UNLOCK_THREAD();
        return readPtr;
    }
    if(0 == circlePtrQueuePtr->currentSize)
    {
        ADT_CIRCLE_BYTE_PTR_UNLOCK_THREAD();
        return readPtr;
    }
    else
    {
        readPtr = circlePtrQueuePtr->queueArrayBuffer[circlePtrQueuePtr->frontPos++];
        circlePtrQueuePtr->frontPos %= circlePtrQueuePtr->maxSize;
        circlePtrQueuePtr->currentSize--;
        ADT_CIRCLE_BYTE_PTR_UNLOCK_THREAD();

        return readPtr;
    }
}













