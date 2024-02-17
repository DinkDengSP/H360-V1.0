/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-17 11:45:17
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-03 15:53:58
**FilePath: \LH_TC_APP_V2.3\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplParam.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplParam.h"

//可能涉及到错误代码的转换预定义的函数
static LH_ERR ServiceImplParamConvertErrorCode(LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //基础错误代码
    errResult = LH_ERR_BOARD9_TC_BASE;
    //模块号
    uint32_t moduleNo = MODULE_NO_SYSTEM_PARAM;
    moduleNo <<= 24;
    //加入模块号
    errResult |= moduleNo;
    //加入原始错误代码
    errResult |= errSrc;
    //返回转换完成的错误代码
    return errResult;
}

//自动化运行程序初始化
void ServiceImplParam_StateMachineInit(void)
{
    asm("nop");
}


//自动化运行程序循环运转
void ServiceImplParam_StateMachineLoop(void)
{
    asm("nop");
}

//读取参数
LH_ERR ServiceImplParam_Read(SERVICE_PARAM_PARAM_MAIN mainIndex,uint8_t subIndex,float* paramRead)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //调用实际实现
    errorCode = ServiceUtilParam_Read(mainIndex,subIndex,paramRead);
    if(errorCode != LH_ERR_NONE)
    {
        errorCode = ServiceImplParamConvertErrorCode(errorCode);
    }
    return errorCode;
}

//写入参数
LH_ERR ServiceImplParam_Write(SERVICE_PARAM_PARAM_MAIN mainIndex,uint8_t subIndex,float paramWrite)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //调用实际实现
    errorCode = ServiceUtilParam_Write(mainIndex,subIndex,paramWrite);
    if(errorCode != LH_ERR_NONE)
    {
        errorCode = ServiceImplParamConvertErrorCode(errorCode);
    }
    return errorCode;
}
















