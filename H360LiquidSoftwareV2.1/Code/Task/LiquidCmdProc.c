#include "LiquidDetectDevice.h"
#include "LiquidParam.h"
#include "ResAD5272.h"
#include "LiquidCmdProc.h"
#include "LiquidDataProc.h"

//决定是否上传的标志
volatile uint8_t liquidSensorUpdateFlag = 0;
//决定液面探测是否使能的标志
volatile LIQUID_ENABLE_MODE liquidDetectEnableFlag = LIQUID_ENABLE_MODE_ENABLE_1;
//探液时最大基准电压
static uint16_t liquidDetectMaxBaseVol = 0;
//探液时最大针尖电压
static uint16_t liquidDetectMaxNeedleVol = 0;

/*发送数据的缓冲区 */
static uint8_t dataTransBuffer[5] = {0};
/*接收数据的缓冲区 */
uint8_t dataUartRecvBuf[3] = {0};
/*数据接收计数器 */
volatile uint8_t dataRecvCount = 0;

//串口中断回调函数
void LiquidUartRecvIntCallBack(uint8_t dat)
{
    if((dataRecvCount&0x80) != 0)
    {
        //有一整包数据未处理,无法接受新的数据
    }
    else if((dataRecvCount&0x40) != 0)
    {
        if(dat == 0x0a)
        {
            dataRecvCount |= 0x80;
        }
        else
        {
            dataRecvCount = 0x00;
        }
    }
    else
    {
        if(dataRecvCount < 3)
        {
            dataUartRecvBuf[dataRecvCount] = dat;
            dataRecvCount += 1;
        }
        else
        {
            if(dat == 0x0d)
            {
                dataRecvCount |= 0x40;
            }
            else
            {
                dataRecvCount = 0;
            }
            
        }
    }
}

//发送数据
void LiquidCmdSendSerialValue(uint8_t dataFlag,uint16_t value)
{
    //等待上一次数据发送完成
    UartWaitSendOver();
    dataTransBuffer[0] = dataFlag;
    dataTransBuffer[1] = (uint8_t)(value>>8);
    dataTransBuffer[2] = (uint8_t)(value);
    dataTransBuffer[3] = 0X0D;
    dataTransBuffer[4] = 0X0A;
    UartSendMsg(dataTransBuffer,5);
}

//发送校准结果
void LiquidCmdSendSerialAdjustResult(uint8_t result,uint16_t resValue)
{
    //等待上一次数据发送完成
    UartWaitSendOver();
    dataTransBuffer[0] = CMD_FUNC_ADJUST_BASE;
    dataTransBuffer[1] = result;
    dataTransBuffer[2] = (uint8_t)(resValue/10);
    dataTransBuffer[3] = 0X0D;
    dataTransBuffer[4] = 0X0A;
    UartSendMsg(dataTransBuffer,5);
}


//初始化指令系统
void LiquidCmdProcInit(void)
{
    liquidSensorUpdateFlag = 0;
    liquidDetectEnableFlag = LIQUID_ENABLE_MODE_ENABLE_1;
}

