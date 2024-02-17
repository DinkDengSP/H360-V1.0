/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-10-31 20:03:15 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-10-31 20:06:31 +0800
************************************************************************************************/ 
#include "ADT_CircularPtrQueue.h"
#include "ADT_Config.h"

//循环队列初始化
ADT_RESULT ADT_CircularPtrQueueInit(ADT_CIRCULAR_PTR_QUEUE* circularPtrQueuePtr,uint16_t bufferSize)
{
    //首先检测各个参数是否为空
    if(circularPtrQueuePtr == NULL)
        return ADT_RESULT_FAIL;
    if(bufferSize == 0)
        return ADT_RESULT_FAIL;
    //内存申请
    circularPtrQueuePtr->queueArrayPtr = (void**)ADT_CIRCULAR_PTR_QUEUE_MEM_MALLOC(bufferSize*sizeof(void*));
    circularPtrQueuePtr->frontPos = 0;
    circularPtrQueuePtr->rearPos = 0;
    circularPtrQueuePtr->maxSize = bufferSize;
    circularPtrQueuePtr->currentSize = 0;
    if(circularPtrQueuePtr->queueArrayPtr == NULL)
    {
        return ADT_RESULT_FAIL;
    }
    return ADT_RESULT_OK;
}

//获取队列最大容量
uint16_t ADT_CircularPtrQueueGetCapacity(ADT_CIRCULAR_PTR_QUEUE* circularPtrQueuePtr)
{
    return circularPtrQueuePtr->maxSize;
}

//获取队列当前总有效数据量
uint16_t ADT_CircularPtrQueueGetCurrentSize(ADT_CIRCULAR_PTR_QUEUE* circularPtrQueuePtr)
{
    return circularPtrQueuePtr->currentSize;
}

//往循环队列中写入数据
ADT_RESULT ADT_CircularPtrQueueWrite(ADT_CIRCULAR_PTR_QUEUE* circularPtrQueuePtr,void* ptr)
{
    int lastState;
    if(circularPtrQueuePtr == NULL)
        return ADT_RESULT_FAIL;
    if(ptr == NULL)
        return ADT_RESULT_FAIL;
    ADT_CIRCULAR_PTR_QUEUE_QUEUE_LOCK(lastState);
    if(circularPtrQueuePtr->maxSize == circularPtrQueuePtr->currentSize)
    {
        //当前数据已满.不接收
        ADT_CIRCULAR_PTR_QUEUE_QUEUE_UNLOCK(lastState);
        return ADT_RESULT_FAIL;
    }
    else
    {
        //将数据写入尾部
        circularPtrQueuePtr->queueArrayPtr[circularPtrQueuePtr->rearPos++] = ptr;
        circularPtrQueuePtr->rearPos %= circularPtrQueuePtr->maxSize;
        circularPtrQueuePtr->currentSize++;
        ADT_CIRCULAR_PTR_QUEUE_QUEUE_UNLOCK(lastState);
        return ADT_RESULT_OK;
    }
}

//从循环队列中读取数据
void* ADT_CircularPtrQueueGet(ADT_CIRCULAR_PTR_QUEUE* circularPtrQueuePtr)
{
    int lastState;
    void* readPtr = NULL;
    if(circularPtrQueuePtr == NULL)
    {
        return NULL;
    }
    ADT_CIRCULAR_PTR_QUEUE_QUEUE_LOCK(lastState);
    if(0 == circularPtrQueuePtr->currentSize)
    {
        //当前数据为空
        ADT_CIRCULAR_PTR_QUEUE_QUEUE_UNLOCK(lastState);
        return NULL;
    }
    else
    {
        //从头部读取数据
        readPtr = circularPtrQueuePtr->queueArrayPtr[circularPtrQueuePtr->frontPos++];
        circularPtrQueuePtr->frontPos %= circularPtrQueuePtr->maxSize;
        circularPtrQueuePtr->currentSize--;
        ADT_CIRCULAR_PTR_QUEUE_QUEUE_UNLOCK(lastState);
        return readPtr;
    }
}




