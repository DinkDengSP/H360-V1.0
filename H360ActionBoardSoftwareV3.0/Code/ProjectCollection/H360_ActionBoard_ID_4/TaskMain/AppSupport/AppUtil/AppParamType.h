/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:55:22
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-07-29 09:21:31
 *FilePath: \H360_ActionBoard_ID_4\TaskMain\AppSupport\AppUtil\AppParamType.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_PARAM_TYPE_H_
#define __APP_PARAM_TYPE_H_
#include "stm32f4xx.h"

//试剂摇匀参数
typedef struct APP_PARAM_MIX_REAGENT
{
    int32_t resetCorrectUpDown;//试剂摇匀升降零位修正
    int32_t resetCorrectRotate;//试剂摇匀旋转零位修正
    int32_t upPosMode1;//试剂摇匀模式1升降高度
    int32_t rotateTimeMsMode1;//试剂摇匀模式1摇匀时间,毫秒
    int32_t upPosMode2;//试剂摇匀模式2升降高度
    int32_t rotateTimeMsMode2;//试剂摇匀模式2摇匀时间,毫秒
    int32_t upPosMode3;//试剂摇匀模式2升降高度
    int32_t rotateTimeMsMode3;//试剂摇匀模式3摇匀时间,毫秒
    int32_t upPosMode4;//试剂摇匀模式2升降高度
    int32_t rotateTimeMsMode4;//试剂摇匀模式4摇匀时间,毫秒
    int32_t upPosMode5;//试剂摇匀模式2升降高度
    int32_t rotateTimeMsMode5;//试剂摇匀模式5摇匀时间,毫秒
    int32_t upPosMode6;//试剂摇匀模式2升降高度
    int32_t rotateTimeMsMode6;//试剂摇匀模式6摇匀时间,毫秒
    int32_t upPosMode7;//试剂摇匀模式2升降高度
    int32_t rotateTimeMsMode7;//试剂摇匀模式7摇匀时间,毫秒
    int32_t upPosMode8;//试剂摇匀模式2升降高度
    int32_t rotateTimeMsMode8;//试剂摇匀模式8摇匀时间,毫秒
    int32_t upPosMode9;//试剂摇匀模式2升降高度
    int32_t rotateTimeMsMode9;//试剂摇匀模式9摇匀时间,毫秒
    int32_t upPosMode10;//试剂摇匀模式2升降高度
    int32_t rotateTimeMsMode10;//试剂摇匀模式10摇匀时间,毫秒
    int32_t motorConfigVersion;//试剂混匀电机配置
    int32_t reserveArray[49];//保留的49个参数
}APP_PARAM_MIX_REAGENT;
//参数长度
#define LENGTH_APP_PARAM_MIX_REAGENT     (sizeof(APP_PARAM_MIX_REAGENT)/sizeof(uint8_t))


//试剂针参数
typedef struct APP_PARAM_NEEDLE_REAGENT
{
    int32_t rotateResetCorrect;//试剂针旋转零位修正
    int32_t upDownResetCorrect;//试剂针升降零位修正
    int32_t posRotateToClean;//试剂针旋转到清洗位坐标
    int32_t posUpDownClean;//试剂针升降到清洗位坐标
    int32_t timeMsCleanSet1;//试剂针清洗时间1,毫秒
    int32_t timeMsCleanSet2;//试剂针清洗时间2,毫秒
    int32_t timeMsCleanSet3;//试剂针清洗时间3,毫秒
    int32_t timeMsCleanSet4;//试剂针清洗时间4,毫秒
    int32_t timeMsCleanSet5;//试剂针清洗时间5,毫秒
    int32_t timeMsPrime;//试剂针清洗液灌注时间,毫秒
    int32_t posRotateToR1;//试剂针旋转到R1坐标
    int32_t posUpDownR1LiquidDetectStart;//试剂针升降R1吸样探液起始点
    int32_t posUpDownR1LiquidDetectMaxLimit;//试剂针升降R1吸样探液极限点
    int32_t posRotateToR2;//试剂针旋转到R2坐标
    int32_t posUpDownR2LiquidDetectStart;//试剂针升降R2吸样探液起始点
    int32_t posUpDownR2LiquidDetectMaxLimit;//试剂针升降R2吸样探液极限点
    int32_t posRotateToPlateReaction;//试剂针旋转到反应盘排样坐标
    int32_t posUpDownInject1Shallowest;//试剂针升降反应盘排样高度1,最浅
    int32_t posUpDownInject2;//试剂针升降反应盘排样高度2
    int32_t posUpDownInject3;//试剂针升降反应盘排样高度3
    int32_t posUpDownInject4;//试剂针升降反应盘排样高度4
    int32_t posUpDownInject5;//试剂针升降反应盘排样高度5
    int32_t posUpDownInject6;//试剂针升降反应盘排样高度6
    int32_t posUpDownInject7;//试剂针升降反应盘排样高度7
    int32_t posUpDownInject8Deepest;//试剂针升降反应盘排样高度8,最深
    int32_t ulAbsorbAirAfterClean;//试剂针清洗完吸空气量
    int32_t ulAbsorbAirAfterLiquidAbsorb;//试剂针吸样完吸空气量
    int32_t ulReserveAirAfterInject;//试剂针注液保留空气量
    int32_t liquidDetectRealTimeComm;//试剂针探液通讯使能
    int32_t pumpGearGap;//试剂针齿间差
    int32_t liquidBoardVersionWB;//探液板版本号回填确认
    int32_t reserveArray[48];//保留的48个参数
}APP_PARAM_NEEDLE_REAGENT;
//参数长度
#define LENGTH_APP_PARAM_NEEDLE_REAGENT     (sizeof(APP_PARAM_NEEDLE_REAGENT)/sizeof(uint8_t))


