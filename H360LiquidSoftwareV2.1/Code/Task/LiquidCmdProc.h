#ifndef __LIQUID_CMD_PROC_H_
#define __LIQUID_CMD_PROC_H_
#include "BaseInclude.h"

//主动上报基准值
#define AUTO_UPDATE_BASE_ADC                    0X00
//主动上报跟随值
#define AUTO_UPDATE_FOLLOW_ADC                  0X01
//主动上报滤波值
#define AUTO_UPDATE_FILTER_ADC                  0X02
//主动上报液面探测状态
#define AUTO_UPDATE_TRIG_FLAG                   0X03
//主动上报校准状态
#define AUTO_UPDATE_ADJUST_VALUE                0X04

//读取液面探测阈值
#define CMD_READ_THRESHOLD_VALUE                0X20
//读取液面探测阈值计数器
#define CMD_READ_THRESHOLD_COUNT_LIMIT          0X21
//读取液面探测斜率
#define CMD_READ_SLOPE_VALUE                    0X22
//读取液面探测斜率计数器
#define CMD_READ_SLOPE_COUNT_LIMIT              0X23
//读取液面探测信号保持时间
#define CMD_READ_SIGNAL_HOLD_MS                 0X25
//读取电子电阻值
#define CMD_READ_AD5272_RES                     0X24
//读取探液时最大基准电压
#define CMD_READ_LIQUID_MAX_BASE_VOL            0X26
//读取探液时最大针尖电压
#define CMD_READ_LIQUID_MAX_NEEDLE_VOL          0X27
//读取探液板版本
#define CMD_FUNC_READ_LIQUID_BOARD_VERSION      0X28

//写入电子电阻值
#define CMD_WRITE_AD5272_RES                    0X40

//基准值校准
#define CMD_FUNC_ADJUST_BASE                    0X60
//开关液面探测数据上传
#define CMD_FUNC_SWITCH_LIQUID_DATA_UPDATE      0X61
//开关液面探测使能
#define CMD_FUNC_SWITCH_LIQUID_ENABLE           0X62

//读取基准电压值
#define CMD_FUNC_READ_NEEDLE_BASE_VOL           0X65
//读取针尖电压值
#define LCMD_FUNC_READ_NEEDLE_CURRENT_VOL       0X66

//基准电压不对
#define LIQUID_DETECT_CHECK_BENCHMARK_50V               0X20
#define LIQUID_DETECT_CHECK_BENCHMARK_45_50V            0X21
#define LIQUID_DETECT_CHECK_BENCHMARK_40_45V            0X22
#define LIQUID_DETECT_CHECK_BENCHMARK_35_40V            0X23
#define LIQUID_DETECT_CHECK_BENCHMARK_30_35V            0X24
#define LIQUID_DETECT_CHECK_BENCHMARK_29_30V            0X25
#define LIQUID_DETECT_CHECK_BENCHMARK_20_21V            0X26
#define LIQUID_DETECT_CHECK_BENCHMARK_15_20V            0X27
#define LIQUID_DETECT_CHECK_BENCHMARK_10_15V            0X28
#define LIQUID_DETECT_CHECK_BENCHMARK_5_10V             0X29
#define LIQUID_DETECT_CHECK_BENCHMARK_0_5V              0X2A
#define LIQUID_DETECT_CHECK_BENCHMARK_0V                0X2B

//自动校准结果
typedef enum RESULT_ADJUST
{
    RESULT_ADJUST_OK = 0X00,//校准成功
    RESULT_ADJUST_MAX_NOT_4 = 0X01,//校准失败,电阻设置为最大基准电压依然不到4V
    RESULT_ADJUST_MIN_NOT_1 = 0X02,//校准失败,电阻设置为最小基准电压依然不为1V
    RESULT_ADJUST_FAIL = 0X03,//校准失败,无法达到2.5 +-0.1的状态
}RESULT_ADJUST;

//液面探液信号上传标志
extern volatile uint8_t liquidSensorUpdateFlag;
//液面探测使能标志
extern volatile LIQUID_ENABLE_MODE liquidDetectEnableFlag;

//串口中断回调函数
void LiquidUartRecvIntCallBack(uint8_t dat);

//初始化指令系统
void LiquidCmdProcInit(void);

//处理系统指令集
void LiquidCommandProcess(void);

//发送数据
void LiquidCmdSendSerialValue(uint8_t dataFlag,uint16_t value);

//保存打开探液过程中最大的基准电压和针尖电压
void liquidDetectSaveMaxBaseAndNeedleVol(void);
#endif






