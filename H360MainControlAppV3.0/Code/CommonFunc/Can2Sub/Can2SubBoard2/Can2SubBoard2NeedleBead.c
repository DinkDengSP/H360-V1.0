/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 15:56:47
**LastEditors: DengXiaoJun
**LastEditTime: 2020-07-08 14:45:08
**FilePath: \Code\CommonFunc\Can2SubBoard2\Can2SubBoard2NeedleBead.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "Can2SubBoard2NeedleBead.h"

//指令板号
#define BOARD_ID_NEEDLE_BEAD     CAN2_SUB_BOARD2

//通道指令
typedef enum CMD_NEEDLE_BEAD
{
    CMD_NEEDLE_BEAD_RESET                       = 0X00000000,//磁珠针复位
    CMD_NEEDLE_BEAD_CLEAN                       = 0X00000001,//磁珠针清洗
    CMD_NEEDLE_BEAD_PRIME                       = 0X00000002,//磁珠针灌注
    CMD_NEEDLE_BEAD_MAINTAIN_CLEAN              = 0X00000003,//磁珠针清洗维护
    CMD_NEEDLE_BEAD_ROTATE                      = 0X00000004,//磁珠针旋转指定位置
    CMD_NEEDLE_BEAD_LIQUID_ABSORB               = 0X00000005,//磁珠针吸液
    CMD_NEEDLE_BEAD_LIQUID_INJECT               = 0X00000006,//磁珠针注液
    CMD_NEEDLE_BEAD_LIQUID_DETECT_ADJUST        = 0X00000007,//磁珠针探液校准
    CMD_NEEDLE_BEAD_LIQUID_DETECT_READ_STATE    = 0X00000008,//磁珠针探液板状态
    CMD_NEEDLE_BEAD_STEP_MOTOR_RESET            = 0X00000009,//磁珠针模块电机复位
    CMD_NEEDLE_BEAD_STEP_MOTOR_RUN_STEPS        = 0X0000000A,//磁珠针模块电机走步数
    CMD_NEEDLE_BEAD_LIQUID_DETECT_READ_RES_VOL  = 0X0000000B,//读取针尖电阻和针尖电压
    CMD_NEEDLE_BEAD_LIQUID_DETECT_WRITE_RES     = 0X0000000C,//写入探液板电阻值
    CMD_NEEDLE_BEAD_LIQUID_DETECT_READ_BASE_VOL = 0X0000000D,//读取针尖基准电压
	CMD_NEEDLE_BEAD_LIQUID_RESIDUE_CHECK        = 0X0000000E,//磁珠针试剂余量检查
    CMD_NEEDLE_BEAD_READ_UPDOWN_EMSTOP_POS      = 0X0000000F,//读磁珠针升降急停高度
    CMD_NEEDLE_BEAD_LITTLE_BEAD_PUMP_MIX        = 0X00000010,//磁珠针小磁珠吸吐混匀
    CMD_NEEDLE_BEAD_READ_LIQUID_BOARD_VERSION   = 0X00000011,//读取磁珠针探液板版本号
}CMD_NEEDLE_BEAD;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN2_CONFIG_UNIT cmdCan2ConfigNeedleBeadArray[] = {
    {BOARD_ID_NEEDLE_BEAD    ,CAN2_CHANNEL_BOARD2_NEEDLE_BEAD ,0  ,60000   ,0 ,CAN2_SUB_DATA_NULL               },//磁珠针复位
    {BOARD_ID_NEEDLE_BEAD    ,CAN2_CHANNEL_BOARD2_NEEDLE_BEAD ,2  ,60000   ,0 ,CAN2_SUB_DATA_NULL               },//磁珠针清洗
    {BOARD_ID_NEEDLE_BEAD    ,CAN2_CHANNEL_BOARD2_NEEDLE_BEAD ,1  ,60000   ,0 ,CAN2_SUB_DATA_NULL               },//磁珠针灌注
    {BOARD_ID_NEEDLE_BEAD    ,CAN2_CHANNEL_BOARD2_NEEDLE_BEAD ,2  ,60000   ,0 ,CAN2_SUB_DATA_NULL               },//磁珠针清洗维护
    {BOARD_ID_NEEDLE_BEAD    ,CAN2_CHANNEL_BOARD2_NEEDLE_BEAD ,1  ,60000   ,0 ,CAN2_SUB_DATA_NULL               },//磁珠针旋转指定位置
    {BOARD_ID_NEEDLE_BEAD    ,CAN2_CHANNEL_BOARD2_NEEDLE_BEAD ,5  ,60000   ,13 ,CAN2_SUB_DATA_MODULE_NEEDLE_BEAD },//磁珠针吸液
    {BOARD_ID_NEEDLE_BEAD    ,CAN2_CHANNEL_BOARD2_NEEDLE_BEAD ,5  ,60000   ,0 ,CAN2_SUB_DATA_NULL               },//磁珠针注液
    {BOARD_ID_NEEDLE_BEAD    ,CAN2_CHANNEL_BOARD2_NEEDLE_BEAD ,0  ,60000   ,0 ,CAN2_SUB_DATA_NULL               },//磁珠针探液校准
    {BOARD_ID_NEEDLE_BEAD    ,CAN2_CHANNEL_BOARD2_NEEDLE_BEAD ,1  ,60000   ,8 ,CAN2_SUB_DATA_MODULE_NEEDLE_BEAD },//磁珠针探液板状态
    {BOARD_ID_NEEDLE_BEAD    ,CAN2_CHANNEL_BOARD2_NEEDLE_BEAD ,1  ,60000   ,0 ,CAN2_SUB_DATA_NULL               },//磁珠针模块电机复位
    {BOARD_ID_NEEDLE_BEAD    ,CAN2_CHANNEL_BOARD2_NEEDLE_BEAD ,5  ,60000   ,7 ,CAN2_SUB_DATA_MODULE_MOTOR       },//磁珠针模块电机走步数
    {BOARD_ID_NEEDLE_BEAD    ,CAN2_CHANNEL_BOARD2_NEEDLE_BEAD ,0  ,60000   ,6 ,CAN2_SUB_DATA_MODULE_NEEDLE_BEAD },//读取针尖电阻和针尖电压
    {BOARD_ID_NEEDLE_BEAD    ,CAN2_CHANNEL_BOARD2_NEEDLE_BEAD ,2  ,60000   ,0 ,CAN2_SUB_DATA_NULL               },//写入探液板电阻值
    {BOARD_ID_NEEDLE_BEAD    ,CAN2_CHANNEL_BOARD2_NEEDLE_BEAD ,0  ,60000   ,4 ,CAN2_SUB_DATA_MODULE_NEEDLE_BEAD },//读取针尖基准电压
	{BOARD_ID_NEEDLE_BEAD    ,CAN2_CHANNEL_BOARD2_NEEDLE_BEAD ,0  ,60000   ,6 ,CAN2_SUB_DATA_MODULE_NEEDLE_BEAD },//磁珠针试剂余量检查
    {BOARD_ID_NEEDLE_BEAD    ,CAN2_CHANNEL_BOARD2_NEEDLE_BEAD ,0  ,60000   ,4 ,CAN2_SUB_DATA_MODULE_NEEDLE_BEAD },//读磁珠针升降急停高度
    {BOARD_ID_NEEDLE_BEAD    ,CAN2_CHANNEL_BOARD2_NEEDLE_BEAD ,3  ,60000   ,0 ,CAN2_SUB_DATA_NULL               },//磁珠针小磁珠吸吐混匀
    {BOARD_ID_NEEDLE_BEAD    ,CAN2_CHANNEL_BOARD2_NEEDLE_BEAD ,0  ,60000   ,4 ,CAN2_SUB_DATA_MODULE_NEEDLE_BEAD },//读取磁珠针探液板版本号
};

