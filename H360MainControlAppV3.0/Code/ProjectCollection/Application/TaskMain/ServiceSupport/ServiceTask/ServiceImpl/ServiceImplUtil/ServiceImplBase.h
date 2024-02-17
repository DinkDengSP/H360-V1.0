/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2019-12-31 13:51:27
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-16 13:44:26
 *FilePath: \Code\ProjectCollection\Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplUtil\ServiceImplBase.h
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __SERVICE_IMPL_BASE_H_
#define __SERVICE_IMPL_BASE_H_
#include "CommonFuncInc.h"
#include "ServiceStateTestFlow.h"
#include "ServiceStatePrivate.h"
#include "ServiceParam.h"
#include "TestFlow_Msg.h"
#include "SystemCmd.h"


//变参宏定义,调试printf,主要用于动作调试,分别是动作开始打印参数,动作结束返回结果,动作错误打印日志
#define ServicePrintf(format,args...)                                                                ActionPrintf(format,##args)
#define ServicePrintfShowStart(msgStr,paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)    ActionPrintf("%s%s - 0X%08X - 0X%08X - 0X%08X - 0X%08X - 0X%08X\r\n",(uint8_t*)msgStr, (uint8_t*)paramStr,paramVal1,paramVal2,paramVal3,paramVal4,paramVal5)
#define ServicePrintfShowEnd(endStr,userErrorCode)                                                   ActionPrintf("%s - 0X%08X\r\n",endStr,userErrorCode)
#define ServicePrintfShowError(errorCode)                                                            ActionPrintf("!!Action Error Func : %s , Line : %d , ErrorCode : 0X%08X\r\n", __FUNCTION__,__LINE__,errorCode)

#define Service_Api_RespondWarning(Level, Code, Param)      IPC_Service_ActionWarningSendToCommon(Level, Code, Param)

#define SystemPrintf(format,args...)            ServiceStateWriteAssistGetLogChannel()==LOG_FILE_CHANNEL_SELECT_SD_CARD ? SDCardPrintf(format,##args) : (ServiceStateReadPlateReagentRFIDLogChannel()==PLATE_REAGENT_RFID_LOG_CHANNEL0 ? MCU_Uart3PrintfVersion0(format,##args):MCU_Uart1PrintfVersion0(format,##args))

//#define SystemPrintf(format,args...)                                                                ActionPrintf(format,##args)

//各个任务指令调用之前的调试信息

//辅助调试信息
extern const uint8_t* DebugStringStart_Assist;
extern const uint8_t* DebugStringEnd_Assist;

//缓冲液注液调试信息
extern const uint8_t* DebugStringStart_BufferSolutionInject;
extern const uint8_t* DebugStringEnd_BufferSolutionInject;

//新杯托盘栈调试信息
extern const uint8_t* DebugStringStart_CupTrayStackManage;
extern const uint8_t* DebugStringEnd_CupTrayStackManage;

//新杯机械手调试信息
extern const uint8_t* DebugStringStart_HandNewCup;
extern const uint8_t* DebugStringEnd_HandNewCup;

//清洗机械手调试信息
extern const uint8_t* DebugStringStart_HandWash;
extern const uint8_t* DebugStringEnd_HandWash;

//液路自动化调试信息
extern const uint8_t* DebugStringStart_LiquidAuto;
extern const uint8_t* DebugStringEnd_LiquidAuto;

//测量模块调试信息
extern const uint8_t* DebugStringStart_MeasureModule;
extern const uint8_t* DebugStringEnd_MeasureModule;

//磁珠混匀调试信息
extern const uint8_t* DebugStringStart_MixBead;
extern const uint8_t* DebugStringEnd_MixBead;

//试剂混匀调试信息
extern const uint8_t* DebugStringStart_MixReagent;
extern const uint8_t* DebugStringEnd_MixReagent;

//清洗混匀调试信息
extern const uint8_t* DebugStringStart_MixWash;
extern const uint8_t* DebugStringEnd_MixWash;

//磁珠针调试信息
extern const uint8_t* DebugStringStart_NeedleBead;
extern const uint8_t* DebugStringEnd_NeedleBead;

//试剂针调试信息
extern const uint8_t* DebugStringStart_NeedleReagent;
extern const uint8_t* DebugStringEnd_NeedleReagent;

//样本针调试信息
extern const uint8_t* DebugStringStart_NeedleSample;
extern const uint8_t* DebugStringEnd_NeedleSample;

//清洗针调试信息
extern const uint8_t* DebugStringStart_NeedleWash;
extern const uint8_t* DebugStringEnd_NeedleWash;

//反应盘调试信息
extern const uint8_t* DebugStringStart_PlateReaction;
extern const uint8_t* DebugStringEnd_PlateReaction;

//试剂盘调试信息
extern const uint8_t* DebugStringStart_PlateReagent;
extern const uint8_t* DebugStringEnd_PlateReagent;

//清洗盘调试信息
extern const uint8_t* DebugStringStart_PlateWash;
extern const uint8_t* DebugStringEnd_PlateWash;

//试管架传送带调试信息
extern const uint8_t* DebugStringStart_SampleRackTrack;
extern const uint8_t* DebugStringEnd_SampleRackTrack;

//温控调试信息
extern const uint8_t* DebugStringStart_TemperatureControl;
extern const uint8_t* DebugStringEnd_TemperatureControl;

