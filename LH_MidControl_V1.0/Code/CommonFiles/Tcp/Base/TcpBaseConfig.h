/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-08-04 14:58:22
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-08-10 14:06:21
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __TCP_BASE_CONFIG_H_
#define __TCP_BASE_CONFIG_H_
#include "stm32h7xx_hal.h"

//协议相关
//协议最大数据包长度=2048Byte
#define TCP_CMD_SIZE_MAX                                (4096u)   
//标准协议起始标识符        
#define TCP_CMD_LEADER_CODE                             (0x5AA5)       
// 命令头长度,即命令最小长度    
#define TCP_CMD_HEADER_SIZE                             (12u)     
// 命令起始位置,2字节长        
#define TCP_CMD_HEADER_CMD_OFFSET                       (2u)   
// 数据长度起始位置,2字节长           
#define TCP_CMD_HEADER_DLEN_OFFSET                      (4u) 
// 包头校验起始位置,4字节长             
#define TCP_CMD_HEADER_SUM_OFFSET                       (10u)  
// 数据校验起始位置,4字节长           
#define TCP_CMD_DATA_SUM_OFFSET                         (6u)    
//数据体偏移          
#define TCP_CMD_BODY_SRART                              (TCP_CMD_HEADER_SIZE)


//系统单元设置
#define MODULE_NO_CELL_1                                    (0)     //单元模块1模块号与中位机模块号
#define MODULE_NO_CELL_2                                    (1)     //单元模块2模块号
#define MODULE_NO_CELL_3                                    (2)     //单元模块3模块号
#define MODULE_NO_CELL_4                                    (3)     //单元模块4模块号
#define MODULE_NO_CELL_ISE                                  (4)     //样本台模块号
#define MODULE_NO_CELL_RAIL_MID                             (5)     //轨道与中位机模块号
#define SYSTEM_MODULE_COUNT                                 5

#define BOARD_ID_MID_CONTROL                                (0x20)    //中位机板号
#define BOARD_ID_BROADCAST                                  (0x3F)    //广播板号

//心跳上传,状态上传周期
#define TCPIP_STATE_AUTO_UPDATE_PERIOD_MS                   8000
//心跳丢失时间
#define TCPIP_HEART_LOSS_MAX_TIME_MS                        11000
//TCPIP数据包分片,两包数据之间的间隔,一个指令分成多包的情况
#define TCPIP_CMD_PACK_FRAME_TIMEOUT_MS                     1000
//TCP数据最大重发次数
#define TCP_DATA_SEND_RETRY_MAX                             3
//TCP数据发送等待上位机回传ACK的最大时间
#define TCP_DATA_SEND_WAIT_ACK_TIME_OUT_MAX_MS              1000


#endif




