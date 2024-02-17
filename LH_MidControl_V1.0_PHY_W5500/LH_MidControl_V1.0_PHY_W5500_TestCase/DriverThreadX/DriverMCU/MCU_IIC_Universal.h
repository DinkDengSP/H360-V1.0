/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:05:46 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 20:25:58 +0800
************************************************************************************************/ 
#ifndef __MCU_IIC_UNIVERSAL_H_
#define __MCU_IIC_UNIVERSAL_H_
#include "CoreUtil.h"
#include "MCU_IIC_DriverConfig.h"

//IIC读写操作定义,在地址操作的时候,地址位的最低位区分读写
#define MCU_IIC_WRITE       0X00
#define MCU_IIC_READ        0X01

//SDA输入输出方向设置
typedef enum SDA_DIR
{
    SDA_DIR_IN      = 0X00,
    SDA_DIR_OUT     = 0X01,
}SDA_DIR;

//IIC的ACK状态
typedef enum IIC_ACK_STATE
{
    IIC_ACK_NONE    = 0X00,
    IIC_ACK_OK      = 0X01,
}IIC_ACK_STATE;

//IIC设备次级地址
typedef enum IIC_SUB_ADDR_SIZE
{
    IIC_SUB_ADDR_SIZE_8BIT   = 0x00,//8BIT地址
    IIC_SUB_ADDR_SIZE_16BIT  = 0x01,//16BIT地址
}IIC_SUB_ADDR_SIZE;



#endif

