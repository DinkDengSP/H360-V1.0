/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-20 18:17:11
**FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppUtil\AppParam.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppParam.h"

//APP写入单个参数的指针
typedef LH_ERR (*AppParamWriteParamSingleFuncPtr)(uint16_t subIndex,int32_t writeParam);
//APP读取单个参数的指针
typedef LH_ERR (*AppParamReadParamSingleFuncPtr)(uint16_t subIndex,int32_t* readParam);

static APP_PARAM* appParamPtr = NULL;

//系统应用参数初始化
void AppParamInit(void)
{
    if(appParamPtr != NULL)
    {
        UserMemFree(SRAM_IS62,appParamPtr);
    }
    //申请内存
    appParamPtr = UserMemMallocWhileSuccess(SRAM_IS62,LENGTH_APP_PARAM);
    //从Flash中加载参数
    BoardMB85RS2MT_ReadBuffer(ADDR_START_APP_PARAM,(uint8_t*)appParamPtr,LENGTH_APP_PARAM);
    //数据校验
    if(appParamPtr->paramFlag != APP_PARAM_DEFAULT_FLAG)
    {
        AppParamResetDefault();
    }
}

//系统应用参数复位到默认状态
void AppParamResetDefault(void)
{
    //设定为默认配置
    appParamPtr->paramFlag = APP_PARAM_DEFAULT_FLAG;
    //参数装载
    UserMemCopy((void*)(&(appParamPtr->paramTubeEntry)),(void*)(&paramTubeEntryDefault),LENGTH_APP_PARAM_TUBE_ENTRY);
    UserMemCopy((void*)(&(appParamPtr->paramTubeRotate)),(void*)(&paramTubeRotateDefault),LENGTH_APP_PARAM_TUBE_ROTATE);
    UserMemCopy((void*)(&(appParamPtr->paramTubeBarScan)),(void*)(&paramTubeBarScanDefault),LENGTH_APP_PARAM_TUBE_BAR_SCAN);
    UserMemCopy((void*)(&(appParamPtr->paramTubeClamp)),(void*)(&paramTubeClampDefault),LENGTH_APP_PARAM_TUBE_CLAMP);
    UserMemCopy((void*)(&(appParamPtr->paramTubeExit)),(void*)(&paramTubeExitDefault),LENGTH_APP_PARAM_TUBE_EXIT);
    //将设定好的值写回外置Flash
    BoardMB85RS2MT_WriteBuffer(ADDR_START_APP_PARAM,(uint8_t*)appParamPtr,LENGTH_APP_PARAM);
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
    BoardMB85RS2MT_WriteBuffer(ADDR_START_APP_PARAM+offset,(uint8_t*)targetParamPtr,4);
}

