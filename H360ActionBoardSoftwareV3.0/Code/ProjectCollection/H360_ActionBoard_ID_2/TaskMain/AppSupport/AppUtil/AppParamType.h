/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 13:54:56
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-11 18:33:40
 *FilePath: \H360_ActionBoard_ID_2\TaskMain\AppSupport\AppUtil\AppParamType.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_PARAM_TYPE_H_
#define __APP_PARAM_TYPE_H_
#include "stm32f4xx.h"

//磁珠针参数
typedef struct APP_PARAM_NEEDLE_BEAD
{
    int32_t rotateResetCorrect;//磁珠针旋转零位修正
    int32_t upDownResetCorrect;//磁珠针升降零位修正
    int32_t posRotateToClean;//磁珠针旋转到清洗位坐标
    int32_t posUpDownClean;//磁珠针升降到清洗位坐标
    int32_t timeMsCleanSet1;//磁珠针清洗时间1,毫秒
    int32_t timeMsCleanSet2;//磁珠针清洗时间2,毫秒
    int32_t timeMsCleanSet3;//磁珠针清洗时间3,毫秒
    int32_t timeMsCleanSet4;//磁珠针清洗时间4,毫秒
    int32_t timeMsCleanSet5;//磁珠针清洗时间5,毫秒
    int32_t timeMsPrime;//磁珠针清洗液灌注时间,毫秒
    int32_t posRotateToR1;//磁珠针旋转到R1坐标
    int32_t posUpDownR1LiquidDetectStart;//磁珠针升降R1吸样探液起始点
    int32_t posUpDownR1LiquidDetectMaxLimit;//磁珠针升降R1吸样探液极限点
    int32_t posRotateToR2;//磁珠针旋转到R2坐标
    int32_t posUpDownR2LiquidDetectStart;//磁珠针升降R2吸样探液起始点
    int32_t posUpDownR2LiquidDetectMaxLimit;//磁珠针升降R2吸样探液极限点
    int32_t posRotateToBead;//磁珠针旋转到磁珠位坐标
    int32_t posUpDownBeadLiquidDetectStart;//磁珠针升降磁珠位吸样探液起始点
    int32_t posUpDownBeadLiquidDetectMaxLimit;//磁珠针升降磁珠位吸样探液极限点
    int32_t posRotateToPlateReaction;//磁珠针旋转到反应盘排样坐标
    int32_t posUpDownInject1Shallowest;//磁珠针升降反应盘排样高度1,最浅
    int32_t posUpDownInject2;//磁珠针升降反应盘排样高度2
    int32_t posUpDownInject3;//磁珠针升降反应盘排样高度3
    int32_t posUpDownInject4;//磁珠针升降反应盘排样高度4
    int32_t posUpDownInject5;//磁珠针升降反应盘排样高度5
    int32_t posUpDownInject6;//磁珠针升降反应盘排样高度6
    int32_t posUpDownInject7;//磁珠针升降反应盘排样高度7
    int32_t posUpDownInject8Deepest;//磁珠针升降反应盘排样高度8,最深
    int32_t ulAbsorbAirAfterClean;//磁珠针清洗完吸空气量
    int32_t ulAbsorbAirAfterLiquidAbsorb;//磁珠针吸样完吸空气量
    int32_t ulReserveAirAfterInject;//磁珠针注液保留空气量
    int32_t liquidDetectRealTimeComm;//磁珠针探液通讯使能
    int32_t pumpGearGap;//泵齿间差
    int32_t posUpDownLittleBeadMix;//小磁针吸吐混匀高度
    int32_t liquidBoardVersionWB;//探液板版本号回填确认
    int32_t reserveArray[47];//保留的47个参数
}APP_PARAM_NEEDLE_BEAD;
//参数长度
#define LENGTH_APP_PARAM_NEEDLE_BEAD     (sizeof(APP_PARAM_NEEDLE_BEAD)/sizeof(uint8_t))

//试剂盘参数
typedef struct APP_PARAM_PLATE_REAGENT
{
    int32_t rotateResetCorrect;//试剂盘零位修正
    int32_t controlVersion;//试剂盘控制版本
    int32_t offsetNeedleReagentWithR1;//试剂盘相对试剂针到R1偏移
    int32_t offsetNeedleReagentWithR2;//试剂盘相对试剂针到R2偏移
    int32_t offsetNeedleBeadWithR1;//试剂盘相对磁珠针到R1偏移
    int32_t offsetNeedleBeadWithR2;//试剂盘相对磁珠针到R2偏移
    int32_t offsetNeedleBeadWithBead;//试剂盘相对磁珠针到磁珠偏移
    int32_t offsetWithQRScan;//试剂盘到扫码位偏移
    int32_t offsetWithQRRFID;//试剂盘到RFID位偏移
    int32_t reserveArray[49];//保留的五十个参数
}APP_PARAM_PLATE_REAGENT;
//参数长度
#define LENGTH_APP_PARAM_PLATE_REAGENT     (sizeof(APP_PARAM_PLATE_REAGENT)/sizeof(uint8_t))


//杯栈管理参数
typedef struct APP_PARAM_CUP_TRAY_STACK_MANAGE
{
    int32_t newCupTrayStackResetCorrect;//新杯盘升降电机零位修正
    int32_t emptyCupTrayStackResetCorrect;//空杯盘升降电机零位修正
    int32_t clampResetCorrect;//夹手电机零位修正
    int32_t pushResetCorrect;//推杆电机零位修正
    int32_t posClampOpen;//夹手打开坐标
    int32_t posClampClose;//夹手关闭坐标
    int32_t posPushTrayToTest;//推杆推新盘到测试位坐标
    int32_t offsetPushTrayTightSecond;//推杆二次推进补充偏移
    int32_t offsetTopToSensorWhenNewCupTrayFull;//新杯栈满时,顶部到检测位偏移
    int32_t offsetTopToPaltformWhenNewCupTrayFull;//新杯栈满时,顶部到平台偏移
    int32_t spacingLayerNewCupTray;//新杯栈上升一个杯盘的偏移
    int32_t offsetTopToSensorWhenEmptyCupTrayFull;//空杯栈满时,顶部到检测位偏移
    int32_t offsetTopToPaltformWhenEmptyCupTrayFull;//空杯栈满时,顶部到承接平台偏移
    int32_t spacingLayerEmptyCupTray;//空杯栈上升一个杯盘的偏移
    int32_t reserveArray[50];//保留的五十个参数
}APP_PARAM_CUP_TRAY_STACK_MANAGE;
//参数长度
#define LENGTH_APP_PARAM_CUP_TRAY_STACK_MANAGE     (sizeof(APP_PARAM_CUP_TRAY_STACK_MANAGE)/sizeof(uint8_t))


//辅助参数
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
typedef struct APP_PARAM_BOARD2
{
    uint32_t paramFlag;//参数已经配置过标识
    APP_PARAM_NEEDLE_BEAD needleBeadParam;//磁珠针参数
    APP_PARAM_PLATE_REAGENT plateReagentParam;//试剂盘参数
    APP_PARAM_CUP_TRAY_STACK_MANAGE cupTrayStackManageParam;//杯栈管理参数
    APP_PARAM_OTHER otherParam;//辅助参数
}APP_PARAM_BOARD2;
//参数长度
#define LENGTH_APP_PARAM_BOARD2     (sizeof(APP_PARAM_BOARD2)/sizeof(uint8_t))






#endif

