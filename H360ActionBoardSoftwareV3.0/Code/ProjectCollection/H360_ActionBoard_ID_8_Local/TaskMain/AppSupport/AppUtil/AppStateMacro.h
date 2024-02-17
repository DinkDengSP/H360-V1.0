/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-11-09 11:27:58
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-12-07 10:19:50
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_STATE_MACRO_H_
#define __APP_STATE_MACRO_H_
#include "BoardInc.h"
#include "CanSubBase.h"

//事件发生标志
typedef enum EVENT_FLAG
{
    EVENT_FLAG_NONE = 0X00,//事件未发生
    EVENT_FLAG_TRIG = 0X01,//事件已发生
}EVENT_FLAG;

//条件是否满足
typedef enum CONDITION_RESULT
{
    CONDITION_FAIL = 0x00,//不满足条件
    CONDITION_OK   = 0x01,//满足条件
}CONDITION_RESULT;

//节点阻挡或者释放状态
typedef enum RAIL_NODE_STATE
{
    RAIL_NODE_UNKNOW    = 0X00,//未知状态
    RAIL_NODE_BLOCK     = 0X01,//挡住状态
    RAIL_NODE_RELEASE   = 0X02,//释放状态
}RAIL_NODE_STATE;

//出样推手旋转状态
typedef enum PUSH_ROTATE_STATE
{
    PUSH_ROTATE_STATE_UNKNOW = 0X00,
    PUSH_ROTATE_STATE_IDLE   = 0X01,
    PUSH_ROTATE_STATE_ACTION = 0X02,
}PUSH_ROTATE_STATE;

//换轨状态
typedef enum RAIL_SWITCH_STATE
{
    RAIL_SWITCH_STATE_UNKNOW    = 0x00,//未知状态
    RAIL_SWITCH_STATE_IN        = 0x01,//在样本入口
    RAIL_SWITCH_STATE_OUT       = 0x02,//在样本出口
}RAIL_SWITCH_STATE;

//测试区试管序号
typedef enum TEST_TUBE_NO
{
    TEST_TUBE_UNKNOW  = 0x00,//未知状态
    TEST_TUBE_1       = 0x01,//试管1处于待测位
    TEST_TUBE_2       = 0x02,//试管2处于待测位
    TEST_TUBE_3       = 0x03,//试管3处于待测位
    TEST_TUBE_4       = 0x04,//试管4处于待测位
    TEST_TUBE_5       = 0x05,//试管5处于待测位
    TEST_TUBE_RELEASE = 0X06,//试管处于释放状态
}TEST_TUBE_NO;

#endif
