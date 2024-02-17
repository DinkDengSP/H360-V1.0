/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-05-11 11:56:50
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-11 11:57:26
 *FilePath: \H360_ActionBoard_ID_3d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\SystemUtil\ADT_CirclePtrQueue.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __ADT_CIRCLE_PTR_QUEUE_H_
#define __ADT_CIRCLE_PTR_QUEUE_H_
#include "stm32f4xx.h"

//在操作数据的时候,必须关闭中断,防止多线程误触发,非中断情况下
#define ADT_CIRCLE_BYTE_PTR_LOCK_THREAD()         if(0 != __get_CONTROL())CPU_IntDis()

//数据操作完成,打开中断,使多线程可以触发,非中断情况下
#define ADT_CIRCLE_BYTE_PTR_UNLOCK_THREAD()       if(0 != __get_CONTROL())CPU_IntEn()

//环形队列
//定义循环队列结构体
typedef struct ADT_CIRCLE_PTR_QUEUE
{
    CanRxMsg** queueArrayBuffer;    //循环队列缓冲区
    uint16_t frontPos;         //循环队列头,下一次取数据从该位置去取
    uint16_t rearPos;          //循环队列位,插入数据的时候看这个数据
    uint16_t maxSize;          //队列最大容量
    uint16_t currentSize;      //当前有效数据长度
}ADT_CIRCLE_PTR_QUEUE;

//循环队列初始化
void ADT_CirclePtrQueueInit(ADT_CIRCLE_PTR_QUEUE* circlePtrQueuePtr,CanRxMsg** ptrBufferInit,uint16_t bufferSize);

//往循环队列中写入数据
void ADT_CirclePtrQueueWrite(ADT_CIRCLE_PTR_QUEUE* circlePtrQueuePtr,CanRxMsg* ptr);

//从循环队列中读取数据
CanRxMsg* ADT_CirclePtrQueueGet(ADT_CIRCLE_PTR_QUEUE* circlePtrQueuePtr);


#endif



