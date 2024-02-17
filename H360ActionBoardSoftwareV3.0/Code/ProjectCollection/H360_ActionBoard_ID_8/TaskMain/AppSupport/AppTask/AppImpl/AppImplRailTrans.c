/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-01-14 13:26:21
**LastEditors: DengXiaoJun
**LastEditTime: 2020-05-02 16:17:00
**FilePath: \H360_ActionBoard_ID_8_Rail\TaskMain\AppSupport\AppTask\AppImpl\AppImplRailTrans.c
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "AppImplRailTrans.h"

//转换轨道传送带错误代码的标识
typedef enum FLAG_RAIL_TRANS_ERR_CONVERT
{
    FLAG_RAIL_TRANS_ERR_RESET                               = 0X01,//轨道传送带复位失败
    FLAG_RAIL_TRANS_ERR_RUNNING                             = 0X02,//轨道传送带运行失败
    FLAG_RAIL_TRANS_ERR_RETURN_ZERO                         = 0X03,//轨道传送带回零失败
    FLAG_RAIL_TRANS_ERR_MAIN_MODULE_NO                      = 0X04,//轨道参数主模块号错误                     
    FLAG_RAIL_TRANS_ERR_SUB_WAIT_TEST_NO                    = 0X05,//轨道参数待测区模块号错误
    FLAG_RAIL_TRANS_ERR_SUB_REPEAT_TEST_NO                  = 0X06,//轨道参数重测区模块号错误
    FLAG_RAIL_TRANS_ERR_SUB_TESTING_PUT_TUBE_INDEX          = 0X07,//轨道参数测试区放架位试管号错误
    FLAG_RAIL_TRANS_ERR_SUB_TESTING_CATCH_TUBE_INDEX        = 0X08,//轨道参数测试区取架位试管号错误
    FLAG_RAIL_TRANS_ERR_SUB_TESTING_ABSORB_TUBE_INDEX       = 0X09,//轨道参数测试区吸样位试管号错误
    FLAG_RAIL_TRANS_ERR_SUB_MODULE_LIMIT                    = 0X0A,//目标模块号大于设定的模块号
}FLAG_RAIL_TRANS_ERR_CONVERT;

//可能涉及到错误代码的转换预定义的函数
static LH_ERR AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_CONVERT flag,LH_ERR errSrc)
{
    LH_ERR errResult = LH_ERR_NONE;
    //模块号
    uint32_t moduleNo = MODULE_NO_RAIL_TRANS;
    moduleNo <<= 24;
    //错误标志
    uint32_t flagInsert = flag;
    flagInsert <<= 16;
    //基础错误代码
    errResult = LH_ERR_BOARD8_RAIL_BASE;
    //加入模块号
    errResult |= moduleNo;
    //加入错误标志
    errResult |= flagInsert;
    //加入原始错误代码
    errResult |= errSrc;
    //返回转换完成的错误代码
    return errResult;
}

//检查模块号
static LH_ERR AppImplRailTransCheckTargetRailModule(RAIL_MODULE_POS moduleNo)
{
    if(moduleNo > railModuleCount)
    {
        return AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_SUB_MODULE_LIMIT,LH_ERR_NONE);
    }
    return LH_ERR_NONE;
}


//以下两个模块组合,就能知道当前轨道的确确位置
//当前轨道传输的主模块号
RAIL_MODULE_POS currentMainRailModule = RAIL_MODULE_POS_COMMON;
//当前轨道传输的子模块号
RAIL_SUB_MODULE currentSubRailRegion = RAIL_SUB_MODULE_COMMON_UNKNOW;

//通过轨道运转的长度决定轨道运转的速度
static SM_CURVE_SELECT AppImplRailTransCalcBestSpeed(int32_t targetCoordinate)
{
    LH_ERR errorCode = LH_ERR_NONE;
    int32_t currentCoordinate = 0;
    //获取当前坐标
    errorCode = IPC_StepMotor9ReflushStateWhileReturn(&currentCoordinate);
    if(errorCode != LH_ERR_NONE)
    {
        return CURVE_RAIL_TRANS_NORMAL_SLOW;
    }
    //当前位置
    uint32_t offset = 0;
    if(targetCoordinate >= currentCoordinate)
    {
        offset = targetCoordinate - currentCoordinate;
    }
    else
    {
        offset = currentCoordinate - targetCoordinate;
    }
    //选择速度曲线
    if(offset < 1000)
    {
        return CURVE_RAIL_TRANS_DEBUG;
    }
    else if(offset < 2500)
    {
        return CURVE_RAIL_TRANS_NORMAL_SLOW;
    }
    else if(offset < 5000)
    {
        return CURVE_RAIL_TRANS_NORMAL_MEDIUM;
    }
    else
    {
        return CURVE_RAIL_TRANS_NORMAL_FAST;
    }
}