//磁珠摇匀参数
typedef struct APP_PARAM_MIX_BEAD
{
    int32_t resetCorrectUpDown;//磁珠摇匀升降零位修正
    int32_t resetCorrectRotate;//磁珠摇匀旋转零位修正
    int32_t upPosMode1;//磁珠摇匀模式1升降高度
    int32_t rotateTimeMsMode1;//磁珠摇匀模式1摇匀时间,毫秒
    int32_t upPosMode2;//磁珠摇匀模式2升降高度
    int32_t rotateTimeMsMode2;//磁珠摇匀模式2摇匀时间,毫秒
    int32_t upPosMode3;//磁珠摇匀模式2升降高度
    int32_t rotateTimeMsMode3;//磁珠摇匀模式3摇匀时间,毫秒
    int32_t upPosMode4;//磁珠摇匀模式2升降高度
    int32_t rotateTimeMsMode4;//磁珠摇匀模式4摇匀时间,毫秒
    int32_t upPosMode5;//磁珠摇匀模式2升降高度
    int32_t rotateTimeMsMode5;//磁珠摇匀模式5摇匀时间,毫秒
    int32_t upPosMode6;//磁珠摇匀模式2升降高度
    int32_t rotateTimeMsMode6;//磁珠摇匀模式6摇匀时间,毫秒
    int32_t upPosMode7;//磁珠摇匀模式2升降高度
    int32_t rotateTimeMsMode7;//磁珠摇匀模式7摇匀时间,毫秒
    int32_t upPosMode8;//磁珠摇匀模式2升降高度
    int32_t rotateTimeMsMode8;//磁珠摇匀模式8摇匀时间,毫秒
    int32_t upPosMode9;//磁珠摇匀模式2升降高度
    int32_t rotateTimeMsMode9;//磁珠摇匀模式9摇匀时间,毫秒
    int32_t upPosMode10;//磁珠摇匀模式2升降高度
    int32_t rotateTimeMsMode10;//磁珠摇匀模式10摇匀时间,毫秒
    int32_t motorConfigVersion;//磁珠混匀电机配置
    int32_t reserveArray[49];//保留的49个参数
}APP_PARAM_MIX_BEAD;
//参数长度
#define LENGTH_APP_PARAM_MIX_BEAD     (sizeof(APP_PARAM_MIX_BEAD)/sizeof(uint8_t))

//反应盘
typedef struct APP_PARAM_PLATE_REACTION
{
    int32_t rotateResetCorrect;//旋转零位修正
    int32_t reserveArray[51];//保留的五十个参数
}APP_PARAM_PLATE_REACTION;
//参数长度
#define LENGTH_APP_PARAM_PLATE_REACTION     (sizeof(APP_PARAM_PLATE_REACTION)/sizeof(uint8_t))

//其余支持
typedef struct APP_PARAM_OTHER
{
    int32_t util;//辅助参数
    int32_t reserveArray[50];//保留的五十个参数
}APP_PARAM_OTHER;
//参数长度
#define LENGTH_APP_PARAM_OTHER     (sizeof(APP_PARAM_OTHER)/sizeof(uint8_t))



//应用配置的标志位,初始化检测该标志位,如果不对,需要设置为默认参数
#define APP_PARAM_DEFAULT_FLAG              0XAA5555AA
//总的系统参数
typedef struct APP_PARAM_BOARD4
{
    uint32_t paramFlag;//参数已经配置过标识
    APP_PARAM_MIX_REAGENT mixReagentParam;//试剂混匀参数
    APP_PARAM_NEEDLE_REAGENT needleReagentParam;//试剂针参数
    APP_PARAM_MIX_BEAD mixBeadParam;//磁珠混匀参数
    APP_PARAM_PLATE_REACTION plateReactionParam;//反应盘参数
    APP_PARAM_OTHER otherParam;//辅助参数
}APP_PARAM_BOARD4;
//参数长度
#define LENGTH_APP_PARAM_BOARD4     (sizeof(APP_PARAM_BOARD4)/sizeof(uint8_t))



#endif

