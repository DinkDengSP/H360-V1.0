/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:54:56
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-17 09:55:53
**FilePath: \H360_ActionBoard_ID_2\TaskMain\AppSupport\AppUtil\AppParam.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppParam.h"
#include "AppTaskConfig.h"

//APP写入单个参数的指针
typedef LH_ERR (*AppParamWriteParamSingleFuncPtr)(uint8_t subIndex,int32_t writeParam);
//APP读取单个参数的指针
typedef LH_ERR (*AppParamReadParamSingleFuncPtr)(uint8_t subIndex,int32_t* readParam);

static APP_PARAM_BOARD2* appParamBoard2Ptr = NULL;

void AppParamInit(void)
{
    //首先申请内存
    appParamBoard2Ptr = NULL;
    //申请内存
    do
    {
        appParamBoard2Ptr = UserMemMalloc(SRAM_IS62,LENGTH_APP_PARAM_BOARD2);
        if(appParamBoard2Ptr == NULL)
        {
            CoreDelayMinTick();
        }
    } while (appParamBoard2Ptr == NULL);

    //从Flash中加载参数
    BoardMB85RS2MT_ReadBuffer(ADDR_START_APP_PARAM,(uint8_t*)appParamBoard2Ptr,LENGTH_APP_PARAM_BOARD2);
    //数据校验
    if(appParamBoard2Ptr->paramFlag == APP_PARAM_DEFAULT_FLAG)
    {
        //校验通过
        return;
    }
    //校验不通过,设定为默认配置
    AppParamSetDefault();
}


//系统参数设置为默认值
LH_ERR AppParamSetDefault(void)
{
    //设定为默认配置
    appParamBoard2Ptr->paramFlag = APP_PARAM_DEFAULT_FLAG;
    //参数装载
    //磁珠针参数
    UserMemCopy((void*)(&(appParamBoard2Ptr->needleBeadParam)),(void*)(&appParamNeedleBeadDefaultSetting),LENGTH_APP_PARAM_NEEDLE_BEAD);
    //试剂盘参数
    UserMemCopy((void*)(&(appParamBoard2Ptr->plateReagentParam)),(void*)(&appParamPlateReagentDefaultSetting),LENGTH_APP_PARAM_PLATE_REAGENT);
    //杯栈管理参数
    UserMemCopy((void*)(&(appParamBoard2Ptr->cupTrayStackManageParam)),(void*)(&appParamCupTrayStackManageDefaultSetting),LENGTH_APP_PARAM_CUP_TRAY_STACK_MANAGE);
    //辅助参数
    UserMemCopy((void*)(&(appParamBoard2Ptr->otherParam)),(void*)(&appParamOtherDefaultSetting),LENGTH_APP_PARAM_OTHER);
    //将设定好的值写回外置Flash
    BoardMB85RS2MT_WriteBuffer(ADDR_START_APP_PARAM,(uint8_t*)appParamBoard2Ptr,LENGTH_APP_PARAM_BOARD2);

    return LH_ERR_NONE;
}