//轨道复位,带零位偏移
LH_ERR AppImplRailTransReset(RAIL_RESET_CORRECT_FLAG resetCorrectFlag)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        int32_t paramUtil = 0;
        //复位结构体
        StepMotorResetCmd resetCmd;
    //首先复位
        if(resetCorrectFlag == RAIL_RESET_CORRECT_OFF)
        {
            paramUtil = 0;
        }
        else
        {
            AppParamReadParam(APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT,APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_RESET_CORRECTION,&paramUtil);
        }
    //开始复位
        IPC_StepMotorUtilResetCmdDataStructInit(&resetCmd);
        resetCmd.motorIndex = RAIL_TRANS_STEP_MOTOR_ID;//电机序号
        resetCmd.correctionCoordinate = paramUtil;//复位修正
        resetCmd.timeOutSet = 55000;//超时时间
        errorCode = IPC_StepMotorResetWhileReturn(&resetCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_RESET,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前位置处于复位位置
        currentMainRailModule = RAIL_MODULE_POS_COMMON;
        currentSubRailRegion = RAIL_SUB_MODULE_COMMON_RESET;
    return LH_ERR_NONE;
}

//轨道移动到进样1
LH_ERR AppImplRailTransMove2PushIn1(void)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        int32_t paramUtil = 0;
        //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //获取坐标
        AppParamReadParam(APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT,APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_PUSH_IN1_POS,&paramUtil);
        SM_CURVE_SELECT curveSelect = SM_CURVE_0;
        //计算速度
        curveSelect = AppImplRailTransCalcBestSpeed(paramUtil);
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = RAIL_TRANS_STEP_MOTOR_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_RUNNING,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前位置处于进样1位置
        currentMainRailModule = RAIL_MODULE_POS_COMMON;
        currentSubRailRegion = RAIL_SUB_MODULE_COMMON_PUSH_IN1;
    return LH_ERR_NONE;
}

//轨道移动到进样2
LH_ERR AppImplRailTransMove2PushIn2(void)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        int32_t paramUtil = 0;
        //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //获取坐标
        AppParamReadParam(APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT,APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_PUSH_IN2_POS,&paramUtil);
        SM_CURVE_SELECT curveSelect = SM_CURVE_0;
        //计算速度
        curveSelect = AppImplRailTransCalcBestSpeed(paramUtil);
        //运行坐标
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = RAIL_TRANS_STEP_MOTOR_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_RUNNING,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前位置处于进样2位置
        currentMainRailModule = RAIL_MODULE_POS_COMMON;
        currentSubRailRegion = RAIL_SUB_MODULE_COMMON_PUSH_IN2;
    return LH_ERR_NONE;
}

//轨道移动到出样1
LH_ERR AppImplRailTransMove2PushOut1(void)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        int32_t paramUtil = 0;
        //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //获取坐标
        AppParamReadParam(APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT,APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_PUSH_OUT1_POS,&paramUtil);
        SM_CURVE_SELECT curveSelect = SM_CURVE_0;
        //计算速度
        curveSelect = AppImplRailTransCalcBestSpeed(paramUtil);
        //运行坐标
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = RAIL_TRANS_STEP_MOTOR_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_RUNNING,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前位置处于出样1位置
        currentMainRailModule = RAIL_MODULE_POS_COMMON;
        currentSubRailRegion = RAIL_SUB_MODULE_COMMON_PUSH_OUT1;
    return LH_ERR_NONE;
}

//轨道移动到出样2
LH_ERR AppImplRailTransMove2PushOut2(void)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        int32_t paramUtil = 0;
        //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //获取坐标
        AppParamReadParam(APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT,APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_PUSH_OUT2_POS,&paramUtil);
        SM_CURVE_SELECT curveSelect = SM_CURVE_0;
        //计算速度
        curveSelect = AppImplRailTransCalcBestSpeed(paramUtil);
        //运行坐标
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = RAIL_TRANS_STEP_MOTOR_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_RUNNING,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前位置处于轨道传送带位置
        currentMainRailModule = RAIL_MODULE_POS_COMMON;
        currentSubRailRegion = RAIL_SUB_MODULE_COMMON_PUSH_OUT2;
    return LH_ERR_NONE;
}

