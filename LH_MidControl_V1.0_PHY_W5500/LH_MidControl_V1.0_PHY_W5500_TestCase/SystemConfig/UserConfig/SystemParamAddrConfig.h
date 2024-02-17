/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-10-27 18:11:51 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-08 13:54:16 +0800
************************************************************************************************/ 
#ifndef __SYSTEM_PARAM_ADDR_CONFIG_H_
#define __SYSTEM_PARAM_ADDR_CONFIG_H_


/*--------------------------板卡参数统一四个字节,存放在AT24CXX芯片中----------------------------*/
//AT24CXX自检标记
#define ADDR_BOARD_PARAM_AT24CXX_CHECK                  0x00
//SRAM自检标记      
#define ADDR_BOARD_PARAM_SRAM_CHECK                     0X04
//BOOT版本号,开发版本       
#define ADDR_BOARD_PARAM_BOOT_VERSION_DEV               0X08
//BOOT版本号,临时版本       
#define ADDR_BOARD_PARAM_BOOT_VERSION_TEMP              0X0C
//BOOT版本号,子版本版本     
#define ADDR_BOARD_PARAM_BOOT_VERSION_SUB               0X10
//BOOT版本号,主版本     
#define ADDR_BOARD_PARAM_BOOT_VERSION_MAIN              0X14
//APP版本号,开发版本        
#define ADDR_BOARD_PARAM_APP_VERSION_DEV                0X18
//APP版本号,临时版本        
#define ADDR_BOARD_PARAM_APP_VERSION_TEMP               0X1C
//APP版本号,子版本      
#define ADDR_BOARD_PARAM_APP_VERSION_SUB                0X20
//APP版本号,主版本      
#define ADDR_BOARD_PARAM_APP_VERSION_MAIN               0X24
//NAND FLASH低格标记        
#define ADDR_BOARD_PARAM_NAND_LOW_LEVEL_FORMAT          0X28
//W25Q64 FLASH低格标记
#define ADDR_BOARD_PARAM_W25Q64_LOW_LEVEL_FORMAT        0X2C
//W25Q64 自检标记      
#define ADDR_BOARD_PARAM_W25Q64_CHECK                   0X30
//W25Q256JV FLASH低格标记
#define ADDR_BOARD_PARAM_W25Q256JV_LOW_LEVEL_FORMAT     0X34
//W25Q256JV 自检标记      
#define ADDR_BOARD_PARAM_W25Q256JV_CHECK                0X38

//服务层参数写入域,使用AT24CXX,16KB用一半
#define SYSTEM_SRV_PARAM_REGION_MIN                     0X2000

#endif


