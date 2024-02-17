/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-01 18:56:24
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-26 15:12:17
 *FilePath: \Code\CommonFunc\Can2SubCommBase\Can2SubBase.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_BASE_H_
#define __CAN2_SUB_BASE_H_
#include "Can2ProcMain.h"

//温控板板号
#define BOARD_ID_TC                                   CAN2_SUB_BOARD9


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
    NEEDLE_CLEAN_TIME_1 = 0x00,//长清洗
    NEEDLE_CLEAN_TIME_2 = 0x01,//短清洗
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
    LIQUID_DETECT_CONFIG_MAX = LIQUID_DETECT_CONFIG_10,
}LIQUID_DETECT_CONFIG;

//探液结果
typedef enum LIQUID_FUNC_RESULT
{
    LIQUID_FUNC_RESULT_OK               = 0X00,//探液功能正常打开
    LIQUID_FUNC_RESULT_OPEN_FAIL        = 0X01,//探液功能打开失败,打开探液针信号一直为高
    LIQUID_FUNC_RESULT_BASE_50_V        = 0x20,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_45_50_V     = 0x21,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_40_45_V     = 0x22,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_35_40_V     = 0x23,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_30_35_V     = 0x24,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_29_30_V     = 0x25,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_20_21_V     = 0x26,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_15_20_V     = 0x27,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_10_15_V     = 0x28,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_5_10_V      = 0x29,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_0_5_V       = 0x2A,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_0_V         = 0x2B,//液面探测基准电压异常
}LIQUID_FUNC_RESULT;

//移液针液面探测结果
typedef enum LIQUID_NEEDLE_LIQUID_DETECT_RESULT{
    LIQUID_NEEDLE_LIQUID_DETECT_FAILE    = 0, //移液针液面探测失败
    LIQUID_NEEDLE_LIQUID_DETECT_SUCCEED  = 1, //移液针液面探测成功
}LIQUID_NEEDLE_LIQUID_DETECT_RESULT;

//运行模式
typedef enum MODE_RUNNING
{
    MODE_DEBUG   = 0X00,//老化模式
    MODE_RELEASE = 0X01,//正常测试模式
}MODE_RUNNING;

//用于区分调用IPC的源头
typedef enum CMD_SOURCE_TEST_FLOW
{
    CMD_SOURCE_TEST_FLOW_MAIN   = 0X00,//主流程调用
    CMD_SOURCE_TEST_FLOW_SUB    = 0X01,//子流程调用
}CMD_SOURCE_TEST_FLOW;

//读取下位机版本
typedef enum READ_SOFTWARE_VERSION
{
    READ_SOFTWARE_VERSION_BOOT  = 0X00,//读取boot版本
    READ_SOFTWARE_VERSION_APP   = 0X01,//读取App版本
}READ_SOFTWARE_VERSION;

//探针时移液针电压
typedef struct LIQUID_DETECT_NEEDLE_VOL_UNIT{
    uint16_t liquidDetectBaseVol;
    uint16_t liquidDetectNeedleVol;
}LIQUID_DETECT_NEEDLE_VOL_UNIT;


//CAN2指令的配置信息
typedef struct CMD_CAN2_CONFIG_UNIT
{
    CAN2_SUB_BOARD targetBoardID;//指令的目标板号
    CAN2_CHANNEL channelNo;//指令的目标通道号
    uint16_t sendDataLength;//指令发出的数据长度
    uint32_t sendDataTimeOutMs;//指令运行超时时间,单位毫秒
    uint16_t needRespDataMinLength;//指令需要接收的最小数据长度
    CAN2_SUB_DATA_FLAG respDataFlag;//需要接收到的数据标识
}CMD_CAN2_CONFIG_UNIT;

//初始化一个发送接收结构体
void Can2SubBoard_CreateSendBuffer(CAN2_SEND_CMD* can2SendCmdPtr,CAN2_READ_DATA* can2ReadDataPtr,uint32_t cmd,const CMD_CAN2_CONFIG_UNIT* configArrayPtr);

//发送指令并初步解析结果
LH_ERR Can2SubBoard_SendAndProcRespBase(CAN2_SEND_CMD* can2SendCmdPtr,CAN2_READ_DATA* can2ReadDataPtr,const CMD_CAN2_CONFIG_UNIT* configArrayPtr);


#endif



