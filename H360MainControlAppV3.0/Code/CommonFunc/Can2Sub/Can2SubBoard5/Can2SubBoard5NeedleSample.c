/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 16:01:34
**LastEditors: DengXiaoJun
**LastEditTime: 2020-09-15 10:36:07
**FilePath: \Code\CommonFunc\Can2SubBoard5\Can2SubBoard5NeedleSample.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "Can2SubBoard5NeedleSample.h"


//指令板号
#define BOARD_ID_NEEDLE_SAMPLE  CAN2_SUB_BOARD5

//通道指令
typedef enum CMD_NEEDLE_SAMPLE
{
    CMD_NEEDLE_SAMPLE_RESET                         = 0x00000000,//样本针复位
    CMD_NEEDLE_SAMPLE_PRIME_NORMAL                  = 0x00000001,//样本针普通清洗液灌注
    CMD_NEEDLE_SAMPLE_PRIME_POWERFUL                = 0X00000002,//样本针强力清洗液灌注
    CMD_NEEDLE_SAMPLE_CLEAN_NORMAL                  = 0X00000003,//样本针普通清洗
    CMD_NEEDLE_SAMPLE_CLEAN_POWERFUL                = 0X00000004,//样本针强力清洗
    CMD_NEEDLE_SAMPLE_MAINTAIN_CLEAN                = 0X00000005,//样本针清洗维护
    CMD_NEEDLE_SAMPLE_ROTATE                        = 0X00000006,//样本针旋转指定位置
    CMD_NEEDLE_SAMPLE_LIQUID_ABSORB_SAMPLE_RACK     = 0x00000007,//样本针吸试管样本
    CMD_NEEDLE_SAMPLE_LIQUID_ABSORB_PLATE_REACTION  = 0X00000008,//样本针吸反应盘样本
    CMD_NEEDLE_SAMPLE_LIQUID_INJECT                 = 0X00000009,//样本针注液
    CMD_NEEDLE_SAMPLE_LIQUID_DETECT_ADJUST          = 0X0000000A,//样本针探液校准
    CMD_NEEDLE_SAMPLE_LIQUID_DETECT_READ_STATE      = 0x0000000B,//样本针读探液板状态
    CMD_NEEDLE_SAMPLE_STEP_MOTOR_RESET              = 0X0000000C,//样本针模块步进电机复位
    CMD_NEEDLE_SAMPLE_STEP_MOTOR_RUN_STEPS          = 0X0000000D,//样本针模块步进电机走步数 
    CMD_NEEDLE_SAMPLE_LIQUID_DETECT_READ_RES_VOL    = 0X0000000E,//读取针尖电阻和针尖电压
    CMD_NEEDLE_SAMPLE_LIQUID_DETECT_WRITE_RES       = 0X0000000F,//写入探液板电阻值
    CMD_NEEDLE_SAMPLE_LIQUID_DETECT_READ_BASE_VOL   = 0X00000010,//读取针尖基础电压
    CMD_NEEDLE_SAMPLE_READ_UPDOWN_EMSTOP_POS        = 0X00000011,//读样本针升降急停高度
    CMD_NEEDLE_SAMPLE_READ_LIQUID_BOARD_VERSION     = 0X00000012,//读取探液板程序版本
}CMD_MEARURE_ROOM;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN2_CONFIG_UNIT cmdCan2ConfigNeedleSampleArray[] = {
    {BOARD_ID_NEEDLE_SAMPLE    ,CAN2_CHANNEL_BOARD5_NEEDLE_SAMPLE ,0  ,60000    ,0   ,CAN2_SUB_DATA_NULL                 },//样本针复位  
    {BOARD_ID_NEEDLE_SAMPLE    ,CAN2_CHANNEL_BOARD5_NEEDLE_SAMPLE ,1  ,60000    ,0   ,CAN2_SUB_DATA_NULL                 },//样本针普通清洗液灌注          
    {BOARD_ID_NEEDLE_SAMPLE    ,CAN2_CHANNEL_BOARD5_NEEDLE_SAMPLE ,0  ,60000    ,0   ,CAN2_SUB_DATA_NULL                 },//样本针强力清洗液灌注                  
    {BOARD_ID_NEEDLE_SAMPLE    ,CAN2_CHANNEL_BOARD5_NEEDLE_SAMPLE ,2  ,60000    ,0   ,CAN2_SUB_DATA_NULL                 },//样本针普通清洗           
    {BOARD_ID_NEEDLE_SAMPLE    ,CAN2_CHANNEL_BOARD5_NEEDLE_SAMPLE ,1  ,60000    ,0   ,CAN2_SUB_DATA_NULL                 },//样本针强力清洗             
    {BOARD_ID_NEEDLE_SAMPLE    ,CAN2_CHANNEL_BOARD5_NEEDLE_SAMPLE ,2  ,60000    ,0   ,CAN2_SUB_DATA_NULL                 },//样本针清洗维护             
    {BOARD_ID_NEEDLE_SAMPLE    ,CAN2_CHANNEL_BOARD5_NEEDLE_SAMPLE ,1  ,60000    ,0   ,CAN2_SUB_DATA_NULL                 },//样本针旋转指定位置                      
    {BOARD_ID_NEEDLE_SAMPLE    ,CAN2_CHANNEL_BOARD5_NEEDLE_SAMPLE ,5  ,60000    ,12   ,CAN2_SUB_DATA_MODULE_NEEDLE_SAMPLE },//样本针吸试管样本                  
    {BOARD_ID_NEEDLE_SAMPLE    ,CAN2_CHANNEL_BOARD5_NEEDLE_SAMPLE ,6  ,60000    ,6   ,CAN2_SUB_DATA_MODULE_NEEDLE_SAMPLE },//样本针吸反应盘样本                
    {BOARD_ID_NEEDLE_SAMPLE    ,CAN2_CHANNEL_BOARD5_NEEDLE_SAMPLE ,5  ,60000    ,0   ,CAN2_SUB_DATA_NULL                 },//样本针注液 
    {BOARD_ID_NEEDLE_SAMPLE    ,CAN2_CHANNEL_BOARD5_NEEDLE_SAMPLE ,0  ,60000    ,0   ,CAN2_SUB_DATA_NULL                 },//样本针探液校准                      
    {BOARD_ID_NEEDLE_SAMPLE    ,CAN2_CHANNEL_BOARD5_NEEDLE_SAMPLE ,1  ,60000    ,8   ,CAN2_SUB_DATA_MODULE_NEEDLE_SAMPLE },//样本针读探液板状态                  
    {BOARD_ID_NEEDLE_SAMPLE    ,CAN2_CHANNEL_BOARD5_NEEDLE_SAMPLE ,1  ,60000    ,0   ,CAN2_SUB_DATA_NULL                 },//样本针模块步进电机复位                
    {BOARD_ID_NEEDLE_SAMPLE    ,CAN2_CHANNEL_BOARD5_NEEDLE_SAMPLE ,5  ,60000    ,7   ,CAN2_SUB_DATA_MODULE_MOTOR         },//样本针模块步进电机走步数  
    {BOARD_ID_NEEDLE_SAMPLE    ,CAN2_CHANNEL_BOARD5_NEEDLE_SAMPLE ,0  ,60000    ,6   ,CAN2_SUB_DATA_MODULE_NEEDLE_SAMPLE },//读取针尖电阻和针尖电压
    {BOARD_ID_NEEDLE_SAMPLE    ,CAN2_CHANNEL_BOARD5_NEEDLE_SAMPLE ,2  ,60000    ,0   ,CAN2_SUB_DATA_NULL                 },//写入探液板电阻值 
    {BOARD_ID_NEEDLE_SAMPLE    ,CAN2_CHANNEL_BOARD5_NEEDLE_SAMPLE ,0  ,60000    ,4   ,CAN2_SUB_DATA_MODULE_NEEDLE_SAMPLE },//读取针尖基准电压
    {BOARD_ID_NEEDLE_SAMPLE    ,CAN2_CHANNEL_BOARD5_NEEDLE_SAMPLE ,0  ,60000    ,4   ,CAN2_SUB_DATA_MODULE_NEEDLE_SAMPLE },//读样本针升降急停高度
    {BOARD_ID_NEEDLE_SAMPLE    ,CAN2_CHANNEL_BOARD5_NEEDLE_SAMPLE ,0  ,60000    ,4   ,CAN2_SUB_DATA_MODULE_NEEDLE_SAMPLE },//读取探液板程序版本
};  

