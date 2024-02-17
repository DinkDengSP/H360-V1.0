/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-05 15:45:24
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-05 17:54:39
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "BoardSD.h"
#include "BoardRS.h"
#include "IntPriority.h"

//SD卡句柄
static SD_HandleTypeDef uSdHandle;
//SD卡相关讯息
HAL_SD_CardInfoTypeDef currentSDCardInfo;
//互斥信号量
static TX_MUTEX mutexBoardSDCard = {0};
//读写信号量
static TX_SEMAPHORE semaphoreBoardSDCardTx = {0};
static TX_SEMAPHORE semaphoreBoardSDCardRx = {0};
//内置4字节对齐的DMA缓冲区,用于读写,只能使用AXI_SRAM
#pragma pack(push) //保存对齐状态 
#pragma pack(4)
static volatile uint8_t sdWriteBufferDMA[BLOCKSIZE]@".axi_sram" = {0};
static volatile uint8_t sdReadBufferDMA[BLOCKSIZE]@".axi_sram" = {0};
#pragma pack(pop) //保存对齐状态

//硬件初始化
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
    //时钟使能
    __HAL_RCC_SDMMC1_CLK_ENABLE();
    //SD时钟选择
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit = {0};
    //选择PLL1Q时钟,最大支持250M,实际120M
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SDMMC;
    RCC_PeriphClkInit.FmcClockSelection = RCC_SDMMCCLKSOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit) != HAL_OK)
    {
        while(1);
    }
    //时钟复位
    __HAL_RCC_SDMMC1_FORCE_RESET();
    __HAL_RCC_SDMMC1_RELEASE_RESET();
    //PC8 SDIO_S0
    MCU_PortInit(MCU_PIN_C_8,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_HIGH,GPIO_AF12_SDIO1);
    //PC9 SDIO_D1
    MCU_PortInit(MCU_PIN_C_9,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_HIGH,GPIO_AF12_SDIO1);
    //PC10 SDIO_D2
    MCU_PortInit(MCU_PIN_C_10,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_HIGH,GPIO_AF12_SDIO1);
    //PC11 SDIO_D3
    MCU_PortInit(MCU_PIN_C_11,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_HIGH,GPIO_AF12_SDIO1);
    //PC12 SDIO_CK
    MCU_PortInit(MCU_PIN_C_12,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_HIGH,GPIO_AF12_SDIO1);
    //PD2 SDIO_CMD
    MCU_PortInit(MCU_PIN_D_2,MCU_PORT_MODE_AF_PP,MCU_PORT_PULL_NOPULL,MCU_PORT_SPEED_FREQ_HIGH,GPIO_AF12_SDIO1);
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
    //创建互斥信号量,支持优先级继承
    if(mutexBoardSDCard.tx_mutex_id == 0)
    {
        tx_mutex_create(&mutexBoardSDCard,"mutexBoardSDCard",TX_INHERIT);
    }
    //创建信号灯
    if(semaphoreBoardSDCardTx.tx_semaphore_id == 0)
    {
        tx_semaphore_create(&semaphoreBoardSDCardTx,"semaphoreBoardSDCardTx",0);
    }
    if(semaphoreBoardSDCardRx.tx_semaphore_id == 0)
    {
        tx_semaphore_create(&semaphoreBoardSDCardRx,"semaphoreBoardSDCardRx",0);
    }
    //配置SD卡类型
    //120M PLLQ,5分24M主频
    uSdHandle.Init.ClockDiv            = 5;		
    uSdHandle.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
    uSdHandle.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
    uSdHandle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_ENABLE;
    uSdHandle.Init.BusWide             = SDMMC_BUS_WIDE_4B;
    //初始化
    halState = HAL_SD_Init(&uSdHandle);
    if(halState != HAL_OK)
    {
        return ErrorConvertHAL(halState);
    }
    return errorCode;
}

