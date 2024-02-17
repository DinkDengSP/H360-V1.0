#include "CanSubInput.h"
#include "BoardInc.h"
#include "SelfBoardID.h"

//计算慕目标板号和指令参数
LH_ERR CanSubInputCalcBoardChannel(CAN_SUB_IN_PIN_INDEX pinIndex,CAN_MASTER_SUB_BOARD* boardIndex,CAN_MASTER_CHANNEL* channelIndex,uint8_t* pinInputTarget)
{
    *channelIndex = CAN_MASTER_CHANNEL_INPUT;
    //超过最大值
    if(pinIndex > CAN_SUB_IN_PIN_BOARD10_MCU_YM_ARM2_PB5)
    {
        return LH_ERR_CAN_MASTER_INPUT_PIN_INDEX;
    }
    if(pinIndex <= CAN_SUB_IN_PIN_BOARD1_MCU_YM_ARM2_PB5)
    {
        *pinInputTarget = (uint8_t)(pinIndex - CAN_SUB_IN_PIN_BOARD1_CPLD_NOP1);
        *boardIndex = CAN_MASTER_SUB_BOARD1;
        return LH_ERR_NONE;
    }
    else if(pinIndex <= CAN_SUB_IN_PIN_BOARD2_MCU_YM_ARM2_PB5)
    {
        *pinInputTarget = (uint8_t)(pinIndex - CAN_SUB_IN_PIN_BOARD2_CPLD_NOP1);
        *boardIndex = CAN_MASTER_SUB_BOARD2;
        return LH_ERR_NONE;
    }
    else if(pinIndex <= CAN_SUB_IN_PIN_BOARD3_MCU_YM_ARM2_PB5)
    {
        *pinInputTarget = (uint8_t)(pinIndex - CAN_SUB_IN_PIN_BOARD3_CPLD_NOP1);
        *boardIndex = CAN_MASTER_SUB_BOARD3;
        return LH_ERR_NONE;
    }
    else if(pinIndex <= CAN_SUB_IN_PIN_BOARD4_MCU_YM_ARM2_PB5)
    {
        *pinInputTarget = (uint8_t)(pinIndex - CAN_SUB_IN_PIN_BOARD4_CPLD_NOP1);
        *boardIndex = CAN_MASTER_SUB_BOARD4;
        return LH_ERR_NONE;
    }
    else if(pinIndex <= CAN_SUB_IN_PIN_BOARD5_MCU_YM_ARM2_PB5)
    {
        *pinInputTarget = (uint8_t)(pinIndex - CAN_SUB_IN_PIN_BOARD5_CPLD_NOP1);
        *boardIndex = CAN_MASTER_SUB_BOARD5;
        return LH_ERR_NONE;
    }
    else if(pinIndex <= CAN_SUB_IN_PIN_BOARD6_MCU_YM_ARM2_PB5)
    {
        *pinInputTarget = (uint8_t)(pinIndex - CAN_SUB_IN_PIN_BOARD6_CPLD_NOP1);
        *boardIndex = CAN_MASTER_SUB_BOARD6;
        return LH_ERR_NONE;
    }
    else if(pinIndex <= CAN_SUB_IN_PIN_BOARD7_MCU_YM_ARM2_PB5)
    {
        *pinInputTarget = (uint8_t)(pinIndex - CAN_SUB_IN_PIN_BOARD7_CPLD_NOP1);
        *boardIndex = CAN_MASTER_SUB_BOARD7;
        return LH_ERR_NONE;
    }
    else if(pinIndex <= CAN_SUB_IN_PIN_BOARD8_MCU_YM_ARM2_PB5)
    {
        *pinInputTarget = (uint8_t)(pinIndex - CAN_SUB_IN_PIN_BOARD8_CPLD_NOP1);
        *boardIndex = CAN_MASTER_SUB_BOARD8;
        return LH_ERR_NONE;
    }
    else if(pinIndex <= CAN_SUB_IN_PIN_BOARD9_MCU_YM_ARM2_PB5)
    {
        *pinInputTarget = (uint8_t)(pinIndex - CAN_SUB_IN_PIN_BOARD9_CPLD_NOP1);
        *boardIndex = CAN_MASTER_SUB_BOARD9;
        return LH_ERR_NONE;
    }
    else if(pinIndex <= CAN_SUB_IN_PIN_BOARD10_MCU_YM_ARM2_PB5)
    {
        *pinInputTarget = (uint8_t)(pinIndex - CAN_SUB_IN_PIN_BOARD10_CPLD_NOP1);
        *boardIndex = CAN_MASTER_SUB_BOARD10;
        return LH_ERR_NONE;
    }
    return LH_ERR_NONE;
}


