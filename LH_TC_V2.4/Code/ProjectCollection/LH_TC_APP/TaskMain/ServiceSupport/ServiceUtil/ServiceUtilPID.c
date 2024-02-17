/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved
**Author: DengXiaoJun
**Date: 2020-01-19 11:58:32
**LastEditors: DengXiaoJun
**LastEditTime: 2020-01-19 13:06:32
**FilePath: \LH_TC_APP_V2.3\TaskMain\ServiceSupport\ServiceUtil\ServiceUtilPID.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceUtilPID.h"
#include "os.h"

//清除PID系统状态
void ServicePidClearState(SERVICE_MSG_PID* pidState)
{
    OS_ERR err;
    pidState->lastIntegralCache = 0.0;//上次的积分缓存
    pidState->lastIntegralTick = OSTimeGet(&err);//上次的积分时间
    pidState->lastValue = 0.0;//微分上次的值缓存
    pidState->lastLastValue = 0.0;//微分上上次的值缓存
    pidState->calcCountD = 0;//微分缓存个数,大于2的时候才能计算微分
    pidState->lastDifferentialTick = OSTimeGet(&err);//上一次微分时间
}

//PID计算
void ServicePidCalcOutput(SERVICE_MSG_PID* pidState,float currentValue,float* output)
{
    OS_ERR err;
    float calcUtil = 0.0;
    float changeLast = 0.0;
    float changeThis = 0.0;
    //比例计算
    calcUtil = (pidState->kp)*(pidState->target - currentValue);
    //在满足积分时间的条件下进行积分计算
    if(OSTimeGet(&err) - (pidState->lastIntegralTick) >= (1000*(pidState->systemIntegralTimeSecond)))
    {
        pidState->lastIntegralTick = OSTimeGet(&err);
        //积分缓存
        pidState->lastIntegralCache += (pidState->target - currentValue);
        //积分计算
        calcUtil += (pidState->ki)*(pidState->lastIntegralCache);
    }
    //在满足微分时间设定的情况下进行微分计算
    if(OSTimeGet(&err) - (pidState->lastDifferentialTick) >= (1000*(pidState->systemDifferentialTimeSecond)))
    {
        pidState->lastDifferentialTick = OSTimeGet(&err);
        if(pidState->calcCountD == 0)
        {
            pidState->lastLastValue = currentValue;
            pidState->calcCountD = 1;
        }
        else if(pidState->calcCountD == 1)
        {
            pidState->lastValue = currentValue;
            pidState->calcCountD = 2;
        }
        else
        {
            //微分计算
            changeLast = (pidState->lastValue) - (pidState->lastLastValue);
            changeThis = currentValue - pidState->lastValue;
            calcUtil += (pidState->kd)*(changeThis - changeLast);
            //记录微分更新
            pidState->lastLastValue = pidState->lastValue;
            pidState->lastValue = currentValue;
        }
    }
    //计算完成
    *output = calcUtil;
}
























