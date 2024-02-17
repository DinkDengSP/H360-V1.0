/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 09:31:54 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 15:22:22 +0800
************************************************************************************************/ 
#include "MCU_IIC_Universal.h"
#include "MCU_IIC2.h"
#include "LowLevelShow.h"

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->Instance == I2C2)
    {
#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_INT)
        //中断任务回调
        MCU_IIC2_MasterTxCpltCallbackInt();
#endif
#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_DMA)
        //中断任务回调
        MCU_IIC2_MasterTxCpltCallbackDMA();
#endif
    }
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->Instance == I2C2)
    {
#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_INT)
        //中断任务回调
        MCU_IIC2_MasterRxCpltCallbackInt();
#endif
#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_DMA)
        //中断任务回调
        MCU_IIC2_MasterRxCpltCallbackDMA();
#endif
    }
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->Instance == I2C2)
    {
        asm("nop");
    }
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->Instance == I2C2)
    {
        asm("nop");
    }
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
    if(hi2c->Instance == I2C2)
    {
        asm("nop");
    }
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{   
    if(hi2c->Instance == I2C2)
    {
        asm("nop");
    }
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->Instance == I2C2)
    {

#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_INT)
        //中断任务回调
        MCU_IIC2_MemTxCpltCallbackInt();
#endif
#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_DMA)
        //中断任务回调
        MCU_IIC2_MemTxCpltCallbackDMA();
#endif
    }
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{   
    if(hi2c->Instance == I2C2)
    {
#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_INT)
        //中断任务回调
        MCU_IIC2_MemRxCpltCallbackInt();
#endif
#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_DMA)
        //中断任务回调
        MCU_IIC2_MemRxCpltCallbackDMA();
#endif
    }
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->Instance == I2C2)
    {
#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_INT)
        ////中断任务回调
        MCU_IIC2_ErrorCallbackInt();
#endif
#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_DMA)
        //中断任务回调
        MCU_IIC2_ErrorCallbackDMA();
#endif
    }
}

void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c)
{   
    if(hi2c->Instance == I2C2)
    {
#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_INT)
        //中断任务回调
        MCU_IIC2_AbortCpltCallbackInt();
#endif
#if(MCU_IIC2_MODE_USER == MCU_IIC_MODE_DMA)
        //中断任务回调
        MCU_IIC2_AbortCpltCallbackDMA();
#endif
    }
}
