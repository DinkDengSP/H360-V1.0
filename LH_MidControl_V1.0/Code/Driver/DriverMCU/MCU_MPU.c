/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-03-18 10:15:50 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-03-29 16:11:03 +0800
************************************************************************************************/ 
#include "MCU_MPU.h"



//初始化芯片MPU配置
void MCU_MPU_ConfigInit(void)
{
    //MPU配置结构体
    MPU_Region_InitTypeDef MPU_InitStruct;
	/*配置MPU之前必须先禁止,禁止 MPU */
	HAL_MPU_Disable();
    //依次配置0-15区域
    //区域0
    MPU_InitStruct.Enable           = mpuRegionConfigNBR0.regionState;//是否使能
    MPU_InitStruct.BaseAddress      = mpuRegionConfigNBR0.baseAddr;//地址
    MPU_InitStruct.Size             = mpuRegionConfigNBR0.regionLength;//区域长度
    MPU_InitStruct.AccessPermission = mpuRegionConfigNBR0.accessPermissions;//对该区域的访问级别设定
    MPU_InitStruct.DisableExec      = mpuRegionConfigNBR0.execState;//指令访问使能
    MPU_InitStruct.TypeExtField     = mpuRegionConfigNBR0.texSet;//设置Cache策略
    MPU_InitStruct.IsBufferable     = mpuRegionConfigNBR0.bufferState;//是否使能指令缓存 B参数
    MPU_InitStruct.IsCacheable      = mpuRegionConfigNBR0.cacheState;//C参数
    MPU_InitStruct.IsShareable      = mpuRegionConfigNBR0.shareState;//是否使能共享
    MPU_InitStruct.Number           = MPU_REGION_NUMBER0;//MPU区域,15优先级最高,0优先级最低
    MPU_InitStruct.SubRegionDisable = mpuRegionConfigNBR0.subRegionEnableConfig;//控制MPU特定子区域是否使能
    //实际配置
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    //区域1
    MPU_InitStruct.Enable           = mpuRegionConfigNBR1.regionState;//是否使能
    MPU_InitStruct.BaseAddress      = mpuRegionConfigNBR1.baseAddr;//地址
    MPU_InitStruct.Size             = mpuRegionConfigNBR1.regionLength;//区域长度
    MPU_InitStruct.AccessPermission = mpuRegionConfigNBR1.accessPermissions;//对该区域的访问级别设定
    MPU_InitStruct.DisableExec      = mpuRegionConfigNBR1.execState;//指令访问使能
    MPU_InitStruct.TypeExtField     = mpuRegionConfigNBR1.texSet;//设置Cache策略
    MPU_InitStruct.IsBufferable     = mpuRegionConfigNBR1.bufferState;//是否使能指令缓存 B参数
    MPU_InitStruct.IsCacheable      = mpuRegionConfigNBR1.cacheState;//C参数
    MPU_InitStruct.IsShareable      = mpuRegionConfigNBR1.shareState;//是否使能共享
    MPU_InitStruct.Number           = MPU_REGION_NUMBER1;//MPU区域,15优先级最高,0优先级最低
    MPU_InitStruct.SubRegionDisable = mpuRegionConfigNBR1.subRegionEnableConfig;//控制MPU特定子区域是否使能
    //实际配置
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    //区域2
    MPU_InitStruct.Enable           = mpuRegionConfigNBR2.regionState;//是否使能
    MPU_InitStruct.BaseAddress      = mpuRegionConfigNBR2.baseAddr;//地址
    MPU_InitStruct.Size             = mpuRegionConfigNBR2.regionLength;//区域长度
    MPU_InitStruct.AccessPermission = mpuRegionConfigNBR2.accessPermissions;//对该区域的访问级别设定
    MPU_InitStruct.DisableExec      = mpuRegionConfigNBR2.execState;//指令访问使能
    MPU_InitStruct.TypeExtField     = mpuRegionConfigNBR2.texSet;//设置Cache策略
    MPU_InitStruct.IsBufferable     = mpuRegionConfigNBR2.bufferState;//是否使能指令缓存 B参数
    MPU_InitStruct.IsCacheable      = mpuRegionConfigNBR2.cacheState;//C参数
    MPU_InitStruct.IsShareable      = mpuRegionConfigNBR2.shareState;//是否使能共享
    MPU_InitStruct.Number           = MPU_REGION_NUMBER2;//MPU区域,15优先级最高,0优先级最低
    MPU_InitStruct.SubRegionDisable = mpuRegionConfigNBR2.subRegionEnableConfig;//控制MPU特定子区域是否使能
    //实际配置
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    //区域3
    MPU_InitStruct.Enable           = mpuRegionConfigNBR3.regionState;//是否使能
    MPU_InitStruct.BaseAddress      = mpuRegionConfigNBR3.baseAddr;//地址
    MPU_InitStruct.Size             = mpuRegionConfigNBR3.regionLength;//区域长度
    MPU_InitStruct.AccessPermission = mpuRegionConfigNBR3.accessPermissions;//对该区域的访问级别设定
    MPU_InitStruct.DisableExec      = mpuRegionConfigNBR3.execState;//指令访问使能
    MPU_InitStruct.TypeExtField     = mpuRegionConfigNBR3.texSet;//设置Cache策略
    MPU_InitStruct.IsBufferable     = mpuRegionConfigNBR3.bufferState;//是否使能指令缓存 B参数
    MPU_InitStruct.IsCacheable      = mpuRegionConfigNBR3.cacheState;//C参数
    MPU_InitStruct.IsShareable      = mpuRegionConfigNBR3.shareState;//是否使能共享
    MPU_InitStruct.Number           = MPU_REGION_NUMBER3;//MPU区域,15优先级最高,0优先级最低
    MPU_InitStruct.SubRegionDisable = mpuRegionConfigNBR3.subRegionEnableConfig;//控制MPU特定子区域是否使能
    //实际配置
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    //区域4
    MPU_InitStruct.Enable           = mpuRegionConfigNBR4.regionState;//是否使能
    MPU_InitStruct.BaseAddress      = mpuRegionConfigNBR4.baseAddr;//地址
    MPU_InitStruct.Size             = mpuRegionConfigNBR4.regionLength;//区域长度
    MPU_InitStruct.AccessPermission = mpuRegionConfigNBR4.accessPermissions;//对该区域的访问级别设定
    MPU_InitStruct.DisableExec      = mpuRegionConfigNBR4.execState;//指令访问使能
    MPU_InitStruct.TypeExtField     = mpuRegionConfigNBR4.texSet;//设置Cache策略
    MPU_InitStruct.IsBufferable     = mpuRegionConfigNBR4.bufferState;//是否使能指令缓存 B参数
    MPU_InitStruct.IsCacheable      = mpuRegionConfigNBR4.cacheState;//C参数
    MPU_InitStruct.IsShareable      = mpuRegionConfigNBR4.shareState;//是否使能共享
    MPU_InitStruct.Number           = MPU_REGION_NUMBER4;//MPU区域,15优先级最高,0优先级最低
    MPU_InitStruct.SubRegionDisable = mpuRegionConfigNBR4.subRegionEnableConfig;//控制MPU特定子区域是否使能
    //实际配置
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    //区域5
    MPU_InitStruct.Enable           = mpuRegionConfigNBR5.regionState;//是否使能
    MPU_InitStruct.BaseAddress      = mpuRegionConfigNBR5.baseAddr;//地址
    MPU_InitStruct.Size             = mpuRegionConfigNBR5.regionLength;//区域长度
    MPU_InitStruct.AccessPermission = mpuRegionConfigNBR5.accessPermissions;//对该区域的访问级别设定
    MPU_InitStruct.DisableExec      = mpuRegionConfigNBR5.execState;//指令访问使能
    MPU_InitStruct.TypeExtField     = mpuRegionConfigNBR5.texSet;//设置Cache策略
    MPU_InitStruct.IsBufferable     = mpuRegionConfigNBR5.bufferState;//是否使能指令缓存 B参数
    MPU_InitStruct.IsCacheable      = mpuRegionConfigNBR5.cacheState;//C参数
    MPU_InitStruct.IsShareable      = mpuRegionConfigNBR5.shareState;//是否使能共享
    MPU_InitStruct.Number           = MPU_REGION_NUMBER5;//MPU区域,15优先级最高,0优先级最低
    MPU_InitStruct.SubRegionDisable = mpuRegionConfigNBR5.subRegionEnableConfig;//控制MPU特定子区域是否使能
    //实际配置
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    //区域6
    MPU_InitStruct.Enable           = mpuRegionConfigNBR6.regionState;//是否使能
    MPU_InitStruct.BaseAddress      = mpuRegionConfigNBR6.baseAddr;//地址
    MPU_InitStruct.Size             = mpuRegionConfigNBR6.regionLength;//区域长度
    MPU_InitStruct.AccessPermission = mpuRegionConfigNBR6.accessPermissions;//对该区域的访问级别设定
    MPU_InitStruct.DisableExec      = mpuRegionConfigNBR6.execState;//指令访问使能
    MPU_InitStruct.TypeExtField     = mpuRegionConfigNBR6.texSet;//设置Cache策略
    MPU_InitStruct.IsBufferable     = mpuRegionConfigNBR6.bufferState;//是否使能指令缓存 B参数
    MPU_InitStruct.IsCacheable      = mpuRegionConfigNBR6.cacheState;//C参数
    MPU_InitStruct.IsShareable      = mpuRegionConfigNBR6.shareState;//是否使能共享
    MPU_InitStruct.Number           = MPU_REGION_NUMBER6;//MPU区域,15优先级最高,0优先级最低
    MPU_InitStruct.SubRegionDisable = mpuRegionConfigNBR6.subRegionEnableConfig;//控制MPU特定子区域是否使能
    //实际配置
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    //区域7
    MPU_InitStruct.Enable           = mpuRegionConfigNBR7.regionState;//是否使能
    MPU_InitStruct.BaseAddress      = mpuRegionConfigNBR7.baseAddr;//地址
    MPU_InitStruct.Size             = mpuRegionConfigNBR7.regionLength;//区域长度
    MPU_InitStruct.AccessPermission = mpuRegionConfigNBR7.accessPermissions;//对该区域的访问级别设定
    MPU_InitStruct.DisableExec      = mpuRegionConfigNBR7.execState;//指令访问使能
    MPU_InitStruct.TypeExtField     = mpuRegionConfigNBR7.texSet;//设置Cache策略
    MPU_InitStruct.IsBufferable     = mpuRegionConfigNBR7.bufferState;//是否使能指令缓存 B参数
    MPU_InitStruct.IsCacheable      = mpuRegionConfigNBR7.cacheState;//C参数
    MPU_InitStruct.IsShareable      = mpuRegionConfigNBR7.shareState;//是否使能共享
    MPU_InitStruct.Number           = MPU_REGION_NUMBER7;//MPU区域,15优先级最高,0优先级最低
    MPU_InitStruct.SubRegionDisable = mpuRegionConfigNBR7.subRegionEnableConfig;//控制MPU特定子区域是否使能
    //实际配置
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    //区域8
    MPU_InitStruct.Enable           = mpuRegionConfigNBR8.regionState;//是否使能
    MPU_InitStruct.BaseAddress      = mpuRegionConfigNBR8.baseAddr;//地址
    MPU_InitStruct.Size             = mpuRegionConfigNBR8.regionLength;//区域长度
    MPU_InitStruct.AccessPermission = mpuRegionConfigNBR8.accessPermissions;//对该区域的访问级别设定
    MPU_InitStruct.DisableExec      = mpuRegionConfigNBR8.execState;//指令访问使能
    MPU_InitStruct.TypeExtField     = mpuRegionConfigNBR8.texSet;//设置Cache策略
    MPU_InitStruct.IsBufferable     = mpuRegionConfigNBR8.bufferState;//是否使能指令缓存 B参数
    MPU_InitStruct.IsCacheable      = mpuRegionConfigNBR8.cacheState;//C参数
    MPU_InitStruct.IsShareable      = mpuRegionConfigNBR8.shareState;//是否使能共享
    MPU_InitStruct.Number           = MPU_REGION_NUMBER8;//MPU区域,15优先级最高,0优先级最低
    MPU_InitStruct.SubRegionDisable = mpuRegionConfigNBR8.subRegionEnableConfig;//控制MPU特定子区域是否使能
    //实际配置
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    //区域9
    MPU_InitStruct.Enable           = mpuRegionConfigNBR9.regionState;//是否使能
    MPU_InitStruct.BaseAddress      = mpuRegionConfigNBR9.baseAddr;//地址
    MPU_InitStruct.Size             = mpuRegionConfigNBR9.regionLength;//区域长度
    MPU_InitStruct.AccessPermission = mpuRegionConfigNBR9.accessPermissions;//对该区域的访问级别设定
    MPU_InitStruct.DisableExec      = mpuRegionConfigNBR9.execState;//指令访问使能
    MPU_InitStruct.TypeExtField     = mpuRegionConfigNBR9.texSet;//设置Cache策略
    MPU_InitStruct.IsBufferable     = mpuRegionConfigNBR9.bufferState;//是否使能指令缓存 B参数
    MPU_InitStruct.IsCacheable      = mpuRegionConfigNBR9.cacheState;//C参数
    MPU_InitStruct.IsShareable      = mpuRegionConfigNBR9.shareState;//是否使能共享
    MPU_InitStruct.Number           = MPU_REGION_NUMBER9;//MPU区域,15优先级最高,0优先级最低
    MPU_InitStruct.SubRegionDisable = mpuRegionConfigNBR9.subRegionEnableConfig;//控制MPU特定子区域是否使能
    //实际配置
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    //区域10
    MPU_InitStruct.Enable           = mpuRegionConfigNBR10.regionState;//是否使能
    MPU_InitStruct.BaseAddress      = mpuRegionConfigNBR10.baseAddr;//地址
    MPU_InitStruct.Size             = mpuRegionConfigNBR10.regionLength;//区域长度
    MPU_InitStruct.AccessPermission = mpuRegionConfigNBR10.accessPermissions;//对该区域的访问级别设定
    MPU_InitStruct.DisableExec      = mpuRegionConfigNBR10.execState;//指令访问使能
    MPU_InitStruct.TypeExtField     = mpuRegionConfigNBR10.texSet;//设置Cache策略
    MPU_InitStruct.IsBufferable     = mpuRegionConfigNBR10.bufferState;//是否使能指令缓存 B参数
    MPU_InitStruct.IsCacheable      = mpuRegionConfigNBR10.cacheState;//C参数
    MPU_InitStruct.IsShareable      = mpuRegionConfigNBR10.shareState;//是否使能共享
    MPU_InitStruct.Number           = MPU_REGION_NUMBER10;//MPU区域,15优先级最高,0优先级最低
    MPU_InitStruct.SubRegionDisable = mpuRegionConfigNBR10.subRegionEnableConfig;//控制MPU特定子区域是否使能
    //实际配置
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    //区域11
    MPU_InitStruct.Enable           = mpuRegionConfigNBR11.regionState;//是否使能
    MPU_InitStruct.BaseAddress      = mpuRegionConfigNBR11.baseAddr;//地址
    MPU_InitStruct.Size             = mpuRegionConfigNBR11.regionLength;//区域长度
    MPU_InitStruct.AccessPermission = mpuRegionConfigNBR11.accessPermissions;//对该区域的访问级别设定
    MPU_InitStruct.DisableExec      = mpuRegionConfigNBR11.execState;//指令访问使能
    MPU_InitStruct.TypeExtField     = mpuRegionConfigNBR11.texSet;//设置Cache策略
    MPU_InitStruct.IsBufferable     = mpuRegionConfigNBR11.bufferState;//是否使能指令缓存 B参数
    MPU_InitStruct.IsCacheable      = mpuRegionConfigNBR11.cacheState;//C参数
    MPU_InitStruct.IsShareable      = mpuRegionConfigNBR11.shareState;//是否使能共享
    MPU_InitStruct.Number           = MPU_REGION_NUMBER11;//MPU区域,15优先级最高,0优先级最低
    MPU_InitStruct.SubRegionDisable = mpuRegionConfigNBR11.subRegionEnableConfig;//控制MPU特定子区域是否使能
    //实际配置
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    //区域12
    MPU_InitStruct.Enable           = mpuRegionConfigNBR12.regionState;//是否使能
    MPU_InitStruct.BaseAddress      = mpuRegionConfigNBR12.baseAddr;//地址
    MPU_InitStruct.Size             = mpuRegionConfigNBR12.regionLength;//区域长度
    MPU_InitStruct.AccessPermission = mpuRegionConfigNBR12.accessPermissions;//对该区域的访问级别设定
    MPU_InitStruct.DisableExec      = mpuRegionConfigNBR12.execState;//指令访问使能
    MPU_InitStruct.TypeExtField     = mpuRegionConfigNBR12.texSet;//设置Cache策略
    MPU_InitStruct.IsBufferable     = mpuRegionConfigNBR12.bufferState;//是否使能指令缓存 B参数
    MPU_InitStruct.IsCacheable      = mpuRegionConfigNBR12.cacheState;//C参数
    MPU_InitStruct.IsShareable      = mpuRegionConfigNBR12.shareState;//是否使能共享
    MPU_InitStruct.Number           = MPU_REGION_NUMBER12;//MPU区域,15优先级最高,0优先级最低
    MPU_InitStruct.SubRegionDisable = mpuRegionConfigNBR12.subRegionEnableConfig;//控制MPU特定子区域是否使能
    //实际配置
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    //区域13
    MPU_InitStruct.Enable           = mpuRegionConfigNBR13.regionState;//是否使能
    MPU_InitStruct.BaseAddress      = mpuRegionConfigNBR13.baseAddr;//地址
    MPU_InitStruct.Size             = mpuRegionConfigNBR13.regionLength;//区域长度
    MPU_InitStruct.AccessPermission = mpuRegionConfigNBR13.accessPermissions;//对该区域的访问级别设定
    MPU_InitStruct.DisableExec      = mpuRegionConfigNBR13.execState;//指令访问使能
    MPU_InitStruct.TypeExtField     = mpuRegionConfigNBR13.texSet;//设置Cache策略
    MPU_InitStruct.IsBufferable     = mpuRegionConfigNBR13.bufferState;//是否使能指令缓存 B参数
    MPU_InitStruct.IsCacheable      = mpuRegionConfigNBR13.cacheState;//C参数
    MPU_InitStruct.IsShareable      = mpuRegionConfigNBR13.shareState;//是否使能共享
    MPU_InitStruct.Number           = MPU_REGION_NUMBER13;//MPU区域,15优先级最高,0优先级最低
    MPU_InitStruct.SubRegionDisable = mpuRegionConfigNBR13.subRegionEnableConfig;//控制MPU特定子区域是否使能
    //实际配置
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    //区域14
    MPU_InitStruct.Enable           = mpuRegionConfigNBR14.regionState;//是否使能
    MPU_InitStruct.BaseAddress      = mpuRegionConfigNBR14.baseAddr;//地址
    MPU_InitStruct.Size             = mpuRegionConfigNBR14.regionLength;//区域长度
    MPU_InitStruct.AccessPermission = mpuRegionConfigNBR14.accessPermissions;//对该区域的访问级别设定
    MPU_InitStruct.DisableExec      = mpuRegionConfigNBR14.execState;//指令访问使能
    MPU_InitStruct.TypeExtField     = mpuRegionConfigNBR14.texSet;//设置Cache策略
    MPU_InitStruct.IsBufferable     = mpuRegionConfigNBR14.bufferState;//是否使能指令缓存 B参数
    MPU_InitStruct.IsCacheable      = mpuRegionConfigNBR14.cacheState;//C参数
    MPU_InitStruct.IsShareable      = mpuRegionConfigNBR14.shareState;//是否使能共享
    MPU_InitStruct.Number           = MPU_REGION_NUMBER14;//MPU区域,15优先级最高,0优先级最低
    MPU_InitStruct.SubRegionDisable = mpuRegionConfigNBR14.subRegionEnableConfig;//控制MPU特定子区域是否使能
    //实际配置
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    //区域15
    MPU_InitStruct.Enable           = mpuRegionConfigNBR15.regionState;//是否使能
    MPU_InitStruct.BaseAddress      = mpuRegionConfigNBR15.baseAddr;//地址
    MPU_InitStruct.Size             = mpuRegionConfigNBR15.regionLength;//区域长度
    MPU_InitStruct.AccessPermission = mpuRegionConfigNBR15.accessPermissions;//对该区域的访问级别设定
    MPU_InitStruct.DisableExec      = mpuRegionConfigNBR15.execState;//指令访问使能
    MPU_InitStruct.TypeExtField     = mpuRegionConfigNBR15.texSet;//设置Cache策略
    MPU_InitStruct.IsBufferable     = mpuRegionConfigNBR15.bufferState;//是否使能指令缓存 B参数
    MPU_InitStruct.IsCacheable      = mpuRegionConfigNBR15.cacheState;//C参数
    MPU_InitStruct.IsShareable      = mpuRegionConfigNBR15.shareState;//是否使能共享
    MPU_InitStruct.Number           = MPU_REGION_NUMBER15;//MPU区域,15优先级最高,0优先级最低
    MPU_InitStruct.SubRegionDisable = mpuRegionConfigNBR15.subRegionEnableConfig;//控制MPU特定子区域是否使能
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





