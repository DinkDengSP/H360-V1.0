/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-17 21:47:56
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-20 13:26:32
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppParam.h"

//系统指针
static APP_PARAM* appParamPtr;

//App写入单个参数的指针
typedef LH_ERR (*AppParamWriteParamSingleFuncPtr)(uint8_t subIndex,int32_t writeParam);
//App读取单个参数的指针
typedef LH_ERR (*AppParamReadParamSingleFuncPtr)(uint8_t subIndex,int32_t* readParam);

//APP参数系统初始化
void AppParamInit(void)
{
    //释放APP内存
    if(appParamPtr != NULL)
    {
        UserMemFree(MEM_D3_SRAM4,appParamPtr);
    }
    //申请新的内存
    appParamPtr = UserMemMallocWhileSuccess(MEM_D3_SRAM4,LENGTH_APP_PARAM);
    //从铁电存储器中加载系统参数
    //从Flash中加载参数
    BoardMB85RS2MT_ReadBuffer(BOARD_MB85RS2MT_APP_PARAM_REGION_MIN,(uint8_t*)appParamPtr,LENGTH_APP_PARAM);
    //数据校验
    if(appParamPtr->paramFlag != APP_PARAM_DEFAULT_FLAG)
    {
        //加载默认值到内存中
        AppParamSetDefault();
    }
}

//APP参数系统设置为默认值
void AppParamSetDefault(void)
{
    //设定为默认配置
    appParamPtr->paramFlag = APP_PARAM_DEFAULT_FLAG;
    //参数装载
    UserMemCopy((void*)(&(appParamPtr->midParam)),(void*)(&midParamDefault),LENGTH_APP_PARAM_MID);
    UserMemCopy((void*)(&(appParamPtr->immParam)),(void*)(&immParamDefault),LENGTH_APP_PARAM_DEVICE_IMM);
    UserMemCopy((void*)(&(appParamPtr->bioParam)),(void*)(&bioParamDefault),LENGTH_APP_PARAM_DEVICE_BIO);
    UserMemCopy((void*)(&(appParamPtr->cloudParam)),(void*)(&cloudParamDefault),LENGTH_APP_PARAM_DEVICE_CLOUD);
    UserMemCopy((void*)(&(appParamPtr->railParam)),(void*)(&railParamDefault),LENGTH_APP_PARAM_DEVICE_RAIL);
    //将装载的参数写入铁电
    BoardMB85RS2MT_WriteBuffer(BOARD_MB85RS2MT_APP_PARAM_REGION_MIN,(uint8_t*)appParamPtr,LENGTH_APP_PARAM);
}


//通用的写入Flash的函数
static void AppParamWriteSingleCommon(int32_t* targetParamPtr,int32_t writeParam)
{
    *targetParamPtr = writeParam;
    int32_t targetAddr = (int32_t)((char*)targetParamPtr);
    int32_t baseAddr = (int32_t)((char*)(appParamPtr));
    //计算偏移
    int32_t offset = targetAddr - baseAddr;
    if(offset < 0)
    {
        //偏移必须是正数
        offset = 0 - offset;
    }
    //写入参数
    BoardMB85RS2MT_WriteBuffer(BOARD_MB85RS2MT_APP_PARAM_REGION_MIN+offset,(uint8_t*)targetParamPtr,4);
}

/*************************************************读写中位机本身参数****************************************************/
LH_ERR AppParamReadMid(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case PARAM_SUB_INDEX_MID_RESERVE:
            *readParam = appParamPtr->midParam.reserve;
            break;
        default:
            *readParam = 0;
            return LH_ERR_PARAM_READ_SUB_INDEX;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}
LH_ERR AppParamWriteMid(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case PARAM_SUB_INDEX_MID_RESERVE:
            targetParamPtr = &(appParamPtr->midParam.reserve);
            break;
        default:
            return LH_ERR_PARAM_WRITE_SUB_INDEX;//不支持的参数
    }
    //参数写入Flash
    AppParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return LH_ERR_NONE;
}

/*************************************************读写免疫相关参数****************************************************/
LH_ERR AppParamReadImm(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case PARAM_SUB_INDEX_IMM_RESERVE:
            *readParam = appParamPtr->immParam.reserve;
            break;
        default:
            *readParam = 0;
            return LH_ERR_PARAM_READ_SUB_INDEX;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}
