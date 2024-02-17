#include "BoardElectricCurrentDetect.h"

//电流检测初始化
void BoardElectricCurrentDetectInit(void)
{
    MCU_ADC_Init();
}

//电流检测读取值,最小0,最大3300
uint16_t BoardElectricCurrentDetectReadChannel(CHANNEL_CURRENT_DETECT channel)
{
    if(channel == PLATE_REAGENT_1_CURRENT_DETECT)
    {
        return MCU_ADC_ReadChannelValueAverage(ADC_Channel_15,BOARD_ELECTRIC_CURRENT_DETECT_SUM_COUNT);
    }
    else if(channel == PLATE_REAGENT_2_CURRENT_DETECT)
    {
        return MCU_ADC_ReadChannelValueAverage(ADC_Channel_12,BOARD_ELECTRIC_CURRENT_DETECT_SUM_COUNT);
    }
    else if(channel == PLATE_REAGENT_3_CURRENT_DETECT)
    {
        return MCU_ADC_ReadChannelValueAverage(ADC_Channel_11,BOARD_ELECTRIC_CURRENT_DETECT_SUM_COUNT);
    }
    else if(channel == PLATE_REAGENT_4_CURRENT_DETECT)
    {
        return MCU_ADC_ReadChannelValueAverage(ADC_Channel_14,BOARD_ELECTRIC_CURRENT_DETECT_SUM_COUNT);
    }
    else if(channel == PLATE_REACTION_CURRENT_DETECT)
    {
        return MCU_ADC_ReadChannelValueAverage(ADC_Channel_13,BOARD_ELECTRIC_CURRENT_DETECT_SUM_COUNT);
    }
    else if(channel == PLATE_REAGENT_COOLER_CURRENT_DETECT)
    {
        return MCU_ADC_ReadChannelValueAverage(ADC_Channel_0,BOARD_ELECTRIC_CURRENT_DETECT_SUM_COUNT);
    }
    else
    {
        return MCU_ADC_ReadChannelValueAverage(ADC_Channel_10,BOARD_ELECTRIC_CURRENT_DETECT_SUM_COUNT);
    }
}
