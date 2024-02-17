/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-19 10:25:14
**LastEditors: DengXiaoJun
**LastEditTime: 2020-06-23 08:29:24
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "MCU_MPU.h"
#include "UserConfigInc.h"

//初始化芯片MPU配置
void MCU_MPU_ConfigInit(void)
{
    //MPU配置结构体
        MPU_Region_InitTypeDef MPU_InitStruct;
	/*配置MPU之前必须先禁止,禁止 MPU */
	    HAL_MPU_Disable();
	/*配置AXI SRAM的MPU属性为Write back, Read allocate，Write allocate -----Buffer为1 Cache为1,Tex为1 Normal配置*/
    /*写入时,同时写入Cache和SRAM,读取时候,若Cache中已经存在,则读取Cache不读取SRAM,否则,读取SRAM*/
        MPU_InitStruct.Enable           = MPU_REGION_ENABLE;//使能SRAM
        MPU_InitStruct.BaseAddress      = D1_AXI_SRAM_BASE_ADDR;//SRAM起始地址
        MPU_InitStruct.Size             = D1_AXI_SRAM_LENGTH;//SRAM长度
        MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;//对该区域的访问级别设定
        MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;//是否使能指令缓存 B参数
        MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;//C参数
        MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;//是否使能共享
        MPU_InitStruct.Number           = D1_AXI_SRAM_NBR;//MPU区域,15优先级最高,0优先级最低
        MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;//设置Cache策略
        MPU_InitStruct.SubRegionDisable = 0x00;//控制MPU特定子区域是否使能
        MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;//指令访问使能
        //实际配置
        HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /*配置D2_SRAM1的MPU属性为Write back, Read allocate，Write allocate -----Buffer为1 Cache为1,Tex为1 Normal配置*/
    /*写入时,同时写入Cache和SRAM,读取时候,若Cache中已经存在,则读取Cache不读取SRAM,否则,读取SRAM*/
        MPU_InitStruct.Enable           = MPU_REGION_ENABLE;//使能SRAM
        MPU_InitStruct.BaseAddress      = D2_SRAM1_BASE_ADDR;//SRAM起始地址
        MPU_InitStruct.Size             = D2_SRAM1_LENGTH;//SRAM长度
        MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;//对该区域的访问级别设定
        MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;//是否使能指令缓存 B参数
        MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;//C参数
        MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;//是否使能共享
        MPU_InitStruct.Number           = D2_SRAM1_NBR;//MPU区域,15优先级最高,0优先级最低
        MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;//设置Cache策略
        MPU_InitStruct.SubRegionDisable = 0x00;//控制MPU特定子区域是否使能
        MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;//指令访问使能
        //实际配置
        HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /*配置D2_SRAM2的MPU属性为Write back, Read allocate，Write allocate -----Buffer为1 Cache为1,Tex为1 Normal配置*/
    /*写入时,同时写入Cache和SRAM,读取时候,若Cache中已经存在,则读取Cache不读取SRAM,否则,读取SRAM*/
        MPU_InitStruct.Enable           = MPU_REGION_ENABLE;//使能SRAM
        MPU_InitStruct.BaseAddress      = D2_SRAM2_BASE_ADDR;//SRAM起始地址
        MPU_InitStruct.Size             = D2_SRAM2_LENGTH;//SRAM长度
        MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;//对该区域的访问级别设定
        MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;//是否使能指令缓存 B参数
        MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;//C参数
        MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;//是否使能共享
        MPU_InitStruct.Number           = D2_SRAM2_NBR;//MPU区域,15优先级最高,0优先级最低
        MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;//设置Cache策略
        MPU_InitStruct.SubRegionDisable = 0x00;//控制MPU特定子区域是否使能
        MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;//指令访问使能
        //实际配置
        HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /*配置D2_SRAM3的MPU属性为Write back, Read allocate，Write allocate -----Buffer为1 Cache为1,Tex为1 Normal配置*/
    /*写入时,同时写入Cache和SRAM,读取时候,若Cache中已经存在,则读取Cache不读取SRAM,否则,读取SRAM*/
        MPU_InitStruct.Enable           = MPU_REGION_ENABLE;//使能SRAM
        MPU_InitStruct.BaseAddress      = D2_SRAM3_BASE_ADDR;//SRAM起始地址
        MPU_InitStruct.Size             = D2_SRAM3_LENGTH;//SRAM长度
        MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;//对该区域的访问级别设定
        MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;//是否使能指令缓存 B参数
        MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;//C参数
        MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;//是否使能共享
        MPU_InitStruct.Number           = D2_SRAM3_NBR;//MPU区域,15优先级最高,0优先级最低
        MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;//设置Cache策略
        MPU_InitStruct.SubRegionDisable = 0x00;//控制MPU特定子区域是否使能
        MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;//指令访问使能
        //实际配置
        HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /*配置D3_SRAM4的MPU属性为Write back, Read allocate，Write allocate -----Buffer为1 Cache为1,Tex为1 Normal配置*/
    /*写入时,同时写入Cache和SRAM,读取时候,若Cache中已经存在,则读取Cache不读取SRAM,否则,读取SRAM*/
        MPU_InitStruct.Enable           = MPU_REGION_ENABLE;//使能SRAM
        MPU_InitStruct.BaseAddress      = D3_SRAM4_BASE_ADDR;//SRAM起始地址
        MPU_InitStruct.Size             = D3_SRAM4_LENGTH;//SRAM长度
        MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;//对该区域的访问级别设定
        MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;//是否使能指令缓存 B参数
        MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;//C参数
        MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;//是否使能共享
        MPU_InitStruct.Number           = D3_SRAM4_NBR;//MPU区域,15优先级最高,0优先级最低
        MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;//设置Cache策略
        MPU_InitStruct.SubRegionDisable = 0x00;//控制MPU特定子区域是否使能
        MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;//指令访问使能
        //实际配置
        HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /*配置D3_BACKUP_SRAM的MPU属性为Write back, Read allocate，Write allocate -----Buffer为1 Cache为1,Tex为1 Normal配置*/
    /*写入时,同时写入Cache和SRAM,读取时候,若Cache中已经存在,则读取Cache不读取SRAM,否则,读取SRAM*/
        MPU_InitStruct.Enable           = MPU_REGION_ENABLE;//使能SRAM
        MPU_InitStruct.BaseAddress      = D3_BACKUP_SRAM_BASE_ADDR;//SRAM起始地址
        MPU_InitStruct.Size             = D3_BACKUP_SRAM_LENGTH;//SRAM长度
        MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;//对该区域的访问级别设定
        MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;//是否使能指令缓存 B参数
        MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;//C参数
        MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;//是否使能共享
        MPU_InitStruct.Number           = D3_BACKUP_SRAM_NBR;//MPU区域,15优先级最高,0优先级最低
        MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;//设置Cache策略
        MPU_InitStruct.SubRegionDisable = 0x00;//控制MPU特定子区域是否使能
        MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;//指令访问使能
        //实际配置
        HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/*配置完成,使能MPU*/
	    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

//使能L1缓存
void MCU_MPU_CacheEnable(void)
{
    /* 使能 I-Cache */
	SCB_EnableICache();
	/* 使能 D-Cache */
	SCB_EnableDCache();
    /*强制开启写透模式,目前不强制开启*/
    //SCB->CACR|=1<<2; 
}

//禁用L1缓存
void MCU_MPU_CacheDisable(void)
{
    //禁用I-Cache
    SCB_DisableICache();
    //禁用D-Cache
    SCB_DisableDCache();
}

//更新更新Cache
void MCU_MPU_CacheReflush(CACHE_REFLUSH_CONFIG config)
{
    if(config == CACHE_WRITE_BACK_ONLY)
    {
        //将CACHE中的数据写回到SRAM中
        SCB_CleanDCache();
    }
    else if(config == CACHE_READ_RELOAD_ONLY)
    {
        //丢弃掉当前Cache中的数据,从SRAM中重新加载数据,可能造成数据丢失
        SCB_InvalidateDCache();
    }
    else if(config == CACHE_READ_WRITE)
    {
        //将CACHE中的数据写回到SRAM并且重新加载到Cache中,消耗的时间最长
        SCB_CleanInvalidateDCache();
    }
}













