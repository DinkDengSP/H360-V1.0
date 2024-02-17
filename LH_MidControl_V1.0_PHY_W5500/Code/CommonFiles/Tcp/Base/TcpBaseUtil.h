/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-04 15:00:53
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-12 19:07:52
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __TCP_BASE_UTIL_H_
#define __TCP_BASE_UTIL_H_
#include "TcpBaseData.h"

/***********************************字节数据转换为整机数据*************************************/
//从缓存中转换出一个无符号64位数
uint64_t TcpBaseUtilConvertBufferToUint64(uint8_t* bufferPtr,uint16_t startPos);
//从缓存中转换出一个无符号三十二位数
uint32_t TcpBaseUtilConvertBufferToUint32(uint8_t* bufferPtr,uint16_t startPos);
//从缓存中转换出一个无符号16位数
uint16_t TcpBaseUtilConvertBufferToUint16(uint8_t* bufferPtr,uint16_t startPos);
//从缓存中转换出一个有符号64位数
int64_t TcpBaseUtilConvertBufferToInt64(uint8_t* bufferPtr,uint16_t startPos);
//从缓存中转换出一个有符号32位数
int32_t TcpBaseUtilConvertBufferToInt32(uint8_t* bufferPtr,uint16_t startPos);
//从缓存中转换出一个有符号16位数
int16_t TcpBaseUtilConvertBufferToInt16(uint8_t* bufferPtr,uint16_t startPos);
//从缓存中转换出一个浮点数
float TcpBaseUtilConvertBufferToFloat(uint8_t* bufferPtr,uint16_t startPos);

/***********************************整体数据转换为字节数据******************************************/
//无符号64位数转换成数组
void TcpBaseUtilConvertUint64ToBuffer(uint64_t srcData,uint8_t* bufferPtr,uint16_t startPos);
//无符号三十二位数转换成数组
void TcpBaseUtilConvertUint32ToBuffer(uint32_t srcData,uint8_t* bufferPtr,uint16_t startPos);
//无符号十六位数转换成数组
void TcpBaseUtilConvertUint16ToBuffer(uint16_t srcData,uint8_t* bufferPtr,uint16_t startPos);
//有符号六十四位数转换成数组
void TcpBaseUtilConvertInt64ToBuffer(int64_t srcData,uint8_t* bufferPtr,uint16_t startPos);
//有符号三十二位数转换成数组
void TcpBaseUtilConvertInt32ToBuffer(int32_t srcData,uint8_t* bufferPtr,uint16_t startPos);
//有符号十六位数转换成数组
void TcpBaseUtilConvertInt16ToBuffer(int16_t srcData,uint8_t* bufferPtr,uint16_t startPos);
//有符号浮点数转换为数组
void TcpBaseUtilConvertFloatToBuffer(float srcData,uint8_t* bufferPtr,uint16_t startPos);


//网口指令包数据初始化
void TcpBaseUtilNetCmdPackInit(NET_COMMAND_PACK* netCmdPackPtr);

//初始化TCP通讯辅助状态
void TcpBaseUtilStateInit(void);

//获取当前网卡ack信号量指针
TX_SEMAPHORE* TcpBaseUtilGetNetAckSemPtr(void);

//获取设置网络通讯ACK标志
TCP_NET_ACK TcpBaseUtilGetNetAckFlag(void);
void TcpBaseUtilSetNetAckFlag(TCP_NET_ACK ackFlag);

//获取设置心跳时间
uint32_t TcpBaseUtilGetLastHeartTick(void);
void TcpBaseUtilSetLastHeartTick(uint32_t timeSet);
//设置心跳时间为当前时间
void TcpBaseUtilSetLastHeartTickNow(void);

//获取网口接收状态指针
NET_CMD_RECV_STATE* TcpBaseUtilGetRecvStatePtr(void);
//清零网口数据接收状态
void TcpBaseUtilClearRecvState(void);






#endif



