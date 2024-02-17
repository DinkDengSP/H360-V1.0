/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:10:51 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-25 10:23:01 +0800
************************************************************************************************/ 
#ifndef __BOARD_W5500_BASE_H_
#define __BOARD_W5500_BASE_H_
#include "BoardW5500_Config.h"
#include "BoardW5500_DriverConfig.h"

//W5500端口初始化
void BoardW5500_PortInit(void);

//获取设备互斥信号量
void BoardW5500_GetLock(void);

//释放设备互斥信号量
void BoardW5500_ReleaseLock(void);

/*----------------------------------基础功能函数-----------------------------------*/
//从SOCKET获取寄存器block
W5500_CONTROL_BYTE_SEL BoardW5500_GetRegBlockBySocket(W5500_SOCKET_NO socketNo);

//从SOCKET获取TX_BUF
W5500_CONTROL_BYTE_SEL BoardW5500_GetTxBufBlockBySocket(W5500_SOCKET_NO socketNo);

//从SOCKET获取RX_BUF
W5500_CONTROL_BYTE_SEL BoardW5500_GetRxBufBlockBySocket(W5500_SOCKET_NO socketNo);

//W5500写入基础寄存器
ERROR_SUB BoardW5500_WriteBaseRegBuf(uint16_t regStartAddr,uint8_t* regValBuf,uint16_t valLen);

//W5500读取基础寄存器
ERROR_SUB BoardW5500_ReadBaseRegBuf(uint16_t regStartAddr,uint8_t* regValBuf,uint16_t valLen);

//W5500写入Socket寄存器
ERROR_SUB BoardW5500_WriteSocketRegBuf(W5500_SOCKET_NO socketNo,uint16_t regStartAddr,uint8_t* regValBuf,uint16_t valLen);

//W5500读取Socket寄存器
ERROR_SUB BoardW5500_ReadSocketRegBuf(W5500_SOCKET_NO socketNo,uint16_t regStartAddr,uint8_t* regValBuf,uint16_t valLen);

//W5500写入Socket TX BUF
ERROR_SUB BoardW5500_WriteSocketTxBuf(W5500_SOCKET_NO socketNo,uint16_t regStartAddr,uint8_t* regValBuf,uint16_t valLen);

//W5500读取Socket TX BUF
ERROR_SUB BoardW5500_ReadSocketTxBuf(W5500_SOCKET_NO socketNo,uint16_t regStartAddr,uint8_t* regValBuf,uint16_t valLen);

//W5500写入Socket RX BUF
ERROR_SUB BoardW5500_WriteSocketRxBuf(W5500_SOCKET_NO socketNo,uint16_t regStartAddr,uint8_t* regValBuf,uint16_t valLen);

//W5500读取Socket RX BUF
ERROR_SUB BoardW5500_ReadSocketRxBuf(W5500_SOCKET_NO socketNo,uint16_t regStartAddr,uint8_t* regValBuf,uint16_t valLen);


/*----------------------------------------BASE寄存器操作---------------------------------------*/
//检查W5500是否存在
ERROR_SUB BoardW5500_CheckVersion(void);

//写入模式寄存器
ERROR_SUB BoardW5500_WriteBaseModeReg(uint8_t regValue);

//读取模式寄存器
ERROR_SUB BoardW5500_ReadBaseModeReg(uint8_t* regValuePtr);

//写入网关 IP 地址寄存器     
ERROR_SUB BoardW5500_WriteBaseGatewayAddressReg(uint32_t gatewayAddress);

//读取网关 IP 地址寄存器     
ERROR_SUB BoardW5500_ReadBaseGatewayAddressReg(uint32_t* gatewayAddressPtr);

//写入子网掩码寄存器
ERROR_SUB BoardW5500_WriteBaseSubNetMaskAddressReg(uint32_t subNetMaskAddress);

//读取子网掩码寄存器
ERROR_SUB BoardW5500_ReadBaseSubNetMaskAddressReg(uint32_t* subNetMaskAddressPtr);

//写入源 MAC 地址寄存器  
ERROR_SUB BoardW5500_WriteBaseSourceMacAddrReg(uint8_t macAddr1,uint8_t macAddr2,uint8_t macAddr3,uint8_t macAddr4,
                                                    uint8_t macAddr5,uint8_t macAddr6);

