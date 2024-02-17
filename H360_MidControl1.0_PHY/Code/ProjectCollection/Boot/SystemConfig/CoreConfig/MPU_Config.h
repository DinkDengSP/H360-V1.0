/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-02 11:02:19 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-02 11:02:19 +0800
************************************************************************************************/ 
#ifndef __MPU_CONFIG_H_
#define __MPU_CONFIG_H_
#include "CoreUtil.h"

//是否是能MPU写透功能
#define MPU_ENABLE_WRITE_THROUGH        0
//MPU区域数量
#define MPU_REGION_COUNT                16

//MPU区域设置,15优先级最高,0优先级最低
typedef enum MPU_REGION_NO
{
    MPU_REGION_NO_0  = MPU_REGION_NUMBER0,
    MPU_REGION_NO_1  = MPU_REGION_NUMBER1,
    MPU_REGION_NO_2  = MPU_REGION_NUMBER2,
    MPU_REGION_NO_3  = MPU_REGION_NUMBER3,
    MPU_REGION_NO_4  = MPU_REGION_NUMBER4,
    MPU_REGION_NO_5  = MPU_REGION_NUMBER5,
    MPU_REGION_NO_6  = MPU_REGION_NUMBER6,
    MPU_REGION_NO_7  = MPU_REGION_NUMBER7,
    MPU_REGION_NO_8  = MPU_REGION_NUMBER8,
    MPU_REGION_NO_9  = MPU_REGION_NUMBER9,
    MPU_REGION_NO_10 = MPU_REGION_NUMBER10,
    MPU_REGION_NO_11 = MPU_REGION_NUMBER11,
    MPU_REGION_NO_12 = MPU_REGION_NUMBER12,
    MPU_REGION_NO_13 = MPU_REGION_NUMBER13,
    MPU_REGION_NO_14 = MPU_REGION_NUMBER14,
    MPU_REGION_NO_15 = MPU_REGION_NUMBER15,
}MPU_REGION_NO;

//MPU区域开关
typedef enum MPU_REGION_SET
{
    MPU_REGION_SET_DISABLE = MPU_REGION_DISABLE,//禁用
    MPU_REGION_SET_ENABLE  = MPU_REGION_ENABLE,//使能
}MPU_REGION_SET;

//MPU管理的地址长度
typedef enum MPU_REGION_LEN
{
    MPU_REGION_LEN_32B   = MPU_REGION_SIZE_32B,  
    MPU_REGION_LEN_64B   = MPU_REGION_SIZE_64B,  
    MPU_REGION_LEN_128B  = MPU_REGION_SIZE_128B, 
    MPU_REGION_LEN_256B  = MPU_REGION_SIZE_256B, 
    MPU_REGION_LEN_512B  = MPU_REGION_SIZE_512B, 
    MPU_REGION_LEN_1KB   = MPU_REGION_SIZE_1KB,  
    MPU_REGION_LEN_2KB   = MPU_REGION_SIZE_2KB,  
    MPU_REGION_LEN_4KB   = MPU_REGION_SIZE_4KB,  
    MPU_REGION_LEN_8KB   = MPU_REGION_SIZE_8KB,  
    MPU_REGION_LEN_16KB  = MPU_REGION_SIZE_16KB, 
    MPU_REGION_LEN_32KB  = MPU_REGION_SIZE_32KB, 
    MPU_REGION_LEN_64KB  = MPU_REGION_SIZE_64KB, 
    MPU_REGION_LEN_128KB = MPU_REGION_SIZE_128KB,
    MPU_REGION_LEN_256KB = MPU_REGION_SIZE_256KB,
    MPU_REGION_LEN_512KB = MPU_REGION_SIZE_512KB,
    MPU_REGION_LEN_1MB   = MPU_REGION_SIZE_1MB,  
    MPU_REGION_LEN_2MB   = MPU_REGION_SIZE_2MB,  
    MPU_REGION_LEN_4MB   = MPU_REGION_SIZE_4MB,  
    MPU_REGION_LEN_8MB   = MPU_REGION_SIZE_8MB,  
    MPU_REGION_LEN_16MB  = MPU_REGION_SIZE_16MB, 
    MPU_REGION_LEN_32MB  = MPU_REGION_SIZE_32MB, 
    MPU_REGION_LEN_64MB  = MPU_REGION_SIZE_64MB, 
    MPU_REGION_LEN_128MB = MPU_REGION_SIZE_128MB,
    MPU_REGION_LEN_256MB = MPU_REGION_SIZE_256MB,
    MPU_REGION_LEN_512MB = MPU_REGION_SIZE_512MB,
    MPU_REGION_LEN_1GB   = MPU_REGION_SIZE_1GB,  
    MPU_REGION_LEN_2GB   = MPU_REGION_SIZE_2GB,  
    MPU_REGION_LEN_4GB   = MPU_REGION_SIZE_4GB,  
}MPU_REGION_LEN;

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
typedef enum MPU_EXEC_SET
{
    MPU_EXEC_SET_DISABLE = MPU_INSTRUCTION_ACCESS_DISABLE,//不可以执行代码
    MPU_EXEC_SET_ENABLE  = MPU_INSTRUCTION_ACCESS_ENABLE,//可以执行代码
}MPU_EXEC_SET;

//该区域的Cache策略
typedef enum MPU_TEX_SET
{
    MPU_TEX_SET_LEVEL0 = MPU_TEX_LEVEL0,
    MPU_TEX_SET_LEVEL1 = MPU_TEX_LEVEL1,
    MPU_TEX_SET_LEVEL2 = MPU_TEX_LEVEL2,
}MPU_TEX_SET;

//CACHE模式下是否使用缓冲
typedef enum MPU_BUFFER_SET
{
    MPU_BUFFER_SET_DISABLE = MPU_ACCESS_NOT_BUFFERABLE,//不使用缓冲
    MPU_BUFFER_SET_ENABLE  = MPU_ACCESS_BUFFERABLE,//使用缓冲
}MPU_BUFFER_SET;

//是否开启cache功能
typedef enum MPU_CACHE_SET
{
    MPU_CACHE_SET_DISABLE = MPU_ACCESS_NOT_CACHEABLE,//不开启cache功能
    MPU_CACHE_SET_ENABLE  = MPU_ACCESS_CACHEABLE,//开启cache功能
}MPU_CACHE_SET;

//Cache模式下是否多核共享内存区域
typedef enum MPU_SHARE_SET
{
    MPU_SHARE_SET_DISABLE = MPU_ACCESS_NOT_SHAREABLE,//不开启共享
    MPU_SHARE_SET_ENABLE  = MPU_ACCESS_SHAREABLE,//开启共享
}MPU_SHARE_SET;

//MPU区域设定
typedef struct MPU_REGION_CONFIG
{
    MPU_REGION_SET enableSet;   //MPU区域功能设置是否启用
    uint32_t baseAddr;              //MPU管理的地址起点
    MPU_REGION_LEN regionLen;       //MPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS accessPermissions;//MPU访问权限
    MPU_EXEC_SET execSet;       //该区域是否可以运行代码
    MPU_TEX_SET texSet;         //该区域的Cache策略
    MPU_BUFFER_SET bufferSet;   //CACHE模式下是否使用缓冲
    MPU_CACHE_SET cacheSet;     //是否开启cache功能
    MPU_SHARE_SET shareSet;     //Cache模式下是否多核共享内存区域
    uint8_t subRegionEnableConfig;  //子区域控制,一共八个,一般设置为0X00
}MPU_REGION_CONFIG;

#endif