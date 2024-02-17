#ifndef _UART_DATA_TYPE_H_
#define _UART_DATA_TYPE_H_
#include "os.h"
typedef enum UART_CMD_DEF
{
    UART_CMD_JUMP_APP                   = 0x01,//跳转到APP 
    UART_CMD_JUMP_BOOT                  = 0x02,//跳转到Boot
    UART_CMD_UPDATE_START               = 0x03,//升级开始
    UART_CMD_UPDATE_DATA                = 0x04,//升级数据
    UART_CMD_UPDATE_END                 = 0x05,//升级结束
    UART_CMD_READ_SYSTEM_MODE           = 0x06,//读系统状态             
    UART_CMD_READ_SOFT_VERSION          = 0x07,//读软件版本
    UART_CMD_READ_SECTION               = 0x10,//读指定扇区
    UART_CMD_WRITE_SECTION              = 0x11,//写指定扇区
    UART_CMD_READ_REAGENT_DATA          = 0x12,//读试剂状态
    UART_CMD_WRITE_REAGENT_DATA         = 0x13,//写试剂状态
    UART_CMD_READ_ASSIST_REAGENT_DATA   = 0x14,//读辅助试剂状态
    UART_CMD_WRITE_ASSIST_REAGENT_DATA  = 0x15,//写辅助试剂状态
    UART_CMD_READ_RFID_CARD_ID          = 0X16,//读取RFID卡片ID
}UART_CMD_DEF;

//串口接收缓冲区长度
#define UART_CMD_RECV_DAT_LENGTH                    1200
//串口接收缓冲区
typedef struct UART_CMD_RECV_STATE{
    uint16_t recvDataLength;
    uint16_t recvBodyLength;
    uint8_t recvDat[UART_CMD_RECV_DAT_LENGTH];
}UART_CMD_RECV_STATE;

//串口数据解析
typedef struct UART_CMD_UTIL{
    uint8_t funcCode;
    uint16_t cmdDatLen;
    uint8_t *cmdDat;
}UART_CMD_UTIL;

//串口数据分发
typedef struct UART_CMD_DISPATCH_UNTIL
{
    uint8_t funcCode;
    OS_TCB *tcbTagetPtr;
}UART_CMD_DISPATCH_UNTIL;

#endif