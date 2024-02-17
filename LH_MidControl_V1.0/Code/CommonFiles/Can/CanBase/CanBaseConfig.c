/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-05 21:19:48
**LastEditors: DengXiaoJun
**LastEditTime: 2022-01-12 16:40:45
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CanBaseConfig.h"

//通过板号和模块号映射缓存通道
typedef struct MAP_CAN_MODULE_BOARD_TO_CHANNEL
{
    uint8_t moduleNo;
    uint8_t boardID;
    uint8_t channelNo;
}MAP_CAN_MODULE_BOARD_TO_CHANNEL;
//协议配置缓存列表
static const MAP_CAN_MODULE_BOARD_TO_CHANNEL mapCanModuleBoardToCacheChannel[] = {
    {CAN_DEVICE_MODULE_1_NO      ,CAN_DEVICE_BOARD_ID   ,CAN_DEVICE_TYPE_DEVICE_1    },//设备1
    {CAN_DEVICE_MODULE_2_NO      ,CAN_DEVICE_BOARD_ID   ,CAN_DEVICE_TYPE_DEVICE_2    },//设备2
    {CAN_DEVICE_MODULE_3_NO      ,CAN_DEVICE_BOARD_ID   ,CAN_DEVICE_TYPE_DEVICE_3    },//设备3
    {CAN_DEVICE_MODULE_4_NO      ,CAN_DEVICE_BOARD_ID   ,CAN_DEVICE_TYPE_DEVICE_4    },//设备4
    {CAN_RAIL_MODULE_NO          ,CAN_RAIL_BOARD_ID     ,CAN_DEVICE_TYPE_RAIL        },//轨道
    {CAN_CLOUD_MODULE_NO         ,CAN_CLOUD_BOARD_ID    ,CAN_DEVICE_TYPE_CLOUD       },//立禾云
};
#define LENGTH_MAP_CAN_MODULE_BOARD_TO_CHANNEL  (sizeof(mapCanModuleBoardToCacheChannel)/sizeof(mapCanModuleBoardToCacheChannel[0]))

//通过模块号和板号计算缓存通道
uint8_t CanCalcCacheChannelByModuleBoard(uint8_t moduleNo,uint8_t boardID)
{
    uint8_t indexUtil = 0;
    //循环查找配置表里面是否存在对应的映射
    for(indexUtil = 0; indexUtil < LENGTH_MAP_CAN_MODULE_BOARD_TO_CHANNEL;indexUtil++)
    {
        if((mapCanModuleBoardToCacheChannel[indexUtil].moduleNo == moduleNo)&&
            (mapCanModuleBoardToCacheChannel[indexUtil].boardID == boardID))
        {
            //缓存通道序号不能大于最大通道数量
            if(mapCanModuleBoardToCacheChannel[indexUtil].channelNo < CAN_RECV_BUFFER_CHANNEL)
            {
                return mapCanModuleBoardToCacheChannel[indexUtil].channelNo;
            }
            //匹配不上,不存在缓存通道
            return CAN_DEVICE_TYPE_INVALID;
        }
    }
    //匹配不上,不存在缓存通道
    return CAN_DEVICE_TYPE_INVALID;
}

//通过模块号和板号找出对应的CAN端口
typedef struct MAP_CAN_MODULE_BOARD_TO_PORT
{
    uint8_t moduleNo;
    uint8_t boardID;
    uint8_t portNo;//端口0为CAN1 端口1为CAN2
}MAP_CAN_MODULE_BOARD_TO_PORT;
//协议配置端口列表
static const MAP_CAN_MODULE_BOARD_TO_PORT mapCanModuleBoardToCachePort[] = {
    {CAN_DEVICE_MODULE_1_NO      ,CAN_DEVICE_BOARD_ID   ,CAN_PORT_1},//化学发光
    {CAN_DEVICE_MODULE_2_NO      ,CAN_DEVICE_BOARD_ID   ,CAN_PORT_1},//生化
    {CAN_RAIL_MODULE_NO          ,CAN_RAIL_BOARD_ID     ,CAN_PORT_1},//轨道
    {CAN_CLOUD_MODULE_NO         ,CAN_CLOUD_BOARD_ID    ,CAN_PORT_1},//立禾云
};
#define LENGTH_MAP_CAN_MODULE_BOARD_TO_PORT  (sizeof(mapCanModuleBoardToCachePort)/sizeof(mapCanModuleBoardToCachePort[0]))

//通过模块号和板号计算缓存通道
uint8_t CanCalcCachePortByModuleBoard(uint8_t moduleNo,uint8_t boardID)
{
    uint8_t indexUtil = 0;
    //循环查找配置表里面是否存在对应的映射
    for(indexUtil = 0; indexUtil < LENGTH_MAP_CAN_MODULE_BOARD_TO_PORT;indexUtil++)
    {
        if((mapCanModuleBoardToCachePort[indexUtil].moduleNo == moduleNo)&&
            (mapCanModuleBoardToCachePort[indexUtil].boardID == boardID))
        {
            return mapCanModuleBoardToCachePort[indexUtil].portNo;
        }
    }
    //匹配不上,不存在缓存通道
    return CAN_PORT_NONE;
}
