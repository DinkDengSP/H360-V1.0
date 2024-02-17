/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-04-24 08:52:11
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-21 15:53:07
**FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplUtil\ServiceImplLiquidAutoPureWater.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplLiquidAutoUtil.h"
#include "ServiceStatePrivate.h"
#include "TestFlow_ActionAPI.h"

static SENSOR_STATE pureWaterFullState;//纯水满传感器
static SENSOR_STATE pureWaterLowState;//纯水空传感器

//纯水补充的累计时间
//从低液位到满液位触发的计时
static uint32_t pureWaterSupplementCountLow2Full;
//从空桶到低液位触发的计时
static uint32_t pureWaterSupplementCountEmpty2Low;

//补纯水第二阶段检测下浮球发送报警标志
static uint8_t  flagUpLoadLowSensorWarning = 0;

//更新纯水传感器讯息
static void ServiceImplLiquidAutoPureWaterSupplementReflushSensor(void)
{
    //初始化无错误
        CAN1_REPORT_ERR_CODE commErrorCode;
        CAN1_REPORT_ERR_LEVEL commErrorLevel;
    //读取传感器讯息
        ServiceImplLiquidAuto_ModuleInputRead(LIQUID_AUTO_PURE_WATER_FULL_SENSOR,&pureWaterFullState,&commErrorCode,&commErrorLevel);
        ServiceImplLiquidAuto_ModuleInputRead(LIQUID_AUTO_PURE_WATER_EMPTY_SENSOR,&pureWaterLowState,&commErrorCode,&commErrorLevel);
}

//检查传感器讯息
static LH_ERR ServiceImplLiquidAutoPureWaterSupplementCheckSensor()
{
    //上下浮球同时有效
    if((pureWaterFullState == SENSOR_STATE_VALID)&&(pureWaterLowState == SENSOR_STATE_VALID))
    {
        return LH_ERR_BOARD_MAIN_LIQUID_AUTO_PURE_WATER_SENSOR_LOGIC;
    }
    return LH_ERR_NONE;
}

//错误处理
static void ServiceImplLiquidAutoPureWaterSupplementErrorProc()
{
    //初始化无错误
        CAN1_REPORT_ERR_CODE commErrorCode;
        CAN1_REPORT_ERR_LEVEL commErrorLevel;
    //关闭纯水补液泵
        ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_DP3,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
    //等待稳定
        CoreDelayMs(30);
    //关闭纯水进水阀
        ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_V241,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
    //进入空闲状态
        ServiceStateWriteLiquidAutoPureWaterStage(PURE_WATER_SUPPLEMENT_NORMAL_IDLE);
    //关闭纯水自动补水
        ServiceStateWriteLiquidAutoPureWaterState(LIQUID_AUTO_STATE_DISABLE);
    //清空计数
        pureWaterSupplementCountEmpty2Low = 0;
        pureWaterSupplementCountLow2Full = 0;
}

//初始化纯水补充状态
LH_ERR ServiceImplLiquidAutoPureWaterSupplementStateInit(void)
{
    //初始化关闭纯水自动补充
    ServiceStateWriteLiquidAutoPureWaterState(LIQUID_AUTO_STATE_DISABLE);
    //设置阶段状态空闲
    ServiceStateWriteLiquidAutoPureWaterStage(PURE_WATER_SUPPLEMENT_NORMAL_IDLE);
    return LH_ERR_NONE;
}

//纯水补充空闲状态
static void ServiceImplLiquidAutoPureWaterSupplementStateIdle(void)
{
    //初始化无错误
        CAN1_REPORT_ERR_CODE commErrorCode;
        CAN1_REPORT_ERR_LEVEL commErrorLevel;
    //读取纯水补水模式
        int32_t pureWaterAddMode = 0;
        ServiceParamReadSingle(INDEX_MAIN_SERVICE_PARAM_LIQUID_AUTO,INDEX_SUB_SERVICE_PARAM_LIQUID_AUTO_PURE_WATER_ADD_MODE,&pureWaterAddMode);
    //如果下液位触发,进入补水状态
        if(pureWaterLowState == SENSOR_STATE_VALID)
        {
            //打开纯水进水阀
            ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_V241,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
            //等待阀打开稳定
            CoreDelayMs(30);
            //打开纯水补液泵
            ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_DP3,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
            //进入空桶到下液位补充状态
            ServiceStateWriteLiquidAutoPureWaterStage(PURE_WATER_SUPPLEMENT_NORMAL_EMPTY_TO_LOW_RUNNING);
            //清空计数
            pureWaterSupplementCountEmpty2Low = 0;
            pureWaterSupplementCountLow2Full = 0;
            //打印开始补充纯水
            SystemPrintf("Tips-PureWaterAdd StartFirst,Mode = %d\r\n",pureWaterAddMode);
        }
}

