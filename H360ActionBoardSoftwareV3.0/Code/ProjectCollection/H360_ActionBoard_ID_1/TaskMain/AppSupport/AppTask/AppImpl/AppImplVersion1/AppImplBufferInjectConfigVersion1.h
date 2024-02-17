/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-03-27 10:35:18
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-27 16:30:14
 *FilePath: \H360_ActionBoard_ID_1\TaskMain\AppSupport\AppTask\AppImpl\AppImplBufferInjectConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_BUFFER_INJECT_CONFIG_VERSION_1_H_
#define __APP_IMPL_BUFFER_INJECT_CONFIG_VERSION_1_H_
#include "AppImplBase.h"

//缓冲液注液电机编号
#define BUFFER_INJECT_STEP_MOTOR_PUMP_ID            STEP_MOTOR10

/*不同的模式下,缓冲液注液的运行曲线 */
#define S_CURVE_BUFFER_INJECT_PUMP_DEBUG            SM_CURVE_0//缓冲液注液调试曲线
#define S_CURVE_BUFFER_INJECT_PUMP_INJECT           SM_CURVE_1//缓冲液注液注液曲线
#define S_CURVE_BUFFER_INJECT_PUMP_PRIME            SM_CURVE_2//缓冲液注液灌注曲线
#define S_CURVE_BUFFER_INJECT_PUMP_SUCK             SM_CURVE_3//缓冲液注液回吸曲线

//注液一次的步数
#define STEPS_FULL_CIRCLE_BUFFER_INJECT_PUMP        1600

//等待灌注稳定时间
#define TIME_MS_WAIT_PRIME_STABLE                   500
//等待注液稳定时间
#define TIME_MS_WAIT_INJECT_STABLE                  100
//等待回吸稳定时间
#define TIME_MS_WAIT_SUCKBACK_STABLE                50


#endif




