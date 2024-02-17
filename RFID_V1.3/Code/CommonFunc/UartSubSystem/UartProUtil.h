#ifndef _UART_PRO_UTIL_H_
#define _UART_PRO_UTIL_H_
#include "CoreUtil.h"
//将无符号32位数转换为buffer
void ConvertUint32ToArrayLowFirst(uint8_t* bufferPtr,uint16_t startPos,uint32_t srcDat);

//将无符号32位数转换为buffer
void ConvertUInt32ToArrayHighFirst(uint8_t* bufferPtr,uint16_t startPos,uint32_t srcDat);

//将无符号16位数转换为buffer,低位在前
void ConvertUint16ToArrayLowFist(uint8_t* bufferPtr,uint16_t startPos,uint16_t srcDat);

//将无符号16位数转换为buffer，高位在前
void ConvertUint16ToArrayHighFist(uint8_t* bufferPtr,uint16_t startPos,uint16_t srcDat);

//将buffer转换为无符号32位数,低位在前
uint32_t ConvertArrayToUint32LowFirst(uint8_t* bufferPtr,uint16_t startPos);

//将buffer转换为无符号32位数,低位在前
uint32_t ConvertArrayToUint32HighFirst(uint8_t* bufferPtr,uint16_t startPos);

//将buffer转换为无符号16位数,高位在前
uint16_t ConvertArrayToUint16HighFirst(uint8_t* bufferPtr,uint16_t startPos);

//将buffer转换为无符号16位数,低位在前
uint16_t ConvertArrayToUint16LowFirst(uint8_t* bufferPtr,uint16_t startPos);

//将buffer转换为有符号16位数,高位在前
int16_t ConvertArrayToInt16HighFirst(uint8_t* bufferPtr,uint16_t startPos);

//将buffer转换为有符号16位数,低位在前
int16_t ConvertArrayToInt16LowFirst(uint8_t* bufferPtr,uint16_t startPos);


#endif