/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-06 13:28:58
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-26 18:05:42
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplCanMsgBio.h"

//软件急停
static void AppImplBioCanMsgProcCmd_EmergencyStop(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//错误上报
static void AppImplBioCanMsgProcCmd_ReportError(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//机械复位
static void AppImplBioCanMsgProcCmd_MachineReset(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//心跳上报
static void AppImplBioCanMsgProcCmd_StateHeartReport(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//升级开始
static void AppImplBioCanMsgProcCmd_IapStart(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//升级数据
static void AppImplBioCanMsgProcCmd_IapData(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//升级完成
static void AppImplBioCanMsgProcCmd_IapEnd(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//升级准备
static void AppImplBioCanMsgProcCmd_IapPrepare(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//APP版本号
static void AppImplBioCanMsgProcCmd_ReadAppVersion(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//BOOT版本号
static void AppImplBioCanMsgProcCmd_ReadBootVersion(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//调试指令
static void AppImplBioCanMsgProcCmd_Debug(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);
//日志讯息
static void AppImplBioCanMsgProcCmd_Log(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength);

static const MAP_CAN_BIO_MSG_CMD_FUNC_UNIT mapCanMsgBioProcFuncArray[] = {
    {CAN_CMD_COMM_SOFT_EMERGENCY_STOP        ,AppImplBioCanMsgProcCmd_EmergencyStop     },//软件急停
    {CAN_CMD_COMM_REPORT_ERR                 ,AppImplBioCanMsgProcCmd_ReportError       },//错误上报
    {CAN_CMD_COMM_MACHINE_RESET              ,AppImplBioCanMsgProcCmd_MachineReset      },//机械复位
    {CAN_CMD_COMM_SYSTEM_STATE_HEART         ,AppImplBioCanMsgProcCmd_StateHeartReport  },//心跳上报
    {CAN_CMD_COMM_IAP_START                  ,AppImplBioCanMsgProcCmd_IapStart          },//升级开始
    {CAN_CMD_COMM_IAP_DATA                   ,AppImplBioCanMsgProcCmd_IapData           },//升级数据
    {CAN_CMD_COMM_IAP_END                    ,AppImplBioCanMsgProcCmd_IapEnd            },//升级完成
    {CAN_CMD_COMM_IAP_PREPARE                ,AppImplBioCanMsgProcCmd_IapPrepare        },//升级准备
    {CAN_CMD_COMM_APP_VERSION                ,AppImplBioCanMsgProcCmd_ReadAppVersion    },//APP版本号
    {CAN_CMD_COMM_BOOT_VERSION               ,AppImplBioCanMsgProcCmd_ReadBootVersion   },//BOOT版本号
    {CAN_CMD_COMM_DEBUG_CONTROL              ,AppImplBioCanMsgProcCmd_Debug             },//调试指令
    {CAN_CMD_COMM_LOG                        ,AppImplBioCanMsgProcCmd_Log               },//日志讯息
};
#define LENGTH_MAP_CAN_MSG_CMD_FUNC_UNIT_RAIL       (sizeof(mapCanMsgBioProcFuncArray)/sizeof(mapCanMsgBioProcFuncArray[0]))

//生化消息处理
void AppImplCanMsgBioProcess(uint8_t srcModule,uint8_t srcBoard,uint16_t cmd,uint8_t* dataBuf,uint16_t dataLength)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < LENGTH_MAP_CAN_MSG_CMD_FUNC_UNIT_RAIL; indexUtil++)
    {
        //配置指令和列表
        if(cmd == mapCanMsgBioProcFuncArray[indexUtil].command)
        {
            //调用相关的处理子函数
            mapCanMsgBioProcFuncArray[indexUtil].canMsgProcFuncPtr(srcModule,srcBoard,dataBuf,dataLength);
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
static void AppImplBioCanMsgProcCmd_EmergencyStop(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
    //输出日志
    SystemPrintf("[MID]Func:%s,Line:%d\r\n",__FUNCTION__,__LINE__);
}

//错误上报
static void AppImplBioCanMsgProcCmd_ReportError(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
    //处理报警
    AppImplCanMsgCommWarningMsg(srcModule,dataBuf,dataLength);
}

//机械复位
static void AppImplBioCanMsgProcCmd_MachineReset(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
    //输出日志
    SystemPrintf("[MID]Func:%s,Line:%d\r\n",__FUNCTION__,__LINE__);
}

//心跳上报
static void AppImplBioCanMsgProcCmd_StateHeartReport(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
    //输出日志
    SystemPrintf("[MID]Func:%s,Line:%d\r\n",__FUNCTION__,__LINE__);
}

//升级准备
static void AppImplBioCanMsgProcCmd_IapPrepare(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
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
static void AppImplBioCanMsgProcCmd_IapStart(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
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
static void AppImplBioCanMsgProcCmd_IapData(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
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
static void AppImplBioCanMsgProcCmd_IapEnd(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
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
static void AppImplBioCanMsgProcCmd_ReadAppVersion(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
    if(dataLength != 8)
    {
        SystemPrintf("[MID]E>Func:%s,Line:%d ,DataLength : 0X%08X\r\n",__FUNCTION__,__LINE__,dataLength);
        return;
    }
    //输出日志
    SystemPrintf("[MID]Func:%s,Line:%d\r\n",__FUNCTION__,__LINE__);
    //调用公共
    AppImplCanMsgCommIapReadAppVersion(srcModule,dataBuf,dataLength);
}

//BOOT版本号
static void AppImplBioCanMsgProcCmd_ReadBootVersion(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
    if(dataLength != 8)
    {
        SystemPrintf("[MID]E>Func:%s,Line:%d ,DataLength : 0X%08X\r\n",__FUNCTION__,__LINE__,dataLength);
        return;
    }
    //输出日志
    SystemPrintf("[MID]Func:%s,Line:%d\r\n",__FUNCTION__,__LINE__);
    //调用公共
    AppImplCanMsgCommIapReadBootVersion(srcModule,dataBuf,dataLength);
}

//调试指令
static void AppImplBioCanMsgProcCmd_Debug(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
    AppImplCanMsgCommDebugResult(dataBuf,dataLength);
}

//日志讯息
static void AppImplBioCanMsgProcCmd_Log(uint8_t srcModule,uint8_t srcBoard,uint8_t* dataBuf,uint16_t dataLength)
{
    AppImplCanMsgCommShowLog("[BIO]",dataBuf);
}