//纯水补充空桶到下液位
static void ServiceImplLiquidAutoPureWaterSupplementStateEmptyToLowRunning(void)
{
    //初始化无错误
        CAN1_REPORT_ERR_CODE commErrorCode;
        CAN1_REPORT_ERR_LEVEL commErrorLevel;
    //读取纯水补水模式
        int32_t pureWaterAddMode = 0;
        ServiceParamReadSingle(INDEX_MAIN_SERVICE_PARAM_LIQUID_AUTO,INDEX_SUB_SERVICE_PARAM_LIQUID_AUTO_PURE_WATER_ADD_MODE,&pureWaterAddMode);
    //确定纯水补水时间
        int32_t pureWaterAddMaxTime;
        if(pureWaterAddMode == 0)pureWaterAddMaxTime = TIME_PURE_WATER_ADD_TIME_MAX_MODE0_EMPTY_2_LOW;
        else if(pureWaterAddMode == 1)pureWaterAddMaxTime = TIME_PURE_WATER_ADD_TIME_MAX_MODE1_EMPTY_2_LOW;
        else pureWaterAddMaxTime = TIME_PURE_WATER_ADD_TIME_MAX_MODE2_EMPTY_2_LOW;
    //空桶到下液位补水时间增加
        pureWaterSupplementCountEmpty2Low++;
    //如果上液位触发,进入空闲模式
        if(pureWaterFullState == SENSOR_STATE_VALID)
        {
            //关闭纯水补液泵
                ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_DP3,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
            //等待稳定
                CoreDelayMs(30);
            //关闭纯水进水阀
                ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_V241,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
            //进入空闲状态
                ServiceStateWriteLiquidAutoPureWaterStage(PURE_WATER_SUPPLEMENT_NORMAL_IDLE);
            //打印补充纯水时间,显示直接空到满
                SystemPrintf("Tips-PureWaterAdd NormalEnd,EmptyToFull,AddTimeCount = %d\r\n",pureWaterSupplementCountEmpty2Low);
            //清空计数
                pureWaterSupplementCountEmpty2Low = 0;
                pureWaterSupplementCountLow2Full = 0;
            //完成返回
                return;
        }
    //如果下液位不触发,进入下液位到满运行状态
        if(pureWaterLowState != SENSOR_STATE_VALID)
        {
            //进入下液位到满运行状态
                ServiceStateWriteLiquidAutoPureWaterStage(PURE_WATER_SUPPLEMENT_NORMAL_LOW_TO_FULL_RUNNING);
            //打印补充纯水时间,显示空到下液位触发
                SystemPrintf("Tips-PureWaterAdd Stage1End,EmptyToLow,AddTimeCount = %d\r\n",pureWaterSupplementCountEmpty2Low);
            //清空计数
                pureWaterSupplementCountEmpty2Low = 0;
                pureWaterSupplementCountLow2Full = 0;
            //使能纯水第二阶段下浮球持续触发的报警
                flagUpLoadLowSensorWarning = 1;
            //完成返回
                return;
        }
    //如果空到下液位补水时间超时,报错停止
        if(pureWaterSupplementCountEmpty2Low >= pureWaterAddMaxTime)
        {
            //打印补水时间异常,超时未补满
                SystemPrintf("Tips-PureWaterAdd ErrorTimeOutStageEmptyToLowEnd,AddMode = %d,AddTimeCount = %d\r\n",pureWaterAddMode,pureWaterSupplementCountEmpty2Low);
            //错误处理
                ServiceImplLiquidAutoPureWaterSupplementErrorProc();
            //上报纯水补充异常
                ServiceReportMsgToAppTask(TESTFLOW_MSG_CMD_EVENT_LIQUID_PURE_WATER_TIME_OUT,NULL,0);
            //完成返回
                return;
        }
}

