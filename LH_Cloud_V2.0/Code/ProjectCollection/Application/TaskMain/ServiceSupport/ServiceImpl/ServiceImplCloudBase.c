/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-09-08 11:04:42
**LastEditors: DengXiaoJun
**LastEditTime: 2020-09-08 16:25:15
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplBase.h"
#include "ServiceImplCloudBase.h"

//PING 服务器测试
LH_ERR ServiceImplCloudBase_PingTestNetWork(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    ec20Result = EC20_CheckNetPingServer();
    if(ec20Result != EC20_RESULT_OK)
    {
        errorCode = EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}

//获取模块厂商信息
LH_ERR ServiceImplCloudBase_QueryManufacturerName(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //执行动作
    ec20Result = EC20_GetManufacturerDesp();
    if(ec20Result != EC20_RESULT_OK)
    {
        errorCode = EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}


//获取模块类型信息
LH_ERR ServiceImplCloudBase_QueryModuleType(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //执行动作
    ec20Result = EC20_GetModuleTypeDesp();
    if(ec20Result != EC20_RESULT_OK)
    {
        errorCode = EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}

//获取模块软件版本信息
LH_ERR ServiceImplCloudBase_QuerySoftwareVersion(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //执行动作
    ec20Result = EC20_GetSoftwareVersionDesp();
    if(ec20Result != EC20_RESULT_OK)
    {
        errorCode = EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}

//获取模块入网许可
LH_ERR ServiceImplCloudBase_QueryModuleIMEI(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //执行动作
    ec20Result = EC20_GetIMEICodeDesp();
    if(ec20Result != EC20_RESULT_OK)
    {
        errorCode = EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}

//获取SIM卡IMSI串号
LH_ERR ServiceImplCloudBase_QuerySimIMSI(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //执行动作
    ec20Result = EC20_GetIMSICodeDesp();
    if(ec20Result != EC20_RESULT_OK)
    {
        errorCode = EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}

//获取SIM卡ICCID识别号
LH_ERR ServiceImplCloudBase_QuerySimICCID(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //执行动作
    ec20Result = EC20_GetICCIDCodeDesp();
    if(ec20Result != EC20_RESULT_OK)
    {
        errorCode = EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}

//获取当前模块连接网络全名
LH_ERR ServiceImplCloudBase_QueryNetFullName(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //执行动作
    ec20Result = EC20_GetNetNameDesp();
    if(ec20Result != EC20_RESULT_OK)
    {
        errorCode = EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}

//获取当前模块连接网络短名
LH_ERR ServiceImplCloudBase_QueryNetShortName(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //执行动作
    ec20Result = EC20_GetNetNameDesp();
    if(ec20Result != EC20_RESULT_OK)
    {
        errorCode = EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}

//获取网络服务商名称
LH_ERR ServiceImplCloudBase_QueryNetProvide(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //执行动作
    ec20Result = EC20_GetNetNameDesp();
    if(ec20Result != EC20_RESULT_OK)
    {
        errorCode = EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}

//获取当前网络制式
LH_ERR ServiceImplCloudBase_QueryNetType(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //执行动作
    ec20Result = EC20_GetNetTypeDesp();
    if(ec20Result != EC20_RESULT_OK)
    {
        errorCode = EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}

//获取当前网络波段
LH_ERR ServiceImplCloudBase_QueryNetBaud(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //执行动作
    ec20Result = EC20_GetNetTypeDesp();
    if(ec20Result != EC20_RESULT_OK)
    {
        errorCode = EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}

//获取LAC基站区域码
LH_ERR ServiceImplCloudBase_QueryBaseStationLAC(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //执行动作
    ec20Result = EC20Util_CheckNetBaseStationMsg();
    if(ec20Result != EC20_RESULT_OK)
    {
        errorCode = EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}

//获取CID基站编号
LH_ERR ServiceImplCloudBase_QueryBaseStationCID(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //执行动作
    ec20Result = EC20Util_CheckNetBaseStationMsg();
    if(ec20Result != EC20_RESULT_OK)
    {
        errorCode = EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}

//获取当前信号强度与误码率
LH_ERR ServiceImplCloudBase_QuerySignalStrength(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    EC20_RESULT ec20Result = EC20_RESULT_OK;
    //执行动作
    ec20Result = EC20_GetSignalStrength();
    if(ec20Result != EC20_RESULT_OK)
    {
        errorCode = EC20_MODULE_RESULT_CONVERT(ec20Result);
    }
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}

//查看当前模块网络是否正常
LH_ERR ServiceImplCloudBase_QueryNetState(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //执行动作
    errorCode = BoardEC20_CheckNetWorkReady();
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}

//打开设备GPRS连接
LH_ERR ServiceImplCloudBase_EnableGPRS(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //执行动作
    errorCode = BoardEC20_EnableNetWork();
    //打印错误代码
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
    }
    return errorCode;
}

//通讯设备重启
LH_ERR ServiceImplCloudBase_ModuleRestartIntegrate(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    OS_ERR err;
    //执行动作
    //模块重启
    EC20Util_ModuleReset();
    OS_TICK startTick = OSTimeGet(&err);
    OS_TICK currentTick = OSTimeGet(&err);
    //循环检测重启完成
    do
    {
        //检查模块是否存在
        errorCode = BoardEC20_CheckExist();
        if(errorCode != LH_ERR_NONE)
        {
            //模块不存在,等待下一次继续检测
            CoreDelayMs(1000);
        }
        //更新时间
        currentTick = OSTimeGet(&err);
        if(currentTick - startTick > RESTART_TIME_MS_MAX)
        {
            //超时,不在检测,直接报错
            errorCode= LH_ERR_CLOUD_EC20_CHECK_EXIST_TIME_OUT;
            break;
        }
    }while(errorCode != LH_ERR_NONE);
    //发生错误就返回,一般是超时
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
        return errorCode;
    }
    //循环检测模块网络初始化完成
    do
    {
        errorCode = BoardEC20_CheckNetWorkReady();
        if(errorCode != LH_ERR_NONE)
        {
            //模块不存在,等待下一次继续检测
            CoreDelayMs(1000);
        }
        //更新时间
        currentTick = OSTimeGet(&err);
        if(currentTick - startTick > RESTART_TIME_MS_MAX)
        {
            //超时,不在检测,直接报错
            errorCode = LH_ERR_CLOUD_EC20_NET_READY_TIME_OUT;
            break;
        }
    }while(errorCode != LH_ERR_NONE);
    //发生错误就返回,一般是超时
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
        return errorCode;
    }
    //使能网络连接
    errorCode = BoardEC20_EnableNetWork();
    //发生错误就返回
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
        return errorCode;
    }
    //循环测试网络是否连上
    do
    {
        errorCode = BoardEC20_CheckNetWorkAvailable();
        if(errorCode != LH_ERR_NONE)
        {
            //模块不存在,等待下一次继续检测
            CoreDelayMs(1000);
        }
        //更新时间
        currentTick = OSTimeGet(&err);
        if(currentTick - startTick > RESTART_TIME_MS_MAX)
        {
            //超时,不在检测,直接报错
            errorCode = LH_ERR_CLOUD_EC20_NET_CHECK_AVAILABLE;
            break;
        }
    }while(errorCode != LH_ERR_NONE);
    //发生错误就返回,一般是超时
    if(errorCode != LH_ERR_NONE)
    {
        SystemDebugShowMsgError(errorCode);
        return errorCode;
    }
    return errorCode;
}
