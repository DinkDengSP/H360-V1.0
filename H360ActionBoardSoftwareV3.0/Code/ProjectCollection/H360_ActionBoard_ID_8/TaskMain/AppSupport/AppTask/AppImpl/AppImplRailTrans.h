/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-28 20:47:13
**FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppTask\AppImpl\AppImplRailTrans.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_RAIL_TRANS_H_
#define __APP_IMPL_RAIL_TRANS_H_
#include "AppImplRailTransConfig.h"

//轨道复位,带零位偏移
LH_ERR AppImplRailTransReset(RAIL_RESET_CORRECT_FLAG resetCorrectFlag);

//轨道移动到进样1
LH_ERR AppImplRailTransMove2PushIn1(void);

//轨道移动到进样2
LH_ERR AppImplRailTransMove2PushIn2(void);

//轨道移动到出样1
LH_ERR AppImplRailTransMove2PushOut1(void);

//轨道移动到出样2
LH_ERR AppImplRailTransMove2PushOut2(void);

//轨道运转到急诊位
LH_ERR AppImplRailTransMove2EmergencyCall(void);

//轨道移动到旋转位1
LH_ERR AppImplRailTransMove2Rotate1(void);

//轨道移动到旋转位2
LH_ERR AppImplRailTransMove2Rotate2(void);

//轨道移动到靠现在位置最近的旋转位
LH_ERR AppImplRailTransMove2RotateAuto(void);

//轨道移动到试管架扫码位
LH_ERR AppImplRailTransMove2RackBarScan(RAIL_BAR_SCAN_POS scanPos);

//轨道移动到试管扫码位,参数位试管序号,从0-4
LH_ERR AppImplRailTransMove2TubeBarScan(RAIL_TUBE_POS tubeIndex,RAIL_BAR_SCAN_POS scanPos);

//轨道移动到待测位,参数位模块号和待测位序号
LH_ERR AppImplRailTransMove2SpecialModuleWaitTest(RAIL_MODULE_POS moduleNo,RAIL_WAITTEST_NO waitTestNo);

//轨道移动到重测位,参数位模块号和待测位序号
LH_ERR AppImplRailTransMove2SpecialModuleReTest(RAIL_MODULE_POS moduleNo,RAIL_RETEST_NO reTestNo);

//轨道移动到测试区放架位,参数位模块号和测试区试管偏移
LH_ERR AppImplRailTransMove2SpecialModuleTestingPut(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex);

//轨道移动到测试区取架位,参数位模块号和测试区试管偏移
LH_ERR AppImplRailTransMove2SpecialModuleTestingCatch(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex);

//轨道移动到测试吸样位,参数位模块号和测试区试管偏移
LH_ERR AppImplRailTransMove2SpecialModuleTestingAbsorb(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex);

//轨道移动到扫码位
LH_ERR AppImplRailTransMove2BarScanIntegrate(RAIL_BAR_SCAN_POS scanPos);

//获取当前轨道位置
void AppImplRailTransReadCurrentModuleAndRegion(RAIL_MODULE_POS* currentMainModulePtr,RAIL_SUB_MODULE* currentSubRegionPtr);



#endif