LH_ERR AppParamWriteImm(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case PARAM_SUB_INDEX_IMM_RESERVE:
            targetParamPtr = &(appParamPtr->immParam.reserve);
            break;
        default:
            return LH_ERR_PARAM_WRITE_SUB_INDEX;//不支持的参数
    }
    //参数写入Flash
    AppParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return LH_ERR_NONE;
}

/*************************************************读写生化相关参数****************************************************/
LH_ERR AppParamReadBio(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case PARAM_SUB_INDEX_BIO_RESERVE:
            *readParam = appParamPtr->bioParam.reserve;
            break;
        default:
            *readParam = 0;
            return LH_ERR_PARAM_READ_SUB_INDEX;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}
LH_ERR AppParamWriteBio(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case PARAM_SUB_INDEX_BIO_RESERVE:
            targetParamPtr = &(appParamPtr->bioParam.reserve);
            break;
        default:
            return LH_ERR_PARAM_WRITE_SUB_INDEX;//不支持的参数
    }
    //参数写入Flash
    AppParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return LH_ERR_NONE;
}

/*************************************************读写立禾云相关参数****************************************************/
LH_ERR AppParamReadCloud(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case PARAM_SUB_INDEX_CLOUD_RESERVE:
            *readParam = appParamPtr->cloudParam.reserve;
            break;
        default:
            *readParam = 0;
            return LH_ERR_PARAM_READ_SUB_INDEX;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}
LH_ERR AppParamWriteCloud(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case PARAM_SUB_INDEX_CLOUD_RESERVE:
            targetParamPtr = &(appParamPtr->cloudParam.reserve);
            break;
        default:
            return LH_ERR_PARAM_WRITE_SUB_INDEX;//不支持的参数
    }
    //参数写入Flash
    AppParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return LH_ERR_NONE;
}

/*************************************************读写轨道相关参数****************************************************/
LH_ERR AppParamReadRail(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case PARAM_SUB_INDEX_RAIL_RESERVE:
            *readParam = appParamPtr->railParam.reserve;
            break;
        default:
            *readParam = 0;
            return LH_ERR_PARAM_READ_SUB_INDEX;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}
LH_ERR AppParamWriteRail(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case PARAM_SUB_INDEX_RAIL_RESERVE:
            targetParamPtr = &(appParamPtr->railParam.reserve);
            break;
        default:
            return LH_ERR_PARAM_WRITE_SUB_INDEX;//不支持的参数
    }
    //参数写入Flash
    AppParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return LH_ERR_NONE;
}

/****************************************参数读取函数指针数组**************************************************/
static const AppParamReadParamSingleFuncPtr AppParamReadParamSingleArray[] = {
    AppParamReadMid,
    AppParamReadImm,
    AppParamReadBio,
    AppParamReadCloud,
    AppParamReadRail,
};
#define COUNT_APP_UTIL_FUNC_READ_PARAM          (sizeof(AppParamReadParamSingleArray)/sizeof(AppParamReadParamSingleArray[0]))

/****************************************参数写入函数指针数组**************************************************/
static const AppParamWriteParamSingleFuncPtr AppParamWriteParamSingleArray[] = {
    AppParamWriteMid,
    AppParamWriteImm,
    AppParamWriteBio,
    AppParamWriteCloud,
    AppParamWriteRail,
};
#define COUNT_APP_UTIL_FUNC_WRITE_PARAM         (sizeof(AppParamWriteParamSingleArray)/sizeof(AppParamWriteParamSingleArray[0]))



//写入参数
LH_ERR AppParamWriteSingle(PARAM_MAIN_INDEX mainIndex,uint8_t subIndex,int32_t paramWrite)
{
    if(mainIndex >= COUNT_APP_UTIL_FUNC_WRITE_PARAM)
    {
        return LH_ERR_PARAM_WRITE_MAIN_INDEX;
    }
    return AppParamWriteParamSingleArray[mainIndex](subIndex,paramWrite);
}

//读取参数
LH_ERR AppParamReadSingle(PARAM_MAIN_INDEX mainIndex,uint8_t subIndex,int32_t* paramRead)
{
    if(mainIndex >= COUNT_APP_UTIL_FUNC_READ_PARAM)
    {
        return LH_ERR_PARAM_READ_MAIN_INDEX;
    }
    return AppParamReadParamSingleArray[mainIndex](subIndex,paramRead);
}