//轨道运转到急诊位
LH_ERR AppImplRailTransMove2EmergencyCall(void)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        int32_t paramUtil = 0;
        //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //获取坐标
        AppParamReadParam(APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT,APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_EMERGENCY_POS,&paramUtil);
        SM_CURVE_SELECT curveSelect = SM_CURVE_0;
        //计算速度
        curveSelect = AppImplRailTransCalcBestSpeed(paramUtil);
        //运行坐标
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = RAIL_TRANS_STEP_MOTOR_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_RUNNING,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前位置处于急诊位置
        currentMainRailModule = RAIL_MODULE_POS_COMMON;
        currentSubRailRegion = RAIL_SUB_MODULE_COMMON_EMERGENCY_CALL;
    return LH_ERR_NONE;
}

//轨道移动到旋转位1
LH_ERR AppImplRailTransMove2Rotate1(void)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        int32_t paramUtil = 0;
        //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //获取坐标
        AppParamReadParam(APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT,APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_ROTATE1_POS,&paramUtil);
        SM_CURVE_SELECT curveSelect = SM_CURVE_0;
        //计算速度
        curveSelect = AppImplRailTransCalcBestSpeed(paramUtil);
        //运行坐标
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = RAIL_TRANS_STEP_MOTOR_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_RUNNING,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前位置处于旋转1位置
        currentMainRailModule = RAIL_MODULE_POS_COMMON;
        currentSubRailRegion = RAIL_SUB_MODULE_COMMON_ROTATE1;
    return LH_ERR_NONE;
}

//轨道移动到旋转位2
LH_ERR AppImplRailTransMove2Rotate2(void)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        int32_t paramUtil = 0;
        //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //获取坐标
        AppParamReadParam(APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT,APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_ROTATE2_POS,&paramUtil);
        SM_CURVE_SELECT curveSelect = SM_CURVE_0;
        //计算速度
        curveSelect = AppImplRailTransCalcBestSpeed(paramUtil);
        //运行坐标
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = RAIL_TRANS_STEP_MOTOR_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_RUNNING,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前位置处于旋转2位置
        currentMainRailModule = RAIL_MODULE_POS_COMMON;
        currentSubRailRegion = RAIL_SUB_MODULE_COMMON_ROTATE2;
    return LH_ERR_NONE;
}

//轨道移动到靠现在位置最近的旋转位
LH_ERR AppImplRailTransMove2RotateAuto(void)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        int32_t paramUtilRotate1 = 0;
        int32_t paramUtilRotate2 = 0;
        int32_t paramUtil = 0;
        //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
        //目标旋转位
        RAIL_SUB_MODULE rotateTarget = RAIL_SUB_MODULE_COMMON_ROTATE1;
    //获取旋转位的坐标
        //旋转位1
        AppParamReadParam(APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT,APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_ROTATE1_POS,&paramUtilRotate1);
        //旋转位2
        AppParamReadParam(APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT,APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_ROTATE2_POS,&paramUtilRotate2);
        //当前坐标
        paramUtil = IPC_StepMotorCurrentCoordinate(RAIL_TRANS_STEP_MOTOR_ID);
    //计算确认旋转位置
        if(paramUtil <= paramUtilRotate1)
        {
            //旋转位1
            rotateTarget = RAIL_SUB_MODULE_COMMON_ROTATE1;
            paramUtil = paramUtilRotate1;
        }
        else if(paramUtil >= paramUtilRotate2)
        {
            //旋转位2
            rotateTarget = RAIL_SUB_MODULE_COMMON_ROTATE2;
            paramUtil = paramUtilRotate2;
        }
        else
        {
            if(paramUtil > ((paramUtilRotate1 + paramUtilRotate2)/2))
            {
                //过了中间
                rotateTarget = RAIL_SUB_MODULE_COMMON_ROTATE2;
                paramUtil = paramUtilRotate2;
            }
            else
            {
                //没过中间
                rotateTarget = RAIL_SUB_MODULE_COMMON_ROTATE1;
                paramUtil = paramUtilRotate1;
            }
        }
    //走起
        SM_CURVE_SELECT curveSelect = SM_CURVE_0;
        //计算速度
        curveSelect = AppImplRailTransCalcBestSpeed(paramUtil);
        //运行坐标
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = RAIL_TRANS_STEP_MOTOR_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_RUNNING,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前位置处于计算出来的旋转位置
        currentMainRailModule = RAIL_MODULE_POS_COMMON;
        currentSubRailRegion = rotateTarget;
    return LH_ERR_NONE;
}

