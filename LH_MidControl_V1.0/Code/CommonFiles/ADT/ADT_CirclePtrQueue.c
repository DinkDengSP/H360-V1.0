/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-07-28 14:43:14
**LastEditors: DengXiaoJun
**LastEditTime: 2022-01-24 13:17:29
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ADT_CirclePtrQueue.h"
#include "ADT_Port.h"

//循环队列初始化
ADT_RESULT ADT_CirclePtrQueueInit(ADT_CIRCLE_PTR_QUEUE* circlePtrQueuePtr,uint16_t bufferSize)
{
    //首先检测各个参数是否为空
    if(circlePtrQueuePtr == NULL)
        return ADT_RESULT_FAIL;
    if(bufferSize == 0)
        return ADT_RESULT_FAIL;
    //内存申请
    circlePtrQueuePtr->queueArrayPtr = ADT_MEM_MALLOC(bufferSize*sizeof(void*));
    circlePtrQueuePtr->frontPos = 0;
    circlePtrQueuePtr->rearPos = 0;
    circlePtrQueuePtr->maxSize = bufferSize;
    circlePtrQueuePtr->currentSize = 0;
    if(circlePtrQueuePtr->queueArrayPtr == NULL)
    {
        return ADT_RESULT_FAIL;
    }
    return ADT_RESULT_OK;
}

//获取队列最大容量
uint16_t ADT_CirclePtrQueueGetCapacity(ADT_CIRCLE_PTR_QUEUE* circlePtrQueuePtr)
{
    return circlePtrQueuePtr->maxSize;
}

//获取队列当前总有效数据量
uint16_t ADT_CirclePtrQueueGetCurrentSize(ADT_CIRCLE_PTR_QUEUE* circlePtrQueuePtr)
{
    return circlePtrQueuePtr->currentSize;
}

//往循环队列中写入数据
ADT_RESULT ADT_CirclePtrQueueWrite(ADT_CIRCLE_PTR_QUEUE* circlePtrQueuePtr,void* ptr)
{
    int lastIntState;
    if(circlePtrQueuePtr == NULL)
        return ADT_RESULT_FAIL;
    if(ptr == NULL)
        return ADT_RESULT_FAIL;
    ADT_DISABLE_INT(&lastIntState);
    if(circlePtrQueuePtr->maxSize == circlePtrQueuePtr->currentSize)
    {
        //当前数据已满.不接收
        ADT_ENABLE_INT(lastIntState);
        return ADT_RESULT_FAIL;
    }
    else
    {
        //将数据写入尾部
        circlePtrQueuePtr->queueArrayPtr[circlePtrQueuePtr->rearPos++] = ptr;
        circlePtrQueuePtr->rearPos %= circlePtrQueuePtr->maxSize;
        circlePtrQueuePtr->currentSize++;
        ADT_ENABLE_INT(lastIntState);
        return ADT_RESULT_OK;
    }
}

//从循环队列中读取数据
void* ADT_CirclePtrQueueGet(ADT_CIRCLE_PTR_QUEUE* circlePtrQueuePtr)
{
    int lastIntState;
    void* readPtr = NULL;
    if(circlePtrQueuePtr == NULL)
    {
        return NULL;
    }
    ADT_DISABLE_INT(&lastIntState);
    if(0 == circlePtrQueuePtr->currentSize)
    {
        //当前数据为空
        ADT_ENABLE_INT(lastIntState);
        return NULL;
    }
    else
    {
        //从头部读取数据
        readPtr = circlePtrQueuePtr->queueArrayPtr[circlePtrQueuePtr->frontPos++];
        circlePtrQueuePtr->frontPos %= circlePtrQueuePtr->maxSize;
        circlePtrQueuePtr->currentSize--;
        ADT_ENABLE_INT(lastIntState);
        return readPtr;
    }
}




