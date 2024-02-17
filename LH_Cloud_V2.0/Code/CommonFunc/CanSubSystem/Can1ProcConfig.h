/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 14:49:50
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-09-04 13:50:25
 *FilePath: \Appc:\LiHe\Local\02-Development\Software\H360_STM32\LH_RAIL_V2.3\Code\CommonFunc\Can1Protocol\Can1ProcConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN1_PROC_CONFIG_H_
#define __CAN1_PROC_CONFIG_H_
#include "BoardInc.h"
#include "Can1CmdDef.h"

//CAN1解析使用CCM内存

//配置本板卡的模块号和板号

//轨道主控模块号
#define CAN1_SELF_MODULE_NO         0X05
//立禾云板号27,轨道板号23
#define CAN1_SELF_BOARD_ID          0X27

//中位机模块号
#define CAN1_COMM_MODULE_NO         0X05
//中位机板号
#define CAN1_COMM_BOARD_ID          0X20

//广播目标板号
#define CAN1_BOARDCAST_BOARD_ID     0X3F

//单包最大值
#define CAN1_SINGLE_PACK_MAX_LENGTH     2048

//长包指令起始包
#define CAN1_COMMAND_LONG_PACK_START    0X7FD  

//长包指令数据包
#define CAN1_COMMAND_LONG_PACK_DATA     0X7FE

//长包指令结束包
#define CAN1_COMMAND_LONG_PACK_END      0X7FF



#endif





