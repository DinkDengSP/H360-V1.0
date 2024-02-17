/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 15:58:38
**LastEditors: DengXiaoJun
**LastEditTime: 2020-04-29 19:22:59
**FilePath: \H360_ActionBoard_ID_3d:\SVN_Local\02-Development\Software\H360_STM32\H360ActionBoardSoftwareV2.3\CommonFunc\CanSubBoard3\CanSubBoard3MeasureModule.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "CanSubBoard3MeasureModule.h"

//指令板号
#define BOARD_ID_MEASURE_MODULE  CAN_MASTER_SUB_BOARD3

//通道指令
typedef enum CMD_MEASURE_MODULE
{
    CMD_MEASURE_MODULE_RESET                    = 0x00000000,//测量模块复位                 
    CMD_MEASURE_MODULE_DOOR_OPEN_FULL           = 0x00000001,//测量室上门全开           
    CMD_MEASURE_MODULE_DOOR_OPEN_HALF           = 0X00000002,//测量室上门半开           
    CMD_MEASURE_MODULE_DOOR_CLOSE               = 0X00000003,//测量室上门关闭           
    CMD_MEASURE_MODULE_WINDOW_OPEN              = 0X00000004,//测量室侧门打开               
    CMD_MEASURE_MODULE_WINDOW_CLOSE             = 0X00000005,//测量室侧门关闭                   
    CMD_MEASURE_MODULE_PRIME_ONCE               = 0X00000006,//激发液灌注                   
    CMD_MEASURE_MODULE_INJECT_ONCE              = 0x00000007,//激发液注液                           
    CMD_MEASURE_MODULE_INJECT_MAINTAIN          = 0X00000008,//测量模块清洗维护                     
    CMD_MEASURE_MODULE_READ_DARK                = 0X00000009,//测量模块暗记数                   
    CMD_MEASURE_MODULE_READ_BACKGROUND          = 0X0000000A,//测量模块读本底          
    CMD_MEASURE_MODULE_READ_LIGHT_INJECT        = 0X0000000B,//测量模块注液测光       
    CMD_MEASURE_MODULE_READ_LIGHT_COMPLEX       = 0X0000000C,//测量模块集成单次测量         
    CMD_MEASURE_MODULE_READ_LIGHT_CHANGE_RATE   = 0X0000000D,//测量模块读光子变化率         
    CMD_MEASURE_MODULE_STEP_MOTOR_RESET         = 0X0000000E,//指定步进电机复位          
    CMD_MEASURE_MODULE_STEP_MOTOR_RUN_STEPS     = 0X0000000F,//指定步进电机走相应的步数       
}CMD_MEASURE_MODULE;