//通用的写入Flash的函数
static void AppParamWriteSingleCommon(int32_t* targetParamPtr,int32_t writeParam)
{
    *targetParamPtr = writeParam;
    int32_t targetAddr = (int32_t)((char*)targetParamPtr);
    int32_t baseAddr = (int32_t)((char*)(appParamBoard2Ptr));
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


/****************************************磁珠针参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamNeedleBead(uint8_t subIndex,int32_t writeParam)
{
    APP_PARAM_SUB_INDEX_NEEDLE_BEAD paramIndex = (APP_PARAM_SUB_INDEX_NEEDLE_BEAD)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_ROTATE_RESET_CORRECT:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.rotateResetCorrect);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_UPDOWN_RESET_CORRECT:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.upDownResetCorrect);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_ROTATE_TO_CLEAN:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posRotateToClean);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_CLEAN:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posUpDownClean);
            break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_TIME_MS_CLEAN_SET1:
        //     targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.timeMsCleanSet1);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_TIME_MS_CLEAN_SET2:
        //     targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.timeMsCleanSet2);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_TIME_MS_CLEAN_SET3:
        //     targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.timeMsCleanSet3);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_TIME_MS_CLEAN_SET4:
        //     targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.timeMsCleanSet4);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_TIME_MS_CLEAN_SET5:
        //     targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.timeMsCleanSet5);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_TIME_MS_PRIME:
        //     targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.timeMsPrime);
        //     break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_ROTATE_TO_R1:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posRotateToR1);
            break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_R1_LIQUID_DETECT_START:
        //     targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posUpDownR1LiquidDetectStart);
        //     break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_R1_LIQUID_DETECT_MAX_LIMIT:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posUpDownR1LiquidDetectMaxLimit);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_ROTATE_TO_R2:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posRotateToR2);
            break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_R2_LIQUID_DETECT_START:
        //     targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posUpDownR2LiquidDetectStart);
        //     break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_R2_LIQUID_DETECT_MAX_LIMIT:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posUpDownR2LiquidDetectMaxLimit);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_ROTATE_TO_BEAD:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posRotateToBead);
            break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_BEAD_LIQUID_DETECT_START:
        //     targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posUpDownBeadLiquidDetectStart);
        //     break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_BEAD_LIQUID_DETECT_MAX_LIMIT:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posUpDownBeadLiquidDetectMaxLimit);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_ROTATE_TO_PLATE_REACTION:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posRotateToPlateReaction);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_INJECT_AT_BOTTOM:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posUpDownInject1Shallowest);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_INJECT_CORRECT:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posUpDownInject2);
            break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_INJECT3:
        //     targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posUpDownInject3);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_INJECT4:
        //     targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posUpDownInject4);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_INJECT5:
        //     targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posUpDownInject5);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_INJECT6:
        //     targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posUpDownInject6);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_INJECT7:
        //     targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posUpDownInject7);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_INJECT8_DEEPEST:
        //     targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posUpDownInject8Deepest);
        //     break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_UL_ABSORB_AIR_AFTER_CLEAN:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.ulAbsorbAirAfterClean);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_UL_ABSORB_AIR_AFTER_LIQUID_ABSORB:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.ulAbsorbAirAfterLiquidAbsorb);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_UL_RESERVE_AIR_AFTER_INJECT:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.ulReserveAirAfterInject);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_LIQUID_DETECT_REAL_TIME_COMM:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.liquidDetectRealTimeComm);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_PUMP_GEAR_GAP:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.pumpGearGap);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_UPDOWN_LITTLE_BEAD_MIX:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.posUpDownLittleBeadMix);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_LIQUID_BOARD_VERSION_WB:
            targetParamPtr = &(appParamBoard2Ptr->needleBeadParam.liquidBoardVersionWB);
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
LH_ERR AppParamReadParamNeedleBead(uint8_t subIndex,int32_t* readParam)
{
    APP_PARAM_SUB_INDEX_NEEDLE_BEAD paramIndex = (APP_PARAM_SUB_INDEX_NEEDLE_BEAD)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_ROTATE_RESET_CORRECT:
            *readParam = appParamBoard2Ptr->needleBeadParam.rotateResetCorrect;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_UPDOWN_RESET_CORRECT:
            *readParam = appParamBoard2Ptr->needleBeadParam.upDownResetCorrect;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_ROTATE_TO_CLEAN:
            *readParam = appParamBoard2Ptr->needleBeadParam.posRotateToClean;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_CLEAN:
            *readParam = appParamBoard2Ptr->needleBeadParam.posUpDownClean;
            break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_TIME_MS_CLEAN_SET1:
        //     *readParam = appParamBoard2Ptr->needleBeadParam.timeMsCleanSet1;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_TIME_MS_CLEAN_SET2:
        //     *readParam = appParamBoard2Ptr->needleBeadParam.timeMsCleanSet2;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_TIME_MS_CLEAN_SET3:
        //     *readParam = appParamBoard2Ptr->needleBeadParam.timeMsCleanSet3;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_TIME_MS_CLEAN_SET4:
        //     *readParam = appParamBoard2Ptr->needleBeadParam.timeMsCleanSet4;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_TIME_MS_CLEAN_SET5:
        //     *readParam = appParamBoard2Ptr->needleBeadParam.timeMsCleanSet5;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_TIME_MS_PRIME:
        //     *readParam = appParamBoard2Ptr->needleBeadParam.timeMsPrime;
        //     break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_ROTATE_TO_R1:
            *readParam = appParamBoard2Ptr->needleBeadParam.posRotateToR1;
            break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_R1_LIQUID_DETECT_START:
        //     *readParam = appParamBoard2Ptr->needleBeadParam.posUpDownR1LiquidDetectStart;
        //     break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_R1_LIQUID_DETECT_MAX_LIMIT:
            *readParam = appParamBoard2Ptr->needleBeadParam.posUpDownR1LiquidDetectMaxLimit;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_ROTATE_TO_R2:
            *readParam = appParamBoard2Ptr->needleBeadParam.posRotateToR2;
            break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_R2_LIQUID_DETECT_START:
        //     *readParam = appParamBoard2Ptr->needleBeadParam.posUpDownR2LiquidDetectStart;
        //     break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_R2_LIQUID_DETECT_MAX_LIMIT:
            *readParam = appParamBoard2Ptr->needleBeadParam.posUpDownR2LiquidDetectMaxLimit;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_ROTATE_TO_BEAD:
            *readParam = appParamBoard2Ptr->needleBeadParam.posRotateToBead;
            break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_BEAD_LIQUID_DETECT_START:
        //     *readParam = appParamBoard2Ptr->needleBeadParam.posUpDownBeadLiquidDetectStart;
        //     break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_BEAD_LIQUID_DETECT_MAX_LIMIT:
            *readParam = appParamBoard2Ptr->needleBeadParam.posUpDownBeadLiquidDetectMaxLimit;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_ROTATE_TO_PLATE_REACTION:
            *readParam = appParamBoard2Ptr->needleBeadParam.posRotateToPlateReaction;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_INJECT_AT_BOTTOM:
            *readParam = appParamBoard2Ptr->needleBeadParam.posUpDownInject1Shallowest;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_INJECT_CORRECT:
            *readParam = appParamBoard2Ptr->needleBeadParam.posUpDownInject2;
            break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_INJECT3:
        //     *readParam = appParamBoard2Ptr->needleBeadParam.posUpDownInject3;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_INJECT4:
        //     *readParam = appParamBoard2Ptr->needleBeadParam.posUpDownInject4;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_INJECT5:
        //     *readParam = appParamBoard2Ptr->needleBeadParam.posUpDownInject5;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_INJECT6:
        //     *readParam = appParamBoard2Ptr->needleBeadParam.posUpDownInject6;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_INJECT7:
        //     *readParam = appParamBoard2Ptr->needleBeadParam.posUpDownInject7;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_POS_UPDOWN_INJECT8_DEEPEST:
        //     *readParam = appParamBoard2Ptr->needleBeadParam.posUpDownInject8Deepest;
        //     break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_UL_ABSORB_AIR_AFTER_CLEAN:
            *readParam = appParamBoard2Ptr->needleBeadParam.ulAbsorbAirAfterClean;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_UL_ABSORB_AIR_AFTER_LIQUID_ABSORB:
            *readParam = appParamBoard2Ptr->needleBeadParam.ulAbsorbAirAfterLiquidAbsorb;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_UL_RESERVE_AIR_AFTER_INJECT:
            *readParam = appParamBoard2Ptr->needleBeadParam.ulReserveAirAfterInject;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_LIQUID_DETECT_REAL_TIME_COMM:
            *readParam = appParamBoard2Ptr->needleBeadParam.liquidDetectRealTimeComm;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_PUMP_GEAR_GAP:
            *readParam = appParamBoard2Ptr->needleBeadParam.pumpGearGap;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_UPDOWN_LITTLE_BEAD_MIX:
            *readParam = appParamBoard2Ptr->needleBeadParam.posUpDownLittleBeadMix;
            break;   
        case APP_PARAM_SUB_INDEX_NEEDLE_BEAD_LIQUID_BOARD_VERSION_WB:
            *readParam = appParamBoard2Ptr->needleBeadParam.liquidBoardVersionWB;
            break;   
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}



/****************************************试剂盘参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamPlateReagent(uint8_t subIndex,int32_t writeParam)
{
    APP_PARAM_SUB_INDEX_PLATE_REAGENT paramIndex = (APP_PARAM_SUB_INDEX_PLATE_REAGENT)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_SUB_INDEX_PLATE_REAGENT_ROTATE_RESET_CORRECT:
            targetParamPtr = &(appParamBoard2Ptr->plateReagentParam.rotateResetCorrect);
            break;
        case APP_PARAM_SUB_INDEX_PLATE_REAGENT_CONTROL_VERSION:
            targetParamPtr = &(appParamBoard2Ptr->plateReagentParam.controlVersion);
            break;
        case APP_PARAM_SUB_INDEX_PLATE_REAGENT_OFFSET_NEEDLE_REAGENT_WITH_R1:
            targetParamPtr = &(appParamBoard2Ptr->plateReagentParam.offsetNeedleReagentWithR1);
            break;
        case APP_PARAM_SUB_INDEX_PLATE_REAGENT_OFFSET_NEEDLE_REAGENT_WITH_R2:
            targetParamPtr = &(appParamBoard2Ptr->plateReagentParam.offsetNeedleReagentWithR2);
            break;
        case APP_PARAM_SUB_INDEX_PLATE_REAGENT_OFFSET_NEEDLE_BEAD_WITH_R1:
            targetParamPtr = &(appParamBoard2Ptr->plateReagentParam.offsetNeedleBeadWithR1);
            break;
        case APP_PARAM_SUB_INDEX_PLATE_REAGENT_OFFSET_NEEDLE_BEAD_WITH_R2:
            targetParamPtr = &(appParamBoard2Ptr->plateReagentParam.offsetNeedleBeadWithR2);
            break;
        case APP_PARAM_SUB_INDEX_PLATE_REAGENT_OFFSET_NEEDLE_BEAD_WITH_BEAD:
            targetParamPtr = &(appParamBoard2Ptr->plateReagentParam.offsetNeedleBeadWithBead);
            break;
        case APP_PARAM_SUB_INDEX_PLATE_REAGENT_OFFSET_WITH_QR_SCAN:
            targetParamPtr = &(appParamBoard2Ptr->plateReagentParam.offsetWithQRScan);
            break;
        case APP_PARAM_SUB_INDEX_PLATE_REAGENT_OFFSET_WITH_QR_RFID:
            targetParamPtr = &(appParamBoard2Ptr->plateReagentParam.offsetWithQRRFID);
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
LH_ERR AppParamReadParamPlateReagent(uint8_t subIndex,int32_t* readParam)
{
    APP_PARAM_SUB_INDEX_PLATE_REAGENT paramIndex = (APP_PARAM_SUB_INDEX_PLATE_REAGENT)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_SUB_INDEX_PLATE_REAGENT_ROTATE_RESET_CORRECT:
            *readParam = appParamBoard2Ptr->plateReagentParam.rotateResetCorrect;
            break;
        case APP_PARAM_SUB_INDEX_PLATE_REAGENT_CONTROL_VERSION:
            *readParam = appParamBoard2Ptr->plateReagentParam.controlVersion;
            break;
        case APP_PARAM_SUB_INDEX_PLATE_REAGENT_OFFSET_NEEDLE_REAGENT_WITH_R1:
            *readParam = appParamBoard2Ptr->plateReagentParam.offsetNeedleReagentWithR1;
            break;
        case APP_PARAM_SUB_INDEX_PLATE_REAGENT_OFFSET_NEEDLE_REAGENT_WITH_R2:
            *readParam = appParamBoard2Ptr->plateReagentParam.offsetNeedleReagentWithR2;
            break;
        case APP_PARAM_SUB_INDEX_PLATE_REAGENT_OFFSET_NEEDLE_BEAD_WITH_R1:
            *readParam = appParamBoard2Ptr->plateReagentParam.offsetNeedleBeadWithR1;
            break;
        case APP_PARAM_SUB_INDEX_PLATE_REAGENT_OFFSET_NEEDLE_BEAD_WITH_R2:
            *readParam = appParamBoard2Ptr->plateReagentParam.offsetNeedleBeadWithR2;
            break;
        case APP_PARAM_SUB_INDEX_PLATE_REAGENT_OFFSET_NEEDLE_BEAD_WITH_BEAD:
            *readParam = appParamBoard2Ptr->plateReagentParam.offsetNeedleBeadWithBead;
            break;
        case APP_PARAM_SUB_INDEX_PLATE_REAGENT_OFFSET_WITH_QR_SCAN:
            *readParam = appParamBoard2Ptr->plateReagentParam.offsetWithQRScan;
            break;
        case APP_PARAM_SUB_INDEX_PLATE_REAGENT_OFFSET_WITH_QR_RFID:
            *readParam = appParamBoard2Ptr->plateReagentParam.offsetWithQRRFID;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}


/****************************************杯栈管理参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamCupTrayStackManage(uint8_t subIndex,int32_t writeParam)
{
    APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE paramIndex = (APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_NEW_CUP_TRAY_STACK_RESET_CORRECT:
            targetParamPtr = &(appParamBoard2Ptr->cupTrayStackManageParam.newCupTrayStackResetCorrect);
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_EMPTY_CUP_TRAY_STACK_RESET_CORRECT:
            targetParamPtr = &(appParamBoard2Ptr->cupTrayStackManageParam.emptyCupTrayStackResetCorrect);
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_CLAMP_RESET_CORRECT:
            targetParamPtr = &(appParamBoard2Ptr->cupTrayStackManageParam.clampResetCorrect);
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_PUSH_RESET_CORRECT:
            targetParamPtr = &(appParamBoard2Ptr->cupTrayStackManageParam.pushResetCorrect);
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_POS_CLAMP_OPEN:
            targetParamPtr = &(appParamBoard2Ptr->cupTrayStackManageParam.posClampOpen);
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_POS_CLAMP_CLOSE:
            targetParamPtr = &(appParamBoard2Ptr->cupTrayStackManageParam.posClampClose);
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_POS_PUSH_TRAY_TO_TEST:
            targetParamPtr = &(appParamBoard2Ptr->cupTrayStackManageParam.posPushTrayToTest);
            break;
        // case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_OFFSET_PUSH_TRAY_TIGHT_SECOND:
        //     targetParamPtr = &(appParamBoard2Ptr->cupTrayStackManageParam.offsetPushTrayTightSecond);
        //     break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_OFFSET_TOP_TO_SENSOR_WHEN_NEW_CUP_TRAY_FULL:
            targetParamPtr = &(appParamBoard2Ptr->cupTrayStackManageParam.offsetTopToSensorWhenNewCupTrayFull);
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_OFFSET_TOP_TO_PALTFORM_WHEN_NEW_CUP_TRAY_FULL:
            targetParamPtr = &(appParamBoard2Ptr->cupTrayStackManageParam.offsetTopToPaltformWhenNewCupTrayFull);
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_SPACING_LAYER_NEW_CUP_TRAY:
            targetParamPtr = &(appParamBoard2Ptr->cupTrayStackManageParam.spacingLayerNewCupTray);
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_OFFSET_TOP_TO_SENSOR_WHEN_EMPTY_CUP_TRAY_FULL:
            targetParamPtr = &(appParamBoard2Ptr->cupTrayStackManageParam.offsetTopToSensorWhenEmptyCupTrayFull);
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_OFFSET_TOP_TO_PALTFORM_WHEN_EMPTY_CUP_TRAY_FULL:
            targetParamPtr = &(appParamBoard2Ptr->cupTrayStackManageParam.offsetTopToPaltformWhenEmptyCupTrayFull);
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_SPACING_LAYER_EMPTY_CUP_TRAY:
            targetParamPtr = &(appParamBoard2Ptr->cupTrayStackManageParam.spacingLayerEmptyCupTray);
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
LH_ERR AppParamReadParamCupTrayStackManage(uint8_t subIndex,int32_t* readParam)
{
    APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE paramIndex = (APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_NEW_CUP_TRAY_STACK_RESET_CORRECT:
            *readParam = appParamBoard2Ptr->cupTrayStackManageParam.newCupTrayStackResetCorrect;
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_EMPTY_CUP_TRAY_STACK_RESET_CORRECT:
            *readParam = appParamBoard2Ptr->cupTrayStackManageParam.emptyCupTrayStackResetCorrect;
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_CLAMP_RESET_CORRECT:
            *readParam = appParamBoard2Ptr->cupTrayStackManageParam.clampResetCorrect;
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_PUSH_RESET_CORRECT:
            *readParam = appParamBoard2Ptr->cupTrayStackManageParam.pushResetCorrect;
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_POS_CLAMP_OPEN:
            *readParam = appParamBoard2Ptr->cupTrayStackManageParam.posClampOpen;
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_POS_CLAMP_CLOSE:
            *readParam = appParamBoard2Ptr->cupTrayStackManageParam.posClampClose;
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_POS_PUSH_TRAY_TO_TEST:
            *readParam = appParamBoard2Ptr->cupTrayStackManageParam.posPushTrayToTest;
            break;
        // case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_OFFSET_PUSH_TRAY_TIGHT_SECOND:
        //     *readParam = appParamBoard2Ptr->cupTrayStackManageParam.offsetPushTrayTightSecond;
        //     break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_OFFSET_TOP_TO_SENSOR_WHEN_NEW_CUP_TRAY_FULL:
            *readParam = appParamBoard2Ptr->cupTrayStackManageParam.offsetTopToSensorWhenNewCupTrayFull;
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_OFFSET_TOP_TO_PALTFORM_WHEN_NEW_CUP_TRAY_FULL:
            *readParam = appParamBoard2Ptr->cupTrayStackManageParam.offsetTopToPaltformWhenNewCupTrayFull;
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_SPACING_LAYER_NEW_CUP_TRAY:
            *readParam = appParamBoard2Ptr->cupTrayStackManageParam.spacingLayerNewCupTray;
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_OFFSET_TOP_TO_SENSOR_WHEN_EMPTY_CUP_TRAY_FULL:
            *readParam = appParamBoard2Ptr->cupTrayStackManageParam.offsetTopToSensorWhenEmptyCupTrayFull;
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_OFFSET_TOP_TO_PALTFORM_WHEN_EMPTY_CUP_TRAY_FULL:
            *readParam = appParamBoard2Ptr->cupTrayStackManageParam.offsetTopToPaltformWhenEmptyCupTrayFull;
            break;
        case APP_PARAM_SUB_INDEX_CUP_TRAY_STACK_MANAGE_SPACING_LAYER_EMPTY_CUP_TRAY:
            *readParam = appParamBoard2Ptr->cupTrayStackManageParam.spacingLayerEmptyCupTray;
            break; 
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}


/****************************************辅助参数参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamOther(uint8_t subIndex,int32_t writeParam)
{
    APP_PARAM_SUB_INDEX_OTHER paramIndex = (APP_PARAM_SUB_INDEX_OTHER)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_SUB_INDEX_OTHER_UTIL:
            targetParamPtr = &(appParamBoard2Ptr->otherParam.util);
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
LH_ERR AppParamReadParamOther(uint8_t subIndex,int32_t* readParam)
{
    APP_PARAM_SUB_INDEX_OTHER paramIndex = (APP_PARAM_SUB_INDEX_OTHER)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_SUB_INDEX_OTHER_UTIL:
            *readParam = appParamBoard2Ptr->otherParam.util;
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
    AppParamReadParamNeedleBead,
    AppParamReadParamPlateReagent,
    AppParamReadParamCupTrayStackManage,
    AppParamReadParamOther,
};
#define COUNT_APP_UTIL_FUNC_READ_PARAM          (sizeof(AppParamReadParamSingleArray)/sizeof(AppParamReadParamSingleArray[0]))

/****************************************参数写入函数指针数组**************************************************/
static const AppParamWriteParamSingleFuncPtr AppParamWriteParamSingleArray[] = {
    AppParamWriteParamNeedleBead,
    AppParamWriteParamPlateReagent,
    AppParamWriteParamCupTrayStackManage,
    AppParamWriteParamOther,
};
#define COUNT_APP_UTIL_FUNC_WRITE_PARAM         (sizeof(AppParamWriteParamSingleArray)/sizeof(AppParamWriteParamSingleArray[0]))



//系统参数读取
LH_ERR AppParamRead(INDEX_CAN_SUB_BOARD2_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* readValue)
{
    if(mainIndex >= COUNT_APP_UTIL_FUNC_READ_PARAM)
    {
        return LH_ERR_APP_PARAM_READ;
    }
    return AppParamReadParamSingleArray[mainIndex](subIndex,readValue);
}

//系统参数写入
LH_ERR AppParamWrite(INDEX_CAN_SUB_BOARD2_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t writeValue)
{
    if(mainIndex >= COUNT_APP_UTIL_FUNC_WRITE_PARAM)
    {
        return LH_ERR_APP_PARAM_WRITE;
    }
    return AppParamWriteParamSingleArray[mainIndex](subIndex,writeValue);
}
