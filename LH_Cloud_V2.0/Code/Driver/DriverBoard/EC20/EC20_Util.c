/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-09-04 09:05:31
**LastEditors: DengXiaoJun
**LastEditTime: 2021-03-22 14:03:58
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "EC20_Util.h"
#include "os.h"

//保存模块讯息
EC20_MESSAGE ec20Message;

//设备操作互斥信号量
OS_MUTEX mutexMCU_EC20;
//定义一个操作系统错误
#define EC20_OS_ERR_DEFINE()                    OS_ERR err
//获取互斥信号量
#define EC20_DEVICE_PEND_MUTEX()                if(OSRunning)OSMutexPend(&mutexMCU_EC20, 0, OS_OPT_PEND_BLOCKING, 0, &err);
//释放互斥信号量
#define EC20_DEVICE_POST_MUTEX()                if(OSRunning)OSMutexPost(&mutexMCU_EC20, OS_OPT_POST_NO_SCHED, &err);

//串口接收数据标识
static uint16_t serialRecvDataLength = 0;
//串口数据缓冲区,留一些空间作为指令缓存
static uint8_t serialRecvDataBuffer[EC20_RECV_DATA_BUFFER_MAX+200] = {0};
//串口接收后缀标识,接收到OK或者Error
EC20_UART_FLAG serialRecvFlagOkError = EC20_UART_FLAG_NONE;

//清除缓冲区
static void EC20Util_ClearRecvBuffer(void)
{
    serialRecvFlagOkError = EC20_UART_FLAG_NONE;
    serialRecvDataLength = 0;
    //清零缓冲区
    memset(serialRecvDataBuffer,0,EC20_RECV_DATA_BUFFER_MAX+200);
}


//EC20回调函数,加入实时监测是否完成接收机制
static void EC20Util_DataRecvProc(uint8_t dat)
{
    if(serialRecvDataLength < EC20_RECV_DATA_BUFFER_MAX+200)
    {
        serialRecvDataBuffer[serialRecvDataLength] = dat;
        serialRecvDataLength++;
    }
    if(serialRecvFlagOkError == EC20_UART_FLAG_NONE)
    {
        if(serialRecvDataLength >=6)
        {
            if(0 == strcmp((const char*)(serialRecvDataBuffer+serialRecvDataLength-6),"\r\nOK\r\n"))
            {
                serialRecvFlagOkError = EC20_UART_FLAG_OK;
            }
        }
        if(serialRecvDataLength >=9)
        {
            if(0 == strcmp((const char*)(serialRecvDataBuffer+serialRecvDataLength-9),"\r\nERROR\r\n"))
            {
                serialRecvFlagOkError = EC20_UART_FLAG_ERROR;
            }
        }
    }
}

//EC20等待执行时间的返回并检查返回是否包含指定数据
static EC20_RESULT EC20Util_WaitModuleResponse(uint16_t maxWaitTime)
{
    uint16_t timeCountMs = 0;
    //循环检测接收是否完成
    do
    {
        EC20_WAIT_RESPONSE(5);
        timeCountMs += 5;
        if(serialRecvFlagOkError == EC20_UART_FLAG_OK)
            break;//接收到OK
        if(serialRecvFlagOkError == EC20_UART_FLAG_ERROR)
            break;//接收到指令错误
    }while(timeCountMs <= maxWaitTime);
    //检测符号标志与接收数据长度
    if(serialRecvFlagOkError == EC20_UART_FLAG_OK)
        return EC20_RESULT_OK;//接收到OK
    if(serialRecvFlagOkError == EC20_UART_FLAG_ERROR)
        return EC20_RESULT_CMD_ERR;//接收到指令错误
    if(serialRecvDataLength == 0)
        return EC20_RESULT_RECV_ZERO;//没有收到数据
    //到这里,有接收到数据,但是数据是不是想要的,需要进一步匹配
        return EC20_RESULT_OK;
}

//EC20检查串口返回字符串中是否包含指定的字串
static EC20_RESULT EC20Util_CheckResultStrContainSubStr(uint8_t* subStr)
{
    if(serialRecvDataLength == 0)
        return EC20_RESULT_RECV_ZERO;
    if(-1 != ADT_KMP_Buffer((const char*)serialRecvDataBuffer,serialRecvDataLength,(const char*)subStr))
    {
        return EC20_RESULT_OK;
    }
    return EC20_RESULT_RECV_NOT_CONTAIN_SUB_STR;
}

//EC20发送字符串并检测指定返回
static EC20_RESULT EC20Util_SendCommandCheckResult(uint8_t* commandStr,uint8_t* checkSubStr,uint16_t waitTimeMs,uint8_t retryCountMax)
{
    uint8_t retryCount = 0;
    EC20_RESULT result = EC20_RESULT_OK;
    while(retryCount < retryCountMax)
    {
        retryCount++;
        //清除接收
        EC20Util_ClearRecvBuffer();
        //发送指令
        EC20_Printf("%s\r\n",commandStr);
        //等待返回
        result = EC20Util_WaitModuleResponse(waitTimeMs);
        if(result == EC20_RESULT_OK)
        {
            //接收到数据,检查结果并返回
            return EC20Util_CheckResultStrContainSubStr(checkSubStr);
        }
    }
    return result;
}

