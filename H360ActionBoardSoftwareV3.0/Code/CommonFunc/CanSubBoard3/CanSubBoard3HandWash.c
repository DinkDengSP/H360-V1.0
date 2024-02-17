/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 15:58:38
**LastEditors: DengXiaoJun
**LastEditTime: 2020-03-05 13:37:43
**FilePath: \H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard3\CanSubBoard3HandWash.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CanSubBoard3HandWash.h"


//指令板号
#define BOARD_ID_HAND_WASH  CAN_MASTER_SUB_BOARD3

//通道指令
typedef enum CMD_HAND_WASH
{
    CMD_HAND_WASH_RESET                     = 0x00000000,//清洗机械手复位               
    CMD_HAND_WASH_ROTATE_TO_PLATE_REACTION  = 0x00000001,//清洗机械手旋转到反应盘 
    CMD_HAND_WASH_ROTATE_TO_PLATE_WASH      = 0X00000002,//清洗机械手旋转到清洗盘  
    CMD_HAND_WASH_ROTATE_TO_MEASURE_MODULE  = 0X00000003,//清洗机械手旋转到测量模块 
    CMD_HAND_WASH_CATCH_CUP_PLATE_REACTION  = 0X00000004,//清洗机械手反应盘取杯     
    CMD_HAND_WASH_PUT_CUP_PLATE_REACTION    = 0X00000005,//清洗机械手反应盘放杯         
    CMD_HAND_WASH_CATCH_CUP_PLATE_WASH      = 0X00000006,//清洗机械手清洗盘取杯         
    CMD_HAND_WASH_PUT_CUP_PLATE_WASH        = 0x00000007,//清洗机械手清洗盘放杯         
    CMD_HAND_WASH_CATCH_CUP_MEASURE_MODULE  = 0X00000008,//清洗机械手测量模块放杯
    CMD_HAND_WASH_PUT_CUP_MEASURE_MODULE    = 0X00000009,//清洗机械手测量模块取杯
    CMD_HAND_WASH_STEP_MOTOR_RESET          = 0X0000000A,//指定步进电机复位             
    CMD_HAND_WASH_STEP_MOTOR_RUN_STEPS      = 0X0000000B,//指定步进电机走指定步数       
}CMD_HAND_WASH;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN_CONFIG_UNIT cmdCanConfigHandWashArray[] = {
    {BOARD_ID_HAND_WASH    ,CAN_MASTER_CHANNEL_BOARD3_HAND_WASH ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL               },//清洗机械手复位                                 
    {BOARD_ID_HAND_WASH    ,CAN_MASTER_CHANNEL_BOARD3_HAND_WASH ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL               },//清洗机械手旋转到反应盘                            
    {BOARD_ID_HAND_WASH    ,CAN_MASTER_CHANNEL_BOARD3_HAND_WASH ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL               },//清洗机械手旋转到清洗盘                                    
    {BOARD_ID_HAND_WASH    ,CAN_MASTER_CHANNEL_BOARD3_HAND_WASH ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL               },//清洗机械手旋转到测量模块                            
    {BOARD_ID_HAND_WASH    ,CAN_MASTER_CHANNEL_BOARD3_HAND_WASH ,0  ,60000   ,3 ,CAN_SUB_DATA_MODULE_HAND_WASH    },//清洗机械手反应盘取杯                                  
    {BOARD_ID_HAND_WASH    ,CAN_MASTER_CHANNEL_BOARD3_HAND_WASH ,0  ,60000   ,3 ,CAN_SUB_DATA_MODULE_HAND_WASH    },//清洗机械手反应盘放杯                              
    {BOARD_ID_HAND_WASH    ,CAN_MASTER_CHANNEL_BOARD3_HAND_WASH ,0  ,60000   ,3 ,CAN_SUB_DATA_MODULE_HAND_WASH    },//清洗机械手清洗盘取杯                                             
    {BOARD_ID_HAND_WASH    ,CAN_MASTER_CHANNEL_BOARD3_HAND_WASH ,0  ,60000   ,3 ,CAN_SUB_DATA_MODULE_HAND_WASH    },//清洗机械手清洗盘放杯                                 
    {BOARD_ID_HAND_WASH    ,CAN_MASTER_CHANNEL_BOARD3_HAND_WASH ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL               },//清洗机械手测量模块放杯                     
    {BOARD_ID_HAND_WASH    ,CAN_MASTER_CHANNEL_BOARD3_HAND_WASH ,0  ,60000   ,0 ,CAN_BASE_DATA_NULL               },//清洗机械手测量模块取杯         
    {BOARD_ID_HAND_WASH    ,CAN_MASTER_CHANNEL_BOARD3_HAND_WASH ,1  ,60000   ,0 ,CAN_BASE_DATA_NULL               },//指定步进电机复位                        
    {BOARD_ID_HAND_WASH    ,CAN_MASTER_CHANNEL_BOARD3_HAND_WASH ,5  ,60000   ,7 ,CAN_BASE_DATA_MODULE_MOTOR       },//指定步进电机走指定步数                            
};



