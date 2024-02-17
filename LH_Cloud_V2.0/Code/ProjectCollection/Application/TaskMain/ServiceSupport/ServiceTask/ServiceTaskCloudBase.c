/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-09-07 20:15:16
**LastEditors: DengXiaoJun
**LastEditTime: 2020-09-08 11:17:32
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceTaskConfig.h"
#include "ServiceImplInc.h"

//任务控制块,8字节对齐
#pragma pack(8)
//任务堆栈
CPU_STK stackBufferServiceTaskCloudBase[STK_SIZE_SERVICE_TASK_CLOUD_BASE]@".ccram";
#pragma pack()
//任务控制块
OS_TCB tcbServiceTaskCloudBase@".ccram";

//系统服务任务接收几个方面的消息
//1. 来自CAN1的消息
static void ServiceTaskFuncCloudBase_CanMsg_Proc(CAN1_CMD_RECV* can1CommandPtr);

//任务函数
void ServiceTaskFuncCloudBase(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    //任务接收数据长度
	OS_MSG_SIZE size;
    //任务接收讯息
    SYSTEM_CMD* taskCommandPtr = NULL;
    //延时等待接收函数初始化成功
    CoreDelayMs(100);
    while(1)
    {
        //任务从队列中读取指令,堵塞指定时间去执行定时任务
        taskCommandPtr = OSTaskQPend((OS_TICK)0,
                                     (OS_OPT)OS_OPT_PEND_BLOCKING,
                                     (OS_MSG_SIZE *)&size,
                                     (CPU_TS *)0,
                                     (OS_ERR *)&err);
        if ((taskCommandPtr != NULL)&&(err == OS_ERR_NONE))
		{
            //从队列里面读取到了数据
            if(SYSTEM_CMD_SRC_CAN == taskCommandPtr->cmdSrc)
            {
                //从CAN1传过来的指令
                CAN1_CMD_RECV* can1RecvDatPtr = (CAN1_CMD_RECV*)taskCommandPtr->cmdDataPtr;
                //对这个数据进行处理
                ServiceTaskFuncCloudBase_CanMsg_Proc(can1RecvDatPtr);
                //处置完成,执行后续操作,只有CAN1才使用SRAM CCM作为指令内存
                UserMemFree(SRAM_CCM,can1RecvDatPtr);
            }
            //程序处理完成,释放掉这个指令的内存
            taskCommandPtr->cmdDataPtr = NULL;
            UserMemFree(SRAM_CCM,taskCommandPtr);
            taskCommandPtr = NULL;
		}
    }
}

