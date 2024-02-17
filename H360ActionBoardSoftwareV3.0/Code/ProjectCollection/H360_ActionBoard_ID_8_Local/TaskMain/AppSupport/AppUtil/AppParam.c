/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-11-08 16:09:26
**LastEditors: DengXiaoJun
**LastEditTime: 2021-12-24 15:16:51
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

static APP_PARAM_SET* appParamPtr = NULL;

//系统应用参数初始化
void AppParamInit(void)
{
    if(appParamPtr != NULL)
    {
        UserMemFree(SRAM_IS62,appParamPtr);
    }
    //首先申请内存
    appParamPtr = NULL;
    //申请内存
    appParamPtr = UserMemMallocWhileSuccess(SRAM_IS62,LENGTH_APP_PARAM_SET);
    //从Flash中加载参数
    BoardMB85RS2MT_ReadBuffer(ADDR_START_APP_PARAM,(uint8_t*)appParamPtr,LENGTH_APP_PARAM_SET);
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
    //参数装载默认值
    UserMemCopy((void*)(&(appParamPtr->paramRackStoreIn))   ,(void*)(&paramRackStoreInDefault)  ,LENGTH_APP_PARAM_RACK_STORE_IN  );
    UserMemCopy((void*)(&(appParamPtr->paramRackStoreOut))  ,(void*)(&paramRackStoreOutDefault) ,LENGTH_APP_PARAM_RACK_STORE_OUT );
    UserMemCopy((void*)(&(appParamPtr->paramRackEntry))     ,(void*)(&paramRackEntryDefault)    ,LENGTH_APP_PARAM_RACK_ENTRY     );
    UserMemCopy((void*)(&(appParamPtr->paramRackBarScan))   ,(void*)(&paramRackBarScanDefault)  ,LENGTH_APP_PARAM_RACK_BAR_SCAN  );
    UserMemCopy((void*)(&(appParamPtr->paramRackTest))      ,(void*)(&paramRackTestDefault)     ,LENGTH_APP_PARAM_RACK_TEST      );
    UserMemCopy((void*)(&(appParamPtr->paramRackTest))      ,(void*)(&paramRackInSwitchDefault) ,LENGTH_APP_PARAM_RACK_IN_SWITCH );
    UserMemCopy((void*)(&(appParamPtr->paramRackWaitOut1))  ,(void*)(&paramRackWaitOut1Default) ,LENGTH_APP_PARAM_RACK_WAIT_OUT1 );
    UserMemCopy((void*)(&(appParamPtr->paramRackWaitOut2))  ,(void*)(&paramRackWaitOut2Default) ,LENGTH_APP_PARAM_RACK_WAIT_OUT2 );
    UserMemCopy((void*)(&(appParamPtr->paramRackOutSwitch)) ,(void*)(&paramRackOutSwitchDefault),LENGTH_APP_PARAM_RACK_OUT_SWITCH);
    //将设定好的值写回外置Flash
    BoardMB85RS2MT_WriteBuffer(ADDR_START_APP_PARAM,(uint8_t*)appParamPtr,LENGTH_APP_PARAM_SET);
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

/****************************************进样仓参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteRackStoreIn(uint16_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case APP_PARAM_SUB_RACK_STORE_IN_PUSH_MOTOR_RESET_CORRECT:
            targetParamPtr = &(appParamPtr->paramRackStoreIn.pushMotorResetCorrect);
            break;
        case APP_PARAM_SUB_RACK_STORE_IN_TO_END:
            targetParamPtr = &(appParamPtr->paramRackStoreIn.pushMotorRackEndMax);
            break;
        case APP_PARAM_SUB_RACK_STORE_IN_STEP_COMPENSATE_TO_ENTRY:
            targetParamPtr = &(appParamPtr->paramRackStoreIn.stepsCompensateToEntry);
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
LH_ERR AppParamReadRackStoreIn(uint16_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case APP_PARAM_SUB_RACK_STORE_IN_PUSH_MOTOR_RESET_CORRECT:
            *readParam = appParamPtr->paramRackStoreIn.pushMotorResetCorrect;
            break;
        case APP_PARAM_SUB_RACK_STORE_IN_TO_END:
            *readParam = appParamPtr->paramRackStoreIn.pushMotorRackEndMax;
            break;
        case APP_PARAM_SUB_RACK_STORE_IN_STEP_COMPENSATE_TO_ENTRY:
            *readParam = appParamPtr->paramRackStoreIn.stepsCompensateToEntry;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}

/****************************************出样仓参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteRackStoreOut(uint16_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case APP_PARAM_SUB_RACK_STORE_OUT_PUSH_MOTOR_RESET_CORRECT:
            targetParamPtr = &(appParamPtr->paramRackStoreOut.pushMotorResetCorrect);
            break;
        case APP_PARAM_SUB_RACK_STORE_OUT_PUSH_START_POS:
            targetParamPtr = &(appParamPtr->paramRackStoreOut.rackStartPushPos);
            break;
        case APP_PARAM_SUB_RACK_STORE_OUT_PUSH_ONCE_STEP:
            targetParamPtr = &(appParamPtr->paramRackStoreOut.pushOnceStep);
            break;
        case APP_PARAM_SUB_RACK_STORE_OUT_PUSH_COUNT_MAX_TARY_NEAR_KEY:
            targetParamPtr = &(appParamPtr->paramRackStoreOut.pushRackCountMaxTaryNearKey);
            break;
        case APP_PARAM_SUB_RACK_STORE_OUT_ROTATE_ACTION_POS:
            targetParamPtr = &(appParamPtr->paramRackStoreOut.rotateActionPos);
            break;
        case APP_PARAM_SUB_RACK_STORE_OUT_ROTATE_IDLE_POS:
            targetParamPtr = &(appParamPtr->paramRackStoreOut.rotateIdlePos);
            break;
        case APP_PARAM_SUB_RACK_STORE_OUT_PUSH_COUNT_MAX_TARY_NEAR_RAIL:
            targetParamPtr = &(appParamPtr->paramRackStoreOut.pushRackCountMaxTaryNearRail);
            break;
        case APP_PARAM_SUB_RACK_STORE_OUT_PUSH_STEP_SWITCH_TO_STORE:
            targetParamPtr = &(appParamPtr->paramRackStoreOut.stepRackFormOutSwitchMoveOutToStore);
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
LH_ERR AppParamReadRackStoreOut(uint16_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case APP_PARAM_SUB_RACK_STORE_OUT_PUSH_MOTOR_RESET_CORRECT:
            *readParam = appParamPtr->paramRackStoreOut.pushMotorResetCorrect;
            break;
        case APP_PARAM_SUB_RACK_STORE_OUT_PUSH_START_POS:
            *readParam = appParamPtr->paramRackStoreOut.rackStartPushPos;
            break;
        case APP_PARAM_SUB_RACK_STORE_OUT_PUSH_ONCE_STEP:
            *readParam = appParamPtr->paramRackStoreOut.pushOnceStep;
            break;
        case APP_PARAM_SUB_RACK_STORE_OUT_PUSH_COUNT_MAX_TARY_NEAR_KEY:
            *readParam = appParamPtr->paramRackStoreOut.pushRackCountMaxTaryNearKey;
            break;
        case APP_PARAM_SUB_RACK_STORE_OUT_ROTATE_ACTION_POS:
            *readParam = appParamPtr->paramRackStoreOut.rotateActionPos;
            break;
        case APP_PARAM_SUB_RACK_STORE_OUT_ROTATE_IDLE_POS:
            *readParam = appParamPtr->paramRackStoreOut.rotateIdlePos;
            break;
        case APP_PARAM_SUB_RACK_STORE_OUT_PUSH_COUNT_MAX_TARY_NEAR_RAIL:
            *readParam = appParamPtr->paramRackStoreOut.pushRackCountMaxTaryNearRail;
            break;
        case APP_PARAM_SUB_RACK_STORE_OUT_PUSH_STEP_SWITCH_TO_STORE:
            *readParam = appParamPtr->paramRackStoreOut.stepRackFormOutSwitchMoveOutToStore;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}

/****************************************轨道进样节点参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteRackEntry(uint16_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case APP_PARAM_SUB_RACK_ENTRY_ACTION_POS:
            targetParamPtr = &(appParamPtr->paramRackEntry.actionPos);
            break;
        case APP_PARAM_SUB_RACK_ENTRY_IDLE_POS:
            targetParamPtr = &(appParamPtr->paramRackEntry.idlePos);
            break;
        case APP_PARAM_SUB_RACK_CONVEYOR_BELT_MOTOR_TYPE:
            targetParamPtr = &(appParamPtr->paramRackEntry.railConveyorBeltMotorType);
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
LH_ERR AppParamReadRackEntry(uint16_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case APP_PARAM_SUB_RACK_ENTRY_ACTION_POS:
            *readParam = appParamPtr->paramRackEntry.actionPos;
            break;
        case APP_PARAM_SUB_RACK_ENTRY_IDLE_POS:
            *readParam = appParamPtr->paramRackEntry.idlePos;
            break;
        case APP_PARAM_SUB_RACK_CONVEYOR_BELT_MOTOR_TYPE:
            *readParam = appParamPtr->paramRackEntry.railConveyorBeltMotorType;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}

/****************************************轨道进样扫码节点参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteRackBarScan(uint16_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case APP_PARAM_SUB_RACK_BAR_SCAN_BLOCK_MOTOR_ACTION_POS:
            targetParamPtr = &(appParamPtr->paramRackBarScan.blockMotorActionPos);
            break;
        case APP_PARAM_SUB_RACK_BAR_SCAN_BLOCK_MOTOR_IDLE_POS:
            targetParamPtr = &(appParamPtr->paramRackBarScan.blockMotorIdlePos);
            break;
        case APP_PARAM_SUB_RACK_BAR_SCAN_PUSH_MOTOR_START_POS:
            targetParamPtr = &(appParamPtr->paramRackBarScan.pushMotorStartPos);
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
LH_ERR AppParamReadRackBarScan(uint16_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case APP_PARAM_SUB_RACK_BAR_SCAN_BLOCK_MOTOR_ACTION_POS:
            *readParam = appParamPtr->paramRackBarScan.blockMotorActionPos;
            break;
        case APP_PARAM_SUB_RACK_BAR_SCAN_BLOCK_MOTOR_IDLE_POS:
            *readParam = appParamPtr->paramRackBarScan.blockMotorIdlePos;
            break;
        case APP_PARAM_SUB_RACK_BAR_SCAN_PUSH_MOTOR_START_POS:
            *readParam = appParamPtr->paramRackBarScan.pushMotorStartPos;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}

/****************************************轨道进样测试节点参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteRackTest(uint16_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case APP_PARAM_SUB_RACK_TEST_RESET_CORRECT_AND_TUBE5_POS:
            targetParamPtr = &(appParamPtr->paramRackTest.resetCorrectAndTube5Pos);
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
LH_ERR AppParamReadRackTest(uint16_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case APP_PARAM_SUB_RACK_TEST_RESET_CORRECT_AND_TUBE5_POS:
            *readParam = appParamPtr->paramRackTest.resetCorrectAndTube5Pos;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}

/****************************************轨道进样换轨节点参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteRackInSwitch(uint16_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case APP_PARAM_SUB_RACK_IN_SWITCH_COME_IN_POS:
            targetParamPtr = &(appParamPtr->paramRackInSwitch.comeInPos);
            break;
        case APP_PARAM_SUB_RACK_IN_SWITCH_MOVE_OUT_POS:
            targetParamPtr = &(appParamPtr->paramRackInSwitch.moveOutPos);
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
LH_ERR AppParamReadRackInSwitch(uint16_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case APP_PARAM_SUB_RACK_IN_SWITCH_COME_IN_POS:
            *readParam = appParamPtr->paramRackInSwitch.comeInPos;
            break;
        case APP_PARAM_SUB_RACK_IN_SWITCH_MOVE_OUT_POS:
            *readParam = appParamPtr->paramRackInSwitch.moveOutPos;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}

/****************************************轨道出样等待节点1参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteRackWaitOut1(uint16_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case APP_PARAM_SUB_RACK_WAIT_OUT1_ACTION_POS:
            targetParamPtr = &(appParamPtr->paramRackWaitOut1.actionPos);
            break;
        case APP_PARAM_SUB_RACK_WAIT_OUT1_IDLE_POS:
            targetParamPtr = &(appParamPtr->paramRackWaitOut1.idlePos);
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
LH_ERR AppParamReadRackWaitOut1(uint16_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case APP_PARAM_SUB_RACK_WAIT_OUT1_ACTION_POS:
            *readParam = appParamPtr->paramRackWaitOut1.actionPos;
            break;
        case APP_PARAM_SUB_RACK_WAIT_OUT1_IDLE_POS:
            *readParam = appParamPtr->paramRackWaitOut1.idlePos;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}

/****************************************轨道出样等待节点2参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteRackWaitOut2(uint16_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case APP_PARAM_SUB_RACK_WAIT_OUT2_ACTION_POS:
            targetParamPtr = &(appParamPtr->paramRackWaitOut2.actionPos);
            break;
        case APP_PARAM_SUB_RACK_WAIT_OUT2_IDLE_POS:
            targetParamPtr = &(appParamPtr->paramRackWaitOut2.idlePos);
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
LH_ERR AppParamReadRackWaitOut2(uint16_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case APP_PARAM_SUB_RACK_WAIT_OUT2_ACTION_POS:
            *readParam = appParamPtr->paramRackWaitOut2.actionPos;
            break;
        case APP_PARAM_SUB_RACK_WAIT_OUT2_IDLE_POS:
            *readParam = appParamPtr->paramRackWaitOut2.idlePos;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}

/****************************************轨道出样换轨节点参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteRackOutSwitch(uint16_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case APP_PARAM_SUB_RACK_OUT_SWITCH_COME_IN_POS:
            targetParamPtr = &(appParamPtr->paramRackOutSwitch.comeInPos);
            break;
        case APP_PARAM_SUB_RACK_OUT_SWITCH_MOVE_OUT_POS:
            targetParamPtr = &(appParamPtr->paramRackOutSwitch.moveOutPos);
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
LH_ERR AppParamReadRackOutSwitch(uint16_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case APP_PARAM_SUB_RACK_OUT_SWITCH_COME_IN_POS:
            *readParam = appParamPtr->paramRackOutSwitch.comeInPos;
            break;
        case APP_PARAM_SUB_RACK_OUT_SWITCH_MOVE_OUT_POS:
            *readParam = appParamPtr->paramRackOutSwitch.moveOutPos;
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
    AppParamReadRackStoreIn,
    AppParamReadRackStoreOut,
    AppParamReadRackEntry,
    AppParamReadRackBarScan,
    AppParamReadRackTest,
    AppParamReadRackInSwitch,
    AppParamReadRackWaitOut1,
    AppParamReadRackWaitOut2,
    AppParamReadRackOutSwitch,
};
#define COUNT_APP_UTIL_FUNC_READ_PARAM          (sizeof(AppParamReadParamSingleArray)/sizeof(AppParamReadParamSingleArray[0]))

/****************************************参数写入函数指针数组**************************************************/
static const AppParamWriteParamSingleFuncPtr AppParamWriteParamSingleArray[] = {
    AppParamWriteRackStoreIn,
    AppParamWriteRackStoreOut,
    AppParamWriteRackEntry,
    AppParamWriteRackBarScan,
    AppParamWriteRackTest,
    AppParamWriteRackInSwitch,
    AppParamWriteRackWaitOut1,
    AppParamWriteRackWaitOut2,
    AppParamWriteRackOutSwitch,
};
#define COUNT_APP_UTIL_FUNC_WRITE_PARAM         (sizeof(AppParamWriteParamSingleArray)/sizeof(AppParamWriteParamSingleArray[0]))

//写入参数
LH_ERR AppParamWriteParam(APP_PARAM_MAIN mainIndex,uint16_t subIndex,int32_t writeParam)
{
    if(mainIndex >= COUNT_APP_UTIL_FUNC_WRITE_PARAM)
    {
        return LH_ERR_APP_PARAM_WRITE;
    }
    return AppParamWriteParamSingleArray[mainIndex](subIndex,writeParam);
}

//读取参数
LH_ERR AppParamReadParam(APP_PARAM_MAIN mainIndex,uint16_t subIndex,int32_t* readParam)
{
    if(mainIndex >= COUNT_APP_UTIL_FUNC_READ_PARAM)
    {
        return LH_ERR_APP_PARAM_READ;
    }
    return AppParamReadParamSingleArray[mainIndex](subIndex,readParam);
}
