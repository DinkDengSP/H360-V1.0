/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-17 10:02:18
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-03 14:16:22
**FilePath: \LH_TC_APP_V2.3\TaskMain\ServiceSupport\ServiceUtil\ServiceUtilParam.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceUtilParam.h"

SERVICE_PARAM* serviceParam = NULL;

//函数类型
typedef LH_ERR (*ServiceUtilParam_ReadSubFuncPtr)(uint8_t subIndex,float* paramRead);
typedef LH_ERR (*ServiceUtilParam_WriteSubFuncPtr)(uint8_t subIndex,float paramWrite);

//参数初始化
void ServiceUtilParamInit(void)
{   
    //如果以前数据没清空,这里就清空
    if(serviceParam != NULL)
    {
        UserMemFree(SRAM_IN,serviceParam);
        serviceParam = NULL;
    }

    //申请内存
    do
    {
        serviceParam = UserMemMalloc(SRAM_IN,SIZE_SERVICE_PARAM);
        if(serviceParam == NULL)
        {
            CoreDelayMs(5);
        }
    }while(serviceParam == NULL);
    
    //从存储器中读取数据
    BoardMB85RS2MT_ReadBuffer(SYSTEM_SERVICE_PARAM_ADDR,(uint8_t*)(&(serviceParam->paramUser)),SIZE_SERVICE_PARAM_USER);
    //检查数据是否正确
    if(serviceParam->paramUser.ConfigFlag != SYSTEM_CONFIG_FLAG)
    {
        //配置文件不正确,切换成默认参数
        UserMemCopy((void*)(&(serviceParam->paramUser)),(void*)(&serviceParamUserDeafult),SIZE_SERVICE_PARAM_USER);
        //写入Flash
        BoardMB85RS2MT_WriteBuffer(SYSTEM_SERVICE_PARAM_ADDR,(uint8_t*)(&(serviceParam->paramUser)),SIZE_SERVICE_PARAM_USER);
    }
    //加载系统功能配置,不能写入的那种
    UserMemCopy((void*)(&(serviceParam->paramConfig)),(void*)(&serviceParamConfig),SIZE_SERVICE_PARAM_CONFIG);
}

//参数保存
static void ServiceUtilParamSave(void)
{
    //写入Flash
    BoardMB85RS2MT_WriteBuffer(SYSTEM_SERVICE_PARAM_ADDR,(uint8_t*)(&(serviceParam->paramUser)),SIZE_SERVICE_PARAM_USER);
}


/***********************************************主板控温参数**************************************************/
static LH_ERR ServiceUtilParam_ReadSubBoardTemp(uint8_t subIndex,float* paramRead)
{
    float paramUtil = 0.0;
    switch(subIndex)
    {
        case 0://boardTempMaxLimit
            paramUtil = serviceParam->paramUser.boardTempMaxLimit;
            break;
        case 1://boardTempMinLimit
            paramUtil = serviceParam->paramUser.boardTempMinLimit;
            break;
        default:
            return LH_ERR_SERVICE_PARAM_READ;
    }
    *paramRead = paramUtil;
    return LH_ERR_NONE;
}

static LH_ERR ServiceUtilParam_WriteSubBoardTemp(uint8_t subIndex,float paramWrite)
{
    float paramUtil = 0.0;
    paramUtil = (float)paramWrite;
    switch(subIndex)
    {
        case 0://boardTempMaxLimit
            serviceParam->paramUser.boardTempMaxLimit = paramUtil;
            break;
        case 1://boardTempMinLimit
            serviceParam->paramUser.boardTempMinLimit = paramUtil;
            break;
        default:
            return LH_ERR_SERVICE_PARAM_WRITE;
    }
    //到这里,说明没报错,写入参数
    ServiceUtilParamSave();
    return LH_ERR_NONE;
}

/********************************************环境温度参数*****************************************************/
static LH_ERR ServiceUtilParam_ReadSubEnvir(uint8_t subIndex,float* paramRead)
{
    float paramUtil = 0.0;
    switch(subIndex)
    {
        case 0://envirMaxLimit
            paramUtil = serviceParam->paramUser.envirMaxLimit;
            break;
        case 1://envirMinLimit
            paramUtil = serviceParam->paramUser.envirMinLimit;
            break;
        case 2://envirTempOpenFan
            paramUtil = serviceParam->paramUser.envirTempOpenFan;
            break;
        default:
            return LH_ERR_SERVICE_PARAM_READ;
    }
    *paramRead = paramUtil;
    return LH_ERR_NONE;
}

