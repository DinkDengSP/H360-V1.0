/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-12-11 18:58:16
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-12-11 19:20:22
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BYTE_CONVERT_H_
#define __BYTE_CONVERT_H_
#include "stdint.h"

//将两个字节合并为一个16位数,高字节在前,低字节在后
#define BYTE_TO_HWORD(hByte, lByte)                     ((uint16_t)((lByte)|((hByte)<<8)))

//将四个字节合并为一个三十二位数,高字节在前,低字节在后
#define BYTE_TO_WORD(hhByte, hlByte, lhByte, llByte)    ((uint32_t)((llByte)|((lhByte)<<8)|((hlByte)<<16)|((hhByte)<<24)))

//将一个16位数的低八位取出来成为一个字节
#define HWORD_TO_BYTE_L(a)                              ((uint8_t)((a)&0xFF))

//将一个16位数的高八位取出来成为一个字节
#define HWORD_TO_BYTE_H(dat)                            ((uint8_t)(((dat)>>8)&0xFF))

//将一个32位数的0-7位取出来成为一个字节
#define WORD_TO_BYTE_LL(dat)                            ((uint8_t)((dat)&0xFF))

//将一个32位数的8-15位取出来成为一个字节
#define WORD_TO_BYTE_LH(dat)                            ((uint8_t)(((dat)>>8)&0xFF))

//将一个32位数的16-23位取出来成为一个字节
#define WORD_TO_BYTE_HL(dat)                            ((uint8_t)(((dat)>>16)&0xFF))

//将一个32位数的24-31位取出来成为一个字节
#define WORD_TO_BYTE_HH(dat)                            ((uint8_t)(((dat)>>24)&0xFF))

//将一个16位数的低八位取出来成为一个字节
#define HWORD_TO_TCP_BYTE_L(dat)                        ((uint8_t)((dat)&0xFF))

//将一个16位数的高八位取出来成为一个字节
#define HWORD_TO_TCP_BYTE_H(dat)                        ((uint8_t)(((dat)>>8)&0xFF))

//将一个32位数的0-7位取出来成为一个字节
#define WORD_TO_TCP_BYTE_LL(dat)                        ((uint8_t)((dat)&0xFF))

//将一个32位数的8-15位取出来成为一个字节
#define WORD_TO_TCP_BYTE_LH(dat)                        ((uint8_t)(((dat)>>8)&0xFF))

//将一个32位数的16-23位取出来成为一个字节
#define WORD_TO_TCP_BYTE_HL(dat)                        ((uint8_t)(((dat)>>16)&0xFF))

//将一个32位数的24-31位取出来成为一个字节
#define WORD_TO_TCP_BYTE_HH(dat)                        ((uint8_t)(((dat)>>24)&0xFF))

//将指定指针位置的两个八位数合成一个16位数,下标低的在低位,下标高的在高位
#define MK_TCP_HWORD(ptr)                               ((uint16_t)(*(ptr)|(*(ptr+1)<<8)))

//从数组中创建一个TCP指令码,参数是指令码数组指针
#define MK_TCP_CMD(ptr)                                 ((TCP_CMD)MK_TCP_HWORD(ptr))

//从数组中构建一个符合协议的TCP 32位数,参数为首字节地址
#define MK_TCP_WORD(ptr)                                ((uint32_t)(*(ptr)|(*(ptr+1)<<8)|(*(ptr+2)<<16)|(*(ptr+3)<<24)))

//从数组中构建一个符合协议的TCP 64位数,参数为首字节地址
#define MK_TCP_DWORD(ptr)                               ((uint64_t)(((uint64_t)(*(ptr)))|((uint64_t)(*(ptr+1))<<8)|((uint64_t)(*(ptr+2))<<16)|((uint64_t)(*(ptr+3))<<24)| \
                                                        ((uint64_t)(*(ptr+4))<<32)|((uint64_t)(*(ptr+5))<<40)|((uint64_t)(*(ptr+6))<<48)|((uint64_t)(*(ptr+7))<<56)))


#endif




