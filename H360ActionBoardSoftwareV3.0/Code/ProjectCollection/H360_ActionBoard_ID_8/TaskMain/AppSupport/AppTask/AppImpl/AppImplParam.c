/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-21 09:22:31
**FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppTask\AppImpl\AppImplParam.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplParam.h"

//写入参数
LH_ERR AppImplParamWrite(uint16_t mainIndex,uint16_t subIndex,int32_t paramWrite)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //检测参数序号是否正确
        if(mainIndex > APP_PARAM_MAIN_INDEX_UTIL)
        {
            return LH_ERR_APP_PARAM_INDEX;
        }

    //写入参数
        errorCode = AppParamWriteParam(mainIndex,subIndex,paramWrite);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }

    return LH_ERR_NONE;
}

//读取参数
LH_ERR AppImplParamRead(uint16_t mainIndex,uint16_t subIndex,int32_t* paramReadPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //检测参数序号是否正确
        if(mainIndex > APP_PARAM_MAIN_INDEX_UTIL)
        {
            return LH_ERR_APP_PARAM_INDEX;
        }

    //读取参数
        errorCode = AppParamReadParam(mainIndex,subIndex,paramReadPtr);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }

    return LH_ERR_NONE;
}

//恢复默认参数
LH_ERR AppImplParamResetDefault(void)
{
    LH_ERR errorCode = LH_ERR_NONE;

    //复位系统参数
        errorCode = AppParamResetDedault();
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }

    return LH_ERR_NONE;
}
