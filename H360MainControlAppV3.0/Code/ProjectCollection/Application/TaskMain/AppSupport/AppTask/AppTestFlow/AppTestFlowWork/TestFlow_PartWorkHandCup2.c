#include "TestFlow_Config.h"
#include "TestFlow_Engine.h"
#include "TestFlow_Warning.h"
#include "TestFlow_PartWorkHandCup2.h"
#include "TestFlow_ActionAPI.h"

static int32_t TestFlow_HandCup2ResetInit(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    TestFlow_Api_HandCup2Reset();
    return TESTFLOW_RESULT_OK;
}

static uint8_t TestFlow_HandCup2Result(void *pStatus)
{
    return TestFlow_Api_HandCup2CheckResult();
}

static void TestFlow_HandCup2ResetCheckOK(uint8_t u8Offset)
{
    DebugMsg("HandCup2ResetOK\r\n");
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_RESET_OK, TESTFLOW_UNIT_HANDCUP2, NULL, 0);
}

static void TestFlow_HandCup2ResetCheckNG(uint8_t u8Offset)
{
    DebugMsg("E>HandCup2ResetNG:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_RESET_NG, TESTFLOW_UNIT_HANDCUP2, NULL, 0);
}

static void TestFlow_HandCup2ResetTimeout(uint8_t u8Offset)
{
    DebugMsg("E>HandCup2ResetTimeout:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_RESET_TIMEOUT, TESTFLOW_UNIT_HANDCUP2, NULL, 0);
}


const FSM_STATE_T g_stWorkHandCup2ResetTable[] = 
{
    /* State 1 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup2ResetInit,          TestFlow_HandCup2Result,       TestFlow_HandCup2ResetCheckOK,         TestFlow_HandCup2ResetCheckNG,          TestFlow_HandCup2ResetTimeout },
};

static uint8_t* g_pu8HandCup2ToMeasureWarningInfo = NULL;
static int32_t TestFlow_HandCup2CupToMeasureInit(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    if (pu8Param == NULL || u8Size < 8) return TESTFLOW_RESULT_ERR_PARAM;
    g_pu8HandCup2ToMeasureWarningInfo = &pu8Param[0];
    return TestFlow_Api_HandCup2ToWashDisk();
}

static int32_t TestFlow_HandCup2CupCatchWashCup(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_CATCH_WASH, TESTFLOW_UNIT_HANDCUP2, NULL, 0);
    return TestFlow_Api_HandCup2CatchWashCup();
}

static int32_t TestFlow_HandCup2CupToMeasure(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    return TestFlow_Api_HandCup2ToMeasure();
}

static uint8_t TestFlow_HandCup2WaitMeasureHalfOpen(void *pStatus)
{
    uint8_t u8Result = TestFlow_Api_HandCup2ActionResult(g_pu8HandCup2ToMeasureWarningInfo, 8);
    if (u8Result == FSM_CHECK_OK)
    {
        if (((TESTFLOW_STATUS_T *)pStatus)->eMeasureDoorState == TESTFLOW_MEASUREDOOR_STATE_HALFOPEN)
        {
            return FSM_CHECK_OK;
        }
        else
        {
            return FSM_CHECK_WAIT;
        }
    }
    else
    {
        return u8Result;
    }
}

static int32_t TestFlow_HandCup2CupToMeasurePutCup(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    //TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_PUT_MEASURE, TESTFLOW_UNIT_HANDCUP2, NULL, 0);
    return TestFlow_Api_HandCup2PutMeasureCup();
}

static uint8_t TestFlow_HandCup2CupToWashDiskAndWaitStopResult(void *pStatus)
{
    if (((TESTFLOW_STATUS_T *)pStatus)->eWashDiskState == TESTFLOW_WASHDISK_STATE_IDLE)
    {
        return TestFlow_Api_HandCup2ActionResult(g_pu8HandCup2ToMeasureWarningInfo, 8);
    }
    else
    {
        return FSM_CHECK_WAIT;
    }
}

static uint8_t TestFlow_HandCup2CupCatchWashCupResult(void *pStatus)
{
    uint8_t u8Result = TestFlow_Api_HandCup2ActionResult(g_pu8HandCup2ToMeasureWarningInfo, 8);
    if (u8Result == FSM_CHECK_OK)
    {
        if (TestFlow_Api_WashDiskHandCup2IsExist() == 0)
        {
            TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_CATCH_WASH_OK, TESTFLOW_UNIT_HANDCUP2, NULL, 0);
            return FSM_CHECK_OK;
        }
        else
        {
            DebugMsg("CupToMR_NG,HasCup\r\n");
            TestFlow_WarningReport(TESTFLOW_WARNING_HANDCUP2_WASH_CUP_EXIST_AFTER_CATCH,NULL,0);
            return FSM_CHECK_NG;
        }
    }
    else
    {
        return u8Result;
    }
}

static uint8_t TestFlow_HandCup2ToMeasureResult(void *pStatus)
{
    return TestFlow_Api_HandCup2ActionResult(g_pu8HandCup2ToMeasureWarningInfo, 8);
}

static void TestFlow_HandCup2CupToMeasureOK(uint8_t u8Offset)
{
    //DebugMsg("HandCup2CupToMeasureOK\r\n");
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_TO_MEASURE_OK, TESTFLOW_UNIT_HANDCUP2, NULL, 0);
}

static void TestFlow_HandCup2CupToMeasureNG(uint8_t u8Offset)
{
    uint8_t  u8TxBuf[4] = {0, 0, 0, 0};
    u8TxBuf[0] = u8Offset;
    if (g_pu8HandCup2ToMeasureWarningInfo != NULL)
    {
        u8TxBuf[1] = g_pu8HandCup2ToMeasureWarningInfo[7];
    }
    DebugMsg("E>HandCup2CupToMeasureNG:%d,%d\r\n", u8TxBuf[0], u8TxBuf[1]);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_TO_MEASURE_NG, TESTFLOW_UNIT_HANDCUP2, &u8TxBuf[0], 2);
}

static void TestFlow_HandCup2CupToMeasureTimeout(uint8_t u8Offset)
{
    uint8_t  u8TxBuf[4] = {0, 0, 0, 0};
    u8TxBuf[0] = u8Offset;
    if (g_pu8HandCup2ToMeasureWarningInfo != NULL)
    {
        u8TxBuf[1] = g_pu8HandCup2ToMeasureWarningInfo[7];
    }
    DebugMsg("E>HandCup2CupToMeasureTimeout:%d,%d\r\n", u8TxBuf[0], u8TxBuf[1]);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_TO_MEASURE_TIMEOUT, TESTFLOW_UNIT_HANDCUP2, &u8TxBuf[0], 2);
}


//旋转至清洗盘->抓清洗盘杯->旋转至测量室->测量室放杯->关门
const FSM_STATE_T g_stWorkHandCup2CupToMeasureTable[] = 
{
    /* State 0 */       {1,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup2CupToMeasureInit,          TestFlow_HandCup2CupToWashDiskAndWaitStopResult,    NULL,                                    TestFlow_HandCup2CupToMeasureNG,          TestFlow_HandCup2CupToMeasureTimeout },
    /* State 1 */       {2,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup2CupCatchWashCup,           TestFlow_HandCup2CupCatchWashCupResult,             NULL,                                    TestFlow_HandCup2CupToMeasureNG,          TestFlow_HandCup2CupToMeasureTimeout },
    /* State 2 */       {3,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      10000,      TestFlow_HandCup2CupToMeasure,              TestFlow_HandCup2WaitMeasureHalfOpen,               NULL,                                    TestFlow_HandCup2CupToMeasureNG,          TestFlow_HandCup2CupToMeasureTimeout },
    /* State 3 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup2CupToMeasurePutCup,        TestFlow_HandCup2ToMeasureResult,                   TestFlow_HandCup2CupToMeasureOK,         TestFlow_HandCup2CupToMeasureNG,          TestFlow_HandCup2CupToMeasureTimeout },
};

static uint8_t* g_pu8HandCup2ReactToWashWarningInfo = NULL;
static uint8_t g_u8CupID = 0;
static int32_t TestFlow_HandCup2CupToWashDiskInit(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    if (pStatus == NULL)
    {
        return TESTFLOW_RESULT_ERR_PARAM;
    }
    if (pu8Param == NULL || u8Size < 8) return TESTFLOW_RESULT_ERR_PARAM;

    g_pu8HandCup2ReactToWashWarningInfo = &pu8Param[0];
	g_u8CupID = g_pu8HandCup2ReactToWashWarningInfo[6];
	DebugMsg("CupToWash:%d\r\n",g_u8CupID);
    return TestFlow_Api_HandCup2ToReact();
}

static uint8_t TestFlow_HandCup2CupToWashDiskResult(void *pStatus)
{
    if (((TESTFLOW_STATUS_T *)pStatus)->eReactState == TESTFLOW_REACT_STATE_4_STOP)
    {
        return TestFlow_Api_HandCup2ActionResult(g_pu8HandCup2ReactToWashWarningInfo, 8);
    }
    else
    {
        return FSM_CHECK_WAIT;
    }
}

static int32_t TestFlow_HandCup2CupToWashDiskCatchReact(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_TO_WASHDISK_CATCH_REACT, TESTFLOW_UNIT_HANDCUP2, NULL, 0);
    return TestFlow_Api_HandCup2CatchReactCup();
}

static int32_t TestFlow_HandCup2CupToWashDiskToWash(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    return TestFlow_Api_HandCup2ToWashDisk();
}

static int32_t TestFlow_HandCup2CupToWashDiskPutWash(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    if (((TESTFLOW_STATUS_T *)pStatus)->eWashDiskState == TESTFLOW_WASHDISK_STATE_IDLE)
    {
        TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_TO_WASHDISK_PUT_WASH, TESTFLOW_UNIT_HANDCUP2, NULL, 0);
        return TestFlow_Api_HandCup2PutWashCup();
    }
    else
    {
        DebugMsg("E>HandCup2CupToWashDiskPutWash:%d\r\n", ((TESTFLOW_STATUS_T *)pStatus)->eWashDiskState);
        return TESTFLOW_RESULT_NG;
    }
}

static uint8_t TestFlow_HandCup2CupCatchReactCupResult(void *pStatus)
{
    uint8_t u8Result = TestFlow_Api_HandCup2ActionResult(g_pu8HandCup2ReactToWashWarningInfo, 8);
    if (u8Result == FSM_CHECK_OK)
    {
        if (TestFlow_Api_ReactHandCup2IsExist() == 1)
        {
            DebugMsg("CupToWashNG,HasCup\r\n");
            TestFlow_WarningReport(TESTFLOW_WARNING_HANDCUP2_REAC_CUP_EXIST_AFTER_CATCH,NULL,0);
            return FSM_CHECK_NG;
        }
        else
        {        
            return FSM_CHECK_OK;
        }
    }
    else
    {
        return u8Result;
    }
}

static uint8_t TestFlow_HandCup2ReactToWashPutCupResult(void *pStatus)
{
    uint8_t u8Result = TestFlow_Api_HandCup2ActionResult(g_pu8HandCup2ReactToWashWarningInfo, 8);
    if (u8Result == FSM_CHECK_OK)
    {
        if (TestFlow_Api_WashDiskHandCup2IsExist() == 1)
        {
            return FSM_CHECK_OK;
        }
        else
        {
            if (TestFlow_System_IsDebug() == 1)
            {
                return FSM_CHECK_OK;
            }
            else
            {
                DebugMsg("CupInWashNG,NoCup\r\n");
                TestFlow_WarningReport(TESTFLOW_WARNING_HANDCUP2_WASH_CUP_NOT_EXIST_AFTER_PUT,NULL,0);
                return FSM_CHECK_NG;
            }
        }
    }
    else
    {
        return u8Result;
    }
}

static uint8_t TestFlow_HandCup2ReactToWashResult(void *pStatus)
{
    return TestFlow_Api_HandCup2ActionResult(g_pu8HandCup2ReactToWashWarningInfo, 8);
}

static void TestFlow_HandCup2CupToWashDiskCatchReactOK(uint8_t u8Offset)
{
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_TO_WASHDISK_REACT_OK, TESTFLOW_UNIT_HANDCUP2, NULL, 0);
}

static void TestFlow_HandCup2CupToWashDiskOK(uint8_t u8Offset)
{
    //DebugMsg("HandCup2CupToWashDiskOK\r\n");
    uint8_t u8TxBuf[4] = {0};
	u8TxBuf[0] = g_u8CupID;
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_TO_WASHDISK_OK, TESTFLOW_UNIT_HANDCUP2, &u8TxBuf[0], 1);
}

static void TestFlow_HandCup2CupToWashDiskNG(uint8_t u8Offset)
{
    uint8_t  u8TxBuf[4] = {0, 0, 0, 0};
    u8TxBuf[0] = u8Offset;
    if (g_pu8HandCup2ReactToWashWarningInfo != NULL)
    {
        u8TxBuf[1] = g_pu8HandCup2ReactToWashWarningInfo[6];
        u8TxBuf[2] = g_pu8HandCup2ReactToWashWarningInfo[7];
    }
    DebugMsg("E>HandCup2CupToWashDiskNG:%d,%d\r\n", u8TxBuf[0], u8TxBuf[1]);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_TO_WASHDISK_NG, TESTFLOW_UNIT_HANDCUP2, &u8TxBuf[0], 3);
}

static void TestFlow_HandCup2CupToWashDiskTimeout(uint8_t u8Offset)
{
    uint8_t  u8TxBuf[4] = {0, 0, 0, 0};
    u8TxBuf[0] = u8Offset;
    if (g_pu8HandCup2ReactToWashWarningInfo != NULL)
    {
        u8TxBuf[1] = g_pu8HandCup2ReactToWashWarningInfo[6];
        u8TxBuf[2] = g_pu8HandCup2ReactToWashWarningInfo[7];
    }
    DebugMsg("E>HandCup2CupToWashDiskTimeout:%d,%d\r\n", u8TxBuf[0], u8TxBuf[1]);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_TO_WASHDISK_TIMEOUT, TESTFLOW_UNIT_HANDCUP2, &u8TxBuf[0], 3);
}


//旋转至反应盘->抓反应盘杯->旋转至清洗盘->清洗盘放杯
const FSM_STATE_T g_stWorkHandCup2CupToWashDiskTable[] = 
{
    /* State 0 */       {1,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup2CupToWashDiskInit,         TestFlow_HandCup2CupToWashDiskResult,               NULL,                                        TestFlow_HandCup2CupToWashDiskNG,     TestFlow_HandCup2CupToWashDiskTimeout },
    /* State 1 */       {2,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup2CupToWashDiskCatchReact,   TestFlow_HandCup2CupCatchReactCupResult,            TestFlow_HandCup2CupToWashDiskCatchReactOK,  TestFlow_HandCup2CupToWashDiskNG,     TestFlow_HandCup2CupToWashDiskTimeout },
    /* State 2 */       {3,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup2CupToWashDiskToWash,       TestFlow_HandCup2ReactToWashResult,                 NULL,                                        TestFlow_HandCup2CupToWashDiskNG,     TestFlow_HandCup2CupToWashDiskTimeout },
    /* State 3 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup2CupToWashDiskPutWash,      TestFlow_HandCup2ReactToWashPutCupResult,           TestFlow_HandCup2CupToWashDiskOK,            TestFlow_HandCup2CupToWashDiskNG,     TestFlow_HandCup2CupToWashDiskTimeout },
};

static uint8_t* g_pu8HandCup2MeasureToReactWarningInfo = NULL;
static int32_t TestFlow_HandCup2CupToReactInit(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    if (pStatus == NULL)
    {
        return TESTFLOW_RESULT_ERR_PARAM;
    }

    if (pu8Param == NULL || u8Size < 8) return TESTFLOW_RESULT_ERR_PARAM;
    
    g_pu8HandCup2MeasureToReactWarningInfo = &pu8Param[0];
    return TestFlow_Api_HandCup2ToMeasure();
}

static uint8_t TestFlow_HandCup2CupToReactToMeasureResult(void *pStatus)
{
    if (((TESTFLOW_STATUS_T *)pStatus)->eMeasureDoorState == TESTFLOW_MEASUREDOOR_STATE_FULLOPEN)
    {
        return TestFlow_Api_HandCup2ActionResult(g_pu8HandCup2MeasureToReactWarningInfo, 8);
    }
    else
    {
        return FSM_CHECK_WAIT;
    }
}

static int32_t TestFlow_HandCup2CupToReactCatchMeasure(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    //TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_TO_REACT_CATCH_MEASURE, TESTFLOW_UNIT_HANDCUP2, NULL, 0);
    return TestFlow_Api_HandCup2CatchMeasureCup();
}

static void TestFlow_HandCup2CupToReactCatchMeasureOK(uint8_t u8Offset)
{
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_TO_REACT_CATCH_MEASURE_OK, TESTFLOW_UNIT_HANDCUP2, NULL, 0);
}

static int32_t TestFlow_HandCup2ToReact(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    return TestFlow_Api_HandCup2ToReact();
}

static int32_t TestFlow_HandCup2CupToReactPutReact(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    if (((TESTFLOW_STATUS_T *)pStatus)->eReactState == TESTFLOW_REACT_STATE_4_STOP)
    {
        TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_TO_REACT_PUT_REACT, TESTFLOW_UNIT_HANDCUP2, NULL, 0);
        return TestFlow_Api_HandCup2PutReactCup();
    }
    else
    {
        DebugMsg("E>HandCup2CupToReactPutReact:%d\r\n", ((TESTFLOW_STATUS_T *)pStatus)->eReactState);        
        return TESTFLOW_RESULT_NG;
    }
}

static uint8_t TestFlow_HandCup2CupPutReactCupResult(void *pStatus)
{
    uint8_t u8Result = TestFlow_Api_HandCup2ActionResult(g_pu8HandCup2MeasureToReactWarningInfo, 8);
    if (u8Result == FSM_CHECK_OK)
    {
        if (TestFlow_Api_ReactHandCup2IsExist() == 1)
        {
            return FSM_CHECK_OK;
        }
        else
        {
            if (TestFlow_System_IsDebug() == 1)
            {
                return FSM_CHECK_OK;
            }
            else
            {
                DebugMsg("HandCup2PutReactNG,NoCup\r\n");
                TestFlow_WarningReport(TESTFLOW_WARNING_HANDCUP2_REAC_CUP_NOT_EXIST_AFTER_PUT,NULL,0);
                return FSM_CHECK_NG;
            }
        }
    }
    else
    {
        return u8Result;
    }
}

static uint8_t TestFlow_HandCup2MeasureToReactResult(void *pStatus)
{
    return TestFlow_Api_HandCup2ActionResult(g_pu8HandCup2MeasureToReactWarningInfo, 8);
}

static void TestFlow_HandCup2CupToReactOK(uint8_t u8Offset)
{
    uint8_t u8TxBuf[4] = {0};
	u8TxBuf[0] = g_pu8HandCup2MeasureToReactWarningInfo[6];
	DebugMsg("HandCup2CupToReactOK:%d\r\n",u8TxBuf[0]);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_TO_REACT_OK, TESTFLOW_UNIT_HANDCUP2, &u8TxBuf[0], 1);
}

static void TestFlow_HandCup2CupToReactNG(uint8_t u8Offset)
{
    uint8_t u8TxBuf[4];
    
    u8TxBuf[0] = u8Offset;
    if (g_pu8HandCup2MeasureToReactWarningInfo != NULL)
    {
        u8TxBuf[1] = g_pu8HandCup2MeasureToReactWarningInfo[6];
        u8TxBuf[2] = g_pu8HandCup2MeasureToReactWarningInfo[7];
    }
    DebugMsg("E>HandCup2CupToReactNG:%d,%d\r\n", u8TxBuf[0], u8TxBuf[1]);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_TO_REACT_NG, TESTFLOW_UNIT_HANDCUP2, &u8TxBuf[0], 3);
}

static void TestFlow_HandCup2CupToReactTimeout(uint8_t u8Offset)
{
    uint8_t u8TxBuf[4];
    
    u8TxBuf[0] = u8Offset;
    if (g_pu8HandCup2MeasureToReactWarningInfo != NULL)
    {
        u8TxBuf[1] = g_pu8HandCup2MeasureToReactWarningInfo[6];
        u8TxBuf[2] = g_pu8HandCup2MeasureToReactWarningInfo[7];
    }
    DebugMsg("E>HandCup2CupToReactTimeout:%d,%d\r\n", u8TxBuf[0], u8TxBuf[1]);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_TO_REACT_TIMEOUT, TESTFLOW_UNIT_HANDCUP2, &u8TxBuf[0], 3);
}


//旋转至测量室->取测量室杯->旋转至反应盘->放杯子(去丢弃)
const FSM_STATE_T g_stWorkHandCup2CupToReactTable[] = 
{
    /* State 0 */       {1,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup2CupToReactInit,            TestFlow_HandCup2CupToReactToMeasureResult,    NULL,                                        TestFlow_HandCup2CupToReactNG,     TestFlow_HandCup2CupToReactTimeout },
    /* State 1 */       {2,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup2CupToReactCatchMeasure,    TestFlow_HandCup2MeasureToReactResult,         TestFlow_HandCup2CupToReactCatchMeasureOK,   TestFlow_HandCup2CupToReactNG,     TestFlow_HandCup2CupToReactTimeout },
    /* State 2 */       {3,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup2ToReact,                   TestFlow_HandCup2MeasureToReactResult,         NULL,                                        TestFlow_HandCup2CupToReactNG,     TestFlow_HandCup2CupToReactTimeout },
    /* State 3 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup2CupToReactPutReact,        TestFlow_HandCup2CupPutReactCupResult,         TestFlow_HandCup2CupToReactOK,               TestFlow_HandCup2CupToReactNG,     TestFlow_HandCup2CupToReactTimeout },
};

static uint8_t* g_pu8HandCup2WashToReactWarningInfo = NULL;
static int32_t TestFlow_HandCup2WashToReactInit(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    if (pStatus == NULL)
    {
        return TESTFLOW_RESULT_ERR_PARAM;
    }
    if (pu8Param == NULL || u8Size < 8) return TESTFLOW_RESULT_ERR_PARAM;

    g_pu8HandCup2WashToReactWarningInfo = &pu8Param[0];
    return TestFlow_Api_HandCup2ToWashDisk();
}

static int32_t TestFlow_HandCup2PutReact(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_TO_REACT_PUT_REACT, TESTFLOW_UNIT_HANDCUP2, NULL, 0);
    return TestFlow_Api_HandCup2PutReactCup();
}

static uint8_t TestFlow_HandCup2CupToReactAndWaitStopResult(void *pStatus)
{
    if (((TESTFLOW_STATUS_T *)pStatus)->eWashDiskState == TESTFLOW_WASHDISK_STATE_IDLE)
    {
        return TestFlow_Api_HandCup2ActionResult(g_pu8HandCup2WashToReactWarningInfo, 8);
    }
    else
    {
        return FSM_CHECK_WAIT;
    }
}

static uint8_t TestFlow_HandCup2ToReactCatchResult(void *pStatus)
{
    uint8_t u8Result = TestFlow_Api_HandCup2ActionResult(g_pu8HandCup2WashToReactWarningInfo, 8);
    if (u8Result == FSM_CHECK_OK)
    {
        if (TestFlow_Api_WashDiskHandCup2IsExist() == 0)
        {
            TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_CATCH_WASH_OK, TESTFLOW_UNIT_HANDCUP2, NULL, 0);
            return FSM_CHECK_OK;
        }
        else
        {
            DebugMsg("CupToReact_NG,HasCup\r\n");
            TestFlow_WarningReport(TESTFLOW_WARNING_HANDCUP2_WASH_CUP_EXIST_AFTER_CATCH,NULL,0);
            return FSM_CHECK_NG;
        }
    }
    else
    {
        return u8Result;
    }
}

static uint8_t TestFlow_HandCup2ToReactPutResult(void *pStatus)
{
    uint8_t u8Result = TestFlow_Api_HandCup2ActionResult(g_pu8HandCup2WashToReactWarningInfo, 8);
    if (u8Result == FSM_CHECK_OK)
    {
        if (TestFlow_Api_ReactHandCup2IsExist() == 1)
        {
            return FSM_CHECK_OK;
        }
        else
        {
            if (TestFlow_System_IsDebug() == 1)
            {
                return FSM_CHECK_OK;
            }
            else
            {
                DebugMsg("HandCup2WashPutReactNG,NoCup\r\n");
                TestFlow_WarningReport(TESTFLOW_WARNING_HANDCUP2_REAC_CUP_NOT_EXIST_AFTER_PUT,NULL,0);
                return FSM_CHECK_NG;
            }
        }
    }
    else
    {
        return u8Result;
    }
}

static void TestFlow_HandCup2WashToReactOK(uint8_t u8Offset)
{
    //DebugMsg("HandCup2CupToReactOK\r\n");
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_WASH_TO_REACT_OK, TESTFLOW_UNIT_HANDCUP2, NULL, 0);
}

static void TestFlow_HandCup2WashToReactNG(uint8_t u8Offset)
{
    uint8_t u8TxBuf[4];
    
    u8TxBuf[0] = u8Offset;
    if (g_pu8HandCup2WashToReactWarningInfo != NULL)
    {
        u8TxBuf[1] = g_pu8HandCup2WashToReactWarningInfo[6];
        u8TxBuf[2] = g_pu8HandCup2WashToReactWarningInfo[7];
    }
    DebugMsg("E>HandCup2WashToReactNG:%d,%d\r\n", u8TxBuf[0], u8TxBuf[1], u8TxBuf[2]);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_WASH_TO_REACT_NG, TESTFLOW_UNIT_HANDCUP2, &u8TxBuf[0], 3);
}

static void TestFlow_HandCup2WashToReactTimeout(uint8_t u8Offset)
{
    uint8_t u8TxBuf[4];
    
    u8TxBuf[0] = u8Offset;
    if (g_pu8HandCup2WashToReactWarningInfo != NULL)
    {
        u8TxBuf[1] = g_pu8HandCup2WashToReactWarningInfo[6];
        u8TxBuf[2] = g_pu8HandCup2WashToReactWarningInfo[7];
    }
    DebugMsg("E>HandCup2WashToReactTimeout:%d,%d,%d\r\n", u8TxBuf[0], u8TxBuf[1], u8TxBuf[2]);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP2_WASH_TO_REACT_TIMEOUT, TESTFLOW_UNIT_HANDCUP2, &u8TxBuf[0], 3);
}

static uint8_t TestFlow_HandCup2WashToReactWaitReactReady(void *pStatus)
{
    uint8_t u8Result = TestFlow_Api_HandCup2ActionResult(g_pu8HandCup2WashToReactWarningInfo, 8);
    if (u8Result == FSM_CHECK_OK)
    {
        if (((TESTFLOW_STATUS_T *)pStatus)->eReactState == TESTFLOW_REACT_STATE_1_STOP)
        {
            return FSM_CHECK_OK;
        }
        else
        {
            return FSM_CHECK_WAIT;
        }
    }
    else
    {
        return u8Result;
    }
}



//旋转至清洗盘->抓清洗盘杯->旋转至反应盘->放杯
const FSM_STATE_T g_stWorkHandCup2WashToReactTable[] = 
{
    /* State 0 */       {1,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      10000,      TestFlow_HandCup2WashToReactInit,           TestFlow_HandCup2CupToReactAndWaitStopResult,       NULL,                                    TestFlow_HandCup2WashToReactNG,          TestFlow_HandCup2WashToReactTimeout },
    /* State 1 */       {2,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      10000,      TestFlow_HandCup2CupCatchWashCup,           TestFlow_HandCup2ToReactCatchResult,                NULL,                                    TestFlow_HandCup2WashToReactNG,          TestFlow_HandCup2WashToReactTimeout },
    /* State 2 */       {3,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      10000,      TestFlow_HandCup2ToReact,                   TestFlow_HandCup2WashToReactWaitReactReady,         NULL,                                    TestFlow_HandCup2WashToReactNG,          TestFlow_HandCup2WashToReactTimeout },
    /* State 3 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      10000,      TestFlow_HandCup2PutReact,                  TestFlow_HandCup2ToReactPutResult,                  TestFlow_HandCup2WashToReactOK,          TestFlow_HandCup2WashToReactNG,          TestFlow_HandCup2WashToReactTimeout },
};