//自动校准
RESULT_ADJUST RES5272Adjust(uint16_t* adjustResResult)
{
    //一个最大值一个最小值,用于二分法
    uint16_t minRes = 1;//最小值,二分法下线
    uint16_t maxRes = 1023;//最大值,二分法上限
    uint16_t resCurrent;//当前值,二分法阈值选择
    //首先设置为1023,查看是否大于5V,如果不大于5V,说明状态不对
    ResAD5272Write(maxRes);
    //等待1S,等待写入的数据生效
    DelayStampDelayMs(1000);
    //刷新数据,查看结果
    LiquidReflushSensorValue();
    //检查结果
    if(baseLiquidSensorValue < 4000)
    {
        *adjustResResult = 0;
        return RESULT_ADJUST_MAX_NOT_4;
    }
    //然后设置为1,查看是否小于1V,如果此时还大于1V,说明状态不对
    ResAD5272Write(minRes);
    //等待1S,等待写入的数据生效
    DelayStampDelayMs(1000);
    //刷新数据,查看结果
    LiquidReflushSensorValue();
    //检查结果
    if(baseLiquidSensorValue > 1000)
    {
        *adjustResResult = 0;
        return RESULT_ADJUST_MIN_NOT_1;
    }
    //到这里,就可以开始查找
    do
    {
        resCurrent = ((maxRes + minRes)/2);
        //没找到,而且已经很贴近了
        if(maxRes - minRes <= 1)
        {
            *adjustResResult = 0;
            return RESULT_ADJUST_FAIL;
        }
        //调整一次
        ResAD5272Write(resCurrent);
        //等待1S,等待写入的数据生效
        DelayStampDelayMs(1000);
        //刷新数据,查看结果
        LiquidReflushSensorValue();
        //将数据发送出去
        LiquidCmdSendSerialValue(AUTO_UPDATE_ADJUST_VALUE,baseLiquidSensorValue);
        //区域校准2.4-2.6之间
        if(baseLiquidSensorValue >= 2600)
        {
            maxRes = resCurrent;
        }
        else if(baseLiquidSensorValue <= 2400)
        {
            minRes = resCurrent;
        }
        else
        {
            //校准完成
            *adjustResResult = resCurrent;
            return RESULT_ADJUST_OK;
        }
    }while(maxRes > minRes);

    return RESULT_ADJUST_FAIL;
}

//读取阈值
void LiquidCommandProcessReadThresholdValue(uint16_t recvData)
{
    if(recvData == 0)
    {
        LiquidCmdSendSerialValue(CMD_READ_THRESHOLD_VALUE,THRESHOLD_VALUE_LIMIT1);
    }
    else if(recvData == 1)
    {
        LiquidCmdSendSerialValue(CMD_READ_THRESHOLD_VALUE,THRESHOLD_VALUE_LIMIT2);
    }
    else if(recvData == 2)
    {
        LiquidCmdSendSerialValue(CMD_READ_THRESHOLD_VALUE,THRESHOLD_VALUE_LIMIT3);
    }
    else if(recvData == 3)
    {
        LiquidCmdSendSerialValue(CMD_READ_THRESHOLD_VALUE,THRESHOLD_VALUE_LIMIT4);
    }
    else if(recvData == 4)
    {
        LiquidCmdSendSerialValue(CMD_READ_THRESHOLD_VALUE,THRESHOLD_VALUE_LIMIT5);
    }
    else if(recvData == 5)
    {
        LiquidCmdSendSerialValue(CMD_READ_THRESHOLD_VALUE,THRESHOLD_VALUE_LIMIT6);
    }
    else if(recvData == 6)
    {
        LiquidCmdSendSerialValue(CMD_READ_THRESHOLD_VALUE,THRESHOLD_VALUE_LIMIT7);
    }
    else if(recvData == 7)
    {
        LiquidCmdSendSerialValue(CMD_READ_THRESHOLD_VALUE,THRESHOLD_VALUE_LIMIT8);
    }
    else if(recvData == 8)
    {
        LiquidCmdSendSerialValue(CMD_READ_THRESHOLD_VALUE,THRESHOLD_VALUE_LIMIT9);
    }
    else if(recvData == 9)
    {
        LiquidCmdSendSerialValue(CMD_READ_THRESHOLD_VALUE,THRESHOLD_VALUE_LIMIT10);
    }
    else
    {
        LiquidCmdSendSerialValue(CMD_READ_THRESHOLD_VALUE,THRESHOLD_VALUE_LIMIT1);
    }
}