//轨道移动到试管架扫码位
LH_ERR AppImplRailTransMove2RackBarScan(RAIL_BAR_SCAN_POS scanPos)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        int32_t paramUtil = 0;
        //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //根据试管架扫码起始位
        AppParamReadParam(APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT,APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE1_SCAN_POS,&paramUtil);
    //计算试管架的坐标
        paramUtil += STEP_RACK_BARSCAN_TO_RACK_SCAN_START_OFFSSET; 
        if(scanPos == RAIL_BAR_SCAN_POS_START)
        {
            //起始位置在实际位置前面一点
            paramUtil -= STEP_RACK_BARSCAN_OFFSET;
        }
        else if(scanPos == RAIL_BAR_SCAN_POS_END)
        {
            //结束位置在实际位置后面一点
            paramUtil += STEP_RACK_BARSCAN_OFFSET;
        }
    //计算合理的速度曲线
        SM_CURVE_SELECT curveSelect = SM_CURVE_0;
        if((currentSubRailRegion == RAIL_SUB_MODULE_COMMON_RACK_SCAN)&&(scanPos == RAIL_BAR_SCAN_POS_END))
        {
            //如果当前位置是试管架扫码位,目标位置是扫码完成位置,那么说明这次我是想扫码,用扫码速度
            curveSelect = CURVE_RAIL_TRANS_BARSCAN_SLOW;
        }
        else
        {
            curveSelect = AppImplRailTransCalcBestSpeed(paramUtil);
        }
    //走起
        //运行坐标
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = RAIL_TRANS_STEP_MOTOR_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_RUNNING,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }

    //设置当前位置处于试管架扫码位
        currentMainRailModule = RAIL_MODULE_POS_COMMON;
        currentSubRailRegion = RAIL_SUB_MODULE_COMMON_RACK_SCAN;
    return LH_ERR_NONE;
}

//轨道移动到试管扫码位,参数位试管序号,从0-4
LH_ERR AppImplRailTransMove2TubeBarScan(RAIL_TUBE_POS tubeIndex,RAIL_BAR_SCAN_POS scanPos)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        int32_t paramUtil = 0;
        RAIL_SUB_MODULE tubeSubModule = RAIL_SUB_MODULE_COMMON_TUBE1_SCAN;
        //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //根据目标位置读取位置参数
        if(tubeIndex == RAIL_TUBE_POS_1)
        {
            //试管架扫码起始位
            AppParamReadParam(APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT,APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE1_SCAN_POS,&paramUtil);
            //试管1扫码位偏移
            paramUtil += STEP_TUBE1_BARSCAN_TO_RACK_SCAN_START_OFFSSET;
            tubeSubModule = RAIL_SUB_MODULE_COMMON_TUBE1_SCAN;
        }
        else if(tubeIndex == RAIL_TUBE_POS_2)
        {
            //试管架扫码起始位
            AppParamReadParam(APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT,APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE1_SCAN_POS,&paramUtil);
            //试管2扫码位偏移
            paramUtil += STEP_TUBE2_BARSCAN_TO_RACK_SCAN_START_OFFSSET;
            tubeSubModule = RAIL_SUB_MODULE_COMMON_TUBE2_SCAN;
        }
        else if(tubeIndex == RAIL_TUBE_POS_3)
        {
            //试管架扫码起始位
            AppParamReadParam(APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT,APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE1_SCAN_POS,&paramUtil);
            //试管3扫码位偏移
            paramUtil += STEP_TUBE3_BARSCAN_TO_RACK_SCAN_START_OFFSSET;
            tubeSubModule = RAIL_SUB_MODULE_COMMON_TUBE3_SCAN;
        }
        else if(tubeIndex == RAIL_TUBE_POS_4)
        {
            //试管架扫码起始位
            AppParamReadParam(APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT,APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE1_SCAN_POS,&paramUtil);
            //试管4扫码位偏移
            paramUtil += STEP_TUBE4_BARSCAN_TO_RACK_SCAN_START_OFFSSET;
            tubeSubModule = RAIL_SUB_MODULE_COMMON_TUBE4_SCAN;
        }
        else if(tubeIndex == RAIL_TUBE_POS_5)
        {
            //试管架扫码起始位
            AppParamReadParam(APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT,APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE1_SCAN_POS,&paramUtil);
            //试管5扫码位偏移
            paramUtil += STEP_TUBE5_BARSCAN_TO_RACK_SCAN_START_OFFSSET;
            tubeSubModule = RAIL_SUB_MODULE_COMMON_TUBE5_SCAN;
        }
    //根据偏移决定最终参数
        if(scanPos == RAIL_BAR_SCAN_POS_START)
        {
            paramUtil -= STEP_TUBE_BARSCAN_OFFSET;
        }
        else if(scanPos == RAIL_BAR_SCAN_POS_END)
        {
            paramUtil += STEP_TUBE_BARSCAN_OFFSET;
        }
    //计算速度曲线
        SM_CURVE_SELECT curveSelect = SM_CURVE_0;
        if((currentSubRailRegion == tubeSubModule)&&(scanPos == RAIL_BAR_SCAN_POS_END))
        {
            //如果当前位置和目标位置是同一个试管位,而且偏移是扫码完成
            curveSelect = CURVE_RAIL_TRANS_BARSCAN_SLOW;
        }
        else
        {
            curveSelect = AppImplRailTransCalcBestSpeed(paramUtil);
        }
    //走起
        //运行坐标
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = RAIL_TRANS_STEP_MOTOR_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_RUNNING,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前位置处于试管架扫码位
        currentMainRailModule = RAIL_MODULE_POS_COMMON;
        currentSubRailRegion = tubeSubModule;

    return LH_ERR_NONE;
}

