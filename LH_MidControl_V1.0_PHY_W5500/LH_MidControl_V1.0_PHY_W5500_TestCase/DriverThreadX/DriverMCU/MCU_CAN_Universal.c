/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:03:25 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:35:17 +0800
************************************************************************************************/ 
#include "MCU_CAN_Universal.h"
#include "LowLevelShow.h"
#include "MCU_CAN1.h"
#include "MCU_CAN2.h"

//CAN总线在不同的速率下的配置
//brp :波特率分频器.范围:1~511; tq = brp/PCANCLK = brp/PLLQ
//tsjw:重新同步跳跃时间单元.默认为1,取值范围1-127,计算默认取1
//tbs1:时间段1的时间单元.取值1-255
//tbs2:时间段2的时间单元,取值1-127
//波特率 = PLLQ/((tbs1+tbs2+1)*brp);
//根据CIA官方建议,(SJW+BS1)/(SJW+BS1+BS2)
// 速度 > 800K以上时候,采样点 75%  
// 速度 > 500K,采样点 80%
// 速度 <= 500K,采样点 87.5%
/*tqWithSJW*//*tqWithBS1*//*tqWithBS2*//*brpDiv*/
//位时间最大25个时间片,最小4个时间片,不能为了匹配采样点随便搞
/*----------------------------CAN仲裁段时序----------------------------------*/
#if(CORE_CLOCK_MODE_USER == CORE_CLOCK_MODE_480M)
//120M下的时钟配置 24时间片
const MCU_CAN_BPS_SETTING MCU_CAN_ArbitrationBaudArray[5] = {
    {6  /*1-128*/, 17  /*2-256*/, 6  /*2-128*/,   5   /*1-512*/   },//1000K 75%
    {5  /*1-128*/, 18  /*2-256*/, 5  /*2-128*/,   10  /*1-512*/   },//500K 79.1%
    {3  /*1-128*/, 20  /*2-256*/, 3  /*2-128*/,   20  /*1-512*/   },//250K 87.5%
    {3  /*1-128*/, 20  /*2-256*/, 3  /*2-128*/,   40  /*1-512*/   },//125K 87.5%
    {3  /*1-128*/, 20  /*2-256*/, 3  /*2-128*/,   100  /*1-512*/   },//50K 87.5%
}; 
#elif(CORE_CLOCK_MODE_USER == CORE_CLOCK_MODE_400M)
//100下的时钟配置
const MCU_CAN_BPS_SETTING MCU_CAN_ArbitrationBaudArray[5] = {
    {7  /*1-128*/, 17  /*2-256*/, 7  /*2-128*/,    4   /*1-512*/  },//1000K 72%
    {5  /*1-128*/, 19  /*2-256*/, 5  /*2-128*/,    8   /*1-512*/  },//500K 80%
    {3  /*1-128*/, 21  /*2-256*/, 3  /*2-128*/,    16  /*1-512*/  },//250K 88%
    {3  /*1-128*/, 21  /*2-256*/, 3  /*2-128*/,    32  /*1-512*/  },//125K 88%
    {3  /*1-128*/, 21  /*2-256*/, 3  /*2-128*/,    80  /*1-512*/  },//50K 88%
};
#endif

#if(CORE_CLOCK_MODE_USER == CORE_CLOCK_MODE_480M)
//120M下的时钟配置
const MCU_CAN_BPS_SETTING MCU_CAN_DataBaudArray[] = {
    {3  /*1-16*/,  8   /*1-32*/,  3  /*1-16*/,    2   /*1-32*/  },//5000K 75%
    {4  /*1-16*/,  15  /*1-32*/,  4  /*1-16*/,    3   /*1-32*/  },//2000K 80%
    {6  /*1-128*/, 17  /*2-256*/, 6  /*2-128*/,   5   /*1-512*/ },//1000K 75%
    {5  /*1-128*/, 18  /*2-256*/, 5  /*2-128*/,   10  /*1-512*/ },//500K 79.1%
    {3  /*1-128*/, 20  /*2-256*/, 3  /*2-128*/,   20  /*1-512*/ },//250K 87.5%
    {3  /*1-128*/, 20  /*2-256*/, 3  /*2-128*/,   40  /*1-512*/ },//125K 87.5%
    {3  /*1-128*/, 20  /*2-256*/, 3  /*2-128*/,   100 /*1-512*/ },//50K 87.5%
};
#elif(CORE_CLOCK_MODE_USER == CORE_CLOCK_MODE_400M)
//100M下的时钟配置
const MCU_CAN_BPS_SETTING MCU_CAN_DataBaudArray[] = {
    {3  /*1-16*/,  6   /*1-32*/,  3  /*1-16*/,    2   /*1-32*/  },//5000K 70%
    {6  /*1-16*/,  18  /*1-32*/,  6  /*1-16*/,    2   /*1-32*/  },//2000K 76%
    {7  /*1-128*/, 17  /*2-256*/, 7  /*2-128*/,   4   /*1-512*/ },//1000K 72%
    {5  /*1-128*/, 19  /*2-256*/, 5  /*2-128*/,   8   /*1-512*/ },//500K 80%
    {3  /*1-128*/, 21  /*2-256*/, 3  /*2-128*/,   16  /*1-512*/ },//250K 88%
    {3  /*1-128*/, 21  /*2-256*/, 3  /*2-128*/,   32  /*1-512*/ },//125K 88%
    {3  /*1-128*/, 21  /*2-256*/, 3  /*2-128*/,   80  /*1-512*/ },//50K 88%
};
#endif

