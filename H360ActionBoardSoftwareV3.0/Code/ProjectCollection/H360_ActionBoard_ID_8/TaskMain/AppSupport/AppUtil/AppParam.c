/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-01 14:41:26
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

static APP_PARAM* appParamLocalPtr = NULL;

//系统应用参数初始化
LH_ERR AppParamInit(void)
{
    //首先申请内存
    appParamLocalPtr = NULL;
    //申请内存
    do
    {
        appParamLocalPtr = UserMemMalloc(SRAM_IS62,EXT_FLASH_APP_PARAM_SIZE);
        if(appParamLocalPtr == NULL)
        {
            CoreDelayMinTick();
        }
    } while (appParamLocalPtr == NULL);

    //从Flash中加载参数
    BoardMB85RS2MT_ReadBuffer(ADDR_START_APP_PARAM,(uint8_t*)appParamLocalPtr,EXT_FLASH_APP_PARAM_SIZE);
    //数据校验
    if(appParamLocalPtr->paramFlag == APP_PARAM_DEFAULT_FLAG)
    {
        //校验通过
        return LH_ERR_NONE;
    }
    else
    {
        //设定为默认配置
        appParamLocalPtr->paramFlag = APP_PARAM_DEFAULT_FLAG;
        //参数装载
        //进样1
        UserMemCopy((void*)(&(appParamLocalPtr->pushIn1Param)),(void*)(&pushInDefaultSetting),sizeof(APP_PARAM_PUSHIN)/sizeof(uint8_t));
        //进样2
        UserMemCopy((void*)(&(appParamLocalPtr->pushIn2Param)),(void*)(&pushInDefaultSetting),sizeof(APP_PARAM_PUSHIN)/sizeof(uint8_t));
        //出样1
        UserMemCopy((void*)(&(appParamLocalPtr->pushOut1Param)),(void*)(&pushOutDefaultSetting),sizeof(APP_PARAM_PUSHOUT)/sizeof(uint8_t));
        //出样2
        UserMemCopy((void*)(&(appParamLocalPtr->pushOut2Param)),(void*)(&pushOutDefaultSetting),sizeof(APP_PARAM_PUSHOUT)/sizeof(uint8_t));
        //轨道独立参数
        UserMemCopy((void*)(&(appParamLocalPtr->railIndependentParam)),(void*)(&RailIndependentDefaultSetting),sizeof(APP_PARAM_RAIL_INDEPENDENT)/sizeof(uint8_t));
        //轨道模块1
        UserMemCopy((void*)(&(appParamLocalPtr->railParamModule1)),(void*)(&railModuleDefaultSetting),sizeof(APP_PARAM_RAIL_MODULE)/sizeof(uint8_t));
        //轨道模块2
        UserMemCopy((void*)(&(appParamLocalPtr->railParamModule2)),(void*)(&railModuleDefaultSetting),sizeof(APP_PARAM_RAIL_MODULE)/sizeof(uint8_t));
        //轨道模块3
        UserMemCopy((void*)(&(appParamLocalPtr->railParamModule3)),(void*)(&railModuleDefaultSetting),sizeof(APP_PARAM_RAIL_MODULE)/sizeof(uint8_t));
        //轨道模块4
        UserMemCopy((void*)(&(appParamLocalPtr->railParamModule4)),(void*)(&railModuleDefaultSetting),sizeof(APP_PARAM_RAIL_MODULE)/sizeof(uint8_t));
        //机械手独立参数
        UserMemCopy((void*)(&(appParamLocalPtr->handParamIndependent)),(void*)(&handIndependentDefaultSetting),sizeof(APP_PARAM_HAND_INDEPENDENT)/sizeof(uint8_t));
        //机械手模块1
        UserMemCopy((void*)(&(appParamLocalPtr->handParamModule1)),(void*)(&handModuleDefaultSetting),sizeof(APP_PARAM_HAND_MODULE)/sizeof(uint8_t));
        //机械手模块2
        UserMemCopy((void*)(&(appParamLocalPtr->handParamModule2)),(void*)(&handModuleDefaultSetting),sizeof(APP_PARAM_HAND_MODULE)/sizeof(uint8_t));
        //机械手模块3
        UserMemCopy((void*)(&(appParamLocalPtr->handParamModule3)),(void*)(&handModuleDefaultSetting),sizeof(APP_PARAM_HAND_MODULE)/sizeof(uint8_t));
        //机械手模块4
        UserMemCopy((void*)(&(appParamLocalPtr->handParamModule4)),(void*)(&handModuleDefaultSetting),sizeof(APP_PARAM_HAND_MODULE)/sizeof(uint8_t));
        //轨道应用辅助参数
        UserMemCopy((void*)(&(appParamLocalPtr->utilParam)),(void*)(&appUtilDefaultSetting),sizeof(APP_PARAM_UTIL)/sizeof(uint8_t));
        //将设定好的值写回外置Flash
        BoardMB85RS2MT_WriteBuffer(ADDR_START_APP_PARAM,(uint8_t*)appParamLocalPtr,EXT_FLASH_APP_PARAM_SIZE);
        return LH_ERR_NONE;
    }
}