//轨道移动到待测位,参数位模块号和待测位序号
LH_ERR AppImplRailTransMove2SpecialModuleWaitTest(RAIL_MODULE_POS moduleNo,RAIL_WAITTEST_NO waitTestNo)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        int32_t paramUtil = 0;
        RAIL_SUB_MODULE targetSubModuleNo = RAIL_SUB_MODULE_WAITTEST_1;
        uint16_t mainIndex = 0;
        uint16_t subIndex = 0;
        //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //检查模块号
        errorCode = AppImplRailTransCheckTargetRailModule(moduleNo);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //计算坐标
        //主序号
        if(moduleNo == RAIL_MODULE_POS_1)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE1;
        }
        else if(moduleNo == RAIL_MODULE_POS_2)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE2;
        }
        else if(moduleNo == RAIL_MODULE_POS_3)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE3;
        }
        else if(moduleNo == RAIL_MODULE_POS_4)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE4;
        }
        else
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_MAIN_MODULE_NO,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //子序号
        if(waitTestNo == RAIL_WAITTEST_1)
        {
            subIndex = APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST1_POS;
            targetSubModuleNo = RAIL_SUB_MODULE_WAITTEST_1;
        }
        else if(waitTestNo == RAIL_WAITTEST_2)
        {
            subIndex = APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST2_POS;
            targetSubModuleNo = RAIL_SUB_MODULE_WAITTEST_2;
        }
        else if(waitTestNo == RAIL_WAITTEST_3)
        {
            subIndex = APP_PARAM_INDEX_SUB_RAIL_MODULE_WAIT_TEST3_POS;
            targetSubModuleNo = RAIL_SUB_MODULE_WAITTEST_3;
        }
        else
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_SUB_WAIT_TEST_NO,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        AppParamReadParam(mainIndex,subIndex,&paramUtil);

    //计算速度
        SM_CURVE_SELECT curveSelect = SM_CURVE_0;
        curveSelect = AppImplRailTransCalcBestSpeed(paramUtil);
    //运行走起
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = RAIL_TRANS_STEP_MOTOR_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_RUNNING,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前位置处于待测位位置
        currentMainRailModule = moduleNo;
        currentSubRailRegion = targetSubModuleNo;
    return LH_ERR_NONE;
}

//轨道移动到重测位,参数位模块号和待测位序号
LH_ERR AppImplRailTransMove2SpecialModuleReTest(RAIL_MODULE_POS moduleNo,RAIL_RETEST_NO reTestNo)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        int32_t paramUtil = 0;
        RAIL_SUB_MODULE targetSubModuleNo = RAIL_SUB_MODULE_WAITTEST_1;
        uint16_t mainIndex = 0;
        uint16_t subIndex = 0;
        //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //检查模块号
        errorCode = AppImplRailTransCheckTargetRailModule(moduleNo);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //计算坐标
        //主序号
        if(moduleNo == RAIL_MODULE_POS_1)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE1;
        }
        else if(moduleNo == RAIL_MODULE_POS_2)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE2;
        }
        else if(moduleNo == RAIL_MODULE_POS_3)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE3;
        }
        else if(moduleNo == RAIL_MODULE_POS_4)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE4;
        }
        else
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_MAIN_MODULE_NO,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        //子序号
        if(reTestNo == RAIL_RETEST_1)
        {
            subIndex = APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST1_POS;
            targetSubModuleNo = RAIL_SUB_MODULE_RETEST_1;
        }
        else if(reTestNo == RAIL_RETEST_2)
        {
            subIndex = APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST2_POS;
            targetSubModuleNo = RAIL_SUB_MODULE_RETEST_2;
        }
        else if(reTestNo == RAIL_RETEST_3)
        {
            subIndex = APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST3_POS;
            targetSubModuleNo = RAIL_SUB_MODULE_RETEST_3;
        }
        else if(reTestNo == RAIL_RETEST_4)
        {
            subIndex = APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST4_POS;
            targetSubModuleNo = RAIL_SUB_MODULE_RETEST_4;
        }
        else if(reTestNo == RAIL_RETEST_5)
        {
            subIndex = APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST5_POS;
            targetSubModuleNo = RAIL_SUB_MODULE_RETEST_5;
        }
        else if(reTestNo == RAIL_RETEST_6)
        {
            subIndex = APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST6_POS;
            targetSubModuleNo = RAIL_SUB_MODULE_RETEST_6;
        }
        else if(reTestNo == RAIL_RETEST_7)
        {
            subIndex = APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST7_POS;
            targetSubModuleNo = RAIL_SUB_MODULE_RETEST_7;
        }
        else if(reTestNo == RAIL_RETEST_8)
        {
            subIndex = APP_PARAM_INDEX_SUB_RAIL_MODULE_RE_TEST8_POS;
            targetSubModuleNo = RAIL_SUB_MODULE_RETEST_8;
        }
        else
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_SUB_REPEAT_TEST_NO,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
        AppParamReadParam(mainIndex,subIndex,&paramUtil);
    //计算速度
        SM_CURVE_SELECT curveSelect = SM_CURVE_0;
        curveSelect = AppImplRailTransCalcBestSpeed(paramUtil);
    //运行走起
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = RAIL_TRANS_STEP_MOTOR_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_RUNNING,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前位置处于待测位位置
        currentMainRailModule = moduleNo;
        currentSubRailRegion = targetSubModuleNo;
    return LH_ERR_NONE;
}

