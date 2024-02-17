#include "MainInclude.h"

int main(void)
{
    /*液面探测硬件端口初始化 */
        LiquidPortInit();
    /*帧延时初始化,毫秒延时 */
        DelayStampInit();
    /*IIC接口初始化 */
        IIC_Init();
    /*系统ADC接口初始化 */
        ADC_Init();
    /*打开系统中断 */
        __enable_interrupt();
    /*系统初始化的时候,探液信号有效,让探液灯亮起来,相当于展现初始化的标志 */
        LiquidPortOutSetValid();
    /*延时等待,系统电路等待稳定 */
        DelayStampDelayMs(1000);
    /*读取系统参数 */
        uint16_t systemUtilParam = LiquidGetParamResWithAD5272RefValue();
    /*写入电子电阻 */
        LiquidSetParamResWithAD5272RefValue(systemUtilParam);
    /*等待1000毫秒,等待电阻值生效 */
        DelayStampDelayMs(1000);
    /*关闭信号灯,表示初始化完成 */
        LiquidPortOutSetUnValid();
    /*串口初始化 */
        //UartInit(UART_BPS_250K,LiquidUartRecvIntCallBack);
        UartInit(UART_BPS_38400,LiquidUartRecvIntCallBack);
    /*系统外部指令系统初始化 */
        LiquidCmdProcInit();
    /*系统状态变量初始化 */
        LiquidDataProcInit();
    while(1)
    {
        /*更新传感器信息 */
            LiquidReflushSensorValue();
        /*处理系统接收到的指令集 */
            LiquidCommandProcess();
        /*数据上传是否打开的定义,决定了是否上传本次更新的探液数据 */
            if(liquidSensorUpdateFlag == 1)
            {
                //上传数据
                LiquidCmdSendSerialValue(AUTO_UPDATE_BASE_ADC,baseLiquidSensorValue);
                LiquidCmdSendSerialValue(AUTO_UPDATE_FOLLOW_ADC,followLiquidSensorValue);
                LiquidCmdSendSerialValue(AUTO_UPDATE_FILTER_ADC,filterFollowLiquidSensorValue);
            }
        /*如果液面探测使能,进行液面探测的处理 */
            if(liquidDetectEnableFlag != LIQUID_ENABLE_MODE_DISABLE)
            {
                //探液打开后，保存基准电压和针尖电压
                liquidDetectSaveMaxBaseAndNeedleVol();
                if(liquidSignalTrigFlag == 0)
                {
                    //处于未触发状态,计算
                    if(1 == LiquidDataProcessADConvertResult())
                    {
                        //触发探液
                        liquidSignalTrigFlag = 1;
                        //探液触发时间
                        liquidSignalTrigStamp = DelayStampGetStamp();
                        //设置探液有效
                        LiquidPortOutSetValid();
                        //如果打开了数据上传,上传探液触发信号
                        if(liquidSensorUpdateFlag == 1)
                        {
                            LiquidCmdSendSerialValue(AUTO_UPDATE_TRIG_FLAG,1);
                        }
                    }
                }
                else
                {
                    //已经处于探液触发状态
                    if(DelayStampGetStamp() - liquidSignalTrigStamp >= TIME_MS_SIGNAL_LIQUID_HOLD)
                    {
                        //超过信号保持时间,解除锁定
                        LiquidDataProcClear();
                        liquidSignalTrigFlag = 0;
                        liquidSignalTrigStamp = 0;
                        //设置无效
                        LiquidPortOutSetUnValid();
                        //如果打开了数据上传,上传解除探液触发信号
                        if(liquidSensorUpdateFlag == 1)
                        {
                            LiquidCmdSendSerialValue(AUTO_UPDATE_TRIG_FLAG,0);
                        }
                    }
                    else
                    {
                        //没有超过信号保持时间,保持锁定信号
                        LiquidPortOutSetValid();
                    }
                }
            }
        
        /*如果液面探测不使能,清除液面探测数据 */
            else
            {
                //清除探液数据
                LiquidDataProcClear();
                liquidSignalTrigFlag = 0;
                liquidSignalTrigStamp = 0;
                //设置无效
                LiquidPortOutSetUnValid();
            }
    }
}


