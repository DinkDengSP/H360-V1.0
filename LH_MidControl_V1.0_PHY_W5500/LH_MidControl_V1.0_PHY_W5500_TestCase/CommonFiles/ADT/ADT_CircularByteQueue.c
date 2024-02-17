/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-10-31 20:02:59 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-10-31 20:05:07 +0800
************************************************************************************************/ 
#include "ADT_CircularByteQueue.h"
#include "ADT_Config.h"

//循环队列初始化
void ADT_CircularByteQueueInit(ADT_CIRCULAR_BYTE_QUEUE* circularByteQueuePtr,uint8_t* bufferStartPtr,uint16_t bufferSize)
{
    int lastState;
    //首先检测各个参数是否为空
    if(circularByteQueuePtr == NULL)
        return;
    if(bufferStartPtr == NULL)
        return;
    if(bufferSize == 0)
        return;
    ADT_CIRCULAR_BYTE_QUEUE_LOCK(lastState);
    circularByteQueuePtr->queueArrayBuffer = bufferStartPtr;
    circularByteQueuePtr->frontPos = 0;
    circularByteQueuePtr->rearPos = 0;
    circularByteQueuePtr->maxSize = bufferSize;
    circularByteQueuePtr->currentSize = 0;
    ADT_CIRCULAR_BYTE_QUEUE_UNLOCK(lastState);
}

//获取循环队列最大容量
uint16_t ADT_CircularByteQueueGetMaxCapacity(ADT_CIRCULAR_BYTE_QUEUE* circularByteQueuePtr)
{
    if(circularByteQueuePtr == 0)
    {
        return 0;
    }
    return circularByteQueuePtr->maxSize;
}

//获取当前循环队列已经存储的数据
uint16_t ADT_CircularByteQueueGetCurrentSize(ADT_CIRCULAR_BYTE_QUEUE* circularByteQueuePtr)
{
    if(circularByteQueuePtr == 0)
    {
        return 0;
    }
    return circularByteQueuePtr->currentSize;
}

//获取当前循环队列中空数据的量
uint16_t ADT_CircularByteQueueGetCurrentEmptySize(ADT_CIRCULAR_BYTE_QUEUE* circularByteQueuePtr)
{
    int lastState;
    if(circularByteQueuePtr == 0)
    {
        return 0;
    }
    uint16_t emptySize = 0;
    ADT_CIRCULAR_BYTE_QUEUE_LOCK(lastState);
    emptySize = circularByteQueuePtr->maxSize - circularByteQueuePtr->currentSize;
    ADT_CIRCULAR_BYTE_QUEUE_UNLOCK(lastState);
    return emptySize;
}

//往循环队列中写入指定数量数据,当空间不足的时候,会将缓冲区数据填满,并返回真实的写入量
void ADT_CircularByteQueueWriteBuffer(ADT_CIRCULAR_BYTE_QUEUE* circularByteQueuePtr,uint8_t* bufferWritePtr,uint16_t length,uint16_t* realWriteCount)
{
    int lastState;
    uint16_t writeCount = 0;
    if(circularByteQueuePtr == NULL)
    {
        *realWriteCount = 0;
        return;
    }
    if(bufferWritePtr == NULL)
    {
        *realWriteCount = 0;
        return;
    }
    ADT_CIRCULAR_BYTE_QUEUE_LOCK(lastState);
    if(circularByteQueuePtr->maxSize == circularByteQueuePtr->currentSize)
    {
        //当前数据已满
        *realWriteCount = 0;
        ADT_CIRCULAR_BYTE_QUEUE_UNLOCK(lastState);
        return;
    }
    else
    {
        while(circularByteQueuePtr->currentSize < circularByteQueuePtr->maxSize)
        {
            circularByteQueuePtr->queueArrayBuffer[circularByteQueuePtr->rearPos++] = bufferWritePtr[writeCount++];
            circularByteQueuePtr->rearPos %= circularByteQueuePtr->maxSize;
            circularByteQueuePtr->currentSize++;
            if(writeCount == length)
            {
                break;
            }
        }
        *realWriteCount = writeCount;
        ADT_CIRCULAR_BYTE_QUEUE_UNLOCK(lastState);
        return;
    }
}

//从循环队列中读取指定数量数据,当数据不足时,已有的数据写到缓冲区,并返回真实读取到的数据
void ADT_CircularByteQueueReadBuffer(ADT_CIRCULAR_BYTE_QUEUE* circularByteQueuePtr,uint8_t* bufferReadPtr,uint16_t length,uint16_t* realReadCount)
{
    int lastState;
    uint16_t readCount = 0;
    if(circularByteQueuePtr == NULL)
    {
        *realReadCount = 0;
        return;
    }
    if(bufferReadPtr == NULL)
    {
        *realReadCount = 0;
        return;
    }
    ADT_CIRCULAR_BYTE_QUEUE_LOCK(lastState);
    if(0 == circularByteQueuePtr->currentSize)
    {
        *realReadCount = 0;
        ADT_CIRCULAR_BYTE_QUEUE_UNLOCK(lastState);
        return;
    }
    else
    {
        while(circularByteQueuePtr->currentSize > 0)
        {
            bufferReadPtr[readCount++] = circularByteQueuePtr->queueArrayBuffer[circularByteQueuePtr->frontPos++];
            circularByteQueuePtr->frontPos %= circularByteQueuePtr->maxSize;
            circularByteQueuePtr->currentSize--;
            if(readCount == length)
            {
                break;
            }
        }
        *realReadCount = readCount;
        ADT_CIRCULAR_BYTE_QUEUE_UNLOCK(lastState);
        return;
    }
}

//清空队列
void ADT_CircularByteQueueClear(ADT_CIRCULAR_BYTE_QUEUE* circularByteQueuePtr)
{
    int lastState;
    //首先检测各个参数是否为空
    if(circularByteQueuePtr == NULL)return;
    ADT_CIRCULAR_BYTE_QUEUE_LOCK(lastState);
    circularByteQueuePtr->frontPos = 0;
    circularByteQueuePtr->rearPos = 0;
    circularByteQueuePtr->currentSize = 0;
    ADT_CIRCULAR_BYTE_QUEUE_UNLOCK(lastState);
}

