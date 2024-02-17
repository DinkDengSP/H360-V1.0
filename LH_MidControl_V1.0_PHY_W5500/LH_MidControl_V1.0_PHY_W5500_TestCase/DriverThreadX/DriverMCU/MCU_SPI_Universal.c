/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:01:42 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-25 11:40:58 +0800
************************************************************************************************/ 
#include "MCU_SPI_Universal.h"
#include "LowLevelShow.h"
#include "MCU_SPI2.h"
#include "MCU_SPI5.h"


void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == SPI2)
    {
#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_INT)
        //中断模式回调函数
        MCU_SPI2_TxCpltCallback_Int();
#endif
#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_DMA)
        //DMA模式回调函数
        MCU_SPI2_TxCpltCallback_DMA();
#endif
    }
    if(hspi->Instance == SPI5)
    {
#if(MCU_SPI5_MODE_USER == MCU_SPI_MODE_INT)
        //中断模式回调函数
        MCU_SPI5_TxCpltCallback_Int();
#endif
#if(MCU_SPI5_MODE_USER == MCU_SPI_MODE_DMA)
        //DMA模式回调函数
        MCU_SPI5_TxCpltCallback_DMA();
#endif        
    }
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == SPI2)
    {
#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_INT)
        //中断模式回调函数
        MCU_SPI2_RxCpltCallback_Int();
#endif
#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_DMA)
        //DMA模式回调函数
        MCU_SPI2_RxCpltCallback_DMA();
#endif
    }
    if(hspi->Instance == SPI5)
    {
#if(MCU_SPI5_MODE_USER == MCU_SPI_MODE_INT)
        //中断模式回调函数
        MCU_SPI5_RxCpltCallback_Int();
#endif
#if(MCU_SPI5_MODE_USER == MCU_SPI_MODE_DMA)
        //DMA模式回调函数
        MCU_SPI5_RxCpltCallback_DMA();
#endif        
    }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == SPI2)
    {
#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_INT)
        //中断模式回调函数
        MCU_SPI2_TxRxCpltCallback_Int();
#endif
#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_DMA)
        //DMA模式回调函数
        MCU_SPI2_TxRxCpltCallback_DMA();
#endif
    }
    if(hspi->Instance == SPI5)
    {
#if(MCU_SPI5_MODE_USER == MCU_SPI_MODE_INT)
        //中断模式回调函数
        MCU_SPI5_TxRxCpltCallback_Int();
#endif
#if(MCU_SPI5_MODE_USER == MCU_SPI_MODE_DMA)
        //DMA模式回调函数
        MCU_SPI5_TxRxCpltCallback_DMA();
#endif        
    }
}

void HAL_SPI_TxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == SPI2)
    {
#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_INT)
        //中断模式回调函数
        //xxxxxxxx();
#endif
#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_DMA)
        //DMA模式回调函数
        //xxxxxxxx();
#endif
    }
    if(hspi->Instance == SPI5)
    {
#if(MCU_SPI5_MODE_USER == MCU_SPI_MODE_INT)
        //中断模式回调函数
        //xxxxxxxx();
#endif
#if(MCU_SPI5_MODE_USER == MCU_SPI_MODE_DMA)
        //DMA模式回调函数
        //xxxxxxxx();
#endif        
    }
}

void HAL_SPI_RxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == SPI2)
    {
#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_INT)
        //中断模式回调函数
        //xxxxxxxx();
#endif
#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_DMA)
        //DMA模式回调函数
        //xxxxxxxx();
#endif
    }
    if(hspi->Instance == SPI5)
    {
#if(MCU_SPI5_MODE_USER == MCU_SPI_MODE_INT)
        //中断模式回调函数
        //xxxxxxxx();
#endif
#if(MCU_SPI5_MODE_USER == MCU_SPI_MODE_DMA)
        //DMA模式回调函数
        //xxxxxxxx();
#endif        
    }
}

void HAL_SPI_TxRxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == SPI2)
    {
#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_INT)
        //中断模式回调函数
        //xxxxxxxx();
#endif
#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_DMA)
        //DMA模式回调函数
        //xxxxxxxx();
#endif
    }
    if(hspi->Instance == SPI5)
    {
#if(MCU_SPI5_MODE_USER == MCU_SPI_MODE_INT)
        //中断模式回调函数
        //xxxxxxxx();
#endif
#if(MCU_SPI5_MODE_USER == MCU_SPI_MODE_DMA)
        //DMA模式回调函数
        //xxxxxxxx();
#endif        
    }
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == SPI2)
    {
#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_INT)
        //中断模式回调函数
        MCU_SPI2_ErrorCallback_Int();
#endif
#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_DMA)
        //DMA模式回调函数
        MCU_SPI2_ErrorCallback_DMA();
#endif
    }
    if(hspi->Instance == SPI5)
    {
#if(MCU_SPI5_MODE_USER == MCU_SPI_MODE_INT)
        //中断模式回调函数
        MCU_SPI5_ErrorCallback_Int();
#endif
#if(MCU_SPI5_MODE_USER == MCU_SPI_MODE_DMA)
        //DMA模式回调函数
        MCU_SPI5_ErrorCallback_DMA();
#endif        
    }
}

void HAL_SPI_AbortCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == SPI2)
    {
#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_INT)
        //中断模式回调函数
        MCU_SPI2_AbortCpltCallback_IT();
#endif
#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_DMA)
        //DMA模式回调函数
        MCU_SPI2_AbortCpltCallback_DMA();
#endif
    }
    if(hspi->Instance == SPI5)
    {
#if(MCU_SPI5_MODE_USER == MCU_SPI_MODE_INT)
        //中断模式回调函数
        MCU_SPI5_AbortCpltCallback_IT();
#endif
#if(MCU_SPI5_MODE_USER == MCU_SPI_MODE_DMA)
        //DMA模式回调函数
        MCU_SPI5_AbortCpltCallback_DMA();
#endif        
    }
}
