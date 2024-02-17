/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:11:15 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-25 10:21:55 +0800
************************************************************************************************/ 
#ifndef __BOARD_W5500_CONFIG_H_
#define __BOARD_W5500_CONFIG_H_
#include "DriverHeaderMCU.h"

//选中端口      
#define W5500_SELECT_PIN                            MCU_PIN_F_6
//选中端口有效电平          
#define VALID_LEVEL_W5500_SELECT_PIN                GPIO_PIN_RESET
//中断输入引脚                          
#define W5500_INT_PIN                               MCU_PIN_F_10

//W5500控制字段,写定义
#define W5500_CONTROL_BYTE_WRITE                    0X04
//W5500控制字段,读定义
#define W5500_CONTROL_BYTE_READ                     0X00

//链接模式
typedef enum W5500_LINK_MODE
{
    W5500_LINK_MODE_NONE        = 0X00,
    W5500_LINK_MODE_100M_FULL   = 0X01,
    W5500_LINK_MODE_100M_HALF   = 0X02,
    W5500_LINK_MODE_10M_FULL    = 0X03,
    W5500_LINK_MODE_10M_HALF    = 0X04,
}W5500_LINK_MODE;

//IP模式
typedef struct W5500_IP_ADDR
{
    uint8_t ip0;
    uint8_t ip1;
    uint8_t ip2;
    uint8_t ip3;
}W5500_IP_ADDR;

//寄存器选择
typedef enum W5500_CONTROL_BYTE_SEL
{
    //W5500控制段选中设定 bsb[4:0]
    W5500_CONTROL_BYTE_SEL_BASE_REG        = 0X00,
    //SOCKET0寄存器选择
    W5500_CONTROL_BYTE_SEL_SOCKET_0_REG    = 0X01,
    W5500_CONTROL_BYTE_SEL_SOCKET_0_TX_BUF = 0X02,
    W5500_CONTROL_BYTE_SEL_SOCKET_0_RX_BUF = 0X03,
    //SOCKET1寄存器选择
    W5500_CONTROL_BYTE_SEL_SOCKET_1_REG    = 0X05,
    W5500_CONTROL_BYTE_SEL_SOCKET_1_TX_BUF = 0X06,
    W5500_CONTROL_BYTE_SEL_SOCKET_1_RX_BUF = 0X07,
    //SOCKET2寄存器选择
    W5500_CONTROL_BYTE_SEL_SOCKET_2_REG    = 0X09,
    W5500_CONTROL_BYTE_SEL_SOCKET_2_TX_BUF = 0X0A,
    W5500_CONTROL_BYTE_SEL_SOCKET_2_RX_BUF = 0X0B,
    //SOCKET3寄存器选择
    W5500_CONTROL_BYTE_SEL_SOCKET_3_REG    = 0X0D,
    W5500_CONTROL_BYTE_SEL_SOCKET_3_TX_BUF = 0X0E,
    W5500_CONTROL_BYTE_SEL_SOCKET_3_RX_BUF = 0X0F,
    //SOCKET4寄存器选择
    W5500_CONTROL_BYTE_SEL_SOCKET_4_REG    = 0X11,
    W5500_CONTROL_BYTE_SEL_SOCKET_4_TX_BUF = 0X12,
    W5500_CONTROL_BYTE_SEL_SOCKET_4_RX_BUF = 0X13,
    //SOCKET5寄存器选择
    W5500_CONTROL_BYTE_SEL_SOCKET_5_REG    = 0X15,
    W5500_CONTROL_BYTE_SEL_SOCKET_5_TX_BUF = 0X16,
    W5500_CONTROL_BYTE_SEL_SOCKET_5_RX_BUF = 0X17,
    //SOCKET6寄存器选择
    W5500_CONTROL_BYTE_SEL_SOCKET_6_REG    = 0X19,
    W5500_CONTROL_BYTE_SEL_SOCKET_6_TX_BUF = 0X1A,
    W5500_CONTROL_BYTE_SEL_SOCKET_6_RX_BUF = 0X1B,
    //SOCKET7寄存器选择
    W5500_CONTROL_BYTE_SEL_SOCKET_7_REG    = 0X1D,
    W5500_CONTROL_BYTE_SEL_SOCKET_7_TX_BUF = 0X1E,
    W5500_CONTROL_BYTE_SEL_SOCKET_7_RX_BUF = 0X1F,
}W5500_CONTROL_BYTE_SEL;

