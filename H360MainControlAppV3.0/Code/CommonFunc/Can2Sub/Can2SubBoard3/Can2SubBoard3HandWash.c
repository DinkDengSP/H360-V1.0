/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 15:58:38
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-17 14:05:04
**FilePath: \Code\CommonFunc\Can2SubBoard3\Can2SubBoard3HandWash.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "Can2SubBoard3HandWash.h"


//指令板号
#define BOARD_ID_HAND_WASH  CAN2_SUB_BOARD3

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
static const CMD_CAN2_CONFIG_UNIT cmdCan2ConfigHandWashArray[] = {
    {BOARD_ID_HAND_WASH    ,CAN2_CHANNEL_BOARD3_HAND_WASH ,0  ,60000   ,0 ,CAN2_SUB_DATA_NULL               },//清洗机械手复位                                 
    {BOARD_ID_HAND_WASH    ,CAN2_CHANNEL_BOARD3_HAND_WASH ,0  ,5000    ,0 ,CAN2_SUB_DATA_NULL               },//清洗机械手旋转到反应盘                            
    {BOARD_ID_HAND_WASH    ,CAN2_CHANNEL_BOARD3_HAND_WASH ,0  ,5000    ,0 ,CAN2_SUB_DATA_NULL               },//清洗机械手旋转到清洗盘                                    
    {BOARD_ID_HAND_WASH    ,CAN2_CHANNEL_BOARD3_HAND_WASH ,0  ,5000    ,0 ,CAN2_SUB_DATA_NULL               },//清洗机械手旋转到测量模块                            
    {BOARD_ID_HAND_WASH    ,CAN2_CHANNEL_BOARD3_HAND_WASH ,0  ,5000    ,3 ,CAN2_SUB_DATA_MODULE_HAND_WASH   },//清洗机械手反应盘取杯                                  
    {BOARD_ID_HAND_WASH    ,CAN2_CHANNEL_BOARD3_HAND_WASH ,0  ,5000    ,3 ,CAN2_SUB_DATA_MODULE_HAND_WASH   },//清洗机械手反应盘放杯                              
    {BOARD_ID_HAND_WASH    ,CAN2_CHANNEL_BOARD3_HAND_WASH ,0  ,5000    ,3 ,CAN2_SUB_DATA_MODULE_HAND_WASH   },//清洗机械手清洗盘取杯                                             
    {BOARD_ID_HAND_WASH    ,CAN2_CHANNEL_BOARD3_HAND_WASH ,0  ,5000    ,3 ,CAN2_SUB_DATA_MODULE_HAND_WASH   },//清洗机械手清洗盘放杯                                 
    {BOARD_ID_HAND_WASH    ,CAN2_CHANNEL_BOARD3_HAND_WASH ,0  ,5000    ,0 ,CAN2_SUB_DATA_NULL               },//清洗机械手测量模块放杯                     
    {BOARD_ID_HAND_WASH    ,CAN2_CHANNEL_BOARD3_HAND_WASH ,0  ,5000    ,0 ,CAN2_SUB_DATA_NULL               },//清洗机械手测量模块取杯         
    {BOARD_ID_HAND_WASH    ,CAN2_CHANNEL_BOARD3_HAND_WASH ,1  ,60000   ,0 ,CAN2_SUB_DATA_NULL               },//指定步进电机复位                        
    {BOARD_ID_HAND_WASH    ,CAN2_CHANNEL_BOARD3_HAND_WASH ,5  ,60000   ,7 ,CAN2_SUB_DATA_MODULE_MOTOR       },//指定步进电机走指定步数                            
};



//清洗机械手复位
LH_ERR Can2SubBoard3HandWash_Reset(void)
{

    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_HAND_WASH_RESET,cmdCan2ConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigHandWashArray);
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

//清洗机械手旋转到反应盘
LH_ERR Can2SubBoard3HandWash_RotatePlateReaction(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_HAND_WASH_ROTATE_TO_PLATE_REACTION,cmdCan2ConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigHandWashArray);
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