//系统应用参数复位到默认状态
LH_ERR AppParamResetDedault(void)
{
    //设定为默认配置
        appParamLocalPtr->paramFlag = APP_PARAM_DEFAULT_FLAG;
        //参数装载
        //进样1
        UserMemCopy((void*)(&(appParamLocalPtr->pushIn1Param)),(void*)(&pushInDefaultSetting),sizeof(APP_PARAM_PUSHIN)/sizeof(uint8_t));
        //进样2
        UserMemCopy((void*)(&(appParamLocalPtr->pushIn2Param)),(void*)(&pushInDefaultSetting),sizeof(APP_PARAM_PUSHIN)/sizeof(uint8_t));
        //出样1
        UserMemCopy((void*)(&(appParamLocalPtr->pushOut1Param)),(void*)(&pushOutDefaultSetting),sizeof(APP_PARAM_PUSHOUT)/sizeof(uint8_t));
        //出样2
        UserMemCopy((void*)(&(appParamLocalPtr->pushOut2Param)),(void*)(&pushOutDefaultSetting),sizeof(APP_PARAM_PUSHOUT)/sizeof(uint8_t));
        //轨道独立参数
        UserMemCopy((void*)(&(appParamLocalPtr->railIndependentParam)),(void*)(&RailIndependentDefaultSetting),sizeof(APP_PARAM_RAIL_INDEPENDENT)/sizeof(uint8_t));
        //轨道模块1
        UserMemCopy((void*)(&(appParamLocalPtr->railParamModule1)),(void*)(&railModuleDefaultSetting),sizeof(APP_PARAM_RAIL_MODULE)/sizeof(uint8_t));
        //轨道模块2
        UserMemCopy((void*)(&(appParamLocalPtr->railParamModule2)),(void*)(&railModuleDefaultSetting),sizeof(APP_PARAM_RAIL_MODULE)/sizeof(uint8_t));
        //轨道模块3
        UserMemCopy((void*)(&(appParamLocalPtr->railParamModule3)),(void*)(&railModuleDefaultSetting),sizeof(APP_PARAM_RAIL_MODULE)/sizeof(uint8_t));
        //轨道模块4
        UserMemCopy((void*)(&(appParamLocalPtr->railParamModule4)),(void*)(&railModuleDefaultSetting),sizeof(APP_PARAM_RAIL_MODULE)/sizeof(uint8_t));
        //机械手独立参数
        UserMemCopy((void*)(&(appParamLocalPtr->handParamIndependent)),(void*)(&handIndependentDefaultSetting),sizeof(APP_PARAM_HAND_INDEPENDENT)/sizeof(uint8_t));
        //机械手模块1
        UserMemCopy((void*)(&(appParamLocalPtr->handParamModule1)),(void*)(&handModuleDefaultSetting),sizeof(APP_PARAM_HAND_MODULE)/sizeof(uint8_t));
        //机械手模块2
        UserMemCopy((void*)(&(appParamLocalPtr->handParamModule2)),(void*)(&handModuleDefaultSetting),sizeof(APP_PARAM_HAND_MODULE)/sizeof(uint8_t));
        //机械手模块3
        UserMemCopy((void*)(&(appParamLocalPtr->handParamModule3)),(void*)(&handModuleDefaultSetting),sizeof(APP_PARAM_HAND_MODULE)/sizeof(uint8_t));
        //机械手模块4
        UserMemCopy((void*)(&(appParamLocalPtr->handParamModule4)),(void*)(&handModuleDefaultSetting),sizeof(APP_PARAM_HAND_MODULE)/sizeof(uint8_t));
        //轨道应用辅助参数
        UserMemCopy((void*)(&(appParamLocalPtr->utilParam)),(void*)(&appUtilDefaultSetting),sizeof(APP_PARAM_UTIL)/sizeof(uint8_t));
        //将设定好的值写回外置Flash
        BoardMB85RS2MT_WriteBuffer(ADDR_START_APP_PARAM,(uint8_t*)appParamLocalPtr,EXT_FLASH_APP_PARAM_SIZE);
        return LH_ERR_NONE;
}



