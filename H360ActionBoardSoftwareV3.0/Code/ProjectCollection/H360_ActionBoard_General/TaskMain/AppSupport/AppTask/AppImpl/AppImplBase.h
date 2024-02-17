/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-01-14 13:26:11
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-02-13 15:23:07
 *FilePath: \H360_ActionBoard_General\TaskMain\AppSupport\AppTask\AppImpl\AppImplBase.h
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
#include "SystemCmd.h"
#include "AppState.h"
#include "AppParam.h"

//是否执行真实动作,还是只进行流程测试
#define APP_DEBUG_SHOW_ENABLE               1

#if(APP_DEBUG_SHOW_ENABLE == 1)
#define AppShowStartMsg(msgStr,paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)        MCU_Uart3Printf("%s%s - %d - %d - %d - %d - %d\r\n",(uint8_t*)msgStr, \
                                                                                                                            (uint8_t*)paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)
#define AppShowEndMsg(endStr,userErrorCode)                                                       MCU_Uart3Printf("%s - 0x%08x\r\n",endStr,userErrorCode)
#define AppShowErrMsg(errorCode)                                                                  MCU_Uart3Printf("!!Action Error Func : %s , Line : %d , ErrorCode : 0x%08x\r\n",\
                                                                                                                            __FUNCTION__,__LINE__,errorCode)
#else
#define AppShowStartMsg(msgStr,paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)        asm("nop")
#define AppShowEndMsg(endStr,userErrorCode)                                                       asm("nop")
#define AppShowErrMsg(errorCode)                                                                  asm("nop")
#endif

//APP LogicOther任务
extern const uint8_t* AppOtherDebugStartMsg;
extern const uint8_t* AppOtherDebugEndMsg;




#endif




