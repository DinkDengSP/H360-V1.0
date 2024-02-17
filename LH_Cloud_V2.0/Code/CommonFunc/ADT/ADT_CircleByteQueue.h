/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-24 12:19:14
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-02 17:51:56
 *FilePath: \H360_ActionBoard_ID_1d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\SystemUtil\ADT_CircleByteQueue.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __ADT_CIRCLE_BYTE_QUEUE_H_
#define __ADT_CIRCLE_BYTE_QUEUE_H_
#include "stm32f4xx.h"

//环形队列
//定义循环队列结构体
typedef struct ADT_CIRCLE_BYTE_QUEUE
{
    uint8_t *queueArrayBuffer; //循环队列缓冲区
    uint16_t frontPos;         //循环队列头,下一次取数据从该位置去取
    uint16_t rearPos;          //循环队列位,插入数据的时候看这个数据
    uint16_t maxSize;          //队列最大容量
    uint16_t currentSize;      //当前有效数据长度
}ADT_CIRCLE_BYTE_QUEUE;

//在操作数据的时候,必须关闭中断,防止多线程误触发
#define ADT_CIRCLE_BYTE_QUEUE_LOCK_THREAD()       if(0 == __get_CONTROL())CPU_IntDis()
//数据操作完成,打开中断,使多线程可以触发
#define ADT_CIRCLE_BYTE_QUEUE_UNLOCK_THREAD()     if(0 == __get_CONTROL())CPU_IntEn()


//循环队列初始化
void ADT_CircleByteQueueInit(ADT_CIRCLE_BYTE_QUEUE* circleByteQueuePtr,uint8_t* bufferStartPtr,uint16_t bufferSize);

//获取循环队列最大容量
uint16_t ADT_CircleByteQueueGetMaxCapacity(ADT_CIRCLE_BYTE_QUEUE* circleByteQueuePtr);

//获取当前循环队列已经存储的数据
uint16_t ADT_CircleByteQueueGetCurrentSize(ADT_CIRCLE_BYTE_QUEUE* circleByteQueuePtr);

//获取当前循环队列中空数据的量
uint16_t ADT_CircleByteQueueGetCurrentEmptySize(ADT_CIRCLE_BYTE_QUEUE* circleByteQueuePtr);

//往循环队列中写入指定数量数据,当空间不足的时候,会将缓冲区数据填满,并返回真实的写入量
void ADT_CircleByteQueueWriteBuffer(ADT_CIRCLE_BYTE_QUEUE* circleByteQueuePtr,uint8_t* bufferWritePtr,uint16_t length,uint16_t* realWriteByteCount);

//从循环队列中读取指定数量数据,当数据不足时,已有的数据写到缓冲区,并返回真实读取到的数据
void ADT_CircleByteQueueReadBuffer(ADT_CIRCLE_BYTE_QUEUE* circleByteQueuePtr,uint8_t* bufferReadPtr,uint16_t length,uint16_t* realReadByteCount);




#endif