//读取系统内部某个IO状态
LH_ERR CanSubInputReadState(CAN_SUB_IN_PIN_INDEX pinIndex,BitAction* pinValue)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    canMasterReadData.recvDataPtr = NULL;
    canMasterReadData.recvDatLength = 0;

    uint8_t pinIndexTarget = 0;

    //计算板卡号码和单元号码
    errCode = CanSubInputCalcBoardChannel(pinIndex,&(canMasterSendCmd.targetBoardID),&(canMasterSendCmd.channelNo),&pinIndexTarget);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }

    //如果计算出来要读取的IO位于自己的板子上,那么就不用通讯
    if((uint8_t)(canMasterSendCmd.targetBoardID) == (uint8_t)(SELF_BOARD_ID))
    {
        *pinValue = BoardInReadValue((BOARD_IN_PIN)pinIndexTarget);
        return LH_ERR_NONE;
    }

    //申请内存
    canMasterSendCmd.paramBuffer = NULL;
    canMasterSendCmd.paramLength = 1;
    do
    {
        canMasterSendCmd.paramBuffer = UserMemMalloc(SRAM_IN,1);
        if(canMasterSendCmd.paramBuffer == NULL)
        {
            CoreDelayMinTick();
        }
    }while(canMasterSendCmd.paramBuffer == NULL);

    //指令码设定
    canMasterSendCmd.commandCode = CAN_SUB_INPUT_CMD_READ_SINGLE;
    canMasterSendCmd.timeoutMs = 1000;
    canMasterSendCmd.paramBuffer[0] = pinIndexTarget;

    //数据发送
    errCode = CanMasterSendCommandWhileReturn(&canMasterSendCmd,&canMasterReadData);
    //发送完成,释放发送内存
    if(canMasterSendCmd.paramBuffer != NULL)
    {
        UserMemFree(SRAM_IN,canMasterSendCmd.paramBuffer);
    }

    //首先看指令是否出错
    if(errCode != LH_ERR_NONE)
    {
        //错误产生
        if(canMasterReadData.recvDataPtr != NULL)
        {
            //释放内存
            UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
        }
        return errCode;
    }
    //数据为空
    if(canMasterReadData.recvDataPtr == NULL)
    {
        return LH_ERR_CAN_MASTER_INPUT_DADA_NULL;
    }
    //接收到的数据不需要
    if(canMasterReadData.recvDatLength != 4)
    {
        //释放接收内存
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
        //释放内存
        return LH_ERR_CAN_MASTER_INPUT_DADA_LENGTH;
    }
    //读取数据
    if(CAN_BASE_DATA_IO_IN != CanBaseReadDataConvertToUint16(canMasterReadData.recvDataPtr,0))
    {
        //释放接收内存
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
        //数据标志位错误
        return LH_ERR_CAN_MASTER_INPUT_DADA_FLAG;
    }
    if(canMasterReadData.recvDataPtr[2] != pinIndexTarget)
    {
        //释放接收内存
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
        //数据标志位错误
        return LH_ERR_CAN_MASTER_INPUT_DADA_INDEX;
    }
    //读取坐标
    *pinValue = (BitAction)canMasterReadData.recvDataPtr[3];
    //释放接收内存
    UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    return errCode;
}

//指定板卡上所有步进电机急停
LH_ERR CanSubInputBoardMotorEmergency(CAN_MASTER_SUB_BOARD board)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化接收器
    canMasterReadData.recvDataPtr = NULL;
    canMasterReadData.recvDatLength = 0;

    canMasterSendCmd.targetBoardID = board;
    canMasterSendCmd.channelNo = CAN_MASTER_CHANNEL_INPUT;

    //不需要申请内存
    canMasterSendCmd.paramBuffer = NULL;
    canMasterSendCmd.paramLength = 0;

    //指令码设定
    canMasterSendCmd.commandCode = CAN_SUB_INPUT_CMD_BOARD_MOTOR_EMERGENCY;
    canMasterSendCmd.timeoutMs = 1000;

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