//读取斜率
void LiquidCommandProcessReadSlopeValue(uint16_t recvData)
{
    if(recvData == 0)
    {
        LiquidCmdSendSerialValue(CMD_READ_SLOPE_VALUE,SLOPE_VALUE_LIMIT1);
    }
    else if(recvData == 1)
    {
        LiquidCmdSendSerialValue(CMD_READ_SLOPE_VALUE,SLOPE_VALUE_LIMIT2);
    }
    else if(recvData == 2)
    {
        LiquidCmdSendSerialValue(CMD_READ_SLOPE_VALUE,SLOPE_VALUE_LIMIT3);
    }
    else if(recvData == 3)
    {
        LiquidCmdSendSerialValue(CMD_READ_SLOPE_VALUE,SLOPE_VALUE_LIMIT4);
    }
    else if(recvData == 4)
    {
        LiquidCmdSendSerialValue(CMD_READ_SLOPE_VALUE,SLOPE_VALUE_LIMIT5);
    }
    else if(recvData == 5)
    {
        LiquidCmdSendSerialValue(CMD_READ_SLOPE_VALUE,SLOPE_VALUE_LIMIT6);
    }
    else if(recvData == 6)
    {
        LiquidCmdSendSerialValue(CMD_READ_SLOPE_VALUE,SLOPE_VALUE_LIMIT7);
    }
    else if(recvData == 7)
    {
        LiquidCmdSendSerialValue(CMD_READ_SLOPE_VALUE,SLOPE_VALUE_LIMIT8);
    }
    else if(recvData == 8)
    {
        LiquidCmdSendSerialValue(CMD_READ_SLOPE_VALUE,SLOPE_VALUE_LIMIT9);
    }
    else if(recvData == 9)
    {
        LiquidCmdSendSerialValue(CMD_READ_SLOPE_VALUE,SLOPE_VALUE_LIMIT10);
    }
    else
    {
        LiquidCmdSendSerialValue(CMD_READ_SLOPE_VALUE,SLOPE_VALUE_LIMIT1);
    }
}

//读取针尖电压
void LiquidCommandProcessReadNeedleCurrentVol(uint16_t recvDat)
{
    /*更新传感器信息 */
    LiquidReflushSensorValue();
    LiquidCmdSendSerialValue(LCMD_FUNC_READ_NEEDLE_CURRENT_VOL,filterFollowLiquidSensorValue);
}

//读取基准电压
void LiquidCommandProcessReadBaseVol(uint16_t recvDat)
{
    /*更新传感器信息 */
    LiquidReflushSensorValue();
    LiquidCmdSendSerialValue(CMD_FUNC_READ_NEEDLE_BASE_VOL,baseLiquidSensorValue);
}


//写入电子电阻值
void LiquidCommandProcessWriteResValud(uint16_t resValue)
{
    LiquidSetParamResWithAD5272RefValue(resValue);
    DelayStampDelayMs(1000);
    LiquidCmdSendSerialValue(CMD_WRITE_AD5272_RES,LiquidGetParamResWithAD5272RefValue());
}

//开关数据上传
void LiquidCommandProcessDataUpdateOpenClose(uint16_t option)
{
    if(option == 0)
    {
        liquidSensorUpdateFlag = 0;//关闭上传
    }
    else
    {
        liquidSensorUpdateFlag = 1;//打开上传
    }
    LiquidCmdSendSerialValue(CMD_FUNC_SWITCH_LIQUID_DATA_UPDATE,liquidSensorUpdateFlag);
}