/****************************************入口节点参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteInput(uint16_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case APP_PARAM_SUB_TUBE_ENTRY_INDEX_BELT_TYPE:
            targetParamPtr = &(appParamPtr->paramTubeEntry.railConveyorBeltMotorType);
            break;
        default:
            return LH_ERR_APP_PARAM_WRITE;//不支持的参数
    }
    //参数写入Flash
    AppParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return LH_ERR_NONE;
}
//读取参数
LH_ERR AppParamReadInput(uint16_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case APP_PARAM_SUB_TUBE_ENTRY_INDEX_BELT_TYPE:
            *readParam = appParamPtr->paramTubeEntry.railConveyorBeltMotorType;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}

/****************************************转盘节点参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteRotate(uint16_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case APP_PARAM_SUB_TUBE_ROTATE_INDEX_STEP_MOTOR_RESET_CORRECT_STEP:
            targetParamPtr = &(appParamPtr->paramTubeRotate.rotateStepMotorCorrectStep);
            break;
        default:
            return LH_ERR_APP_PARAM_WRITE;//不支持的参数
    }
    //参数写入Flash
    AppParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return LH_ERR_NONE;
}
//读取参数
LH_ERR AppParamReadRotate(uint16_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case APP_PARAM_SUB_TUBE_ROTATE_INDEX_STEP_MOTOR_RESET_CORRECT_STEP:
            *readParam = appParamPtr->paramTubeRotate.rotateStepMotorCorrectStep;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}

/****************************************扫码节点参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteBarScan(uint16_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case APP_PARAM_SUB_TUBE_BAR_SCAN_INDEX_RESERVE:
            targetParamPtr = &(appParamPtr->paramTubeBarScan.reserve);
            break;
        default:
            return LH_ERR_APP_PARAM_WRITE;//不支持的参数
    }
    //参数写入Flash
    AppParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return LH_ERR_NONE;
}
//读取参数
LH_ERR AppParamReadBarScan(uint16_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case APP_PARAM_SUB_TUBE_BAR_SCAN_INDEX_RESERVE:
            *readParam = appParamPtr->paramTubeBarScan.reserve;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}

/****************************************测试试管夹紧参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteTubeClamp(uint16_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case APP_PARAM_SUB_TUBE_CLAMP_INDEX_RESERVE:
            targetParamPtr = &(appParamPtr->paramTubeClamp.reserve);
            break;
        case APP_PARAM_SUB_TUBE_CLAMP_INDEX_UPDOWN_STEP_MOTOR_RESET_CORRECT_STEP:
            targetParamPtr = &(appParamPtr->paramTubeClamp.updownStepMotorCorrectStep);
            break;
        case APP_PARAM_SUB_TUBE_CLAMP_INDEX_UPDOWN_STEP_MOTOR_DOWN_POS:
            targetParamPtr = &(appParamPtr->paramTubeClamp.updownStepMotorClampTubeDownPos);
            break;
        default:
            return LH_ERR_APP_PARAM_WRITE;//不支持的参数
    }
    //参数写入Flash
    AppParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return LH_ERR_NONE;
}
//读取参数
LH_ERR AppParamReadTubeClamp(uint16_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case APP_PARAM_SUB_TUBE_CLAMP_INDEX_RESERVE:
            *readParam = appParamPtr->paramTubeClamp.reserve;
            break;
        case APP_PARAM_SUB_TUBE_CLAMP_INDEX_UPDOWN_STEP_MOTOR_RESET_CORRECT_STEP:
            *readParam = appParamPtr->paramTubeClamp.updownStepMotorCorrectStep;
            break;
        case APP_PARAM_SUB_TUBE_CLAMP_INDEX_UPDOWN_STEP_MOTOR_DOWN_POS:
            *readParam = appParamPtr->paramTubeClamp.updownStepMotorClampTubeDownPos;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}

/****************************************输出节点参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteOutput(uint16_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case APP_PARAM_SUB_TUBE_EXIT_INDEX_RESERVE:
            targetParamPtr = &(appParamPtr->paramTubeExit.reserve);
            break;
        default:
            return LH_ERR_APP_PARAM_WRITE;//不支持的参数
    }
    //参数写入Flash
    AppParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return LH_ERR_NONE;
}
//读取参数
LH_ERR AppParamReadOutput(uint16_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case APP_PARAM_SUB_TUBE_EXIT_INDEX_RESERVE:
            *readParam = appParamPtr->paramTubeExit.reserve;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}


/****************************************参数读取函数指针数组**************************************************/
static const AppParamReadParamSingleFuncPtr AppParamReadParamSingleArray[] = {
    AppParamReadInput,
    AppParamReadRotate,
    AppParamReadBarScan,
    AppParamReadTubeClamp,
    AppParamReadOutput,
};
#define COUNT_APP_UTIL_FUNC_READ_PARAM          (sizeof(AppParamReadParamSingleArray)/sizeof(AppParamReadParamSingleArray[0]))

/****************************************参数写入函数指针数组**************************************************/
static const AppParamWriteParamSingleFuncPtr AppParamWriteParamSingleArray[] = {
    AppParamWriteInput,
    AppParamWriteRotate,
    AppParamWriteBarScan,
    AppParamWriteTubeClamp,
    AppParamWriteOutput,
};
#define COUNT_APP_UTIL_FUNC_WRITE_PARAM         (sizeof(AppParamWriteParamSingleArray)/sizeof(AppParamWriteParamSingleArray[0]))

//写入参数
LH_ERR AppParamWriteParam(APP_PARAM_MAIN_INDEX mainIndex,uint16_t subIndex,int32_t writeParam)
{
    if(mainIndex >= COUNT_APP_UTIL_FUNC_WRITE_PARAM)
    {
        return LH_ERR_APP_PARAM_WRITE;
    }
    return AppParamWriteParamSingleArray[mainIndex](subIndex,writeParam);
}

//读取参数
LH_ERR AppParamReadParam(APP_PARAM_MAIN_INDEX mainIndex,uint16_t subIndex,int32_t* readParam)
{
    if(mainIndex >= COUNT_APP_UTIL_FUNC_READ_PARAM)
    {
        return LH_ERR_APP_PARAM_READ;
    }
    return AppParamReadParamSingleArray[mainIndex](subIndex,readParam);
}



