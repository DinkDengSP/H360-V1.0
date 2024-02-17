/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 14:44:47
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-13 19:13:10
 *FilePath: \H360_ActionBoard_ID_4\TaskMain\AppSupport\AppTask\AppImpl\AppImplBase.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __APP_IMPL_BASE_H_
#define __APP_IMPL_BASE_H_
#include "BoardInc.h"
#include "CanSlaveMain.h"
#include "CanMasterMain.h"
#include "CanSubSystemInc.h"
#include "SystemCmd.h"
#include "ServiceIpcInc.h"
#include "AppState.h"
#include "AppParam.h"


//转换磁珠混匀错误代码的标识
typedef enum FLAG_MIX_BEAD_ERR_CONVERT
{
    FLAG_MIX_BEAD_ERR_ROTATE_RESET = 0X01,//磁珠混匀旋转复位时候产生的错误代码
    FLAG_MIX_BEAD_ERR_UPDOWN_RESET = 0X02,//磁珠混匀升降复位的时候产生的错误代码
    FLAG_MIX_BEAD_ERR_UPDOWN_RUN   = 0X03,//磁珠混匀升降运行的时候产生的错误代码
    FLAG_MIX_BEAD_ERR_UPDOWN_UP    = 0X04,//磁珠混匀升降上升的时候产生的错误代码
    FLAG_MIX_BEAD_ERR_UPDOWN_DOWN  = 0X05,//磁珠混匀升降下降的时候产生的错误代码
    FLAG_MIX_BEAD_ERR_ROTATE_RUN   = 0X06,//磁珠混匀旋转运行的时候产生的错误代码
    FLAG_MIX_BEAD_ERR_ROTATE_STOP  = 0X07,//磁珠混匀旋转停止的时候产生的错误代码
}FLAG_MIX_BEAD_ERR_CONVERT;

//转换试剂混匀错误代码的标识
typedef enum FLAG_MIX_REAGENT_ERR_CONVERT
{
    FLAG_MIX_REAGENT_ERR_ROTATE_RESET = 0X01,//试剂混匀旋转复位时候产生的错误代码
    FLAG_MIX_REAGENT_ERR_UPDOWN_RESET = 0X02,//试剂混匀升降复位的时候产生的错误代码
    FLAG_MIX_REAGENT_ERR_UPDOWN_RUN   = 0X03,//试剂混匀升降运行的时候产生的错误代码
    FLAG_MIX_REAGENT_ERR_UPDOWN_UP    = 0X04,//试剂混匀升降上升的时候产生的错误代码
    FLAG_MIX_REAGENT_ERR_UPDOWN_DOWN  = 0X05,//试剂混匀升降下降的时候产生的错误代码
    FLAG_MIX_REAGENT_ERR_ROTATE_RUN   = 0X06,//试剂混匀旋转运行的时候产生的错误代码
    FLAG_MIX_REAGENT_ERR_ROTATE_STOP  = 0X07,//试剂混匀旋转停止的时候产生的错误代码
}FLAG_MIX_REAGENT_ERR_CONVERT;

//移液针探液电压
typedef struct LIQUID_DETECT_NEEDLE_VOL_RESULT{
    uint16_t liquidDetectBaseVol;
    uint16_t liquidDetectNeedleVol;
}LIQUID_DETECT_NEEDLE_VOL_RESULT;

//应用层模块号,用于生成模块错误代码,插入到模块错误代码的24-27位
#define MODULE_NO_MIX_BEAD                  1//磁珠摇匀
#define MODULE_NO_MIX_REAGENT               2//试剂摇匀
#define MODULE_NO_NEEDLE_REAGENT            3//试剂针
#define MODULE_NO_PLATE_REACTION            4//反应盘

//是否执行真实动作,还是只进行流程测试
#define APP_DEBUG_SHOW_ENABLE               1

#if(APP_DEBUG_SHOW_ENABLE == 1)
#define AppShowStartMsg(msgStr,paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)        MCU_Uart3Printf("%s%s - %d - %d - %d - %d - %d\r\n",(uint8_t*)msgStr, (uint8_t*)paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)
#define AppShowEndMsg(endStr,userErrorCode)                                                       MCU_Uart3Printf("%s - 0X%08X\r\n",endStr,userErrorCode)
#define AppShowErrMsg(errorCode)                                                                  MCU_Uart3Printf("!!Action Error Func : %s , Line : %d , ErrorCode : 0x%08X\r\n",__FUNCTION__,__LINE__,errorCode)
#else
#define AppShowStartMsg(msgStr,paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)        asm("nop")
#define AppShowEndMsg(endStr,userErrorCode)                                                       asm("nop")
#define AppShowErrMsg(errorCode)                                                                  asm("nop")
#endif

//APP MixReagent任务
extern const uint8_t* AppMixReagentDebugStartMsg ;
extern const uint8_t* AppMixReagentDebugEndMsg;

//APP NeedleReagent任务
extern const uint8_t* AppNeedleReagentDebugStartMsg;
extern const uint8_t* AppNeedleReagentDebugEndMsg;

//APP MixBead任务
extern const uint8_t* AppMixBeadDebugStartMsg;
extern const uint8_t* AppMixBeadDebugEndMsg;

//APP PlateReaction任务
extern const uint8_t* AppPlateReactionDebugStartMsg;
extern const uint8_t* AppPlateReactionDebugEndMsg;

//APP Param任务
extern const uint8_t* AppParamDebugStartMsg;
extern const uint8_t* AppParamDebugEndMsg;


#endif
