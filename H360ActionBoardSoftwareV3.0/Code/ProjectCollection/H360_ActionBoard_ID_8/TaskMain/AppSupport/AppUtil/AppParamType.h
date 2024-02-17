/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-05-01 14:36:06
 *FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppUtil\AppParamType.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_PARAM_TYPE_H_
#define __APP_PARAM_TYPE_H_
#include "BoardInc.h"

//应用配置的标志位,初始化检测该标志位,如果不对,需要设置为默认参数
#define APP_PARAM_DEFAULT_FLAG              0XAA5555AA

//进样
typedef struct APP_PARAM_PUSHIN
{
    int32_t resetCorrection;//复位的零位修正
    int32_t pushMaxPosition;//进样最大坐标
    //保留参数
    int32_t resever[50];
}APP_PARAM_PUSHIN;

//出样
typedef struct APP_PARAM_PUSHOUT
{
    int32_t resetCorrection;//复位的零位修正
    int32_t pushOneRackOffset;//出样出一次试管架运行的偏移
    //保留参数
    int32_t resever[50];
}APP_PARAM_PUSHOUT;

//轨道独立参数
typedef struct APP_PARAM_RAIL_INDEPENDENT
{
    int32_t resetCorrection;//轨道复位修正坐标
    int32_t emergencyPosition;//急诊位坐标
    int32_t pushIn1Position;//进样1位置坐标
    int32_t pushIn2Position;//进样2位置坐标
    int32_t rackBarScanPosition;//试管架条码扫描位坐标
    int32_t tube1BarScanPosition;//试管1扫描位坐标
    int32_t tube2BarScanPosition;//试管2扫描位坐标
    int32_t tube3BarScanPosition;//试管3扫描位坐标
    int32_t tube4BarScanPosition;//试管4扫描位坐标
    int32_t tube5BarScanPosition;//试管5扫描位坐标
    int32_t rotate1Position;//转向位1坐标
    int32_t rotate2Position;//转向位2坐标
    int32_t pushOut1Position;//出样位1坐标
    int32_t pushOut2Position;//出样位2坐标
    int32_t barcodeScannerType;//扫码枪类型
    //保留参数
    int32_t resever[49];
}APP_PARAM_RAIL_INDEPENDENT;

//轨道模块参数
typedef struct APP_PARAM_RAIL_MODULE
{
    //待测位三个的坐标
    int32_t WaitTest1Position;
    int32_t WaitTest2Position;
    int32_t WaitTest3Position;
    //重测位
    int32_t ReTest1Position;
    int32_t ReTest2Position;
    int32_t ReTest3Position;
    int32_t ReTest4Position;
    int32_t ReTest5Position;
    int32_t ReTest6Position;
    int32_t ReTest7Position;
    int32_t ReTest8Position;
    //正在测试位
    int32_t TestingPutPosition;//放架位
    int32_t TestingCatchPosition;//取架位
    int32_t TestingAbsorbPosition;//吸样位
    int32_t tubeOffsetTesting;//测试区试管偏移
    //保留参数
    int32_t resever[49];
}APP_PARAM_RAIL_MODULE;

//机械手独立参数
typedef struct APP_PARAM_HAND_INDEPENDENT
{
    int32_t handUpDownResetCorrection;//手升降零位偏移
    int32_t handBackFrontResetCorrection;//手前后零位偏移
    int32_t handRotateResetCorrection;//手旋转零位偏移
    int32_t handUpDownHighMaxPosition;//手升降最高位
    int32_t handUpDownLowMaxPosition;//手升降最低位
    int32_t handBackFrontPushIn1Position;//手前后进样1位置坐标
    int32_t handBackFrontPushIn2Position;//手前后进样2位置坐标
    int32_t handBackFrontPushOut1Position;//手前后出样1位置坐标
    int32_t handBackFrontPushOut2Position;//手前后出样2位置坐标
    int32_t handBackFrontEmergencyPosition;//手前后急诊位置坐标
    int32_t handBackFrontUtilRotateOffset;//手前后辅助旋转的偏移
    int32_t handRotateDirPushInPosition;//手旋转到进样的坐标
    int32_t handRotateDirEmergencyPosition;//手旋转到急诊方向的坐标
    //保留参数
    int32_t resever[48];
}APP_PARAM_HAND_INDEPENDENT;

//机械手模块参数
typedef struct APP_PARAM_HAND_MODULE
{
    int32_t handBackFrontWaitTest1Position;//手前后在模块上进样缓冲区1的前后位置
    int32_t handBackFrontWaitTest2Position;//手前后在模块上进样缓冲区2的前后位置
    int32_t handBackFrontWaitTest3Position;//手前后在模块上进样缓冲区3的前后位置
    int32_t handBackFrontReTest1Position;//手前后在模块上重测缓冲区的前后位置
    int32_t handBackFrontReTest2Position;//手前后在模块上重测缓冲区的前后位置
    int32_t handBackFrontReTest3Position;//手前后在模块上重测缓冲区的前后位置
    int32_t handBackFrontReTest4Position;//手前后在模块上重测缓冲区的前后位置
    int32_t handBackFrontReTest5Position;//手前后在模块上重测缓冲区的前后位置
    int32_t handBackFrontReTest6Position;//手前后在模块上重测缓冲区的前后位置
    int32_t handBackFrontReTest7Position;//手前后在模块上重测缓冲区的前后位置
    int32_t handBackFrontReTest8Position;//手前后在模块上重测缓冲区的前后位置
    int32_t handBackFrontTestingPosition;//手前后模块上测试区的前后位置
    //保留参数
    int32_t resever[40];
}APP_PARAM_HAND_MODULE;

//轨道辅助参数
typedef struct APP_PARAM_UTIL
{
    int32_t reserveInit;
    //保留参数
    int32_t resever[50];
}APP_PARAM_UTIL;

//系统参数所有线程的集合
typedef struct APP_PARAM
{
    uint32_t paramFlag;
    APP_PARAM_PUSHIN pushIn1Param;//进样1参数
    APP_PARAM_PUSHIN pushIn2Param;//进样2参数
    APP_PARAM_PUSHOUT pushOut1Param;//出样1参数
    APP_PARAM_PUSHOUT pushOut2Param;//出样2参数
    APP_PARAM_RAIL_INDEPENDENT railIndependentParam;//轨道独立参数
    APP_PARAM_RAIL_MODULE railParamModule1;//轨道在模块1参数
    APP_PARAM_RAIL_MODULE railParamModule2;//轨道在模块2参数
    APP_PARAM_RAIL_MODULE railParamModule3;//轨道在模块3参数
    APP_PARAM_RAIL_MODULE railParamModule4;//轨道在模块4参数
    APP_PARAM_HAND_INDEPENDENT handParamIndependent;//机械手独立参数
    APP_PARAM_HAND_MODULE handParamModule1;//机械手在模块1参数
    APP_PARAM_HAND_MODULE handParamModule2;//机械手在模块2参数
    APP_PARAM_HAND_MODULE handParamModule3;//机械手在模块3参数
    APP_PARAM_HAND_MODULE handParamModule4;//机械手在模块4参数
    APP_PARAM_UTIL utilParam;//轨道辅助保留参数
}APP_PARAM;


//系统参数长度
#define EXT_FLASH_APP_PARAM_SIZE    (sizeof(APP_PARAM)/sizeof(uint8_t))

#endif




