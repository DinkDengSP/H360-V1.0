/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-09-08 11:04:56
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-03-22 14:18:05
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_CLOUD_BASE_H_
#define __SERVICE_IMPL_CLOUD_BASE_H_
#include "ServiceImplBase.h"

//设备重启注册的最长时间,90S重启
#define RESTART_TIME_MS_MAX             90000

//PING 服务器测试
LH_ERR ServiceImplCloudBase_PingTestNetWork(void);

//获取模块厂商信息
LH_ERR ServiceImplCloudBase_QueryManufacturerName(void);

//获取模块类型信息
LH_ERR ServiceImplCloudBase_QueryModuleType(void);

//获取模块软件版本信息
LH_ERR ServiceImplCloudBase_QuerySoftwareVersion(void);

//获取模块入网许可
LH_ERR ServiceImplCloudBase_QueryModuleIMEI(void);

//获取SIM卡IMSI串号
LH_ERR ServiceImplCloudBase_QuerySimIMSI(void);

//获取SIM卡ICCID识别号
LH_ERR ServiceImplCloudBase_QuerySimICCID(void);

//获取当前模块连接网络全名
LH_ERR ServiceImplCloudBase_QueryNetFullName(void);

//获取当前模块连接网络短名
LH_ERR ServiceImplCloudBase_QueryNetShortName(void);

//获取网络服务商名称
LH_ERR ServiceImplCloudBase_QueryNetProvide(void);

//获取当前网络制式
LH_ERR ServiceImplCloudBase_QueryNetType(void);

//获取当前网络波段
LH_ERR ServiceImplCloudBase_QueryNetBaud(void);

//获取LAC基站区域码
LH_ERR ServiceImplCloudBase_QueryBaseStationLAC(void);

//获取CID基站编号
LH_ERR ServiceImplCloudBase_QueryBaseStationCID(void);

//获取当前信号强度与误码率
LH_ERR ServiceImplCloudBase_QuerySignalStrength(void);

//查看当前模块网络是否正常
LH_ERR ServiceImplCloudBase_QueryNetState(void);

//打开设备GPRS连接
LH_ERR ServiceImplCloudBase_EnableGPRS(void);

//通讯设备重启
LH_ERR ServiceImplCloudBase_ModuleRestartIntegrate(void);

#endif




