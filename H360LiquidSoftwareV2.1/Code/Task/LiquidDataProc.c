#include "LiquidDataProc.h"
#include "LiquidParam.h"
#include "LiquidCmdProc.h"

//传感器数据
//基准传感器数据
uint16_t baseLiquidSensorValue = 0;
//跟随传感器数据
uint16_t followLiquidSensorValue = 0;
//跟随传感器滤波数据
uint16_t filterFollowLiquidSensorValue = 0;

//当前斜率基准值,可变的,运行时计算
static uint16_t currentSlopeBaseValue;
//当前斜率计数值
static uint16_t currentSlopeCount;
//阈值计数器,主要用于阈值检测,多次检测到大于指定阈值,表明探液触发
static uint16_t liquidThresholdFlagCount;

//液面探测标志位
uint8_t liquidSignalTrigFlag = 0;
//液面探测触发事件
uint32_t liquidSignalTrigStamp = 0;

//更新传感器数据
void LiquidReflushSensorValue(void)
{
    //读取基准值
    baseLiquidSensorValue = ADC_ReadChannel(ADC_CHANNEL_BASE);
    //读取目标数据
    followLiquidSensorValue = ADC_ReadChannel(ADC_CHANNEL_TARGET);
    //当跟随电压大于滤波下限,才有滤波值,否则,滤波值为0
    if(followLiquidSensorValue < FILTER_LIMIT_LOW)
    {
        filterFollowLiquidSensorValue = 0;
    }
    else
    {
        //滤波算法
        if(filterFollowLiquidSensorValue == 0)
        {
            //没有滤波值的情况下,滤波值就是计算值
            filterFollowLiquidSensorValue = followLiquidSensorValue;
        }
        else
        {
            //有滤波值的情况下,滑动滤波
            filterFollowLiquidSensorValue = (filterFollowLiquidSensorValue + followLiquidSensorValue)/2;
        }
    }
}

//液面探测数据处理相关的变量初始化
void LiquidDataProcInit(void)
{
    //数据保存相关状态的处理
    baseLiquidSensorValue = 0;
    followLiquidSensorValue = 0;
    filterFollowLiquidSensorValue = 0;
    //液面探测过程数据相关信号的处理
    currentSlopeBaseValue = 0;
    currentSlopeCount = 0;
    liquidThresholdFlagCount = 0;
}

//液面探测相关数据的清除
void LiquidDataProcClear(void)
{
    //数据保存相关状态的处理
    baseLiquidSensorValue = 0;
    followLiquidSensorValue = 0;
    filterFollowLiquidSensorValue = 0;
    //液面探测过程数据相关信号的处理
    currentSlopeBaseValue = 0;
    currentSlopeCount = 0;
    liquidThresholdFlagCount = 0;
}


//阈值计算
static uint8_t LiquidSensorProcessWithThreshold(void)
{
    uint16_t thresholdValueCurrent = 0;
    //选择特定阈值计算
    switch(liquidDetectEnableFlag)
    {
        case LIQUID_ENABLE_MODE_ENABLE_1:
            thresholdValueCurrent = THRESHOLD_VALUE_LIMIT1;
            break;
        case LIQUID_ENABLE_MODE_ENABLE_2:
            thresholdValueCurrent = THRESHOLD_VALUE_LIMIT2;
            break;
        case LIQUID_ENABLE_MODE_ENABLE_3:
            thresholdValueCurrent = THRESHOLD_VALUE_LIMIT3;
            break;
        case LIQUID_ENABLE_MODE_ENABLE_4:
            thresholdValueCurrent = THRESHOLD_VALUE_LIMIT4;
            break;
        case LIQUID_ENABLE_MODE_ENABLE_5:
            thresholdValueCurrent = THRESHOLD_VALUE_LIMIT5;
            break;
        case LIQUID_ENABLE_MODE_ENABLE_6:
            thresholdValueCurrent = THRESHOLD_VALUE_LIMIT6;
            break;
        case LIQUID_ENABLE_MODE_ENABLE_7:
            thresholdValueCurrent = THRESHOLD_VALUE_LIMIT7;
            break;
        case LIQUID_ENABLE_MODE_ENABLE_8:
            thresholdValueCurrent = THRESHOLD_VALUE_LIMIT8;
            break;  
        case LIQUID_ENABLE_MODE_ENABLE_9:
            thresholdValueCurrent = THRESHOLD_VALUE_LIMIT9;
            break;
        case LIQUID_ENABLE_MODE_ENABLE_10:
            thresholdValueCurrent = THRESHOLD_VALUE_LIMIT10;
            break;
        case LIQUID_ENABLE_MODE_DISABLE:
            thresholdValueCurrent = 5000;
            break;
        default:
            thresholdValueCurrent = 5000;
            break;
    }
    //阈值计数计算
    if(liquidThresholdFlagCount == 0)
    {
        if(filterFollowLiquidSensorValue  >= thresholdValueCurrent)
        {
            liquidThresholdFlagCount++;
        }
    }
    else
    {
        //大于阈值,继续计数
        if(filterFollowLiquidSensorValue >= thresholdValueCurrent)
        {
            liquidThresholdFlagCount++;
        }
        //小于阈值的100,清零
        else if(filterFollowLiquidSensorValue < (thresholdValueCurrent-100))
        {
            liquidThresholdFlagCount = 0;
        }
        else
        {
            //暂时不处理
        }
    }
    //对计数进行判定
    if(liquidThresholdFlagCount >= THRESHOLD_TRIG_COUNT_LIMIT)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}


