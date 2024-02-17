#ifndef __ADT_KMP_H_
#define __ADT_KMP_H_
#include "stm32f4xx.h"

//KMP滑动匹配算法
int ADT_KMP_Buffer(const char* srcString,uint16_t srcStringLength,const char* subString);

#endif



