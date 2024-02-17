/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-08-08 14:24:33 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-08-09 11:49:03 +0800
************************************************************************************************/ 
#include "SrvParam.h"

//Srv写入单个参数的指针
typedef LH_ERR (*SrvParamWriteParamSingleFuncPtr)(uint8_t subIndex,int32_t writeParam);
//Srv读取单个参数的指针
typedef LH_ERR (*SrvParamReadParamSingleFuncPtr)(uint8_t subIndex,int32_t* readParam);

static SRV_PARAM* srvParamPtr = NULL;

//设置默认值
static void SrvParamSetDefault(void);

//服务层参数初始化
void SrvParamInit(void)
{
    //首先申请内存
    srvParamPtr = NULL;
    //申请内存
    srvParamPtr = UserMemMallocWhileSuccess(MEM_SDRAM1,LENGTH_SRV_PARAM);
    //从Flash中加载参数
    BoardMB85RS2MT_ReadBuffer(BOARD_MB85RS2MT_SRV_PARAM_REGION_MIN,(uint8_t*)srvParamPtr,LENGTH_SRV_PARAM);
    //数据校验
    if(srvParamPtr->initFlag == SRV_PARAM_INIT_FLAG_SPEC)
    {
        //校验通过
        return;
    }
    //校验不通过,设定为默认配置
    SrvParamSetDefault();
}

//设置默认值
static void SrvParamSetDefault(void)
{
    srvParamPtr->initFlag = SRV_PARAM_INIT_FLAG_SPEC;
    UserMemCopy((void*)&(srvParamPtr->netBase),(void*)&srvParamNetIpDefault,LENGTH_SRV_PARAM_NET_BASE);
    //写入
    BoardMB85RS2MT_WriteBuffer(BOARD_MB85RS2MT_SRV_PARAM_REGION_MIN,(uint8_t*)srvParamPtr,LENGTH_SRV_PARAM);
}

//通用的写入Flash的函数
static void SrvParamWriteSingleCommon(int32_t* targetParamPtr,int32_t writeParam)
{
    *targetParamPtr = writeParam;
    int32_t targetAddr = (int32_t)((char*)targetParamPtr);
    int32_t baseAddr = (int32_t)((char*)(srvParamPtr));
    //计算偏移
    int32_t offset = targetAddr - baseAddr;
    if(offset < 0)
    {
        //偏移必须是正数
        offset = 0 - offset;
    }
    //写入参数
    BoardMB85RS2MT_WriteBuffer(BOARD_MB85RS2MT_SRV_PARAM_REGION_MIN+offset,(uint8_t*)targetParamPtr,4);
}

/****************************************网络基础模块模块参数读写**************************************************/
//写入参数
LH_ERR SrvParamWriteNetBase(uint8_t subIndex,int32_t writeParam)
{
    INDEX_SUB_SRV_NET_BASE paramIndex = (INDEX_SUB_SRV_NET_BASE)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case INDEX_SUB_SRV_NET_BASE_IP:
            targetParamPtr = &(srvParamPtr->netBase.ipValue);
            break;
        default:
            return LH_ERR_PARAM_WRITE_SUB_INDEX;//不支持的参数
    }
    //参数写入Flash
    SrvParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return LH_ERR_NONE;
}
//读取参数
LH_ERR SrvParamReadNetBase(uint8_t subIndex,int32_t* readParam)
{
    INDEX_SUB_SRV_NET_BASE paramIndex = (INDEX_SUB_SRV_NET_BASE)subIndex;
    switch(paramIndex)
    {
        case INDEX_SUB_SRV_NET_BASE_IP:
            *readParam = srvParamPtr->netBase.ipValue;
            break;
        default:
            *readParam = 0;
            return LH_ERR_PARAM_READ_SUB_INDEX;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}

/****************************************参数读取函数指针数组**************************************************/
static const SrvParamReadParamSingleFuncPtr SrvParamReadParamSingleArray[] = {
    SrvParamReadNetBase,
};
#define COUNT_SRV_FUNC_READ_PARAM          (sizeof(SrvParamReadParamSingleArray)/sizeof(SrvParamReadParamSingleArray[0]))

/****************************************参数写入函数指针数组**************************************************/
static const SrvParamWriteParamSingleFuncPtr SrvParamWriteParamSingleArray[] = {
    SrvParamWriteNetBase,
};
#define COUNT_SRV_FUNC_WRITE_PARAM         (sizeof(SrvParamWriteParamSingleArray)/sizeof(SrvParamWriteParamSingleArray[0]))


//写入参数
LH_ERR SrvParamWriteSingle(INDEX_MAIN_SRV_PARAM mainIndex,uint8_t subIndex,int32_t paramWrite)
{
    if(mainIndex >= COUNT_SRV_FUNC_WRITE_PARAM)
    {
        return LH_ERR_PARAM_WRITE_MAIN_INDEX;
    }
    return SrvParamWriteParamSingleArray[mainIndex](subIndex,paramWrite);
}

//读取参数
LH_ERR SrvParamReadSingle(INDEX_MAIN_SRV_PARAM mainIndex,uint8_t subIndex,int32_t* paramRead)
{
    if(mainIndex >= COUNT_SRV_FUNC_READ_PARAM)
    {
        return LH_ERR_PARAM_READ_MAIN_INDEX;
    }
    return SrvParamReadParamSingleArray[mainIndex](subIndex,paramRead);
}

//设置系统IP为默认IP
void SrvParamSetNetIpDefault(void)
{
    SrvParamWriteSingle(INDEX_MAIN_NET_BASE,INDEX_SUB_SRV_NET_BASE_IP,SRV_PARAM_NET_IP_DEFAULT);
}