//样本针复位,带零位偏移
LH_ERR Can2SubBoard5NeedleSample_Reset(void)
{
    CAN2_SEND_CMD can2SendCmd; 
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_RESET,cmdCan2ConfigNeedleSampleArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
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

//样本针清洗液灌注
LH_ERR Can2SubBoard5NeedleSample_PrimeNormal(NEEDLE_PRIME_OPT opt)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_PRIME_NORMAL,cmdCan2ConfigNeedleSampleArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)opt;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
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

//样本针强力清洗液灌注
LH_ERR Can2SubBoard5NeedleSample_PrimePowerful(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_PRIME_POWERFUL,cmdCan2ConfigNeedleSampleArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)opt;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
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

//样本针清洗
LH_ERR Can2SubBoard5NeedleSample_CleanNormal(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_CLEAN_NORMAL,cmdCan2ConfigNeedleSampleArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(opt);
    can2SendCmd.paramBuffer[1] = (uint8_t)(time);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
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

//样本针强力清洗
LH_ERR Can2SubBoard5NeedleSample_CleanPowerful(uint8_t count)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_CLEAN_POWERFUL,cmdCan2ConfigNeedleSampleArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(count);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
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

//样本针清洗维护
LH_ERR Can2SubBoard5NeedleSample_MaintainClean(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_MAINTAIN_CLEAN,cmdCan2ConfigNeedleSampleArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(opt);
    can2SendCmd.paramBuffer[1] = (uint8_t)(time);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
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

//样本针旋转指定位置
LH_ERR Can2SubBoard5NeedleSample_Rotate(NEEDLE_SAMPLE_POS targetPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_ROTATE,cmdCan2ConfigNeedleSampleArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(targetPos);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
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

//样本针吸试管样本
LH_ERR Can2SubBoard5NeedleSample_LiquidAbsorbSample(uint16_t CountMinUl,uint16_t lastLiquidDetectPos,TUBE_TYPE tubeType,uint16_t* currentLiquidDetectPos,uint16_t* pumpMinUlCount
                                                            ,LIQUID_FUNC_RESULT* liquidResultRerurn,LIQUID_NEEDLE_LIQUID_DETECT_RESULT *needleLiquidDetectResult,LIQUID_DETECT_NEEDLE_VOL_UNIT *needleVolReturnPtr)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_LIQUID_ABSORB_SAMPLE_RACK,cmdCan2ConfigNeedleSampleArray);
    //如果存在数据需要传送,在这里填充数据
    Can2ProcUtilConvertUint16ToArray(can2SendCmd.paramBuffer,0,CountMinUl);
    Can2ProcUtilConvertUint16ToArray(can2SendCmd.paramBuffer,2,lastLiquidDetectPos);
    can2SendCmd.paramBuffer[4] = (uint8_t)(tubeType);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *currentLiquidDetectPos = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,2);
    *pumpMinUlCount = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,4);
    *liquidResultRerurn = (LIQUID_FUNC_RESULT)(can2ReadData.recvDataPtr[6]);
    *needleLiquidDetectResult =(LIQUID_NEEDLE_LIQUID_DETECT_RESULT)(can2ReadData.recvDataPtr[7]);
    needleVolReturnPtr->liquidDetectBaseVol = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,8);
    needleVolReturnPtr->liquidDetectNeedleVol = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,10);
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//样本针吸反应盘样本
LH_ERR Can2SubBoard5NeedleSample_LiquidAbsorbPlateReaction(uint16_t CountMinUl,uint32_t cupLiquidAfterAbsorb,uint16_t* pumpMinUlCount,int16_t *needleDownPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_LIQUID_ABSORB_PLATE_REACTION,cmdCan2ConfigNeedleSampleArray);
    //如果存在数据需要传送,在这里填充数据
    Can2ProcUtilConvertUint16ToArray(can2SendCmd.paramBuffer,0,CountMinUl);
    Can2ProcUtilConvertUint32ToArray(can2SendCmd.paramBuffer,2,cupLiquidAfterAbsorb);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *pumpMinUlCount = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,2);
    *needleDownPos = Can2ProcUtilConvertArrayToInt16(can2ReadData.recvDataPtr,4);
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//样本针注液
LH_ERR Can2SubBoard5NeedleSample_LiquidInject(uint32_t cupLiquidUl,uint8_t mixCount)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_LIQUID_INJECT,cmdCan2ConfigNeedleSampleArray);
    //如果存在数据需要传送,在这里填充数据
    Can2ProcUtilConvertUint32ToArray(can2SendCmd.paramBuffer,0,cupLiquidUl);
    can2SendCmd.paramBuffer[4] = (uint8_t)(mixCount);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
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

