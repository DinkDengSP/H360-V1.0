/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-02-24 12:19:19
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-15 09:57:58
**FilePath: \H360_ActionBoard_ID_1\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplSerialUserRS1.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplSerialUserRS1.h"

//传感器触发次数
uint32_t liquidSensorCount = 0;
//传感器设定目标脉冲
uint32_t liquidSensorTargetCount = 0;
//传感器触发标志
uint8_t trigFlag = 0;

//传感器1回调处理信号
void LiquidSensorEventProcess()
{
    //中断计数值增加
    liquidSensorCount++;
    if((liquidSensorTargetCount != 0)&&(trigFlag == 0))
    {
        //不为0的时候说明要进行处理
        if(liquidSensorCount >= liquidSensorTargetCount)
        {
            //脉冲数和计数
            trigFlag = 1;
            //关闭指定的IO口,高电平失效,低电平有效
            BoardOutWrite(PUMP_AUTO_CLOSE_WHILE_SENSOR_TRIG,LEVEL_CLOSE_PUMP_AUTO_WHILE_SENSOR_TRIG);
        }
    }
}

//液路传感器液路注册
void LiquidSensorRegister(void)
{
    //设定变量初始化
    liquidSensorCount = 0;
    liquidSensorTargetCount = 0;
    trigFlag = 0;
    //注册中断
    BoardExtiRegister(LIQUID_AUTO_SENSOR_PORT,EXTI_Trigger_Rising,LiquidSensorEventProcess);
}

//调试串口输出一帧数据
void ServiceImplSerialUserRS1WriteBuffer(uint8_t* bufferStartPtr,uint16_t bufferLength)
{
    BoardRS_SendBuffer(BOARD_RS_PORT_1,bufferStartPtr,bufferLength);
}