/*************************************************1. 来自CAN1的消息******************************************/
//PING 服务器测试
void ServiceTaskFuncCloudBase_Can1Msg_Proc_Ping(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//获取模块厂商信息
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryManufacturerName(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//获取模块类型信息
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryModuleType(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//获取模块软件版本信息
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QuerySoftwareVersion(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//获取模块入网许可
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryModuleIMEI(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//获取SIM卡IMSI串号
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QuerySimIMSI(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//获取SIM卡ICCID识别号
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QuerySimICCID(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//获取当前模块连接网络全名
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryNetFullName(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//获取当前模块连接网络短名
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryNetShortName(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//获取网络服务商名称
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryNetProvide(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//获取当前网络制式
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryNetType(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//获取当前网络波段
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryNetBaud(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//获取LAC基站区域码
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryBaseStationLAC(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//获取CID基站编号
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryBaseStationCID(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//获取当前信号强度与误码率
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QuerySignalStrength(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//查看当前模块网络是否正常
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryNetState(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//打开设备GPRS连接
void ServiceTaskFuncCloudBase_Can1Msg_Proc_EnableGPRS(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);
//通讯设备重启
void ServiceTaskFuncCloudBase_Can1Msg_Proc_ModuleRestart(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr);

//前一个是指令代码,后一个是处理函数
const SYSTEM_TASK_CAN_MSG_PROC_UNIT CloudBaseCan1CommandProcFuncPtrArray[] = {
    {CAN1_CMD_CLOUD_BASE_PING                      ,ServiceTaskFuncCloudBase_Can1Msg_Proc_Ping                      ,"Ping"                     },//PING 服务器测试
    {CAN1_CMD_CLOUD_BASE_QUERY_MANUFACTURER_NAME   ,ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryManufacturerName     ,"QueryManufacturerName"    },//获取模块厂商信息
    {CAN1_CMD_CLOUD_BASE_QUERY_MODULE_TYPE         ,ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryModuleType           ,"QueryModuleType"          },//获取模块类型信息
    {CAN1_CMD_CLOUD_BASE_QUERY_SOFTWARE_VERSION    ,ServiceTaskFuncCloudBase_Can1Msg_Proc_QuerySoftwareVersion      ,"QuerySoftwareVersion "    },//获取模块软件版本信息
    {CAN1_CMD_CLOUD_BASE_QUERY_MODULE_IMEI         ,ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryModuleIMEI           ,"QueryModuleIMEI"          },//获取模块入网许可
    {CAN1_CMD_CLOUD_BASE_QUERY_SIM_IMSI            ,ServiceTaskFuncCloudBase_Can1Msg_Proc_QuerySimIMSI              ,"QuerySimIMSI"             },//获取SIM卡IMSI串号
    {CAN1_CMD_CLOUD_BASE_QUERY_SIM_ICCID           ,ServiceTaskFuncCloudBase_Can1Msg_Proc_QuerySimICCID             ,"QuerySimICCID"            },//获取SIM卡ICCID识别号
    {CAN1_CMD_CLOUD_BASE_QUERY_NET_FULL_NAME       ,ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryNetFullName          ,"QueryNetFullName"         },//获取当前模块连接网络全名
    {CAN1_CMD_CLOUD_BASE_QUERY_NET_SHORT_NAME      ,ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryNetShortName         ,"QueryNetShortName"        },//获取当前模块连接网络短名
    {CAN1_CMD_CLOUD_BASE_QUERY_NET_PROVIDE         ,ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryNetProvide           ,"QueryNetProvide"          },//获取网络服务商名称
    {CAN1_CMD_CLOUD_BASE_QUERY_NET_TYPE            ,ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryNetType              ,"QueryNetType"             },//获取当前网络制式
    {CAN1_CMD_CLOUD_BASE_QUERY_NET_BAUD            ,ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryNetBaud              ,"QueryNetBaud"             },//获取当前网络波段
    {CAN1_CMD_CLOUD_BASE_QUERY_BASE_STATION_LAC    ,ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryBaseStationLAC       ,"QueryBaseStationLAC"      },//获取LAC基站区域码
    {CAN1_CMD_CLOUD_BASE_QUERY_BASE_STATION_CID    ,ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryBaseStationCID       ,"QueryBaseStationCID"      },//获取CID基站编号
    {CAN1_CMD_CLOUD_BASE_QUERY_SIGNAL_STRENGTH     ,ServiceTaskFuncCloudBase_Can1Msg_Proc_QuerySignalStrength       ,"QuerySignalStrength"      },//获取当前信号强度与误码率
    {CAN1_CMD_CLOUD_BASE_QUERY_NET_STATE           ,ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryNetState             ,"QueryNetState"            },//查看当前模块网络是否正常
    {CAN1_CMD_CLOUD_BASE_ENABLE_GPRS               ,ServiceTaskFuncCloudBase_Can1Msg_Proc_EnableGPRS                ,"EnableGPRS"               },//打开设备GPRS连接
    {CAN1_CMD_CLOUD_BASE_MODULE_RESTART            ,ServiceTaskFuncCloudBase_Can1Msg_Proc_ModuleRestart             ,"ModuleRestart"            },//通讯设备重启
};
//CAN指令支持的数量
#define TASK_CAN1_MSG_CLOUD_BASE_CMD_COUNT        (sizeof(CloudBaseCan1CommandProcFuncPtrArray)/sizeof(CloudBaseCan1CommandProcFuncPtrArray[0]))

//在配置数组中查找能和发送来的指令匹配的解析函数
static void ServiceTaskFuncCloudBase_CanMsg_Proc(CAN1_CMD_RECV* can1CommandPtr)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < TASK_CAN1_MSG_CLOUD_BASE_CMD_COUNT; indexUtil++)
    {
        //指令匹配
        if(can1CommandPtr->command == CloudBaseCan1CommandProcFuncPtrArray[indexUtil].cmd)
        {
            //判定处理函数不为空
            if(CloudBaseCan1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr != NULL)
            {
                //调用函数处理
                CloudBaseCan1CommandProcFuncPtrArray[indexUtil].can1CmdProcFuncPtr(can1CommandPtr,CloudBaseCan1CommandProcFuncPtrArray[indexUtil].debugDetailMsg);
                break;
            }
        }
    }
}

//PING 服务器测试
void ServiceTaskFuncCloudBase_Can1Msg_Proc_Ping(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数

    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudBase:  ",detailDebugMsgPtr,0,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudBase_PingTestNetWork();
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_PING_ERR;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    //返回执行结果
    Can1SendCommandResult(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudBase:  ",errorCode);
}

//获取模块厂商信息
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryManufacturerName(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数

    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudBase:  ",detailDebugMsgPtr,0,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudBase_QueryManufacturerName();
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_QUERY_MANUFACTURER_NAME_ERR;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    if(errorCode != LH_ERR_NONE)
    {
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    }
    else
    {
        uint16_t length = strlen((const char*)ec20Message.manufacturerString);
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,length,ec20Message.manufacturerString);
    }
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudBase:  ",errorCode);
}

//获取模块类型信息
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryModuleType(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数

    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudBase:  ",detailDebugMsgPtr,0,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudBase_QueryModuleType();
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_QUERY_MODULE_TYPE_ERR;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    if(errorCode != LH_ERR_NONE)
    {
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    }
    else
    {
        uint16_t length = strlen((const char*)ec20Message.moduleTypeString);
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,length,ec20Message.moduleTypeString);
    }
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudBase:  ",errorCode);
}

//获取模块软件版本信息
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QuerySoftwareVersion(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数

    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudBase:  ",detailDebugMsgPtr,0,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudBase_QuerySoftwareVersion();
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_QUERY_SOFTWARE_VERSION_ERR;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    if(errorCode != LH_ERR_NONE)
    {
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    }
    else
    {
        uint16_t length = strlen((const char*)ec20Message.softwareVersion);
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,length,ec20Message.softwareVersion);
    }
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudBase:  ",errorCode);
}

//获取模块入网许可
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryModuleIMEI(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数

    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudBase:  ",detailDebugMsgPtr,0,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudBase_QueryModuleIMEI();
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_QUERY_MODULE_IMEI_ERR;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    if(errorCode != LH_ERR_NONE)
    {
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    }
    else
    {
        uint16_t length = strlen((const char*)ec20Message.imeiCodeString);
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,length,ec20Message.imeiCodeString);
    }
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudBase:  ",errorCode);
}

//获取SIM卡IMSI串号
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QuerySimIMSI(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{   
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数

    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudBase:  ",detailDebugMsgPtr,0,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudBase_QuerySimIMSI();
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_QUERY_SIM_IMSI_ERR;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    if(errorCode != LH_ERR_NONE)
    {
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    }
    else
    {
        uint16_t length = strlen((const char*)ec20Message.imsiCodeString);
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,length,ec20Message.imsiCodeString);
    }
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudBase:  ",errorCode);
}

//获取SIM卡ICCID识别号
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QuerySimICCID(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数

    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudBase:  ",detailDebugMsgPtr,0,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudBase_QuerySimICCID();
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_QUERY_SIM_ICCID_ERR;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    if(errorCode != LH_ERR_NONE)
    {
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    }
    else
    {
        uint16_t length = strlen((const char*)ec20Message.iccidCodeString);
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,length,ec20Message.iccidCodeString);
    }
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudBase:  ",errorCode);
}

//获取当前模块连接网络全名
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryNetFullName(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数

    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudBase:  ",detailDebugMsgPtr,0,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudBase_QueryNetFullName();
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_QUERY_NET_FULL_NAME_ERR;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    if(errorCode != LH_ERR_NONE)
    {
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    }
    else
    {
        uint16_t length = strlen((const char*)ec20Message.fullNetWorkNameString);
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,length,ec20Message.fullNetWorkNameString);
    }
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudBase:  ",errorCode);
}

//获取当前模块连接网络短名
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryNetShortName(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数

    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudBase:  ",detailDebugMsgPtr,0,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudBase_QueryNetShortName();
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_QUERY_NET_SHORT_NAME_ERR;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    if(errorCode != LH_ERR_NONE)
    {
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    }
    else
    {
        uint16_t length = strlen((const char*)ec20Message.shortNetworkNameString);
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,length,ec20Message.shortNetworkNameString);
    }
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudBase:  ",errorCode);
}

//获取网络服务商名称
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryNetProvide(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数

    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudBase:  ",detailDebugMsgPtr,0,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudBase_QueryNetProvide();
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_QUERY_NET_PROVIDE_ERR;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    if(errorCode != LH_ERR_NONE)
    {
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    }
    else
    {
        uint16_t length = strlen((const char*)ec20Message.netServiceProviderNameString);
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,length,ec20Message.netServiceProviderNameString);
    }
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudBase:  ",errorCode);
}

//获取当前网络制式
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryNetType(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数

    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudBase:  ",detailDebugMsgPtr,0,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudBase_QueryNetType();
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_QUERY_NET_TYPE_ERR;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    if(errorCode != LH_ERR_NONE)
    {
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    }
    else
    {
        uint16_t length = strlen((const char*)ec20Message.networkModeString);
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,length,ec20Message.networkModeString);
    }
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudBase:  ",errorCode);
}

//获取当前网络波段
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryNetBaud(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数

    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudBase:  ",detailDebugMsgPtr,0,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudBase_QueryNetBaud();
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_QUERY_NET_BAUD_ERR;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    if(errorCode != LH_ERR_NONE)
    {
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    }
    else
    {
        uint16_t length = strlen((const char*)ec20Message.networkBandString);
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,length,ec20Message.networkBandString);
    }
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudBase:  ",errorCode);
}

//获取LAC基站区域码
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryBaseStationLAC(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数

    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudBase:  ",detailDebugMsgPtr,0,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudBase_QueryBaseStationLAC();
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_QUERY_BASE_STATION_LAC_ERR;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    if(errorCode != LH_ERR_NONE)
    {
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    }
    else
    {
        uint16_t length = strlen((const char*)ec20Message.baseStationCodeStringLAC);
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,length,ec20Message.baseStationCodeStringLAC);
    }
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudBase:  ",errorCode);
}

//获取CID基站编号
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryBaseStationCID(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数

    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudBase:  ",detailDebugMsgPtr,0,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudBase_QueryBaseStationCID();
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_QUERY_BASE_STATION_CID_ERR;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    if(errorCode != LH_ERR_NONE)
    {
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    }
    else
    {
        uint16_t length = strlen((const char*)ec20Message.baseStationCodeStringCID);
        Can1SendCommandResultWithLength(can1CommandPtr,reportCode,reportErrorLevel,errorCode,length,ec20Message.baseStationCodeStringCID);
    }
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudBase:  ",errorCode);
}

//获取当前信号强度与误码率
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QuerySignalStrength(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数

    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudBase:  ",detailDebugMsgPtr,0,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudBase_QuerySignalStrength();
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_QUERY_SIGNAL_STRENGTH_ERR;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    uint8_t returnBuffer[2] = {0,0};
    if(errorCode == LH_ERR_NONE)
    {
        returnBuffer[0] = ec20Message.singalStrength;
        returnBuffer[1] = ec20Message.errorValue;
    }
    Can1SendCommandResult(can1CommandPtr,reportCode,reportErrorLevel,errorCode,2,returnBuffer);
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudBase:  ",errorCode);
}

//查看当前模块网络是否正常
void ServiceTaskFuncCloudBase_Can1Msg_Proc_QueryNetState(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数

    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudBase:  ",detailDebugMsgPtr,0,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudBase_QueryNetState();
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_QUERY_NET_STATE_ERR;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    Can1SendCommandResult(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudBase:  ",errorCode);
}

//打开设备GPRS连接
void ServiceTaskFuncCloudBase_Can1Msg_Proc_EnableGPRS(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数

    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudBase:  ",detailDebugMsgPtr,0,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudBase_EnableGPRS();
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_ENABLE_GPRS_ERR;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    Can1SendCommandResult(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudBase:  ",errorCode);
}

//通讯设备重启
void ServiceTaskFuncCloudBase_Can1Msg_Proc_ModuleRestart(CAN1_CMD_RECV* can1CommandPtr,uint8_t* detailDebugMsgPtr)
{
    //错误参数
    LH_ERR errorCode = LH_ERR_NONE;
    //错误代码
    CAN1_REPORT_ERR_CODE reportCode = CAN1_REPORT_OK;
    //错误级别
    CAN1_REPORT_ERR_LEVEL reportErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //获取参数

    //打印调试代码
    SystemDebugShowMsgStart("TaskFuncCloudBase:  ",detailDebugMsgPtr,0,0,0,0,0);
    //执行具体的操作
    errorCode = ServiceImplCloudBase_ModuleRestartIntegrate();
    if(errorCode != LH_ERR_NONE)
    {
        reportCode = CAN1_REPORT_MODULE_RESTART_ERR;
        reportErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    Can1SendCommandResult(can1CommandPtr,reportCode,reportErrorLevel,errorCode,0,NULL);
    //打印结果讯息
    SystemDebugShowMsgEnd("TaskFuncCloudBase:  ",errorCode);
}

