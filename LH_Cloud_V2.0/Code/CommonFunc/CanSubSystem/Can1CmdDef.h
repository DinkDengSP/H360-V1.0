/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-21 08:55:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-08 10:34:25
 *FilePath: \Appd:\SVN_Local\02-Development\Software\H360_STM32\LH_RAIL_V2.3\Code\CommonFunc\Can1Protocol\Can1CmdDef.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN1_CMD_DEF_H_
#define __CAN1_CMD_DEF_H_
#include "BoardInc.h"

//CAN1支持的指令列表
typedef enum CAN1_CMD_DEF
{
    /*****************************TEST_FLOW代表正常运行中位机下发的指令*****************************************************/
    CAN1_CMD_TEST_FLOW_SOFT_EMERGENCY_STOP                              = 0X0002,//运行时软件急停
    CAN1_CMD_TEST_FLOW_REPORT_ERR                                       = 0X0003,//运行时报警指令
    //在线升级程序
    CAN1_CMD_TEST_FLOW_CMD_CELL_UPDATA_START                            = 0x000F,//开始升级,升级信息
    CAN1_CMD_TEST_FLOW_CMD_CELL_UPDATA_DATA                             = 0x0010,//升级数据
    CAN1_CMD_TEST_FLOW_CMD_CELL_UPDATA_END                              = 0x0011,//升级结束，程序CRC校验码
    CAN1_CMD_TEST_FLOW_CMD_CELL_UPDATA_PREPARE                          = 0x0012,//升级准备，跳转Boot等
    CAN1_CMD_TEST_FLOW_CMD_CELL_VERSION                                 = 0x001D,//读取版本号
    CAN1_CMD_TEST_FLOW_CMD_CELL_BOOT_VERSION                            = 0x001E,//读取Boot版本号
    CAN1_CMD_TEST_FLOW_CMD_CELL_PROGRAM_JUMP                            = 0x0045,//程序跳转
    //基本调试指令
    CAN1_CMD_CLOUD_BASE_PING                                            = 0X0700,//PING 服务器测试
    CAN1_CMD_CLOUD_BASE_QUERY_MANUFACTURER_NAME                         = 0X0701,//获取模块厂商信息
    CAN1_CMD_CLOUD_BASE_QUERY_MODULE_TYPE                               = 0X0702,//获取模块类型信息
    CAN1_CMD_CLOUD_BASE_QUERY_SOFTWARE_VERSION                          = 0X0703,//获取模块软件版本信息
    CAN1_CMD_CLOUD_BASE_QUERY_MODULE_IMEI                               = 0X0704,//获取模块入网许可
    CAN1_CMD_CLOUD_BASE_QUERY_SIM_IMSI                                  = 0X0705,//获取SIM卡IMSI串号
    CAN1_CMD_CLOUD_BASE_QUERY_SIM_ICCID                                 = 0X0706,//获取SIM卡ICCID识别号
    CAN1_CMD_CLOUD_BASE_QUERY_NET_FULL_NAME                             = 0X0707,//获取当前模块连接网络全名
    CAN1_CMD_CLOUD_BASE_QUERY_NET_SHORT_NAME                            = 0X0708,//获取当前模块连接网络短名
    CAN1_CMD_CLOUD_BASE_QUERY_NET_PROVIDE                               = 0X0709,//获取网络服务商名称
    CAN1_CMD_CLOUD_BASE_QUERY_NET_TYPE                                  = 0X070A,//获取当前网络制式
    CAN1_CMD_CLOUD_BASE_QUERY_NET_BAUD                                  = 0X070B,//获取当前网络波段
    CAN1_CMD_CLOUD_BASE_QUERY_BASE_STATION_LAC                          = 0X070C,//获取LAC基站区域码
    CAN1_CMD_CLOUD_BASE_QUERY_BASE_STATION_CID                          = 0X070D,//获取CID基站编号
    CAN1_CMD_CLOUD_BASE_QUERY_SIGNAL_STRENGTH                           = 0X070E,//获取当前信号强度与误码率
    CAN1_CMD_CLOUD_BASE_QUERY_NET_STATE                                 = 0X070F,//查看当前模块网络是否正常
    CAN1_CMD_CLOUD_BASE_ENABLE_GPRS                                     = 0X0710,//打开设备GPRS连接
    CAN1_CMD_CLOUD_BASE_MODULE_RESTART                                  = 0X0711,//通讯设备重启
    //网络通讯指令
    CAN1_CMD_CLOUD_NET_CONNECT                                          = 0X0720,//设备连接网络
    CAN1_CMD_CLOUD_NET_DISCONNECT                                       = 0X0721,//设备断开网络连接
    CAN1_CMD_CLOUD_NET_SEND_WITHOUT_CONNECT                             = 0X0722,//设备发送一包数据
    CAN1_CMD_CLOUD_NET_READ_DATA                                        = 0X0723,//设备读取一次数据
    CAN1_CMD_CLOUD_NET_SEND_RECV_WITH_CONNECT                           = 0X0724,//设备集成发送并接收数据
}CAN1_CMD_DEF;

