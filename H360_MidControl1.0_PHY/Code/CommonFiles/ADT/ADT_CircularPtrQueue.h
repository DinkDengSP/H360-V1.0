/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-02 14:15:41 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-02 14:22:11 +0800
************************************************************************************************/ 
#ifndef __ADT_CIRCULAR_PTR_QUEUE_H_
#define __ADT_CIRCULAR_PTR_QUEUE_H_
#include "ADT_Base.h"

//环形队列
//定义循环队列结构体
typedef struct ADT_CIRCULAR_PTR_QUEUE
{
    void** queueArrayPtr;       //循环队列缓冲区
    uint16_t frontPos;          //循环队列头,下一次取数据从该位置去取
    uint16_t rearPos;           //循环队列位,插入数据的时候看这个数据
    uint16_t maxSize;           //队列最大容量
    uint16_t currentSize;       //当前有效数据长度
}ADT_CIRCULAR_PTR_QUEUE;

//循环队列初始化
ADT_RESULT ADT_CircularPtrQueueInit(ADT_CIRCULAR_PTR_QUEUE* circularPtrQueuePtr,uint16_t bufferSize);

//获取队列最大容量
uint16_t ADT_CircularPtrQueueGetCapacity(ADT_CIRCULAR_PTR_QUEUE* circularPtrQueuePtr);

//获取队列当前总有效数据量
uint16_t ADT_CircularPtrQueueGetCurrentSize(ADT_CIRCULAR_PTR_QUEUE* circularPtrQueuePtr);

//往循环队列中写入数据
ADT_RESULT ADT_CircularPtrQueueWrite(ADT_CIRCULAR_PTR_QUEUE* circularPtrQueuePtr,void* ptr);

//从循环队列中读取数据
void* ADT_CircularPtrQueueGet(ADT_CIRCULAR_PTR_QUEUE* circularPtrQueuePtr);


#endif