//纯水补充下液位到满液位
static void ServiceImplLiquidAutoPureWaterSupplementStateLowToFullRunning(void)
{
    //初始化无错误
        CAN1_REPORT_ERR_CODE commErrorCode;
        CAN1_REPORT_ERR_LEVEL commErrorLevel;
    //读取纯水补水模式
        int32_t pureWaterAddMode = 0;
        ServiceParamReadSingle(INDEX_MAIN_SERVICE_PARAM_LIQUID_AUTO,INDEX_SUB_SERVICE_PARAM_LIQUID_AUTO_PURE_WATER_ADD_MODE,&pureWaterAddMode);
    //确定纯水补水时间
        int32_t pureWaterAddMaxTime;
        if(pureWaterAddMode == 0)pureWaterAddMaxTime = TIME_PURE_WATER_ADD_TIME_MAX_MODE0_LOW_TO_FULL;
        else if(pureWaterAddMode == 1)pureWaterAddMaxTime = TIME_PURE_WATER_ADD_TIME_MAX_MODE1_LOW_TO_FULL;
        else pureWaterAddMaxTime = TIME_PURE_WATER_ADD_TIME_MAX_MODE2_LOW_TO_FULL;
    //时间计数增加
        pureWaterSupplementCountLow2Full++;
    //如果上液位触发,则进入空闲状态
        if(pureWaterFullState == SENSOR_STATE_VALID)
        {
            //关闭纯水补液泵
                ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_DP3,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
            //等待稳定
                CoreDelayMs(30);
            //关闭纯水进水阀
                ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_V241,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
            //进入空闲状态
                ServiceStateWriteLiquidAutoPureWaterStage(PURE_WATER_SUPPLEMENT_NORMAL_IDLE);
            //打印补充纯水时间
                SystemPrintf("Tips-PureWaterAdd NormalEnd,LowToFull,AddTimeCount = %d\r\n",pureWaterSupplementCountLow2Full);
            //清空计数
                pureWaterSupplementCountEmpty2Low = 0;
                pureWaterSupplementCountLow2Full = 0;
            //完成操作返回
                return;
        }
    //有水机有水泵模式下纯水补充一段时间下浮球依然触发,上报一个报警
        if(pureWaterAddMaxTime == TIME_PURE_WATER_ADD_TIME_MAX_MODE2_LOW_TO_FULL  && pureWaterSupplementCountLow2Full > TIME_PURE_WATER_ADD_TIME_CHECK_LOW_SENSOR_AT_LOW_TO_FULL && pureWaterLowState == SENSOR_STATE_VALID)
        {
            if(flagUpLoadLowSensorWarning != 0)
                flagUpLoadLowSensorWarning++;
            //检测次数3
            if(flagUpLoadLowSensorWarning > 3)
            {
                //报警只发送一次
                flagUpLoadLowSensorWarning = 0;
                Service_Api_RespondWarning(CAN1_REPORT_ERR_LEVEL_WARNING,CAN1_REPORT_ERR_LIQUID_AUTO_PURE_WATER_LOW_SENSOR_CONTINUE_VALID,LH_ERR_BOARD_MAIN_LIQUID_AUTO_PURE_WATER_LOW_STATE_CONTINUE_VALID);
            }
        }
    //如果此时下液位再次触发,切换进入第一阶段补水
        if(pureWaterLowState == SENSOR_STATE_VALID)
        {
            //打印开始补充纯水
                SystemPrintf("Tips-PureWaterAdd StartAgain,Mode = %d,LowToFullTime:%d\r\n",pureWaterAddMode,pureWaterSupplementCountLow2Full);
            //打开纯水进水阀
                ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_V241,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
            //等待阀打开稳定
                CoreDelayMs(30);
            //打开纯水补液泵
                ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_DP3,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
            //进入空桶到下液位补充状态
                ServiceStateWriteLiquidAutoPureWaterStage(PURE_WATER_SUPPLEMENT_NORMAL_EMPTY_TO_LOW_RUNNING);
            //清空计数
                pureWaterSupplementCountEmpty2Low = 0;
                pureWaterSupplementCountLow2Full = 0;
            //完成操作,返回
                return;
        }
    //如果上液位超时未触发
        if(pureWaterSupplementCountLow2Full >= pureWaterAddMaxTime)
        {
            //打印补水时间异常,超时未补满
                SystemPrintf("Tips-PureWaterAdd WarningTimeOutLowToFullEnd,AddMode = %d,AddTimeCount = %d\r\n",pureWaterAddMode,pureWaterSupplementCountLow2Full);
            //这个不当成错误,而是上报一个警告给客户,状态切入空闲模式,等待下次下浮球再次触发再进行补水
            //关闭纯水补液泵
                ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_DP3,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
            //等待稳定
                CoreDelayMs(30);
            //关闭纯水进水阀
                ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_V241,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
            //进入空闲状态
                ServiceStateWriteLiquidAutoPureWaterStage(PURE_WATER_SUPPLEMENT_NORMAL_IDLE);
            //清空计数
                pureWaterSupplementCountEmpty2Low = 0;
                pureWaterSupplementCountLow2Full = 0;
            //上报纯水补充异常,注意级别
                Service_Api_RespondWarning(CAN1_REPORT_ERR_LEVEL_WARNING, CAN1_REPORT_ERR_LIQUID_AUTO_PURE_WATER_ADD_TIME_OUT_HIGH_SENSOR,LH_ERR_NONE);
            //完成操作返回
                return;
        }
}