//错误代码的错误级别
typedef enum CAN1_REPORT_ERR_LEVEL
{
    CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP    = 1,//急停级报警
    CAN1_REPORT_ERR_LEVEL_NORMAL_STOP       = 2,//停止级报警
    CAN1_REPORT_ERR_LEVEL_WARNING           = 3,//注意级报警
    CAN1_REPORT_ERR_LEVEL_NORMAL            = 4,//常规报警
}CAN1_REPORT_ERR_LEVEL;

//错误代码
typedef enum CAN1_REPORT_ERR_CODE
{
    CAN1_REPORT_OK                                      = 0X0000,//无错误
    //基本调试指令
    CAN1_REPORT_QUERY_MANUFACTURER_NAME_ERR             = 0x2000,//获取模块厂商信息
    CAN1_REPORT_QUERY_MODULE_TYPE_ERR                   = 0X2001,//获取模块类型信息
    CAN1_REPORT_QUERY_SOFTWARE_VERSION_ERR              = 0X2002,//获取模块软件版本信息
    CAN1_REPORT_QUERY_MODULE_IMEI_ERR                   = 0X2003,//获取模块入网许可
    CAN1_REPORT_QUERY_SIM_IMSI_ERR                      = 0X2004,//获取SIM卡IMSI串号
    CAN1_REPORT_QUERY_SIM_ICCID_ERR                     = 0X2005,//获取SIM卡ICCID识别号
    CAN1_REPORT_QUERY_NET_FULL_NAME_ERR                 = 0X2006,//获取当前模块连接网络全名
    CAN1_REPORT_QUERY_NET_SHORT_NAME_ERR                = 0X2007,//获取当前模块连接网络短名
    CAN1_REPORT_QUERY_NET_PROVIDE_ERR                   = 0X2008,//获取网络服务商名称
    CAN1_REPORT_QUERY_NET_TYPE_ERR                      = 0X2009,//获取当前网络制式
    CAN1_REPORT_QUERY_NET_BAUD_ERR                      = 0X200A,//获取当前网络波段
    CAN1_REPORT_QUERY_BASE_STATION_LAC_ERR              = 0X200B,//获取LAC基站区域码
    CAN1_REPORT_QUERY_BASE_STATION_CID_ERR              = 0X200C,//获取CID基站编号
    CAN1_REPORT_QUERY_SIGNAL_STRENGTH_ERR               = 0X200D,//获取当前信号强度与误码率
    CAN1_REPORT_QUERY_NET_STATE_ERR                     = 0X200E,//查看当前模块网络是否正常
    CAN1_REPORT_ENABLE_GPRS_ERR                         = 0X200F,//打开设备GPRS连接
    CAN1_REPORT_MODULE_RESTART_ERR                      = 0X2010,//通讯设备重启
    CAN1_REPORT_PING_ERR                                = 0X2011,//PING 服务器测试
    //网络通讯指令
    CAN1_REPORT_NET_PARAM_ERR                           = 0X2100,//参数错误
    CAN1_REPORT_NET_CONNECT                             = 0X2101,//设备连接网络
    CAN1_REPORT_NET_DISCONNECT                          = 0X2102,//设备断开网络连接
    CAN1_REPORT_NET_SEND_WITHOUT_CONNECT                = 0X2103,//设备发送一包数据
    CAN1_REPORT_NET_READ_DATA                           = 0X2104,//设备读取一次数据
    CAN1_REPORT_NET_SEND_RECV_WITH_CONNECT              = 0X2105,//设备集成发送并接收数据
}CAN1_REPORT_ERR_CODE;

#endif




