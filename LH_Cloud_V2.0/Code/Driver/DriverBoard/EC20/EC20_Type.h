/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-09-04 09:40:29
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-10-23 11:33:10
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __EC20_TYPE_H_
#define __EC20_TYPE_H_
#include "string.h"
#include "stdio.h"
#include "MCU_Inc.h"

//EC20电源标识
typedef enum EC20_POWER_MODE
{
    EC20_POWER_OFF,
    EC20_POWER_ON,
}EC20_POWER_MODE;

//EC20使能标志
typedef enum EC20_FUNC
{
    EC20_FUNC_DISABLE   = 0,
    EC20_FUNC_ENABLE    = 1,
}EC20_FUNC;

//串口数据的标识
typedef enum EC20_UART_FLAG
{
    EC20_UART_FLAG_NONE     = 0X00,//没接收到标志
    EC20_UART_FLAG_OK       = 0X01,//返回完成
    EC20_UART_FLAG_ERROR    = 0X02,//返回错误
}EC20_UART_FLAG;

//连接状态
typedef enum EC20_CONNECT_STATE
{
    EC20_CONNECT_STATE_INIT     = 0,
    EC20_CONNECT_STATE_OPENING  = 1,
    EC20_CONNECT_STATE_CONNECT  = 2,
    EC20_CONNECT_STATE_LISTING  = 3,
    EC20_CONNECT_STATE_CLOSEING = 4,
}EC20_CONNECT_STATE;

//模块的各类信息字符串
#define LENGTH_EC20_CODE_STRING     50
//EC20的模块状态讯息
typedef struct EC20_MESSAGE
{
    uint8_t manufacturerString[LENGTH_EC20_CODE_STRING];//模块厂家信息
    uint8_t moduleTypeString[LENGTH_EC20_CODE_STRING];//模块类型信息
    uint8_t softwareVersion[LENGTH_EC20_CODE_STRING];//软件版本讯息
    uint8_t imeiCodeString[LENGTH_EC20_CODE_STRING];//模块入网许可信息
    uint8_t imsiCodeString[LENGTH_EC20_CODE_STRING];//SIM卡IMSI串号讯息
    uint8_t iccidCodeString[LENGTH_EC20_CODE_STRING];//SIM卡ICCID讯息
    uint8_t fullNetWorkNameString[LENGTH_EC20_CODE_STRING];//网络全名
    uint8_t shortNetworkNameString[LENGTH_EC20_CODE_STRING];//网络短名
    uint8_t netServiceProviderNameString[LENGTH_EC20_CODE_STRING];//网络提供商名称
    uint8_t networkModeString[LENGTH_EC20_CODE_STRING];//网络制式
    uint8_t networkBandString[LENGTH_EC20_CODE_STRING];//网络制式选择的波段
    uint8_t baseStationCodeStringLAC[LENGTH_EC20_CODE_STRING];//基站位置区域码
    uint8_t baseStationCodeStringCID[LENGTH_EC20_CODE_STRING];//基站编号
    int singalStrength;//信号强度
    int errorValue;//误码率
}EC20_MESSAGE;

//EC20模块状态标识码
typedef enum EC20_RESULT
{
    EC20_RESULT_OK                      = 0X0000,
    EC20_RESULT_CMD_ERR                 = 0X0001,//发送指令报错
    EC20_RESULT_RECV_ZERO               = 0X0002,//指令没有返回数据
    EC20_RESULT_RECV_NOT_CONTAIN_SUB_STR= 0X0003,//返回数据不包含子串
    EC20_RESULT_AT_NOT_RESP             = 0X0004,//AT指令收不到回复
    EC20_RESULT_AT_SAVE_NOT_RESP        = 0X0005,//保存波特率配置收不到返回
    EC20_RESULT_MODULE_RINGING          = 0X0006,//正在震铃
    EC20_RESULT_MODULE_CALLING_OR_HOLD  = 0X0007,//正在打电话或者正在拨号
    EC20_RESULT_MODULE_NOT_RDY          = 0X0008,//模块正在忙
    EC20_RESULT_SIM_CARD_NOT_RDY        = 0X0009,//SIM卡还没准备好
    EC20_RESULT_SIM_CARD_NOT_EXIST      = 0X000A,//SIM卡未插入
    EC20_RESULT_SIM_CARD_NOT_INIT       = 0X000B,//SIM卡还没初始化完成,可能卡还没插入
    EC20_RESULT_NET_REGISTER_DENIED     = 0X000C,//网络注册被拒绝
    EC20_RESULT_NET_REGISTER_NOT_INIT   = 0X000D,//SIM卡没有注册网络
    EC20_RESULT_GPRS_ATTACH_FAILED      = 0X000E,//GPRS网络附着失败
    EC20_RESULT_GPRS_ATTACH_UNKNOW      = 0X000F,//GPRS网络附着未知错误
    EC20_RESULT_STR_FIND_ERR            = 0X0010,//字符串查找异常
    EC20_RESULT_STR_FORMAT              = 0X0011,//字符串格式错误
    EC20_RESULT_PING_BAIDU_FAIL         = 0X0012,//PING服务器失败
    EC20_RESULT_SOCKET_OPEN_EXIST       = 0X0013,//socket已经被打开了
    EC20_RESULT_SOCKET_CONNECT_TIME_OUT = 0X0014,//SOCKET建立超时
    EC20_RESULT_SOCKET_SEND_FAIL        = 0X0015,//发送失败
    EC20_RESULT_SOCKET_SEND_NO_PESP     = 0X0016,//发送无响应
    EC20_RESULT_SOCKET_SEND_TIME_OUT    = 0X0017,//发送超时
    EC20_RESULT_SOCKET_SEND_ERROR       = 0X0018,//发送错误
    EC20_RESULT_SOCKET_SEND_UNKNOW      = 0x0019,//未知错误
    EC20_RESULT_SOCKET_NOT_CONNECT      = 0X001A,//SCOKET未连接
}EC20_RESULT;




#endif