//通用的写入Flash的函数
static void AppParamWriteSingleCommon(int32_t* targetParamPtr,int32_t writeParam)
{
    *targetParamPtr = writeParam;
    int32_t targetAddr = (int32_t)((char*)targetParamPtr);
    int32_t baseAddr = (int32_t)((char*)(appParamLocalPtr));
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

/****************************************进样1参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamPushIn1(uint16_t subIndex,int32_t writeParam)
{
    APP_PARAM_INDEX_SUB_PUSH_IN paramIndex = (APP_PARAM_INDEX_SUB_PUSH_IN)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_PUSH_IN_RESET_CORRECTION:
            targetParamPtr = &(appParamLocalPtr->pushIn1Param.resetCorrection);
            break;
        case APP_PARAM_INDEX_SUB_PUSH_IN_MAX_POS:
            targetParamPtr = &(appParamLocalPtr->pushIn1Param.pushMaxPosition);
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
LH_ERR AppParamReadParamPushIn1(uint16_t subIndex,int32_t* readParam)
{
    APP_PARAM_INDEX_SUB_PUSH_IN paramIndex = (APP_PARAM_INDEX_SUB_PUSH_IN)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_PUSH_IN_RESET_CORRECTION:
            *readParam = appParamLocalPtr->pushIn1Param.resetCorrection;
            break;
        case APP_PARAM_INDEX_SUB_PUSH_IN_MAX_POS:
            *readParam = appParamLocalPtr->pushIn1Param.pushMaxPosition;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}
/****************************************进样2参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamPushIn2(uint16_t subIndex,int32_t writeParam)
{
    APP_PARAM_INDEX_SUB_PUSH_IN paramIndex = (APP_PARAM_INDEX_SUB_PUSH_IN)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_PUSH_IN_RESET_CORRECTION:
            targetParamPtr = &(appParamLocalPtr->pushIn2Param.resetCorrection);
            break;
        case APP_PARAM_INDEX_SUB_PUSH_IN_MAX_POS:
            targetParamPtr = &(appParamLocalPtr->pushIn2Param.pushMaxPosition);
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
LH_ERR AppParamReadParamPushIn2(uint16_t subIndex,int32_t* readParam)
{
    APP_PARAM_INDEX_SUB_PUSH_IN paramIndex = (APP_PARAM_INDEX_SUB_PUSH_IN)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_PUSH_IN_RESET_CORRECTION:
            *readParam = appParamLocalPtr->pushIn2Param.resetCorrection;
            break;
        case APP_PARAM_INDEX_SUB_PUSH_IN_MAX_POS:
            *readParam = appParamLocalPtr->pushIn2Param.pushMaxPosition;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}
/****************************************出样1参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamPushOut1(uint16_t subIndex,int32_t writeParam)
{
    APP_PARAM_INDEX_SUB_PUSH_OUT paramIndex = (APP_PARAM_INDEX_SUB_PUSH_OUT)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_PUSH_OUT_RESET_CORRECTION:
            targetParamPtr = &(appParamLocalPtr->pushOut1Param.resetCorrection);
            break;
        // case APP_PARAM_INDEX_SUB_PUSH_OUT_ONE_RACK_OFFSET:
        //     targetParamPtr = &(appParamLocalPtr->pushOut1Param.pushOneRackOffset);
        //     break;
        default:
            return LH_ERR_APP_PARAM_WRITE;//不支持的参数
    }
    //参数写入Flash
    AppParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return LH_ERR_NONE;
}
//读取参数
LH_ERR AppParamReadParamPushOut1(uint16_t subIndex,int32_t* readParam)
{
    APP_PARAM_INDEX_SUB_PUSH_OUT paramIndex = (APP_PARAM_INDEX_SUB_PUSH_OUT)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_PUSH_OUT_RESET_CORRECTION:
            *readParam = appParamLocalPtr->pushOut1Param.resetCorrection;
            break;
        // case APP_PARAM_INDEX_SUB_PUSH_OUT_ONE_RACK_OFFSET:
        //     *readParam = appParamLocalPtr->pushOut1Param.pushOneRackOffset;
        //     break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}
/****************************************出样2参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamPushOut2(uint16_t subIndex,int32_t writeParam)
{
    APP_PARAM_INDEX_SUB_PUSH_OUT paramIndex = (APP_PARAM_INDEX_SUB_PUSH_OUT)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_PUSH_OUT_RESET_CORRECTION:
            targetParamPtr = &(appParamLocalPtr->pushOut2Param.resetCorrection);
            break;
        // case APP_PARAM_INDEX_SUB_PUSH_OUT_ONE_RACK_OFFSET:
        //     targetParamPtr = &(appParamLocalPtr->pushOut2Param.pushOneRackOffset);
        //     break;
        default:
            return LH_ERR_APP_PARAM_WRITE;//不支持的参数
    }
    //参数写入Flash
    AppParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return LH_ERR_NONE;
}
//读取参数
LH_ERR AppParamReadParamPushOut2(uint16_t subIndex,int32_t* readParam)
{
    APP_PARAM_INDEX_SUB_PUSH_OUT paramIndex = (APP_PARAM_INDEX_SUB_PUSH_OUT)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_PUSH_OUT_RESET_CORRECTION:
            *readParam = appParamLocalPtr->pushOut2Param.resetCorrection;
            break;
        // case APP_PARAM_INDEX_SUB_PUSH_OUT_ONE_RACK_OFFSET:
        //     *readParam = appParamLocalPtr->pushOut2Param.pushOneRackOffset;
        //     break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}
/****************************************轨道独立参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamRailIndependent(uint16_t subIndex,int32_t writeParam)
{
    APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT paramIndex = (APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_RESET_CORRECTION:
            targetParamPtr = &(appParamLocalPtr->railIndependentParam.resetCorrection);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_EMERGENCY_POS:
            targetParamPtr = &(appParamLocalPtr->railIndependentParam.emergencyPosition);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_PUSH_IN1_POS:
            targetParamPtr = &(appParamLocalPtr->railIndependentParam.pushIn1Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_PUSH_IN2_POS:
            targetParamPtr = &(appParamLocalPtr->railIndependentParam.pushIn2Position);
            break;
        // case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_RACK_SCAN_POS:
        //     targetParamPtr = &(appParamLocalPtr->railIndependentParam.rackBarScanPosition);
        //     break;
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE1_SCAN_POS:
            targetParamPtr = &(appParamLocalPtr->railIndependentParam.tube1BarScanPosition);
            break;
        // case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE2_SCAN_POS:
        //     targetParamPtr = &(appParamLocalPtr->railIndependentParam.tube2BarScanPosition);
        //     break;
        // case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE3_SCAN_POS:
        //     targetParamPtr = &(appParamLocalPtr->railIndependentParam.tube3BarScanPosition);
        //     break;
        // case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE4_SCAN_POS:
        //     targetParamPtr = &(appParamLocalPtr->railIndependentParam.tube4BarScanPosition);
        //     break;
        // case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE5_SCAN_POS:
        //     targetParamPtr = &(appParamLocalPtr->railIndependentParam.tube5BarScanPosition);
        //     break;
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_ROTATE1_POS:
            targetParamPtr = &(appParamLocalPtr->railIndependentParam.rotate1Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_ROTATE2_POS:
            targetParamPtr = &(appParamLocalPtr->railIndependentParam.rotate2Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_PUSH_OUT1_POS:
            targetParamPtr = &(appParamLocalPtr->railIndependentParam.pushOut1Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_PUSH_OUT2_POS:
            targetParamPtr = &(appParamLocalPtr->railIndependentParam.pushOut2Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_BARCODE_SCANNER_TYPE:
            targetParamPtr = &(appParamLocalPtr->railIndependentParam.barcodeScannerType);
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
LH_ERR AppParamReadParamRailIndependent(uint16_t subIndex,int32_t* readParam)
{
    APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT paramIndex = (APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_RESET_CORRECTION:
            *readParam = appParamLocalPtr->railIndependentParam.resetCorrection;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_EMERGENCY_POS:
            *readParam = appParamLocalPtr->railIndependentParam.emergencyPosition;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_PUSH_IN1_POS:
            *readParam = appParamLocalPtr->railIndependentParam.pushIn1Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_PUSH_IN2_POS:
            *readParam = appParamLocalPtr->railIndependentParam.pushIn2Position;
            break;
        // case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_RACK_SCAN_POS:
        //     *readParam = appParamLocalPtr->railIndependentParam.rackBarScanPosition;
        //     break;
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE1_SCAN_POS:
            *readParam = appParamLocalPtr->railIndependentParam.tube1BarScanPosition;
            break;
        // case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE2_SCAN_POS:
        //     *readParam = appParamLocalPtr->railIndependentParam.tube2BarScanPosition;
        //     break;
        // case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE3_SCAN_POS:
        //     *readParam = appParamLocalPtr->railIndependentParam.tube3BarScanPosition;
        //     break;
        // case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE4_SCAN_POS:
        //     *readParam = appParamLocalPtr->railIndependentParam.tube4BarScanPosition;
        //     break;
        // case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE5_SCAN_POS:
        //     *readParam = appParamLocalPtr->railIndependentParam.tube5BarScanPosition;
        //     break;
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_ROTATE1_POS:
            *readParam = appParamLocalPtr->railIndependentParam.rotate1Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_ROTATE2_POS:
            *readParam = appParamLocalPtr->railIndependentParam.rotate2Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_PUSH_OUT1_POS:
            *readParam = appParamLocalPtr->railIndependentParam.pushOut1Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_PUSH_OUT2_POS:
            *readParam = appParamLocalPtr->railIndependentParam.pushOut2Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_BARCODE_SCANNER_TYPE:
            *readParam = appParamLocalPtr->railIndependentParam.barcodeScannerType;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}
/****************************************轨道模块1参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamRailModule1(uint16_t subIndex,int32_t writeParam)
{
    APP_PARAM_INDEX_SUB_RAIL_MODULE paramIndex = (APP_PARAM_INDEX_SUB_RAIL_MODULE)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST1_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule1.WaitTest1Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST2_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule1.WaitTest2Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST3_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule1.WaitTest3Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST1_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule1.ReTest1Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST2_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule1.ReTest2Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST3_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule1.ReTest3Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST4_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule1.ReTest4Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST5_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule1.ReTest5Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST6_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule1.ReTest6Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST7_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule1.ReTest7Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST8_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule1.ReTest8Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_PUT:
            targetParamPtr = &(appParamLocalPtr->railParamModule1.TestingPutPosition);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_CATCH:
            targetParamPtr = &(appParamLocalPtr->railParamModule1.TestingCatchPosition);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_ABSORB:
            targetParamPtr = &(appParamLocalPtr->railParamModule1.TestingAbsorbPosition);
            break;
        // case APP_PARAM_INDEX_SUB_RAIL_MODULE_TUBE_OFFSET_TESTING:
        //     targetParamPtr = &(appParamLocalPtr->railParamModule1.tubeOffsetTesting);
        //     break;
        default:
            return LH_ERR_APP_PARAM_WRITE;//不支持的参数
    }
    //参数写入Flash
    AppParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return LH_ERR_NONE;
}
//读取参数
LH_ERR AppParamReadParamRailModule1(uint16_t subIndex,int32_t* readParam)
{
    APP_PARAM_INDEX_SUB_RAIL_MODULE paramIndex = (APP_PARAM_INDEX_SUB_RAIL_MODULE)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST1_POS:
            *readParam = appParamLocalPtr->railParamModule1.WaitTest1Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST2_POS:
            *readParam = appParamLocalPtr->railParamModule1.WaitTest2Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST3_POS:
            *readParam = appParamLocalPtr->railParamModule1.WaitTest3Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST1_POS:
            *readParam = appParamLocalPtr->railParamModule1.ReTest1Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST2_POS:
            *readParam = appParamLocalPtr->railParamModule1.ReTest2Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST3_POS:
            *readParam = appParamLocalPtr->railParamModule1.ReTest3Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST4_POS:
            *readParam = appParamLocalPtr->railParamModule1.ReTest4Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST5_POS:
            *readParam = appParamLocalPtr->railParamModule1.ReTest5Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST6_POS:
            *readParam = appParamLocalPtr->railParamModule1.ReTest6Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST7_POS:
            *readParam = appParamLocalPtr->railParamModule1.ReTest7Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST8_POS:
            *readParam = appParamLocalPtr->railParamModule1.ReTest8Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_PUT:
            *readParam = appParamLocalPtr->railParamModule1.TestingPutPosition;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_CATCH:
            *readParam = appParamLocalPtr->railParamModule1.TestingCatchPosition;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_ABSORB:
            *readParam = appParamLocalPtr->railParamModule1.TestingAbsorbPosition;
            break;
        // case APP_PARAM_INDEX_SUB_RAIL_MODULE_TUBE_OFFSET_TESTING:
        //     *readParam = appParamLocalPtr->railParamModule1.tubeOffsetTesting;
        //     break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}
/****************************************轨道模块2参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamRailModule2(uint16_t subIndex,int32_t writeParam)
{
    APP_PARAM_INDEX_SUB_RAIL_MODULE paramIndex = (APP_PARAM_INDEX_SUB_RAIL_MODULE)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST1_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule2.WaitTest1Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST2_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule2.WaitTest2Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST3_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule2.WaitTest3Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST1_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule2.ReTest1Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST2_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule2.ReTest2Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST3_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule2.ReTest3Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST4_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule2.ReTest4Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST5_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule2.ReTest5Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST6_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule2.ReTest6Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST7_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule2.ReTest7Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST8_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule2.ReTest8Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_PUT:
            targetParamPtr = &(appParamLocalPtr->railParamModule2.TestingPutPosition);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_CATCH:
            targetParamPtr = &(appParamLocalPtr->railParamModule2.TestingCatchPosition);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_ABSORB:
            targetParamPtr = &(appParamLocalPtr->railParamModule2.TestingAbsorbPosition);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TUBE_OFFSET_TESTING:
            targetParamPtr = &(appParamLocalPtr->railParamModule2.tubeOffsetTesting);
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
LH_ERR AppParamReadParamRailModule2(uint16_t subIndex,int32_t* readParam)
{
    APP_PARAM_INDEX_SUB_RAIL_MODULE paramIndex = (APP_PARAM_INDEX_SUB_RAIL_MODULE)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST1_POS:
            *readParam = appParamLocalPtr->railParamModule2.WaitTest1Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST2_POS:
            *readParam = appParamLocalPtr->railParamModule2.WaitTest2Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST3_POS:
            *readParam = appParamLocalPtr->railParamModule2.WaitTest3Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST1_POS:
            *readParam = appParamLocalPtr->railParamModule2.ReTest1Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST2_POS:
            *readParam = appParamLocalPtr->railParamModule2.ReTest2Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST3_POS:
            *readParam = appParamLocalPtr->railParamModule2.ReTest3Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST4_POS:
            *readParam = appParamLocalPtr->railParamModule2.ReTest4Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST5_POS:
            *readParam = appParamLocalPtr->railParamModule2.ReTest5Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST6_POS:
            *readParam = appParamLocalPtr->railParamModule2.ReTest6Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST7_POS:
            *readParam = appParamLocalPtr->railParamModule2.ReTest7Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST8_POS:
            *readParam = appParamLocalPtr->railParamModule2.ReTest8Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_PUT:
            *readParam = appParamLocalPtr->railParamModule2.TestingPutPosition;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_CATCH:
            *readParam = appParamLocalPtr->railParamModule2.TestingCatchPosition;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_ABSORB:
            *readParam = appParamLocalPtr->railParamModule2.TestingAbsorbPosition;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TUBE_OFFSET_TESTING:
            *readParam = appParamLocalPtr->railParamModule2.tubeOffsetTesting;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}
/****************************************轨道模块3参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamRailModule3(uint16_t subIndex,int32_t writeParam)
{
    APP_PARAM_INDEX_SUB_RAIL_MODULE paramIndex = (APP_PARAM_INDEX_SUB_RAIL_MODULE)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST1_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule3.WaitTest1Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST2_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule3.WaitTest2Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST3_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule3.WaitTest3Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST1_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule3.ReTest1Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST2_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule3.ReTest2Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST3_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule3.ReTest3Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST4_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule3.ReTest4Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST5_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule3.ReTest5Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST6_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule3.ReTest6Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST7_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule3.ReTest7Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST8_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule3.ReTest8Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_PUT:
            targetParamPtr = &(appParamLocalPtr->railParamModule3.TestingPutPosition);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_CATCH:
            targetParamPtr = &(appParamLocalPtr->railParamModule3.TestingCatchPosition);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_ABSORB:
            targetParamPtr = &(appParamLocalPtr->railParamModule3.TestingAbsorbPosition);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TUBE_OFFSET_TESTING:
            targetParamPtr = &(appParamLocalPtr->railParamModule3.tubeOffsetTesting);
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
LH_ERR AppParamReadParamRailModule3(uint16_t subIndex,int32_t* readParam)
{
    APP_PARAM_INDEX_SUB_RAIL_MODULE paramIndex = (APP_PARAM_INDEX_SUB_RAIL_MODULE)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST1_POS:
            *readParam = appParamLocalPtr->railParamModule3.WaitTest1Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST2_POS:
            *readParam = appParamLocalPtr->railParamModule3.WaitTest2Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST3_POS:
            *readParam = appParamLocalPtr->railParamModule3.WaitTest3Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST1_POS:
            *readParam = appParamLocalPtr->railParamModule3.ReTest1Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST2_POS:
            *readParam = appParamLocalPtr->railParamModule3.ReTest2Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST3_POS:
            *readParam = appParamLocalPtr->railParamModule3.ReTest3Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST4_POS:
            *readParam = appParamLocalPtr->railParamModule3.ReTest4Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST5_POS:
            *readParam = appParamLocalPtr->railParamModule3.ReTest5Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST6_POS:
            *readParam = appParamLocalPtr->railParamModule3.ReTest6Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST7_POS:
            *readParam = appParamLocalPtr->railParamModule3.ReTest7Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST8_POS:
            *readParam = appParamLocalPtr->railParamModule3.ReTest8Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_PUT:
            *readParam = appParamLocalPtr->railParamModule3.TestingPutPosition;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_CATCH:
            *readParam = appParamLocalPtr->railParamModule3.TestingCatchPosition;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_ABSORB:
            *readParam = appParamLocalPtr->railParamModule3.TestingAbsorbPosition;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TUBE_OFFSET_TESTING:
            *readParam = appParamLocalPtr->railParamModule3.tubeOffsetTesting;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}
/****************************************轨道模块4参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamRailModule4(uint16_t subIndex,int32_t writeParam)
{
    APP_PARAM_INDEX_SUB_RAIL_MODULE paramIndex = (APP_PARAM_INDEX_SUB_RAIL_MODULE)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST1_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule4.WaitTest1Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST2_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule4.WaitTest2Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST3_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule4.WaitTest3Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST1_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule4.ReTest1Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST2_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule4.ReTest2Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST3_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule4.ReTest3Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST4_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule4.ReTest4Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST5_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule4.ReTest5Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST6_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule4.ReTest6Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST7_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule4.ReTest7Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST8_POS:
            targetParamPtr = &(appParamLocalPtr->railParamModule4.ReTest8Position);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_PUT:
            targetParamPtr = &(appParamLocalPtr->railParamModule4.TestingPutPosition);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_CATCH:
            targetParamPtr = &(appParamLocalPtr->railParamModule4.TestingCatchPosition);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_ABSORB:
            targetParamPtr = &(appParamLocalPtr->railParamModule4.TestingAbsorbPosition);
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TUBE_OFFSET_TESTING:
            targetParamPtr = &(appParamLocalPtr->railParamModule4.tubeOffsetTesting);
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
LH_ERR AppParamReadParamRailModule4(uint16_t subIndex,int32_t* readParam)
{
    APP_PARAM_INDEX_SUB_RAIL_MODULE paramIndex = (APP_PARAM_INDEX_SUB_RAIL_MODULE)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST1_POS:
            *readParam = appParamLocalPtr->railParamModule4.WaitTest1Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST2_POS:
            *readParam = appParamLocalPtr->railParamModule4.WaitTest2Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST3_POS:
            *readParam = appParamLocalPtr->railParamModule4.WaitTest3Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST1_POS:
            *readParam = appParamLocalPtr->railParamModule4.ReTest1Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST2_POS:
            *readParam = appParamLocalPtr->railParamModule4.ReTest2Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST3_POS:
            *readParam = appParamLocalPtr->railParamModule4.ReTest3Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST4_POS:
            *readParam = appParamLocalPtr->railParamModule4.ReTest4Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST5_POS:
            *readParam = appParamLocalPtr->railParamModule4.ReTest5Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST6_POS:
            *readParam = appParamLocalPtr->railParamModule4.ReTest6Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST7_POS:
            *readParam = appParamLocalPtr->railParamModule4.ReTest7Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST8_POS:
            *readParam = appParamLocalPtr->railParamModule4.ReTest8Position;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_PUT:
            *readParam = appParamLocalPtr->railParamModule4.TestingPutPosition;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_CATCH:
            *readParam = appParamLocalPtr->railParamModule4.TestingCatchPosition;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_ABSORB:
            *readParam = appParamLocalPtr->railParamModule4.TestingAbsorbPosition;
            break;
        case APP_PARAM_INDEX_SUB_RAIL_MODULE_TUBE_OFFSET_TESTING:
            *readParam = appParamLocalPtr->railParamModule4.tubeOffsetTesting;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}
/****************************************机械手独立参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamHandIndependent(uint16_t subIndex,int32_t writeParam)
{
    APP_PARAM_INDEX_SUB_HAND_INDENPENDENT paramIndex = (APP_PARAM_INDEX_SUB_HAND_INDENPENDENT)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_UPDOWN_RESET_CORRECTION:
            targetParamPtr = &(appParamLocalPtr->handParamIndependent.handUpDownResetCorrection);
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_RESET_CORRECTION:
            targetParamPtr = &(appParamLocalPtr->handParamIndependent.handBackFrontResetCorrection);
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_ROTATE_RESET_CORRECTION:
            targetParamPtr = &(appParamLocalPtr->handParamIndependent.handRotateResetCorrection);
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_UPDOWN_HIGH_MAX_POS:
            targetParamPtr = &(appParamLocalPtr->handParamIndependent.handUpDownHighMaxPosition);
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_UPDOWN_LOW_MIN_POS:
            targetParamPtr = &(appParamLocalPtr->handParamIndependent.handUpDownLowMaxPosition);
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_PUSHIN_1_POS:
            targetParamPtr = &(appParamLocalPtr->handParamIndependent.handBackFrontPushIn1Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_PUSHIN_2_POS:
            targetParamPtr = &(appParamLocalPtr->handParamIndependent.handBackFrontPushIn2Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_PUSH_OUT_1_POS:
            targetParamPtr = &(appParamLocalPtr->handParamIndependent.handBackFrontPushOut1Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_PUSH_OUT_2_POS:
            targetParamPtr = &(appParamLocalPtr->handParamIndependent.handBackFrontPushOut2Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_EMERGENCY_POS:
            targetParamPtr = &(appParamLocalPtr->handParamIndependent.handBackFrontEmergencyPosition);
            break;
        // case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_UTIL_ROTATE_OFFSET:
        //     targetParamPtr = &(appParamLocalPtr->handParamIndependent.handBackFrontUtilRotateOffset);
        //     break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_ROTATE_DIR_PUSH_IN_POS:
            targetParamPtr = &(appParamLocalPtr->handParamIndependent.handRotateDirPushInPosition);
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_ROTATE_DIR_EMERGENCY_POS:
            targetParamPtr = &(appParamLocalPtr->handParamIndependent.handRotateDirEmergencyPosition);
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
LH_ERR AppParamReadParamHandIndependent(uint16_t subIndex,int32_t* readParam)
{
    APP_PARAM_INDEX_SUB_HAND_INDENPENDENT paramIndex = (APP_PARAM_INDEX_SUB_HAND_INDENPENDENT)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_UPDOWN_RESET_CORRECTION:
            *readParam = appParamLocalPtr->handParamIndependent.handUpDownResetCorrection;
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_RESET_CORRECTION:
            *readParam = appParamLocalPtr->handParamIndependent.handBackFrontResetCorrection;
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_ROTATE_RESET_CORRECTION:
            *readParam = appParamLocalPtr->handParamIndependent.handRotateResetCorrection;
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_UPDOWN_HIGH_MAX_POS:
            *readParam = appParamLocalPtr->handParamIndependent.handUpDownHighMaxPosition;
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_UPDOWN_LOW_MIN_POS:
            *readParam = appParamLocalPtr->handParamIndependent.handUpDownLowMaxPosition;
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_PUSHIN_1_POS:
            *readParam = appParamLocalPtr->handParamIndependent.handBackFrontPushIn1Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_PUSHIN_2_POS:
            *readParam = appParamLocalPtr->handParamIndependent.handBackFrontPushIn2Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_PUSH_OUT_1_POS:
            *readParam = appParamLocalPtr->handParamIndependent.handBackFrontPushOut1Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_PUSH_OUT_2_POS:
            *readParam = appParamLocalPtr->handParamIndependent.handBackFrontPushOut2Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_EMERGENCY_POS:
            *readParam = appParamLocalPtr->handParamIndependent.handBackFrontEmergencyPosition;
            break;
        // case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_BACKFRONT_UTIL_ROTATE_OFFSET:
        //     *readParam = appParamLocalPtr->handParamIndependent.handBackFrontUtilRotateOffset;
        //     break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_ROTATE_DIR_PUSH_IN_POS:
            *readParam = appParamLocalPtr->handParamIndependent.handRotateDirPushInPosition;
            break;
        case APP_PARAM_INDEX_SUB_HAND_INDENPENDENT_ROTATE_DIR_EMERGENCY_POS:
            *readParam = appParamLocalPtr->handParamIndependent.handRotateDirEmergencyPosition;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}
/****************************************机械手模块1参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamHandModule1(uint16_t subIndex,int32_t writeParam)
{
    APP_PARAM_INDEX_SUB_HAND_MODULE paramIndex = (APP_PARAM_INDEX_SUB_HAND_MODULE)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_1_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule1.handBackFrontWaitTest1Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_2_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule1.handBackFrontWaitTest2Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_3_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule1.handBackFrontWaitTest3Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_1_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule1.handBackFrontReTest1Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_2_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule1.handBackFrontReTest2Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_3_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule1.handBackFrontReTest3Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_4_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule1.handBackFrontReTest4Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_5_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule1.handBackFrontReTest5Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_6_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule1.handBackFrontReTest6Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_7_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule1.handBackFrontReTest7Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_8_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule1.handBackFrontReTest8Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_TESTING_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule1.handBackFrontTestingPosition);
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
LH_ERR AppParamReadParamHandModule1(uint16_t subIndex,int32_t* readParam)
{
    APP_PARAM_INDEX_SUB_HAND_MODULE paramIndex = (APP_PARAM_INDEX_SUB_HAND_MODULE)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_1_POS:
            *readParam = appParamLocalPtr->handParamModule1.handBackFrontWaitTest1Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_2_POS:
            *readParam = appParamLocalPtr->handParamModule1.handBackFrontWaitTest2Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_3_POS:
            *readParam = appParamLocalPtr->handParamModule1.handBackFrontWaitTest3Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_1_POS:
            *readParam = appParamLocalPtr->handParamModule1.handBackFrontReTest1Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_2_POS:
            *readParam = appParamLocalPtr->handParamModule1.handBackFrontReTest2Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_3_POS:
            *readParam = appParamLocalPtr->handParamModule1.handBackFrontReTest3Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_4_POS:
            *readParam = appParamLocalPtr->handParamModule1.handBackFrontReTest4Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_5_POS:
            *readParam = appParamLocalPtr->handParamModule1.handBackFrontReTest5Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_6_POS:
            *readParam = appParamLocalPtr->handParamModule1.handBackFrontReTest6Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_7_POS:
            *readParam = appParamLocalPtr->handParamModule1.handBackFrontReTest7Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_8_POS:
            *readParam = appParamLocalPtr->handParamModule1.handBackFrontReTest8Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_TESTING_POS:
            *readParam = appParamLocalPtr->handParamModule1.handBackFrontTestingPosition;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}
/****************************************机械手模块2参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamHandModule2(uint16_t subIndex,int32_t writeParam)
{
    APP_PARAM_INDEX_SUB_HAND_MODULE paramIndex = (APP_PARAM_INDEX_SUB_HAND_MODULE)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_1_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule2.handBackFrontWaitTest1Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_2_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule2.handBackFrontWaitTest2Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_3_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule2.handBackFrontWaitTest3Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_1_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule2.handBackFrontReTest1Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_2_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule2.handBackFrontReTest2Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_3_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule2.handBackFrontReTest3Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_4_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule2.handBackFrontReTest4Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_5_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule2.handBackFrontReTest5Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_6_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule2.handBackFrontReTest6Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_7_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule2.handBackFrontReTest7Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_8_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule2.handBackFrontReTest8Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_TESTING_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule2.handBackFrontTestingPosition);
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
LH_ERR AppParamReadParamHandModule2(uint16_t subIndex,int32_t* readParam)
{
    APP_PARAM_INDEX_SUB_HAND_MODULE paramIndex = (APP_PARAM_INDEX_SUB_HAND_MODULE)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_1_POS:
            *readParam = appParamLocalPtr->handParamModule2.handBackFrontWaitTest1Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_2_POS:
            *readParam = appParamLocalPtr->handParamModule2.handBackFrontWaitTest2Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_3_POS:
            *readParam = appParamLocalPtr->handParamModule2.handBackFrontWaitTest3Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_1_POS:
            *readParam = appParamLocalPtr->handParamModule2.handBackFrontReTest1Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_2_POS:
            *readParam = appParamLocalPtr->handParamModule2.handBackFrontReTest2Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_3_POS:
            *readParam = appParamLocalPtr->handParamModule2.handBackFrontReTest3Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_4_POS:
            *readParam = appParamLocalPtr->handParamModule2.handBackFrontReTest4Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_5_POS:
            *readParam = appParamLocalPtr->handParamModule2.handBackFrontReTest5Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_6_POS:
            *readParam = appParamLocalPtr->handParamModule2.handBackFrontReTest6Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_7_POS:
            *readParam = appParamLocalPtr->handParamModule2.handBackFrontReTest7Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_8_POS:
            *readParam = appParamLocalPtr->handParamModule2.handBackFrontReTest8Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_TESTING_POS:
            *readParam = appParamLocalPtr->handParamModule2.handBackFrontTestingPosition;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}
/****************************************机械手模块3参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamHandModule3(uint16_t subIndex,int32_t writeParam)
{
    APP_PARAM_INDEX_SUB_HAND_MODULE paramIndex = (APP_PARAM_INDEX_SUB_HAND_MODULE)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_1_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule3.handBackFrontWaitTest1Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_2_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule3.handBackFrontWaitTest2Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_3_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule3.handBackFrontWaitTest3Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_1_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule3.handBackFrontReTest1Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_2_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule3.handBackFrontReTest2Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_3_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule3.handBackFrontReTest3Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_4_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule3.handBackFrontReTest4Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_5_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule3.handBackFrontReTest5Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_6_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule3.handBackFrontReTest6Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_7_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule3.handBackFrontReTest7Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_8_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule3.handBackFrontReTest8Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_TESTING_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule3.handBackFrontTestingPosition);
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
LH_ERR AppParamReadParamHandModule3(uint16_t subIndex,int32_t* readParam)
{
    APP_PARAM_INDEX_SUB_HAND_MODULE paramIndex = (APP_PARAM_INDEX_SUB_HAND_MODULE)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_1_POS:
            *readParam = appParamLocalPtr->handParamModule3.handBackFrontWaitTest1Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_2_POS:
            *readParam = appParamLocalPtr->handParamModule3.handBackFrontWaitTest2Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_3_POS:
            *readParam = appParamLocalPtr->handParamModule3.handBackFrontWaitTest3Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_1_POS:
            *readParam = appParamLocalPtr->handParamModule3.handBackFrontReTest1Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_2_POS:
            *readParam = appParamLocalPtr->handParamModule3.handBackFrontReTest2Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_3_POS:
            *readParam = appParamLocalPtr->handParamModule3.handBackFrontReTest3Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_4_POS:
            *readParam = appParamLocalPtr->handParamModule3.handBackFrontReTest4Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_5_POS:
            *readParam = appParamLocalPtr->handParamModule3.handBackFrontReTest5Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_6_POS:
            *readParam = appParamLocalPtr->handParamModule3.handBackFrontReTest6Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_7_POS:
            *readParam = appParamLocalPtr->handParamModule3.handBackFrontReTest7Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_8_POS:
            *readParam = appParamLocalPtr->handParamModule3.handBackFrontReTest8Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_TESTING_POS:
            *readParam = appParamLocalPtr->handParamModule3.handBackFrontTestingPosition;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}
/****************************************机械手模块4参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamHandModule4(uint16_t subIndex,int32_t writeParam)
{
    APP_PARAM_INDEX_SUB_HAND_MODULE paramIndex = (APP_PARAM_INDEX_SUB_HAND_MODULE)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_1_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule4.handBackFrontWaitTest1Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_2_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule4.handBackFrontWaitTest2Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_3_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule4.handBackFrontWaitTest3Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_1_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule4.handBackFrontReTest1Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_2_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule4.handBackFrontReTest2Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_3_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule4.handBackFrontReTest3Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_4_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule4.handBackFrontReTest4Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_5_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule4.handBackFrontReTest5Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_6_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule4.handBackFrontReTest6Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_7_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule4.handBackFrontReTest7Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_8_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule4.handBackFrontReTest8Position);
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_TESTING_POS:
            targetParamPtr = &(appParamLocalPtr->handParamModule4.handBackFrontTestingPosition);
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
LH_ERR AppParamReadParamHandModule4(uint16_t subIndex,int32_t* readParam)
{
    APP_PARAM_INDEX_SUB_HAND_MODULE paramIndex = (APP_PARAM_INDEX_SUB_HAND_MODULE)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_1_POS:
            *readParam = appParamLocalPtr->handParamModule4.handBackFrontWaitTest1Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_2_POS:
            *readParam = appParamLocalPtr->handParamModule4.handBackFrontWaitTest2Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_WAIT_TEST_3_POS:
            *readParam = appParamLocalPtr->handParamModule4.handBackFrontWaitTest3Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_1_POS:
            *readParam = appParamLocalPtr->handParamModule4.handBackFrontReTest1Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_2_POS:
            *readParam = appParamLocalPtr->handParamModule4.handBackFrontReTest2Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_3_POS:
            *readParam = appParamLocalPtr->handParamModule4.handBackFrontReTest3Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_4_POS:
            *readParam = appParamLocalPtr->handParamModule4.handBackFrontReTest4Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_5_POS:
            *readParam = appParamLocalPtr->handParamModule4.handBackFrontReTest5Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_6_POS:
            *readParam = appParamLocalPtr->handParamModule4.handBackFrontReTest6Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_7_POS:
            *readParam = appParamLocalPtr->handParamModule4.handBackFrontReTest7Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_RE_TEST_8_POS:
            *readParam = appParamLocalPtr->handParamModule4.handBackFrontReTest8Position;
            break;
        case APP_PARAM_INDEX_SUB_HAND_MODULE_BACKFRONT_TESTING_POS:
            *readParam = appParamLocalPtr->handParamModule4.handBackFrontTestingPosition;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}
/****************************************应用辅助参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamUtil(uint16_t subIndex,int32_t writeParam)
{
    APP_PARAM_INDEX_SUB_UTIL paramIndex = (APP_PARAM_INDEX_SUB_UTIL)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_HAND_UTIL_RESERVE:
            targetParamPtr = &(appParamLocalPtr->utilParam.reserveInit);
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
LH_ERR AppParamReadParamUtil(uint16_t subIndex,int32_t* readParam)
{
    APP_PARAM_INDEX_SUB_UTIL paramIndex = (APP_PARAM_INDEX_SUB_UTIL)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_INDEX_SUB_HAND_UTIL_RESERVE:
            *readParam = appParamLocalPtr->utilParam.reserveInit;
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
    AppParamReadParamPushIn1,
    AppParamReadParamPushIn2,
    AppParamReadParamPushOut1,
    AppParamReadParamPushOut2,
    AppParamReadParamRailIndependent,
    AppParamReadParamRailModule1,
    AppParamReadParamRailModule2,
    AppParamReadParamRailModule3,
    AppParamReadParamRailModule4,
    AppParamReadParamHandIndependent,
    AppParamReadParamHandModule1,
    AppParamReadParamHandModule2,
    AppParamReadParamHandModule3,
    AppParamReadParamHandModule4,
    AppParamReadParamUtil,
};
#define COUNT_APP_UTIL_FUNC_READ_PARAM          (sizeof(AppParamReadParamSingleArray)/sizeof(AppParamReadParamSingleArray[0]))

/****************************************参数写入函数指针数组**************************************************/
static const AppParamWriteParamSingleFuncPtr AppParamWriteParamSingleArray[] = {
    AppParamWriteParamPushIn1,
    AppParamWriteParamPushIn2,
    AppParamWriteParamPushOut1,
    AppParamWriteParamPushOut2,
    AppParamWriteParamRailIndependent,
    AppParamWriteParamRailModule1,
    AppParamWriteParamRailModule2,
    AppParamWriteParamRailModule3,
    AppParamWriteParamRailModule4,
    AppParamWriteParamHandIndependent,
    AppParamWriteParamHandModule1,
    AppParamWriteParamHandModule2,
    AppParamWriteParamHandModule3,
    AppParamWriteParamHandModule4,
    AppParamWriteParamUtil,
};
#define COUNT_APP_UTIL_FUNC_WRITE_PARAM         (sizeof(AppParamWriteParamSingleArray)/sizeof(AppParamWriteParamSingleArray[0]))

//写入参数
LH_ERR AppParamWriteParam(uint16_t mainIndex,uint16_t subIndex,int32_t writeParam)
{
    if(mainIndex >= COUNT_APP_UTIL_FUNC_WRITE_PARAM)
    {
        return LH_ERR_APP_PARAM_WRITE;
    }
    return AppParamWriteParamSingleArray[mainIndex](subIndex,writeParam);
}

//读取参数
LH_ERR AppParamReadParam(uint16_t mainIndex,uint16_t subIndex,int32_t* readParam)
{
    if(mainIndex >= COUNT_APP_UTIL_FUNC_READ_PARAM)
    {
        return LH_ERR_APP_PARAM_READ;
    }
    return AppParamReadParamSingleArray[mainIndex](subIndex,readParam);
}