//磁珠针复位,带零位偏移
LH_ERR Can2SubBoard2NeedleBead_Reset(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_BEAD_RESET,cmdCan2ConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//磁珠针清洗
LH_ERR Can2SubBoard2NeedleBead_Clean(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_BEAD_CLEAN,cmdCan2ConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(opt);
    can2SendCmd.paramBuffer[1] = (uint8_t)(time);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//磁珠针灌注
LH_ERR Can2SubBoard2NeedleBead_Prime(NEEDLE_PRIME_OPT opt)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_BEAD_PRIME,cmdCan2ConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(opt);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//磁珠针清洗维护
LH_ERR Can2SubBoard2NeedleBead_MaintainClean(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_BEAD_MAINTAIN_CLEAN,cmdCan2ConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(opt);
    can2SendCmd.paramBuffer[1] = (uint8_t)(time);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//磁珠针旋转指定位置
LH_ERR Can2SubBoard2NeedleBead_Rotate(NEEDLE_BEAD_POS targetPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_BEAD_ROTATE,cmdCan2ConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(targetPos);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//磁珠针吸液
LH_ERR Can2SubBoard2NeedleBead_LiquidAbsorb(uint16_t CountMinUl,uint16_t lastLiquidDetectPos,uint8_t liquidDetectStage,uint16_t* currentLiquidDetectPos,uint16_t* pumpMinUlCount
                                                ,LIQUID_FUNC_RESULT* liquidResultRerurn,LIQUID_NEEDLE_LIQUID_DETECT_RESULT *needleLiquidDetectResult,uint8_t*liquidDetectStageReturn,LIQUID_DETECT_NEEDLE_VOL_UNIT *needleVolReturnPtr)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_BEAD_LIQUID_ABSORB,cmdCan2ConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    Can2ProcUtilConvertUint16ToArray(can2SendCmd.paramBuffer,0,CountMinUl);
    Can2ProcUtilConvertUint16ToArray(can2SendCmd.paramBuffer,2,lastLiquidDetectPos);
    can2SendCmd.paramBuffer[4] = liquidDetectStage;
    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *currentLiquidDetectPos = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,2);
    *pumpMinUlCount = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,4);
    *liquidResultRerurn = (LIQUID_FUNC_RESULT)(can2ReadData.recvDataPtr[6]);
    *needleLiquidDetectResult =(LIQUID_NEEDLE_LIQUID_DETECT_RESULT)(can2ReadData.recvDataPtr[7]);
    *liquidDetectStageReturn = can2ReadData.recvDataPtr[8];
    needleVolReturnPtr->liquidDetectBaseVol = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,9);
    needleVolReturnPtr->liquidDetectNeedleVol = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,11);
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//磁珠针注液
LH_ERR Can2SubBoard2NeedleBead_LiquidInject(uint32_t cupLiquidUl,uint8_t mixCount)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_BEAD_LIQUID_INJECT,cmdCan2ConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    Can2ProcUtilConvertUint32ToArray(can2SendCmd.paramBuffer,0,cupLiquidUl);
    can2SendCmd.paramBuffer[4] = (uint8_t)(mixCount);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//磁珠针探液校准