//纯水传感器信号检测,增加稳定性检测内容
static LH_ERR ServiceImplLiquidAutoPureWaterSupplementCheckSensorStable(void)
{
    LH_ERR errorCode;
    //检测传感器信号
    errorCode = ServiceImplLiquidAutoPureWaterSupplementCheckSensor();
    if(errorCode != LH_ERR_NONE)
    {
        //纯水传感器异常
        if(errorCode == LH_ERR_BOARD_MAIN_LIQUID_AUTO_PURE_WATER_SENSOR_LOGIC)
        {
            //异常计数器增加
            ServiceStateWritePureWaterSensorErrorCount(ServiceStateReadPureWaterSensorErrorCount() +1);
            //如果异常计数器没有大于设置的异常阈值
            if(ServiceStateReadPureWaterSensorErrorCount() <= LIQUID_AUTO_SENSOR_CHECK_ERROR_THRESHOLD)
            {
                SystemPrintf("Tips-PureWaterAdd WarningSensorCheck,CheckCount = %d\r\n",ServiceStateReadPureWaterSensorErrorCount());
                //还没有达到设置的错误检测阈值,本次循环退出等待下一次检测
                return LH_ERR_BOARD_MAIN_LIQUID_AUTO_SENSOR_WARNING_SKIP;
            }
        }
        //发生异常退出
        return errorCode;
    }
    else
    {
        //纯水补水传感器无异常,清空异常计数
        ServiceStateClearPureWaterSensorErrorCount();
        //返回无错误
        return LH_ERR_NONE;
    }
}