//检验发送长度并转换为HAL库可以识别的字长
ERROR_SUB MCU_CAN_ConvertLengthToBytes(CAN_PACK_SRC packSrc,uint8_t length,uint32_t* dataLen)
{
    *dataLen = FDCAN_DLC_BYTES_0;
    switch(length)
    {
        case 0:
            *dataLen = FDCAN_DLC_BYTES_0;
            return ERROR_SUB_OK;
        case 1:
            *dataLen = FDCAN_DLC_BYTES_1;
            return ERROR_SUB_OK;
        case 2:
            *dataLen = FDCAN_DLC_BYTES_2;
            return ERROR_SUB_OK;
        case 3:
            *dataLen = FDCAN_DLC_BYTES_3;
            return ERROR_SUB_OK;
        case 4:
            *dataLen = FDCAN_DLC_BYTES_4;
            return ERROR_SUB_OK;
        case 5:
            *dataLen = FDCAN_DLC_BYTES_5;
            return ERROR_SUB_OK;
        case 6:
            *dataLen = FDCAN_DLC_BYTES_6;
            return ERROR_SUB_OK;
        case 7:
            *dataLen = FDCAN_DLC_BYTES_7;
            return ERROR_SUB_OK;
        case 8:
            *dataLen = FDCAN_DLC_BYTES_8;
            return ERROR_SUB_OK;
        case 12:
            *dataLen = FDCAN_DLC_BYTES_12;
            return ERROR_SUB_OK;
        case 16:
            *dataLen = FDCAN_DLC_BYTES_16;
            return ERROR_SUB_OK;
        case 20:
            *dataLen = FDCAN_DLC_BYTES_20;
            return ERROR_SUB_OK;
        case 24:
            *dataLen = FDCAN_DLC_BYTES_24;
            return ERROR_SUB_OK;
        case 32:
            *dataLen = FDCAN_DLC_BYTES_32;
            return ERROR_SUB_OK;
        case 48:
            *dataLen = FDCAN_DLC_BYTES_48;
            return ERROR_SUB_OK;
        case 64:
            *dataLen = FDCAN_DLC_BYTES_64;
            return ERROR_SUB_OK;
        default:
            *dataLen = FDCAN_DLC_BYTES_0;
            if(packSrc == CAN_PACK_SRC_1)
                return ERROR_SUB_MCU_CAN1_TX_LENGTH;
            else
                return ERROR_SUB_MCU_CAN2_TX_LENGTH;
    }
}


//将HAL库的数据长度转换为应用识别的数据长度
uint8_t MCU_CAN_ConvertBytesToLength(uint32_t dataLen)
{
    switch(dataLen)
    {
        case FDCAN_DLC_BYTES_0:
            return 0;
        case FDCAN_DLC_BYTES_1:
            return 1;
        case FDCAN_DLC_BYTES_2:
            return 2;
        case FDCAN_DLC_BYTES_3:
            return 3;
        case FDCAN_DLC_BYTES_4:
            return 4;
        case FDCAN_DLC_BYTES_5:
            return 5;
        case FDCAN_DLC_BYTES_6:
            return 6;
        case FDCAN_DLC_BYTES_7:
            return 7;
        case FDCAN_DLC_BYTES_8:
            return 8;
        case FDCAN_DLC_BYTES_12:
            return 12;
        case FDCAN_DLC_BYTES_16:
            return 16;
        case FDCAN_DLC_BYTES_20:
            return 20;
        case FDCAN_DLC_BYTES_24:
            return 24;
        case FDCAN_DLC_BYTES_32:
            return 32;
        case FDCAN_DLC_BYTES_48:
            return 48;
        case FDCAN_DLC_BYTES_64:
            return 64;
        default:
            return 0;
    }
}

//校准中断
void FDCAN_CAL_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();

    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}


