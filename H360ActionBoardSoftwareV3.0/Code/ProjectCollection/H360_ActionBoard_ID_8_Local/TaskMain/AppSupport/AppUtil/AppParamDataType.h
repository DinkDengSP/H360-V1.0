/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-09 09:47:55
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-12-24 15:12:16
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_PARAM_DATA_TYPE_H_
#define __APP_PARAM_DATA_TYPE_H_
#include "BoardInc.h"

//应用配置的标志位,初始化检测该标志位,如果不对,需要设置为默认参数
#define APP_PARAM_DEFAULT_FLAG              0XAA5555AA

//进样仓
typedef struct APP_PARAM_RACK_STORE_IN
{
    int32_t pushMotorResetCorrect;  //推进电机复位修正
    int32_t pushMotorRackEndMax;    //推进电机推架子到末端最大步数
    int32_t stepsCompensateToEntry; //进样进入入口的补偿
    int32_t reserveArray[101];       //保留参数
}APP_PARAM_RACK_STORE_IN;
#define LENGTH_APP_PARAM_RACK_STORE_IN              (sizeof(APP_PARAM_RACK_STORE_IN)/sizeof(uint8_t))

//出样仓
typedef struct APP_PARAM_RACK_STORE_OUT
{
    int32_t pushMotorResetCorrect;      //出样前后电机复位修正
    int32_t rackStartPushPos;           //出样前后电机工作起点
    int32_t pushOnceStep;               //出样前后电机推进一次的步数
    int32_t pushRackCountMaxTaryNearKey;//出样前后电机推到靠近按键托盘末端最大推进次数
    int32_t rotateActionPos;            //出样旋转电机工作位置坐标
    int32_t rotateIdlePos;              //出样旋转电机空闲位置坐标
    int32_t pushRackCountMaxTaryNearRail;//出样前后电机推到靠近轨道托盘末端最大推进次数
    int32_t stepRackFormOutSwitchMoveOutToStore;//将试管架从换轨出口推到出样仓入口的步数
    int32_t reserveArray[98];      //保留参数
}APP_PARAM_RACK_STORE_OUT;
#define LENGTH_APP_PARAM_RACK_STORE_OUT             (sizeof(APP_PARAM_RACK_STORE_OUT)/sizeof(uint8_t))

//轨道进样节点
typedef struct APP_PARAM_RACK_ENTRY
{
    int32_t actionPos;          //防堵电机顶住前端的工作坐标
    int32_t idlePos;            //防堵电机释放的工作坐标
    int32_t railConveyorBeltMotorType;//轨道传送带电机
    int32_t reserveArray[99];  //保留参数
}APP_PARAM_RACK_ENTRY;
#define LENGTH_APP_PARAM_RACK_ENTRY             (sizeof(APP_PARAM_RACK_ENTRY)/sizeof(uint8_t))

//轨道进样扫码节点
typedef struct APP_PARAM_RACK_BAR_SCAN
{
    int32_t blockMotorActionPos;   //扫码限位电机挡住试管架的坐标
    int32_t blockMotorIdlePos;     //扫码限位电机释放试管架的坐标
    int32_t pushMotorStartPos;     //扫码移动电机扫码起点坐标
    int32_t reserveArray[100];     //保留参数
}APP_PARAM_RACK_BAR_SCAN;
#define LENGTH_APP_PARAM_RACK_BAR_SCAN    (sizeof(APP_PARAM_RACK_BAR_SCAN)/sizeof(uint8_t))

//轨道进样测试节点
typedef struct APP_PARAM_RACK_TEST
{
    int32_t resetCorrectAndTube5Pos;//复位修正以及试管5位置
    int32_t reserveArray[100];//保留参数
}APP_PARAM_RACK_TEST;
#define LENGTH_APP_PARAM_RACK_TEST        (sizeof(APP_PARAM_RACK_TEST)/sizeof(uint8_t))

//轨道进样换轨节点
typedef struct APP_PARAM_RACK_IN_SWITCH
{
    int32_t comeInPos;          //进样换轨电机换轨初始位置(试管架进入位置)
    int32_t moveOutPos;         //进样换轨电机换轨目标位置(试管架离开位置)
    int32_t reserveArray[100];  //保留参数
}APP_PARAM_RACK_IN_SWITCH;
#define LENGTH_APP_PARAM_RACK_IN_SWITCH (sizeof(APP_PARAM_RACK_IN_SWITCH)/sizeof(uint8_t))

//轨道出样等待节点1
typedef struct APP_PARAM_RACK_WAIT_OUT1
{
    int32_t actionPos;          //限位电机挡住试管架位置
    int32_t idlePos;            //限位电机释放试管架位置
    int32_t reserveArray[100];  //保留参数
}APP_PARAM_RACK_WAIT_OUT1;
#define LENGTH_APP_PARAM_RACK_WAIT_OUT1  (sizeof(APP_PARAM_RACK_WAIT_OUT1)/sizeof(uint8_t))

//轨道出样等待节点2
typedef struct APP_PARAM_RACK_WAIT_OUT2
{
    int32_t actionPos;        //限位电机挡住试管架位置
    int32_t idlePos;          //限位电机释放试管架位置
    int32_t reserveArray[100];//保留参数
}APP_PARAM_RACK_WAIT_OUT2;
#define LENGTH_APP_PARAM_RACK_WAIT_OUT2              (sizeof(APP_PARAM_RACK_WAIT_OUT2)/sizeof(uint8_t))

//轨道出样换轨节点
typedef struct APP_PARAM_RACK_OUT_SWITCH
{
    int32_t comeInPos;        //出样换轨电机换轨初始位置(试管架进入位置)
    int32_t moveOutPos;       //出样换轨电机换轨目标位置(试管架离开位置)
    int32_t reserveArray[100];//保留参数
}APP_PARAM_RACK_OUT_SWITCH;
#define LENGTH_APP_PARAM_RACK_OUT_SWITCH              (sizeof(APP_PARAM_RACK_OUT_SWITCH)/sizeof(uint8_t))

//系统参数所有线程的集合
typedef struct APP_PARAM_SET
{
    uint32_t paramFlag;
    APP_PARAM_RACK_STORE_IN paramRackStoreIn;
    APP_PARAM_RACK_STORE_OUT paramRackStoreOut;
    APP_PARAM_RACK_ENTRY paramRackEntry;
    APP_PARAM_RACK_BAR_SCAN paramRackBarScan;
    APP_PARAM_RACK_TEST paramRackTest;
    APP_PARAM_RACK_IN_SWITCH paramRackInSwitch;
    APP_PARAM_RACK_WAIT_OUT1 paramRackWaitOut1;
    APP_PARAM_RACK_WAIT_OUT2 paramRackWaitOut2;
    APP_PARAM_RACK_OUT_SWITCH paramRackOutSwitch;
}APP_PARAM_SET;
#define LENGTH_APP_PARAM_SET            (sizeof(APP_PARAM_SET)/sizeof(uint8_t))

#endif


