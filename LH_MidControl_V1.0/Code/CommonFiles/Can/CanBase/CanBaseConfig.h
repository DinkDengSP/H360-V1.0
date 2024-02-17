/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-07-30 11:48:54
**LastEditors: DengXiaoJun
**LastEditTime: 2022-01-20 11:52:51
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_BASE_CONFIG_H_
#define __CAN_BASE_CONFIG_H_
#include "stm32h7xx_hal.h"

//广播目标板号
#define CAN_BOARDCAST_BOARD_ID              0X3F
//系统支持的CAN并发模块数量
#define CAN_RECV_BUFFER_CHANNEL             7
//设备类型
typedef enum CAN_DEVICE_TYPE
{
    CAN_DEVICE_TYPE_DEVICE_1     = 0     ,//设备1
    CAN_DEVICE_TYPE_DEVICE_2     = 1     ,//设备2
    CAN_DEVICE_TYPE_DEVICE_3     = 2     ,//设备3
    CAN_DEVICE_TYPE_DEVICE_4     = 3     ,//设备4
    CAN_DEVICE_TYPE_RAIL         = 5     ,//轨道
    CAN_DEVICE_TYPE_CLOUD        = 6     ,//立禾云
    CAN_DEVICE_TYPE_INVALID      = 0XFF, //不支持的模块
}CAN_DEVICE_TYPE;
//通道类型
typedef enum CAN_PORT
{
    CAN_PORT_1       = 0,    //CAN通道1
    CAN_PORT_2       = 1,    //CAN通道2
    CAN_PORT_NONE    = 0XFF, //CAN通道不存在
}CAN_PORT;

//中位机模块号
#define CAN_SELF_MODULE_NO                  0X05
//中位机板号
#define CAN_SELF_BOARD_ID                   0X20

//轨道主控模块号
#define CAN_RAIL_MODULE_NO                  0X05
//轨道主控板号
#define CAN_RAIL_BOARD_ID                   0X23

//立禾云模块号
#define CAN_CLOUD_MODULE_NO                 0X05
//立禾云板号
#define CAN_CLOUD_BOARD_ID                  0X27
///////////////////////////////////////////////////////////////
//设备1模块号
#define CAN_DEVICE_MODULE_1_NO              0X00
//设备2模块号
#define CAN_DEVICE_MODULE_2_NO              0X01
//设备1模块号
#define CAN_DEVICE_MODULE_3_NO              0X02
//设备2模块号
#define CAN_DEVICE_MODULE_4_NO              0X03
//设备主控板号
#define CAN_DEVICE_BOARD_ID                 0X01


//单包最大值
#define CAN_SINGLE_PACK_MAX_LENGTH         2048
//长包指令起始包
#define CAN_COMMAND_LONG_PACK_START        0X7FD  
//长包指令数据包
#define CAN_COMMAND_LONG_PACK_DATA         0X7FE
//长包指令结束包
#define CAN_COMMAND_LONG_PACK_END          0X7FF
//ACK最大等待时间
#define TIME_MS_MAX_CAN_WAIT_ACK           2000
//收不到ACK的最大重发次数
#define TIME_COUNT_MAX_CAN_WAIT_ACK        3   

//通过模块号和板号计算缓存通道
uint8_t CanCalcCacheChannelByModuleBoard(uint8_t moduleNo,uint8_t boardID);
//通过模块号和板号计算缓存通道
uint8_t CanCalcCachePortByModuleBoard(uint8_t moduleNo,uint8_t boardID);

#endif



