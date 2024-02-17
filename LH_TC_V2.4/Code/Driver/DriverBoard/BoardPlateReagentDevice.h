/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-16 15:59:44
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-01-19 09:23:17
 *FilePath: \LH_TC_APP_V2.3c:\LiHe\Local\02-Development\Software\H360_STM32\LH_TC_V2.3\Code\Driver\DriverBoard\BoardPlateReagentDevice.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __BOARD_PLATE_REAGENT_DEVICE_H_
#define __BOARD_PLATE_REAGENT_DEVICE_H_
#include "BoardCommBase.h"

//试剂盘初始化
void BoardPlateReagentDeviceInit(void);

//打开试剂盘制冷
void BoardPlateReagentDeviceEnable(void);

//停止试剂盘制冷
void BoardPlateReagentDeviceDisable(void);

//开关试剂盘循环泵和循环扇
void BoardPlateReagentDeviceSetFanAndPump(IO_SIGNAL_STATE setState);

//开关制冷灯
void BoardPlateReagentDeviceSetCoolerLed(IO_SIGNAL_STATE setState);

//试剂仓玻璃加热控制
void BoardPlateReagentDeviceSetGlass(IO_SIGNAL_STATE setState);

//读取试剂盘水浮状态
IO_SIGNAL_STATE BoardPlateReagentReadFloatingState(void);

//开关制冷灯
void BoardPlateReagentDeviceSetLed(IO_SIGNAL_STATE setState);

#endif