//SOCKET选择
typedef enum W5500_SOCKET_NO
{
    W5500_SOCKET_NO_0   = 0x00,
    W5500_SOCKET_NO_1   = 0x01,
    W5500_SOCKET_NO_2   = 0x02,
    W5500_SOCKET_NO_3   = 0x03,
    W5500_SOCKET_NO_4   = 0x04,
    W5500_SOCKET_NO_5   = 0x05,
    W5500_SOCKET_NO_6   = 0x06,
    W5500_SOCKET_NO_7   = 0x07,
}W5500_SOCKET_NO;

/*------------------------通用寄存器------------------------*/
//模式寄存器
#define W5500_REG_ADDR_BASE_MR                          (0X0000)
//寄存器值
#define W5500_REG_VAL_BASE_MR_RST                       0X80 		
#define W5500_REG_VAL_BASE_MR_WOL                       0X20		
#define W5500_REG_VAL_BASE_MR_PB                        0X10		
#define W5500_REG_VAL_BASE_MR_PPPOE                     0X08 		
#define W5500_REG_VAL_BASE_MR_UDP_FARP                  0X02 		
//网关 IP 地址寄存器            
#define W5500_REG_ADDR_BASE_GAR0                        (0x0001)
#define W5500_REG_ADDR_BASE_GAR1                        (0x0002)
#define W5500_REG_ADDR_BASE_GAR2                        (0x0003)
#define W5500_REG_ADDR_BASE_GAR3                        (0x0004)
//子网掩码寄存器            
#define W5500_REG_ADDR_BASE_SUBR0                       (0x0005)
#define W5500_REG_ADDR_BASE_SUBR1                       (0x0006)
#define W5500_REG_ADDR_BASE_SUBR2                       (0x0007)
#define W5500_REG_ADDR_BASE_SUBR3                       (0x0008)
//源 MAC 地址寄存器         
#define W5500_REG_ADDR_BASE_SHAR0                       (0x0009)
#define W5500_REG_ADDR_BASE_SHAR1                       (0x000A)
#define W5500_REG_ADDR_BASE_SHAR2                       (0x000B)
#define W5500_REG_ADDR_BASE_SHAR3                       (0x000C)
#define W5500_REG_ADDR_BASE_SHAR4                       (0x000D)
#define W5500_REG_ADDR_BASE_SHAR5                       (0x000E)
//源 IP 地址寄存器          
#define W5500_REG_ADDR_BASE_SIPR0                       (0x000F)
#define W5500_REG_ADDR_BASE_SIPR1                       (0x0010)
#define W5500_REG_ADDR_BASE_SIPR2                       (0x0011)
#define W5500_REG_ADDR_BASE_SIPR3                       (0x0012)
//低电平中断定时器寄存器            
#define W5500_REG_ADDR_BASE_INTLEVEL0                   (0x0013)
#define W5500_REG_ADDR_BASE_INTLEVEL1                   (0x0014)
//中断寄存器            
#define W5500_REG_ADDR_BASE_IR                          (0x0015)
//寄存器值
#define W5500_REG_VAL_BASE_IR_CONFLICT                  0x80
#define W5500_REG_VAL_BASE_IR_UNREACH                   0x40
#define W5500_REG_VAL_BASE_IR_PPPoE                     0x20
#define W5500_REG_VAL_BASE_IR_MAGIC                     0x10
//中断屏蔽寄存器            
#define W5500_REG_ADDR_BASE_IMR                         (0x0016)
//Socket 中断寄存器         
#define W5500_REG_ADDR_BASE_SIR                         (0x0017) 
//Socket 中断屏蔽寄存器         
#define W5500_REG_ADDR_BASE_SIMR                        (0x0018)
//RTR 重试时间值寄存器          
#define W5500_REG_ADDR_BASE_RTR0                        (0x0019)
#define W5500_REG_ADDR_BASE_RTR1                        (0x001A)
//RCR 重试计数寄存器            
#define W5500_REG_ADDR_BASE_WIZ_RCR                     (0x001B)
//PTIMER PPP 连接控制协议请求定时寄存器         
#define W5500_REG_ADDR_BASE_PTIMER                      (0x001C)
//PMAGIC PPP 连接控制协议幻数寄存器         
#define W5500_REG_ADDR_BASE_PMAGIC                      (0x001D)
//PHAR PPPoE 模式下目标 MAC 寄存器          
#define W5500_REG_ADDR_BASE_PDHAR0                      (0x001E)
#define W5500_REG_ADDR_BASE_PDHAR1                      (0x001F)
#define W5500_REG_ADDR_BASE_PDHAR2                      (0x0020)
#define W5500_REG_ADDR_BASE_PDHAR3                      (0x0021)
#define W5500_REG_ADDR_BASE_PDHAR4                      (0x0022)
#define W5500_REG_ADDR_BASE_PDHAR5                      (0x0023)
//PSID PPPoE 模式下会话 ID 寄存器           
#define W5500_REG_ADDR_BASE_PSID0                       (0x0024)               
#define W5500_REG_ADDR_BASE_PSID1                       (0x0025)           
//PMRU PPPoE 模式下最大接收单元         
#define W5500_REG_ADDR_BASE_PMR0                        (0x0026)        
#define W5500_REG_ADDR_BASE_PMR1                        (0x0027)        
//UIPR 无法抵达 IP 地址寄存器           
#define W5500_REG_ADDR_BASE_UIPR0                       (0x0028)
#define W5500_REG_ADDR_BASE_UIPR1                       (0x0029)
#define W5500_REG_ADDR_BASE_UIPR2                       (0x002A)
#define W5500_REG_ADDR_BASE_UIPR3                       (0x002B)
//UPORTR 无法抵达端口寄存器         
#define W5500_REG_ADDR_BASE_UPORT0                      (0x002C)
#define W5500_REG_ADDR_BASE_UPORT1                      (0x002D)
//PHYCFGR W5500 PHY 配置寄存器          
#define W5500_REG_ADDR_BASE_PHYCFGR                     (0x002E)
#define W5500_REG_MASK_PHYCFGR_LINK_STATE               (0X01)
#define W5500_REG_MASK_PHYCFGR_LINK_SPEED               (0X02)
#define W5500_REG_MASK_PHYCFGR_LINK_DPX                 (0X04)
//VERSIONR W5500 芯片版本寄存器         
#define W5500_REG_ADDR_BASE_VERSIONR                    (0x0039)  

