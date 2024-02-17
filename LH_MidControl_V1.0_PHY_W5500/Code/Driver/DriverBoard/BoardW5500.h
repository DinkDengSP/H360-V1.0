/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-14 13:42:57 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-14 13:49:57 +0800
************************************************************************************************/ 
#ifndef __BOARD_W5500_H_
#define __BOARD_W5500_H_
#include "BoardW5500_Base.h"

//获取指定socket空闲内存
LH_ERR BoardW5500_GetSocketTxFreeBufferSize(W5500_SOCKET_NO socketNo,uint16_t* freeSizeBytes);
LH_ERR BoardW5500_GetSocketRxFreeBufferSize(W5500_SOCKET_NO socketNo,uint16_t* freeSizeBytes);

//写入数据到W5500
LH_ERR BoardW5500_WriteSocketTxDataBuffer(W5500_SOCKET_NO socketNo,uint8_t* dataBufferPtr,uint16_t dataLength);
//读取当前接收缓冲区数据长度
LH_ERR BoardW5500_ReadSocketRecvDataLength(W5500_SOCKET_NO socketNo,uint16_t* dataLength);
//从W5500读取数据
LH_ERR BoardW5500_ReadSocketRxDataBuffer(W5500_SOCKET_NO socketNo,uint8_t* dataBufferPtr,uint16_t dataLength);
//释放接收到的数据
void BoardW5500_ReleaseRecvDataBuffer(uint8_t* recvDataBufferPtr);

//配置全部的SOCKET缓冲区
LH_ERR BoardW5500_ConfigAllTxRxBufferSize(void);

//板卡复位
LH_ERR BoardW5500_ChipReset(void);
//PHY复位
LH_ERR BoardW5500_PhyReset(void);
//获取PHY状态
LH_ERR BoardW5500_GetPhyState(W5500_LINK_MODE* linkModePtr);
//配置W5500通讯模式
LH_ERR BoardW5500_ConfigMacRawMode(uint32_t ipValue);
//关闭W5500 MACRAW
LH_ERR BoardW5500_CloseMacRawMode(void);
//W5500发出一帧数据
LH_ERR BoardW5500_SendOneFrameMacRaw(uint8_t* dataBufferPtr,uint16_t dataLength);
//W5500读取一帧数据
LH_ERR BoardW5500_RecvOneFrameMacRaw(uint8_t** dataBufferPtr,uint16_t* dataLength);







#endif
