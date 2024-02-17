/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-03-18 10:15:54 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-03-29 16:10:55 +0800
************************************************************************************************/ 
#ifndef __MCU_MPU_H_
#define __MCU_MPU_H_
#include "CoreUtil.h"

//MPU区域功能设置
typedef enum MPU_REGION_STATE
{
    MPU_REGION_STATE_DISABLE    = MPU_REGION_DISABLE,//禁用
    MPU_REGION_STATE_ENABLE     = MPU_REGION_ENABLE,//使能
}MPU_REGION_STATE;

//MPU管理的地址长度
typedef enum MPU_REGION_LENGTH
{
    MPU_REGION_LENGTH_32B   = MPU_REGION_SIZE_32B,  
    MPU_REGION_LENGTH_64B   = MPU_REGION_SIZE_64B,  
    MPU_REGION_LENGTH_128B  = MPU_REGION_SIZE_128B, 
    MPU_REGION_LENGTH_256B  = MPU_REGION_SIZE_256B, 
    MPU_REGION_LENGTH_512B  = MPU_REGION_SIZE_512B, 
    MPU_REGION_LENGTH_1KB   = MPU_REGION_SIZE_1KB,  
    MPU_REGION_LENGTH_2KB   = MPU_REGION_SIZE_2KB,  
    MPU_REGION_LENGTH_4KB   = MPU_REGION_SIZE_4KB,  
    MPU_REGION_LENGTH_8KB   = MPU_REGION_SIZE_8KB,  
    MPU_REGION_LENGTH_16KB  = MPU_REGION_SIZE_16KB, 
    MPU_REGION_LENGTH_32KB  = MPU_REGION_SIZE_32KB, 
    MPU_REGION_LENGTH_64KB  = MPU_REGION_SIZE_64KB, 
    MPU_REGION_LENGTH_128KB = MPU_REGION_SIZE_128KB,
    MPU_REGION_LENGTH_256KB = MPU_REGION_SIZE_256KB,
    MPU_REGION_LENGTH_512KB = MPU_REGION_SIZE_512KB,
    MPU_REGION_LENGTH_1MB   = MPU_REGION_SIZE_1MB,  
    MPU_REGION_LENGTH_2MB   = MPU_REGION_SIZE_2MB,  
    MPU_REGION_LENGTH_4MB   = MPU_REGION_SIZE_4MB,  
    MPU_REGION_LENGTH_8MB   = MPU_REGION_SIZE_8MB,  
    MPU_REGION_LENGTH_16MB  = MPU_REGION_SIZE_16MB, 
    MPU_REGION_LENGTH_32MB  = MPU_REGION_SIZE_32MB, 
    MPU_REGION_LENGTH_64MB  = MPU_REGION_SIZE_64MB, 
    MPU_REGION_LENGTH_128MB = MPU_REGION_SIZE_128MB,
    MPU_REGION_LENGTH_256MB = MPU_REGION_SIZE_256MB,
    MPU_REGION_LENGTH_512MB = MPU_REGION_SIZE_512MB,
    MPU_REGION_LENGTH_1GB   = MPU_REGION_SIZE_1GB,  
    MPU_REGION_LENGTH_2GB   = MPU_REGION_SIZE_2GB,  
    MPU_REGION_LENGTH_4GB   = MPU_REGION_SIZE_4GB,  
}MPU_REGION_LENGTH;

//MPU访问权限
typedef enum MPU_REGION_ACCESS_PERMISSIONS
{
    MPU_REGION_ACCESS_PERMISSIONS_NO_ACCESS   = MPU_REGION_NO_ACCESS,//普通级特权级都禁止访问
    MPU_REGION_ACCESS_PERMISSIONS_PRIV_RW     = MPU_REGION_PRIV_RW,//特权级可读写,普通级禁止访问
    MPU_REGION_ACCESS_PERMISSIONS_PRIV_RW_URO = MPU_REGION_PRIV_RW_URO,//特权级可读写普通级只读
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS = MPU_REGION_FULL_ACCESS,//特权级普通级均可读写
    MPU_REGION_ACCESS_PERMISSIONS_PRIV_RO     = MPU_REGION_PRIV_RO,//特权级只读,普通级不可访问
    MPU_REGION_ACCESS_PERMISSIONS_PRIV_RO_URO = MPU_REGION_PRIV_RO_URO,//特权级和普通级均只读
}MPU_REGION_ACCESS_PERMISSIONS;

//该区域是否可以运行代码
typedef enum MPU_EXEC_STATE
{
    MPU_EXEC_STATE_DISABLE = MPU_INSTRUCTION_ACCESS_DISABLE,//不可以执行代码
    MPU_EXEC_STATE_ENABLE  = MPU_INSTRUCTION_ACCESS_ENABLE,//可以执行代码
}MPU_EXEC_STATE;

