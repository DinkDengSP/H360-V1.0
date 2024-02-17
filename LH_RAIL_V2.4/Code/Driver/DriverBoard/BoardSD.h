/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-06-19 09:11:12
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-06-22 13:39:57
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_SD_H_
#define __BOARD_SD_H_
#include "BoardCommon.h"


//SD卡当前状态
typedef enum SD_STATE
{
    SD_NULL   = 0X00,//未插入
    SD_INSERT = 0X01,//已经插入
}SD_STATE;

//SD卡传输状态
typedef enum SD_TRANS_STATE
{
    SD_TRANS_STATE_IDLE = 0X00,//传输状态空闲
    SD_TRANS_STATE_BUSY = 0X01,//传输状态忙
}SD_TRANS_STATE;

//sd卡通讯超时时间
#define BOARD_SD_TIME_OUT   1000
//打印函数
#define SDCardPrintf(format,args...)            MCU_Uart1Printf(format,##args)

//SD卡状态初始化
LH_ERR BoardSD_Init(void);

//读取指定块
LH_ERR BoardSD_ReadBlocks(uint32_t* pData, uint32_t ReadAddr, uint32_t NumOfBlocks);

//写入指定块
LH_ERR BoardSD_WriteBlocks(uint32_t* pData, uint32_t WriteAddr, uint32_t NumOfBlocks);

//读取指定块
LH_ERR BoardSD_ReadBlocksDMA(uint32_t* pData, uint32_t ReadAddr, uint32_t NumOfBlocks);

//写入指定块
LH_ERR BoardSD_WriteBlocksDMA(uint32_t* pData, uint32_t WriteAddr, uint32_t NumOfBlocks);

//擦除指定地址
LH_ERR BoardSD_Erase(uint32_t StartAddr, uint32_t EndAddr);

//获取内存卡状态
SD_TRANS_STATE BoardSD_GetCardState(void);

//获取内存卡讯息
LH_ERR BoardSD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo);

//获取SD卡是否已经插入
SD_STATE BoardSD_IsDetected(void);

//打印SD卡讯息
void BoardSD_PrintfCardMsg(void);

#endif




