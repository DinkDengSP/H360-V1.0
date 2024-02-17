/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-10-27 18:10:43 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-28 10:31:46 +0800
************************************************************************************************/ 
#ifndef __BASE_DATA_TYPE_H_
#define __BASE_DATA_TYPE_H_
#include "stdint.h"

//串口数据源
typedef enum UART_PACK_SRC
{
    UART_PACK_SRC_UART1     = 0X00,
    UART_PACK_SRC_UART6     = 0X01,
}UART_PACK_SRC;

//串口接收数据包结构
typedef struct UART_RECV_PACK
{
    UART_PACK_SRC packSrc;//数据源
    uint8_t* recvDataBufferPtr;//接收数据缓存区
    uint16_t recvDataLength;//接受数据长度
}UART_RECV_PACK;
#define LENGTH_UART_RECV_PACK       (sizeof(UART_RECV_PACK)/sizeof(uint8_t))

//CAN数据源
typedef enum CAN_PACK_SRC
{
    CAN_PACK_SRC_1  = 0X00,//CAN1端口数据
    CAN_PACK_SRC_2  = 0X01,//CAN2端口数据
}CAN_PACK_SRC;

//CAN数据包类型
typedef enum CAN_ID_TYPE
{
    CAN_ID_STD      = 0X00,//标准帧
    CAN_ID_EXTEND   = 0X01,//扩展帧
}CAN_ID_TYPE;

//CAN接收数据包结构
typedef struct CAN_RECV_PACK
{
    CAN_PACK_SRC packSrc;//数据包源
    CAN_ID_TYPE idType;//ID类型
    uint32_t recvCmdID;//接收到的ID
    uint8_t* recvDataBufferPtr;//接收数据指针
    uint16_t recvDataLength;//接收数据长度
}CAN_RECV_PACK;

//网口数据源
typedef enum NET_PACK_SRC
{
    NET_PACK_SRC_LAN8700 = 0X00,
    NET_PACK_SRC_W5500 = 0X01,
}NET_PACK_SRC;

//网口数据包
typedef struct NET_RECV_PACK
{
    NET_PACK_SRC netPackSrc;//数据源
    uint32_t peerIpAddress;    //发送数据的远端IP
    uint32_t peerPort;         //发送数据的远端端口
    uint32_t recvDataLength;   //接收数据长度
    uint8_t* recvDatBufPtr; //接收数据指针
}NET_RECV_PACK;
#define LENGTH_NET_RECV_PACK        (sizeof(NET_RECV_PACK)/sizeof(uint8_t))



#endif



