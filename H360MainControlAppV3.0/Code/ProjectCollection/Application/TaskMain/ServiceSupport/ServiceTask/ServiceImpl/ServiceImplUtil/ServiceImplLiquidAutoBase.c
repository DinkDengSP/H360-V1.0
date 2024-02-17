/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-04-24 13:51:33
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 13:44:08
**FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplUtil\ServiceImplLiquidAutoBase.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplLiquidAutoUtil.h"


//模块输入配置映射,前一个是系统序号,后一个是有效电平,旧液路配置
static const MODULE_INPUT_MAP_UNIT moduleLiquidAutoInputMapArrayVersion0[] = {
    {INPUT_LIQUID_AUTO_PURE_WATER_EMPTY_SENSOR                                ,VALID_LEVEL_INPUT_LIQUID_AUTO_PURE_WATER_EMPTY_SENSOR                            },//纯水空
    {INPUT_LIQUID_AUTO_PURE_WATER_FULL_SENSOR                                 ,VALID_LEVEL_INPUT_LIQUID_AUTO_PURE_WATER_FULL_SENSOR                             },//纯水满
    {INPUT_LIQUID_AUTO_LIQUID_WASTE_FULL_SENSOR                               ,VALID_LEVEL_INPUT_LIQUID_AUTO_LIQUID_WASTE_FULL_SENSOR                           },//废液桶满
    {INPUT_LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_A_LEVEL_EMPTY_VERSION_0       ,VALID_LEVEL_INPUT_LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_A_LEVEL_EMPTY_VERSION_0   },//三针浓缩液A瓶空
    {INPUT_LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_B_LEVEL_EMPTY_VERSION_0       ,VALID_LEVEL_INPUT_LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_B_LEVEL_EMPTY_VERSION_0   },//三针浓缩液B瓶空
    {INPUT_LIQUID_AUTO_THREE_NEEDLE_TRANSFER_DILUENT_LEVEL_EMPTY_VERSION_0    ,VALID_LEVEL_INPUT_LIQUID_AUTO_THREE_NEEDLE_TRANSFER_DILUENT_LEVEL_EMPTY_VERSION_0},//三针稀释液中转瓶空
    {INPUT_LIQUID_AUTO_THREE_NEEDLE_TRANSFER_DILUENT_LEVEL_FULL_VERSION_0     ,VALID_LEVEL_INPUT_LIQUID_AUTO_THREE_NEEDLE_TRANSFER_DILUENT_LEVEL_FULL_VERSION_0 },//三针稀释液中转瓶满
    {INPUT_LIQUID_AUTO_THREE_NEEDLE_TARGET_DILUENT_LEVEL_EMPTY_VERSION_0      ,VALID_LEVEL_INPUT_LIQUID_AUTO_THREE_NEEDLE_TARGET_DILUENT_LEVEL_EMPTY_VERSION_0  },//三针稀释液目标瓶空
    {INPUT_LIQUID_AUTO_THREE_NEEDLE_TARGET_DILUENT_LEVEL_FULL_VERSION_0       ,VALID_LEVEL_INPUT_LIQUID_AUTO_THREE_NEEDLE_TARGET_DILUENT_LEVEL_FULL_VERSION_0   },//三针稀释液目标瓶满
};

//模块输入配置映射,前一个是系统序号,后一个是有效电平,新液路配置
static const MODULE_INPUT_MAP_UNIT moduleLiquidAutoInputMapArrayVersion1[] = {
    {INPUT_LIQUID_AUTO_PURE_WATER_EMPTY_SENSOR                                ,VALID_LEVEL_INPUT_LIQUID_AUTO_PURE_WATER_EMPTY_SENSOR                            },//纯水空
    {INPUT_LIQUID_AUTO_PURE_WATER_FULL_SENSOR                                 ,VALID_LEVEL_INPUT_LIQUID_AUTO_PURE_WATER_FULL_SENSOR                             },//纯水满
    {INPUT_LIQUID_AUTO_LIQUID_WASTE_FULL_SENSOR                               ,VALID_LEVEL_INPUT_LIQUID_AUTO_LIQUID_WASTE_FULL_SENSOR                           },//废液桶满
    {INPUT_LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_A_LEVEL_EMPTY_VERSION_1       ,VALID_LEVEL_INPUT_LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_A_LEVEL_EMPTY_VERSION_1   },//三针浓缩液A瓶空
    {INPUT_LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_B_LEVEL_EMPTY_VERSION_1       ,VALID_LEVEL_INPUT_LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_B_LEVEL_EMPTY_VERSION_1   },//三针浓缩液B瓶空
    {INPUT_LIQUID_AUTO_THREE_NEEDLE_TRANSFER_DILUENT_LEVEL_EMPTY_VERSION_1    ,VALID_LEVEL_INPUT_LIQUID_AUTO_THREE_NEEDLE_TRANSFER_DILUENT_LEVEL_EMPTY_VERSION_1},//三针稀释液中转瓶空
    {INPUT_LIQUID_AUTO_THREE_NEEDLE_TRANSFER_DILUENT_LEVEL_FULL_VERSION_1     ,VALID_LEVEL_INPUT_LIQUID_AUTO_THREE_NEEDLE_TRANSFER_DILUENT_LEVEL_FULL_VERSION_1 },//三针稀释液中转瓶满
    {INPUT_LIQUID_AUTO_THREE_NEEDLE_TARGET_DILUENT_LEVEL_EMPTY_VERSION_1      ,VALID_LEVEL_INPUT_LIQUID_AUTO_THREE_NEEDLE_TARGET_DILUENT_LEVEL_EMPTY_VERSION_1  },//三针稀释液目标瓶空
    {INPUT_LIQUID_AUTO_THREE_NEEDLE_TARGET_DILUENT_LEVEL_FULL_VERSION_1       ,VALID_LEVEL_INPUT_LIQUID_AUTO_THREE_NEEDLE_TARGET_DILUENT_LEVEL_FULL_VERSION_1   },//三针稀释液目标瓶满
};