//文件系统调试信息
extern const uint8_t* DebugStringStart_FatFsLog;
extern const uint8_t* DebugStringEnd_FatFsLog;

//同步任务信息
extern const uint8_t* DebugStringStart_TaskSyn;
extern const uint8_t* DebugStringEnd_TaskSyn;

//读取版本号的序号
typedef enum VERSION_BOARD_ID
{
    VERSION_BOARD_MAIN          = 0X00,//主控板
    VERSION_BOARD_ACTION_BOARD1 = 0X01,//下位机板1
    VERSION_BOARD_ACTION_BOARD2 = 0X02,//下位机板2
    VERSION_BOARD_ACTION_BOARD3 = 0X03,//下位机板3
    VERSION_BOARD_ACTION_BOARD4 = 0X04,//下位机板4
    VERSION_BOARD_ACTION_BOARD5 = 0X05,//下位机板5
    VERSION_BOARD_TC_BOARD      = 0X09,//温控板
}VERSION_BOARD_ID;

//选择A瓶还是B瓶
typedef enum BOTTLE_SELECT
{
    BOTTLE_A    = 0X01,//选择A瓶
    BOTTLE_B    = 0X02,//选择B瓶
}BOTTLE_SELECT;
//针防撞触发错误代码掩码
#define NEEDLE_EMSTOP_ERROR_CODE_MASK        0X0000FFFF
//针防撞触发错误代码
#define NEEDLE_EMSTOP_ERROR_CODE             0X00000504

//服务层上报事件给应用层
void ServiceReportMsgToAppTask(TESTFLOW_MSG_CMD_E eventCode,uint8_t* dataBuffer,uint8_t dataLength);

//返回程序强制跳转结果
void Can1ProgramFoceJumpResult(uint8_t result,uint8_t state);

//向中位机传送调试的结果讯息
void Can1ReportDebugResult(SYSTEM_CMD_CAN1* can1CmdPtr,CAN1_REPORT_ERR_CODE commBoardErrCode,CAN1_REPORT_ERR_LEVEL errLevel,LH_ERR systemErrCode,uint16_t dataLength,uint8_t* dataBufferPtr);

//将buffer转换为无符号32位数
uint32_t Can1ProcUtilConvertArrayToUint32(uint8_t* bufferPtr,uint16_t startPos);

//将buffer转换为有符号32位数
int32_t Can1ProcUtilConvertArrayToInt32(uint8_t* bufferPtr,uint16_t startPos);

//将buffer转换为无符号16位数
uint16_t Can1ProcUtilConvertArrayToUint16(uint8_t* bufferPtr,uint16_t startPos);

//将buffer转换为有符号16位数
int16_t Can1ProcUtilConvertArrayToInt16(uint8_t* bufferPtr,uint16_t startPos);

//将buffer转换为无符号64位数
uint64_t Can1ProcUtilConvertArrayToUint64(uint8_t* bufferPtr,uint16_t startPos);

//将无符号64位数转换为buffer
void Can1ProcUtilConvertUint64ToArray(uint8_t* bufferPtr,uint16_t startPos,uint64_t srcDat);

//将无符号32位数转换为buffer
void Can1ProcUtilConvertUint32ToArray(uint8_t* bufferPtr,uint16_t startPos,uint32_t srcDat);

//将有符号32位数转换为buffer
void Can1ProcUtilConvertInt32ToArray(uint8_t* bufferPtr,uint16_t startPos,int32_t srcDat);

//将无符号16位数转换为buffer
void Can1ProcUtilConvertUint16ToArray(uint8_t* bufferPtr,uint16_t startPos,uint16_t srcDat);

//将有符号16位数转换为buffer
void Can1ProcUtilConvertInt16ToArray(uint8_t* bufferPtr,uint16_t startPos,int16_t srcDat);


//模块输入单元映射
typedef struct MODULE_INPUT_MAP_UNIT
{
    SYSTEM_INPUT_INDEX inputIndex;
    BitAction validState;
}MODULE_INPUT_MAP_UNIT;
//保留输入,当模块没有输入的时候用这个占位
#define MODULE_BASE_INPUT_PIN_RESERVE                       SYSTEM_INPUT_MAIN_BOARD_ARM_SEN1_PH10
#define VALID_LEVEL_MODULE_BASE_INPUT_PIN_RESERVE           Bit_RESET

//模块输出单元映射
typedef struct MODULE_OUTPUT_MAP_UNIT
{
    SYSTEM_OUTPUT_INDEX outputIndex;
    BitAction validState;
}MODULE_OUTPUT_MAP_UNIT;
//保留输出,当模块没有输出的时候用这个占位
#define MODULE_BASE_OUTPUT_PIN_RESERVE                      SYSTEM_OUTPUT_MAIN_BOARD_ARM_OUT1_PG15
#define VALID_LEVEL_MODULE_BASE_OUTPUT_PIN_RESERVE          Bit_RESET

//模块读取指定输入
LH_ERR ServiceImplModuleInputRead(uint8_t index,uint8_t indexMax,MODULE_INPUT_MAP_UNIT* arrayPtr,SENSOR_STATE* state);

//模块写入指定输出
LH_ERR ServiceImplModuleOutputWrite(uint8_t index,OUT_STATE state,uint8_t indexMax,MODULE_OUTPUT_MAP_UNIT* arrayPtr);











#endif