//轨道移动到测试区放架位,参数位模块号和测试区试管偏移
LH_ERR AppImplRailTransMove2SpecialModuleTestingPut(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        //两个试管之前的偏移
        int32_t paramUtilOneTubeOffset = 0;
        //辅助计算
        int32_t paramUtil = 0;
        //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //检查模块号
        errorCode = AppImplRailTransCheckTargetRailModule(moduleNo);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //计算模块号
        uint8_t mainIndex = 0;
        if(moduleNo == RAIL_MODULE_POS_1)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE1;
        }
        else if(moduleNo == RAIL_MODULE_POS_2)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE2;
        }
        else if(moduleNo == RAIL_MODULE_POS_3)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE3;
        }
        else if(moduleNo == RAIL_MODULE_POS_4)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE4;
        }
        else
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_MAIN_MODULE_NO,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //先计算一个试管的偏移
        //AppParamReadParam(mainIndex,APP_PARAM_INDEX_SUB_RAIL_MODULE_TUBE_OFFSET_TESTING,&paramUtilOneTubeOffset);
        paramUtilOneTubeOffset = RAIL_MODULE_TUBE_OFFSET_TESTING;
    //计算获取放架位置
        AppParamReadParam(mainIndex,APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_PUT,&paramUtil);
    //检查试管序号
        if(tubeIndex > RAIL_TUBE_POS_5)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_SUB_TESTING_PUT_TUBE_INDEX,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //加上试管位偏移
        paramUtil += (((uint8_t)(tubeIndex - RAIL_TUBE_POS_1))*paramUtilOneTubeOffset);
    //计算速度
        SM_CURVE_SELECT curveSelect = SM_CURVE_0;
        curveSelect = AppImplRailTransCalcBestSpeed(paramUtil);
    //运行走起
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = RAIL_TRANS_STEP_MOTOR_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_RUNNING,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前位置处于待测位位置
        currentMainRailModule = moduleNo;
        if(tubeIndex == RAIL_TUBE_POS_1)
        {
            currentSubRailRegion = RAIL_SUB_MODULE_TESTING_PUT_TUBE1;
        }
        else if(tubeIndex == RAIL_TUBE_POS_2)
        {
            currentSubRailRegion = RAIL_SUB_MODULE_TESTING_PUT_TUBE2;
        }
        else if(tubeIndex == RAIL_TUBE_POS_3)
        {
            currentSubRailRegion = RAIL_SUB_MODULE_TESTING_PUT_TUBE3;
        }
        else if(tubeIndex == RAIL_TUBE_POS_4)
        {
            currentSubRailRegion = RAIL_SUB_MODULE_TESTING_PUT_TUBE4;
        }
        else if(tubeIndex == RAIL_TUBE_POS_5)
        {
            currentSubRailRegion = RAIL_SUB_MODULE_TESTING_PUT_TUBE5;
        }
    return LH_ERR_NONE;
}