//读取源 MAC 地址寄存器  
ERROR_SUB BoardW5500_ReadBaseSourceMacAddrReg(uint8_t* macAddr1Ptr,uint8_t* macAddr2Ptr,uint8_t* macAddr3Ptr,uint8_t* macAddr4Ptr,
                                                    uint8_t* macAddr5Ptr,uint8_t* macAddr6Ptr);

//写入源 IP 地址寄存器  
ERROR_SUB BoardW5500_WriteBaseSourceIpAddrReg(uint32_t ipAddress);

//读取源 IP 地址寄存器  
ERROR_SUB BoardW5500_ReadBaseSourceIpAddrReg(uint32_t* ipAddressPtr);

//写入低电平中断定时器寄存器
ERROR_SUB BoardW5500_WriteBaseIntLevelTimeReg(uint16_t intLevelTime);

//读取低电平中断定时器寄存器
ERROR_SUB BoardW5500_ReadBaseIntLevelTimeReg(uint16_t* intLevelTimePtr);

//写入中断寄存器
ERROR_SUB BoardW5500_WriteBaseIntReg(uint8_t regValue);

//读取中断寄存器
ERROR_SUB BoardW5500_ReadBaseIntReg(uint8_t* regValuePtr);

//写入中断屏蔽寄存器
ERROR_SUB BoardW5500_WriteBaseIntMaskReg(uint8_t regValue);

//读取中断屏蔽寄存器
ERROR_SUB BoardW5500_ReadBaseIntMaskReg(uint8_t* regValuePtr);

//写入Socket 中断寄存器      
ERROR_SUB BoardW5500_WriteBaseSocketIntReg(uint8_t regValue);

//读取Socket 中断寄存器      
ERROR_SUB BoardW5500_ReadBaseSocketIntReg(uint8_t* regValuePtr);

//写入Socket 中断屏蔽寄存器
ERROR_SUB BoardW5500_WriteBaseSocketIntMaskReg(uint8_t regValue);

//读取Socket 中断屏蔽寄存器
ERROR_SUB BoardW5500_ReadBaseSocketIntMaskReg(uint8_t* regValuePtr);

//写入RTR 重试时间值寄存器
ERROR_SUB BoardW5500_WriteBaseRtrReg(uint16_t rtrMs);

//读取RTR 重试时间值寄存器
ERROR_SUB BoardW5500_ReadBaseRtrReg(uint16_t* rtrMsPtr);

//写入RCR 重试计数寄存器 
ERROR_SUB BoardW5500_WriteBaseRcrReg(uint8_t rcrReg);

//读取RCR 重试计数寄存器 
ERROR_SUB BoardW5500_ReadBaseRcrReg(uint8_t* rcrRegPtr);

//写入PTIMER PPP 连接控制协议请求定时寄存器  
ERROR_SUB BoardW5500_WriteBasePtimerReg(uint8_t ptimerMs);

//读取PTIMER PPP 连接控制协议请求定时寄存器  
ERROR_SUB BoardW5500_ReadBasePtimerReg(uint8_t* ptimerMsPtr);

//写入PMAGIC PPP 连接控制协议幻数寄存器    
ERROR_SUB BoardW5500_WriteBasePMagicReg(uint8_t magicNumber);

//读取PMAGIC PPP 连接控制协议幻数寄存器    
ERROR_SUB BoardW5500_ReadBasePMagicReg(uint8_t* magicNumberPtr);

//写入PHAR PPPoE 模式下目标 MAC 寄存器 
ERROR_SUB BoardW5500_WriteBasePoeTargetMacAddrReg(uint8_t macAddr1,uint8_t macAddr2,uint8_t macAddr3,uint8_t macAddr4,
                                                    uint8_t macAddr5,uint8_t macAddr6);

//读取PHAR PPPoE 模式下目标 MAC 寄存器 
ERROR_SUB BoardW5500_ReadBasePoeTargetMacAddrReg(uint8_t* macAddr1Ptr,uint8_t* macAddr2Ptr,uint8_t* macAddr3Ptr,uint8_t* macAddr4Ptr,
                                                    uint8_t* macAddr5Ptr,uint8_t* macAddr6Ptr);

//写入PSID PPPoE 模式下会话 ID 寄存器 
ERROR_SUB BoardW5500_WriteBasePoeSidReg(uint16_t sid);

//读取PSID PPPoE 模式下会话 ID 寄存器 
ERROR_SUB BoardW5500_ReadBasePoeSidReg(uint16_t* sidPtr);

//写入PMRU PPPoE 模式下最大接收单元   
ERROR_SUB BoardW5500_WriteBasePoeMaxRecvUnitReg(uint16_t maxRecvUnit);

