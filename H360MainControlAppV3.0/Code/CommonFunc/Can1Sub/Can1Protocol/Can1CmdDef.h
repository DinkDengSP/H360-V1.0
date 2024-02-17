/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-21 08:55:09
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-11 13:39:55
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
    CAN1_CMD_TEST_FLOW_MACHINE_RESET                                    = 0X0004,//轨道整机复位
    CAN1_CMD_SYSTEM_STATE_HEART                                         = 0X0005,//系统状态以及心跳上传
    //在线升级程序
    CAN1_CMD_TEST_FLOW_CELL_CELL_BOOT_VERSION                           = 0x0006,//读取Boot版本号
    CAN1_CMD_TEST_FLOW_CELL_VERSION                                     = 0x0007,//读取版本号
    CAN1_CMD_TEST_FLOW_CELL_UPDATA_PREPARE                              = 0x0008,//升级准备，跳转Boot等
    CAN1_CMD_TEST_FLOW_CMD_CELL_UPDATA_START                            = 0x0009,//开始升级,升级信息
    CAN1_CMD_TEST_FLOW_CMD_CELL_UPDATA_DATA                             = 0x000A,//升级数据
    CAN1_CMD_TEST_FLOW_CELL_UPDATA_END                                  = 0x000B,//升级结束，程序CRC校验码
    CAN1_CMD_TEST_FLOW_CELL_TEST_PARAM                                  = 0x0033,//测试参数,boot模式下无用,防止中位机卡住返回ACK响应
    CAN1_CMD_TEST_FLOW_CMD_PROGRAM_JUMP                                 = 0x07FA,//程序跳转
    CAN1_CMD_ACK                                                        = 0X07FC,//回传ACK讯息
}CAN1_CMD_DEF;

//错误代码的错误级别
typedef enum CAN1_REPORT_ERR_LEVEL
{
    CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP    = 1,//急停级报警
    CAN1_REPORT_ERR_LEVEL_NORMAL_STOP       = 2,//加样暂停级报警
    CAN1_REPORT_ERR_LEVEL_WARNING           = 3,//注意级报警
    CAN1_REPORT_ERR_LEVEL_NORMAL            = 4,//常规报警
}CAN1_REPORT_ERR_LEVEL;

//错误代码
typedef enum CAN1_REPORT_ERR_CODE
{
    CAN1_REPORT_OK                                      = 0X0000,//无错误
}CAN1_REPORT_ERR_CODE;


