/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-01-13 10:55:30
**LastEditors: DengXiaoJun
**LastEditTime: 2021-01-13 12:14:24
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppUtilParam.h"

static APP_UTIL_PARAM appUtilParam = {0};


//初始化系统参数为默认参数
void AppUtilParamSetDefault(void)
{
    //设定为默认配置
    appUtilParam.paramFlag = APP_PARAM_DEFAULT_FLAG;
    //参数装载
    //板卡型号参数
    UserMemCopy((void*)(&(appUtilParam.boardType)),(void*)(&appUtilParamBoardTypeDefault),LENGTH_APP_UTIL_PARAM_BOARD_TYPE);
    //将设定好的值写回外置Flash
    BoardMB85RS256_WriteBuffer(APP_UTIL_PARAM_BASE_ADDR,(uint8_t*)(&appUtilParam),LENGTH_APP_UTIL_PARAM);
}

//参数系统初始化
void AppParamInit(void)
{
    //从Flash中加载参数
    BoardMB85RS256_ReadBuffer(APP_UTIL_PARAM_BASE_ADDR,(uint8_t*)(&appUtilParam),LENGTH_APP_UTIL_PARAM);
    //数据校验
    if(appUtilParam.paramFlag == APP_PARAM_DEFAULT_FLAG)
    {
        //校验通过
        return;
    }
    //校验不通过,设定为默认配置
    AppUtilParamSetDefault();
}

//读取指定数据
int32_t AppParamReadBoardType(APP_UTIL_PARAM_BOARD_TYPE_INDEX index)
{
    int32_t paramValue = 0;
    if(index == APP_UTIL_PARAM_BOARD_TYPE_INDEX_BOARD_NO)
    {
        paramValue = appUtilParam.boardType.boardNo;
    }
    return paramValue;
}

//写入指定数据
void AppParamWriteBoardType(APP_UTIL_PARAM_BOARD_TYPE_INDEX index,int32_t writeDat)
{
    if(index == APP_UTIL_PARAM_BOARD_TYPE_INDEX_BOARD_NO)
    {
        appUtilParam.boardType.boardNo = writeDat;
        //将设定好的值写回外置Flash
        BoardMB85RS256_WriteBuffer(APP_UTIL_PARAM_BASE_ADDR,(uint8_t*)(&appUtilParam),LENGTH_APP_UTIL_PARAM);
    }
}
