/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-01 14:36:55
**FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppUtil\AppParamDefault.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppParamDefault.h"

//进样的默认值
const APP_PARAM_PUSHIN pushInDefaultSetting = {
    0,//复位的零位修正
    17000,//进样最大坐标
    {0},
};

//出样的默认值
const APP_PARAM_PUSHOUT pushOutDefaultSetting = {
    0,//复位的零位修正
    1000,//出样出一次试管架运行的偏移
    {0},
};


//轨道独立参数的默认值
const APP_PARAM_RAIL_INDEPENDENT RailIndependentDefaultSetting = {
    0,//轨道复位修正坐标
    0,//急诊位坐标
    100,//进样1位置坐标
    500,//进样2位置坐标
    2000,//试管架条码扫描位坐标
    2500,//试管1扫描位坐标
    2800,//试管2扫描位坐标
    3000,//试管3扫描位坐标
    3200,//试管4扫描位坐标
    3500,//试管5扫描位坐标
    1500,//转向位1坐标
    5500,//转向位2坐标
    6000,//出样位1坐标
    6300,//出样位2坐标
    0,//扫码枪类型
    {0},
};


//轨道模块参数的默认值
const APP_PARAM_RAIL_MODULE railModuleDefaultSetting = {
    0,//待测位1
    0,//待测位2
    0,//待测位3
    0,//重测位1
    0,//重测位2
    0,//重测位3
    0,//重测位4
    0,//重测位5
    0,//重测位6
    0,//重测位7
    0,//重测位8
    0,//放架位
    0,//取架位
    0,//吸样位
    300,//测试区单个试管偏移
    {0},
};


//机械手独立参数的默认值
const APP_PARAM_HAND_INDEPENDENT handIndependentDefaultSetting = {
    0,//手升降零位偏移
    0,//手前后零位偏移
    0,//手旋转零位偏移
    600,//手升降最高位
    0,//手升降最低位
    0,//手前后进样1位置坐标
    0,//手前后进样2位置坐标
    0,//手前后出样1位置坐标
    0,//手前后出样2位置坐标
    0,//手前后急诊位置坐标
    0,//手前后辅助旋转的偏移
    0,//手旋转到进样的坐标
    800,//手旋转到急诊方向的坐标
    {0},
};


//机械手模块参数的默认值
const APP_PARAM_HAND_MODULE handModuleDefaultSetting = {
    0,//手前后在模块上进样缓冲区1的前后位置
    0,//手前后在模块上进样缓冲区2的前后位置
    0,//手前后在模块上进样缓冲区3的前后位置
    0,//手前后在模块上重测缓冲区1的前后位置
    0,//手前后在模块上重测缓冲区2的前后位置
    0,//手前后在模块上重测缓冲区3的前后位置
    0,//手前后在模块上重测缓冲区4的前后位置
    0,//手前后在模块上重测缓冲区5的前后位置
    0,//手前后在模块上重测缓冲区6的前后位置
    0,//手前后在模块上重测缓冲区7的前后位置
    0,//手前后在模块上重测缓冲区8的前后位置
    0,//手前后模块上测试区的前后位置
    {0},
};

//辅助参数的默认值
const APP_PARAM_UTIL appUtilDefaultSetting = {
    0,
    {0},
};












