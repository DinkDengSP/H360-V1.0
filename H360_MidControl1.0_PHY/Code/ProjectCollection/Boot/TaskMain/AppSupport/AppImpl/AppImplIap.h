/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-19 15:52:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-19 17:09:32 +0800
************************************************************************************************/ 
#ifndef __APP_IMPL_IAP_H_
#define __APP_IMPL_IAP_H_
#include "AppImplBase.h"

//IAP的状态
typedef enum BOARD_IAP_STATE
{
    BOARD_IAP_IDLE = 0X00,
    BOARD_IAP_BUSY = 0X01,
    BOARD_IAP_ERR  = 0X02,
}BOARD_IAP_STATE;

typedef struct BOARD_IAP_RECORD
{
    uint8_t boardID;
    BOARD_IAP_STATE iapState;
    uint32_t exceptRecvDataLengthBytes;//期望接收的数据长度
    uint16_t exceptRecvDataPackCount;//期望接收的数据总包数
    uint16_t singlePackLengthBytesSet;//期望接收的每一包数据的有效数据长度
    uint32_t currentRecvDataLengthBytes;//当前已经接收到的数据长度
    uint16_t currentRecvDataPackCount;//当前已经接收到的数据包数
    uint8_t* recvDataBuffer;
}BOARD_IAP_RECORD;

extern BOARD_IAP_RECORD boardIapRecord;

//初始化
void AppImplIapStackInit(void);

//包处理
void AppImplIapNetPackProcess(NET_COMMAND_PACK* netCmdPackPtr);


#endif


