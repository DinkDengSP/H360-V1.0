/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 15:44:51
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-04 15:39:26
 *FilePath: \DeviceMainApplicationc:\LiHe\Local\02-Development\Software\H360_STM32\H360MainControlAppV2.2\Code\CommonFunc\Can2Protocol\Can2ProcConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_PROC_CONFIG_H_
#define __CAN2_PROC_CONFIG_H_
#include "BoardInc.h"

//CAN2协议内存,使用SRAM IN

//主控板号
#define DEVICE_MAIN_BOARD_ID                           0X12

//协议一个指令包能支持的最大数据长度,接收
#define SYSTEM_CAN2_SINGLE_RECV_DATA_MAX               400

//协议支持的下位机板的最大数量
#define SYSTEM_CAN2_BOARD_MAX                           10

//协议支持的下位机板的每个板卡的通道上限
#define SYSTEM_CAN2_BOARD_CHANNEL_MAX                   32

//协议规定的ACK响应时间
#define SYSTEM_CAN2_ACK_TIMEOUT                         500

//协议规定的ACK重试次数
#define SYSTEM_CAN2_ACK_RETRY_MAX                       3



#endif




