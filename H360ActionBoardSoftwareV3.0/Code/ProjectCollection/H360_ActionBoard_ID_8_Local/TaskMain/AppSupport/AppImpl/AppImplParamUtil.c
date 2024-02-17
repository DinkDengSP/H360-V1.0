/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-11-09 09:06:16
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-09 11:25:02
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplParamUtil.h"

//初始化参数与辅助功能状态机
void AppImplParamUtilFSM_Init(void)
{
    asm("nop");
}

//运行参数与辅助功能状态机
void AppImplParamUtilFSM_Run(void)
{
    asm("nop");
}

//复位板卡上参数为默认值
void AppImplParamUtilResetAllParam(void)
{
    AppParamResetDefault();
}

//读取板卡参数
LH_ERR AppImplParamUtilReadParam(uint8_t mainIndex,uint8_t subIndex,int32_t* readDataPtr)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = AppParamReadParam((APP_PARAM_MAIN)mainIndex,subIndex,readDataPtr);
    return errorCode;
}

//写入板卡指定参数
LH_ERR AppImplParamUtilWriteParam(uint8_t mainIndex,uint8_t subIndex,int32_t writeData)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = AppParamWriteParam((APP_PARAM_MAIN)mainIndex,subIndex,writeData);
    return errorCode;
}
