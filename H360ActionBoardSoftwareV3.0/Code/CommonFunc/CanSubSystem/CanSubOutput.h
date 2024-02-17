#ifndef __CAN_SUB_OUTPUT_H_
#define __CAN_SUB_OUTPUT_H_
#include "CanSubOutputIndex.h"
#include "CanMasterMain.h"

//信道指令支持
typedef enum CAN_CMD_OUTPUT
{
    CAN_CMD_OUTPUT_WRITE_SINGLE = 0X00000000,
}CAN_CMD_OUTPUT;

//写入系统内部某个IO状态
LH_ERR CanSubOutPutWriteSingle(CAN_SUB_IO_OUTPUT outputPinIndex,BitAction writeValue);


#endif





