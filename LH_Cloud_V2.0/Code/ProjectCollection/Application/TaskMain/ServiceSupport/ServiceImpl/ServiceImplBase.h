/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-09-08 11:09:16
 *LastEditors: DengXiaoJun
 *LastEditTime: 2020-10-23 09:10:10
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_BASE_H_
#define __SERVICE_IMPL_BASE_H_
#include "BoardInc.h"
#include "os.h"

//使能调试信息输出
#define SYSTEM_DEBUG_MSG_SHOW_ENABLE           1

//这个打印信息是动作任务在响应应用任务调用时候的输出信息
#if(SYSTEM_DEBUG_MSG_SHOW_ENABLE == 1)
//输出任务调试讯息
#define SystemDebugShowMsgStart(msgStr,paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)       SystemPrintf("%s%s - %d - %d - %d - %d - %d\r\n",(uint8_t*)msgStr, (uint8_t*)paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)                                                                                                           
//输出任务结束讯息
#define SystemDebugShowMsgEnd(endStr,userErrorCode)                                                      SystemPrintf("%s - 0X%08X\r\n",endStr,userErrorCode)
//输出错误信息
#define SystemDebugShowMsgError(errorCode)                                                               SystemPrintf("!!Action Error Func : %s , Line : %d , ErrorCode : 0X%08X\r\n", __FUNCTION__,__LINE__,errorCode)
#else
//不输出信息调试
#define SystemDebugShowMsgStart(msgStr,paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)       asm("nop")
#define SystemDebugShowMsgEnd(endStr,userErrorCode)                                                      asm("nop")
#define SystemDebugShowMsgError(errorCode)                                                               asm("nop")
#endif

#endif