//清洗机械手旋转到清洗盘
LH_ERR Can2SubBoard3HandWash_RotatePlateWash(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_HAND_WASH_ROTATE_TO_PLATE_WASH,cmdCan2ConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigHandWashArray);
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

//清洗机械手旋转到测量模块
LH_ERR Can2SubBoard3HandWash_RotateMeasureModule(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_HAND_WASH_ROTATE_TO_MEASURE_MODULE,cmdCan2ConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigHandWashArray);
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

//清洗机械手反应盘取杯
LH_ERR Can2SubBoard3HandWash_CatchCupPlateReaction(SENSOR_STATE* cupExistPlateReactionOld)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_HAND_WASH_CATCH_CUP_PLATE_REACTION,cmdCan2ConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigHandWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *cupExistPlateReactionOld = (SENSOR_STATE)(can2ReadData.recvDataPtr[2]);    
    
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//清洗机械手反应盘放杯
LH_ERR Can2SubBoard3HandWash_PutCupPlateReaction(SENSOR_STATE* cupExistPlateReactionOld)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_HAND_WASH_PUT_CUP_PLATE_REACTION,cmdCan2ConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigHandWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *cupExistPlateReactionOld = (SENSOR_STATE)(can2ReadData.recvDataPtr[2]);   
     
    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//清洗机械手清洗盘取杯
LH_ERR Can2SubBoard3HandWash_CatchCupPlateWash(SENSOR_STATE* cupExistPlateWashNew)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_HAND_WASH_CATCH_CUP_PLATE_WASH,cmdCan2ConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigHandWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *cupExistPlateWashNew = (SENSOR_STATE)(can2ReadData.recvDataPtr[2]);    

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//清洗机械手清洗盘放杯
LH_ERR Can2SubBoard3HandWash_PutCupPlateWash(SENSOR_STATE* cupExistPlateWashNew)
{
   CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_HAND_WASH_PUT_CUP_PLATE_WASH,cmdCan2ConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigHandWashArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *cupExistPlateWashNew = (SENSOR_STATE)(can2ReadData.recvDataPtr[2]);    

    //释放接收内存
    if(can2ReadData.recvDataPtr != NULL)
    {
        UserMemFree(MEM_AXI_SRAM,can2ReadData.recvDataPtr);
    }
    return errCode;
}

//清洗机械手测量模块放杯
LH_ERR Can2SubBoard3HandWash_CatchCupMeasureModule(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_HAND_WASH_CATCH_CUP_MEASURE_MODULE,cmdCan2ConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigHandWashArray);
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

//清洗机械手测量模块取杯
LH_ERR Can2SubBoard3HandWash_PutCupMeasureModule(void)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_HAND_WASH_PUT_CUP_MEASURE_MODULE,cmdCan2ConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    //can2SendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigHandWashArray);
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

//指定步进电机复位
LH_ERR Can2SubBoard3HandWash_StepMotorReset(INDEX_MOTOR_HAND_WASH index)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_HAND_WASH_STEP_MOTOR_RESET,cmdCan2ConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(index);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigHandWashArray);
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

//指定步进电机走指定步数
LH_ERR Can2SubBoard3HandWash_StepMotorRunSteps(INDEX_MOTOR_HAND_WASH index,int32_t steps,int32_t* posAfterRun)
{
    CAN2_SEND_CMD can2SendCmd;
    CAN2_READ_DATA can2ReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    Can2SubBoard_CreateSendBuffer(&can2SendCmd,&can2ReadData,CMD_HAND_WASH_STEP_MOTOR_RUN_STEPS,cmdCan2ConfigHandWashArray);
    //如果存在数据需要传送,在这里填充数据
    can2SendCmd.paramBuffer[0] = (uint8_t)(index);
    Can2ProcUtilConvertInt32ToArray(can2SendCmd.paramBuffer,1,steps);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = Can2SubBoard_SendAndProcRespBase(&can2SendCmd,&can2ReadData,cmdCan2ConfigHandWashArray);
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














