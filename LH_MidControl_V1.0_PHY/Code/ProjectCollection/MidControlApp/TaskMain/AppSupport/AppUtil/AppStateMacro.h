/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-20 10:32:45
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-20 10:38:01
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_STATE_MACRO_H_
#define __APP_STATE_MACRO_H_
#include "BoardInc.h"


typedef enum STATE_MODULE_NO
{
    STATE_MODULE_1  = 0X00,
    STATE_MODULE_2  = 0X01,
    STATE_MODULE_3  = 0X02,
    STATE_MODULE_4  = 0X03,
    STATE_MODULE_5  = 0X04,
}STATE_MODULE_NO;

//比对结果
typedef enum STATE_CMP_RESULT
{
    STATE_CMP_EQUAL     = 0X00,//比较结果相等
    STATE_CMP_NOT_EQUAL = 0X01,//比较结果不等
}STATE_CMP_RESULT;

//主流程状态
typedef enum MAIN_STATE_TEST_FLOW
{
    MAIN_STATE_TEST_FLOW_IDLE   = 0X00,                 //状态空闲
    MAIN_STATE_TEST_FLOW_ONILNE_ING,                    //正在联机
    MAIN_STATE_TEST_FLOW_ONILNE_TRACK_OK,               //轨道已联机
    MAIN_STATE_TEST_FLOW_ONILNE_DEVICE_OK,              //设备已联机
    MAIN_STATE_TEST_FLOW_ONILNE,                        //联机
    MAIN_STATE_TEST_FLOW_OFFILNE,                       //脱机
    MAIN_STATE_TEST_FLOW_RESET_ING,                     //复位
    MAIN_STATE_TEST_FLOW_RESET_TRACK_OK,                //轨道已复位
    MAIN_STATE_TEST_FLOW_RESET_DEVICE_OK,               //设备已复位
    MAIN_STATE_TEST_FLOW_RESET,                         //复位
}MAIN_STATE_TEST_FLOW;


#endif




