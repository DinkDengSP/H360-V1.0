/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-14 13:52:48 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-14 14:03:37 +0800
************************************************************************************************/ 
#ifndef __MCU_SPI_UNIVERSAL_H_
#define __MCU_SPI_UNIVERSAL_H_
#include "CoreUtil.h"
#include "MCU_Config.h"

//SPI速度设定 PLLQ 100M输入主频
typedef enum MCU_SPI_123_CLK
{
    SPI_123_BAUD_RATE_50M       = SPI_BAUDRATEPRESCALER_2,
    SPI_123_BAUD_RATE_25M       = SPI_BAUDRATEPRESCALER_4,
    SPI_123_BAUD_RATE_12_5M     = SPI_BAUDRATEPRESCALER_8,
    SPI_123_BAUD_RATE_6_25_M    = SPI_BAUDRATEPRESCALER_16,
    SPI_123_BAUD_RATE_3_125_M   = SPI_BAUDRATEPRESCALER_32,
    SPI_123_BAUD_RATE_1_6_M     = SPI_BAUDRATEPRESCALER_64,
    SPI_123_BAUD_RATE_800K      = SPI_BAUDRATEPRESCALER_128,
    SPI_123_BAUD_RATE_400K      = SPI_BAUDRATEPRESCALER_256,
}MCU_SPI_123_CLK;

//SPI速度设定 APB2 100M输入主频
typedef enum MCU_SPI_45_CLK
{
    SPI_45_BAUD_RATE_50M       = SPI_BAUDRATEPRESCALER_2,
    SPI_45_BAUD_RATE_25M       = SPI_BAUDRATEPRESCALER_4,
    SPI_45_BAUD_RATE_12_5M     = SPI_BAUDRATEPRESCALER_8,
    SPI_45_BAUD_RATE_6_25_M    = SPI_BAUDRATEPRESCALER_16,
    SPI_45_BAUD_RATE_3_125_M   = SPI_BAUDRATEPRESCALER_32,
    SPI_45_BAUD_RATE_1_6_M     = SPI_BAUDRATEPRESCALER_64,
    SPI_45_BAUD_RATE_800K      = SPI_BAUDRATEPRESCALER_128,
    SPI_45_BAUD_RATE_400K      = SPI_BAUDRATEPRESCALER_256,
}MCU_SPI_45_CLK;

//SPI速度设定 APB4 100M输入主频
typedef enum MCU_SPI_6_CLK
{
    SPI_6_BAUD_RATE__50M       = SPI_BAUDRATEPRESCALER_2,
    SPI_6_BAUD_RATE__25M       = SPI_BAUDRATEPRESCALER_4,
    SPI_6_BAUD_RATE__12_5M     = SPI_BAUDRATEPRESCALER_8,
    SPI_6_BAUD_RATE__6_25_M    = SPI_BAUDRATEPRESCALER_16,
    SPI_6_BAUD_RATE__3_125_M   = SPI_BAUDRATEPRESCALER_32,
    SPI_6_BAUD_RATE__1_6_M     = SPI_BAUDRATEPRESCALER_64,
    SPI_6_BAUD_RATE__800K      = SPI_BAUDRATEPRESCALER_128,
    SPI_6_BAUD_RATE__400K      = SPI_BAUDRATEPRESCALER_256,
}MCU_SPI_6_CLK;

//SPI信号极性
typedef enum MCU_SPI_CPOL
{
    MCU_SPI_CPOL_HIGH   = SPI_POLARITY_HIGH,
    MCU_SPI_CPOL_LOW    = SPI_POLARITY_LOW,
}MCU_SPI_CPOL;

//SPI采样点设置
typedef enum MCU_SPI_CPHA
{
    MCU_SPI_CPHA_1EDGE = SPI_PHASE_1EDGE,
    MCU_SPI_CPHA_2EDGE = SPI_PHASE_2EDGE,
}MCU_SPI_CPHA;

//SPI传输状态
typedef enum SPI_TRANS_STATE
{
    SPI_TRANS_STATE_IDLE        = 0x00,//空闲状态
    SPI_TRANS_STATE_START       = 0x01,//启动状态
    SPI_TRANS_STATE_END         = 0x02,//结束状态
    SPI_TRANS_STATE_ERROR       = 0x03,//错误状态
}SPI_TRANS_STATE;

//SPI123通讯配置
typedef struct SPI123_CONFIG
{
    MCU_SPI_123_CLK baudRate;
    MCU_SPI_CPOL cpolConfig;
    MCU_SPI_CPHA cphaConfig;
}SPI123_CONFIG;

//SPI45通讯配置
typedef struct SPI45_CONFIG
{
    MCU_SPI_45_CLK baudRate;
    MCU_SPI_CPOL cpolConfig;
    MCU_SPI_CPHA cphaConfig;
}SPI45_CONFIG;

//SPI通讯,单字节最长时间
#define TIME_OUT_MS_SPI_SINGLE_BYTE                10
//SPI空数据,用于不想读取的时候
#define SPI_PTR_NULL                               NULL


#endif