//时钟校准回调
void HAL_FDCAN_ClockCalibrationCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ClkCalibrationITs)
{
    if(hfdcan->Instance == FDCAN1)
    {
        asm("nop");
#if(MCU_CAN1_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_ClockCalibrationCallback FDCAN1\r\n");
#endif
    }
    if(hfdcan->Instance == FDCAN2)
    {
        asm("nop");
#if(MCU_CAN2_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_ClockCalibrationCallback FDCAN2\r\n");
#endif
    }
}

//发送事件回调
void HAL_FDCAN_TxEventFifoCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t TxEventFifoITs)
{
    if(hfdcan->Instance == FDCAN1)
    {
        asm("nop");
#if(MCU_CAN1_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TxEventFifoCallback FDCAN1\r\n");
#endif
    }
    if(hfdcan->Instance == FDCAN2)
    {
        asm("nop");
#if(MCU_CAN2_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TxEventFifoCallback FDCAN2\r\n");
#endif
    }
}

//FIFO0 接收数据回调
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    if(hfdcan->Instance == FDCAN1)
    {
        MCU_CAN1_RxFifo0CallBack(RxFifo0ITs);
#if(MCU_CAN1_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_RxFifo0Callback FDCAN1\r\n");
#endif
    }
    if(hfdcan->Instance == FDCAN2)
    {
        MCU_CAN2_RxFifo0CallBack(RxFifo0ITs);
#if(MCU_CAN2_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_RxFifo0Callback FDCAN2\r\n");
#endif
    }
}

//FIFO1接收数据回调
void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs)
{
    if(hfdcan->Instance == FDCAN1)
    {
        MCU_CAN1_RxFifo1CallBack(RxFifo1ITs);
#if(MCU_CAN1_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_RxFifo1Callback FDCAN1\r\n");
#endif
    }
    if(hfdcan->Instance == FDCAN2)
    {
        MCU_CAN2_RxFifo1CallBack(RxFifo1ITs);
#if(MCU_CAN2_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_RxFifo1Callback FDCAN2\r\n");
#endif
    }
}   

//TX FIFO空回调
void HAL_FDCAN_TxFifoEmptyCallback(FDCAN_HandleTypeDef *hfdcan)
{
    if(hfdcan->Instance == FDCAN1)
    {
        asm("nop");
#if(MCU_CAN1_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TxFifoEmptyCallback FDCAN1\r\n");
#endif
    }
    if(hfdcan->Instance == FDCAN2)
    {
        asm("nop");
#if(MCU_CAN2_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TxFifoEmptyCallback FDCAN2\r\n");
#endif
    }
}

//一个buffer发送完成回调
void HAL_FDCAN_TxBufferCompleteCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t BufferIndexes)
{
    if(hfdcan->Instance == FDCAN1)
    {
        MCU_CAN1_TxCompleteCallBack(BufferIndexes);
#if(MCU_CAN1_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TxBufferCompleteCallback FDCAN1\r\n");
#endif
    }
    if(hfdcan->Instance == FDCAN2)
    {
        MCU_CAN2_TxCompleteCallBack(BufferIndexes);
#if(MCU_CAN2_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TxBufferCompleteCallback FDCAN2\r\n");
#endif
    }
}

//发送终止回调
void HAL_FDCAN_TxBufferAbortCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t BufferIndexes)
{
    if(hfdcan->Instance == FDCAN1)
    {
        asm("nop");
#if(MCU_CAN1_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TxBufferAbortCallback FDCAN1\r\n");
#endif
    }
    if(hfdcan->Instance == FDCAN2)
    {
        asm("nop");
#if(MCU_CAN2_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TxBufferAbortCallback FDCAN2\r\n");
#endif
    }
}

//接收缓存接收到新的数据包回调
void HAL_FDCAN_RxBufferNewMessageCallback(FDCAN_HandleTypeDef *hfdcan)
{
    if(hfdcan->Instance == FDCAN1)
    {
        asm("nop");
#if(MCU_CAN1_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_RxBufferNewMessageCallback FDCAN1\r\n");
#endif
    }
    if(hfdcan->Instance == FDCAN2)
    {
        asm("nop");
#if(MCU_CAN2_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_RxBufferNewMessageCallback FDCAN2\r\n");
#endif
    }
}

//高优先级消息接收回调
void HAL_FDCAN_HighPriorityMessageCallback(FDCAN_HandleTypeDef *hfdcan)
{
    if(hfdcan->Instance == FDCAN1)
    {
        asm("nop");
#if(MCU_CAN1_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_HighPriorityMessageCallback FDCAN1\r\n");
#endif
    }
    if(hfdcan->Instance == FDCAN2)
    {
        asm("nop");
#if(MCU_CAN2_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_HighPriorityMessageCallback FDCAN2\r\n");
#endif
    }
}

