/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-19 14:49:50
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-01-13 13:42:55
 *FilePath: \Appc:\LiHe\Local\02-Development\Software\H360_STM32\LH_RAIL_V2.3\Code\CommonFunc\CanProtocol\CanProcConfig.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN_PROC_CONFIG_H_
#define __CAN_PROC_CONFIG_H_
#include "BoardInc.h"
#include "CanCmdDef.h"


//配置本板卡的模块号和板号
//自身模块号
#define CAN_SELF_MODULE_NO                      0X07

//流水线主控模块号
#define CAN_MAIN_CONTROL_MODULE_NO              0X07
//流水线主控板号
#define CAN_MAIN_CONTROL_BOARD_ID               0X3E

//广播目标板号
#define CAN_BOARDCAST_BOARD_ID                  0X3F

//单包最大值
#define CAN_SINGLE_PACK_MAX_LENGTH              128

//长包指令起始包
#define CAN_COMMAND_LONG_PACK_START             0X7FD  
//长包指令数据包
#define CAN_COMMAND_LONG_PACK_DATA              0X7FE
//长包指令结束包
#define CAN_COMMAND_LONG_PACK_END               0X7FF



#endif