//多个扇区擦除
LH_ERR BoardSDCardEraseMultiBlock(uint32_t startBlockNo, uint32_t blockCount,uint32_t timeOutMs)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //申请互斥信号量
    tx_mutex_get(&mutexBoardSDCard,TX_WAIT_FOREVER);
    //发出擦除指令
    halState = HAL_SD_Erase(&uSdHandle, startBlockNo, startBlockNo+blockCount);
    if(halState != HAL_OK)
    {
        //终止操作
        HAL_SD_Abort(&uSdHandle);
        //释放互斥信号量
        tx_mutex_put(&mutexBoardSDCard);
        return ErrorConvertHAL(halState);
    }
    //等待擦除完成
    while(timeOutMs > 0)
    {
      if(HAL_SD_GetCardState(&uSdHandle) == HAL_SD_CARD_TRANSFER)
      {
          //完成传输
          break;
      }
      timeOutMs--;
      //延时
      CoreDelayMinTick();
    }
    if(timeOutMs == 0)
    {
        //终止操作
        HAL_SD_Abort(&uSdHandle);
        //释放互斥信号量
        tx_mutex_put(&mutexBoardSDCard);
        return LH_ERR_BOARD_MAIN_COMM_SD_WAIT_TIME_OUT;
    }
    //释放互斥信号量
    tx_mutex_put(&mutexBoardSDCard);
    return LH_ERR_NONE;
}

//多个扇区写入,内部使用单次写入组合,速度不快但是稳定性高
LH_ERR BoardSDCardWriteMultiBlock(uint8_t* writeDataBuffer,uint32_t startBlockNo, uint32_t blockCount)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint32_t txResult = TX_SUCCESS;
    uint32_t indexUtilBlock = 0;
    //申请互斥信号量
    tx_mutex_get(&mutexBoardSDCard,TX_WAIT_FOREVER);
    for(indexUtilBlock = 0; indexUtilBlock < blockCount;indexUtilBlock++)
    {
        //数据拷贝
        UserMemCopy((void*)sdWriteBufferDMA,(writeDataBuffer+indexUtilBlock*BLOCKSIZE),BLOCKSIZE);
        //数据拷贝好了,开始传输
        halState = HAL_SD_WriteBlocks_DMA(&uSdHandle, (uint8_t*)sdWriteBufferDMA, startBlockNo+indexUtilBlock,1);
        if(halState != HAL_OK)
        {
            //终止操作
            HAL_SD_Abort(&uSdHandle);
            //释放互斥信号量
            tx_mutex_put(&mutexBoardSDCard);
            return ErrorConvertHAL(halState);
        }
        //等待发送完成申请一个信号灯,必须成功
        txResult = tx_semaphore_get(&semaphoreBoardSDCardTx,BOARD_SD_TIME_OUT);
        if(txResult != TX_SUCCESS)
        {
            //信号灯获取失败,没触发发送完成中断
            //终止操作
            HAL_SD_Abort(&uSdHandle);
            //释放互斥信号量
            tx_mutex_put(&mutexBoardSDCard);
            //写入超时
            return LH_ERR_BOARD_MAIN_COMM_SD_WRITE;
        }
    }
    //释放互斥信号量
    tx_mutex_put(&mutexBoardSDCard);
    //运行完成
    return LH_ERR_NONE;
}

