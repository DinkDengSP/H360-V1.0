
#ifndef __SERVICE_IPC_FATFS_LOG_H_
#define __SERVICE_IPC_FATFS_LOG_H_
#include "ServiceIpcBase.h"

//IPC调用基础结构初始化
void IPC_ServiceFatFsLogDataInit(void);
//查询任务的上一次执行结果,也就是有没有执行完成,非阻塞
STATE_IPC IPC_ServiceFatFsLogGetLastCommandStateAndResult(LH_ERR* resultCode,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);
//等待执行完成,阻塞
LH_ERR IPC_ServiceFatFsLogWaitLastReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);



/* 非阻塞指令区 */

//日志写入,不等待指令完成,非堵塞
LH_ERR IPC_ServiceFatFsLogWriteFileWhileAck(uint8_t *writeBufferPtr,uint32_t writeLength);

//刷新日志,不等待指令完成,非堵塞
LH_ERR IPC_ServiceFatFsLogFlushCacheDataWhileAck(void);





/* 阻塞指令区 */

//日志写入,等待指令完成,堵塞
LH_ERR IPC_ServiceFatFsLogWriteFileWhileReturn(uint8_t *writeBufferPtr,uint32_t writeLength,CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

//刷新日志,等待指令完成,堵塞
LH_ERR IPC_ServiceFatFsLogFlushCacheDataWhileReturn(CAN1_REPORT_ERR_LEVEL* can1ReportErrorLevel,CAN1_REPORT_ERR_CODE* can1ReportErrorCode);

#endif