LH_ERR Can2SubBoard2NeedleBead_LiquidDetectAdjust(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_BEAD_LIQUID_DETECT_ADJUST,cmdCan2ConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//读磁珠针探液板状态
LH_ERR Can2SubBoard2NeedleBead_LiquidDetectReadState(LIQUID_DETECT_CONFIG config,uint16_t* needleVol,uint16_t* thresholdValue,uint16_t* slopeValue)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_BEAD_LIQUID_DETECT_READ_STATE,cmdCan2ConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(config);
    
    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *needleVol = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,2);
    *thresholdValue = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,4);
    *slopeValue = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,6);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//磁珠针试剂余量探测
LH_ERR Can2SubBoard2NeedleBead_LiquidResidueCheck(uint16_t* currentLiquidDetectPos,LIQUID_FUNC_RESULT* liquidResultRerurn,LIQUID_NEEDLE_LIQUID_DETECT_RESULT *needleLiquidDetectResult)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_BEAD_LIQUID_RESIDUE_CHECK,cmdCan2ConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    
    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    * currentLiquidDetectPos = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,2);
    * liquidResultRerurn = (LIQUID_FUNC_RESULT)(can2ReadData.recvDataPtr[4]);
    * needleLiquidDetectResult = (LIQUID_NEEDLE_LIQUID_DETECT_RESULT)(can2ReadData.recvDataPtr[5]);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}
//指定步进电机复位
LH_ERR Can2SubBoard2NeedleBead_StepMotorReset(INDEX_MOTOR_NEEDLE_BEAD index)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_BEAD_STEP_MOTOR_RESET,cmdCan2ConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(index);
    
    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//指定步进电机走步数
LH_ERR Can2SubBoard2NeedleBead_StepMotorRunSteps(INDEX_MOTOR_NEEDLE_BEAD index,int32_t steps,int32_t* posAfterRun)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_BEAD_STEP_MOTOR_RUN_STEPS,cmdCan2ConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)index;
    Can2ProcUtilConvertInt32ToArray(can2SendCmd.paramBuffer,1,steps);
    
    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *posAfterRun = Can2ProcUtilConvertArrayToInt32(can2ReadData.recvDataPtr,3);
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}


//读取电阻和针尖电压
LH_ERR Can2SubBoard2NeedleBead_LiquidDetectReadResVol(uint16_t* resValue,uint16_t* volValue)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_BEAD_LIQUID_DETECT_READ_RES_VOL,cmdCan2ConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *resValue = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,2);
    *volValue = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,4);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//写入电阻值
LH_ERR Can2SubBoard2NeedleBead_LiquidDetectWriteRes(uint16_t resValue)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_BEAD_LIQUID_DETECT_WRITE_RES,cmdCan2ConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    Can2ProcUtilConvertUint16ToArray(can2SendCmd.paramBuffer,0,resValue);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//读取针尖基准电压
LH_ERR Can2SubBoard2NeedleBead_LiquidDetectReadNeedleBaseVol(uint16_t* needleBaseVol)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_BEAD_LIQUID_DETECT_READ_BASE_VOL,cmdCan2ConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *needleBaseVol = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,2);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}


//读磁珠针升降急停高度
LH_ERR Can2SubBoard2NeedleBead_ReadNeedleUpDownEmStopPos(uint16_t* needleEmStopPosPtr)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_BEAD_READ_UPDOWN_EMSTOP_POS,cmdCan2ConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *needleEmStopPosPtr = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,2);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}


//磁珠针小磁珠吸吐混匀
LH_ERR Can2SubBoard2NeedleBead_LittleBeadPumpMix(uint16_t mixLiquidUnit,uint8_t mixCount)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_BEAD_LITTLE_BEAD_PUMP_MIX,cmdCan2ConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    Can2ProcUtilConvertUint16ToArray(can2SendCmd.paramBuffer,0,mixLiquidUnit);
    can2SendCmd.paramBuffer[2] = mixCount;
    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//读磁珠针探液板软件版本号
LH_ERR Can2SubBoard2NeedleBead_ReadLiquidBoardVersion(uint16_t* boardVersionPtr)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_BEAD_READ_LIQUID_BOARD_VERSION,cmdCan2ConfigNeedleBeadArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleBeadArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *boardVersionPtr = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,2);

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}








