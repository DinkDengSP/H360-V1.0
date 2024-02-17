/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-20 19:22:12
**LastEditors: DengXiaoJun
**LastEditTime: 2021-11-16 11:18:38
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

//测试试管固定状态
typedef enum TUBE_CLAMP_STATE
{
    TUBE_CLAMP_UNKNOW  = 0X00,//未知状态
    TUBE_CLAMP_RELEASE = 0X01,//释放状态
    TUBE_CLAMP_CATCH   = 0X02,//夹紧状态
}TUBE_CLAMP_STATE;

//事件发生标志
typedef enum EVENT_FLAG
{
    EVENT_FLAG_NONE,//事件未发生
    EVENT_FLAG_TRIG,//事件已发生
}EVENT_FLAG;


#endif



