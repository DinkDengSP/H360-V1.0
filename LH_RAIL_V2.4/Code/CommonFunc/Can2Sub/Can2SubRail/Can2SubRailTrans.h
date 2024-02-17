/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-24 12:19:45
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-27 15:48:04
 *FilePath: \Appd:\SVN_Local\02-Development\Software\H360_STM32\LH_RAIL_V2.3\Code\CommonFunc\Can2SubSystem\Can2SubRail\Can2SubRailTrans.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __CAN2_SUB_RAIL_TRANS_H_
#define __CAN2_SUB_RAIL_TRANS_H_
#include "Can2SubRailBase.h"

//轨道传输带复位,不带位置返回
LH_ERR Can2SubRailTransResetWhileReturnWithOutPos(RAIL_RESET_CORRECT_FLAG resetFlag);

//轨道传输带复位
LH_ERR Can2SubRailTransResetWhileReturn(RAIL_RESET_CORRECT_FLAG resetFlag,RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos);

//轨道传送带移动到进样1
LH_ERR Can2SubRailTransMove2PushIn1WhileReturn(RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos);

//轨道传送带移动到进样2
LH_ERR Can2SubRailTransMove2PushIn2WhileReturn(RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos);

//轨道传送带移动到出样1
LH_ERR Can2SubRailTransMove2PushOut1WhileReturn(RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos);

//轨道传送带移动到出样2
LH_ERR Can2SubRailTransMove2PushOut2WhileReturn(RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos);

//轨道传送到移动到急诊位
LH_ERR Can2SubRailTransMove2EmergencyCallWhileReturn(RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos);

//轨道传送带移动到旋转位1
LH_ERR Can2SubRailTransMove2Rotate1WhileReturn(RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos);

//轨道传送到移动到旋转位2
LH_ERR Can2SubRailTransMove2Rotate2WhileReturn(RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos);

//轨道传送带自动选择一个位置旋转
LH_ERR Can2SubRailTransMove2RotateAutoWhileReturn(RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos);

//轨道传送带移动到指定试管架扫码位
LH_ERR Can2SubRailTransMove2RackBarScanWhileReturn(RAIL_BAR_SCAN_POS barScanPos,RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos);

//轨道传送带移动到指定试管扫码位
LH_ERR Can2SubRailTransMove2TubeBarScanWhileReturn(RAIL_TUBE_POS tubeIndex,RAIL_BAR_SCAN_POS barScanPos,RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos);

//轨道传送带移动到特定模块的待测位
LH_ERR Can2SubRailTransMove2WaitTestWhileReturn(RAIL_MODULE_POS railModuleNo,RAIL_WAITTEST_NO waitTestNo,RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos);

//轨道传送带移动到特定模块的重测位
LH_ERR Can2SubRailTransMove2RepeatTestWhileReturn(RAIL_MODULE_POS railModuleNo,RAIL_RETEST_NO repeatTestNo,RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos);

//轨道传送带移动到特定模块测试放架位
LH_ERR Can2SubRailTransMove2TestingPutWhileReturn(RAIL_MODULE_POS railModuleNo,RAIL_TUBE_POS tubeIndex,RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos);

//轨道传送到移动到特定模块测试取架位
LH_ERR Can2SubRailTransMove2TestingCatchWhileReturn(RAIL_MODULE_POS railModuleNo,RAIL_TUBE_POS tubeIndex,RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos);

//轨道传送带移动到特定模块测试吸样位
LH_ERR Can2SubRailTransMove2TestingAbsorbWhileReturn(RAIL_MODULE_POS railModuleNo,RAIL_TUBE_POS tubeIndex,RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos);

//轨道传送带移动到指定集成扫码位
LH_ERR Can2SubRailTransMove2BarScanIntegrateWhileReturn(RAIL_BAR_SCAN_POS barScanPos,RAIL_MODULE_POS* railMainPos,RAIL_SUB_MODULE* railSubPos);



















#endif