//读取PMRU PPPoE 模式下最大接收单元   
ERROR_SUB BoardW5500_ReadBasePoeMaxRecvUnitReg(uint16_t* maxRecvUnitPtr);

//读取UIPR 无法抵达 IP 地址寄存器          
ERROR_SUB BoardW5500_ReadUnArrivedIpReg(uint32_t* ipValue);

//读取UPORTR 无法抵达端口寄存器
ERROR_SUB BoardW5500_ReadUnArrivedPortReg(uint16_t* portValue);

//写入W5500 PHY 配置寄存器   
ERROR_SUB BoardW5500_WriteBasePhyConfigReg(uint8_t phyConfig);

//读取W5500 PHY 配置寄存器   
ERROR_SUB BoardW5500_ReadBasePhyConfigReg(uint8_t* phyConfigPtr);


/*-----------------------------------SOCKET寄存器操作-------------------------------------*/
//写入Socket n模式寄存器
ERROR_SUB BoardW5500_WriteSocketModeReg(W5500_SOCKET_NO socketNo,uint8_t regValue);
//读取Socket n模式寄存器
ERROR_SUB BoardW5500_ReadSocketModeReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr);

//写入Socket n 配置寄存器
ERROR_SUB BoardW5500_WriteSocketConfigReg(W5500_SOCKET_NO socketNo,uint8_t regValue);
//读取Socket n 配置寄存器
ERROR_SUB BoardW5500_ReadSocketConfigReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr);

//写入Socket n 中断寄存器
ERROR_SUB BoardW5500_WriteSockeIntReg(W5500_SOCKET_NO socketNo,uint8_t regValue);
//读取Socket n 中断寄存器
ERROR_SUB BoardW5500_ReadSocketIntReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr);

//读取Socket n 状态寄存器
ERROR_SUB BoardW5500_ReadSocketStateReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr);

//写入Socket n 源端口寄存器
ERROR_SUB BoardW5500_WriteSocketSourcePortReg(W5500_SOCKET_NO socketNo,uint16_t portValue);
//读取Socket n 源端口寄存器
ERROR_SUB BoardW5500_ReadSocketSourcePortReg(W5500_SOCKET_NO socketNo,uint16_t* portValuePtr);

//写入Socket n目的 MAC 地址寄存器
ERROR_SUB BoardW5500_WriteSocketDestMacAddrReg(W5500_SOCKET_NO socketNo,uint8_t macAddr1,uint8_t macAddr2,uint8_t macAddr3,uint8_t macAddr4,
                                                    uint8_t macAddr5,uint8_t macAddr6);
//读取Socket n目的 MAC 地址寄存器
ERROR_SUB BoardW5500_ReadSocketDestMacAddrReg(W5500_SOCKET_NO socketNo,uint8_t* macAddr1Ptr,uint8_t* macAddr2Ptr,uint8_t* macAddr3Ptr,uint8_t* macAddr4Ptr,
                                                    uint8_t* macAddr5Ptr,uint8_t* macAddr6Ptr);

//写入Socket 目标 IP 地址寄存器
ERROR_SUB BoardW5500_WriteSocketDestIpReg(W5500_SOCKET_NO socketNo,uint32_t destIpValue);
//读取Socket 目标 IP 地址寄存器
ERROR_SUB BoardW5500_ReadSocketDestIpReg(W5500_SOCKET_NO socketNo,uint32_t* destIpValuePtr);

//写入Socket n 目标端口寄存器
ERROR_SUB BoardW5500_WriteSocketDestPortReg(W5500_SOCKET_NO socketNo,uint16_t portValue);
//读取Socket n 目标端口寄存器
ERROR_SUB BoardW5500_ReadSocketDestPortReg(W5500_SOCKET_NO socketNo,uint16_t* portValuePtr);

//写入Socket n-th 最大分段寄存器
ERROR_SUB BoardW5500_WriteSocketMssReg(W5500_SOCKET_NO socketNo,uint16_t mssValue);
//读取Socket n-th 最大分段寄存器
ERROR_SUB BoardW5500_ReadSocketMssReg(W5500_SOCKET_NO socketNo,uint16_t* mssValuePtr);

//写入Socket IP 服务类型寄存器
ERROR_SUB BoardW5500_WriteSocketTosReg(W5500_SOCKET_NO socketNo,uint8_t regValue);
//读取Socket IP 服务类型寄存器
ERROR_SUB BoardW5500_ReadSocketTosReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr);