//开关液面探测功能
void LiquidCommandProcessEnableDisableLiquidFunc(uint16_t enableOption)
{   
    if(enableOption == 0)
    {
        //关闭探液
        liquidDetectEnableFlag = LIQUID_ENABLE_MODE_DISABLE;
        //回传关闭状态
        LiquidCmdSendSerialValue(CMD_FUNC_SWITCH_LIQUID_ENABLE,liquidDetectEnableFlag);
    }
    else
    {
        //如果是从关闭探液到打开探液,那么必须等待探液信号降低到阈值以下才能退出
        //这是为了在开探液的时候保证不满足探液条件,这样才能触发探液
        if(liquidDetectEnableFlag == LIQUID_ENABLE_MODE_DISABLE)
        {
            //设置无效
                LiquidPortOutSetUnValid();
                LiquidDataProcClear();
            //关闭触发和计数器
                liquidSignalTrigFlag = 0;
                liquidSignalTrigStamp = 0;
            //清除记录的基准电压和针尖电压
                liquidDetectMaxBaseVol = 0;
                liquidDetectMaxNeedleVol = 0;
            //循环读取液面探测信号
                do
                {
                    //循环检测信号,直到信号小于特定值
                    LiquidReflushSensorValue();
                    //延时一定时间
                    DelayStampDelayMs(5);
                    //计数器增加
                    liquidSignalTrigStamp++;
                    //超时设置为100
                    if(liquidSignalTrigStamp > 400)
                    {
                        break;
                    }
                }while(filterFollowLiquidSensorValue > FILTER_LIMIT_LOW);
            //超过400次,也就是将近2S
            if(liquidSignalTrigStamp > 400)
            {
                //探液无法打开
                liquidDetectEnableFlag = LIQUID_ENABLE_MODE_DISABLE;
                //回传状态
                LiquidCmdSendSerialValue(CMD_FUNC_SWITCH_LIQUID_ENABLE,liquidDetectEnableFlag);
            }
            else
            {
                //打开探液成功,使用设置的模式打开
                if(enableOption > (uint8_t)(LIQUID_ENABLE_MODE_ENABLE_10))
                {
                    liquidDetectEnableFlag = LIQUID_ENABLE_MODE_ENABLE_10;
                }
                else
                {
                    liquidDetectEnableFlag = (LIQUID_ENABLE_MODE)enableOption;
                }
                //基准电压检测
                liquidSignalTrigStamp = 0;
                //检测基准电压,如果基准电压不对,一样进行检测
                    do
                    {
                        //循环检测信号,直到信号小于特定值
                        LiquidReflushSensorValue();
                        //延时一定时间
                        DelayStampDelayMs(5);
                        //计数器增加
                        liquidSignalTrigStamp++;
                        //超时设置为100
                        if(liquidSignalTrigStamp > 400)
                        {
                            break;
                        }
                    }while((baseLiquidSensorValue >= BASE_VOLTAGE_HIGH_LIMIT)||(baseLiquidSensorValue <= BASE_VOLTAGE_LOW_LIMIT));
                //尝试2S都无法回到正常
                if(liquidSignalTrigStamp > 400)
                {
                    //计数器清空
                    liquidSignalTrigStamp = 0;
                    //根据基准电压状态决定探液结果
                    if(baseLiquidSensorValue >= 5000)
                    {
                        //不同的基准电压代表不同的结果
                        LiquidCmdSendSerialValue(CMD_FUNC_SWITCH_LIQUID_ENABLE,LIQUID_DETECT_CHECK_BENCHMARK_50V);
                    }
                    else if(baseLiquidSensorValue >= 4500)
                    {
                        //不同的基准电压代表不同的结果
                        LiquidCmdSendSerialValue(CMD_FUNC_SWITCH_LIQUID_ENABLE,LIQUID_DETECT_CHECK_BENCHMARK_45_50V);
                    }
                    else if(baseLiquidSensorValue >= 4000)
                    {
                        //不同的基准电压代表不同的结果
                        LiquidCmdSendSerialValue(CMD_FUNC_SWITCH_LIQUID_ENABLE,LIQUID_DETECT_CHECK_BENCHMARK_40_45V);
                    }
                    else if(baseLiquidSensorValue >= 3500)
                    {
                        //不同的基准电压代表不同的结果
                        LiquidCmdSendSerialValue(CMD_FUNC_SWITCH_LIQUID_ENABLE,LIQUID_DETECT_CHECK_BENCHMARK_35_40V);
                    }
                    else if(baseLiquidSensorValue >= 3000)
                    {
                        //不同的基准电压代表不同的结果
                        LiquidCmdSendSerialValue(CMD_FUNC_SWITCH_LIQUID_ENABLE,LIQUID_DETECT_CHECK_BENCHMARK_30_35V);
                    }
                    else if(baseLiquidSensorValue >= 2900)
                    {
                        //不同的基准电压代表不同的结果
                        LiquidCmdSendSerialValue(CMD_FUNC_SWITCH_LIQUID_ENABLE,LIQUID_DETECT_CHECK_BENCHMARK_29_30V);
                    }
                    else if(baseLiquidSensorValue <= 0)
                    {
                        //不同的基准电压代表不同的结果
                        LiquidCmdSendSerialValue(CMD_FUNC_SWITCH_LIQUID_ENABLE,LIQUID_DETECT_CHECK_BENCHMARK_0V);
                    }
                    else if(baseLiquidSensorValue <= 500)
                    {
                        //不同的基准电压代表不同的结果
                        LiquidCmdSendSerialValue(CMD_FUNC_SWITCH_LIQUID_ENABLE,LIQUID_DETECT_CHECK_BENCHMARK_0_5V);
                    }
                    else if(baseLiquidSensorValue <= 1000)
                    {
                        //不同的基准电压代表不同的结果
                        LiquidCmdSendSerialValue(CMD_FUNC_SWITCH_LIQUID_ENABLE,LIQUID_DETECT_CHECK_BENCHMARK_5_10V);
                    }
                    else if(baseLiquidSensorValue <= 1500)
                    {
                        //不同的基准电压代表不同的结果
                        LiquidCmdSendSerialValue(CMD_FUNC_SWITCH_LIQUID_ENABLE,LIQUID_DETECT_CHECK_BENCHMARK_10_15V);
                    }
                    else if(baseLiquidSensorValue <= 2000)
                    {
                        //不同的基准电压代表不同的结果
                        LiquidCmdSendSerialValue(CMD_FUNC_SWITCH_LIQUID_ENABLE,LIQUID_DETECT_CHECK_BENCHMARK_15_20V);
                    }
                    else if(baseLiquidSensorValue <= 2100)
                    {
                        //不同的基准电压代表不同的结果
                        LiquidCmdSendSerialValue(CMD_FUNC_SWITCH_LIQUID_ENABLE,LIQUID_DETECT_CHECK_BENCHMARK_20_21V);
                    }
                }
                else
                {
                    //计数器清空
                    liquidSignalTrigStamp = 0;
                    //回传状态
                    LiquidCmdSendSerialValue(CMD_FUNC_SWITCH_LIQUID_ENABLE,liquidDetectEnableFlag);
                }
            }
        }
        else//已经打开探液的情况下,修改打开模式
        {
            if(enableOption > (uint8_t)(LIQUID_ENABLE_MODE_ENABLE_10))
            {
                liquidDetectEnableFlag = LIQUID_ENABLE_MODE_ENABLE_10;
            }
            else
            {
                liquidDetectEnableFlag = (LIQUID_ENABLE_MODE)enableOption;
            }
            //计数器清空
            liquidSignalTrigStamp = 0;
            //回传已经打开
            LiquidCmdSendSerialValue(CMD_FUNC_SWITCH_LIQUID_ENABLE,liquidDetectEnableFlag);
        }
    }
}