/*------------------------SOCKET寄存器------------------------*/
//Socket n 模式寄存器
#define W5500_REG_ADDR_SOCKET_Sn_MR                     (0X0000)
//寄存器值
#define W5500_REG_VAL_SOCKET_Sn_MR_CLOSE                0x00
#define W5500_REG_VAL_SOCKET_Sn_MR_TCP                  0x01
#define W5500_REG_VAL_SOCKET_Sn_MR_UDP                  0x02
#define W5500_REG_VAL_SOCKET_Sn_MR_IPRAW                0x03
#define W5500_REG_VAL_SOCKET_Sn_MR_MACRAW               0x04
#define W5500_REG_VAL_SOCKET_Sn_MR_PPPOE                0x05
#define W5500_REG_VAL_SOCKET_Sn_MR_UCASTB               0x10
#define W5500_REG_VAL_SOCKET_Sn_MR_ND                   0x20
#define W5500_REG_VAL_SOCKET_Sn_MR_MC                   0x20
#define W5500_REG_VAL_SOCKET_Sn_MR_BCASTB               0x40
#define W5500_REG_VAL_SOCKET_Sn_MR_MULTI                0x80
#define W5500_REG_VAL_SOCKET_Sn_MR_MIP6N                0x10
#define W5500_REG_VAL_SOCKET_Sn_MR_MMB                  0x20
#define W5500_REG_VAL_SOCKET_Sn_MR_MFEN                 0x80
//Socket n 配置寄存器
#define W5500_REG_ADDR_SOCKET_Sn_CR                     (0x0001)
//寄存器值
#define W5500_REG_VAL_SOCKET_Sn_CR_OPEN                 0x01
#define W5500_REG_VAL_SOCKET_Sn_CR_LISTEN               0x02
#define W5500_REG_VAL_SOCKET_Sn_CR_CONNECT              0x04
#define W5500_REG_VAL_SOCKET_Sn_CR_DISCON               0x08
#define W5500_REG_VAL_SOCKET_Sn_CR_CLOSE                0x10
#define W5500_REG_VAL_SOCKET_Sn_CR_SEND                 0x20
#define W5500_REG_VAL_SOCKET_Sn_CR_SEND_MAC             0x21
#define W5500_REG_VAL_SOCKET_Sn_CR_SEND_KEEP            0x22
#define W5500_REG_VAL_SOCKET_Sn_CR_RECV                 0x40
#define W5500_REG_VAL_SOCKET_Sn_CR_PCON                 0x23  
#define W5500_REG_VAL_SOCKET_Sn_CR_PDISCON              0x24  
#define W5500_REG_VAL_SOCKET_Sn_CR_PCR                  0x25  
#define W5500_REG_VAL_SOCKET_Sn_CR_PCN                  0x26  
#define W5500_REG_VAL_SOCKET_Sn_CR_PCJ                  0x27  
//Socket n 中断寄存器
#define W5500_REG_ADDR_SOCKET_Sn_IR                     (0x0002)
//寄存器值
#define W5500_REG_VAL_SOCKET_Sn_IR_PRECV                0x80  
#define W5500_REG_VAL_SOCKET_Sn_IR_PFAIL                0x40  
#define W5500_REG_VAL_SOCKET_Sn_IR_PNEXT                0x20  
#define W5500_REG_VAL_SOCKET_Sn_IR_SEND_OK              0x10
#define W5500_REG_VAL_SOCKET_Sn_IR_TIMEOUT              0x08
#define W5500_REG_VAL_SOCKET_Sn_IR_RECV                 0x04
#define W5500_REG_VAL_SOCKET_Sn_IR_DISCON               0x02
#define W5500_REG_VAL_SOCKET_Sn_IR_CON                  0x01
//Socket n 状态寄存器
#define W5500_REG_ADDR_SOCKET_Sn_SR                     (0x0003)
//寄存器值
#define W5500_REG_VAL_SOCKET_Sn_SR_SOCK_CLOSED          0x00
#define W5500_REG_VAL_SOCKET_Sn_SR_SOCK_INIT            0x13
#define W5500_REG_VAL_SOCKET_Sn_SR_SOCK_LISTEN          0x14
#define W5500_REG_VAL_SOCKET_Sn_SR_SOCK_SYNSENT         0x15
#define W5500_REG_VAL_SOCKET_Sn_SR_SOCK_SYNRECV         0x16
#define W5500_REG_VAL_SOCKET_Sn_SR_SOCK_ESTABLISHED     0x17
#define W5500_REG_VAL_SOCKET_Sn_SR_SOCK_FIN_WAIT        0x18
#define W5500_REG_VAL_SOCKET_Sn_SR_SOCK_CLOSING         0x1A
#define W5500_REG_VAL_SOCKET_Sn_SR_SOCK_TIME_WAIT       0x1B
#define W5500_REG_VAL_SOCKET_Sn_SR_SOCK_CLOSE_WAIT      0x1C
#define W5500_REG_VAL_SOCKET_Sn_SR_SOCK_LAST_ACK        0x1D
#define W5500_REG_VAL_SOCKET_Sn_SR_SOCK_UDP             0x22
#define W5500_REG_VAL_SOCKET_Sn_SR_SOCK_IPRAW           0x32
#define W5500_REG_VAL_SOCKET_Sn_SR_SOCK_MACRAW          0x42
#define W5500_REG_VAL_SOCKET_Sn_SR_SOCK_PPPOE           0x5F
//Socket n 源端口寄存器
#define W5500_REG_ADDR_SOCKET_Sn_PORT0                  (0x0004)
#define W5500_REG_ADDR_SOCKET_Sn_PORT1                  (0x0005)
//Socket n目的 MAC 地址寄存器
#define W5500_REG_ADDR_SOCKET_Sn_DHAR0                  (0x0006)                  
#define W5500_REG_ADDR_SOCKET_Sn_DHAR1                  (0x0007)
#define W5500_REG_ADDR_SOCKET_Sn_DHAR2                  (0x0008)
#define W5500_REG_ADDR_SOCKET_Sn_DHAR3                  (0x0009)
#define W5500_REG_ADDR_SOCKET_Sn_DHAR4                  (0x000A)
#define W5500_REG_ADDR_SOCKET_Sn_DHAR5                  (0x000B)
//Socket 目标 IP 地址寄存器
#define W5500_REG_ADDR_SOCKET_Sn_DIPR0                  (0x000C)
#define W5500_REG_ADDR_SOCKET_Sn_DIPR1                  (0x000D)
#define W5500_REG_ADDR_SOCKET_Sn_DIPR2                  (0x000E)
#define W5500_REG_ADDR_SOCKET_Sn_DIPR3                  (0x000F)
//Socket n 目标端口寄存器
#define W5500_REG_ADDR_SOCKET_Sn_DPORT0                 (0x0010)
#define W5500_REG_ADDR_SOCKET_Sn_DPORT1                 (0x0011)
//Socket n-th 最大分段寄存器
#define W5500_REG_ADDR_SOCKET_Sn_MSSR0                  (0x0012)
#define W5500_REG_ADDR_SOCKET_Sn_MSSR1                  (0x0013)
//Socket IP 服务类型寄存器
#define W5500_REG_ADDR_SOCKET_Sn_TOS                    (0x0015)
//Socket IP 生存时间寄存器
#define W5500_REG_ADDR_SOCKET_Sn_TTL                    (0x0016)
//Socket n 接收缓存大小寄存器
#define W5500_REG_ADDR_SOCKET_Sn_RXMEM_SIZE             (0x001E)
//Socket n 发送缓存大小寄存器
#define W5500_REG_ADDR_SOCKET_Sn_TXMEM_SIZE             (0x001F)
//Socket n 空闲发送缓存寄存器
#define W5500_REG_ADDR_SOCKET_Sn_TX_FSR0                (0x0020)
#define W5500_REG_ADDR_SOCKET_Sn_TX_FSR1                (0x0021)
//Socket n 发送读指针寄存器
#define W5500_REG_ADDR_SOCKET_Sn_TX_RD0                 (0x0022)
#define W5500_REG_ADDR_SOCKET_Sn_TX_RD1                 (0x0023)
//Socket n 发送写指针寄存器
#define W5500_REG_ADDR_SOCKET_Sn_TX_WR0                 (0x0024)
#define W5500_REG_ADDR_SOCKET_Sn_TX_WR1                 (0x0025)
//Socket n 空闲接收缓存寄存器
#define W5500_REG_ADDR_SOCKET_Sn_RX_RSR0                (0x0026)
#define W5500_REG_ADDR_SOCKET_Sn_RX_RSR1                (0x0027)
//Socket n 接收读指针寄存器
#define W5500_REG_ADDR_SOCKET_Sn_RX_RD0                 (0x0028)
#define W5500_REG_ADDR_SOCKET_Sn_RX_RD1                 (0x0029)
//Socket n 接收写指针寄存器
#define W5500_REG_ADDR_SOCKET_Sn_RX_WR0                 (0x002A)
#define W5500_REG_ADDR_SOCKET_Sn_RX_WR1                 (0x002B)
//Socket n 中断屏蔽寄存器
#define W5500_REG_ADDR_SOCKET_Sn_IMR                    (0x002C)
//Socket n 分段寄存器
#define W5500_REG_ADDR_SOCKET_Sn_FRAG                   (0x002D)
//Socket 在线时间寄存器
#define W5500_REG_ADDR_SOCKET_Sn_KPALVTR                (0x002F)






#endif



