/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-26 16:55:41
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-10-29 13:47:07
 *FilePath: \App\TaskMain\ServiceSupport\ServiceUtil\ServiceStateTypeEnum.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_STATE_TYPE_ENUM_H_
#define __SERVICE_STATE_TYPE_ENUM_H_

//进样样本盘状态
typedef enum PUSH_IN_STATE
{
    PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST    = 0, // 状态空闲,门关着,托盘在位
    PUSH_IN_STATE_IDLE_DOOR_CLOSE_TRAY_LOSS     = 1, // 状态空闲,门关着,托盘丢失
    PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_EXIST     = 2, // 状态空闲,门开着,托盘在位
    PUSH_IN_STATE_IDLE_DOOR_OPEN_TRAY_LOSS      = 3, // 状态空闲,门开着,托盘丢失
    PUSH_IN_STATE_BUSY                          = 4, // 状态忙
    PUSH_IN_STATE_NULL                             , // 空
}PUSH_IN_STATE;

//出样样本盘状态
typedef enum PUSH_OUT_STATE
{
    PUSH_OUT_STATE_IDLE_DOOR_CLOSE_TRAY_EXIST   = 0, //状态空闲,门关着,托盘在位
    PUSH_OUT_STATE_IDLE_DOOR_CLOSE_TRAY_LOSS    = 1, //状态空闲,门关着,托盘丢失
    PUSH_OUT_STATE_IDLE_DOOR_OPEN_TRAY_EXIST    = 2, //状态空闲,门开着,托盘在位
    PUSH_OUT_STATE_IDLE_DOOR_OPEN_TRAY_LOSS     = 3, //状态空闲,门开着,托盘丢失
    PUSH_OUT_STATE_BUSY                         = 4, //状态忙,正在工作
    PUSH_OUT_STATE_NULL,                             // 空
}PUSH_OUT_STATE;

//轨道状态
typedef enum RAIL_TRANS_STATE
{
    RAIL_TRANS_STATE_IDLE = 0,//轨道空闲
    RAIL_TRANS_STATE_BUSY = 1,//轨道忙
}RAIL_TRANS_STATE;

//机械手状态
typedef enum RAIL_HAND_STATE
{
    RAIL_HAND_STATE_IDLE = 0,//机械手空闲
    RAIL_HAND_STATE_BUSY = 1,//机械手正忙
}RAIL_HAND_STATE;

//机械手夹爪位置
typedef enum RAIL_HAND_CLAW_STATE
{
    RAIL_HAND_CLAW_STATE_FRONT  = 0,//机械手爪手状态伸出
    RAIL_HAND_CLAW_STATE_BACK   = 1,//机械手爪手收回
}RAIL_HAND_CLAW_STATE;
#endif