static LH_ERR ServiceUtilParam_WriteSubEnvir(uint8_t subIndex,float paramWrite)
{
    float paramUtil = 0.0;
    paramUtil = (float)paramWrite;
    switch(subIndex)
    {
        case 0://envirMaxLimit
            serviceParam->paramUser.envirMaxLimit = paramUtil;
            break;
        case 1://envirMinLimit
            serviceParam->paramUser.envirMinLimit = paramUtil;
            break;
        case 2://envirTempOpenFan
            serviceParam->paramUser.envirTempOpenFan = paramUtil;
            break;
        default:
            return LH_ERR_SERVICE_PARAM_WRITE;
    }
    //到这里,说明没报错,写入参数
    ServiceUtilParamSave();
    return LH_ERR_NONE;
}

/********************************************反应盘控温参数*****************************************************/
static LH_ERR ServiceUtilParam_ReadSubPlateReaction(uint8_t subIndex,float* paramRead)
{
    float paramUtil = 0.0;
    switch(subIndex)
    {
        case 0://plateReactionTargetTemp
            paramUtil = serviceParam->paramUser.plateReactionTargetTemp;
            break;
        case 1://plateReactionKP
            paramUtil = serviceParam->paramUser.plateReactionKP;
            break;
        case 2://plateReactionKI
            paramUtil = serviceParam->paramUser.plateReactionKI;
            break;
        case 3://plateReactionKD
            paramUtil = serviceParam->paramUser.plateReactionKD;
            break;
        case 4://plateReactionTempHighThreshold
            paramUtil = serviceParam->paramUser.plateReactionTempHighThreshold;
            break;
        case 5://plateReactionTempLowThreshold
            paramUtil = serviceParam->paramUser.plateReactionTempLowThreshold;
            break;
        case 6://plateReactionMaxPower
            paramUtil = serviceParam->paramUser.plateReactionMaxPower;
            break;
        case 7://plateReactionMinPower
            paramUtil = serviceParam->paramUser.plateReactionMinPower;
            break;
        case 8://plateReactionProtectHighTemp
            paramUtil = serviceParam->paramUser.plateReactionProtectHighTemp;
            break;
        case 9://plateReactionProtectLowTemp
            paramUtil = serviceParam->paramUser.plateReactionProtectLowTemp;
            break;
        case 10://plateReactionKI_TimeSetSecond
            paramUtil = serviceParam->paramUser.plateReactionKI_TimeSetSecond;
            break;
        case 11://plateReactionKD_TimeSetSecond
            paramUtil = serviceParam->paramUser.plateReactionKD_TimeSetSecond;
            break;
        default:
            return LH_ERR_SERVICE_PARAM_READ;
    }
    *paramRead = paramUtil;
    return LH_ERR_NONE;
}

static LH_ERR ServiceUtilParam_WriteSubPlateReaction(uint8_t subIndex,float paramWrite)
{
    float paramUtil = 0.0;
    paramUtil = (float)paramWrite;
    switch(subIndex)
    {
        case 0://plateReactionTargetTemp
            serviceParam->paramUser.plateReactionTargetTemp = paramUtil;
            break;
        case 1://plateReactionKP
            serviceParam->paramUser.plateReactionKP = paramUtil;
            break;
        case 2://plateReactionKI
            serviceParam->paramUser.plateReactionKI = paramUtil;
            break;
        case 3://plateReactionKD
            serviceParam->paramUser.plateReactionKD = paramUtil;
            break;
        case 4://plateReactionTempHighThreshold
            serviceParam->paramUser.plateReactionTempHighThreshold = paramUtil;
            break;
        case 5://plateReactionTempLowThreshold
            serviceParam->paramUser.plateReactionTempLowThreshold = paramUtil;
            break;
        case 6://plateReactionMaxPower
            serviceParam->paramUser.plateReactionMaxPower = paramUtil;
            break;
        case 7://plateReactionMinPower
            serviceParam->paramUser.plateReactionMinPower = paramUtil;
            break;
        case 8://plateReactionProtectHighTemp
            serviceParam->paramUser.plateReactionProtectHighTemp = paramUtil;
            break;
        case 9://plateReactionProtectLowTemp
            serviceParam->paramUser.plateReactionProtectLowTemp = paramUtil;
            break;
        case 10://plateReactionKI_TimeSetSecond
            serviceParam->paramUser.plateReactionKI_TimeSetSecond = (uint32_t)(paramUtil);
            break;
        case 11://plateReactionKD_TimeSetSecond
            serviceParam->paramUser.plateReactionKD_TimeSetSecond = (uint32_t)(paramUtil);
            break;
        default:
            return LH_ERR_SERVICE_PARAM_WRITE;
    }
    //到这里,说明没报错,写入参数
    ServiceUtilParamSave();
    return LH_ERR_NONE;
}

