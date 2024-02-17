/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-19 09:11:06
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-22 13:40:27
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardSD.h"

#include "BoardSD.h"

//SD卡句柄
static SD_HandleTypeDef uSdHandle;
//SD卡相关讯息
static HAL_SD_CardInfoTypeDef currentSDCardInfo;

//硬件初始化
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
    //时钟使能
        __HAL_RCC_SDMMC1_CLK_ENABLE();
    //时钟复位
        __HAL_RCC_SDMMC1_FORCE_RESET();
        __HAL_RCC_SDMMC1_RELEASE_RESET();
    //PC8 SDIO_S0
        MCU_PortInit(MCU_PIN_C_8,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_SDIO1);
    //PC9 SDIO_D1
        MCU_PortInit(MCU_PIN_C_9,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_SDIO1);
    //PC10 SDIO_D2
        MCU_PortInit(MCU_PIN_C_10,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_SDIO1);
    //PC11 SDIO_D3
        MCU_PortInit(MCU_PIN_C_11,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_SDIO1);
    //PC12 SDIO_CK
        MCU_PortInit(MCU_PIN_C_12,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_SDIO1);
    //PD2 SDIO_CMD
        MCU_PortInit(MCU_PIN_D_2,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_VERY_HIGH,GPIO_AF12_SDIO1);
    //中断初初始化
        HAL_NVIC_SetPriority(SDMMC1_IRQn, SDIO_PREEMPT_PRIORITY, SDIO_SUB_PRIORITY);
        HAL_NVIC_EnableIRQ(SDMMC1_IRQn);
}

//硬件重新初始化
void HAL_SD_MspDeInit(SD_HandleTypeDef *hsd)
{
    //中断关闭
        HAL_NVIC_DisableIRQ(SDMMC1_IRQn);
    //IO口初始化
        MCU_PortDeInit(MCU_PIN_C_8);
        MCU_PortDeInit(MCU_PIN_C_9);
        MCU_PortDeInit(MCU_PIN_C_10);
        MCU_PortDeInit(MCU_PIN_C_11);
        MCU_PortDeInit(MCU_PIN_C_12);
        MCU_PortDeInit(MCU_PIN_D_2);
    /* 复位SDMMC */
        __HAL_RCC_SDMMC1_FORCE_RESET();
        __HAL_RCC_SDMMC1_RELEASE_RESET();
    //关闭时钟
        __HAL_RCC_SDMMC1_CLK_DISABLE();
}

//SD卡状态初始化
LH_ERR BoardSD_Init(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    HAL_StatusTypeDef halState;
    //SD卡接口选择
        uSdHandle.Instance = SDMMC1;
    //SD接口初始化
        HAL_SD_DeInit(&uSdHandle);
    //配置SD卡类型
        //高速卡可以用1，低速卡用2（用1不稳定）
        uSdHandle.Init.ClockDiv            = 2;		
        uSdHandle.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
        uSdHandle.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
        uSdHandle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
        uSdHandle.Init.BusWide             = SDMMC_BUS_WIDE_4B;
    //初始化
        halState = HAL_SD_Init(&uSdHandle);
        if(halState != HAL_OK)
        {
            return ErrorConvertHAL(halState);
        }
    //SD卡初始化
        halState = HAL_SD_InitCard(&uSdHandle);
        if(halState != HAL_OK)
        {
            return ErrorConvertHAL(halState);
        }
    //更新SD卡相关讯息
        errorCode = BoardSD_GetCardInfo(&currentSDCardInfo);
    //完成
        return errorCode;
}

//读取指定块
LH_ERR BoardSD_ReadBlocks(uint32_t* pData, uint32_t ReadAddr, uint32_t NumOfBlocks)
{
    HAL_StatusTypeDef halState;
    //读取数据
        halState = HAL_SD_ReadBlocks(&uSdHandle, (uint8_t *)pData, ReadAddr, NumOfBlocks, BOARD_SD_TIME_OUT);
        if(halState != HAL_OK)
        {
            return ErrorConvertHAL(halState);
        }
    //完成
        return LH_ERR_NONE;
}