//通道指令发送返回的配置信息
//1. 指令的目标板号
//2. 指令的目标通道号
//3. 指令发出的数据长度  
//4. 指令运行超时时间,单位毫秒
//5. 指令需要接收的最小数据长度
//6. 需要接收到的数据标识
static const CMD_CAN_CONFIG_UNIT cmdCanConfigMeasureArray[] = {
    {BOARD_ID_MEASURE_MODULE    ,CAN_MASTER_CHANNEL_BOARD3_MEASURE_UNIT ,0  ,60000   ,0   ,CAN_BASE_DATA_NULL                 },//测量模块复位                                  
    {BOARD_ID_MEASURE_MODULE    ,CAN_MASTER_CHANNEL_BOARD3_MEASURE_UNIT ,0  ,60000   ,0   ,CAN_BASE_DATA_NULL                 },//测量室上门全开                                
    {BOARD_ID_MEASURE_MODULE    ,CAN_MASTER_CHANNEL_BOARD3_MEASURE_UNIT ,0  ,60000   ,0   ,CAN_BASE_DATA_NULL                 },//测量室上门半开                                        
    {BOARD_ID_MEASURE_MODULE    ,CAN_MASTER_CHANNEL_BOARD3_MEASURE_UNIT ,0  ,60000   ,0   ,CAN_BASE_DATA_NULL                 },//测量室上门关闭                                 
    {BOARD_ID_MEASURE_MODULE    ,CAN_MASTER_CHANNEL_BOARD3_MEASURE_UNIT ,0  ,60000   ,0   ,CAN_BASE_DATA_NULL                 },//测量室侧门打开                                     
    {BOARD_ID_MEASURE_MODULE    ,CAN_MASTER_CHANNEL_BOARD3_MEASURE_UNIT ,0  ,60000   ,0   ,CAN_BASE_DATA_NULL                 },//测量室侧门关闭                                 
    {BOARD_ID_MEASURE_MODULE    ,CAN_MASTER_CHANNEL_BOARD3_MEASURE_UNIT ,1  ,60000   ,0   ,CAN_BASE_DATA_NULL                 },//激发液灌注                                                  
    {BOARD_ID_MEASURE_MODULE    ,CAN_MASTER_CHANNEL_BOARD3_MEASURE_UNIT ,1  ,60000   ,0   ,CAN_BASE_DATA_NULL                 },//激发液注液                                      
    {BOARD_ID_MEASURE_MODULE    ,CAN_MASTER_CHANNEL_BOARD3_MEASURE_UNIT ,1  ,60000   ,0   ,CAN_BASE_DATA_NULL                 },//测量模块清洗维护                        
    {BOARD_ID_MEASURE_MODULE    ,CAN_MASTER_CHANNEL_BOARD3_MEASURE_UNIT ,0  ,60000   ,6   ,CAN_SUB_DATA_MODULE_MEASURE_UNIT   },//测量模块暗记数             
    {BOARD_ID_MEASURE_MODULE    ,CAN_MASTER_CHANNEL_BOARD3_MEASURE_UNIT ,0  ,60000   ,6   ,CAN_SUB_DATA_MODULE_MEASURE_UNIT   },//测量模块读本底                                 
    {BOARD_ID_MEASURE_MODULE    ,CAN_MASTER_CHANNEL_BOARD3_MEASURE_UNIT ,1  ,60000   ,6   ,CAN_SUB_DATA_MODULE_MEASURE_UNIT   },//测量模块注液测光                                               
    {BOARD_ID_MEASURE_MODULE    ,CAN_MASTER_CHANNEL_BOARD3_MEASURE_UNIT ,2  ,60000   ,8  ,CAN_SUB_DATA_MODULE_MEASURE_UNIT   },//测量模块集成单次测量                      
    {BOARD_ID_MEASURE_MODULE    ,CAN_MASTER_CHANNEL_BOARD3_MEASURE_UNIT ,2  ,60000   ,3   ,CAN_SUB_DATA_MODULE_MEASURE_UNIT   },//测量模块读光子变化率          
    {BOARD_ID_MEASURE_MODULE    ,CAN_MASTER_CHANNEL_BOARD3_MEASURE_UNIT ,1  ,60000   ,0   ,CAN_BASE_DATA_NULL                 },//指定步进电机复位                        
    {BOARD_ID_MEASURE_MODULE    ,CAN_MASTER_CHANNEL_BOARD3_MEASURE_UNIT ,5  ,60000   ,7   ,CAN_BASE_DATA_MODULE_MOTOR         },//指定步进电机走相应的步数                           
};


//测量模块复位
LH_ERR CanSubBoard3MeasureModule_Reset(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MEASURE_MODULE_RESET,cmdCanConfigMeasureArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMeasureArray);
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

//测量上门全开
LH_ERR CanSubBoard3MeasureModule_DoorOpenFull(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MEASURE_MODULE_DOOR_OPEN_FULL,cmdCanConfigMeasureArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMeasureArray);
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

//测量上门半开
LH_ERR CanSubBoard3MeasureModule_DoorOpenHalf(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MEASURE_MODULE_DOOR_OPEN_HALF,cmdCanConfigMeasureArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMeasureArray);
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

//测量上门关闭
LH_ERR CanSubBoard3MeasureModule_DoorClose(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MEASURE_MODULE_DOOR_CLOSE,cmdCanConfigMeasureArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMeasureArray);
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

//测量侧门打开
LH_ERR CanSubBoard3MeasureModule_WindowOpen(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MEASURE_MODULE_WINDOW_OPEN,cmdCanConfigMeasureArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMeasureArray);
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

//测量侧门关闭
LH_ERR CanSubBoard3MeasureModule_WindowClose(void)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MEASURE_MODULE_WINDOW_CLOSE,cmdCanConfigMeasureArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMeasureArray);
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

//激发液灌注
LH_ERR CanSubBoard3MeasureModule_PrimeOnce(BOTTLE_SELECT_MEASURE_MODULE bottle)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MEASURE_MODULE_PRIME_ONCE,cmdCanConfigMeasureArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(bottle);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMeasureArray);
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

//激发液注液
LH_ERR CanSubBoard3MeasureModule_InjectOnce(BOTTLE_SELECT_MEASURE_MODULE bottle)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MEASURE_MODULE_INJECT_ONCE,cmdCanConfigMeasureArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(bottle);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMeasureArray);
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

//测量模块清洗维护
LH_ERR CanSubBoard3MeasureModule_InjectMaintain(BOTTLE_SELECT_MEASURE_MODULE bottle)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MEASURE_MODULE_INJECT_MAINTAIN,cmdCanConfigMeasureArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(bottle);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMeasureArray);
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

//测量模块暗计数
LH_ERR CanSubBoard3MeasureModule_ReadDark(uint32_t* darkValue)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MEASURE_MODULE_READ_DARK,cmdCanConfigMeasureArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMeasureArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *darkValue = CanBaseReadDataConvertToUint32(canMasterReadData.recvDataPtr,2);   

    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//测量模块读本底