//写入Socket IP 生存时间寄存器
ERROR_SUB BoardW5500_WriteSocketTtlReg(W5500_SOCKET_NO socketNo,uint8_t regValue);
//读取Socket IP 生存时间寄存器
ERROR_SUB BoardW5500_ReadSocketTtlReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr);

//写入Socket n 接收缓存大小寄存器
ERROR_SUB BoardW5500_WriteSocketRxMemSizeReg(W5500_SOCKET_NO socketNo,uint8_t regValue);
//读取Socket n 接收缓存大小寄存器
ERROR_SUB BoardW5500_ReadSocketRxMemSizeReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr);

//写入Socket n 发送缓存大小寄存器
ERROR_SUB BoardW5500_WriteSocketTxMemSizeReg(W5500_SOCKET_NO socketNo,uint8_t regValue);
//读取Socket n 发送缓存大小寄存器
ERROR_SUB BoardW5500_ReadSocketTxMemSizeReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr);

//写入Socket n 空闲发送缓存寄存器
ERROR_SUB BoardW5500_WriteSocketTxFreeSizeReg(W5500_SOCKET_NO socketNo,uint16_t regValue);
//读取Socket n 空闲发送缓存寄存器
ERROR_SUB BoardW5500_ReadSocketTxFreeSizeReg(W5500_SOCKET_NO socketNo,uint16_t* regValuePtr);

//写入Socket n 发送读指针寄存器
ERROR_SUB BoardW5500_WriteSocketTxReadPtrReg(W5500_SOCKET_NO socketNo,uint16_t regValue);
//读取Socket n 发送读指针寄存器
ERROR_SUB BoardW5500_ReadSocketTxReadPtrReg(W5500_SOCKET_NO socketNo,uint16_t* regValuePtr);

//写入Socket n 发送写指针寄存器
ERROR_SUB BoardW5500_WriteSocketTxWritePtrReg(W5500_SOCKET_NO socketNo,uint16_t regValue);
//读取Socket n 发送写指针寄存器
ERROR_SUB BoardW5500_ReadSocketTxWritePtrReg(W5500_SOCKET_NO socketNo,uint16_t* regValuePtr);

//写入Socket n 空闲接收缓存寄存器
ERROR_SUB BoardW5500_WriteSocketRxFreeSizeReg(W5500_SOCKET_NO socketNo,uint16_t regValue);
//读取Socket n 空闲接收缓存寄存器
ERROR_SUB BoardW5500_ReadSocketRxFreeSizeReg(W5500_SOCKET_NO socketNo,uint16_t* regValuePtr);

//写入Socket n 接收读指针寄存器
ERROR_SUB BoardW5500_WriteSocketRxReadPtrReg(W5500_SOCKET_NO socketNo,uint16_t regValue);
//读取Socket n 接收读指针寄存器
ERROR_SUB BoardW5500_ReadSocketRxReadPtrReg(W5500_SOCKET_NO socketNo,uint16_t* regValuePtr);

//写入Socket n 接收写指针寄存器
ERROR_SUB BoardW5500_WriteSocketRxWritePtrReg(W5500_SOCKET_NO socketNo,uint16_t regValue);
//读取Socket n 接收写指针寄存器
ERROR_SUB BoardW5500_ReadSocketRxWritePtrReg(W5500_SOCKET_NO socketNo,uint16_t* regValuePtr);

//写入Socket n 中断屏蔽寄存器
ERROR_SUB BoardW5500_WriteSocketIntMaskReg(W5500_SOCKET_NO socketNo,uint8_t regValue);
//读取Socket n 中断屏蔽寄存器
ERROR_SUB BoardW5500_ReadSocketIntMaskReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr);

//写入Socket n 分段寄存器
ERROR_SUB BoardW5500_WriteSocketFragReg(W5500_SOCKET_NO socketNo,uint8_t regValue);
//读取Socket n 分段寄存器
ERROR_SUB BoardW5500_ReadSocketFragMentReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr);

//写入Socket 在线时间寄存器
ERROR_SUB BoardW5500_WriteSocketKeepAliveCountReg(W5500_SOCKET_NO socketNo,uint8_t regValue);
//读取Socket 在线时间寄存器
ERROR_SUB BoardW5500_ReadSocketKeepAliveCountReg(W5500_SOCKET_NO socketNo,uint8_t* regValuePtr);




#endif



