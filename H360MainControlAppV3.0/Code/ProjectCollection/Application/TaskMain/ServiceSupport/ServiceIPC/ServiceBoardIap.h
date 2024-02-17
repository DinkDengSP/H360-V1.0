/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-07-05 14:21:10
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-07-05 15:42:10
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_BOARD_IAP_H_
#define __SERVICE_BOARD_IAP_H_
#include "ServiceImplInc.h"

#define CAN1_RAIL_ACT_BOARD1_ID     0x02//一号板
#define CAN1_RAIL_ACT_BOARD2_ID     0x03//二号板
#define CAN1_RAIL_ACT_BOARD3_ID     0x04//三号板
#define CAN1_RAIL_ACT_BOARD4_ID     0x05//四号板
#define CAN1_RAIL_ACT_BOARD5_ID     0x06//五号板
#define CAN1_RAIL_TC_BOARD_ID       0x0A//温控板
#define CAN1_IMM_RFID_BOARD_ID      0x80//试剂盘RFID板,0X80以下CAN升级,0X80以上串口


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
}BOARD_IAP_RECORD;

//初始化IAP讯息
void ServiceBoardIapInit(void);

//程序跳转
BOARD_IAP_STATE ServiceBoardIapProgramJump(uint8_t boardID,uint8_t *currentState);

//开始升级准备
BOARD_IAP_STATE ServiceBoardIapPrepareUpdata(uint8_t boardID);

//写入起始讯息
BOARD_IAP_STATE ServiceBoardIapWriteStartMsg(uint8_t boardID,uint32_t exceptBytes,uint16_t exceptPackCount,uint16_t singlePackLength);

//写入数据讯息
BOARD_IAP_STATE ServiceBoardIapWriteData(uint8_t boardID,uint16_t packIndex,uint16_t dataLength,uint8_t* dataBuffer);

//计算结果校验和
BOARD_IAP_STATE ServiceBoardIapCalcCRC32(uint8_t boardID,uint32_t crc32Result);

//开始将数据写入Flash
BOARD_IAP_STATE ServiceBoardIapStartUpdata(uint8_t boardID);

#endif





