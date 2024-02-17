/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-05 11:59:15
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-23 09:30:41
 *FilePath: \H360_ActionBoard_ID_2d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanSubSystem\CanSubBase.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_SUB_BASE_H_
#define __CAN_SUB_BASE_H_
#include "CanMasterMain.h"

//杯中最大液体量
#define CUP_MAX_CAPACITY_UL                         1400
//从杯口到杯底步数
#define STEPS_FROM_CUP_TOP_TO_BOTTOM                1048.0

//传感器状态,有效还是无效
typedef enum SENSOR_STATE
{
    SENSOR_STATE_INVALID = 0X00,//传感器状态无效
    SENSOR_STATE_VALID   = 0X01,//传感器状态有效
}SENSOR_STATE;

//输出状态
typedef enum OUT_STATE
{
    OUT_STATE_DISABLE = 0X00,//输出无效
    OUT_STATE_ENABLE  = 0X01,//输出有效
}OUT_STATE;

//混匀模式,每个混匀有多种混匀模式,具体需要底层下位机动作版自己定义
typedef enum MIX_MODE
{
    MIX_MODE_1   = 0X00,
    MIX_MODE_2   = 0X01,
    MIX_MODE_3   = 0X02,
    MIX_MODE_4   = 0X03,
    MIX_MODE_5   = 0X04,
    MIX_MODE_6   = 0X05,
    MIX_MODE_7   = 0X06,
    MIX_MODE_8   = 0X07,
    MIX_MODE_9   = 0X08,
    MIX_MODE_10  = 0X09,
    MIX_MODE_MAX = MIX_MODE_10,
}MIX_MODE;

//针清洗配置
typedef enum NEEDLE_CLEAN_OPT
{
    NEEDLE_CLEAN_OPT_IN     = 0X01,//内壁清洗
    NEEDLE_CLEAN_OPT_OUT    = 0X02,//外壁清洗
    NEEDLE_CLEAN_OPT_ALL    = 0X03,//内外壁同时清洗
}NEEDLE_CLEAN_OPT;

//针灌注配置
typedef enum NEEDLE_PRIME_OPT
{
    NEEDLE_PRIME_OPT_IN     = 0X01,//内壁灌注
    NEEDLE_PRIME_OPT_OUT    = 0X02,//外壁灌注
    NEEDLE_PRIME_OPT_ALL    = 0X03,//内外壁同时灌注
}NEEDLE_PRIME_OPT;

//针维护配置
typedef enum NEEDLE_MAINTAIN_OPT
{
    NEEDLE_MAINTAIN_OPT_IN     = 0X01,//内壁灌注
    NEEDLE_MAINTAIN_OPT_OUT    = 0X02,//外壁灌注
    NEEDLE_MAINTAIN_OPT_ALL    = 0X03,//内外壁同时灌注
}NEEDLE_MAINTAIN_OPT;

//针清洗时间
typedef enum NEEDLE_CLEAN_TIME
{
    NEEDLE_CLEAN_TIME_1 = 0x00,
    NEEDLE_CLEAN_TIME_2 = 0x01,
    NEEDLE_CLEAN_TIME_3 = 0x02,
    NEEDLE_CLEAN_TIME_4 = 0x03,
    NEEDLE_CLEAN_TIME_5 = 0x04,
}NEEDLE_CLEAN_TIME;

//注液高度
typedef enum NEEDLE_INJECT_LEVEL
{
    NEEDLE_INJECT_LEVEL_1_SHALLOWEST    = 0x00,//排液位置最浅
    NEEDLE_INJECT_LEVEL_2               = 0X01,
    NEEDLE_INJECT_LEVEL_3               = 0X02,
    NEEDLE_INJECT_LEVEL_4               = 0X03,
    NEEDLE_INJECT_LEVEL_5               = 0X04,
    NEEDLE_INJECT_LEVEL_6               = 0X05,
    NEEDLE_INJECT_LEVEL_7               = 0X06,
    NEEDLE_INJECT_LEVEL_8_DEEPEST       = 0X07,//排液位置最深
}NEEDLE_INJECT_LEVEL;

//吸液高度
typedef enum NEEDLE_ABSORB_LEVEL
{
    NEEDLE_ABSORB_LEVEL_1_SHALLOWEST    = 0x00,//吸液位置最浅
    NEEDLE_ABSORB_LEVEL_2               = 0X01,
    NEEDLE_ABSORB_LEVEL_3               = 0X02,
    NEEDLE_ABSORB_LEVEL_4               = 0X03,
    NEEDLE_ABSORB_LEVEL_5               = 0X04,
    NEEDLE_ABSORB_LEVEL_6               = 0X05,
    NEEDLE_ABSORB_LEVEL_7               = 0X06,
    NEEDLE_ABSORB_LEVEL_8_DEEPEST       = 0X07,//吸液位置最深
}NEEDLE_ABSORB_LEVEL;

//探液板配置
typedef enum LIQUID_DETECT_CONFIG
{
    LIQUID_DETECT_CONFIG_1  = 0,
    LIQUID_DETECT_CONFIG_2  = 1,
    LIQUID_DETECT_CONFIG_3  = 2,
    LIQUID_DETECT_CONFIG_4  = 3,
    LIQUID_DETECT_CONFIG_5  = 4,
    LIQUID_DETECT_CONFIG_6  = 5,
    LIQUID_DETECT_CONFIG_7  = 6,
    LIQUID_DETECT_CONFIG_8  = 7,
    LIQUID_DETECT_CONFIG_9  = 8,
    LIQUID_DETECT_CONFIG_10 = 9,
}LIQUID_DETECT_CONFIG;

//探液段落
typedef enum LIQUID_DETECT_SEGMENT
{
    LIQUID_DETECT_SEGMENT_HIGH          = 0X00,//高
    LIQUID_DETECT_SEGMENT_LOW           = 0x01,//低
}LIQUID_DETECT_SEGMENT;

//移液针液面探测结果
typedef enum LIQUID_NEEDLE_LIQUID_DETECT_RESULT{
    LIQUID_NEEDLE_LIQUID_DETECT_FAILE    = 0, //移液针液面探测失败
    LIQUID_NEEDLE_LIQUID_DETECT_SUCCEED  = 1, //移液针液面探测成功
}LIQUID_NEEDLE_LIQUID_DETECT_RESULT;

//CAN2指令的配置信息
typedef struct CMD_CAN_CONFIG_UNIT
{
    CAN_MASTER_SUB_BOARD targetBoardID;//指令的目标板号
    CAN_MASTER_CHANNEL channelNo;//指令的目标通道号
    uint16_t sendDataLength;//指令发出的数据长度
    uint32_t sendDataTimeOutMs;//指令运行超时时间,单位毫秒
    uint16_t needRespDataMinLength;//指令需要接收的最小数据长度
    CAN_DATA_FLAG respDataFlag;//需要接收到的数据标识
}CMD_CAN_CONFIG_UNIT;

//初始化一个发送接收结构体
void CanSubBoardBase_CreateSendBuffer(CAN_MASTER_SEND_CMD* canSendCmdPtr,CAN_MASTER_READ_DATA* canReadDataPtr,uint32_t cmd,const CMD_CAN_CONFIG_UNIT* configArrayPtr);

//发送指令并初步解析结果
LH_ERR CanSubBoardBase_SendAndProcRespBase(CAN_MASTER_SEND_CMD* canSendCmdPtr,CAN_MASTER_READ_DATA* canReadDataPtr,const CMD_CAN_CONFIG_UNIT* configArrayPtr);

#endif