//样本针探液校准
LH_ERR Can2SubBoard5NeedleSample_LiquidDetectAdjust(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_LIQUID_DETECT_ADJUST,cmdCan2ConfigNeedleSampleArray);
    //如果存在数据需要传送,在这里填充数据

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
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

//读样本针探液板状态
LH_ERR Can2SubBoard5NeedleSample_LiquidDetectReadState(LIQUID_DETECT_CONFIG config,uint16_t* needleVol,uint16_t* thresholdValue,uint16_t* slopeValue)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_LIQUID_DETECT_READ_STATE,cmdCan2ConfigNeedleSampleArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(config);    

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
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

//指定步进电机复位
LH_ERR Can2SubBoard5NeedleSample_StepMotorReset(INDEX_MOTOR_NEEDLE_SAMPLE index)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_STEP_MOTOR_RESET,cmdCan2ConfigNeedleSampleArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(index);    

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
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
LH_ERR Can2SubBoard5NeedleSample_StepMotorRunSteps(INDEX_MOTOR_NEEDLE_SAMPLE index,int32_t steps,int32_t* posAfterRun)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_STEP_MOTOR_RUN_STEPS,cmdCan2ConfigNeedleSampleArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)index;
    Can2ProcUtilConvertInt32ToArray(can2SendCmd.paramBuffer,1,steps); 

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
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
LH_ERR Can2SubBoard5NeedleSample_LiquidDetectReadResVol(uint16_t* resValue,uint16_t* volValue)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_LIQUID_DETECT_READ_RES_VOL,cmdCan2ConfigNeedleSampleArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
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
LH_ERR Can2SubBoard5NeedleSample_LiquidDetectWriteRes(uint16_t resValue)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_LIQUID_DETECT_WRITE_RES,cmdCan2ConfigNeedleSampleArray);
    //如果存在数据需要传送,在这里填充数据
    Can2ProcUtilConvertUint16ToArray(can2SendCmd.paramBuffer,0,resValue);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
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
LH_ERR Can2SubBoard5NeedleSample_LiquidDetectReadNeedleBaseVol(uint16_t* needleBaseVol)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_LIQUID_DETECT_READ_BASE_VOL,cmdCan2ConfigNeedleSampleArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0];

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
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

