/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2019-12-31 13:58:34
**LastEditors: DengXiaoJun
**LastEditTime: 2020-08-18 14:52:41
**FilePath: \Application\TaskMain\ServiceSupport\ServiceTask\ServiceImpl\ServiceImplPlateReagent.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ServiceImplPlateReagent.h"
#include "ServiceStatePrivate.h"

//模块输入配置映射,前一个是系统序号,后一个是有效电平
static const MODULE_INPUT_MAP_UNIT modulePlateReagentInputMapArray[] = {
    {INPUT_PLATE_REAGENT_ROTATE_ORGIN_SENSOR        ,VALID_LEVEL_INPUT_PLATE_REAGENT_ROTATE_ORGIN_SENSOR    },//旋转电机原点
    {INPUT_PLATE_REAGENT_USER_KEY_SENSOR            ,VALID_LEVEL_INPUT_PLATE_REAGENT_USER_KEY_SENSOR        },//试剂盘按键
    {INPUT_PLATE_REAGENT_DOOR_SENSOR                ,VALID_LEVEL_INPUT_PLATE_REAGENT_DOOR_SENSOR            },//试剂盘门
};
//配置数组长度
#define MODULE_PLATE_REAGENT_INPUT_MAP_ARRAY_LENGTH   (sizeof(modulePlateReagentInputMapArray)/sizeof(modulePlateReagentInputMapArray[0]))
/*******************************************************************
**description: 读取指定输入 
******************************************************************/
static LH_ERR ServiceImplPlateReagent_ModuleInputRead(INPUT_PLATE_REAGENT_NAME name,SENSOR_STATE* state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //输入读取
        errorCode = ServiceImplModuleInputRead((uint8_t)name,MODULE_PLATE_REAGENT_INPUT_MAP_ARRAY_LENGTH,(MODULE_INPUT_MAP_UNIT*)modulePlateReagentInputMapArray,state);
        if(errorCode != LH_ERR_NONE)
        {
            if(errorCode == LH_ERR_BOARD_MAIN_COMM_INPUT_PIN_INDEX)
            {
                *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_INPUT_INDEX;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
            else
            {
                *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_INPUT_READ;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
        }
    return errorCode;
}

//模块输出配置映射
static const MODULE_OUTPUT_MAP_UNIT modulePlateReagentOutputMapArray[] = {
    {OUTPUT_PLATE_REAGENT_LED_YELLOW    ,VALID_LEVEL_OUTPUT_PLATE_REAGENT_LED_YELLOW},//试剂盘黄灯
    {OUTPUT_PLATE_REAGENT_LED_GREEN     ,VALID_LEVEL_OUTPUT_PLATE_REAGENT_LED_GREEN },//试剂盘绿灯
    {OUTPUT_PLATE_REAGENT_LED_KEY       ,VALID_LEVEL_OUTPUT_PLATE_REAGENT_LED_KEY   },//试剂盘按键灯
};
//配置数组长度
#define MODULE_PLATE_REAGENT_OUTPUT_MAP_UNIT_LENGTH   (sizeof(modulePlateReagentOutputMapArray)/sizeof(modulePlateReagentOutputMapArray[0]))
/*******************************************************************
**description: 写入指定输出 
******************************************************************/
static LH_ERR ServiceImplPlateReagent_ModuleOutputWrite(OUTPUT_PLATE_REAGENT_NAME name,OUT_STATE state,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //输出写入
        errorCode = ServiceImplModuleOutputWrite((uint8_t)name,state,MODULE_PLATE_REAGENT_OUTPUT_MAP_UNIT_LENGTH,(MODULE_OUTPUT_MAP_UNIT*)modulePlateReagentOutputMapArray);
        if(errorCode != LH_ERR_NONE)
        {
            if(errorCode == LH_ERR_BOARD_MAIN_COMM_OUTPUT_PIN_INDEX)
            {
                *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_OUTPUT_INDEX;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
            else
            {
                *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_OUTPUT_WRITE;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
            }
            return errorCode;
        }
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 模块运行前的检查,检查当前模块可不可以运动
**param: mode 检查模式
**return: 
**Author: DengXiaoJun
**LastEditors: DengXiaoJun
**LastEditTime: 
**LastModifyRecord: 
******************************************************************/
static LH_ERR ServiceImplPlateReagent_ModuleSafeCheckBeforeRun(uint8_t mode,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //检查试剂针状态
        if(NEEDLE_SAFE_EFFECTIVE_PLATE_REAGENT == ServiceStateReadNeedleReagentSafeState())
        {
            errorCode = LH_ERR_BOARD_MAIN_PLATE_REAGENT_SAFE_PROTECT;
            *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_MODULE_NEEDLE_REAGENT_SAFE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            return errorCode;
        }
    //检查磁珠针状态
        if(NEEDLE_SAFE_EFFECTIVE_PLATE_REAGENT == ServiceStateReadNeedleBeadSafeState())
        {
            errorCode = LH_ERR_BOARD_MAIN_PLATE_REAGENT_SAFE_PROTECT;
            *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_MODULE_NEEDLE_BEAD_SAFE;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            return errorCode;
        }
    return LH_ERR_NONE;
}


//试剂盘上一次按键状态
static SENSOR_STATE plateReagentKeySensorLastState;
//试剂盘本次按键状态
static SENSOR_STATE plateReagentKeySensorCurrentState;
/*******************************************************************
**description: 自动运行状态机初始化 
******************************************************************/
LH_ERR ServiceImplPlateReagent_StateMachineInit(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        CAN1_REPORT_ERR_CODE commErrorCode;
        CAN1_REPORT_ERR_LEVEL commErrorLevel;
    //延时初始化
        CoreDelayMs(500);
    //初始化状态空闲
        ServiceStateWritePlateReagentState(PLATE_REAGENT_STATE_IDLE);
    //试剂盘空闲,绿灯亮,按键灯亮,黄灯灭
        ServiceImplPlateReagent_ModuleOutputWrite(PLATE_REAGENT_LED_YELLOW,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
        ServiceImplPlateReagent_ModuleOutputWrite(PLATE_REAGENT_LED_GREEN,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
        ServiceImplPlateReagent_ModuleOutputWrite(PLATE_REAGENT_LED_KEY,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
    //初始化读取按键状态
        ServiceImplPlateReagent_ModuleInputRead(PLATE_REAGENT_USER_KEY,&plateReagentKeySensorCurrentState,&commErrorCode,&commErrorLevel);
        plateReagentKeySensorLastState = plateReagentKeySensorCurrentState;
    return errorCode;
}

/*******************************************************************
**description: 自动运行状态机轮转 
******************************************************************/
LH_ERR ServiceImplPlateReagent_StateMachineLoop(void)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        CAN1_REPORT_ERR_CODE commErrorCode;
        CAN1_REPORT_ERR_LEVEL commErrorLevel;
        SENSOR_STATE plateReagentKeySensorState;
    //试剂盘空闲状态
        if(PLATE_REAGENT_STATE_IDLE == ServiceStateReadPlateReagentState())
        {
            //试剂盘空闲,绿灯亮,按键灯亮,黄灯灭
                ServiceImplPlateReagent_ModuleOutputWrite(PLATE_REAGENT_LED_YELLOW,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
                ServiceImplPlateReagent_ModuleOutputWrite(PLATE_REAGENT_LED_GREEN,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
                ServiceImplPlateReagent_ModuleOutputWrite(PLATE_REAGENT_LED_KEY,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
            //读取状态
                ServiceImplPlateReagent_ModuleInputRead(PLATE_REAGENT_USER_KEY,&plateReagentKeySensorState,&commErrorCode,&commErrorLevel);
            //状态变化
                if(plateReagentKeySensorState != plateReagentKeySensorCurrentState)
                {
                    //按键状态改变
                        plateReagentKeySensorLastState = plateReagentKeySensorCurrentState;
                        plateReagentKeySensorCurrentState = plateReagentKeySensorState;
                    //触发有效,当前状态有效,前面状态无效
                        if((plateReagentKeySensorLastState == SENSOR_STATE_INVALID)&&(plateReagentKeySensorCurrentState == SENSOR_STATE_VALID))
                        {
                            //检查试剂盘是否可以运转
                            errorCode = ServiceImplPlateReagent_ModuleSafeCheckBeforeRun(0,&commErrorCode,&commErrorLevel);
                            if(errorCode != LH_ERR_NONE)
                            {
                                //错误代码打印
                                ServicePrintfShowError(errorCode);
                                return errorCode;
                            }
                            //运行到这,试剂盘可以运转
                            ServiceImplPlateReagent_ModuleOutputWrite(PLATE_REAGENT_LED_YELLOW,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
                            ServiceImplPlateReagent_ModuleOutputWrite(PLATE_REAGENT_LED_GREEN,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
                            ServiceImplPlateReagent_ModuleOutputWrite(PLATE_REAGENT_LED_KEY,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
                            //试剂盘走格数
                            uint8_t plateReagentIndex;
                            PLATE_REAGENT_OFFSET currentOffset;
                            ServiceImplPlateReagent_RunAnyCups(5,&plateReagentIndex,&currentOffset,&commErrorCode,&commErrorLevel);
                            //走完了,设置空闲
                            ServiceStateWritePlateReagentState(PLATE_REAGENT_STATE_IDLE);
                        }
                }
        }
    //试剂盘忙状态
        else
        {
            //试剂盘忙,黄灯亮,绿灯灭,按键灯灭
            ServiceImplPlateReagent_ModuleOutputWrite(PLATE_REAGENT_LED_YELLOW,OUT_STATE_ENABLE,&commErrorCode,&commErrorLevel);
            ServiceImplPlateReagent_ModuleOutputWrite(PLATE_REAGENT_LED_GREEN,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
            ServiceImplPlateReagent_ModuleOutputWrite(PLATE_REAGENT_LED_KEY,OUT_STATE_DISABLE,&commErrorCode,&commErrorLevel);
        }
    return errorCode;
}

/*******************************************************************
**description: 试剂盘复位 
******************************************************************/
LH_ERR ServiceImplPlateReagent_ResetAll(uint8_t* plateReagentIndex,PLATE_REAGENT_OFFSET* currentOffset,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
        *commErrorCode = CAN1_REPORT_ERR_OK;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //安全状态判定
        errorCode = ServiceImplPlateReagent_ModuleSafeCheckBeforeRun(0,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //写入试剂盘保护
        ServiceStateWritePlateReagentSafeState(PLATE_SAFE_EFFECTIVE);
    //复位
        errorCode = Can2SubBoard2PlateReagent_Reset(plateReagentIndex,currentOffset);
        if(errorCode != LH_ERR_NONE)
        {
            *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_RESET;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
        //检测试剂盘条码枪,重试三次
        uint8_t retryCount = 0;
        if(ServiceStateReadPlateReagentBarCodeScanVersion() == PLATE_REAGENT_BARCODE_VERSION0)
        {
            
            do
            {
                errorCode = BoardQRScanCM200CheckExist();
                if(errorCode == LH_ERR_NONE)
                {
                    break;
                }
                retryCount++;
            }while(retryCount < 3);
            if(errorCode != LH_ERR_NONE)
            {
                *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_BARSCAN_EXIST;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                //错误代码打印
                ServicePrintfShowError(errorCode);
                return errorCode;
            }
        }
        else if(ServiceStateReadPlateReagentBarCodeScanVersion() == PLATE_REAGENT_BARCODE_VERSION1)
        {
            RFID_SOFT_STATE RFIDSoftState;
            do
            {
                errorCode = QRScanRFIDReadSoftState(&RFIDSoftState);
                if(errorCode == LH_ERR_NONE && RFIDSoftState == RFID_SOFT_STATE_APP)
                {
                    break;
                }
                retryCount++;
            }while(retryCount < 3);
            if(errorCode != LH_ERR_NONE)
            {
                *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_RFID_EXIST;
                *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
                //错误代码打印
                ServicePrintfShowError(errorCode);
                return errorCode;
            }
        }
    
    //设置状态
        ServiceStateWritePlateReagentIndex(*plateReagentIndex);
        ServiceStateWritePlateReagentOffset(*currentOffset);
    //清除试剂盘探液高度
        ServiceStateClearPlateReagentLiquidDetectPos();
    //设置试剂盘状态空闲,可以响应按键消息
        ServiceStateWritePlateReagentState(PLATE_REAGENT_STATE_IDLE);
    //关闭试剂盘保护
        ServiceStateWritePlateReagentSafeState(PLATE_SAFE_INVALID);
    //读取试剂盘门状态
        SENSOR_STATE doorState;
        errorCode = ServiceImplPlateReagent_ModuleInputRead(PLATE_REAGENT_DOOR,&doorState,commErrorCode,commErrorLevel);
        if(errorCode != LH_ERR_NONE)
        {
            //错误代码打印
            ServicePrintfShowError(errorCode);
            return errorCode;
        }
    //如果门没关,上传一个报警
        if(doorState != SENSOR_STATE_VALID)
        {
            *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_DOOR_OPEN;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
        }
    return errorCode;
}

/*******************************************************************
**description: 试剂盘运行指定个杯位 
******************************************************************/
LH_ERR ServiceImplPlateReagent_RunAnyCups(uint8_t cupsCount,uint8_t* plateReagentIndex,PLATE_REAGENT_OFFSET* currentOffset,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //安全状态判定
    errorCode = ServiceImplPlateReagent_ModuleSafeCheckBeforeRun(0,commErrorCode,commErrorLevel);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //写入试剂盘保护
    ServiceStateWritePlateReagentSafeState(PLATE_SAFE_EFFECTIVE);
    //走到指定杯位
    errorCode = Can2SubBoard2PlateReagent_RunAnyCups(cupsCount,plateReagentIndex,currentOffset);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_RUN_ANY_CUP;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //设置状态
    ServiceStateWritePlateReagentIndex(*plateReagentIndex);
    ServiceStateWritePlateReagentOffset(*currentOffset);
    //设置试剂盘状态忙
    ServiceStateWritePlateReagentState(PLATE_REAGENT_STATE_BUSY);
    //关闭试剂盘保护
    ServiceStateWritePlateReagentSafeState(PLATE_SAFE_INVALID);
    //读取试剂盘门状态
    SENSOR_STATE doorState;
    errorCode = ServiceImplPlateReagent_ModuleInputRead(PLATE_REAGENT_DOOR,&doorState,commErrorCode,commErrorLevel);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //如果门没关,上传一个报警
    if(doorState != SENSOR_STATE_VALID)
    {
        *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_DOOR_OPEN;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    return errorCode;
}

/*******************************************************************
**description: 试剂盘运行到指定杯位带偏移 扫码枪版本
******************************************************************/
LH_ERR ServiceImplPlateReagent_Run2IndexWithOffsetVersion0(uint8_t targetIndex,PLATE_REAGENT_OFFSET offset,uint8_t* plateReagentIndex,PLATE_REAGENT_OFFSET* currentOffset,
                                                            CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //安全状态判定
    errorCode = ServiceImplPlateReagent_ModuleSafeCheckBeforeRun(0,commErrorCode,commErrorLevel);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //写入试剂盘保护
    ServiceStateWritePlateReagentSafeState(PLATE_SAFE_EFFECTIVE);
    //走到指定杯位
    errorCode = Can2SubBoard2PlateReagent_Run2CupWithOffset(targetIndex,offset,plateReagentIndex,currentOffset);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_RUN_TO_INDEX;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //设置状态
    ServiceStateWritePlateReagentIndex(*plateReagentIndex);
    ServiceStateWritePlateReagentOffset(*currentOffset);
    //设置试剂盘状态忙,不再响应按键动作
    ServiceStateWritePlateReagentState(PLATE_REAGENT_STATE_BUSY);
    //关闭试剂盘保护
    ServiceStateWritePlateReagentSafeState(PLATE_SAFE_INVALID);
    //读取试剂盘门状态
    SENSOR_STATE doorState;
    errorCode = ServiceImplPlateReagent_ModuleInputRead(PLATE_REAGENT_DOOR,&doorState,commErrorCode,commErrorLevel);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //如果门没关,上传一个报警
    if(doorState != SENSOR_STATE_VALID)
    {
        *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_DOOR_OPEN;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    return errorCode;
}

/*******************************************************************
 * **description: 试剂盘运行到指定杯位带偏移 RFID版本
******************************************************************/
LH_ERR ServiceImplPlateReagent_Run2IndexWithOffsetVersion1(uint8_t targetIndex,PLATE_REAGENT_OFFSET offset,uint8_t* plateReagentIndex,PLATE_REAGENT_OFFSET* currentOffset,
                                                            CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //安全状态判定
    errorCode = ServiceImplPlateReagent_ModuleSafeCheckBeforeRun(0,commErrorCode,commErrorLevel);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //写入试剂盘保护
    ServiceStateWritePlateReagentSafeState(PLATE_SAFE_EFFECTIVE);
    //扫码偏移改为RFID偏移,
    if(offset == PLATE_REAGENT_OFFSET_QR_SCAN)
    {
        offset = PLATE_REAGENT_OFFSET_QR_RFID;
    }
    //走到指定杯位
    errorCode = Can2SubBoard2PlateReagent_Run2CupWithOffset(targetIndex,offset,plateReagentIndex,currentOffset);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_RUN_TO_INDEX;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //返回当前位置为RFID时,仍按照扫码位进行上传
    if(*currentOffset == PLATE_REAGENT_OFFSET_QR_RFID)
    {
        *currentOffset = PLATE_REAGENT_OFFSET_QR_SCAN;
    }
    //设置状态
    ServiceStateWritePlateReagentIndex(*plateReagentIndex);
    ServiceStateWritePlateReagentOffset(*currentOffset);
    //设置试剂盘状态忙,不再响应按键动作
    ServiceStateWritePlateReagentState(PLATE_REAGENT_STATE_BUSY);
    //关闭试剂盘保护
    ServiceStateWritePlateReagentSafeState(PLATE_SAFE_INVALID);
    //读取试剂盘门状态
    SENSOR_STATE doorState;
    errorCode = ServiceImplPlateReagent_ModuleInputRead(PLATE_REAGENT_DOOR,&doorState,commErrorCode,commErrorLevel);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //如果门没关,上传一个报警
    if(doorState != SENSOR_STATE_VALID)
    {
        *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_DOOR_OPEN;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    return errorCode;
}

/*******************************************************************
**description: 试剂盘运行到指定杯位带偏移 
******************************************************************/
LH_ERR ServiceImplPlateReagent_Run2IndexWithOffset(uint8_t targetIndex,PLATE_REAGENT_OFFSET offset,uint8_t* plateReagentIndex,PLATE_REAGENT_OFFSET* currentOffset,
                                                            CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //条码枪
    if(ServiceStateReadPlateReagentBarCodeScanVersion() == PLATE_REAGENT_BARCODE_VERSION0)
    {
        errorCode = ServiceImplPlateReagent_Run2IndexWithOffsetVersion0(targetIndex,offset,plateReagentIndex,currentOffset,commErrorCode,commErrorLevel);
    }
    //RFID
    else if(ServiceStateReadPlateReagentBarCodeScanVersion() == PLATE_REAGENT_BARCODE_VERSION1)
    {
        errorCode = ServiceImplPlateReagent_Run2IndexWithOffsetVersion1(targetIndex,offset,plateReagentIndex,currentOffset,commErrorCode,commErrorLevel);
    }
    return errorCode;
}

/*******************************************************************
**description: 试剂盘运行到指定杯位的扫码位并扫码-条码枪
******************************************************************/
LH_ERR ServiceImplPlateReagent_Run2IndexScanAndQRScanVersion0(uint8_t targetIndex,uint8_t* plateReagentIndex,PLATE_REAGENT_OFFSET* currentOffset,uint8_t** barCodeBufferPtr,
                                                            uint16_t* barCodeLength,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    OS_ERR err;
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //安全状态判定
    errorCode = ServiceImplPlateReagent_ModuleSafeCheckBeforeRun(0,commErrorCode,commErrorLevel);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //写入试剂盘保护
    ServiceStateWritePlateReagentSafeState(PLATE_SAFE_EFFECTIVE);
    /* 运行到扫码起始位 */
    errorCode = Can2SubBoard2PlateReagent_Run2CupWithOffsetForScan(targetIndex,PLATE_REAGENT_SCAN_OFFSET_START,plateReagentIndex,currentOffset);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_RUN_TO_INDEX;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //设置试剂盘状态忙,不再响应按键动作
    ServiceStateWritePlateReagentState(PLATE_REAGENT_STATE_BUSY);
    //打开试剂盘保护
    ServiceStateWritePlateReagentSafeState(PLATE_SAFE_INVALID);
    //打开条码枪
    BoardQRScanCM200Open();
    //计时辅助
    OS_TICK tickCheckUtilStart,tickCheckUtilCurrent;
    //计时
    tickCheckUtilStart = OSTimeGet(&err);
    /* 运行到扫码结束位 */
    errorCode = Can2SubBoard2PlateReagent_Run2CupWithOffsetForScan(targetIndex,PLATE_REAGENT_SCAN_OFFSET_END,plateReagentIndex,currentOffset);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_RUN_TO_INDEX;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //设置状态
    ServiceStateWritePlateReagentIndex(*plateReagentIndex);
    ServiceStateWritePlateReagentOffset(*currentOffset);
    //循环检测条码枪扫描结果
    do
    {
        CoreDelayMs(5);
        tickCheckUtilCurrent = OSTimeGet(&err);
        //检查有没有收到完整
        if(0 != BoardQRScanCM200CheckRecvCompletePacket())
        {
            break;
        }
    }while(tickCheckUtilCurrent - tickCheckUtilStart < 4000);
    //检查有没有收到完整数据
    if(0 == BoardQRScanCM200CheckRecvCompletePacket())
    {
        *barCodeBufferPtr = NULL;
        *barCodeLength = 0;
    }
    else
    {
        *barCodeBufferPtr = NULL;
        //申请内存
        *barCodeLength = BoardQRScanCM200CheckRecvCompletePacket();
        do
        {
            *barCodeBufferPtr = UserMemMalloc(MEM_AXI_SRAM,*barCodeLength);
            if((*barCodeBufferPtr) == NULL)
            {
                CoreDelayMinTick();
            }
        }while((*barCodeBufferPtr) == NULL);
        //读取数据
        *barCodeLength = BoardQRScanCM200ReadLastRecvData(*barCodeBufferPtr);
    }
    //关闭条码枪
    BoardQRScanCM200Close();
    //将结果写入缓存
    ServiceStateWritePlateReagentBarcodeScanData(*barCodeBufferPtr,*barCodeLength);
    //读取试剂盘门状态
    SENSOR_STATE doorState;
    errorCode = ServiceImplPlateReagent_ModuleInputRead(PLATE_REAGENT_DOOR,&doorState,commErrorCode,commErrorLevel);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //如果门没关,上传一个报警
    if(doorState != SENSOR_STATE_VALID)
    {
        *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_DOOR_OPEN;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    return LH_ERR_NONE;
}
/*******************************************************************
**description: 试剂盘运行到指定杯位的扫码位并扫码-RFID 
******************************************************************/
static LH_ERR ServiceImplPlateReagent_Run2IndexScanAndQRScanVersion1(uint8_t targetIndex,uint8_t* plateReagentIndex,PLATE_REAGENT_OFFSET* currentOffset,uint8_t** barCodeBufferPtr,
                                                            uint16_t* barCodeLength,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //安全状态判定
    errorCode = ServiceImplPlateReagent_ModuleSafeCheckBeforeRun(0,commErrorCode,commErrorLevel);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //写入试剂盘保护
    ServiceStateWritePlateReagentSafeState(PLATE_SAFE_EFFECTIVE);
    /* 运行到扫码起始位 */
    errorCode = Can2SubBoard2PlateReagent_Run2CupWithOffsetForRFID(targetIndex,PLATE_REAGENT_SCAN_OFFSET_NONE,plateReagentIndex,currentOffset);
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_RUN_TO_INDEX;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_EMERGENCY_STOP;
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //设置试剂盘状态忙,不再响应按键动作
    ServiceStateWritePlateReagentState(PLATE_REAGENT_STATE_BUSY);
    //打开试剂盘保护
    ServiceStateWritePlateReagentSafeState(PLATE_SAFE_INVALID);
    //发送RFID扫码开始
    QRScanRFIDReadTargetReagentInfo();
    //设置状态
    ServiceStateWritePlateReagentIndex(*plateReagentIndex);
    //返回当前位置为RFID时,仍按照扫码位进行上传
    if(*currentOffset == PLATE_REAGENT_OFFSET_QR_RFID)
    {
        *currentOffset = PLATE_REAGENT_OFFSET_QR_SCAN;
    }
    ServiceStateWritePlateReagentOffset(*currentOffset);
    
    //申请内存
    do
    {
        *barCodeBufferPtr = UserMemMalloc(MEM_AXI_SRAM,RFID_DATA_CACHE_MAX_LENGHT);
        if((*barCodeBufferPtr) == NULL)
        {
            CoreDelayMinTick();
        }
    }while((*barCodeBufferPtr) == NULL);
    //读取数据
    errorCode = QRScanRFIDGetBarCode(*barCodeBufferPtr,barCodeLength);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        //读卡错误不返回
        if((errorCode >= LH_ERR_BOARD_MAIN_RFID_RC522_NO_TARGET) && (errorCode <= LH_ERR_BOARD_MAIN_RFID_RC522_WAIT_RESPONSE_TIMEOUT))
        {
            //不作为错误上传
            errorCode = LH_ERR_NONE;
        }
        else
        {
            *commErrorCode = CAN1_REPORT_ERR_PALTE_REAGENT_RFID_READ_CARED_FAILED;
            *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
            return errorCode;
        }
    }
    //将结果写入缓存
    ServiceStateWritePlateReagentBarcodeScanData(*barCodeBufferPtr,*barCodeLength);
    //读取试剂盘门状态
    SENSOR_STATE doorState;
    errorCode = ServiceImplPlateReagent_ModuleInputRead(PLATE_REAGENT_DOOR,&doorState,commErrorCode,commErrorLevel);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //如果门没关,上传一个报警
    if(doorState != SENSOR_STATE_VALID)
    {
        *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_DOOR_OPEN;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_WARNING;
    }
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 试剂盘运行到指定杯位的扫码位并扫码 
******************************************************************/
LH_ERR ServiceImplPlateReagent_Run2IndexScanAndQRScan(uint8_t targetIndex,uint8_t* plateReagentIndex,PLATE_REAGENT_OFFSET* currentOffset,uint8_t** barCodeBufferPtr,
                                                            uint16_t* barCodeLength,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //条码枪
    if(ServiceStateReadPlateReagentBarCodeScanVersion() == PLATE_REAGENT_BARCODE_VERSION0)
    {
        errorCode = ServiceImplPlateReagent_Run2IndexScanAndQRScanVersion0(targetIndex,plateReagentIndex,currentOffset,barCodeBufferPtr,barCodeLength,
                                                                            commErrorCode,commErrorLevel);
    }
    //RFID
    else if(ServiceStateReadPlateReagentBarCodeScanVersion() == PLATE_REAGENT_BARCODE_VERSION1)
    {
        errorCode = ServiceImplPlateReagent_Run2IndexScanAndQRScanVersion1(targetIndex,plateReagentIndex,currentOffset,barCodeBufferPtr,barCodeLength,
                                                                            commErrorCode,commErrorLevel);
    }
    return errorCode;
}

/*******************************************************************
**description: 试剂盘扫码一次-条码枪 
******************************************************************/
static LH_ERR ServiceImplPlateReagent_BarScanOnceVersion0(uint8_t** barCodeBufferPtr,uint16_t* barCodeLength,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    OS_ERR err;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //打开条码枪
    BoardQRScanCM200Open();
    //循环检测条码枪扫描结果
    //计时辅助
    OS_TICK tickCheckUtilStart,tickCheckUtilCurrent;
    //计时
    tickCheckUtilStart = OSTimeGet(&err);
    do
    {
        CoreDelayMs(5);
        tickCheckUtilCurrent = OSTimeGet(&err);
        //检查有没有收到完整
        if(0 != BoardQRScanCM200CheckRecvCompletePacket())
        {
            break;
        }
    }while(tickCheckUtilCurrent - tickCheckUtilStart < 4000);
    //检查有没有收到完整数据
    if(0 == BoardQRScanCM200CheckRecvCompletePacket())
    {
        *barCodeBufferPtr = NULL;
        *barCodeLength = 0;
    }
    else
    {
        *barCodeBufferPtr = NULL;
        //申请内存
        *barCodeLength = BoardQRScanCM200CheckRecvCompletePacket();
        do
        {
            *barCodeBufferPtr = UserMemMalloc(MEM_AXI_SRAM,*barCodeLength);
            if((*barCodeBufferPtr) == NULL)
            {
                CoreDelayMinTick();
            }
        }while((*barCodeBufferPtr) == NULL);
        //读取数据
        *barCodeLength = BoardQRScanCM200ReadLastRecvData(*barCodeBufferPtr);
    }
    //关闭条码枪
    BoardQRScanCM200Close();
    //将结果写入缓存
    ServiceStateWritePlateReagentBarcodeScanData(*barCodeBufferPtr,*barCodeLength);
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 试剂盘扫码一次-RFID
******************************************************************/
LH_ERR ServiceImplPlateReagent_BarScanOnceVersion1(uint8_t** barCodeBufferPtr,uint16_t* barCodeLength,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;

    *barCodeBufferPtr = NULL;
    //申请内存
    do
    {
        *barCodeBufferPtr = UserMemMalloc(MEM_AXI_SRAM,RFID_DATA_CACHE_MAX_LENGHT);
        if((*barCodeBufferPtr) == NULL)
        {
            CoreDelayMinTick();
        }
    }while((*barCodeBufferPtr) == NULL);
    errorCode = QRScanRFIDReadTargetReagentInfo();
    if(errorCode != LH_ERR_NONE)
    {
        return  errorCode;
    }
    errorCode = QRScanRFIDGetBarCode(*barCodeBufferPtr,barCodeLength);
    //读卡错误不返回
    if((errorCode >= LH_ERR_BOARD_MAIN_RFID_RC522_NO_TARGET) && (errorCode <= LH_ERR_BOARD_MAIN_RFID_RC522_WAIT_RESPONSE_TIMEOUT))
    {
        //不作为错误上传
        errorCode = LH_ERR_NONE;
    }
    //结果写入
    if(errorCode == LH_ERR_NONE)
    {
        //将结果写入缓存
        ServiceStateWritePlateReagentBarcodeScanData(*barCodeBufferPtr,*barCodeLength); 
    }
    return errorCode;
}


/*******************************************************************
**description: 试剂盘扫码一次 
******************************************************************/
LH_ERR ServiceImplPlateReagent_BarScanOnce(uint8_t** barCodeBufferPtr,uint16_t* barCodeLength,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //条码枪
    if(ServiceStateReadPlateReagentBarCodeScanVersion() == PLATE_REAGENT_BARCODE_VERSION0)
    {
        errorCode = ServiceImplPlateReagent_BarScanOnceVersion0(barCodeBufferPtr,barCodeLength,commErrorCode,commErrorLevel);
    }
    //RFID
    else if(ServiceStateReadPlateReagentBarCodeScanVersion() == PLATE_REAGENT_BARCODE_VERSION1)
    {
        errorCode = ServiceImplPlateReagent_BarScanOnceVersion1(barCodeBufferPtr,barCodeLength,commErrorCode,commErrorLevel);
    }
    return errorCode;
}



/*******************************************************************
**description: 试剂盘清除探液数据 
******************************************************************/
LH_ERR ServiceImplPlateReagent_ClearLiquidDetectData(uint8_t targetIndex,uint8_t clearConfig,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //子参数错误
    if(clearConfig > 5)
    {
        *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_LIQUID_SUB_CONFIG;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        errorCode = LH_ERR_BOARD_MAIN_PLATE_REAGENT_LIQUID_PARAM;
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //主序号错误
    if((targetIndex == 0)||(targetIndex > 30))
    {
        *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_LIQUID_MAIN_INDEX;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        errorCode = LH_ERR_BOARD_MAIN_PLATE_REAGENT_LIQUID_PARAM;
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    if(clearConfig == 0)
    {
        //全部清除
        ServiceStateWritePlateReagentLiquidDetectPos(targetIndex,LIQUID_DETECT_OFFSET_NEEDLE_REAGENT_R1,0);
        ServiceStateWritePlateReagentLiquidDetectPos(targetIndex,LIQUID_DETECT_OFFSET_NEEDLE_REAGENT_R2,0);
        ServiceStateWritePlateReagentLiquidDetectPos(targetIndex,LIQUID_DETECT_OFFSET_NEEDLE_BEAD_R1,0);
        ServiceStateWritePlateReagentLiquidDetectPos(targetIndex,LIQUID_DETECT_OFFSET_NEEDLE_BEAD_R2,0);
        ServiceStateWritePlateReagentLiquidDetectPos(targetIndex,LIQUID_DETECT_OFFSET_NEEDLE_BEAD_BEAD,0);
    }
    else if(clearConfig == 1)
    {
        //试剂针R1
        ServiceStateWritePlateReagentLiquidDetectPos(targetIndex,LIQUID_DETECT_OFFSET_NEEDLE_REAGENT_R1,0);
    }
    else if(clearConfig == 2)
    {
        //试剂针R2
        ServiceStateWritePlateReagentLiquidDetectPos(targetIndex,LIQUID_DETECT_OFFSET_NEEDLE_REAGENT_R2,0);
    }
    else if(clearConfig == 3)
    {
        //磁珠针R1
        ServiceStateWritePlateReagentLiquidDetectPos(targetIndex,LIQUID_DETECT_OFFSET_NEEDLE_BEAD_R1,0);
    }
    else if(clearConfig == 4)
    {
        //磁珠针R2
        ServiceStateWritePlateReagentLiquidDetectPos(targetIndex,LIQUID_DETECT_OFFSET_NEEDLE_BEAD_R2,0);
    }
    else if(clearConfig == 5)
    {
        //磁珠针BEAD
        ServiceStateWritePlateReagentLiquidDetectPos(targetIndex,LIQUID_DETECT_OFFSET_NEEDLE_BEAD_BEAD,0);
    }
    return LH_ERR_NONE;
}

/*******************************************************************
**description: 设置盘设置状态空闲 
******************************************************************/
LH_ERR ServiceImplPlateReagent_SetIdle(CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //设置试剂盘状态空闲,可以响应按键动作
    ServiceStateWritePlateReagentState(PLATE_REAGENT_STATE_IDLE);
    return errorCode;
}

/*******************************************************************
**description: 读取模块内部指定输入的状态 
******************************************************************/
LH_ERR ServiceImplPlateReagent_InputRead(uint8_t inputIndex,SENSOR_STATE* inputState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = ServiceImplPlateReagent_ModuleInputRead((INPUT_PLATE_REAGENT_NAME)inputIndex,inputState,commErrorCode,commErrorLevel);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
    }
    return errorCode;
}

/*******************************************************************
**description: 写入模块指定输出的状态 
******************************************************************/
LH_ERR ServiceImplPlateReagent_OutputWrite(uint8_t outputIndex,OUT_STATE setState,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    errorCode = ServiceImplPlateReagent_ModuleOutputWrite((OUTPUT_PLATE_REAGENT_NAME)outputIndex,setState,commErrorCode,commErrorLevel);
    if(errorCode != LH_ERR_NONE)
    {
        //错误代码打印
        ServicePrintfShowError(errorCode);
    }
    return errorCode;
}

/*******************************************************************
**description: 指定模块特定电机复位 
******************************************************************/
LH_ERR ServiceImplPlateReagent_MotorReset(uint8_t motorIndex,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //判定参数,主要是电机序号
    if(motorIndex > INDEX_MOTOR_PLATE_REAGENT_ROTATE)
    {
        errorCode = LH_ERR_BOARD_MAIN_PLATE_REAGENT_MOTOR_INDEX;
        *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_STEP_MOTOR_INDEX;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //指定步进电机复位带复位修正
    INDEX_MOTOR_PLATE_REAGENT stepMotorIndex = (INDEX_MOTOR_PLATE_REAGENT)(motorIndex);
    errorCode = Can2SubBoard2PlateReagent_StepMotorReset(stepMotorIndex);
    //读取失败,错误处理
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_STEP_MOTOR_SINGLE_RESET;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //返回数据
    return errorCode;
}

/*******************************************************************
**description: 指定模块特动电机运行指定步数 
******************************************************************/
LH_ERR ServiceImplPlateReagent_MotorRunSteps(uint8_t motorIndex,int32_t steps,int32_t* posAfterRun,CAN1_REPORT_ERR_CODE* commErrorCode,CAN1_REPORT_ERR_LEVEL* commErrorLevel)
{
    LH_ERR errorCode = LH_ERR_NONE;
    //初始化无错误
    *commErrorCode = CAN1_REPORT_ERR_OK;
    *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL;
    //判定参数,主要是电机序号
    if(motorIndex > INDEX_MOTOR_PLATE_REAGENT_ROTATE)
    {
        errorCode = LH_ERR_BOARD_MAIN_PLATE_REAGENT_MOTOR_INDEX;
        *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_STEP_MOTOR_INDEX;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //试剂盘电机走步数
    INDEX_MOTOR_PLATE_REAGENT stepMotorIndex = (INDEX_MOTOR_PLATE_REAGENT)(motorIndex);
    errorCode = Can2SubBoard2PlateReagent_StepMotorRunSteps(stepMotorIndex,steps,posAfterRun);
    //读取失败,错误处理
    if(errorCode != LH_ERR_NONE)
    {
        *commErrorCode = CAN1_REPORT_ERR_PLATE_REAGENT_STEP_MOTOR_SINGLE_RUN;
        *commErrorLevel = CAN1_REPORT_ERR_LEVEL_NORMAL_STOP;
        //错误代码打印
        ServicePrintfShowError(errorCode);
        return errorCode;
    }
    //返回数据
    return errorCode;
}




























