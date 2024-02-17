/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-11 18:49:59
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-11 19:24:35
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ADT_ByteFifo.h"
#include "ADT_Port.h"

//队列初始化
void ADT_ByteFifoInit(ADT_BYTE_FIFO* byteFifoPtr,uint8_t* bufferStartPtr,uint16_t bufferSize)
{
    //首先检测各个参数是否为空
    if(byteFifoPtr == NULL)return;
    if(bufferStartPtr == NULL)return;
    if(bufferSize == 0)return;
    byteFifoPtr->queueArrayBuffer = bufferStartPtr;
    byteFifoPtr->frontPos = 0;
    byteFifoPtr->rearPos = 0;
    byteFifoPtr->maxSize = bufferSize;
    byteFifoPtr->currentSize = 0;
}

//获取当前队列的元素数量
uint16_t ADT_ByteFifoGetCurrentSize(ADT_BYTE_FIFO* byteFifoPtr)
{
    if(byteFifoPtr == NULL)
    {
        return 0;
    }
    return byteFifoPtr->currentSize;
}

//检查数据是否已经在队列中存在,如果存在,返回0,不存在,返回1
static uint8_t ADT_ByteFifoCheckExist(ADT_BYTE_FIFO* byteFifoPtr,uint8_t byteValue)
{
    uint16_t startPos = byteFifoPtr->frontPos;
    uint16_t currentSize = byteFifoPtr->currentSize;
    if(currentSize == 0)return 1;
    while(currentSize != 0)
    {
        if(byteValue == byteFifoPtr->queueArrayBuffer[startPos++])
        {
            //已经存在相同的元素
            return 0;
        }
        //定位修正
        startPos %= byteFifoPtr->maxSize;
        //数量减少
        currentSize--;
    }
    //都没匹配上
    return 1;
}

//往循环队列中写入指定数量数据,成功返回1,失败返回0
uint8_t ADT_ByteFifoWriteByte(ADT_BYTE_FIFO* byteFifoPtr,uint8_t byteValue,uint8_t uniqueSet)
{
    if(byteFifoPtr == NULL)return 0;
    if(byteFifoPtr->maxSize == byteFifoPtr->currentSize)
    {
        //长度超出限制
        return 0;
    }
    else
    {
        if((0 == ADT_ByteFifoCheckExist(byteFifoPtr,byteValue))&&(uniqueSet == 1))
        {
            //队列中不存在该元素且要求必须元素唯一
            return 0;
        }
        else
        {
            ThreadLockInt();
            //写入数据
            byteFifoPtr->queueArrayBuffer[byteFifoPtr->rearPos++] = byteValue;
            //更新位置
            byteFifoPtr->rearPos %= byteFifoPtr->maxSize;
            //记录尺寸
            byteFifoPtr->currentSize++;
            ThreadUnlockInt();
            return 1;
        }
    }
}

//从循环队列中读取指定数量数据,成功返回1,失败返回0
uint8_t ADT_ByteFifoReadByte(ADT_BYTE_FIFO* byteFifoPtr,uint8_t* byteValue)
{
    if(byteFifoPtr == NULL)return 0;
    if(byteValue == NULL)return 0;
    if(0 == byteFifoPtr->currentSize)
    {
        //无数据
        *byteValue = 0;
        return 0;
    }
    else
    {
        ThreadLockInt();
        *byteValue = byteFifoPtr->queueArrayBuffer[byteFifoPtr->frontPos++];
        byteFifoPtr->frontPos %= byteFifoPtr->maxSize;
        byteFifoPtr->currentSize--;
        ThreadUnlockInt();
        return 1;
    }
}