//循环纯水补充状态
LH_ERR ServiceImplLiquidAutoPureWaterSupplementStateLoop(void)
{
    LH_ERR errorCode;
    //打开纯水检查才会进行循环检测,没有打开补水,直接退出
        if(LIQUID_AUTO_STATE_ENABLE != ServiceStateReadLiquidAutoPureWaterState())
        {
            return LH_ERR_NONE;
        }
    //读取纯水补水模式
        int32_t pureWaterAddMode = 0;
        ServiceParamReadSingle(INDEX_MAIN_SERVICE_PARAM_LIQUID_AUTO,INDEX_SUB_SERVICE_PARAM_LIQUID_AUTO_PURE_WATER_ADD_MODE,&pureWaterAddMode);
    //更新传感器信息
        ServiceImplLiquidAutoPureWaterSupplementReflushSensor();
    //状态机展示
        PURE_WATER_SUPPLEMENT_STATE stage = ServiceStateReadLiquidAutoPureWaterStage();
    //检测传感器状态是否正常
        errorCode = ServiceImplLiquidAutoPureWaterSupplementCheckSensorStable();
        if(errorCode == LH_ERR_BOARD_MAIN_LIQUID_AUTO_SENSOR_WARNING_SKIP)
        {
           //跳过本次检测
           return LH_ERR_NONE;
        }
        else if(errorCode != LH_ERR_NONE)
        {
            //发生错误
            //传感器异常的检测次数大于检测阈值,打印补水时间异常,传感器异常
            SystemPrintf("Tips-PureWaterAdd ErrorSensorEnd,AddMode = %d,AddTimeCountEmptyToLow = %d,AddTimeCountLowToFull = %d\r\n",pureWaterAddMode,pureWaterSupplementCountEmpty2Low,pureWaterSupplementCountLow2Full);
            //错误处理
            ServiceImplLiquidAutoPureWaterSupplementErrorProc();
            //清空计数
            ServiceStateClearPureWaterSensorErrorCount();
            //上报纯水补充异常
            ServiceReportMsgToAppTask(TESTFLOW_MSG_CMD_EVENT_LIQUID_PURE_WATER_SENSOR_LOGIC,NULL,0);
            return errorCode;
        }
        //任何错误没发生,自然进入下一步
    //状态机
        switch(stage)
        {
            case PURE_WATER_SUPPLEMENT_NORMAL_IDLE:
                ServiceImplLiquidAutoPureWaterSupplementStateIdle();
                break;
            case PURE_WATER_SUPPLEMENT_NORMAL_EMPTY_TO_LOW_RUNNING:
                ServiceImplLiquidAutoPureWaterSupplementStateEmptyToLowRunning();
                break;
            case PURE_WATER_SUPPLEMENT_NORMAL_LOW_TO_FULL_RUNNING:
                ServiceImplLiquidAutoPureWaterSupplementStateLowToFullRunning();
                break;
        }
    return LH_ERR_NONE;
}

//执行一次强制纯水补充
LH_ERR ServiceImplLiquidAutoPureWaterSupplementEnforceOnce(void)
{
    LH_ERR errorCode;
    //模式
        int32_t pureWaterAddMode = 0;
    //最长补水时间
        int32_t pureWaterAddMaxTime;
    //初始化无错误
        CAN1_REPORT_ERR_CODE commErrorCode;
        CAN1_REPORT_ERR_LEVEL commErrorLevel;
    //首先检查当前自动补水是否打开,打开了就报忙
        if(LIQUID_AUTO_STATE_ENABLE == ServiceStateReadLiquidAutoPureWaterState())
        {
            //返回纯水忙
            return LH_ERR_BOARD_MAIN_LIQUID_AUTO_PURE_WATER_BUSY;
        }
    //读取纯水补水模式
        ServiceParamReadSingle(INDEX_MAIN_SERVICE_PARAM_LIQUID_AUTO,INDEX_SUB_SERVICE_PARAM_LIQUID_AUTO_PURE_WATER_ADD_MODE,&pureWaterAddMode);
    //确定纯水补水时间
        //使用水桶,无水泵
        if(pureWaterAddMode == 0)pureWaterAddMaxTime = TIME_PURE_WATER_ENFORCE_ADD_TIME_MAX_MODE0_LOW_STATE_VALID ;
        //使用水桶,带水泵
        else if(pureWaterAddMode == 1)pureWaterAddMaxTime = TIME_PURE_WATER_ENFORCE_ADD_TIME_MAX_MODE1_LOW_STATE_VALID ;
        //使用水机,带水泵
        else pureWaterAddMaxTime = TIME_PURE_WATER_ENFORCE_ADD_TIME_MAX_MODE2_LOW_STATE_VALID ;
    //更新纯水传感器
        ServiceImplLiquidAutoPureWaterSupplementReflushSensor();
    //检查传感器状态
        errorCode = ServiceImplLiquidAutoPureWaterSupplementCheckSensor();
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //打印模式与理论时间
        SystemPrintf("PureWaterAdd_Mode:%d,MaxAddTime:%d\r\n",pureWaterAddMode,pureWaterAddMaxTime);
    //打开纯水进水阀
        ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_V241,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
    //等待阀打开稳定
        CoreDelayMs(30);
    //打开纯水补液泵
        ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_DP3,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
    //循环检测是否水满了
        uint16_t timeSecondCount = 0;
        do
        {
            //每次1S延时,计数器增加
                CoreDelayMs(1000);
                timeSecondCount++;
            //更新纯水传感器
                ServiceImplLiquidAutoPureWaterSupplementReflushSensor();
            //检查传感器状态
                errorCode = ServiceImplLiquidAutoPureWaterSupplementCheckSensor();
                if(errorCode != LH_ERR_NONE)
                {
                    break;
                }
            //检查是否满了
                if(pureWaterFullState == SENSOR_STATE_VALID)
                {
                    //中途满了
                    SystemPrintf("PureWaterAdd_FullState:%d,UsedTime:%d\r\n",pureWaterFullState,timeSecondCount);
                    break;
                }
        }while(timeSecondCount < pureWaterAddMaxTime);
    //关闭纯水补液泵
        ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_DP3,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
    //等待稳定
        CoreDelayMs(30);
    //关闭纯水进水阀
        ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_V241,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
    //更新纯水传感器
        ServiceImplLiquidAutoPureWaterSupplementReflushSensor();
    //强制补纯水后低液位任然有效
        if(pureWaterLowState == SENSOR_STATE_VALID)
        {
            SystemPrintf("E>PureWaterAdd_LowState:%d,UsedTime:%d\r\n",pureWaterLowState,timeSecondCount);
            return LH_ERR_BOARD_MAIN_LIQUID_AUTO_PURE_WATER_LOW_STATE_VALID_AFTER_ENFORCE_ADD;
        }
        else
        {
            SystemPrintf("PureWaterAdd_LowState:%d,UsedTime:%d\r\n",pureWaterLowState,timeSecondCount);
        }
        
    return errorCode;
}