/******************************************清洗盘控温参数*******************************************************/
static LH_ERR ServiceUtilParam_ReadSubPlateWash(uint8_t subIndex,float* paramRead)
{
    float paramUtil = 0.0;
    switch(subIndex)
    {
        case 0://plateWashTargetTemp
            paramUtil = serviceParam->paramUser.plateWashTargetTemp;
            break;
        case 1://plateWashKP
            paramUtil = serviceParam->paramUser.plateWashKP;
            break;
        case 2://plateWashKI
            paramUtil = serviceParam->paramUser.plateWashKI;
            break;
        case 3://plateWashKD
            paramUtil = serviceParam->paramUser.plateWashKD;
            break;
        case 4://plateWashTempHighThreshold
            paramUtil = serviceParam->paramUser.plateWashTempHighThreshold;
            break;
        case 5://plateWashTempLowThreshold
            paramUtil = serviceParam->paramUser.plateWashTempLowThreshold;
            break;
        case 6://plateWashMaxPower
            paramUtil = serviceParam->paramUser.plateWashMaxPower;
            break;
        case 7://plateWashMinPower
            paramUtil = serviceParam->paramUser.plateWashMinPower;
            break;
        case 8://plateWashProtectHighTemp
            paramUtil = serviceParam->paramUser.plateWashProtectHighTemp;
            break;
        case 9://plateWashProtectLowTemp
            paramUtil = serviceParam->paramUser.plateWashProtectLowTemp;
            break;
        case 10://plateWashKI_TimeSetSecond
            paramUtil = serviceParam->paramUser.plateWashKI_TimeSetSecond;
            break;
        case 11://plateWashKD_TimeSetSecond
            paramUtil = serviceParam->paramUser.plateWashKD_TimeSetSecond;
            break;
        default:
            return LH_ERR_SERVICE_PARAM_READ;
    }
    *paramRead = paramUtil;
    return LH_ERR_NONE;
}

static LH_ERR ServiceUtilParam_WriteSubPlateWash(uint8_t subIndex,float paramWrite)
{
    float paramUtil = 0.0;
    paramUtil = (float)paramWrite;
    switch(subIndex)
    {
        case 0://plateWashTargetTemp
            serviceParam->paramUser.plateWashTargetTemp = paramUtil;
            break;
        case 1://plateWashKP
            serviceParam->paramUser.plateWashKP = paramUtil;
            break;
        case 2://plateWashKI
            serviceParam->paramUser.plateWashKI = paramUtil;
            break;
        case 3://plateWashKD
            serviceParam->paramUser.plateWashKD = paramUtil;
            break;
        case 4://plateWashTempHighThreshold
            serviceParam->paramUser.plateWashTempHighThreshold = paramUtil;
            break;
        case 5://plateWashTempLowThreshold
            serviceParam->paramUser.plateWashTempLowThreshold = paramUtil;
            break;
        case 6://plateWashMaxPower
            serviceParam->paramUser.plateWashMaxPower = paramUtil;
            break;
        case 7://plateWashMinPower
            serviceParam->paramUser.plateWashMinPower = paramUtil;
            break;
        case 8://plateWashProtectHighTemp
            serviceParam->paramUser.plateWashProtectHighTemp = paramUtil;
            break;
        case 9://plateWashProtectLowTemp
            serviceParam->paramUser.plateWashProtectLowTemp = paramUtil;
            break;
        case 10://plateWashKI_TimeSetSecond
            serviceParam->paramUser.plateWashKI_TimeSetSecond = (uint32_t)(paramUtil);
            break;
        case 11://plateWashKD_TimeSetSecond
            serviceParam->paramUser.plateWashKD_TimeSetSecond = (uint32_t)(paramUtil);
            break;
        default:
            return LH_ERR_SERVICE_PARAM_WRITE;
    }
    //到这里,说明没报错,写入参数
    ServiceUtilParamSave();
    return LH_ERR_NONE;
}