//处理系统指令集
void LiquidCommandProcess(void)
{
    uint16_t recvData = 0;
    RESULT_ADJUST adjustResult = RESULT_ADJUST_FAIL;
    uint16_t adjustResValue = 0;
    //接收到数据
    if((dataRecvCount&0x80) == 0)
    {
        return;
    }
    recvData = dataUartRecvBuf[1];
    recvData <<= 8;
    recvData += dataUartRecvBuf[2];
    switch(dataUartRecvBuf[0])
    {
        case CMD_READ_THRESHOLD_VALUE://读取阈值
            LiquidCommandProcessReadThresholdValue(recvData);
            break;
        case CMD_READ_THRESHOLD_COUNT_LIMIT://读取检测次数
            LiquidCmdSendSerialValue(CMD_READ_THRESHOLD_COUNT_LIMIT,THRESHOLD_TRIG_COUNT_LIMIT);
            break;
        case CMD_READ_SLOPE_VALUE://读取斜率
            LiquidCommandProcessReadSlopeValue(recvData);
            break;
        case CMD_READ_SLOPE_COUNT_LIMIT://读取斜率计数
            LiquidCmdSendSerialValue(CMD_READ_SLOPE_COUNT_LIMIT,SLOPE_TRIG_COUNT_LIMIT);
            break;
        case LCMD_FUNC_READ_NEEDLE_CURRENT_VOL://读取针尖电压
            LiquidCommandProcessReadNeedleCurrentVol(recvData);
            break;
        case CMD_FUNC_READ_NEEDLE_BASE_VOL://读取基准电压
            LiquidCommandProcessReadBaseVol(recvData);
            break;
        case CMD_READ_SIGNAL_HOLD_MS:
            //读取信号保持时间
            LiquidCmdSendSerialValue(CMD_READ_SIGNAL_HOLD_MS,TIME_MS_SIGNAL_LIQUID_HOLD);
            break;
        case CMD_READ_AD5272_RES:
            //读取滑动变阻器当前值
            LiquidCmdSendSerialValue(CMD_READ_AD5272_RES,LiquidGetParamResWithAD5272RefValue());
            break;
        case CMD_READ_LIQUID_MAX_BASE_VOL:
            //读取探液时最大基准电压
            LiquidCmdSendSerialValue(CMD_READ_LIQUID_MAX_BASE_VOL,liquidDetectMaxBaseVol);
            break;
        case CMD_READ_LIQUID_MAX_NEEDLE_VOL:
            //读取探液时最大针尖电压
            LiquidCmdSendSerialValue(CMD_READ_LIQUID_MAX_NEEDLE_VOL,liquidDetectMaxNeedleVol);
            break;
        case CMD_WRITE_AD5272_RES:
            //写入滑动变阻器当前值
            LiquidCommandProcessWriteResValud(recvData);
            break;
        case CMD_FUNC_ADJUST_BASE:
            //校准先关闭数据上传
            liquidSensorUpdateFlag = 0;//关闭上传
            //数据校准
            adjustResult = RES5272Adjust(&adjustResValue);
            if(adjustResult == RESULT_ADJUST_OK)
            {
                //将数据写入EEPROM
                LiquidSetParamResWithAD5272RefValue(adjustResValue);
            }
            //将数据结果返回
            LiquidCmdSendSerialAdjustResult((uint8_t)adjustResult,adjustResValue);
            break;
        case CMD_FUNC_SWITCH_LIQUID_DATA_UPDATE:
            //开关数据上传
            LiquidCommandProcessDataUpdateOpenClose(recvData);
            break;
        case CMD_FUNC_SWITCH_LIQUID_ENABLE:
            //开关探液,在探液关闭的情况下想要打开探液,需要等待探液触发条件消失才能打开探液,否则回传探液打不开
            LiquidCommandProcessEnableDisableLiquidFunc(recvData);
            break;
        case CMD_FUNC_READ_LIQUID_BOARD_VERSION:
            //返回探液板版本
            LiquidCmdSendSerialValue(CMD_FUNC_READ_LIQUID_BOARD_VERSION,LIQUID_BOARD_MAIN_VERSION);
            break;
        default:
            break;
    }
    dataRecvCount = 0x00;
}

//保存打开探液过程中最大的基准电压和针尖电压
void liquidDetectSaveMaxBaseAndNeedleVol(void)
{
    //保存基准电压
    if(liquidDetectMaxBaseVol < baseLiquidSensorValue)
    {
        liquidDetectMaxBaseVol = baseLiquidSensorValue;
    }
    //保存针尖电压
    if(liquidDetectMaxNeedleVol < filterFollowLiquidSensorValue)
    {
        liquidDetectMaxNeedleVol = filterFollowLiquidSensorValue;
    }

}