//配置数组长度
#define MODULE_LIQUID_AUTO_INPUT_MAP_ARRAY_LENGTH_VERSION_0   (sizeof(moduleLiquidAutoInputMapArrayVersion0)/sizeof(moduleLiquidAutoInputMapArrayVersion0[0]))
#define MODULE_LIQUID_AUTO_INPUT_MAP_ARRAY_LENGTH_VERSION_1   (sizeof(moduleLiquidAutoInputMapArrayVersion1)/sizeof(moduleLiquidAutoInputMapArrayVersion1[0]))
/*******************************************************************
**description: 读取指定输入 
******************************************************************/
LH_ERR ServiceImplLiquidAuto_ModuleInputRead(INPUT_LIQUID_AUTO_NAME name,SENSOR_STATE* state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //先读取液路版本讯息
        int32_t liquidControlVersion = 0;
        ServiceParamReadSingle(INDEX_MAIN_SERVICE_PARAM_LIQUID_AUTO,INDEX_SUB_SERVICE_PARAM_LIQUID_AUTO_LIQUID_CONTROL_VERSION,&liquidControlVersion);
        if(liquidControlVersion >1)liquidControlVersion = 1;
        if(liquidControlVersion <0)liquidControlVersion = 0;
    //输入读取,分版本读取
        if(liquidControlVersion == 0)
        {
            errorCode = ServiceImplModuleInputRead((uint8_t)name,MODULE_LIQUID_AUTO_INPUT_MAP_ARRAY_LENGTH_VERSION_0,(MODULE_INPUT_MAP_UNIT*)moduleLiquidAutoInputMapArrayVersion0,state);
        }
        else
        {
            errorCode = ServiceImplModuleInputRead((uint8_t)name,MODULE_LIQUID_AUTO_INPUT_MAP_ARRAY_LENGTH_VERSION_1,(MODULE_INPUT_MAP_UNIT*)moduleLiquidAutoInputMapArrayVersion1,state);
        }
        if(errorCode != LH_ERR_NONE)
        {
            if(errorCode == LH_ERR_BOARD_MAIN_COMM_INPUT_PIN_INDEX)
            {
                *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_INPUT_INDEX;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
            else
            {
                *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_INPUT_READ;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
        }
    return errorCode;
}


//模块输出配置映射
static const MODULE_OUTPUT_MAP_UNIT moduleLiquidAutoOutputMapArray[] = {
    {OUTPUT_LIQUID_AUTO_PURE_WATER_V241                            ,VALID_LEVEL_OUTPUT_LIQUID_AUTO_PURE_WATER_V241                        },//纯水进水阀
    {OUTOUT_LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_A_LED_EMPTY       ,VALID_LEVEL_OUTOUT_LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_A_LED_EMPTY   },//浓缩液A空指示灯
    {OUTOUT_LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_B_LED_EMPTY       ,VALID_LEVEL_OUTOUT_LIQUID_AUTO_THREE_NEEDLE_CONCENTRATE_B_LED_EMPTY   },//浓缩液B空指示灯
    {OUTPUT_LIQUID_AUTO_PURE_WATER_DP3                             ,VALID_LEVEL_OUTPUT_LIQUID_AUTO_PURE_WATER_DP3                         },//纯水进水泵
};
//配置数组长度
#define MODULE_LIQUID_AUTO_OUTPUT_MAP_UNIT_LENGTH   (sizeof(moduleLiquidAutoOutputMapArray)/sizeof(moduleLiquidAutoOutputMapArray[0]))
/*******************************************************************
**description: 写入指定输出 
******************************************************************/
LH_ERR ServiceImplLiquidAuto_ModuleOutputWrite(OUTPUT_LIQUID_AUTO_NAME name,OUT_STATE state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //输出写入
        errorCode = ServiceImplModuleOutputWrite((uint8_t)name,state,MODULE_LIQUID_AUTO_OUTPUT_MAP_UNIT_LENGTH,(MODULE_OUTPUT_MAP_UNIT*)moduleLiquidAutoOutputMapArray);
        if(errorCode != LH_ERR_NONE)
        {
            if(errorCode == LH_ERR_BOARD_MAIN_COMM_OUTPUT_PIN_INDEX)
            {
                *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_OUTPUT_INDEX;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
            else
            {
                *commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_OUTPUT_WRITE;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
            return errorCode;
        }
    return LH_ERR_NONE;
}

