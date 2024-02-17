/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-07-28 14:43:24
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-07-28 15:13:46
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __ADT_CIRCLE_PTR_QUEUE_H_
#define __ADT_CIRCLE_PTR_QUEUE_H_
#include "stm32h7xx_hal.h"
#include "ADT_Port.h"

//环形队列
//定义循环队列结构体
typedef struct ADT_CIRCLE_PTR_QUEUE
{
    void** queueArrayPtr;       //循环队列缓冲区
    uint16_t frontPos;          //循环队列头,下一次取数据从该位置去取
    uint16_t rearPos;           //循环队列位,插入数据的时候看这个数据
    uint16_t maxSize;           //队列最大容量
    uint16_t currentSize;       //当前有效数据长度
}ADT_CIRCLE_PTR_QUEUE;

//循环队列初始化
ADT_RESULT ADT_CirclePtrQueueInit(ADT_CIRCLE_PTR_QUEUE* circlePtrQueuePtr,uint16_t bufferSize);

//获取队列最大容量
uint16_t ADT_CirclePtrQueueGetCapacity(ADT_CIRCLE_PTR_QUEUE* circlePtrQueuePtr);

//获取队列当前总有效数据量
uint16_t ADT_CirclePtrQueueGetCurrentSize(ADT_CIRCLE_PTR_QUEUE* circlePtrQueuePtr);

//往循环队列中写入数据
ADT_RESULT ADT_CirclePtrQueueWrite(ADT_CIRCLE_PTR_QUEUE* circlePtrQueuePtr,void* ptr);

//从循环队列中读取数据
void* ADT_CirclePtrQueueGet(ADT_CIRCLE_PTR_QUEUE* circlePtrQueuePtr);



#endif