//斜率计算
static uint8_t LiquidSensorProcessWithSlope(void)
{
    uint16_t currentSlopeLimit = 0;
    //选择特定斜率计算
    switch(liquidDetectEnableFlag)
    {
        case LIQUID_ENABLE_MODE_ENABLE_1:
            currentSlopeLimit = SLOPE_VALUE_LIMIT1;
            break;
        case LIQUID_ENABLE_MODE_ENABLE_2:
            currentSlopeLimit = SLOPE_VALUE_LIMIT2;
            break;
        case LIQUID_ENABLE_MODE_ENABLE_3:
            currentSlopeLimit = SLOPE_VALUE_LIMIT3;
            break;
        case LIQUID_ENABLE_MODE_ENABLE_4:
            currentSlopeLimit = SLOPE_VALUE_LIMIT4;
            break;
        case LIQUID_ENABLE_MODE_ENABLE_5:
            currentSlopeLimit = SLOPE_VALUE_LIMIT5;
            break;
        case LIQUID_ENABLE_MODE_ENABLE_6:
            currentSlopeLimit = SLOPE_VALUE_LIMIT6;
            break;
        case LIQUID_ENABLE_MODE_ENABLE_7:
            currentSlopeLimit = SLOPE_VALUE_LIMIT7;
            break;
        case LIQUID_ENABLE_MODE_ENABLE_8:
            currentSlopeLimit = SLOPE_VALUE_LIMIT8;
            break;  
        case LIQUID_ENABLE_MODE_ENABLE_9:
            currentSlopeLimit = SLOPE_VALUE_LIMIT9;
            break;
        case LIQUID_ENABLE_MODE_ENABLE_10:
            currentSlopeLimit = SLOPE_VALUE_LIMIT10;
            break;
        case LIQUID_ENABLE_MODE_DISABLE:
            currentSlopeLimit = 5000;
            break;
        default:
            currentSlopeLimit = 5000;
            break;
    }
    if(currentSlopeCount == 0)
    {
        if(filterFollowLiquidSensorValue > currentSlopeLimit)
        {
            currentSlopeCount = 1;
            currentSlopeBaseValue = filterFollowLiquidSensorValue;
        }
    }
    else
    {
        //数据开始下降
        if(filterFollowLiquidSensorValue < currentSlopeBaseValue)
        {
            //只有数据降低状态大于一定值的时候,才能算是清除,否则,这个数据就先不管
            //防止一些小的变化影响
            if((currentSlopeBaseValue - filterFollowLiquidSensorValue) > 100)
            {
                //清除斜率计数
                currentSlopeCount = 0;
                currentSlopeBaseValue = 0;
            }
            else
            {
                //这里先不管
            }
        }
        else
        {
            if((filterFollowLiquidSensorValue - currentSlopeBaseValue) >= currentSlopeLimit)
            {
                currentSlopeCount++;
                currentSlopeBaseValue = filterFollowLiquidSensorValue;
            }
        }
    }
    if(currentSlopeCount >= SLOPE_TRIG_COUNT_LIMIT)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//计算探液结果
uint8_t LiquidDataProcessADConvertResult(void)
{
    uint8_t thresholdResult;
    uint8_t slopeResult;
    //首先进行阈值计算
    thresholdResult = LiquidSensorProcessWithThreshold();
    if(thresholdResult == 1)
    {
        return 1;
    }
    //然后进行斜率计算
    slopeResult = LiquidSensorProcessWithSlope();
    if(slopeResult == 1)
    {
        return 1;
    }
    //斜率和阈值都不通过,探液未触发
    return 0;
}