//打开自动纯水补充
LH_ERR ServiceImplLiquidAutoPureWaterSupplementEnable(void)
{
    LH_ERR errorCode;
    //初始化无错误
        CAN1_REPORT_ERR_CODE commErrorCode;
        CAN1_REPORT_ERR_LEVEL commErrorLevel;
    //清空纯水浮球异常状态计数值
        ServiceStateClearPureWaterSensorErrorCount();
    //如果已经打开了
        if(LIQUID_AUTO_STATE_ENABLE == ServiceStateReadLiquidAutoPureWaterState())
        {
            return LH_ERR_NONE;
        }
    //更新纯水传感器
        ServiceImplLiquidAutoPureWaterSupplementReflushSensor();
    //检查传感器状态
        errorCode = ServiceImplLiquidAutoPureWaterSupplementCheckSensor();
        if(errorCode != LH_ERR_NONE)
        {
            return errorCode;
        }
    //打开纯水补充设置初始阶段
        ServiceStateWriteLiquidAutoPureWaterState(LIQUID_AUTO_STATE_ENABLE);
        ServiceStateWriteLiquidAutoPureWaterStage(PURE_WATER_SUPPLEMENT_NORMAL_IDLE);
    //累计时间清零
        pureWaterSupplementCountLow2Full = 0;
        pureWaterSupplementCountEmpty2Low = 0;
    //关闭纯水补液泵
        ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_DP3,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
    //等待稳定
        CoreDelayMs(30);
    //关闭纯水进水阀
        ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_V241,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
    return LH_ERR_NONE;
}

//关闭自动纯水补充
LH_ERR ServiceImplLiquidAutoPureWaterSupplementDisable(void)
{
    //初始化无错误
        CAN1_REPORT_ERR_CODE commErrorCode;
        CAN1_REPORT_ERR_LEVEL commErrorLevel;
    //清空纯水异常检测计数值
        ServiceStateClearPureWaterSensorErrorCount();
    //关闭纯水补充设置初始阶段
        ServiceStateWriteLiquidAutoPureWaterState(LIQUID_AUTO_STATE_DISABLE);
        ServiceStateWriteLiquidAutoPureWaterStage(PURE_WATER_SUPPLEMENT_NORMAL_IDLE);
    //关闭纯水补液泵
        ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_DP3,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
    //等待稳定
        CoreDelayMs(30);
    //关闭纯水进水阀
        ServiceImplLiquidAuto_ModuleOutputWrite(LIQUID_AUTO_PURE_WATER_V241,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
    //累计时间清零
        pureWaterSupplementCountLow2Full = 0;
        pureWaterSupplementCountEmpty2Low = 0;
    return LH_ERR_NONE;
}


