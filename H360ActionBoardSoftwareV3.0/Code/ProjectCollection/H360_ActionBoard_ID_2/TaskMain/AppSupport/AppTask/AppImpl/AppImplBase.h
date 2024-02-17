/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-02-11 14:07:18
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-04-13 19:00:09
 *FilePath: \H360_ActionBoard_ID_2\TaskMain\AppSupport\AppTask\AppImpl\AppImplBase.h
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


//应用层模块号,用于生成模块错误代码,插入到模块错误代码的24-27位
#define MODULE_NO_CUP_TRAY_MANAGE           1//杯栈管理模块
#define MODULE_NO_NEEDLE_BEAD               2//磁珠针
#define MODULE_NO_PLATE_REAGENT             3//试剂盘

//转换试剂盘错误代码的标识
typedef enum FLAG_PLATE_REAGENT_ERR_CONVERT
{
    FLAG_PLATE_REAGENT_ERR_ROTATE_RESET            = 0X01,//试剂盘旋转复位时候产生的错误代码
    FLAG_PLATE_REAGENT_ERR_ROTATE_RUN              = 0X02,//试剂盘旋转运行时候产生的错误代码
    FLAG_PLATE_REAGENT_ERR_OFFSET                  = 0X03,//试剂盘旋转相对偏移错误
    FLAG_PLATE_REAGENT_ERR_TARGET_CUP              = 0X04,//目标杯位错误
    FLAG_PLATE_REAGENT_ERR_CUP_COUNT               = 0X05,//运行杯位数量错误
}FLAG_PLATE_REAGENT_ERR_CONVERT;

//移液针探液电压
typedef struct LIQUID_DETECT_NEEDLE_VOL_RESULT{
    uint16_t liquidDetectBaseVol;
    uint16_t liquidDetectNeedleVol;
}LIQUID_DETECT_NEEDLE_VOL_RESULT;

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

//APP CupTrayManage任务
extern const uint8_t* AppCupTrayManageDebugStartMsg;
extern const uint8_t* AppCupTrayManageDebugEndMsg;

//APP NeedleBead任务
extern const uint8_t* AppNeedleBeadDebugStartMsg;
extern const uint8_t* AppNeedleBeadDebugEndMsg;

//APP Param任务
extern const uint8_t* AppParamDebugStartMsg;
extern const uint8_t* AppParamDebugEndMsg;

//APP PlateReagent任务
extern const uint8_t* AppPlateReagentDebugStartMsg;
extern const uint8_t* AppPlateReagentDebugEndMsg;



#endif