//TTCAN时间帧相关回调
void HAL_FDCAN_TimestampWraparoundCallback(FDCAN_HandleTypeDef *hfdcan)
{
    if(hfdcan->Instance == FDCAN1)
    {
        asm("nop");
#if(MCU_CAN1_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TimestampWraparoundCallback FDCAN1\r\n");
#endif
    }
    if(hfdcan->Instance == FDCAN2)
    {
        asm("nop");
#if(MCU_CAN2_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TimestampWraparoundCallback FDCAN2\r\n");
#endif
    }
}

//超时回调
void HAL_FDCAN_TimeoutOccurredCallback(FDCAN_HandleTypeDef *hfdcan)
{
    if(hfdcan->Instance == FDCAN1)
    {
        asm("nop");
#if(MCU_CAN1_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TimeoutOccurredCallback FDCAN1\r\n");
#endif
    }
    if(hfdcan->Instance == FDCAN2)
    {
        asm("nop");
#if(MCU_CAN2_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TimeoutOccurredCallback FDCAN2\r\n");
#endif
    }
}

//错误回调,指API运行时遇到了错误
void HAL_FDCAN_ErrorCallback(FDCAN_HandleTypeDef *hfdcan)
{
    if(hfdcan->Instance == FDCAN1)
    {
        //CAN1错误回调函数
        MCU_CAN1_ErrorCallBack();
#if(MCU_CAN1_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_ErrorCallback FDCAN1\r\n");
#endif
    }
    if(hfdcan->Instance == FDCAN2)
    {
        //CAN2错误回调函数
        MCU_CAN2_ErrorCallBack();
#if(MCU_CAN2_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_ErrorCallback FDCAN2\r\n");
#endif
    }
}

//错误状态回调.只CAN模块的寄存器指示了错误
void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs)
{
    if(hfdcan->Instance == FDCAN1)
    {
        //CAN1错误状态回调函数
        MCU_CAN1_ErrorStatusCallBack(ErrorStatusITs);
#if(MCU_CAN1_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_ErrorStatusCallback FDCAN1\r\n");
#endif
    }
    if(hfdcan->Instance == FDCAN2)
    {
        //CAN2错误状态回调函数
        MCU_CAN2_ErrorStatusCallBack(ErrorStatusITs);
#if(MCU_CAN2_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_ErrorStatusCallback FDCAN2\r\n");
#endif
    }
}

//同步调度回调
void HAL_FDCAN_TT_ScheduleSyncCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t TTSchedSyncITs)
{
    if(hfdcan->Instance == FDCAN1)
    {
        asm("nop");
#if(MCU_CAN1_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TT_ScheduleSyncCallback FDCAN1\r\n");
#endif
    }
    if(hfdcan->Instance == FDCAN2)
    {
        asm("nop");
#if(MCU_CAN2_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TT_ScheduleSyncCallback FDCAN2\r\n");
#endif
    }
}

//时间mark回调
void HAL_FDCAN_TT_TimeMarkCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t TTTimeMarkITs)
{   
    if(hfdcan->Instance == FDCAN1)
    {
        asm("nop");
#if(MCU_CAN1_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TT_TimeMarkCallback FDCAN1\r\n");
#endif
    }
    if(hfdcan->Instance == FDCAN2)
    {
        asm("nop");
#if(MCU_CAN2_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TT_TimeMarkCallback FDCAN2\r\n");
#endif
    }
}

//TTCAN停止监视回调
void HAL_FDCAN_TT_StopWatchCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t SWTime, uint32_t SWCycleCount)
{
    if(hfdcan->Instance == FDCAN1)
    {
        asm("nop");
#if(MCU_CAN1_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TT_StopWatchCallback FDCAN1\r\n");
#endif
    }
    if(hfdcan->Instance == FDCAN2)
    {
        asm("nop");
#if(MCU_CAN2_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TT_StopWatchCallback FDCAN2\r\n");
#endif
    }
}

//全局事件异常回调
void HAL_FDCAN_TT_GlobalTimeCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t TTGlobTimeITs)
{
    if(hfdcan->Instance == FDCAN1)
    {
        asm("nop");
#if(MCU_CAN1_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TT_GlobalTimeCallback FDCAN1\r\n");
#endif
    }
    if(hfdcan->Instance == FDCAN2)
    {
        asm("nop");
#if(MCU_CAN2_ENANLE_SHOW_INT_MSG == 1)
        LowLevelSendStringInt("HAL_FDCAN_TT_GlobalTimeCallback FDCAN2\r\n");
#endif
    }
}


