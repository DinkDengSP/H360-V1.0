/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 14:41:23
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-03-07 12:52:43
 *FilePath: \H360_ActionBoard_ID_3\TaskMain\AppSupport\AppIpc\AppIpcMeasureUnit.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IPC_MEASURE_UNIT_H_
#define __APP_IPC_MEASURE_UNIT_H_
#include "AppIpcBase.h"

//初始化IPC调用结构
void IPC_MeasureUnitDataInit(void);

/*****************************************************等待任务完成*********************************************************/
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_MeasureUnitGetLastCommandStatusAndResult(LH_ERR* resultCode);
//等待执行完成,阻塞
LH_ERR IPC_MeasureUnitWaitLastReturn(void);

/****************************************************发出指令不等待完成*******************************************************/
//测量模块初始化
LH_ERR IPC_MeasureUnitResetWhileAck(void);
//测量室上门全开
LH_ERR IPC_MeasureUnitDoorOpenFullWhileAck(void);
//测量室上门半开
LH_ERR IPC_MeasureUnitDoorOpenHalfWhileAck(void);
//测量室上门关闭
LH_ERR IPC_MeasureUnitDoorCloseWhileAck(void);
//测量室侧门打开
LH_ERR IPC_MeasureUnitWindowOpenWhileAck(void);
//测量室侧门关闭
LH_ERR IPC_MeasureUnitWindowCloseWhileAck(void);
//激发液灌注
LH_ERR IPC_MeasureUnitPrimeOnceWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle);
//激发液注液
LH_ERR IPC_MeasureUnitInjectOnceWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle);
//测量模块清洗维护
LH_ERR IPC_MeasureUnitInjectMaintainWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle);
//测量模块暗记数
LH_ERR IPC_MeasureUnitReadDarkWhileAck(void);
//测量模块读本底
LH_ERR IPC_MeasureUnitReadBackGroundWhileAck(void);
//测量模块注液测光
LH_ERR IPC_MeasureUnitReadLightWithInjectWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle);
//测量模块集成单次测量
LH_ERR IPC_MeasureUnitReadLightComplexWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle);
//测量模块读光子变化率
LH_ERR IPC_MeasureUnitReadLightChangeRateWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle,uint8_t setChangeCount);



/****************************************************发出指令并等待完成********************************************************/
//测量模块初始化
LH_ERR IPC_MeasureUnitResetWhileReturn(void);
//测量室上门全开
LH_ERR IPC_MeasureUnitDoorOpenFullWhileReturn(void);
//测量室上门半开
LH_ERR IPC_MeasureUnitDoorOpenHalfWhileReturn(void);
//测量室上门关闭
LH_ERR IPC_MeasureUnitDoorCloseWhileReturn(void);
//测量室侧门打开
LH_ERR IPC_MeasureUnitWindowOpenWhileReturn(void);
//测量室侧门关闭
LH_ERR IPC_MeasureUnitWindowCloseWhileReturn(void);
//激发液灌注
LH_ERR IPC_MeasureUnitPrimeOnceWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle);
//激发液注液
LH_ERR IPC_MeasureUnitInjectOnceWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle);
//测量模块清洗维护
LH_ERR IPC_MeasureUnitInjectMaintainWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle);
//测量模块暗记数
LH_ERR IPC_MeasureUnitReadDarkWhileReturn(void);
//测量模块读本底
LH_ERR IPC_MeasureUnitReadBackGroundWhileReturn(void);
//测量模块注液测光
LH_ERR IPC_MeasureUnitReadLightWithInjectWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle);
//测量模块集成单次测量
LH_ERR IPC_MeasureUnitReadLightComplexWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle);
//测量模块读光子变化率
LH_ERR IPC_MeasureUnitReadLightChangeRateWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle,uint8_t setChangeCount);



#endif