//CAN1回传系统状态
typedef enum CAN1_SYSTEM_STATE
{
    CAN1_SYSTEM_STATE_IDLE                          = 0, //无效
    CAN1_SYSTEM_STATE_STANDBY                       = 1, //待机
    CAN1_SYSTEM_STATE_RESET                         = 2, //复位
    CAN1_SYSTEM_STATE_TEST                          = 3, //测试
    CAN1_SYSTEM_STATE_EMSTOP                        = 4, //急停
    CAN1_SYSTEM_STATE_SAMPLE_PAUSE                  = 5, //加样暂停
    CAN1_SYSTEM_STATE_SAMPLE_STOP                   = 6, //加样停止
    CAN1_SYSTEM_STATE_RACK_PAUSE                    = 7, //进架暂停
    CAN1_SYSTEM_STATE_RACK_STOP                     = 8, //进架停止
    CAN1_SYSTEM_STATE_RESET_NG                      = 9, //复位失败
    CAN1_SYSTEM_STATE_SIMULATE                      = 10,//机械动作检查
    CAN1_SYSTEM_STATE_CUP_BLANK                     = 11,//杯空白
    CAN1_SYSTEM_STATE_CHECK_LIGHT                   = 12,//光通量检查
    CAN1_SYSTEM_STATE_LIGHT_STABILIZE               = 13,//光稳定性检查
    CAN1_SYSTEM_STATE_SCAN_REAGENT_BAR              = 14,//试剂条码扫描
    CAN1_SYSTEM_STATE_SCAN_SAMPLE_BAR               = 15,//样本条码扫描
    CAN1_SYSTEM_STATE_CLEAN_CUP                     = 16,//清洗反应杯
    CAN1_SYSTEM_STATE_WASH_WARMSINK                 = 17,//清洗恒温槽
    CAN1_SYSTEM_STATE_WASH_SAMPLE                   = 18,//清洗样本针
    CAN1_SYSTEM_STATE_WASH_WATERTANK                = 19,//清洗水箱
    CAN1_SYSTEM_STATE_INJECT_EXHAUST                = 20,//注射泵排气
    CAN1_SYSTEM_STATE_REAGENT_QUANTITY              = 27,//试剂余量扫描
    CAN1_SYSTEM_STATE_WASH_PIPE_EXHAUST             = 28,//清洗液管路排气
    CAN1_SYSTEM_STATE_ABANDON_CUP                   = 35,//抛杯
    CAN1_SYSTEM_STATE_RECYCLE_RACK                  = 36,//样本架回收
    CAN1_SYSTEM_STATE_PRIME                         = 38,//探针灌注
    CAN1_SYSTEM_STATE_M_SEPARATE_AUTO               = 44,//磁分离测试
    CAN1_SYSTEM_STATE_M_SEPARATE                    = 45,//手动磁分离测试
    CAN1_SYSTEM_STATE_MANUAL_TEST                   = 46,//人工测值
    CAN1_SYSTEM_STATE_DARK_TEST                     = 47,//暗计数
    CAN1_SYSTEM_STATE_NEEDLE_CLEAN_OUT              = 48,//清洁针外壁
    CAN1_SYSTEM_STATE_PURE_WATER_MAINTAIN           = 49,//纯水清洗维护
    CAN1_SYSTEM_STATE_DARK_ABSORB                   = 50,//暗计数(灌注)
    CAN1_SYSTEM_STATE_SAMPLE_ACCURACY               = 51,//样本针精度测试
    CAN1_SYSTEM_STATE_NEEDLE_WASH_ACCURACY          = 53,//清洗针精度测试
    CAN1_SYSTEM_STATE_TEST_TO_STANDBY               = 54,//测试完成,准备待机
    CAN1_SYSTEM_STATE_PERFORMANCE_TEST              = 55,//性能测试
    CAN1_SYSTEM_STATE_WAIT_SAMPLE_PAUSE             = 56,//加样暂停,等待加试剂完成
    CAN1_SYSTEM_STATE_TEST_REAGENT_QUANTITY         = 57,//测试中试剂余量扫描
    CAN1_SYSTEM_STATE_TEST_REAGENT_BARCODE          = 58,//测试中试剂条码扫描
    CAN1_SYSTEM_STATE_LIGHT_CHECK                   = 59,//光检测试
    CAN1_SYSTEM_STATE_NEEDLE_ACCURACY               = 60,//针精度测试
    CAN1_SYSTEM_STATE_BACK_TEST                     = 61,//本底测试
    CAN1_SYSTEM_STATE_EMPTY_CUP_TEST                = 62,//空杯测试
    CAN1_SYSTEM_STATE_A_NEEDLE_ACCURACY             = 63,//A液针精度测试
    CAN1_SYSTEM_STATE_B_NEEDLE_ACCURACY             = 64,//B液针精度测试
    CAN1_SYSTEM_STATE_MACHINE_RESET                 = 65,//机械复位
    CAN1_SYSTEM_STATE_PLATE_REAGENT_MIX             = 66,//试剂盘试剂混匀
    CAN1_SYSTEM_STATE_SAMPLE_MAINTAIN               = 67,//样本针维护
    CAN1_SYSTEM_STATE_POWERON                       = 68,//上电后初始化的状态,等待下位机更新各模块状态
    CAN1_SYSTEM_STATE_TIMEOUT                       = 69,//心跳超时
    CAN1_SYSTEM_STATE_ERROR_STOP                    = 70,//异常停止
    CAN1_SYSTEM_STATE_REAGENT_DETECT                = 71,//试剂余量探测,待删除
    CAN1_SYSTEM_STATE_NEEDLE_LIQUID_ADJUST          = 72,//移液针探液校准
    CAN1_SYSTEM_STATE_NEEDLE_LIQUID_CLEAN           = 73,//移液针清洗状态
    CAN1_SYSTEM_STATE_MID_BOOT                      = 253,//中位机BOOT状态
    CAN1_SYSTEM_STATE_DEVICE_BOOT                   = 254,//设备处于BOOT状态
    CAN1_SYSTEM_STATE_ERROR                         = 255
}CAN1_SYSTEM_STATE;

#endif