//样本针携带维护
LH_ERR Can2SubBoard5NeedleSample_InjectWithMaintain(uint8_t maintainCount,uint16_t injectUl)
{
    // CAN2_SEND_CMD can2SendCmd;
    // CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    // //初始化发送接收的结构,但是数据没有填充,需要自己填充
    // Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_INJECT_WITH_MAINTAIN,cmdCan2ConfigNeedleSampleArray);
    // //如果存在数据需要传送,在这里填充数据
    // can2SendCmd.paramBuffer[0] = maintainCount;
    // Can2ProcUtilConvertUint16ToArray(can2SendCmd.paramBuffer,1,injectUl);

    // //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    // errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
    // if(errCode != LH_ERR_NONE)
    // {
    //     return errCode;
    // }
    // //数据解析,此处需要每个人自己写
    
    // //释放接收内存
    // if(can2ReadData.recvDataPtr != NULL)
    // {
    //     UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    // }
    return errCode;
}

//读样本针升降急停高度
LH_ERR Can2SubBoard5NeedleSample_ReadNeedleUpDownEmStopPos(uint16_t *needleEmStopPos)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_READ_UPDOWN_EMSTOP_POS,cmdCan2ConfigNeedleSampleArray);
    //如果存在数据需要传送,在这里填充数据
    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *needleEmStopPos = Can2ProcUtilConvertArrayToUint16(can2ReadData.recvDataPtr,2);
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//读探液板程序版本
LH_ERR Can2SubBoard5NeedleSample_ReadLiquidBoardVersion(uint16_t *boardVersionPtr)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_NEEDLE_SAMPLE_READ_LIQUID_BOARD_VERSION,cmdCan2ConfigNeedleSampleArray);
    //如果存在数据需要传送,在这里填充数据
    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigNeedleSampleArray);
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
















