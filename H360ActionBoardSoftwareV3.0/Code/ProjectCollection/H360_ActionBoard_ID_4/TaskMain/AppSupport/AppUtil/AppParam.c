/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-11 13:55:22
**LastEditors: DengXiaoJun
**LastEditTime: 2020-07-29 09:22:22
**FilePath: \H360_ActionBoard_ID_4\TaskMain\AppSupport\AppUtil\AppParam.c
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

static APP_PARAM_BOARD4* appParamBoard4Ptr = NULL;


void AppParamInit(void)
{
    //首先申请内存
    appParamBoard4Ptr = NULL;
    //申请内存
    do
    {
        appParamBoard4Ptr = UserMemMalloc(SRAM_IS62,LENGTH_APP_PARAM_BOARD4);
        if(appParamBoard4Ptr == NULL)
        {
            CoreDelayMinTick();
        }
    } while (appParamBoard4Ptr == NULL);

    //从Flash中加载参数
    BoardMB85RS2MT_ReadBuffer(ADDR_START_APP_PARAM,(uint8_t*)appParamBoard4Ptr,LENGTH_APP_PARAM_BOARD4);
    //数据校验
    if(appParamBoard4Ptr->paramFlag == APP_PARAM_DEFAULT_FLAG)
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
    appParamBoard4Ptr->paramFlag = APP_PARAM_DEFAULT_FLAG;
    //参数装载
    //试剂混匀参数
    UserMemCopy((void*)(&(appParamBoard4Ptr->mixReagentParam)),(void*)(&appParamMixReagentDefaultSetting),LENGTH_APP_PARAM_MIX_REAGENT);
    //试剂针参数
    UserMemCopy((void*)(&(appParamBoard4Ptr->needleReagentParam)),(void*)(&appParamNeedleReagentDefaultSetting),LENGTH_APP_PARAM_NEEDLE_REAGENT);
    //磁珠混匀参数
    UserMemCopy((void*)(&(appParamBoard4Ptr->mixBeadParam)),(void*)(&appParamMixBeadDefaultSetting),LENGTH_APP_PARAM_MIX_BEAD);
    //反应盘参数
    UserMemCopy((void*)(&(appParamBoard4Ptr->plateReactionParam)),(void*)(&appParamPlateReactionDefaultSetting),LENGTH_APP_PARAM_PLATE_REACTION);
    //辅助参数
    UserMemCopy((void*)(&(appParamBoard4Ptr->otherParam)),(void*)(&appParamOtherDefaultSetting),LENGTH_APP_PARAM_OTHER);
    //将设定好的值写回外置Flash
    BoardMB85RS2MT_WriteBuffer(ADDR_START_APP_PARAM,(uint8_t*)appParamBoard4Ptr,LENGTH_APP_PARAM_BOARD4);

    return LH_ERR_NONE;
}

