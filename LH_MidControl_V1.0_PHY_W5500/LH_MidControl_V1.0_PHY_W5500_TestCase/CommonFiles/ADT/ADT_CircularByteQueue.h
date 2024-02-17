/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-10-31 20:03:03 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-10-31 20:04:56 +0800
************************************************************************************************/ 
#ifndef __ADT_CIRCULAR_BYTE_QUEUE_H_
#define __ADT_CIRCULAR_BYTE_QUEUE_H_
#include "ADT_Base.h"

//环形队列
//定义循环队列结构体
typedef struct ADT_CIRCULAR_BYTE_QUEUE
{
    uint8_t *queueArrayBuffer; //循环队列缓冲区
    uint16_t frontPos;         //循环队列头,下一次取数据从该位置去取
    uint16_t rearPos;          //循环队列位,插入数据的时候看这个数据
    uint16_t maxSize;          //队列最大容量
    uint16_t currentSize;      //当前有效数据长度
}ADT_CIRCULAR_BYTE_QUEUE;

//循环队列初始化
void ADT_CircularByteQueueInit(ADT_CIRCULAR_BYTE_QUEUE* circularByteQueuePtr,uint8_t* bufferStartPtr,uint16_t bufferSize);

//获取循环队列最大容量
uint16_t ADT_CircularByteQueueGetMaxCapacity(ADT_CIRCULAR_BYTE_QUEUE* circularByteQueuePtr);

//获取当前循环队列已经存储的数据
uint16_t ADT_CircularByteQueueGetCurrentSize(ADT_CIRCULAR_BYTE_QUEUE* circularByteQueuePtr);

//获取当前循环队列中空数据的量
uint16_t ADT_CircularByteQueueGetCurrentEmptySize(ADT_CIRCULAR_BYTE_QUEUE* circularByteQueuePtr);

//往循环队列中写入指定数量数据,当空间不足的时候,会将缓冲区数据填满,并返回真实的写入量
void ADT_CircularByteQueueWriteBuffer(ADT_CIRCULAR_BYTE_QUEUE* circularByteQueuePtr,uint8_t* bufferWritePtr,uint16_t length,uint16_t* realWriteByteCount);

//从循环队列中读取指定数量数据,当数据不足时,已有的数据写到缓冲区,并返回真实读取到的数据
void ADT_CircularByteQueueReadBuffer(ADT_CIRCULAR_BYTE_QUEUE* circularByteQueuePtr,uint8_t* bufferReadPtr,uint16_t length,uint16_t* realReadByteCount);

//清空队列
void ADT_CircularByteQueueClear(ADT_CIRCULAR_BYTE_QUEUE* circularByteQueuePtr);



#endif



