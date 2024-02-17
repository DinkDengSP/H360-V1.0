/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-06 13:28:19
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-26 18:05:51
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplCanMsgCloud.h"

//软件急停
static void AppImplCloudCanMsgProcCmd_EmergencyStop(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//错误上报
static void AppImplCloudCanMsgProcCmd_ReportError(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//机械复位
static void AppImplCloudCanMsgProcCmd_MachineReset(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//心跳上报
static void AppImplCloudCanMsgProcCmd_StateHeartReport(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//升级开始
static void AppImplCloudCanMsgProcCmd_IapStart(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//升级数据
static void AppImplCloudCanMsgProcCmd_IapData(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//升级完成
static void AppImplCloudCanMsgProcCmd_IapEnd(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//升级准备
static void AppImplCloudCanMsgProcCmd_IapPrepare(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//APP版本号
static void AppImplCloudCanMsgProcCmd_ReadAppVersion(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//BOOT版本号
static void AppImplCloudCanMsgProcCmd_ReadBootVersion(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//调试指令
static void AppImplCloudCanMsgProcCmd_Debug(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//日志讯息
static void AppImplCloudCanMsgProcCmd_Log(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);


static const MAP_CAN_BIO_MSG_CMD_FUNC_UNIT mapCanMsgCloudProcFuncArray[] = {
    {CAN_CMD_COMM_SOFT_EMERGENCY_STOP        ,AppImplCloudCanMsgProcCmd_EmergencyStop     },//软件急停
    {CAN_CMD_COMM_REPORT_ERR                 ,AppImplCloudCanMsgProcCmd_ReportError       },//错误上报
    {CAN_CMD_COMM_MACHINE_RESET              ,AppImplCloudCanMsgProcCmd_MachineReset      },//机械复位
    {CAN_CMD_COMM_SYSTEM_STATE_HEART         ,AppImplCloudCanMsgProcCmd_StateHeartReport  },//心跳上报
    {CAN_CMD_COMM_IAP_START                  ,AppImplCloudCanMsgProcCmd_IapStart          },//升级开始
    {CAN_CMD_COMM_IAP_DATA                   ,AppImplCloudCanMsgProcCmd_IapData           },//升级数据
    {CAN_CMD_COMM_IAP_END                    ,AppImplCloudCanMsgProcCmd_IapEnd            },//升级完成
    {CAN_CMD_COMM_IAP_PREPARE                ,AppImplCloudCanMsgProcCmd_IapPrepare        },//升级准备
    {CAN_CMD_COMM_APP_VERSION                ,AppImplCloudCanMsgProcCmd_ReadAppVersion    },//APP版本号
    {CAN_CMD_COMM_BOOT_VERSION               ,AppImplCloudCanMsgProcCmd_ReadBootVersion   },//BOOT版本号
    {CAN_CMD_COMM_DEBUG_CONTROL              ,AppImplCloudCanMsgProcCmd_Debug             },//调试指令
    {CAN_CMD_COMM_LOG                        ,AppImplCloudCanMsgProcCmd_Log               },//日志讯息
};
#define LENGTH_MAP_CAN_MSG_CMD_FUNC_UNIT_RAIL       (sizeof(mapCanMsgCloudProcFuncArray)/sizeof(mapCanMsgCloudProcFuncArray[0]))

//立禾云CAN消息处理
void AppImplCanMsgCloudProcess(uint8_t srcModule,uint8_t srcBoard,uint16_t cmd,uint8_t* dataBuf,uint16_t dataLength)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < LENGTH_MAP_CAN_MSG_CMD_FUNC_UNIT_RAIL; indexUtil++)
    {
        //配置指令和列表
        if(cmd == mapCanMsgCloudProcFuncArray[indexUtil].command)
        {
            //调用相关的处理子函数
            mapCanMsgCloudProcFuncArray[indexUtil].canMsgProcFuncPtr(srcModule,srcBoard,dataBuf,dataLength);
            //调用完成跳出循环
            break;
        }
    }
    if(indexUtil >= LENGTH_MAP_CAN_MSG_CMD_FUNC_UNIT_RAIL)
    {
        //没有找到对应的指令
        SystemPrintf("[MID]E>Func:%s,Line:%d , NotSupport Command : 0X%02X\r\n",__FUNCTION__,__LINE__,cmd);
    }
}


//软件急停
static void AppImplCloudCanMsgProcCmd_EmergencyStop(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
    //输出日志
    SystemPrintf("[MID]Func:%s,Line:%d\r\n",__FUNCTION__,__LINE__);
}

//错误上报
static void AppImplCloudCanMsgProcCmd_ReportError(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
    //处理报警
    AppImplCanMsgCommWarningMsg(srcModule,dataBuf,dataLength);
}

//机械复位
static void AppImplCloudCanMsgProcCmd_MachineReset(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
    //输出日志
    SystemPrintf("[MID]Func:%s,Line:%d\r\n",__FUNCTION__,__LINE__);
}

//心跳上报
static void AppImplCloudCanMsgProcCmd_StateHeartReport(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
    //输出日志
    SystemPrintf("[MID]Func:%s,Line:%d\r\n",__FUNCTION__,__LINE__);
}

//升级准备
static void AppImplCloudCanMsgProcCmd_IapPrepare(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
     if(dataLength != 3)
    {
        SystemPrintf("[MID]E>Func:%s,Line:%d ,DataLength : 0X%08X\r\n",__FUNCTION__,__LINE__,dataLength);
        return;
    }
    //输出日志
    SystemPrintf("[MID]Func:%s,Line:%d\r\n",__FUNCTION__,__LINE__);
    //调用公共
    AppImplCanMsgCommIapParpare(dataBuf,dataLength);
}

//升级开始
static void AppImplCloudCanMsgProcCmd_IapStart(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
    if(dataLength != 3)
    {
        SystemPrintf("[MID]E>Func:%s,Line:%d ,DataLength : 0X%08X\r\n",__FUNCTION__,__LINE__,dataLength);
        return;
    }
    //输出日志
    SystemPrintf("[MID]Func:%s,Line:%d\r\n",__FUNCTION__,__LINE__);
    //调用公共
    AppImplCanMsgCommIapStart(dataBuf,dataLength);
}

//升级数据
static void AppImplCloudCanMsgProcCmd_IapData(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
    if(dataLength != 3)
    {
        SystemPrintf("[MID]E>Func:%s,Line:%d ,DataLength : 0X%08X\r\n",__FUNCTION__,__LINE__,dataLength);
        return;
    }
    //输出日志
    SystemPrintf("[MID]Func:%s,Line:%d\r\n",__FUNCTION__,__LINE__);
    //调用公共
    AppImplCanMsgCommIapData(dataBuf,dataLength);
}

//升级完成
static void AppImplCloudCanMsgProcCmd_IapEnd(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
    if(dataLength != 3)
    {
        SystemPrintf("[MID]E>Func:%s,Line:%d ,DataLength : 0X%08X\r\n",__FUNCTION__,__LINE__,dataLength);
        return;
    }
    //输出日志
    SystemPrintf("[MID]Func:%s,Line:%d\r\n",__FUNCTION__,__LINE__);
    //调用公共
    AppImplCanMsgCommIapEnd(dataBuf,dataLength);
}

//APP版本号
static void AppImplCloudCanMsgProcCmd_ReadAppVersion(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
    if(dataLength != 8)
    {
        SystemPrintf("[MID]E>Func:%s,Line:%d ,DataLength : 0X%08X\r\n",__FUNCTION__,__LINE__,dataLength);
        return;
    }
    //输出日志
    SystemPrintf("[MID]Func:%s,Line:%d\r\n",__FUNCTION__,__LINE__);
    //调用公共
    AppImplCanMsgCommIapReadAppVersion(CAN_CLOUD_MODULE_NO,dataBuf,dataLength);
}

//BOOT版本号
static void AppImplCloudCanMsgProcCmd_ReadBootVersion(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
    if(dataLength != 8)
    {
        SystemPrintf("[MID]E>Func:%s,Line:%d ,DataLength : 0X%08X\r\n",__FUNCTION__,__LINE__,dataLength);
        return;
    }
    //输出日志
    SystemPrintf("[MID]Func:%s,Line:%d\r\n",__FUNCTION__,__LINE__);
    //调用公共
    AppImplCanMsgCommIapReadBootVersion(CAN_CLOUD_MODULE_NO,dataBuf,dataLength);
}

//调试指令
static void AppImplCloudCanMsgProcCmd_Debug(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
    AppImplCanMsgCommDebugResult(dataBuf,dataLength);
}

//日志讯息
static void AppImplCloudCanMsgProcCmd_Log(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
    AppImplCanMsgCommShowLog("[CLOUD]",dataBuf);
}