//轨道移动到测试区取架位,参数位模块号和测试区试管偏移
LH_ERR AppImplRailTransMove2SpecialModuleTestingCatch(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        //两个试管之前的偏移
        int32_t paramUtilOneTubeOffset = 0;
        //辅助计算
        int32_t paramUtil = 0;
        //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //检查模块号
        errorCode = AppImplRailTransCheckTargetRailModule(moduleNo);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //计算模块号
        uint8_t mainIndex = 0;
        if(moduleNo == RAIL_MODULE_POS_1)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE1;
        }
        else if(moduleNo == RAIL_MODULE_POS_2)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE2;
        }
        else if(moduleNo == RAIL_MODULE_POS_3)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE3;
        }
        else if(moduleNo == RAIL_MODULE_POS_4)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE4;
        }
        else
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_MAIN_MODULE_NO,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //先计算一个试管的偏移
        //AppParamReadParam(mainIndex,APP_PARAM_INDEX_SUB_RAIL_MODULE_TUBE_OFFSET_TESTING,&paramUtilOneTubeOffset);
        paramUtilOneTubeOffset = RAIL_MODULE_TUBE_OFFSET_TESTING;
    //计算获取放架位置
        AppParamReadParam(mainIndex,APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_CATCH,&paramUtil);
    //检查试管序号
        if(tubeIndex > RAIL_TUBE_POS_5)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_SUB_TESTING_CATCH_TUBE_INDEX,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //加上试管位偏移
        paramUtil += (((uint8_t)(tubeIndex - RAIL_TUBE_POS_1))*paramUtilOneTubeOffset);
    //计算速度
        SM_CURVE_SELECT curveSelect = SM_CURVE_0;
        curveSelect = AppImplRailTransCalcBestSpeed(paramUtil);
    //运行走起
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = RAIL_TRANS_STEP_MOTOR_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_RUNNING,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前位置处于待测位位置
        currentMainRailModule = moduleNo;
        if(tubeIndex == RAIL_TUBE_POS_1)
        {
            currentSubRailRegion = RAIL_SUB_MODULE_TESTING_CATCH_TUBE1;
        }
        else if(tubeIndex == RAIL_TUBE_POS_2)
        {
            currentSubRailRegion = RAIL_SUB_MODULE_TESTING_CATCH_TUBE2;
        }
        else if(tubeIndex == RAIL_TUBE_POS_3)
        {
            currentSubRailRegion = RAIL_SUB_MODULE_TESTING_CATCH_TUBE3;
        }
        else if(tubeIndex == RAIL_TUBE_POS_4)
        {
            currentSubRailRegion = RAIL_SUB_MODULE_TESTING_CATCH_TUBE4;
        }
        else if(tubeIndex == RAIL_TUBE_POS_5)
        {
            currentSubRailRegion = RAIL_SUB_MODULE_TESTING_CATCH_TUBE5;
        }
    return LH_ERR_NONE;
}

//轨道移动到测试吸样位,参数位模块号和测试区试管偏移
LH_ERR AppImplRailTransMove2SpecialModuleTestingAbsorb(RAIL_MODULE_POS moduleNo,RAIL_TUBE_POS tubeIndex)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        //两个试管之前的偏移
        int32_t paramUtilOneTubeOffset = 0;
        //辅助计算
        int32_t paramUtil = 0;
        //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //检查模块号
        errorCode = AppImplRailTransCheckTargetRailModule(moduleNo);
        if(errorCode != LH_ERR_NONE)
        {
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //计算模块号
        uint8_t mainIndex = 0;
        if(moduleNo == RAIL_MODULE_POS_1)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE1;
        }
        else if(moduleNo == RAIL_MODULE_POS_2)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE2;
        }
        else if(moduleNo == RAIL_MODULE_POS_3)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE3;
        }
        else if(moduleNo == RAIL_MODULE_POS_4)
        {
            mainIndex = APP_PARAM_MAIN_INDEX_RAIL_MODULE4;
        }
        else
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_MAIN_MODULE_NO,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //先计算一个试管的偏移
        //AppParamReadParam(mainIndex,APP_PARAM_INDEX_SUB_RAIL_MODULE_TUBE_OFFSET_TESTING,&paramUtilOneTubeOffset);
        paramUtilOneTubeOffset = RAIL_MODULE_TUBE_OFFSET_TESTING;
    //计算获取放架位置
        AppParamReadParam(mainIndex,APP_PARAM_INDEX_SUB_RAIL_MODULE_TESTING_ABSORB,&paramUtil);
    //检查试管序号
        if(tubeIndex > RAIL_TUBE_POS_5)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_SUB_TESTING_ABSORB_TUBE_INDEX,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //加上试管位偏移
        paramUtil += (((uint8_t)(tubeIndex - RAIL_TUBE_POS_1))*paramUtilOneTubeOffset);
    //计算速度
        SM_CURVE_SELECT curveSelect = SM_CURVE_0;
        curveSelect = AppImplRailTransCalcBestSpeed(paramUtil);
    //运行走起
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = RAIL_TRANS_STEP_MOTOR_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_RUNNING,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //设置当前位置处于待测位位置
        currentMainRailModule = moduleNo;
        if(tubeIndex == RAIL_TUBE_POS_1)
        {
            currentSubRailRegion = RAIL_SUB_MODULE_TESTING_ABSORB_TUBE1;
        }
        else if(tubeIndex == RAIL_TUBE_POS_2)
        {
            currentSubRailRegion = RAIL_SUB_MODULE_TESTING_ABSORB_TUBE2;
        }
        else if(tubeIndex == RAIL_TUBE_POS_3)
        {
            currentSubRailRegion = RAIL_SUB_MODULE_TESTING_ABSORB_TUBE3;
        }
        else if(tubeIndex == RAIL_TUBE_POS_4)
        {
            currentSubRailRegion = RAIL_SUB_MODULE_TESTING_ABSORB_TUBE4;
        }
        else if(tubeIndex == RAIL_TUBE_POS_5)
        {
            currentSubRailRegion = RAIL_SUB_MODULE_TESTING_ABSORB_TUBE5;
        }
    return LH_ERR_NONE;
}

