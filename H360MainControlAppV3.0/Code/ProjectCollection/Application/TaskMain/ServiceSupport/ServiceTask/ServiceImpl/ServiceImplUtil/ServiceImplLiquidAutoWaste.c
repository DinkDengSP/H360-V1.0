/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-04-24 08:52:35
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-17 15:33:35
**FilePath: \Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplUtil\ServiceImplLiquidAutoWaste.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplLiquidAutoUtil.h"
#include "ServiceStatePrivate.h"
#include "TestFlow_ActionAPI.h"

//废液桶满浮球的计数值
uint32_t liquidAutoWasteFullSensorValidCount = 0;
//上一次执行检测的时间
static uint32_t lastWasteFullSensorCheckTimeStamp = 0;
//废液浮球检测一次的时间
#define LIQUID_WASTE_AUTO_SENSOR_CHECK_PERIOD_MS    5000
//废液上浮球持续触发的报错时间
#define LIQUID_WASTE_AUTO_SENSOR_CHECK_MAX_TIMES    (90000/LIQUID_WASTE_AUTO_SENSOR_CHECK_PERIOD_MS)

//初始化排废液状态
LH_ERR ServiceImplLiquidAutoWasteStateInit(void)
{
    OS_ERR err = OS_ERR_NONE;
    liquidAutoWasteFullSensorValidCount = 0;
    //获取当前时间
    lastWasteFullSensorCheckTimeStamp = OSTimeGet(&err);
    //设置自动排废液关闭
    return LH_ERR_NONE;
}

//循环排废液状态
LH_ERR ServiceImplLiquidAutoWasteStateLoop(void)
{
    OS_ERR err = OS_ERR_NONE;
    uint32_t currentTimeStamp;
    currentTimeStamp = OSTimeGet(&err);
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    CAN1_REPORT_ERR_CODE commErrorCode = CAN1_REPORT_ERR_OK;
    CAN1_REPORT_ERR_LEVEL commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //满传感器状态
    SENSOR_STATE fullSensorState = SENSOR_STATE_VALID;
    //如果当前已经联机,正常模式,打开了自动抽废液
    if((FLAG_HAND_SHAKE_TRIG == ServiceStateReadAssistHandShakeFlag())&&(MODE_RELEASE == ServiceStateReadAssistRunningMode())&&(LIQUID_AUTO_STATE_ENABLE == ServiceStateReadLiquidAutoWasteState()))
    {
        //获取当前时间,达到检测时间
        if(currentTimeStamp - lastWasteFullSensorCheckTimeStamp >= LIQUID_WASTE_AUTO_SENSOR_CHECK_PERIOD_MS)
        {
            lastWasteFullSensorCheckTimeStamp = currentTimeStamp;
            //读取废液桶满传感器
            errorCode = ServiceImplLiquidAuto_ModuleInputRead(LIQUID_AUTO_LIQUID_WASTE_FULL_SENSOR,&fullSensorState,&commErrorCode,&commErrorLevel);
            if(errorCode != LH_ERR_NONE)
            {
                //错误代码打印
                ServicePrintfShowError(errorCode);
                return errorCode;
            }
            //如果上浮球触发
            if(fullSensorState == SENSOR_STATE_VALID)
            {
                liquidAutoWasteFullSensorValidCount++;
                if(liquidAutoWasteFullSensorValidCount < LIQUID_WASTE_AUTO_SENSOR_CHECK_MAX_TIMES)
                {
                    //给上位机提示一下,抽废液上浮球触发
                    Service_Api_RespondWarning(CAN1_REPORT_ERR_LEVEL_WARNING, CAN1_REPORT_ERR_LIQUID_AUTO_WASTE_HIGH_SENSOR_TRIG, (LH_ERR)liquidAutoWasteFullSensorValidCount);
                    SystemPrintf("Tips: AutoWaste Warning,SensorTrig %d\r\n",liquidAutoWasteFullSensorValidCount);
                }
                else if(liquidAutoWasteFullSensorValidCount == LIQUID_WASTE_AUTO_SENSOR_CHECK_MAX_TIMES)
                {
                    //废水上浮球持续触发,此刻急停
                    ServiceReportMsgToAppTask(TESTFLOW_MSG_CMD_EVENT_LIQUID_WASTE_TIME_OUT,NULL,0);
                    //设置自动抽废液状态关闭
                    ServiceStateWriteLiquidAutoWasteState(LIQUID_AUTO_STATE_DISABLE);
                    //关闭自动抽废液
                    errorCode = Can2SubBoard1LiquidAuto_DisableWasteLiquidAutomatic();
                    if(errorCode != LH_ERR_NONE)
                    {
                        commErrorCode = CAN1_REPORT_ERR_LIQUID_AUTO_DISABLE_WASTE_DISCHARGE_AUTO;
                        commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
                        //错误代码打印
                        ServicePrintfShowError(errorCode);
                    }
                    SystemPrintf("Tips: AutoWaste Error,SensorTrig %d\r\n",liquidAutoWasteFullSensorValidCount);
                }
            }
            else
            {
                //上浮球信号消息,计数清除
                liquidAutoWasteFullSensorValidCount = 0;
            }
        }
    }
    return LH_ERR_NONE;
}
