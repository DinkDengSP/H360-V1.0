#ifndef __LIQUID_PARAM_CONFIG_H_
#define __LIQUID_PARAM_CONFIG_H_

typedef enum LIQUID_ENABLE_MODE
{
    LIQUID_ENABLE_MODE_DISABLE = 0,
    LIQUID_ENABLE_MODE_ENABLE_1 = 1,
    LIQUID_ENABLE_MODE_ENABLE_2 = 2,
    LIQUID_ENABLE_MODE_ENABLE_3 = 3,
    LIQUID_ENABLE_MODE_ENABLE_4 = 4,
    LIQUID_ENABLE_MODE_ENABLE_5 = 5,
    LIQUID_ENABLE_MODE_ENABLE_6 = 6,
    LIQUID_ENABLE_MODE_ENABLE_7 = 7,
    LIQUID_ENABLE_MODE_ENABLE_8 = 8,
    LIQUID_ENABLE_MODE_ENABLE_9 = 9,
    LIQUID_ENABLE_MODE_ENABLE_10 = 10,
}LIQUID_ENABLE_MODE;

//检测阈值
//杯口到杯中部的探液阈值
#define THRESHOLD_VALUE_LIMIT1                   2000
//杯中部到底部的探液阈值
#define THRESHOLD_VALUE_LIMIT2                   1000
#define THRESHOLD_VALUE_LIMIT3                   1000
#define THRESHOLD_VALUE_LIMIT4                   900
#define THRESHOLD_VALUE_LIMIT5                   800
#define THRESHOLD_VALUE_LIMIT6                   700
#define THRESHOLD_VALUE_LIMIT7                   600
#define THRESHOLD_VALUE_LIMIT8                   500
#define THRESHOLD_VALUE_LIMIT9                   400
#define THRESHOLD_VALUE_LIMIT10                  300

//阈值触发探液的触发次数
#define THRESHOLD_TRIG_COUNT_LIMIT              5

//检测斜率
//杯口到杯中部的探液斜率
#define SLOPE_VALUE_LIMIT1                       500
//杯口到杯底部的探液斜率
#define SLOPE_VALUE_LIMIT2                       300
#define SLOPE_VALUE_LIMIT3                       300
#define SLOPE_VALUE_LIMIT4                       200
#define SLOPE_VALUE_LIMIT5                       190
#define SLOPE_VALUE_LIMIT6                       180
#define SLOPE_VALUE_LIMIT7                       170
#define SLOPE_VALUE_LIMIT8                       160
#define SLOPE_VALUE_LIMIT9                       150
#define SLOPE_VALUE_LIMIT10                      140
//斜率检测到触发探液的检测次数
#define SLOPE_TRIG_COUNT_LIMIT                  4
//信号保持时间
#define TIME_MS_SIGNAL_LIQUID_HOLD             200

//滤波下线,低于某个值的信号认为是毛刺,不处理
#define FILTER_LIMIT_LOW                            200
//打开探液的时候,基准电压下限
#define BASE_VOLTAGE_LOW_LIMIT                      2000
//打开探液的时候,基准电压上限
#define BASE_VOLTAGE_HIGH_LIMIT                     3000

//软件版本号
#define LIQUID_BOARD_MAIN_VERSION                       1
#endif




