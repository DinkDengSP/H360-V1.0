/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-08-08 13:45:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-08-17 11:57:29 +0800
************************************************************************************************/ 
#ifndef __MCU_ETH_H_
#define __MCU_ETH_H_
#include "CoreUtil.h"
#include "ETH_Config.h"

#define MCU_ETH_MODE_MII                       0
#define MCU_ETH_MODE_RMII                      1

//用户选择的以太网模式
#define MCU_ETH_MODE_USER                       MCU_ETH_MODE_MII 

//MCU动态内存匹配对象
#define MCU_ETH_MEM_REGION                      MEM_SDRAM1
//是否需要CACHE更新机制
#define MCU_ETH_ENABLE_CACHE_INVALID_STRATEGY   0
//ETH单包数据发送超时时间
#define TIME_OUT_MS_ETH_SEND                    1000
//双工模式不支持
#define PHY_DUPLEX_MODE_ETH_INVALID             0XFFFFFFFF
//速度不支持
#define PHY_SPEED_ETH_INVALID                   0XFFFFFFFF

//网卡连接状态
typedef enum NET_PORT_STATE
{
    NET_PORT_STATE_LINK_DOWN             = 0X00,//网线未连接
    NET_PORT_STATE_100MBITS_FULLDUPLEX   = 0X01,//100M 全双工
    NET_PORT_STATE_100MBITS_HALFDUPLEX   = 0X02,//100M 半双工
    NET_PORT_STATE_10MBITS_FULLDUPLEX    = 0X03,//10M 全双工
    NET_PORT_STATE_10MBITS_HALFDUPLEX    = 0X04,//10M 半双工
    NET_PORT_STATE_AUTONEGO_NOTDONE      = 0X05,//自动协商未完成
    NET_PORT_STATE_UNKNOW                = 0XFF,//网络接口状态未知
}NET_PORT_STATE;

//多包发送结构体
typedef struct ETH_MULTI_PACK
{
    uint8_t* dataBufferPtrArray[ETH_TX_DESC_CNT];
    uint16_t dataLengthArray[ETH_TX_DESC_CNT];
    uint16_t packCount;
}ETH_MULTI_PACK;


//导出控制器句柄
extern ETH_HandleTypeDef ETH_Handler;

//释放接收到的内存
void MCU_ETH_ReleaseRecvBuf(uint8_t* recvBufPtr);

//端口初始化
LH_ERR MCU_ETH_PortInit(void);

//使能多播
void MCU_ETH_EnableMultiCast(void);

//禁止多播
void MCU_ETH_DisableMultiCast(void);

//根据网卡连接状态修改MAC状态
void MCU_ETH_ReflushModeSpeed(NET_PORT_STATE netPortState);

//读取PHY 寄存器数据
LH_ERR MCU_ETH_ReadPhyReg(uint32_t phyAddr, uint32_t regAddr, uint32_t *readValuePtr);

//写入PHY 寄存器数据
LH_ERR MCU_ETH_WritePhyReg(uint32_t phyAddr, uint32_t regAddr, uint32_t writeValue);

//写入网卡发送数据
LH_ERR MCU_ETH_WriteMultiPack(ETH_MULTI_PACK* multiPackPtr);

//读取网卡接收到的数据
void MCU_ETH_ReadRecvData(uint8_t** recvBuf,uint32_t* length);

//端口开始
void MCU_ETH_Start(void);

//端口停止
void MCU_ETH_Stop(void);


#endif