//数据拷贝,不包含尾指针的字符
static void EC20Util_MemCopyWithStartPtrAndEndPtr(uint8_t* dstMem,char* startPtr,char* endPtr,uint16_t maxSize)
{
    uint16_t index = 0;
    //序号不对,清除掉
    if(startPtr >= endPtr)return;
    //数据超过最大长度,清除掉
    if(endPtr - startPtr >= maxSize)return;
    while(startPtr < endPtr)
    {
        dstMem[index] = *startPtr;
        startPtr++;
        index++;
    }
}

//EC20发送设置波特率指令
EC20_RESULT EC20_SetUartBaud(uint32_t baud)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    //清除接收
    EC20Util_ClearRecvBuffer();
    //设置波特率
    EC20_Printf("%s%d\r\n",EC20_CMD_SET_MODULE_BAUD,baud);
    //等待返回
    result = EC20Util_WaitModuleResponse(AT_COMMAND_WAIT_RESPONSE_TIME_MS);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //检查返回格式
    result = EC20Util_CheckResultStrContainSubStr("OK");
    if(result == EC20_RESULT_OK)
    {
        //设置成功的话,需要将本机波特率改成和设置的波特率同样的波特率,防止后续通讯不上
        EC20_CHANGE_BAUD(baud);        
    }
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//EC20保存用户配置讯息
EC20_RESULT EC20_SaveCurrentUserConfig(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    //保存配置讯息
    result = EC20Util_SendCommandCheckResult(EC20_CMD_SAVE_USER_CONFIG,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,2);
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//EC20开关回显
EC20_RESULT EC20_OpenCloseUartCmdEcho(EC20_FUNC echoFunc)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    if(echoFunc == EC20_FUNC_DISABLE)
    {
        //关闭回显
        result = EC20Util_SendCommandCheckResult(EC20_CMD_CLOSE_AT_ECHO,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,2);
    }
    else
    {
        //打开回显
        result = EC20Util_SendCommandCheckResult(EC20_CMD_OPEN_AT_ECHO,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,2);
    }
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//EC20检测是否存在,指定波特率
EC20_RESULT EC20Util_CheckModuleExistSub(uint32_t baud)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    //切换无硬件流控制波特率
    EC20_CHANGE_BAUD(baud);
    //发送指令等待返回
    result = EC20Util_SendCommandCheckResult(EC20_CMD_CHECK_MODULE_EXIST,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,3);
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//检测EC20模块是否准备好
EC20_RESULT EC20Util_CheckModuleReady(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    //发送指令
    result = EC20Util_SendCommandCheckResult(EC20_CMD_CHECK_MODULE_READY,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,1);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //检查返回,是否准备好
    if(EC20_RESULT_OK == EC20Util_CheckResultStrContainSubStr("+CPAS: 0"))
    {
        result = EC20_RESULT_OK;
    }
    else if(EC20_RESULT_OK == EC20Util_CheckResultStrContainSubStr("+CPAS: 3"))
    {
        //检查返回,是否正在响铃
        result = EC20_RESULT_MODULE_RINGING;
    }
    else if(EC20_RESULT_OK == EC20Util_CheckResultStrContainSubStr("+CPAS: 4"))
    {
        //检查返回,是否正在通话或者拨号
        result = EC20_RESULT_MODULE_CALLING_OR_HOLD;
    }
    else
    {
        result = EC20_RESULT_MODULE_NOT_RDY;
    }
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//检测EC20卡是否准备好
EC20_RESULT EC20Util_CheckSIMCardReady(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    //发送指令,检查卡是否初始化
    result = EC20Util_SendCommandCheckResult(EC20_CMD_CHECK_SIM_CARD_INIT_READY,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,3);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //检查返回,是否准备好
    if(EC20_RESULT_OK == EC20Util_CheckResultStrContainSubStr("+QINISTAT: 7"))
    {
        result = EC20_RESULT_OK;//7是准备好,0是正在初始化,1是PIN已经初始化,2是短信SMS初始化,4是电话本初始化
    }
    else
    {
        result = EC20_RESULT_SIM_CARD_NOT_INIT;
    }
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //发送指令,检查卡有没有解锁
    result = EC20Util_SendCommandCheckResult(EC20_CMD_CHECK_SIM_CARD_PIN_READY,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,3);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //检查返回
    if(EC20_RESULT_OK == EC20Util_CheckResultStrContainSubStr("ERROR"))
    {
        //SIM卡不存在,报错
        result = EC20_RESULT_SIM_CARD_NOT_EXIST; 
    }
    else if(EC20_RESULT_OK == EC20Util_CheckResultStrContainSubStr("READY"))
    {
        //卡密码PIN初始化完成
        result = EC20_RESULT_OK;
    }
    else
    {
        //卡没有准备好,需要密码才能工作
        result = EC20_RESULT_SIM_CARD_NOT_RDY;
    }
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//设置网络注册格式,返回注册小区号码
EC20_RESULT EC20_SetNetWorkRegisterFormat(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    result = EC20Util_SendCommandCheckResult(EC20_CMD_SET_NOTWORK_REGISTER_FORMAT,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,3);
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//检测EC20网络注册是否完成
EC20_RESULT EC20Util_CheckNetRegisterReady(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    //先设置注册格式
    result = EC20_SetNetWorkRegisterFormat();
    if(result != EC20_RESULT_OK)
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //发送指令,检查网络注册情况
    result = EC20Util_SendCommandCheckResult(EC20_CMD_CHECK_NETWORK_REGISTERED,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,3);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //检查注册结果
    if(EC20_RESULT_OK == EC20Util_CheckResultStrContainSubStr("+CREG: 2,1"))
    {
        result = EC20_RESULT_OK;//本地网络
    }
    else if(EC20_RESULT_OK == EC20Util_CheckResultStrContainSubStr("+CREG: 2,5"))
    {
        result = EC20_RESULT_OK;//漫游网络
    }
    else if(EC20_RESULT_OK == EC20Util_CheckResultStrContainSubStr("+CREG: 2,3"))
    {
        result = EC20_RESULT_NET_REGISTER_DENIED;//网络注册被拒绝
    }
    else
    {
        result = EC20_RESULT_NET_REGISTER_NOT_INIT;
    }
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//获取EC20网络注册基站信息,LAC基站位置区域码以及CID基站编号
EC20_RESULT EC20Util_CheckNetBaseStationMsg(void)
{
    //首先检测网络是否注册
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    //先检查是否准备好
    result = EC20Util_CheckNetRegisterReady();
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //解析相关信息
    char* startPos = NULL;
    char* endPos = NULL;
    //解析基站位置区域码
    //先找第一个
    startPos = strchr((const char*)serialRecvDataBuffer, '"');
    if(startPos == NULL)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    startPos++;
    //再找第二个
    endPos = strchr((const char*)startPos, '"');
    if(endPos == NULL)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    memset(ec20Message.baseStationCodeStringLAC,0,LENGTH_EC20_CODE_STRING);
    EC20Util_MemCopyWithStartPtrAndEndPtr(ec20Message.baseStationCodeStringLAC,startPos,endPos,LENGTH_EC20_CODE_STRING);
    //获取第二个起始结束,基站编号
    startPos = endPos+1;
    startPos = strchr((const char*)startPos, '"');
    if(startPos == NULL)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    startPos++;
    endPos = strchr((const char*)startPos, '"');
    if(endPos == NULL)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    memset(ec20Message.baseStationCodeStringCID,0,LENGTH_EC20_CODE_STRING);
    EC20Util_MemCopyWithStartPtrAndEndPtr(ec20Message.baseStationCodeStringCID,startPos,endPos,LENGTH_EC20_CODE_STRING);
    EC20_DEVICE_POST_MUTEX();
    return EC20_RESULT_OK;
}

//检测EC20的GPS附着是否完成
EC20_RESULT EC20Util_CheckGprsAttachReady(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    result = EC20Util_SendCommandCheckResult(EC20_CMD_CHECK_GPS_ATTACH,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,3);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    if(EC20_RESULT_OK == EC20Util_CheckResultStrContainSubStr("+CGATT: 1"))
    {
        result = EC20_RESULT_OK;//GPRS网络附着成功
    }
    else if(EC20_RESULT_OK == EC20Util_CheckResultStrContainSubStr("+CGATT: 0"))
    {
        result = EC20_RESULT_GPRS_ATTACH_FAILED;//GPRS网络附着失败
    }
    else
    {
        result = EC20_RESULT_GPRS_ATTACH_UNKNOW;//GPRS网络附着未知错误
    }
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//配置GPRS数据网点接入
EC20_RESULT EC20_SetGprsConfig(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    result = EC20Util_SendCommandCheckResult(EC20_CMD_SET_GPRS_APN_POINT,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,3);
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//关闭GPRS移动场景
EC20_RESULT EC20_DisableGprs(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    result = EC20Util_SendCommandCheckResult(EC20_CMD_DEACT_GPRS,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS*10,1);
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//打开GPRS移动场景
EC20_RESULT EC20_EnableGprs(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    result = EC20Util_SendCommandCheckResult(EC20_CMD_ACT_GPRS,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS*10,1);
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//测试服务器能否ping通,可以看有没有网络
EC20_RESULT EC20_CheckNetPingServer(void)
{
    uint32_t timeoutCount = 0;
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    result = EC20Util_SendCommandCheckResult(EC20_CMD_PING_SERVER,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS*10,1);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    do
    {
        //检查是否有正常的返回
        result = EC20Util_CheckResultStrContainSubStr("+QPING: 0,4,4,0");
        if(result == EC20_RESULT_OK)
        {
            EC20_DEVICE_POST_MUTEX();
            return EC20_RESULT_OK;
        }
        //没有返回,等待一下
        EC20_WAIT_RESPONSE(AT_COMMAND_WAIT_RESPONSE_TIME_MS);
        timeoutCount += AT_COMMAND_WAIT_RESPONSE_TIME_MS;
    } while (timeoutCount <= 4000);
    EC20_DEVICE_POST_MUTEX();
    //超时失败
    return EC20_RESULT_PING_BAIDU_FAIL;
}

//发送指令获取厂家信息
EC20_RESULT EC20_GetManufacturerDesp(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    result = EC20Util_SendCommandCheckResult(EC20_CMD_QUERY_MANUFACTURER_STR,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,3);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //后续数据处理
    //跳过回车换行,查找第一个不带回车换行的开始
    uint16_t startPos = strspn((const char*)serialRecvDataBuffer,"\r\n");
    //接下来用回车换行切出一行
    char* subString = strtok((char*)serialRecvDataBuffer+startPos,(char*)"\r\n");
    if(subString != NULL)
    {
        memset(ec20Message.manufacturerString,0,LENGTH_EC20_CODE_STRING);
        strcpy((char*)ec20Message.manufacturerString,subString);
    }
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//发送指令获取模块类型信息
EC20_RESULT EC20_GetModuleTypeDesp(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    result = EC20Util_SendCommandCheckResult(EC20_CMD_QUERY_MODULE_TYPE_STR,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,3);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //后续数据处理
    //跳过回车换行,查找第一个不带回车换行的开始
    uint16_t startPos = strspn((const char*)serialRecvDataBuffer,"\r\n");
    //接下来用回车换行切出一行
    char* subString = strtok((char*)serialRecvDataBuffer+startPos,(char*)"\r\n");
    if(subString != NULL)
    {
        memset(ec20Message.moduleTypeString,0,LENGTH_EC20_CODE_STRING);
        strcpy((char*)ec20Message.moduleTypeString,subString);
    }
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//发送指令获取软件版本讯息
EC20_RESULT EC20_GetSoftwareVersionDesp(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    result = EC20Util_SendCommandCheckResult(EC20_CMD_QUERY_SOFTWARE_VERSION_STR,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,3);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //后续数据处理
    //跳过回车换行,查找第一个不带回车换行的开始
    uint16_t startPos = strspn((const char*)serialRecvDataBuffer,"\r\n");
    //接下来用回车换行切出一行
    char* subString = strtok((char*)serialRecvDataBuffer+startPos,(char*)"\r\n");
    if(subString != NULL)
    {
        memset(ec20Message.softwareVersion,0,LENGTH_EC20_CODE_STRING);
        strcpy((char*)ec20Message.softwareVersion,subString);
    }
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//发送指令获取IMEI
EC20_RESULT EC20_GetIMEICodeDesp(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    result = EC20Util_SendCommandCheckResult(EC20_CMD_QUERY_MODULE_IMEI_STR,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,3);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //后续数据处理
    //跳过回车换行,查找第一个不带回车换行的开始
    uint16_t startPos = strspn((const char*)serialRecvDataBuffer,"\r\n");
    //接下来用回车换行切出一行
    char* subString = strtok((char*)serialRecvDataBuffer+startPos,(char*)"\r\n");
    if(subString != NULL)
    {
        memset(ec20Message.imeiCodeString,0,LENGTH_EC20_CODE_STRING);
        strcpy((char*)ec20Message.imeiCodeString,subString);
    }
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//发送指令获取SIM卡IMSI串号
EC20_RESULT EC20_GetIMSICodeDesp(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    result = EC20Util_SendCommandCheckResult(EC20_CMD_QUERY_SIM_IMSI_STR,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,3);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //后续数据处理
    //跳过回车换行,查找第一个不带回车换行的开始
    uint16_t startPos = strspn((const char*)serialRecvDataBuffer,"\r\n");
    //接下来用回车换行切出一行
    char* subString = strtok((char*)serialRecvDataBuffer+startPos,(char*)"\r\n");
    if(subString != NULL)
    {
        memset(ec20Message.imsiCodeString,0,LENGTH_EC20_CODE_STRING);
        strcpy((char*)ec20Message.imsiCodeString,subString);
    }
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//发送指令获取SIM卡ICCID讯息
EC20_RESULT EC20_GetICCIDCodeDesp(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    result = EC20Util_SendCommandCheckResult(EC20_CMD_QUERY_SIM_ICCID_STR,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,3);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //后续数据处理
    //跳过回车换行,查找第一个不带指定前缀的开始
    uint16_t startPos = strspn((const char*)serialRecvDataBuffer,"\r\n+QCCID: ");
    //接下来用回车换行切出一行
    char* subString = strtok((char*)serialRecvDataBuffer+startPos,(char*)"\r\n");
    if(subString != NULL)
    {
        memset(ec20Message.iccidCodeString,0,LENGTH_EC20_CODE_STRING);
        strcpy((char*)ec20Message.iccidCodeString,subString);
    }
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//发送指令获取网络全名,短名,供应商讯息
EC20_RESULT EC20_GetNetNameDesp(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    char* startPos = NULL;
    char* endPos = NULL;
    result = EC20Util_SendCommandCheckResult(EC20_CMD_QUERY_NET_NAME,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,3);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //后续数据处理
    //获取第一个起始字符和结束字符,解析网络供应商长名称
    startPos = strchr((const char*)serialRecvDataBuffer, '"');
    if(startPos == NULL)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    startPos++;
    endPos = strchr((const char*)startPos, '"');
    if(endPos == NULL)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    memset(ec20Message.fullNetWorkNameString,0,LENGTH_EC20_CODE_STRING);
    EC20Util_MemCopyWithStartPtrAndEndPtr(ec20Message.fullNetWorkNameString,startPos,endPos,LENGTH_EC20_CODE_STRING);
    //获取第二个起始结束,解析网络短名称
    startPos = endPos+1;
    startPos = strchr((const char*)startPos, '"');
    if(startPos == NULL)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    startPos++;
    endPos = strchr((const char*)startPos, '"');
    if(endPos == NULL)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    memset(ec20Message.shortNetworkNameString,0,LENGTH_EC20_CODE_STRING);
    EC20Util_MemCopyWithStartPtrAndEndPtr(ec20Message.shortNetworkNameString,startPos,endPos,LENGTH_EC20_CODE_STRING);
    //获取第三个起始位置,解析网络服务提供商名称
    startPos = endPos+1;
    startPos = strchr((const char*)startPos, '"');
    if(startPos == NULL)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    startPos++;
    endPos = strchr((const char*)startPos, '"');
    if(endPos == NULL)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    memset(ec20Message.netServiceProviderNameString,0,LENGTH_EC20_CODE_STRING);
    EC20Util_MemCopyWithStartPtrAndEndPtr(ec20Message.netServiceProviderNameString,startPos,endPos,LENGTH_EC20_CODE_STRING);
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//发送指令获取网络类型和波段选择
EC20_RESULT EC20_GetNetTypeDesp(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    char* startPos = NULL;
    char* endPos = NULL;
    result = EC20Util_SendCommandCheckResult(EC20_CMD_QUERY_NET_TYPE,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,3);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //后续数据处理
    //获取第一个起始字符和结束字符,解析网络制式
    startPos = strchr((const char*)serialRecvDataBuffer, '"');
    if(startPos == NULL)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    startPos++;
    endPos = strchr((const char*)startPos, '"');
    if(endPos == NULL)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    memset(ec20Message.networkModeString,0,LENGTH_EC20_CODE_STRING);
    EC20Util_MemCopyWithStartPtrAndEndPtr(ec20Message.networkModeString,startPos,endPos,LENGTH_EC20_CODE_STRING);
    //第二个解析掉,不要
    startPos = endPos+1;
    startPos = strchr((const char*)startPos, '"');
    if(startPos == NULL)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    startPos++;
    endPos = strchr((const char*)startPos, '"');
    if(endPos == NULL)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    //第三个需要,网络波段
    startPos = endPos+1;
    startPos = strchr((const char*)startPos, '"');
    if(startPos == NULL)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    startPos++;
    endPos = strchr((const char*)startPos, '"');
    if(endPos == NULL)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    memset(ec20Message.networkBandString,0,LENGTH_EC20_CODE_STRING);
    EC20Util_MemCopyWithStartPtrAndEndPtr(ec20Message.networkBandString,startPos,endPos,LENGTH_EC20_CODE_STRING);
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//获取信号强度,前一个是信号强度,后一个是误码率
EC20_RESULT EC20_GetSignalStrength(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    int signalStrength;
    int errorValue;
    result = EC20Util_SendCommandCheckResult(EC20_CMD_QUERY_SIGNAL_STRENGTH,"OK",AT_COMMAND_WAIT_RESPONSE_TIME_MS,3);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    char strPrefix[20];char strSuffix[20];
    int resultSscanf = 0;
    resultSscanf = sscanf((char const*)serialRecvDataBuffer, "%s %d,%d  %s", strPrefix, &signalStrength, &errorValue, strSuffix );
    if(resultSscanf == 0)
    {
        EC20_DEVICE_POST_MUTEX();
        //字符串格式错误,解析不出来
        return EC20_RESULT_STR_FORMAT;
    }
    ec20Message.singalStrength = signalStrength;
    ec20Message.errorValue = errorValue;
    EC20_DEVICE_POST_MUTEX();
    return EC20_RESULT_OK;
}

//TCPIP连接网络
EC20_RESULT EC20_SocketConnectServicePort(uint8_t socketPort,uint8_t ip1,uint8_t ip2,uint8_t ip3,uint8_t ip4,uint16_t remotePort)
{
    uint16_t timeoutCount = 0;
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    //清除接收
    EC20Util_ClearRecvBuffer();
    //发送联机指令
    EC20_Printf("%s,%d,\"TCP\",\"%d.%d.%d.%d\",%d,0,0\r\n",EC20_CMD_SOCKET_CONNECT,socketPort,ip1,ip2,ip3,ip4,remotePort);
    //等待返回
    result = EC20Util_WaitModuleResponse(AT_COMMAND_WAIT_RESPONSE_TIME_MS*10);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //检查返回格式
    result = EC20Util_CheckResultStrContainSubStr("OK");
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //循环等待响应结果
    do
    {
        //检查是否有正常的返回
        result = EC20Util_CheckResultStrContainSubStr("+QIOPEN: 0,0");
        if(result == EC20_RESULT_OK)
        {
            EC20_DEVICE_POST_MUTEX();
            return EC20_RESULT_OK;
        }
        //检查是否有异常的返回,Socket已经被打开了
        result = EC20Util_CheckResultStrContainSubStr("+QIOPEN: 0,563");
        if(result == EC20_RESULT_OK)
        {
            EC20_DEVICE_POST_MUTEX();
            return EC20_RESULT_SOCKET_OPEN_EXIST;
        }
        //没有返回,等待一下
        EC20_WAIT_RESPONSE(AT_COMMAND_WAIT_RESPONSE_TIME_MS);
        timeoutCount += AT_COMMAND_WAIT_RESPONSE_TIME_MS;
    } while (timeoutCount <= 4000);
    EC20_DEVICE_POST_MUTEX();
    //超时失败
    return EC20_RESULT_SOCKET_CONNECT_TIME_OUT;
}

//TCPIP网络断开
EC20_RESULT EC20_SocketDisconnectServicePort(uint8_t socketPort)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    //清除接收
    EC20Util_ClearRecvBuffer();
    uint8_t retryCount = 0;
    do
    {
        //设置波特率
        EC20_Printf("%s%d\r\n",EC20_CMD_SOCKET_DISCONNECT,socketPort);
        //等待返回,Socket关闭的等待时间格外长
        result = EC20Util_WaitModuleResponse(5000);
        if(result == EC20_RESULT_OK)
        {
            break;
        }
        retryCount++;
    }while(retryCount < 3);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //检查返回格式
    result = EC20Util_CheckResultStrContainSubStr("OK");
    EC20_DEVICE_POST_MUTEX();
    return result;
}

//EC20检查数据是否发送完成
EC20_RESULT EC20Util_WaitModuleDataSendResponse(uint16_t maxWaitTime)
{
    uint16_t timeCountMs = 0;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    //循环检测接收是否完成
    do
    {
        EC20_WAIT_RESPONSE(5);
        timeCountMs += 5;
        if(serialRecvDataLength != 0)
        {
            if(-1 != ADT_KMP_Buffer((const char*)serialRecvDataBuffer,serialRecvDataLength, "SEND OK"))
            {
                EC20_DEVICE_POST_MUTEX();
                return EC20_RESULT_OK;
            }
            else if(-1 != ADT_KMP_Buffer((const char*)serialRecvDataBuffer,serialRecvDataLength, "SEND FAIL"))
            {
                EC20_DEVICE_POST_MUTEX();
                return EC20_RESULT_OK;
            }
            else if(-1 != ADT_KMP_Buffer((const char*)serialRecvDataBuffer,serialRecvDataLength, "ERROR"))
            {
                EC20_DEVICE_POST_MUTEX();
                return EC20_RESULT_OK;
            }
        }
        //循环检测
    }while(timeCountMs <= maxWaitTime);
    EC20_DEVICE_POST_MUTEX();
    return EC20_RESULT_RECV_ZERO;
}

//获取当前连接状态
EC20_RESULT EC20_SocketCheckConnectState(uint8_t socketNumber,EC20_CONNECT_STATE state)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    //清除接收
    EC20Util_ClearRecvBuffer();
    //查询状态指令
    EC20_Printf("%s%d\r\n",EC20_CMD_CHECK_CONNECT_STATE,socketNumber);
    //+QISTATE: 0,"TCP","47.99.148.230",80,5965,4,1,0,0,"usbat"
    //OK
    return result;
}

//TCPIP发送数据
EC20_RESULT EC20_SocketSendBufferMsg(uint8_t socketPort,uint8_t* msgBuffer,uint16_t msgLength)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    uint8_t packCount = 1+ (msgLength/EC20_RECV_DATA_BUFFER_MAX);
    if(msgLength == 0)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_OK;
    }
    //当前传输的数据报数和当前要传输的数据长度
    uint8_t currentPackCount = 0;
    uint16_t currentSendPackLength = 0;
    //支持多包发送
    do
    {
        //计算本次发送数据
        if(currentPackCount == packCount-1)//发送数据报序号和总序号相等,将数据发完
            currentSendPackLength = msgLength%EC20_RECV_DATA_BUFFER_MAX;
        else
            currentSendPackLength = EC20_RECV_DATA_BUFFER_MAX;
        //清除接收
        EC20Util_ClearRecvBuffer();
        //设置波特率
        EC20_Printf("%s%d,%d\r\n",EC20_CMD_SOCKET_SEND_DATA,socketPort,currentSendPackLength);
        //等待>出现
        result = EC20Util_WaitModuleResponse(AT_COMMAND_WAIT_RESPONSE_TIME_MS);
        if(result == EC20_RESULT_CMD_ERR)//收到ERROR标识端口未连接
        {
            EC20_DEVICE_POST_MUTEX();
            return EC20_RESULT_SOCKET_NOT_CONNECT;
        }
        if(result != EC20_RESULT_OK)
        {
            EC20_DEVICE_POST_MUTEX();
            return result;
        }
        //检查返回格式
        result = EC20Util_CheckResultStrContainSubStr(">");
        if(result != EC20_RESULT_OK)
        {
            EC20_DEVICE_POST_MUTEX();
            return EC20_RESULT_SOCKET_SEND_NO_PESP;
        }
        //发送数据
        EC20_SEND_BUFFER(msgBuffer+(currentPackCount*EC20_RECV_DATA_BUFFER_MAX),currentSendPackLength);
        //等待结果
        result = EC20Util_WaitModuleDataSendResponse(AT_COMMAND_WAIT_RESPONSE_TIME_MS*10);
        if(result != EC20_RESULT_OK)
        {
            EC20_DEVICE_POST_MUTEX();
            return EC20_RESULT_SOCKET_SEND_TIME_OUT;
        }
        //检查返回格式
        result = EC20Util_CheckResultStrContainSubStr("SEND FAIL");
        if(result == EC20_RESULT_OK)
        {
            EC20_DEVICE_POST_MUTEX();
            return EC20_RESULT_SOCKET_SEND_FAIL;
        }
        //检查返回格式
        result = EC20Util_CheckResultStrContainSubStr("ERROR");
        if(result == EC20_RESULT_OK)
        {
            EC20_DEVICE_POST_MUTEX();
            return EC20_RESULT_SOCKET_SEND_ERROR;
        }
        //检查返回格式
        result = EC20Util_CheckResultStrContainSubStr("SEND OK");
        if(result != EC20_RESULT_OK)
        {
            EC20_DEVICE_POST_MUTEX();
            return EC20_RESULT_SOCKET_SEND_UNKNOW;
        }
        //正常
        currentPackCount++;
    } while (currentPackCount < packCount);
    EC20_DEVICE_POST_MUTEX();
    return EC20_RESULT_OK;
}

//TCPIP检查当前当前缓冲区数据长度
EC20_RESULT EC20_SocketCheckWaitReadDataLength(uint8_t socketPort,uint16_t* needReadCount)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    //清除接收
    EC20Util_ClearRecvBuffer();
    //查询接收缓存区指令
    EC20_Printf("%s%d,0\r\n",EC20_CMD_SOCKET_CHECK_RECV_LENGTH,socketPort);
    //等待返回
    result = EC20Util_WaitModuleResponse(AT_COMMAND_WAIT_RESPONSE_TIME_MS);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //检查返回格式,包含OK
    result = EC20Util_CheckResultStrContainSubStr("OK");
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //检查返回格式,包含指定返回
    result = EC20Util_CheckResultStrContainSubStr("+QIRD: ");
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //首先找起始位置
    int32_t findIndex = 0;
    findIndex = ADT_KMP_Buffer((const char*)serialRecvDataBuffer,serialRecvDataLength,(const char*)"+QIRD: ");
    if(-1 == findIndex)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    char* startPos = (char*)(serialRecvDataBuffer+findIndex);
    //然后从起始位置找结束位置
    findIndex = ADT_KMP_Buffer((const char*)startPos,serialRecvDataLength-findIndex,(const char*)"\r\n");
    if(-1 == findIndex)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    char* endPos = startPos+findIndex;
    //将结束位置设置为结束符
    *endPos = '\0';
    //进行匹配
    char strPrefix[20];
    int resultSscanf = 0;
    int totalRecvLength = 0,hasReadLength = 0,unreadLength = 0;
    resultSscanf = sscanf((char const*)startPos, "%s %d,%d,%d", strPrefix, &totalRecvLength, &hasReadLength, &unreadLength);
    if(resultSscanf == 0)
    {
        EC20_DEVICE_POST_MUTEX();
        //字符串格式错误,解析不出来
        return EC20_RESULT_STR_FORMAT;
    }
    //回传等待读取数据长度
    *needReadCount = unreadLength;
    EC20_DEVICE_POST_MUTEX();
    return EC20_RESULT_OK;
}

//TCPIP清空当前接收缓冲区
EC20_RESULT EC20_SocketClearRecvBuffer(uint8_t socketPort)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    //清除接收
    EC20Util_ClearRecvBuffer();
    //查询接收缓存区指令
    EC20_Printf("%s%d\r\n",EC20_CMD_SOCKET_CLEAR_BUF,socketPort);
    //等待返回
    result = EC20Util_WaitModuleResponse(AT_COMMAND_WAIT_RESPONSE_TIME_MS);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //检查返回格式,包含OK
    result = EC20Util_CheckResultStrContainSubStr("OK");
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //检查返回格式,包含指定返回
    result = EC20Util_CheckResultStrContainSubStr("+QIRD: ");
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    EC20_DEVICE_POST_MUTEX();
    return EC20_RESULT_OK;
}

//TCPIP读取当前缓冲区数据
EC20_RESULT EC20_SocketReadRecvData(uint8_t socketPort,uint16_t needReadLength,uint8_t* readBuffer,uint16_t* realReadLength)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    //清除接收
    EC20Util_ClearRecvBuffer();
    //读取接收缓冲区指令
    EC20_Printf("%s%d,%d\r\n",EC20_CMD_SOCKET_READ_DATA,socketPort,needReadLength);
    //等待返回
    result = EC20Util_WaitModuleResponse(AT_COMMAND_WAIT_RESPONSE_TIME_MS);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //检查返回格式,包含OK
    result = EC20Util_CheckResultStrContainSubStr("OK");
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //检查返回格式,包含指定返回
    result = EC20Util_CheckResultStrContainSubStr("+QIRD: ");
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //查找数据开始位置
    int32_t findIndex = 0;
    findIndex = ADT_KMP_Buffer((const char*)serialRecvDataBuffer,serialRecvDataLength,(const char*)"+QIRD:");
    if(-1 == findIndex)
    {
         EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    //匹配数据
    int resultSscanf = 0;
    int realReadLengthLocal = 0;
    resultSscanf = sscanf((char const*)serialRecvDataBuffer+findIndex, "+QIRD: %d\r\n", &realReadLengthLocal);
    if(resultSscanf == 0)
    {
        EC20_DEVICE_POST_MUTEX();
        //字符串格式错误,解析不出来
        return EC20_RESULT_STR_FORMAT;
    }
    //获取数据长度
    *realReadLength = (uint16_t)realReadLengthLocal;
    if(realReadLengthLocal == 0)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_OK;
    }
    //查找数据开始位置
    findIndex = ADT_KMP_Buffer((const char*)serialRecvDataBuffer,serialRecvDataLength,(const char*)"+QIRD:");
    if(-1 == findIndex)
    {
         EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    char* startPos = (char*)(serialRecvDataBuffer+findIndex);
    //查找回车换行的位置
    findIndex = ADT_KMP_Buffer((const char*)startPos,serialRecvDataLength - findIndex,(const char*)"\r\n");
    if(-1 == findIndex)
    {
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_STR_FIND_ERR;
    }
    startPos += findIndex;
    //回车换行过两个字符就是真正接收到的数据
    startPos += 2;
    //开始拷贝数据
    memcpy(readBuffer,startPos,realReadLengthLocal);
    EC20_DEVICE_POST_MUTEX();
    //拷贝完成
    return EC20_RESULT_OK;
}

//查询上次连接是否断开
EC20_RESULT EC20_SocketCheckIsClose(uint8_t* disconnectFlag)
{
    EC20_RESULT result1 = EC20_RESULT_OK;
    EC20_RESULT result2 = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    //检查返回格式,包含OK
    result1 = EC20Util_CheckResultStrContainSubStr("+QIURC: ");
    result2 = EC20Util_CheckResultStrContainSubStr("closed");
    if((result1 == EC20_RESULT_OK)&&(result2 == EC20_RESULT_OK))
    {
        *disconnectFlag = 1;
        EC20_DEVICE_POST_MUTEX();
        return EC20_RESULT_OK;
    }
    *disconnectFlag = 0;
    EC20_DEVICE_POST_MUTEX();
    return EC20_RESULT_OK;
}

//初始化EC20端口
void EC20Util_PortInit(void)
{
    OS_ERR err;
    //创建一个互斥信号量,设备操作
	OSMutexCreate((OS_MUTEX*	)&mutexMCU_EC20,
				  (CPU_CHAR*	)"mutexMCU_EC20",
                  (OS_ERR*		)&err);	
    //电源控制端口初始化
    EC20_INIT_POWER_PORT((BitAction)(VALID_LEVEL_EC20_POWER_PORT));
    //复位端口初始化
    EC20_INIT_RST_PORT((BitAction)(!VALID_LEVEL_EC20_RST_PORT));
    //串口初始化
    EC20_INIT_COMM_PORT(EC20_USE_BPS,EC20Util_DataRecvProc);
    //清空模块讯息
    memset((void*)&ec20Message,0,(sizeof(EC20_MESSAGE)/sizeof(uint8_t)));
}

//控制电源
void EC20Util_PowerControl(EC20_POWER_MODE powerMode)
{
    asm("nop");
}

//模块复位
void EC20Util_ModuleReset(void)
{
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    //复位引脚拉低
    EC20_RST_PORT_SET(VALID_LEVEL_EC20_RST_PORT);
    //延时1000MS
    EC20_WAIT_RESPONSE(1000);
    //释放复位
    EC20_RST_PORT_SET((BitAction)(!VALID_LEVEL_EC20_RST_PORT));
    //等待复位完成
    EC20_WAIT_RESPONSE(100);
    EC20_DEVICE_POST_MUTEX();
}

//EC20检测是否存在,指定波特率
EC20_RESULT EC20Util_CheckModuleExist(void)
{
    EC20_RESULT result = EC20_RESULT_OK;
    EC20_OS_ERR_DEFINE();
    EC20_DEVICE_PEND_MUTEX();
    //首先看看默认波特率是否有返回
    result = EC20Util_CheckModuleExistSub(EC20_USE_BPS);
    if(result == EC20_RESULT_OK)
    {
        //有返回,关闭回显
        result = EC20_OpenCloseUartCmdEcho(EC20_FUNC_DISABLE);
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //如果没有检测到,进行轮询检测
    for(uint8_t index =0 ; index < (sizeof(EC20_SUPPORT_BAUD_ARRAY)/sizeof(EC20_SUPPORT_BAUD_ARRAY[0]));index++)
    {
        //各个波特率试一下看看哪个有返回
        result = EC20Util_CheckModuleExistSub(EC20_SUPPORT_BAUD_ARRAY[index]);
        if(result == EC20_RESULT_OK)break;
    }
    //没检测到,返回错误
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //检查到了,将比特率切换到默认波特率
    result = EC20_SetUartBaud(EC20_USE_BPS);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //检查一下有没有切换过来
    result = EC20Util_CheckModuleExistSub(EC20_USE_BPS);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //关闭模块回显
    result = EC20_OpenCloseUartCmdEcho(EC20_FUNC_DISABLE);
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    //保存配置讯息
    result = EC20_SaveCurrentUserConfig();
    if(result != EC20_RESULT_OK)
    {
        EC20_DEVICE_POST_MUTEX();
        return result;
    }
    EC20_DEVICE_POST_MUTEX();
    return result;
}




































