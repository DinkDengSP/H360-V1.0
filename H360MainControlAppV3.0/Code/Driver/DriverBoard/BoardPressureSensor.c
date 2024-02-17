/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-19 09:09:36
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-19 16:47:52
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardPressureSensor.h"

//四个堵针检测分属于不同的针不同的模块,所以需要互斥信号量控制访问
static OS_MUTEX mutexWithBoardPressure;

//压力传感器检测接口初始化
void BoardPressureSensorInit(void)
{
    OS_ERR err;
    //创建互斥信号量
        OSMutexCreate((OS_MUTEX*	)&mutexWithBoardPressure,
                        (CPU_CHAR*	)"mutexWithBoardPressure",
                        (OS_ERR*		)&err);	
    //初始化ADC接口
        MCU_ADC_Init();
    //四个IO口初始化
        //PC0 ADC123 INP10 模拟输入
        MCU_PortInit(MCU_PIN_C_0,MCU_PORT_MODE_ANALOG,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_HIGH,0);
        //PC1 ADC123 INP11 模拟输入
        MCU_PortInit(MCU_PIN_C_1,MCU_PORT_MODE_ANALOG,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_HIGH,0);
        //PC2 ADC123 INP12 模拟输入
        MCU_PortInit(MCU_PIN_C_2,MCU_PORT_MODE_ANALOG,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_HIGH,0);
        //PC3 ADC123 INP13 模拟输入
        MCU_PortInit(MCU_PIN_C_3,MCU_PORT_MODE_ANALOG,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_HIGH,0);
}

//压力传感器读取当前值
uint16_t BoardPressureSensorReadValueSingle(BOARD_PRESSURE_CHANNEL channel)
{
    OS_ERR err;
    uint16_t resultValue = 0;
    //先申请信号量,阻塞线程
        do
        {
            OSMutexPend (&mutexWithBoardPressure,0,OS_OPT_PEND_BLOCKING,0,&err);
            if(err != OS_ERR_NONE)
            {
                CoreDelayMinTick();
            }
        }while(err != OS_ERR_NONE);
    //读取指定通道结果
        resultValue = MCU_ADC_ReadChannelValue(&ADC1_Handler,channel,1);
    //释放互斥信号量
        OSMutexPost(&mutexWithBoardPressure,OS_OPT_POST_NONE,&err);
    return resultValue;
}

//压力传感器读取多次值并平均
uint16_t BoardPressureSensorReadValueAverage(BOARD_PRESSURE_CHANNEL channel,uint16_t count)
{
    OS_ERR err;
    uint16_t resultValue = 0;
    //先申请信号量,阻塞线程
        do
        {
            //先申请信号量,阻塞线程
            OSMutexPend (&mutexWithBoardPressure,0,OS_OPT_PEND_BLOCKING,0,&err);
            if(err != OS_ERR_NONE)
            {
                CoreDelayMinTick();
            }
        }while(err != OS_ERR_NONE);
    //多次获取求平均值
        resultValue = MCU_ADC_ReadChannelValueAverage(&ADC1_Handler,channel,count,1);
    //释放互斥信号量
        OSMutexPost(&mutexWithBoardPressure,OS_OPT_POST_NONE,&err);
    return resultValue;
}