LH_ERR CanSubBoard3MeasureModule_ReadBackGround(uint32_t* backGroundValue)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MEASURE_MODULE_READ_BACKGROUND,cmdCanConfigMeasureArray);
    //如果存在数据需要传送,在这里填充数据
    //canMasterSendCmd.paramBuffer[0] = (uint8_t)mixMode;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMeasureArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *backGroundValue = CanBaseReadDataConvertToUint32(canMasterReadData.recvDataPtr,2);   

    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//测量模块注液测光
LH_ERR CanSubBoard3MeasureModule_ReadLightWithInject(BOTTLE_SELECT_MEASURE_MODULE bottle,uint32_t* lightValue)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MEASURE_MODULE_READ_LIGHT_INJECT,cmdCanConfigMeasureArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(bottle);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMeasureArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *lightValue = CanBaseReadDataConvertToUint32(canMasterReadData.recvDataPtr,2);   
    
    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//测量模块集成单次测量
LH_ERR CanSubBoard3MeasureModule_ReadLightComplex(BOTTLE_SELECT_MEASURE_MODULE bottle,FunctionalState enableInject,uint16_t* backGroundValue,uint32_t* lightValue)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MEASURE_MODULE_READ_LIGHT_COMPLEX,cmdCanConfigMeasureArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)bottle;
    canMasterSendCmd.paramBuffer[1] = (uint8_t)enableInject;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMeasureArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *backGroundValue = CanBaseReadDataConvertToUint16(canMasterReadData.recvDataPtr,2); 
    *lightValue = CanBaseReadDataConvertToUint32(canMasterReadData.recvDataPtr,4);   

    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//测量模块读光子变化率
LH_ERR CanSubBoard3MeasureModule_ReadLightChangeRate(BOTTLE_SELECT_MEASURE_MODULE bottle,uint8_t setChangeCount,uint8_t* resultPointCount,uint32_t** resultArrayPtr)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MEASURE_MODULE_READ_LIGHT_CHANGE_RATE,cmdCanConfigMeasureArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)(bottle);
    canMasterSendCmd.paramBuffer[1] = (uint8_t)(setChangeCount);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMeasureArray);
    if(errCode != LH_ERR_NONE)
    {
        return errCode;
    }
    //数据解析,此处需要每个人自己写
    *resultPointCount = canMasterReadData.recvDataPtr[2]; 
    *resultArrayPtr = NULL;
    //申请内存,记得调用者需要去释放内存
    if((*resultPointCount) != 0)
    {
        do
        {
            *resultArrayPtr = (uint32_t*)(UserMemMalloc(SRAM_IN,4*(*resultPointCount)));
            if((*resultArrayPtr) == NULL)
            {
                CoreDelayMinTick();
            }
        }while((*resultArrayPtr) == NULL);
        //数据转换
        uint8_t utilIndex = 0;
        for(utilIndex = 0; utilIndex < (*resultPointCount);utilIndex++)
        {
            (*resultArrayPtr)[utilIndex] = CanBaseReadDataConvertToInt32(canMasterReadData.recvDataPtr,3+(4*utilIndex));
        }
    }

    //释放接收内存
    if(canMasterReadData.recvDataPtr != NULL)
    {
        UserMemFree(SRAM_IN,canMasterReadData.recvDataPtr);
    }
    return errCode;
}

//指定步进电机复位
LH_ERR CanSubBoard3MeasureModule_StepMotorReset(INDEX_MOTOR_MEASURE_MODULE index)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MEASURE_MODULE_STEP_MOTOR_RESET,cmdCanConfigMeasureArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)index;

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMeasureArray);
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
LH_ERR CanSubBoard3MeasureModule_StepMotorRunSteps(INDEX_MOTOR_MEASURE_MODULE index,int32_t steps,int32_t* posAfterRun)
{
    CAN_MASTER_SEND_CMD canMasterSendCmd;
    CAN_MASTER_READ_DATA canMasterReadData;
    LH_ERR errCode = LH_ERR_NONE;
    //初始化发送接收的结构,但是数据没有填充,需要自己填充
    CanSubBoardBase_CreateSendBuffer(&canMasterSendCmd,&canMasterReadData,CMD_MEASURE_MODULE_STEP_MOTOR_RUN_STEPS,cmdCanConfigMeasureArray);
    //如果存在数据需要传送,在这里填充数据
    canMasterSendCmd.paramBuffer[0] = (uint8_t)index;
    CanBaseWriteInt32DataToBuffer(canMasterSendCmd.paramBuffer,1,steps);

    //发送数据并进行基本解析,如果数据有问题,那么会自动释放发送内存和接收内存,没问题,需要用户自己去解析收到数据,并进行内存释放
    errCode = CanSubBoardBase_SendAndProcRespBase(&canMasterSendCmd,&canMasterReadData,cmdCanConfigMeasureArray);
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
















