#ifndef _UART_CMD_PROTOCOL_H_
#define _UART_CMD_PROTOCOL_H_
#include "UartProUtil.h"
#include "UartDatatype.h"
//--区域----帧头------功能码-----数据长度------数据-----和校验-------帧尾--------------------
//--地址-----0-1--------2----------3-4----------5--------6----------7-8-------
//--数据---0xAA55-------1---------0x001---------xx--------n----------0X55AA----------------
//串口接收命令最小长度
#define UART_CMD_MIN_PROTOCOL_LEN                   8
#define UART_ACK_MIN_PROTOCOL_LEN                   12
//串口接收头
#define UART_CMD_HEAD_CODE                          0XAA55
//串口接收尾
#define UART_CMD_TAIL_CODE                          0X55AA
//功能码地址
#define UART_FUNC_CODE_OFFSET                        2
//数据长度偏移
#define UART_CMD_BODY_LEN_OFFSET                     3
//数据地址偏移
#define UART_CMD_BODY_DATA_OFFSET                    5

//--------------------------------------函数声明---------------------------------------------------------
//清空串口接收状态
void Uart1CmdRecvStateClear(UART_CMD_RECV_STATE *cmdRecvState);

//数据和校验
uint8_t Uart1DataSumCheck(uint8_t *dat,uint16_t size);

//写应答数据包
void Uart1CmdSendAckPack(uint8_t funcCode,uint32_t errorCode,uint16_t datLen,uint8_t *datPtr);
#endif