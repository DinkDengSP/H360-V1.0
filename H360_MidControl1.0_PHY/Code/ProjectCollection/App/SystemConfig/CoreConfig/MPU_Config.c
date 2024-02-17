/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-02 11:02:53 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-15 13:49:13 +0800
************************************************************************************************/ 
#include "MPU_Config.h"

//MPU组件区域设定,15优先级最高,优先级最低,高优先级覆盖低优先级
//Region15 AXI SRAM 作为D1域的DMA缓存存在
static const MPU_REGION_CONFIG mpuRegionConfigNbr15 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0x24000000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_32KB,                        //MPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL0,                         //该区域的Cache策略
    MPU_BUFFER_SET_DISABLE,                     //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_DISABLE,                      //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region14 AXI SRAM 作为系统常规内存使用,打开了CACHE功能
static const MPU_REGION_CONFIG mpuRegionConfigNbr14 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0x24000000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_512KB,                       //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                         //该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                      //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                       //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region13 配置D2_SRAM1的一块DMA_MPU属性
static const MPU_REGION_CONFIG mpuRegionConfigNbr13 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0x30000000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_32KB,                        //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL0,                         //该区域的Cache策略
    MPU_BUFFER_SET_DISABLE,                     //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_DISABLE,                      //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region12 配置D2_SRAM1的MPU属性
static const MPU_REGION_CONFIG mpuRegionConfigNbr12 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0x30000000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_128KB,                       //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                         //该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                      //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                       //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region11 配置D2_SRAM2的MPU属性
static const MPU_REGION_CONFIG mpuRegionConfigNbr11 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0x30020000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_128KB,                       //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                         //该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                      //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                       //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region10 配置D2_SRAM3的MPU属性
static const MPU_REGION_CONFIG mpuRegionConfigNbr10 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0x30040000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_32KB,                        //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL0,                         //该区域的Cache策略
    MPU_BUFFER_SET_DISABLE,                     //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_DISABLE,                      //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region9 配置D3_SRAM4的MPU属性 DMA
static const MPU_REGION_CONFIG mpuRegionConfigNbr9 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0x38000000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_16KB,                        //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL0,                         //该区域的Cache策略
    MPU_BUFFER_SET_DISABLE,                     //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_DISABLE,                      //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region8 配置D3_SRAM4的MPU属性
static const MPU_REGION_CONFIG mpuRegionConfigNbr8 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0x38000000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_64KB,                        //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                         //该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                      //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                       //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region7 BK_RAM
static const MPU_REGION_CONFIG mpuRegionConfigNbr7 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0x38800000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_4KB,                         //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL0,                         //该区域的Cache策略
    MPU_BUFFER_SET_DISABLE,                      //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_DISABLE,                       //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region5 SDRAM1的MPU配置
static const MPU_REGION_CONFIG mpuRegionConfigNbr6 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0xC0000000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_32MB,                        //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                         //该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                      //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                       //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region5 SDRAM2的MPU配置
static const MPU_REGION_CONFIG mpuRegionConfigNbr5 = {
    MPU_REGION_SET_ENABLE,                      //MPU区域功能设置
    0xD0000000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_32MB,                        //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL1,                         //该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                      //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                       //是否开启cache功能
    MPU_SHARE_SET_DISABLE,                      //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region4 
static const MPU_REGION_CONFIG mpuRegionConfigNbr4 = {
    MPU_REGION_SET_DISABLE,                     //MPU区域功能设置
    0x00000000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_4GB,                         //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL0,                         //该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                      //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                       //是否开启cache功能
    MPU_SHARE_SET_ENABLE,                       //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region3 
static const MPU_REGION_CONFIG mpuRegionConfigNbr3 = {
    MPU_REGION_SET_DISABLE,                     //MPU区域功能设置
    0x00000000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_4GB,                         //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL0,                         //该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                      //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                       //是否开启cache功能
    MPU_SHARE_SET_ENABLE,                       //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region2
static const MPU_REGION_CONFIG mpuRegionConfigNbr2 = {
    MPU_REGION_SET_DISABLE,                     //MPU区域功能设置
    0x00000000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_4GB,                         //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL0,                         //该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                      //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                       //是否开启cache功能
    MPU_SHARE_SET_ENABLE,                       //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region1
static const MPU_REGION_CONFIG mpuRegionConfigNbr1 = {
    MPU_REGION_SET_DISABLE,                     //MPU区域功能设置
    0x00000000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_4GB,                         //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL0,                         //该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                      //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                       //是否开启cache功能
    MPU_SHARE_SET_ENABLE,                       //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//Region0
static const MPU_REGION_CONFIG mpuRegionConfigNbr0 = {
    MPU_REGION_SET_DISABLE,                     //MPU区域功能设置
    0x00000000,                                 //MPU管理的地址起点
    MPU_REGION_LEN_4GB,                         //MMPU管理的地址长度
    MPU_REGION_ACCESS_PERMISSIONS_FULL_ACCESS,  //MPU访问权限
    MPU_EXEC_SET_ENABLE,                        //该区域是否可以运行代码
    MPU_TEX_SET_LEVEL0,                         //该区域的Cache策略
    MPU_BUFFER_SET_ENABLE,                      //CACHE模式下是否使用缓冲
    MPU_CACHE_SET_ENABLE,                       //是否开启cache功能
    MPU_SHARE_SET_ENABLE,                       //是否多核共享内存区域
    0x00,                                       //子区域控制,一共八个,一般设置为0X00
};

//MPU管理数组
MPU_REGION_CONFIG* mpuRegionConfigPtrArray[MPU_REGION_COUNT] = {
    (MPU_REGION_CONFIG*)&mpuRegionConfigNbr0,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNbr1,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNbr2,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNbr3,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNbr4,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNbr5,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNbr6,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNbr7,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNbr8,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNbr9,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNbr10,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNbr11,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNbr12,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNbr13,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNbr14,
    (MPU_REGION_CONFIG*)&mpuRegionConfigNbr15,
};



