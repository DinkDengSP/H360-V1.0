/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-11 18:50:51
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-11 19:11:24
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __ADT_BYTE_FIFO_H_
#define __ADT_BYTE_FIFO_H_
#include "stm32h7xx_hal.h"

//环形队列
//定义循环队列结构体
typedef struct ADT_BYTE_FIFO
{
    uint8_t *queueArrayBuffer; //循环队列缓冲区
    uint16_t frontPos;         //循环队列头,下一次取数据从该位置去取
    uint16_t rearPos;          //循环队列位,插入数据的时候看这个数据
    uint16_t maxSize;          //队列最大容量
    uint16_t currentSize;      //当前有效数据长度
}ADT_BYTE_FIFO;

//队列初始化
void ADT_ByteFifoInit(ADT_BYTE_FIFO* byteFifoPtr,uint8_t* bufferStartPtr,uint16_t bufferSize);

//获取当前队列的元素数量
uint16_t ADT_ByteFifoGetCurrentSize(ADT_BYTE_FIFO* byteFifoPtr);

//往循环队列中写入指定数量数据,成功返回1,失败返回0
uint8_t ADT_ByteFifoWriteByte(ADT_BYTE_FIFO* byteFifoPtr,uint8_t byteValue,uint8_t uniqueSet);

//从循环队列中读取指定数量数据,成功返回1,失败返回0
uint8_t ADT_ByteFifoReadByte(ADT_BYTE_FIFO* byteFifoPtr,uint8_t* byteValue);



#endif

