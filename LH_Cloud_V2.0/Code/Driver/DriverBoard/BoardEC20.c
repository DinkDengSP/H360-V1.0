/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-09-02 13:43:06
**LastEditors: DengXiaoJun
**LastEditTime: 2021-03-22 14:17:26
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardEC20.h"
#include "BoardRS.h"

//EC20初始化
void BoardEC20_PortInit(void)
{
    //端口初始化
    EC20Util_PortInit();
}

//EC20检测是否存在
LH_ERR BoardEC20_CheckExist(void)
{
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //申请信号量
    OS_ERR err;
    if(OSRunning)OSMutexPend(&mutexMCU_EC20, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //检测EC20存在
    ec20Result = EC20Util_CheckModuleExist();
    if(ec20Result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
    return LH_ERR_NONE;
}

//EC20重启
void BoardEC20_Restart(void)
{
    //模块重启
    EC20Util_ModuleReset();
}

//EC20检测模块ready
LH_ERR BoardEC20_CheckNetWorkReady(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    //申请信号量
    OS_ERR err;
    if(OSRunning)OSMutexPend(&mutexMCU_EC20, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //检查EC20模块是否准备好
    result = EC20Util_CheckModuleReady();
    if(result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(result);
    }
    //检查EC20卡是否准备好
    result = EC20Util_CheckSIMCardReady();
    if(result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(result);
    }
    //检查网络是否注册
    result = EC20Util_CheckNetRegisterReady();
    if(result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(result);
    }
    //检查GPS附着是否完成
    result = EC20Util_CheckGprsAttachReady();
    if(result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(result);
    }
    if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
    //检测完成
    return LH_ERR_NONE;
}

//EC20使能网络连接
LH_ERR BoardEC20_EnableNetWork(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    //申请信号量
    OS_ERR err;
    if(OSRunning)OSMutexPend(&mutexMCU_EC20, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //设置接入点
    result = EC20_SetGprsConfig();
    if(result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(result);
    }
    //关闭GPRS
    result = EC20_DisableGprs();
    if(result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(result);
    }
    //增加延时
    CoreDelayMs(2000);
    //打开GPRS
    result = EC20_EnableGprs();
    if(result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(result);
    }
    if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
    //网络连接完成
    return LH_ERR_NONE;
}

//EC20检测网络可用
LH_ERR BoardEC20_CheckNetWorkAvailable(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    //申请信号量
    OS_ERR err;
    if(OSRunning)OSMutexPend(&mutexMCU_EC20, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //测试服务器能否ping通
    result = EC20_CheckNetPingServer();
    if(result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(result);
    }
    if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
    return LH_ERR_NONE;
}

//EC20更新模块全部讯息
LH_ERR BoardEC20_ReflushModuleAllMsg(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    //申请信号量
    OS_ERR err;
    if(OSRunning)OSMutexPend(&mutexMCU_EC20, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //厂商ID获取
    result = EC20_GetManufacturerDesp();
    if(result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(result);
    }
    //模块信号获取
    result = EC20_GetModuleTypeDesp();
    if(result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(result);
    }
    //软件版本获取
    result = EC20_GetSoftwareVersionDesp();
    if(result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(result);
    }
    //模块入网许可获取
    result = EC20_GetIMEICodeDesp();
    if(result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(result);
    }
    //SIM卡入网许可获取
    result = EC20_GetIMSICodeDesp();
    if(result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(result);
    }
    //SIM卡唯一标识ICCID获取
    result = EC20_GetICCIDCodeDesp();
    if(result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(result);
    }
    //网络名称获取
    result = EC20_GetNetNameDesp();
    if(result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(result);
    }
    //网络类型获取
    result = EC20_GetNetTypeDesp();
    if(result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(result);
    }
    //信号强度获取
    result = EC20_GetSignalStrength();
    if(result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(result);
    }
    //基站信息获取
    result = EC20Util_CheckNetBaseStationMsg();
    if(result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(result);
    }
    if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
    return LH_ERR_NONE;
}

//EC20打印模块全部讯息
void BoardEC20_PrintModuleAllMsg(void)
{
    //显示模块讯息
    SystemPrintf("EC20 manufacturerString : %s\r\n",ec20Message.manufacturerString);
    SystemPrintf("EC20 moduleTypeString : %s\r\n",ec20Message.moduleTypeString);
    SystemPrintf("EC20 softwareVersion : %s\r\n",ec20Message.softwareVersion);
    SystemPrintf("EC20 imeiCodeString : %s\r\n",ec20Message.imeiCodeString);
    SystemPrintf("EC20 imsiCodeString : %s\r\n",ec20Message.imsiCodeString);
    SystemPrintf("EC20 iccidCodeString : %s\r\n",ec20Message.iccidCodeString);
    SystemPrintf("EC20 fullNetWorkNameString : %s\r\n",ec20Message.fullNetWorkNameString);
    SystemPrintf("EC20 shortNetworkNameString : %s\r\n",ec20Message.shortNetworkNameString);
    SystemPrintf("EC20 netServiceProviderNameString : %s\r\n",ec20Message.netServiceProviderNameString);
    SystemPrintf("EC20 networkModeString : %s\r\n",ec20Message.networkModeString);
    SystemPrintf("EC20 networkBandString : %s\r\n",ec20Message.networkBandString);
    SystemPrintf("EC20 singalStrength : %d\r\n",ec20Message.singalStrength);
    SystemPrintf("EC20 errorValue : %d\r\n",ec20Message.errorValue);
    SystemPrintf("EC20 baseStationCodeStringLAC : %s\r\n",ec20Message.baseStationCodeStringLAC);
    SystemPrintf("EC20 baseStationCodeStringCID : %s\r\n",ec20Message.baseStationCodeStringCID);
}

//获取信号强度
LH_ERR BoardEC20_GetSignalStrength(uint8_t* strength)
{
    EC20_RESULT result = EC20_RESULT_OK;
    //申请信号量
    OS_ERR err;
    if(OSRunning)OSMutexPend(&mutexMCU_EC20, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //信号强度获取
    result = EC20_GetSignalStrength();
    if(result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(result);
    }
    if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
    *strength = ec20Message.singalStrength;
    return LH_ERR_NONE;
}

//EC20连接网络
LH_ERR BoardEC20_SocketConnect(uint8_t socketPort,uint8_t ip1,uint8_t ip2,uint8_t ip3,uint8_t ip4,uint16_t remotePort)
{
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //申请信号量
    OS_ERR err;
    if(OSRunning)OSMutexPend(&mutexMCU_EC20, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //建立连接并发送一包数据
    ec20Result = EC20_SocketConnectServicePort(socketPort,ip1,ip2,ip3,ip4,remotePort);
    if(ec20Result != EC20_RESULT_OK)
    {
        //已经打开了,先关上
        if(ec20Result == EC20_RESULT_SOCKET_OPEN_EXIST)
        {
            ec20Result = EC20_SocketDisconnectServicePort(socketPort);
            if(ec20Result != EC20_RESULT_OK)
            {
                if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
                return EC20_MODULE_RESULT_CONVERT(ec20Result);
            }
        }
        else
        {
            if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
            return EC20_MODULE_RESULT_CONVERT(ec20Result);
        }
        //再打开
        ec20Result = EC20_SocketConnectServicePort(socketPort,ip1,ip2,ip3,ip4,remotePort);
        if(ec20Result != EC20_RESULT_OK)
        {
            if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
            return EC20_MODULE_RESULT_CONVERT(ec20Result);
        }
    }
    if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
    return LH_ERR_NONE;
}

//EC20断开网络
LH_ERR BoardEC20_SocketDisconnect(uint8_t socketPort)
{
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //申请信号量
    OS_ERR err;
    if(OSRunning)OSMutexPend(&mutexMCU_EC20, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    ec20Result = EC20_SocketDisconnectServicePort(socketPort);
    if(ec20Result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
    return LH_ERR_NONE;
}

//EC20发送一帧数据,不自动联机断开
LH_ERR BoardEC20_SocketSendMsgWithoutConnect(uint8_t socketPort,uint8_t* msgBuffer,uint16_t msgLength)
{
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //申请信号量
    OS_ERR err;
    if(OSRunning)OSMutexPend(&mutexMCU_EC20, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //发送一包数据
    ec20Result = EC20_SocketSendBufferMsg(socketPort,msgBuffer,msgLength);
    if(ec20Result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
    return LH_ERR_NONE;
}

//EC20发送一帧数据,自动联机断开
LH_ERR BoardEC20_SocketSendMsgWithConnect(uint8_t socketPort,uint8_t ip1,uint8_t ip2,uint8_t ip3,uint8_t ip4,uint16_t remotePort,uint8_t* msgBuffer,uint16_t msgLength)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //申请信号量
    OS_ERR err;
    if(OSRunning)OSMutexPend(&mutexMCU_EC20, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //联机
    errorCode = BoardEC20_SocketConnect(socketPort,ip1,ip2,ip3,ip4,remotePort);
    if(errorCode != LH_ERR_NONE)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return errorCode;
    }
    //数据发送
    errorCode = BoardEC20_SocketSendMsgWithoutConnect(socketPort,msgBuffer,msgLength);
    if(errorCode != LH_ERR_NONE)
    {
        //发送异常断开连接
        LH_ERR errorCodeSub = BoardEC20_SocketDisconnect(socketPort);
        if(errorCodeSub != LH_ERR_NONE)
        {
            if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
            return errorCodeSub;
        }
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return errorCode;
    }
    //发送完成,断开连接
    errorCode = BoardEC20_SocketDisconnect(socketPort);
    if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
    return errorCode;
}

//读取当前缓冲区等待读取的数据数量
LH_ERR BoardEC20_SocketGetLengthWaitRead(uint8_t socketPort,uint16_t* waitReadLength)
{
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //申请信号量
    OS_ERR err;
    if(OSRunning)OSMutexPend(&mutexMCU_EC20, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    ec20Result = EC20_SocketCheckWaitReadDataLength(socketPort,waitReadLength);
    if(ec20Result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
    return LH_ERR_NONE;
}

//清除当前缓冲区全部讯息
LH_ERR BoardEC20_SocketClearBuf(uint8_t socketPort)
{
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //申请信号量
    OS_ERR err;
    if(OSRunning)OSMutexPend(&mutexMCU_EC20, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    ec20Result = EC20_SocketClearRecvBuffer(socketPort);
    if(ec20Result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
    return LH_ERR_NONE;
}

//读取指定长度缓冲区数据
LH_ERR BoardEC20_SocketReadBufferByLength(uint8_t socketPort,uint8_t* recvBuffer,uint16_t readLengthSet,uint16_t* realReadLength)
{
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //申请信号量
    OS_ERR err;
    if(OSRunning)OSMutexPend(&mutexMCU_EC20, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    ec20Result = EC20_SocketReadRecvData(socketPort,readLengthSet,recvBuffer,realReadLength);
    if(ec20Result != EC20_RESULT_OK)
    {
        if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
        return EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);
    return LH_ERR_NONE;
}










