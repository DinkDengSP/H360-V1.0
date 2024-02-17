/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-13 17:57:05 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-25 14:03:14 +0800
************************************************************************************************/ 
#include "MCU_CAN1.h"

#if(MCU_CAN1_STD_FILTER_NBR_EXIST != 0)
//CAN1标准帧过滤器配置
const CAN_STD_FILTER can1StdFilterArray[] = {
    //过滤00
    {
        FDCAN_FILTER_MASK,
        FDCAN_FILTER_TO_RXFIFO0,
        0X00000000,
        0X00000001,
    },
    //过滤01
    {
        FDCAN_FILTER_MASK,
        FDCAN_FILTER_TO_RXFIFO1,
        0X00000001,
        0X00000001,
    },
};
#define LENGTH_CAN1_STD_FILTER_ARRAY        (sizeof(can1StdFilterArray)/sizeof(can1StdFilterArray[0]))
//供给外部调用的,过滤元素表长度
uint16_t MCU_Can1GetStdFilterElementCount(void)
{
    return LENGTH_CAN1_STD_FILTER_ARRAY;
}
#else
#define LENGTH_CAN1_STD_FILTER_ARRAY        0
#endif

#if(MCU_CAN1_EXT_FILTER_NBR_EXIST != 0)
//CAN1扩展帧过滤器
const CAN_EXTEND_FILTER can1ExtendFilterArray[] = {
    //过滤00
    {
        FDCAN_FILTER_MASK,
        FDCAN_FILTER_TO_RXFIFO0,
        0X00000001,
        0X0FFFFFFF,
    },
    //过滤01
    {
        FDCAN_FILTER_MASK,
        FDCAN_FILTER_TO_RXFIFO1,
        0X00000002,
        0X0FFFFFFF,
    },
};
#define LENGTH_CAN1_EXTENR_FILTER_ARRAY        (sizeof(can1ExtendFilterArray)/sizeof(can1ExtendFilterArray[0]))
//供给外部调用的,过滤元素表长度
uint16_t MCU_Can1GetExtendFilterElementCount(void)
{
    return LENGTH_CAN1_EXTENR_FILTER_ARRAY;
}
#else 
#define LENGTH_CAN1_EXTENR_FILTER_ARRAY         0
#endif
