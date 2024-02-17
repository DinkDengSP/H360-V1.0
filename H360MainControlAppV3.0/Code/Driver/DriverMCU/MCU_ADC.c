/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-19 08:52:41
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-19 16:28:28
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "MCU_ADC.h"
#include "MCU_Port.h"

//ADC1句柄
ADC_HandleTypeDef ADC1_Handler;
//ADC3句柄
ADC_HandleTypeDef ADC3_Handler;

//初始化ADC
//ch: ADC_channels 
//通道值 0~16取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
void MCU_ADC_Init(void)
{
    ADC1_Handler.Instance=ADC1;
    //复位
    HAL_ADC_DeInit(&ADC1_Handler);
    //4分频，ADCCLK=PER_CK/4=64/4=16MHZ
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4; 	
    //16位模式
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_16B;        
    //非扫描模式   	
    ADC1_Handler.Init.ScanConvMode=DISABLE;                    
    //关闭EOC中断	
    ADC1_Handler.Init.EOCSelection=ADC_EOC_SINGLE_CONV;       
    //自动低功耗关闭	
	ADC1_Handler.Init.LowPowerAutoWait=DISABLE;						
    //关闭连续转换			
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;               
    //1个转换在规则序列中 也就是只转换规则序列1 
    ADC1_Handler.Init.NbrOfConversion=1;                        
    //禁止不连续采样模式
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;            
    //不连续采样通道数为0
    ADC1_Handler.Init.NbrOfDiscConversion=0;                    
    //软件触发
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;      
    //使用软件触发
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;					
    //有新的数据的死后直接覆盖掉旧数据
	ADC1_Handler.Init.Overrun=ADC_OVR_DATA_OVERWRITTEN;			
    //过采样关闭
	ADC1_Handler.Init.OversamplingMode=DISABLE;					
    //规则通道的数据仅仅保存在DR寄存器里面
	ADC1_Handler.Init.ConversionDataManagement=ADC_CONVERSIONDATA_DR; 
    //初始化 
    HAL_ADC_Init(&ADC1_Handler);
    //Boost模式打开
    ADC_ConfigureBoostMode(&ADC1_Handler);                                 
	//ADC校准
	HAL_ADCEx_Calibration_Start(&ADC1_Handler,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED);

    //ADC3初始化
    ADC3_Handler.Instance=ADC3;
    //复位
    HAL_ADC_DeInit(&ADC3_Handler);
    //4分频，ADCCLK=PER_CK/4=64/4=16MHZ
    ADC3_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4; 	
    //16位模式
    ADC3_Handler.Init.Resolution=ADC_RESOLUTION_16B;           	
    //非扫描模式
    ADC3_Handler.Init.ScanConvMode=DISABLE;                    	
    //关闭EOC中断
    ADC3_Handler.Init.EOCSelection=ADC_EOC_SINGLE_CONV;       	
    //自动低功耗关闭	
	ADC3_Handler.Init.LowPowerAutoWait=DISABLE;					
    //关闭连续转换			
    ADC3_Handler.Init.ContinuousConvMode=DISABLE;           
    //1个转换在规则序列中 也就是只转换规则序列1     
    ADC3_Handler.Init.NbrOfConversion=1;                       
    //禁止不连续采样模式 
    ADC3_Handler.Init.DiscontinuousConvMode=DISABLE;            
    //不连续采样通道数为0
    ADC3_Handler.Init.NbrOfDiscConversion=0;                
    //软件触发    
    ADC3_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;      
    //使用软件触发
    ADC3_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;						
    //有新的数据的死后直接覆盖掉旧数据
	ADC3_Handler.Init.Overrun=ADC_OVR_DATA_OVERWRITTEN;			
    //过采样关闭
	ADC3_Handler.Init.OversamplingMode=DISABLE;					
    //规则通道的数据仅仅保存在DR寄存器里面
	ADC3_Handler.Init.ConversionDataManagement=ADC_CONVERSIONDATA_DR;  
    //初始化
    HAL_ADC_Init(&ADC3_Handler);
    //Boost模式打开
    ADC_ConfigureBoostMode(&ADC3_Handler);                                   
	//ADC校准
	HAL_ADCEx_Calibration_Start(&ADC3_Handler,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED);  
}

//ADC底层驱动，引脚配置，时钟使能
//此函数会被HAL_ADC_Init()调用
//hadc:ADC句柄
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    RCC_PeriphCLKInitTypeDef  ADCClkInitStruct;
    //使能ADC1/2时钟
    __HAL_RCC_ADC12_CLK_ENABLE(); 
    //使能ADC3时钟
    __HAL_RCC_ADC3_CLK_ENABLE();			        

    ADCClkInitStruct.PeriphClockSelection=RCC_PERIPHCLK_ADC; 
	ADCClkInitStruct.AdcClockSelection=RCC_ADCCLKSOURCE_CLKP; 
	HAL_RCCEx_PeriphCLKConfig(&ADCClkInitStruct);
}

//ADC读取当前通道值
uint16_t MCU_ADC_ReadChannelValue(ADC_HandleTypeDef* ADCx,uint32_t channel,uint8_t convertVolFlag)
{
    uint16_t result = 0;
    float vol = 0.0;
    //转换配置
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    //通道
    ADC1_ChanConf.Channel=channel;   
    //1个序列                                
    ADC1_ChanConf.Rank=ADC_REGULAR_RANK_1;          
    //采样时间        	
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_810CYCLES_5;     
    //单边采集   	       
	ADC1_ChanConf.SingleDiff=ADC_SINGLE_ENDED;  				        		
	ADC1_ChanConf.OffsetNumber=ADC_OFFSET_NONE;             	
	ADC1_ChanConf.Offset=0;   
    //通道配置
    HAL_ADC_ConfigChannel(ADCx,&ADC1_ChanConf);        
    //开启ADC
    HAL_ADC_Start(ADCx);                               
	//轮询转换
    HAL_ADC_PollForConversion(ADCx,10);             
    //返回最近一次ADC1规则组的转换结果   
	result = (uint16_t)HAL_ADC_GetValue(ADCx);
    if(convertVolFlag == 1)
    {
        //计算成电压
        vol = result;
        vol /= 65535.0;
        vol *= 3300;
        result = (uint16_t)vol; 
    }	
    return result;         
}

//ADC读取当前通道值并取平均
uint16_t MCU_ADC_ReadChannelValueAverage(ADC_HandleTypeDef* ADCx,uint32_t channel,uint16_t count,uint8_t convertVolFlag)
{
    uint32_t resultAdd = 0;
    uint16_t index = 0;
    for(index = 0;index < count; index++)
    {
        resultAdd += MCU_ADC_ReadChannelValue(ADCx,channel,convertVolFlag);
    }
    return (uint16_t)(resultAdd/count);
}

//ADC读取当前芯片温度
float MCU_ADC_ReadChipTemp(void)
{
    uint32_t adcx;
 	double temperate;
    float temp=0;
    uint16_t ts_cal1,ts_cal2;
    
    ts_cal1=*(volatile uint16_t*)(0X1FF1E820);
    ts_cal2=*(volatile uint16_t*)(0X1FF1E840);
    temp=(float)((110.0f-30.0f)/(ts_cal2-ts_cal1));
    //读取内部温度传感器通道,10次取平均
	adcx=MCU_ADC_ReadChannelValueAverage(&ADC3_Handler,ADC_CHANNEL_TEMPSENSOR,10,0);
    temperate=(float)(temp*(adcx-ts_cal1)+30);				
	return (float)temperate;
}