//清洗机械手复位
LH_ERR CanSubBoard3HandWash_Reset(void)
{

    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_WASH_RESET,cmdCanConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//清洗机械手旋转到反应盘
LH_ERR CanSubBoard3HandWash_RotatePlateReaction(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_WASH_ROTATE_TO_PLATE_REACTION,cmdCanConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//清洗机械手旋转到清洗盘
LH_ERR CanSubBoard3HandWash_RotatePlateWash(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_WASH_ROTATE_TO_PLATE_WASH,cmdCanConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//清洗机械手旋转到测量模块
LH_ERR CanSubBoard3HandWash_RotateMeasureModule(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_WASH_ROTATE_TO_MEASURE_MODULE,cmdCanConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//清洗机械手反应盘取杯
LH_ERR CanSubBoard3HandWash_CatchCupPlateReaction(SENSOR_STATE* cupExistPlateReactionOld)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_WASH_CATCH_CUP_PLATE_REACTION,cmdCanConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *cupExistPlateReactionOld = (SENSOR_STATE)(canMasterReadData.recvDataPtr[2]);    
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//清洗机械手反应盘放杯
LH_ERR CanSubBoard3HandWash_PutCupPlateReaction(SENSOR_STATE* cupExistPlateReactionOld)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_WASH_PUT_CUP_PLATE_REACTION,cmdCanConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *cupExistPlateReactionOld = (SENSOR_STATE)(canMasterReadData.recvDataPtr[2]);   
     
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//清洗机械手清洗盘取杯
LH_ERR CanSubBoard3HandWash_CatchCupPlateWash(SENSOR_STATE* cupExistPlateWashNew)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_WASH_CATCH_CUP_PLATE_WASH,cmdCanConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *cupExistPlateWashNew = (SENSOR_STATE)(canMasterReadData.recvDataPtr[2]);    

    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//清洗机械手清洗盘放杯
LH_ERR CanSubBoard3HandWash_PutCupPlateWash(SENSOR_STATE* cupExistPlateWashNew)
{
   CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_WASH_PUT_CUP_PLATE_WASH,cmdCanConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *cupExistPlateWashNew = (SENSOR_STATE)(canMasterReadData.recvDataPtr[2]);    

    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//清洗机械手测量模块放杯
LH_ERR CanSubBoard3HandWash_CatchCupMeasureModule(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_WASH_CATCH_CUP_MEASURE_MODULE,cmdCanConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//清洗机械手测量模块取杯
LH_ERR CanSubBoard3HandWash_PutCupMeasureModule(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_WASH_PUT_CUP_MEASURE_MODULE,cmdCanConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//指定步进电机复位
LH_ERR CanSubBoard3HandWash_StepMotorReset(INDEX_MOTOR_HAND_WASH index)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_WASH_STEP_MOTOR_RESET,cmdCanConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(index);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//指定步进电机走指定步数
LH_ERR CanSubBoard3HandWash_StepMotorRunSteps(INDEX_MOTOR_HAND_WASH index,int32_t steps,int32_t* posAfterRun)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_HAND_WASH_STEP_MOTOR_RUN_STEPS,cmdCanConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(index);
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,1,steps);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigHandWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *posAfterRun = CanBaseReadDataConvertToInt32(canMasterReadData.recvDataPtr,3);
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}