/*************************************************试剂盘控温参数************************************************/
static LH_ERR ServiceUtilParam_ReadSubPlateReagent(uint8_t subIndex,float* paramRead)
{
    float paramUtil = 0.0;
    switch(subIndex)
    {
        case 0://plateReagentTargetTemp
            paramUtil = serviceParam->paramUser.plateReagentTargetTemp;
            break;
        case 1://plateReagentMaxPower
            paramUtil = serviceParam->paramUser.plateReagentMaxPower;
            break;
        case 2://plateReagentMinPower
            paramUtil = serviceParam->paramUser.plateReagentMinPower;
            break;
        case 3://plateReagentProtectHighTemp
            paramUtil = serviceParam->paramUser.plateReagentProtectHighTemp;
            break;
        case 4://plateReagentProtectLowTemp
            paramUtil = serviceParam->paramUser.plateReagentProtectLowTemp;
            break;
        case 5://plateReagentTempCtrlType
            paramUtil = serviceParam->paramUser.plateReagentTempCtrlType;
            break;
        default:
            return LH_ERR_SERVICE_PARAM_READ;
    }
    *paramRead = paramUtil;
    return LH_ERR_NONE;
}

static LH_ERR ServiceUtilParam_WriteSubPlateReagent(uint8_t subIndex,float paramWrite)
{
    float paramUtil = 0.0;
    paramUtil = (float)paramWrite;
    switch(subIndex)
    {
        case 0://plateReagentTargetTemp
            serviceParam->paramUser.plateReagentTargetTemp = paramUtil;
            break;
        case 1://plateReagentMaxPower
            serviceParam->paramUser.plateReagentMaxPower = paramUtil;
            break;
        case 2://plateReagentMinPower
            serviceParam->paramUser.plateReagentMinPower = paramUtil;
            break;
        case 3://plateReagentProtectHighTemp
            serviceParam->paramUser.plateReagentProtectHighTemp = paramUtil;
            break;
        case 4://plateReagentProtectLowTemp
            serviceParam->paramUser.plateReagentProtectLowTemp = paramUtil;
            break;
        case 5://plateReagentTempCtrlType
            serviceParam->paramUser.plateReagentTempCtrlType = (uint32_t)paramUtil;
            break;
        default:
            return LH_ERR_SERVICE_PARAM_WRITE;
    }
    //到这里,说明没报错,写入参数
    ServiceUtilParamSave();
    return LH_ERR_NONE;
}


/****************************************表驱动*******************************************/
//写入参数函数数组
static const ServiceUtilParam_ReadSubFuncPtr ServiceUtilParam_ReadArray[] = {
    ServiceUtilParam_ReadSubBoardTemp,
    ServiceUtilParam_ReadSubEnvir,
    ServiceUtilParam_ReadSubPlateReaction,
    ServiceUtilParam_ReadSubPlateWash,
    ServiceUtilParam_ReadSubPlateReagent,
};
#define SIZE_PARAM_READ_SUB     (sizeof(ServiceUtilParam_ReadArray)/sizeof(ServiceUtilParam_ReadArray[0]))


//写入参数函数数组
static const ServiceUtilParam_WriteSubFuncPtr ServiceUtilParam_WriteArray[] = {
    ServiceUtilParam_WriteSubBoardTemp,
    ServiceUtilParam_WriteSubEnvir,
    ServiceUtilParam_WriteSubPlateReaction,
    ServiceUtilParam_WriteSubPlateWash,
    ServiceUtilParam_WriteSubPlateReagent,
};
#define SIZE_PARAM_WRITE_SUB     (sizeof(ServiceUtilParam_WriteArray)/sizeof(ServiceUtilParam_WriteArray[0]))


/****************************************************对外开放*************************************************/
//读取参数
LH_ERR ServiceUtilParam_Read(SERVICE_PARAM_PARAM_MAIN mainIndex,uint8_t subIndex,float* paramRead)
{
    //主序号超出
    if(mainIndex >= SIZE_PARAM_READ_SUB)
    {
        *paramRead = 0;
        return LH_ERR_SERVICE_PARAM_READ;
    }
    //调用实际实现
    return ServiceUtilParam_ReadArray[mainIndex](subIndex,paramRead);
}

//写入参数
LH_ERR ServiceUtilParam_Write(SERVICE_PARAM_PARAM_MAIN mainIndex,uint8_t subIndex,float paramWrite)
{
    if(mainIndex >= SIZE_PARAM_WRITE_SUB)
    {
        return LH_ERR_SERVICE_PARAM_WRITE;
    }
    //调用实际实现
    return ServiceUtilParam_WriteArray[mainIndex](subIndex,paramWrite);
}







