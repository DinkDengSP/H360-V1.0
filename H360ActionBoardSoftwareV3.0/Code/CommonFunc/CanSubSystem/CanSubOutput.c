#include "CanSubOutput.h"
#include "BoardInc.h"
#include "SelfBoardID.h"

static LH_ERR CanSubOutputWriteCalcPinIndex(CAN_SUB_IO_OUTPUT outPinIndexSrc,CAN_MASTER_SUB_BOARD* boardIndex,CAN_MASTER_CHANNEL* channelIndex,uint8_t* outPinIndexTarget)
{
    *channelIndex = CAN_MASTER_CHANNEL_OUTPUT;
    //超过最大值
    if(outPinIndexSrc > CAN_SUB_OUT_PIN_BOARD10_MCU_YM_ARM4_PG15)
    {
        return LH_ERR_CAN_MASTER_OUTPUT_PIN_INDEX;
    }
    if(outPinIndexSrc <= CAN_SUB_OUT_PIN_BOARD1_MCU_YM_ARM4_PG15)
    {
        *outPinIndexTarget = (uint8_t)(outPinIndexSrc - CAN_SUB_OUT_PIN_BOARD1_CPLD_CDC1);
        *boardIndex = CAN_MASTER_SUB_BOARD1;
        return LH_ERR_NONE;
    }
    else if(outPinIndexSrc <= CAN_SUB_OUT_PIN_BOARD2_MCU_YM_ARM4_PG15)
    {
        *outPinIndexTarget = (uint8_t)(outPinIndexSrc - CAN_SUB_OUT_PIN_BOARD2_CPLD_CDC1);
        *boardIndex = CAN_MASTER_SUB_BOARD2;
        return LH_ERR_NONE;
    }
    else if(outPinIndexSrc <= CAN_SUB_OUT_PIN_BOARD3_MCU_YM_ARM4_PG15)
    {
        *outPinIndexTarget = (uint8_t)(outPinIndexSrc - CAN_SUB_OUT_PIN_BOARD3_CPLD_CDC1);
        *boardIndex = CAN_MASTER_SUB_BOARD3;
        return LH_ERR_NONE;
    }
    else if(outPinIndexSrc <= CAN_SUB_OUT_PIN_BOARD4_MCU_YM_ARM4_PG15)
    {
        *outPinIndexTarget = (uint8_t)(outPinIndexSrc - CAN_SUB_OUT_PIN_BOARD4_CPLD_CDC1);
        *boardIndex = CAN_MASTER_SUB_BOARD4;
        return LH_ERR_NONE;
    }
    else if(outPinIndexSrc <= CAN_SUB_OUT_PIN_BOARD5_MCU_YM_ARM4_PG15)
    {
        *outPinIndexTarget = (uint8_t)(outPinIndexSrc - CAN_SUB_OUT_PIN_BOARD5_CPLD_CDC1);
        *boardIndex = CAN_MASTER_SUB_BOARD5;
        return LH_ERR_NONE;
    }
    else if(outPinIndexSrc <= CAN_SUB_OUT_PIN_BOARD6_MCU_YM_ARM4_PG15)
    {
        *outPinIndexTarget = (uint8_t)(outPinIndexSrc - CAN_SUB_OUT_PIN_BOARD6_CPLD_CDC1);
        *boardIndex = CAN_MASTER_SUB_BOARD6;
        return LH_ERR_NONE;
    }
    else if(outPinIndexSrc <= CAN_SUB_OUT_PIN_BOARD7_MCU_YM_ARM4_PG15)
    {
        *outPinIndexTarget = (uint8_t)(outPinIndexSrc - CAN_SUB_OUT_PIN_BOARD7_CPLD_CDC1);
        *boardIndex = CAN_MASTER_SUB_BOARD7;
        return LH_ERR_NONE;
    }
    else if(outPinIndexSrc <= CAN_SUB_OUT_PIN_BOARD8_MCU_YM_ARM4_PG15)
    {
        *outPinIndexTarget = (uint8_t)(outPinIndexSrc - CAN_SUB_OUT_PIN_BOARD8_CPLD_CDC1);
        *boardIndex = CAN_MASTER_SUB_BOARD8;
        return LH_ERR_NONE;
    }
    else if(outPinIndexSrc <= CAN_SUB_OUT_PIN_BOARD9_MCU_YM_ARM4_PG15)
    {
        *outPinIndexTarget = (uint8_t)(outPinIndexSrc - CAN_SUB_OUT_PIN_BOARD9_CPLD_CDC1);
        *boardIndex = CAN_MASTER_SUB_BOARD9;
        return LH_ERR_NONE;
    }
    else if(outPinIndexSrc <= CAN_SUB_OUT_PIN_BOARD10_MCU_YM_ARM4_PG15)
    {
        *outPinIndexTarget = (uint8_t)(outPinIndexSrc - CAN_SUB_OUT_PIN_BOARD10_CPLD_CDC1);
        *boardIndex = CAN_MASTER_SUB_BOARD10;
        return LH_ERR_NONE;
    }
    return LH_ERR_NONE;
}


//写入系统内部某个IO状态
LH_ERR CanSubOutPutWriteSingle(CAN_SUB_IO_OUTPUT outputPinIndex,BitAction writeValue)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    uint8_t outputPinTarget = 0;
    //初始化接收器
    canMasterReadData.recvDataPtr = NULL;
    canMasterReadData.recvDatLength = 0;

    //计算板卡号码和单元号码
    errCode = CanSubOutputWriteCalcPinIndex(outputPinIndex,&(canMasterSendCmd.targetBoardID),&(canMasterSendCmd.channelNo),&outputPinTarget);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }

    //如果计算出来要读取的IO位于自己的板子上,那么就不用通讯
    if((uint8_t)(canMasterSendCmd.targetBoardID) == (uint8_t)(SELF_BOARD_ID))
    {
        BoardOutWrite((BOARD_OUT_PIN)outputPinTarget,writeValue);
        return LH_ERR_NONE;
    }

    //申请内存
    canMasterSendCmd.paramBuffer = NULL;
    canMasterSendCmd.paramLength = 2;
    do
    {
        canMasterSendCmd.paramBuffer = UserMemMalloc(SRAM_IN,2);
        if(canMasterSendCmd.paramBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(canMasterSendCmd.paramBuffer == NULL);

    //指令码设定
    canMasterSendCmd.commandCode = CAN_CMD_OUTPUT_WRITE_SINGLE;
    canMasterSendCmd.timeoutMs = 1000;
    //写入参数
    canMasterSendCmd.paramBuffer[0] = outputPinTarget;
    canMasterSendCmd.paramBuffer[1] = writeValue;
    //数据发送
    errCode = CanMasterSendCommandWhileReturn(&canMasterSendCmd,&canMasterReadData);
    //发送完成,释放发送内存
    if(canMasterSendCmd.paramBuffer != NULL)
    {
        UserMemFree(SRAM_IN,canMasterSendCmd.paramBuffer);
    }
    //接收到的数据不需要
    if(canMasterReadData.recvDataPtr != NULL)
    {
        //释放内存
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}
