#ifndef __CORE_SUPPORT_H_
#define __CORE_SUPPORT_H_

//编程IDE对于本款芯片的支持
#include "iom8a.h"
#include "avr_macros.h"
#include "intrinsics.h"

typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned long   uint32_t;
typedef signed char     int8_t;
typedef signed short    int16_t;
typedef signed long     int32_t;

#ifndef NULL
#define NULL    ((void*)0)
#endif

/*	函数库通用类型定义
 *	符号标志,中断标志*/
typedef enum {RESET = 0, SET = 1} FlagStatus, ITStatus;

/*	某个特性使能或者禁止与否*/
typedef enum {DISABLE = 0, ENABLE = 1} FunctionalState;

/*	系统错误状态*/
typedef enum {ERROR = 0, SUCCESS = 1} ErrorStatus;

//端口号
typedef enum INOUT_PORT
{
    INOUT_PORT_B,
    INOUT_PORT_C,
    INOUT_PORT_D,
}INOUT_PORT;

//引脚编号
typedef enum INOUT_PIN
{
    INOUT_PIN_0,
    INOUT_PIN_1,
    INOUT_PIN_2,
    INOUT_PIN_3,
    INOUT_PIN_4,
    INOUT_PIN_5,
    INOUT_PIN_6,
    INOUT_PIN_7,
}INOUT_PIN;

//输入输出方向,输入时候默认为上拉
typedef enum INOUT_DIR
{
    INOUT_DIR_IN,
    INOUT_DIR_OUT,
}INOUT_DIR;

//当前引脚状态
typedef enum INOUT_LEVEL
{
    INOUT_LEVEL_LOW,
    INOUT_LEVEL_HIGH,
}INOUT_LEVEL;

//串口波特率定义
typedef enum UART_BPS
{
    UART_BPS_38400,//正常蓝牙通讯
    UART_BPS_250K,//有线通讯
}UART_BPS;

//串口回调函数
typedef void (*UartRecvCallBack)(uint8_t dat);

//adcT通道名称
typedef enum ADC_CHANNEL
{
    ADC_CHANNEL_BASE,//基础通道
    ADC_CHANNEL_TARGET,//经过信号处理的通道
}ADC_CHANNEL;


/*	主板晶振频率定义 内置时钟*/
#define BOARD_XTAL_FREQ_HZ	8000000


#endif



