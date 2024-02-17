/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-17 10:02:42
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-03 13:48:37
 *FilePath: \LH_TC_APP_V2.3\TaskMain\ServiceSupport\ServiceUtil\ServiceUtilParamDataType.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_UTIL_PARAM_DATA_TYPE_H_
#define __SERVICE_UTIL_PARAM_DATA_TYPE_H_
#include "BoardInc.h"

//标志位,标识系统参数状态
#define SYSTEM_CONFIG_FLAG              0XAA5555AA
//系统参数起始空间
#define SYSTEM_SERVICE_PARAM_ADDR       0X00000000

//系统配置
typedef struct SERVICE_PARAM_CONFIG
{
    FunctionalState plateReactionEnable;            //是否使能反应盘控温
    FunctionalState plateWashEnable;                //是否使能清洗盘控温
    FunctionalState plateReagentEnable;             //是否使能试剂盘控温
    FunctionalState envirEnable;                    //是否使能环境温度检测与相关控制
    
    FunctionalState plateReactionCurrentDetectEnable;   //是否使能反应盘电流检测
    uint16_t plateReactionCurrentDetectMin;             //反应盘电流检测下限值
    uint16_t plateReactionCurrentDetectMax;             //反应盘电流检测上限值

    FunctionalState plateWashCurrentDetectEnable;       //是否使能清洗盘电流检测
    uint16_t plateWashCurrentDetectMin;                 //清洗盘电流检测下限值
    uint16_t plateWashCurrentDetectMax;                 //清洗盘电流检测上限值

    FunctionalState plateReagent1CurrentDetectEnable;   //是否使能试剂盘电流检测1
    uint16_t plateReagent1CurrentDetectMin;             //试剂盘电流检测1电流检测下限值
    uint16_t plateReagent1CurrentDetectMax;             //试剂盘电流检测1电流检测上限值
    
    FunctionalState plateReagent2CurrentDetectEnable;   //是否使能试剂盘电流检测2
    uint16_t plateReagent2CurrentDetectMin;             //试剂盘电流检测2电流检测下限值
    uint16_t plateReagent2CurrentDetectMax;             //试剂盘电流检测2电流检测上限值

    FunctionalState plateReagent3CurrentDetectEnable;   //是否使能试剂盘电流检测3
    uint16_t plateReagent3CurrentDetectMin;             //试剂盘电流检测3电流检测下限值
    uint16_t plateReagent3CurrentDetectMax;             //试剂盘电流检测3电流检测上限值

    FunctionalState plateReagent4CurrentDetectEnable;   //是否使能试剂盘电流检测4
    uint16_t plateReagent4CurrentDetectMin;             //试剂盘电流检测4电流检测下限值
    uint16_t plateReagent4CurrentDetectMax;             //试剂盘电流检测4电流检测上限值
    uint32_t reserve[50];                               //保留参数
}SERVICE_PARAM_CONFIG;
//配置信息尺寸
#define SIZE_SERVICE_PARAM_CONFIG            (sizeof(SERVICE_PARAM_CONFIG)/sizeof(uint8_t))

//用户参数
typedef struct SERVICE_PARAM_USER
{
    //参数配置标识
    uint32_t ConfigFlag;//4字节对齐
    //反应盘
    float plateReactionTargetTemp;          //目标温度
    float plateReactionKP;                  //PID控温参数 比例
    float plateReactionKI;                  //PID控温参数 积分
    float plateReactionKD;                  //PID控温参数 微分
    float plateReactionTempHighThreshold;   //温度上限阈值,超过该数据,最大功率降温
    float plateReactionTempLowThreshold;    //温度下限阈值,低于该数据,最大功率升温
    float plateReactionMaxPower;            //输出最大功率
    float plateReactionMinPower;            //输出最小功率
    float plateReactionProtectHighTemp;     //保护温度上限,高于该温度不控温,报警
    float plateReactionProtectLowTemp;      //保护温度下限,低于该温度不控温,报警
    uint32_t plateReactionKI_TimeSetSecond; //积分时间设定
    uint32_t plateReactionKD_TimeSetSecond; //微分时间设定
    float plateReactionReserved[30];        //预留空间
    //清洗盘
    float plateWashTargetTemp;              //目标温度
    float plateWashKP;                      //PID控温参数 比例
    float plateWashKI;                      //PID控温参数 积分
    float plateWashKD;                      //PID控温参数 微分
    float plateWashTempHighThreshold;       //温度上限阈值
    float plateWashTempLowThreshold;        //温度下限阈值
    float plateWashMaxPower;                //输出最大功率
    float plateWashMinPower;                //输出最小功率
    float plateWashProtectHighTemp;         //保护温度上限
    float plateWashProtectLowTemp;          //保护温度下限
    uint32_t plateWashKI_TimeSetSecond;     //积分时间设定
    uint32_t plateWashKD_TimeSetSecond;     //微分时间设定
    float plateWashReserved[30];            //预留空间
    //试剂盘
    float plateReagentTargetTemp;           //目标温度
    float plateReagentMaxPower;             //输出最大功率
    float plateReagentMinPower;             //输出最小功率
    float plateReagentProtectHighTemp;      //保护温度上限
    float plateReagentProtectLowTemp;       //保护温度下限
    uint32_t plateReagentTempCtrlType;       //试剂盘温控控制类型
    float plateReagentReserved[29];         //预留空间
    //环境温度
    float envirMaxLimit;                    //环境温度上限温度
    float envirMinLimit;                    //环境温度下限温度
    float envirTempOpenFan;                 //打开风扇的温度
    float envirReserved[30];                //预留空间
    //板上温度
    float boardTempMaxLimit;                //板上温度上限
    float boardTempMinLimit;                //板上温度下限
    float boardTempReserved[30];            //预留空间
}SERVICE_PARAM_USER;
//配置信息尺寸
#define SIZE_SERVICE_PARAM_USER            (sizeof(SERVICE_PARAM_USER)/sizeof(uint8_t))

//系统参数
typedef struct SERVICE_PARAM
{
    //系统配置文件
    SERVICE_PARAM_CONFIG paramConfig;
    //用户配置文件
    SERVICE_PARAM_USER paramUser;
}SERVICE_PARAM;
//配置信息尺寸
#define SIZE_SERVICE_PARAM            (sizeof(SERVICE_PARAM)/sizeof(uint8_t))


#endif