//该区域的Cache策略
typedef enum MPU_TEX_SET
{
    MPU_TEX_SET_LEVEL0 = MPU_TEX_LEVEL0,
    MPU_TEX_SET_LEVEL1 = MPU_TEX_LEVEL1,
    MPU_TEX_SET_LEVEL2 = MPU_TEX_LEVEL2,
}MPU_TEX_SET;

//CACHE模式下是否使用缓冲
typedef enum MPU_BUFFER_STATE
{
    MPU_BUFFER_STATE_DISABLE = MPU_ACCESS_NOT_BUFFERABLE,//不使用缓冲
    MPU_BUFFER_STATE_ENABLE  = MPU_ACCESS_BUFFERABLE,//使用缓冲
}MPU_BUFFER_STATE;

//是否开启cache功能
typedef enum MPU_CACHE_STATE
{
    MPU_CACHE_STATE_DISABLE = MPU_ACCESS_NOT_CACHEABLE,//不开启cache功能
    MPU_CACHE_STATE_ENABLE  = MPU_ACCESS_CACHEABLE,//开启cache功能
}MPU_CACHE_STATE;

//Cache模式下是否多核共享内存区域
typedef enum MPU_SHARE_STATE
{
    MPU_SHARE_STATE_DISABLE = MPU_ACCESS_NOT_SHAREABLE,//不开启共享
    MPU_SHARE_STATE_ENABLE  = MPU_ACCESS_SHAREABLE,//开启共享
}MPU_SHARE_STATE;

//cache更新配置
typedef enum CACHE_REFLUSH_CONFIG
{
    CACHE_WRITE_BACK_ONLY   = 0x00,//将CACHE中的数据写回到SRAM中
    CACHE_READ_RELOAD_ONLY  = 0x01,//丢弃掉当前Cache中的数据,从SRAM中重新加载数据,可能造成数据丢失
    CACHE_READ_WRITE        = 0x02,//将CACHE中的数据写回到SRAM并且重新加载到Cache中,消耗的时间最长
}CACHE_REFLUSH_CONFIG;

//MPU区域设定
typedef struct MPU_REGION_CONFIG
{
    MPU_REGION_STATE regionState;//MPU区域功能设置
    uint32_t baseAddr;                      //MPU管理的地址起点
    MPU_REGION_LENGTH regionLength;         //MPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS accessPermissions;//MPU访问权限
    MPU_EXEC_STATE execState;       //该区域是否可以运行代码
    MPU_TEX_SET texSet;             //该区域的Cache策略
    MPU_BUFFER_STATE bufferState;   //CACHE模式下是否使用缓冲
    MPU_CACHE_STATE cacheState;     //是否开启cache功能
    MPU_SHARE_STATE shareState;     //Cache模式下是否多核共享内存区域
    uint8_t subRegionEnableConfig;  //子区域控制,一共八个,一般设置为0X00
}MPU_REGION_CONFIG;

//外部导出十六个分区的设定,NBR15优先级最高
//数字越大优先级越高,高优先级可以覆盖低优先级配置
extern const MPU_REGION_CONFIG mpuRegionConfigNBR0;
extern const MPU_REGION_CONFIG mpuRegionConfigNBR1;
extern const MPU_REGION_CONFIG mpuRegionConfigNBR2;
extern const MPU_REGION_CONFIG mpuRegionConfigNBR3;
extern const MPU_REGION_CONFIG mpuRegionConfigNBR4;
extern const MPU_REGION_CONFIG mpuRegionConfigNBR5;
extern const MPU_REGION_CONFIG mpuRegionConfigNBR6;
extern const MPU_REGION_CONFIG mpuRegionConfigNBR7;
extern const MPU_REGION_CONFIG mpuRegionConfigNBR8;
extern const MPU_REGION_CONFIG mpuRegionConfigNBR9;
extern const MPU_REGION_CONFIG mpuRegionConfigNBR10;
extern const MPU_REGION_CONFIG mpuRegionConfigNBR11;
extern const MPU_REGION_CONFIG mpuRegionConfigNBR12;
extern const MPU_REGION_CONFIG mpuRegionConfigNBR13;
extern const MPU_REGION_CONFIG mpuRegionConfigNBR14;
extern const MPU_REGION_CONFIG mpuRegionConfigNBR15;

//初始化芯片MPU配置
void MCU_MPU_ConfigInit(void);

//使能L1缓存
void MCU_MPU_CacheEnable(void);

//禁用L1缓存
void MCU_MPU_CacheDisable(void);

//更新更新Cache
void MCU_MPU_CacheReflush(CACHE_REFLUSH_CONFIG config);

#endif