//轨道移动到扫码位
LH_ERR AppImplRailTransMove2BarScanIntegrate(RAIL_BAR_SCAN_POS scanPos)
{
    //必要的临时变量
        LH_ERR errorCode = LH_ERR_NONE;
        int32_t paramUtil = 0;
        RAIL_SUB_MODULE tubeSubModule = RAIL_SUB_MODULE_COMMON_TUBE1_SCAN;
        //电机走坐标结构体
        StepMotorRun2CoordationCmd runToPosCmd;
    //根据偏移决定最终参数
        //扫码位起始坐标
        AppParamReadParam(APP_PARAM_MAIN_INDEX_RAIL_INDEPENDENT,APP_PARAM_INDEX_SUB_RAIL_INDENPENDENT_TUBE1_SCAN_POS,&paramUtil);
        if(scanPos == RAIL_BAR_SCAN_POS_START)
        {
            tubeSubModule = RAIL_SUB_MODULE_COMMON_TUBE1_SCAN;
        }
        else if(scanPos == RAIL_BAR_SCAN_POS_END)
        {
            //扫码位结束
            tubeSubModule = RAIL_SUB_MODULE_COMMON_RACK_SCAN;
            paramUtil += (STEP_TUBE_BARSCAN_OFFSET+STEP_SAMPLE_RACK_MAX_WITH);
        }
    //计算速度曲线
        SM_CURVE_SELECT curveSelect = SM_CURVE_0;
        if((currentSubRailRegion == RAIL_SUB_MODULE_COMMON_TUBE1_SCAN)&&(scanPos == RAIL_BAR_SCAN_POS_END))
        {
            //如果当前位置是扫码起始位,而且目标位置是扫码完成
            curveSelect = CURVE_RAIL_TRANS_BARSCAN_SLOW;
        }
        else
        {
            curveSelect = AppImplRailTransCalcBestSpeed(paramUtil);
        }
    //走起
        //运行坐标
        IPC_StepMotorUtilRunCoordinateDataStructInit(&runToPosCmd);
        runToPosCmd.motorIndex = RAIL_TRANS_STEP_MOTOR_ID;//电机序号
        runToPosCmd.targetCoordinate = paramUtil;//目标坐标
        runToPosCmd.selectConfig = curveSelect;//速度曲线
        runToPosCmd.timeOutSet = 60000;
        errorCode = IPC_StepMotorRun2CoordinateWhileReturn(&runToPosCmd);
        if(errorCode != LH_ERR_NONE)
        {
            errorCode = AppImplRailTransConvertErrorCode(FLAG_RAIL_TRANS_ERR_RUNNING,errorCode);
            AppShowErrMsg(errorCode);
            return errorCode;
        }
    //根据当前扫码位控制机械手动作
        IPC_RailHandUpDownBarScanPosWhileReturn(scanPos);
    //设置当前位置处于试管架扫码位
        currentMainRailModule = RAIL_MODULE_POS_COMMON;
        currentSubRailRegion = tubeSubModule;
    return LH_ERR_NONE;
}

//获取当前轨道位置
void AppImplRailTransReadCurrentModuleAndRegion(RAIL_MODULE_POS* currentMainModulePtr,RAIL_SUB_MODULE* currentSubRegionPtr)
{
    *currentMainModulePtr = currentMainRailModule;
    *currentSubRegionPtr = currentSubRailRegion;
}