//通用的写入Flash的函数
static void AppParamWriteSingleCommon(int32_t* targetParamPtr,int32_t writeParam)
{
    *targetParamPtr = writeParam;
    int32_t targetAddr = (int32_t)((char*)targetParamPtr);
    int32_t baseAddr = (int32_t)((char*)(appParamBoard4Ptr));
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

/****************************************试剂混匀参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamMixReagent(uint8_t subIndex,int32_t writeParam)
{
    APP_PARAM_SUB_INDEX_MIX_REAGENT paramIndex = (APP_PARAM_SUB_INDEX_MIX_REAGENT)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_RESET_CORRECT_UPDOWN:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.resetCorrectUpDown);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_RESET_CORRECT_ROTATE:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.resetCorrectRotate);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE1:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.upPosMode1);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE1:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode1);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE2:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.upPosMode2);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE2:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode2);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE3:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.upPosMode3);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE3:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode3);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE4:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.upPosMode4);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE4:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode4);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE5:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.upPosMode5);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE5:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode5);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE6:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.upPosMode6);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE6:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode6);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE7:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.upPosMode7);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE7:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode7);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE8:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.upPosMode8);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE8:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode8);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE9:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.upPosMode9);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE9:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode9);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE10:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.upPosMode10);
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE10:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode10);
            break; 
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_MOTOR_CONFIG_VERSION:
            targetParamPtr = &(appParamBoard4Ptr->mixReagentParam.motorConfigVersion);
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
LH_ERR AppParamReadParamMixReagent(uint8_t subIndex,int32_t* readParam)
{
    APP_PARAM_SUB_INDEX_MIX_REAGENT paramIndex = (APP_PARAM_SUB_INDEX_MIX_REAGENT)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_RESET_CORRECT_UPDOWN:
            *readParam = appParamBoard4Ptr->mixReagentParam.resetCorrectUpDown;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_RESET_CORRECT_ROTATE:
            *readParam = appParamBoard4Ptr->mixReagentParam.resetCorrectRotate;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE1:
            *readParam = appParamBoard4Ptr->mixReagentParam.upPosMode1;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE1:
            *readParam = appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode1;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE2:
            *readParam = appParamBoard4Ptr->mixReagentParam.upPosMode2;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE2:
            *readParam = appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode2;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE3:
            *readParam = appParamBoard4Ptr->mixReagentParam.upPosMode3;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE3:
            *readParam = appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode3;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE4:
            *readParam = appParamBoard4Ptr->mixReagentParam.upPosMode4;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE4:
            *readParam = appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode4;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE5:
            *readParam = appParamBoard4Ptr->mixReagentParam.upPosMode5;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE5:
            *readParam = appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode5;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE6:
            *readParam = appParamBoard4Ptr->mixReagentParam.upPosMode6;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE6:
            *readParam = appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode6;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE7:
            *readParam = appParamBoard4Ptr->mixReagentParam.upPosMode7;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE7:
            *readParam = appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode7;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE8:
            *readParam = appParamBoard4Ptr->mixReagentParam.upPosMode8;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE8:
            *readParam = appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode8;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE9:
            *readParam = appParamBoard4Ptr->mixReagentParam.upPosMode9;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE9:
            *readParam = appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode9;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_UP_POS_MODE10:
            *readParam = appParamBoard4Ptr->mixReagentParam.upPosMode10;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_ROTATE_TIME_MS_MODE10:
            *readParam = appParamBoard4Ptr->mixReagentParam.rotateTimeMsMode10;
            break;
        case APP_PARAM_SUB_INDEX_MIX_REAGENT_MOTOR_CONFIG_VERSION:
            *readParam = appParamBoard4Ptr->mixReagentParam.motorConfigVersion;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}

/****************************************试剂针参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamNeedleReagent(uint8_t subIndex,int32_t writeParam)
{
    APP_PARAM_SUB_INDEX_NEEDLE_REAGENT paramIndex = (APP_PARAM_SUB_INDEX_NEEDLE_REAGENT)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_ROTATE_RESET_CORRECT:
            targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.rotateResetCorrect);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_UPDOWN_RESET_CORRECT:
            targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.upDownResetCorrect);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_ROTATE_TO_CLEAN:
            targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.posRotateToClean);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_CLEAN:
            targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.posUpDownClean);
            break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_TIME_MS_CLEAN_SET1:
        //     targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.timeMsCleanSet1);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_TIME_MS_CLEAN_SET2:
        //     targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.timeMsCleanSet2);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_TIME_MS_CLEAN_SET3:
        //     targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.timeMsCleanSet3);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_TIME_MS_CLEAN_SET4:
        //     targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.timeMsCleanSet4);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_TIME_MS_CLEAN_SET5:
        //     targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.timeMsCleanSet5);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_TIME_MS_PRIME:
        //     targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.timeMsPrime);
        //     break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_ROTATE_TO_R1:
            targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.posRotateToR1);
            break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_R1_LIQUID_DETECT_START:
        //     targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.posUpDownR1LiquidDetectStart);
        //     break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_R1_LIQUID_DETECT_MAX_LIMIT:
            targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.posUpDownR1LiquidDetectMaxLimit);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_ROTATE_TO_R2:
            targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.posRotateToR2);
            break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_R2_LIQUID_DETECT_START:
        //     targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.posUpDownR2LiquidDetectStart);
        //     break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_R2_LIQUID_DETECT_MAX_LIMIT:
            targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.posUpDownR2LiquidDetectMaxLimit);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_ROTATE_TO_PLATE_REACTION:
            targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.posRotateToPlateReaction);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_INJECT_AT_BOTTOM:
            targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.posUpDownInject1Shallowest);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_INJECT_CORRECT:
            targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.posUpDownInject2);
            break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_INJECT3:
        //     targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.posUpDownInject3);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_INJECT4:
        //     targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.posUpDownInject4);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_INJECT5:
        //     targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.posUpDownInject5);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_INJECT6:
        //     targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.posUpDownInject6);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_INJECT7:
        //     targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.posUpDownInject7);
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_INJECT8_DEEPEST:
        //     targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.posUpDownInject8Deepest);
        //     break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_UL_ABSORB_AIR_AFTER_CLEAN:
            targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.ulAbsorbAirAfterClean);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_UL_ABSORB_AIR_AFTER_LIQUID_ABSORB:
            targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.ulAbsorbAirAfterLiquidAbsorb);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_UL_RESERVE_AIR_AFTER_INJECT:
            targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.ulReserveAirAfterInject);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_LIQUID_DETECT_REAL_TIME_COMM:
            targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.liquidDetectRealTimeComm);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_PUMP_GEAR_GAP:
            targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.pumpGearGap);
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_LIQUID_BOARD_VERSION_WB:
            targetParamPtr = &(appParamBoard4Ptr->needleReagentParam.liquidBoardVersionWB);
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
LH_ERR AppParamReadParamNeedleReagent(uint8_t subIndex,int32_t* readParam)
{
    APP_PARAM_SUB_INDEX_NEEDLE_REAGENT paramIndex = (APP_PARAM_SUB_INDEX_NEEDLE_REAGENT)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_ROTATE_RESET_CORRECT:
            *readParam = appParamBoard4Ptr->needleReagentParam.rotateResetCorrect;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_UPDOWN_RESET_CORRECT:
            *readParam = appParamBoard4Ptr->needleReagentParam.upDownResetCorrect;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_ROTATE_TO_CLEAN:
            *readParam = appParamBoard4Ptr->needleReagentParam.posRotateToClean;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_CLEAN:
            *readParam = appParamBoard4Ptr->needleReagentParam.posUpDownClean;
            break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_TIME_MS_CLEAN_SET1:
        //     *readParam = appParamBoard4Ptr->needleReagentParam.timeMsCleanSet1;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_TIME_MS_CLEAN_SET2:
        //     *readParam = appParamBoard4Ptr->needleReagentParam.timeMsCleanSet2;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_TIME_MS_CLEAN_SET3:
        //     *readParam = appParamBoard4Ptr->needleReagentParam.timeMsCleanSet3;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_TIME_MS_CLEAN_SET4:
        //     *readParam = appParamBoard4Ptr->needleReagentParam.timeMsCleanSet4;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_TIME_MS_CLEAN_SET5:
        //     *readParam = appParamBoard4Ptr->needleReagentParam.timeMsCleanSet5;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_TIME_MS_PRIME:
        //     *readParam = appParamBoard4Ptr->needleReagentParam.timeMsPrime;
        //     break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_ROTATE_TO_R1:
            *readParam = appParamBoard4Ptr->needleReagentParam.posRotateToR1;
            break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_R1_LIQUID_DETECT_START:
        //     *readParam = appParamBoard4Ptr->needleReagentParam.posUpDownR1LiquidDetectStart;
        //     break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_R1_LIQUID_DETECT_MAX_LIMIT:
            *readParam = appParamBoard4Ptr->needleReagentParam.posUpDownR1LiquidDetectMaxLimit;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_ROTATE_TO_R2:
            *readParam = appParamBoard4Ptr->needleReagentParam.posRotateToR2;
            break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_R2_LIQUID_DETECT_START:
        //     *readParam = appParamBoard4Ptr->needleReagentParam.posUpDownR2LiquidDetectStart;
        //     break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_R2_LIQUID_DETECT_MAX_LIMIT:
            *readParam = appParamBoard4Ptr->needleReagentParam.posUpDownR2LiquidDetectMaxLimit;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_ROTATE_TO_PLATE_REACTION:
            *readParam = appParamBoard4Ptr->needleReagentParam.posRotateToPlateReaction;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_INJECT_AT_BOTTOM:
            *readParam = appParamBoard4Ptr->needleReagentParam.posUpDownInject1Shallowest;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_INJECT_CORRECT:
            *readParam = appParamBoard4Ptr->needleReagentParam.posUpDownInject2;
            break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_INJECT3:
        //     *readParam = appParamBoard4Ptr->needleReagentParam.posUpDownInject3;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_INJECT4:
        //     *readParam = appParamBoard4Ptr->needleReagentParam.posUpDownInject4;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_INJECT5:
        //     *readParam = appParamBoard4Ptr->needleReagentParam.posUpDownInject5;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_INJECT6:
        //     *readParam = appParamBoard4Ptr->needleReagentParam.posUpDownInject6;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_INJECT7:
        //     *readParam = appParamBoard4Ptr->needleReagentParam.posUpDownInject7;
        //     break;
        // case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_POS_UPDOWN_INJECT8_DEEPEST:
        //     *readParam = appParamBoard4Ptr->needleReagentParam.posUpDownInject8Deepest;
        //     break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_UL_ABSORB_AIR_AFTER_CLEAN:
            *readParam = appParamBoard4Ptr->needleReagentParam.ulAbsorbAirAfterClean;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_UL_ABSORB_AIR_AFTER_LIQUID_ABSORB:
            *readParam = appParamBoard4Ptr->needleReagentParam.ulAbsorbAirAfterLiquidAbsorb;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_UL_RESERVE_AIR_AFTER_INJECT:
            *readParam = appParamBoard4Ptr->needleReagentParam.ulReserveAirAfterInject;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_LIQUID_DETECT_REAL_TIME_COMM:
            *readParam = appParamBoard4Ptr->needleReagentParam.liquidDetectRealTimeComm;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_PUMP_GEAR_GAP:
            *readParam = appParamBoard4Ptr->needleReagentParam.pumpGearGap;
            break;
        case APP_PARAM_SUB_INDEX_NEEDLE_REAGENT_LIQUID_BOARD_VERSION_WB:
            *readParam = appParamBoard4Ptr->needleReagentParam.liquidBoardVersionWB;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}

/****************************************磁珠混匀参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamMixBead(uint8_t subIndex,int32_t writeParam)
{
    APP_PARAM_SUB_INDEX_MIX_BEAD paramIndex = (APP_PARAM_SUB_INDEX_MIX_BEAD)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_SUB_INDEX_MIX_BEAD_RESET_CORRECT_UPDOWN:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.resetCorrectUpDown);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_RESET_CORRECT_ROTATE:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.resetCorrectRotate);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE1:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.upPosMode1);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE1:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode1);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE2:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.upPosMode2);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE2:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode2);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE3:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.upPosMode3);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE3:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode3);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE4:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.upPosMode4);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE4:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode4);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE5:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.upPosMode5);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE5:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode5);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE6:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.upPosMode6);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE6:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode6);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE7:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.upPosMode7);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE7:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode7);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE8:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.upPosMode8);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE8:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode8);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE9:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.upPosMode9);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE9:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode9);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE10:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.upPosMode10);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE10:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode10);
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_MOTOR_CONFIG_VERSION:
            targetParamPtr = &(appParamBoard4Ptr->mixBeadParam.motorConfigVersion);
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
LH_ERR AppParamReadParamMixBead(uint8_t subIndex,int32_t* readParam)
{
    APP_PARAM_SUB_INDEX_MIX_BEAD paramIndex = (APP_PARAM_SUB_INDEX_MIX_BEAD)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_SUB_INDEX_MIX_BEAD_RESET_CORRECT_UPDOWN:
            *readParam = appParamBoard4Ptr->mixBeadParam.resetCorrectUpDown;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_RESET_CORRECT_ROTATE:
            *readParam = appParamBoard4Ptr->mixBeadParam.resetCorrectRotate;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE1:
            *readParam = appParamBoard4Ptr->mixBeadParam.upPosMode1;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE1:
            *readParam = appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode1;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE2:
            *readParam = appParamBoard4Ptr->mixBeadParam.upPosMode2;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE2:
            *readParam = appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode2;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE3:
            *readParam = appParamBoard4Ptr->mixBeadParam.upPosMode3;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE3:
            *readParam = appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode3;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE4:
            *readParam = appParamBoard4Ptr->mixBeadParam.upPosMode4;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE4:
            *readParam = appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode4;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE5:
            *readParam = appParamBoard4Ptr->mixBeadParam.upPosMode5;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE5:
            *readParam = appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode5;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE6:
            *readParam = appParamBoard4Ptr->mixBeadParam.upPosMode6;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE6:
            *readParam = appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode6;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE7:
            *readParam = appParamBoard4Ptr->mixBeadParam.upPosMode7;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE7:
            *readParam = appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode7;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE8:
            *readParam = appParamBoard4Ptr->mixBeadParam.upPosMode8;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE8:
            *readParam = appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode8;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE9:
            *readParam = appParamBoard4Ptr->mixBeadParam.upPosMode9;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE9:
            *readParam = appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode9;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_UP_POS_MODE10:
            *readParam = appParamBoard4Ptr->mixBeadParam.upPosMode10;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_ROTATE_TIME_MS_MODE10:
            *readParam = appParamBoard4Ptr->mixBeadParam.rotateTimeMsMode10;
            break;
        case APP_PARAM_SUB_INDEX_MIX_BEAD_MOTOR_CONFIG_VERSION:
            *readParam = appParamBoard4Ptr->mixBeadParam.motorConfigVersion;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}

/****************************************反应盘参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamPlateReaction(uint8_t subIndex,int32_t writeParam)
{
    APP_PARAM_SUB_INDEX_PLATE_REACTION paramIndex = (APP_PARAM_SUB_INDEX_PLATE_REACTION)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_SUB_INDEX_PLATE_REACTION_ROTATE_RESET_CORRECT:
            targetParamPtr = &(appParamBoard4Ptr->plateReactionParam.rotateResetCorrect);
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
LH_ERR AppParamReadParamPlateReaction(uint8_t subIndex,int32_t* readParam)
{
    APP_PARAM_SUB_INDEX_PLATE_REACTION paramIndex = (APP_PARAM_SUB_INDEX_PLATE_REACTION)subIndex;
    switch(paramIndex)
    {
        case APP_PARAM_SUB_INDEX_PLATE_REACTION_ROTATE_RESET_CORRECT:
            *readParam = appParamBoard4Ptr->plateReactionParam.rotateResetCorrect;
            break;
        default:
            *readParam = 0;
            return LH_ERR_APP_PARAM_READ;//不支持的参数
    }
    //返回结果
    return LH_ERR_NONE;
}

/****************************************辅助参数读写**************************************************/
//写入参数
LH_ERR AppParamWriteParamOther(uint8_t subIndex,int32_t writeParam)
{
    APP_PARAM_SUB_INDEX_OTHER paramIndex = (APP_PARAM_SUB_INDEX_OTHER)subIndex;
    int32_t* targetParamPtr = NULL;
    switch(paramIndex)
    {
        case APP_PARAM_SUB_INDEX_OTHER_UTIL:
            targetParamPtr = &(appParamBoard4Ptr->otherParam.util);
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
            *readParam = appParamBoard4Ptr->otherParam.util;
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
    AppParamReadParamMixReagent,
    AppParamReadParamNeedleReagent,
    AppParamReadParamMixBead,
    AppParamReadParamPlateReaction,
    AppParamReadParamOther,
};
#define COUNT_APP_UTIL_FUNC_READ_PARAM          (sizeof(AppParamReadParamSingleArray)/sizeof(AppParamReadParamSingleArray[0]))

/****************************************参数写入函数指针数组**************************************************/
static const AppParamWriteParamSingleFuncPtr AppParamWriteParamSingleArray[] = {
    AppParamWriteParamMixReagent,
    AppParamWriteParamNeedleReagent,
    AppParamWriteParamMixBead,
    AppParamWriteParamPlateReaction,
    AppParamWriteParamOther,
};
#define COUNT_APP_UTIL_FUNC_WRITE_PARAM         (sizeof(AppParamWriteParamSingleArray)/sizeof(AppParamWriteParamSingleArray[0]))




//系统参数读取
LH_ERR AppParamRead(INDEX_CAN_SUB_BOARD4_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t* readValue)
{
    if(mainIndex >= COUNT_APP_UTIL_FUNC_READ_PARAM)
    {
        return LH_ERR_APP_PARAM_READ;
    }
    return AppParamReadParamSingleArray[mainIndex](subIndex,readValue);
}

//系统参数写入
LH_ERR AppParamWrite(INDEX_CAN_SUB_BOARD4_PARAM_MAIN mainIndex,uint8_t subIndex,int32_t writeValue)
{
    if(mainIndex >= COUNT_APP_UTIL_FUNC_WRITE_PARAM)
    {
        return LH_ERR_APP_PARAM_WRITE;
    }
    return AppParamWriteParamSingleArray[mainIndex](subIndex,writeValue);
}