//写入指定块
LH_ERR BoardSD_WriteBlocks(uint32_t* pData, uint32_t WriteAddr, uint32_t NumOfBlocks)
{
    HAL_StatusTypeDef halState;
    //写入数据
        halState = HAL_SD_WriteBlocks(&uSdHandle, (uint8_t *)pData, WriteAddr, NumOfBlocks, BOARD_SD_TIME_OUT);
        if(halState != HAL_OK)
        {
            return ErrorConvertHAL(halState);
        }
    //完成
        return LH_ERR_NONE;
}

//读取指定块
LH_ERR BoardSD_ReadBlocksDMA(uint32_t* pData, uint32_t ReadAddr, uint32_t NumOfBlocks)
{
    HAL_StatusTypeDef halState;
    //读取数据
        halState = HAL_SD_ReadBlocks_DMA(&uSdHandle, (uint8_t *)pData, ReadAddr, NumOfBlocks);
        if(halState != HAL_OK)
        {
            return ErrorConvertHAL(halState);
        }
    //完成
        return LH_ERR_NONE;
}

//写入指定块
LH_ERR BoardSD_WriteBlocksDMA(uint32_t* pData, uint32_t WriteAddr, uint32_t NumOfBlocks)
{
    HAL_StatusTypeDef halState;
    //写入数据
        halState = HAL_SD_WriteBlocks_DMA(&uSdHandle, (uint8_t *)pData, WriteAddr, NumOfBlocks);
        if(halState != HAL_OK)
        {
            return ErrorConvertHAL(halState);
        }
    //完成
        return LH_ERR_NONE;
}

//擦除指定地址
LH_ERR BoardSD_Erase(uint32_t StartAddr, uint32_t EndAddr)
{
    HAL_StatusTypeDef halState;
    //擦除指定扇区
        halState = HAL_SD_Erase(&uSdHandle, StartAddr, EndAddr);
        if(halState != HAL_OK)
        {
            return ErrorConvertHAL(halState);
        }
    //完成
        return LH_ERR_NONE;
}

//获取内存卡状态
SD_TRANS_STATE BoardSD_GetCardState(void)
{
    return((HAL_SD_GetCardState(&uSdHandle) == HAL_SD_CARD_TRANSFER ) ? SD_TRANS_STATE_IDLE : SD_TRANS_STATE_BUSY);
}

//获取内存卡讯息
LH_ERR BoardSD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo)
{
    HAL_StatusTypeDef halState;
    //获取卡讯息
        halState = HAL_SD_GetCardInfo(&uSdHandle, CardInfo);
        if(halState != HAL_OK)
        {
            return ErrorConvertHAL(halState);
        }
    //完成
        return LH_ERR_NONE;
}

//打印SD卡讯息
void BoardSD_PrintfCardMsg(void)
{
    SDCardPrintf("currentSDCardInfo.CardType : %d \r\n",currentSDCardInfo.CardType);
    SDCardPrintf("currentSDCardInfo.CardVersion : %d \r\n",currentSDCardInfo.CardVersion);
    SDCardPrintf("currentSDCardInfo.Class : %d \r\n",currentSDCardInfo.Class);
    SDCardPrintf("currentSDCardInfo.RelCardAdd : %d \r\n",currentSDCardInfo.RelCardAdd);
    SDCardPrintf("currentSDCardInfo.BlockNbr : %d \r\n",currentSDCardInfo.BlockNbr);
    SDCardPrintf("currentSDCardInfo.BlockSize : %d \r\n",currentSDCardInfo.BlockSize);
    SDCardPrintf("currentSDCardInfo.LogBlockNbr : %d \r\n",currentSDCardInfo.LogBlockNbr);
    SDCardPrintf("currentSDCardInfo.LogBlockSize : %d \r\n",currentSDCardInfo.LogBlockSize);
    SDCardPrintf("currentSDCardInfo.LogBlockSize : %d \r\n",currentSDCardInfo.LogBlockSize);

}

//获取SD卡是否已经插入
SD_STATE BoardSD_IsDetected(void)
{
    return SD_INSERT;
}



//传输退出回调
void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)
{
    asm("nop");
}

//发送完成回调
void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
    asm("nop");
}

//接收完成回调
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
    asm("nop");
}

//错误回调
void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
{
    asm("nop");
}
  
//1.8V卡检测到回调
void HAL_SD_DriveTransciver_1_8V_Callback(FlagStatus status)
{
    asm("nop");
}

//SDIO中断
void SDMMC1_IRQHandler(void)
{
	HAL_SD_IRQHandler(&uSdHandle);
}





