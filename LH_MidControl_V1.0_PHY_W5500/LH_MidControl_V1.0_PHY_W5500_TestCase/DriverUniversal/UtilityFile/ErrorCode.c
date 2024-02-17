/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-10-27 18:09:18 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-10-28 09:02:19 +0800
************************************************************************************************/ 
#include "ErrorCode.h"

//设置错误代码默认值
void ErrorCodeSetDefaultByPtr(ERROR_CODE* errorCodePtr)
{
    errorCodePtr->errorMain = ERROR_MAIN_OK;
    errorCodePtr->errorLevel = ERROR_LEVEL_NONE;
    errorCodePtr->errorSub = ERROR_SUB_OK;
}

//设置错误代码指定值
void ErrorCodeSet(ERROR_CODE* errorCodePtr,ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,ERROR_SUB errorSub)
{
    errorCodePtr->errorMain = errorMain;
    errorCodePtr->errorLevel = errorLevel;
    errorCodePtr->errorSub = errorSub;
}

//设置主错误代码
void ErrorCodeSetMain(ERROR_CODE* errorCodePtr,ERROR_MAIN errorMain)
{
    errorCodePtr->errorMain = errorMain;
}

//设置错误级别
void ErrorCodeSetLevel(ERROR_CODE* errorCodePtr,ERROR_LEVEL errorLevel)
{
    errorCodePtr->errorLevel = errorLevel;
}

//设置子错误代码
void ErrorCodeSetSub(ERROR_CODE* errorCodePtr,ERROR_SUB errorSub)
{
    errorCodePtr->errorSub = errorSub;
}