//多个扇区读取,内部使用单次读写组合,速度不快但是稳定性高
LH_ERR BoardSDCardReadMultiBlock(uint8_t* readDataBuffer,uint32_t startBlockNo,uint32_t blockCount)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint32_t indexUtilBlock = 0;
    uint32_t txResult = TX_SUCCESS;
    //申请互斥信号量
    tx_mutex_get(&mutexBoardSDCard,TX_WAIT_FOREVER);
    for(indexUtilBlock = 0; indexUtilBlock < blockCount;indexUtilBlock++)
    {
        //开始传输
        halState = HAL_SD_ReadBlocks_DMA(&uSdHandle, (uint8_t*)sdReadBufferDMA, startBlockNo+indexUtilBlock,1);
        if(halState != HAL_OK)
        {
            //终止操作
            HAL_SD_Abort(&uSdHandle);
            //释放互斥信号量
            tx_mutex_put(&mutexBoardSDCard);
            return ErrorConvertHAL(halState);
        }
        //等待接收完成申请一个信号灯,必须成功
        txResult = tx_semaphore_get(&semaphoreBoardSDCardRx,BOARD_SD_TIME_OUT);
        if(txResult != TX_SUCCESS)
        {
            //终止操作
            HAL_SD_Abort(&uSdHandle);
            //释放互斥信号量
            tx_mutex_put(&mutexBoardSDCard);
            //写入超时
            return LH_ERR_BOARD_MAIN_COMM_SD_READ;
        }
        //读取到的数据拷贝
        UserMemCopy((readDataBuffer+indexUtilBlock*BLOCKSIZE),(void*)sdReadBufferDMA,BLOCKSIZE);
    }
    //释放互斥信号量
    tx_mutex_put(&mutexBoardSDCard);
    //运行完成
    return LH_ERR_NONE;
}

//获取内存卡讯息
LH_ERR BoardSD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo)
{
    HAL_StatusTypeDef halState;
    //申请互斥信号量
    tx_mutex_get(&mutexBoardSDCard,TX_WAIT_FOREVER);
    //获取卡讯息
    halState = HAL_SD_GetCardInfo(&uSdHandle, CardInfo);
    if(halState != HAL_OK)
    {
        //终止操作
        HAL_SD_Abort(&uSdHandle);
        //释放互斥信号量
        tx_mutex_put(&mutexBoardSDCard);
        return ErrorConvertHAL(halState);
    }
    //释放互斥信号量
    tx_mutex_put(&mutexBoardSDCard);
    //完成
    return LH_ERR_NONE;
}

//打印SD卡讯息
void BoardSD_PrintfCardMsg(HAL_SD_CardInfoTypeDef *CardInfo)
{
    LowLevelPrintf("CardInfo.CardType : %d \r\n",CardInfo->CardType);
    LowLevelPrintf("CardInfo.CardVersion : %d \r\n",CardInfo->CardVersion);
    LowLevelPrintf("CardInfo.Class : %d \r\n",CardInfo->Class);
    LowLevelPrintf("CardInfo.RelCardAdd : %d \r\n",CardInfo->RelCardAdd);
    LowLevelPrintf("CardInfo.BlockNbr : %d \r\n",CardInfo->BlockNbr);
    LowLevelPrintf("CardInfo.BlockSize : %d \r\n",CardInfo->BlockSize);
    LowLevelPrintf("CardInfo.LogBlockNbr : %d \r\n",CardInfo->LogBlockNbr);
    LowLevelPrintf("CardInfo.LogBlockSize : %d \r\n",CardInfo->LogBlockSize);
    LowLevelPrintf("CardInfo.LogBlockSize : %d \r\n",CardInfo->LogBlockSize);

}

//错误中断
void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
{
    if(hsd->Instance == SDMMC1)
    {
        asm("nop");
    }
}
//取消中断
void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)
{
    if(hsd->Instance == SDMMC1)
    {
        asm("nop");
    }
}

//SDMMC 发送完成回调函数
void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
    if(hsd->Instance == SDMMC1)
    {
        //增加发送信号灯
        tx_semaphore_ceiling_put(&semaphoreBoardSDCardTx,1);
    }
}

//SDMMC 接收完成回调函数
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{   
    if(hsd->Instance == SDMMC1)
    {
        //增加接收信号灯
        tx_semaphore_ceiling_put(&semaphoreBoardSDCardRx,1);
    }
}

//SDIO中断
void SDMMC1_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
	HAL_SD_IRQHandler(&uSdHandle);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}







