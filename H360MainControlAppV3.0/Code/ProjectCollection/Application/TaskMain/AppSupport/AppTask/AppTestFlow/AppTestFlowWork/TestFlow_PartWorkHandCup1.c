#include "TestFlow_Config.h"
#include "TestFlow_Engine.h"
#include "TestFlow_Warning.h"
#include "TestFlow_PartWorkHandCup1.h"
#include "TestFlow_ActionAPI.h"

static int32_t TestFlow_HandCup1ResetInit(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    TestFlow_Api_HandCup1Reset();
    return TESTFLOW_RESULT_OK;
}

static uint8_t TestFlow_HandCup1CheckResult(void *pStatus)
{
    return TestFlow_Api_HandCup1CheckResult();
}

static void TestFlow_HandCup1ResetCheckOK(uint8_t u8Offset)
{
    DebugMsg("HandCup1ResetOK\r\n");
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP1_RESET_OK, TESTFLOW_UNIT_HANDCUP1, NULL, 0);
}

static void TestFlow_HandCup1ResetCheckNG(uint8_t u8Offset)
{
    DebugMsg("E>HandCup1ResetNG:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP1_RESET_NG, TESTFLOW_UNIT_HANDCUP1, NULL, 0);
}

static void TestFlow_HandCup1ResetTimeout(uint8_t u8Offset)
{
    DebugMsg("E>HandCup1ResetTimeout:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP1_RESET_TIMEOUT, TESTFLOW_UNIT_HANDCUP1, NULL, 0);
}

//机械手复位流程
const FSM_STATE_T g_stWorkHandCup1ResetTable[] = 
{
    /* State 0 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup1ResetInit,          TestFlow_HandCup1CheckResult,       TestFlow_HandCup1ResetCheckOK,         TestFlow_HandCup1ResetCheckNG,          TestFlow_HandCup1ResetTimeout },
};

static uint8_t TestFlow_HandCup1CupPutNewCupResult(void *pStatus)
{
    uint8_t u8Result = TestFlow_Api_HandCup1CheckResult();
    if (u8Result == FSM_CHECK_OK)
    {
        if (TestFlow_Api_ReactHandCup1IsExist() == 1)
        {
            return FSM_CHECK_OK;
        }
        else
        {
            if (TestFlow_System_IsDebug() == 1) return FSM_CHECK_OK;
            else return FSM_CHECK_NG;
        }
    }
    else
    {
        return u8Result;
    }
}

static uint8_t TestFlow_HandCup1CatchReactCupResult(void *pStatus)
{
    uint8_t u8Result = TestFlow_Api_HandCup1CheckResult();
    if (u8Result == FSM_CHECK_OK)
    {
        if (TestFlow_Api_ReactHandCup1IsExist() == 0)
        {
            return FSM_CHECK_OK;
        }
        //抓杯后有杯报错
        else
        {
            //抓杯后有杯底层会报错,不会执行到这里
            // TestFlow_WarningReport(TESTFLOW_WARNING_HANDCUP1_REACCUP_EXIST_AFTER_CATCH,NULL,0);
            return FSM_CHECK_NG;                     
        }
    }
    else
    {
        return u8Result;
    }
}

static uint8_t g_u8GarbageNO = 1;
static int32_t TestFlow_HandCup1NewCupCatchInit(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    return TestFlow_Api_HandCup1ToNewDisk(pu8Param[0], pu8Param[1]);
}

static int32_t TestFlow_HandCup1NewCupCatch(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    return TestFlow_Api_HandCup1CatchNewDiskCup();
}

static int32_t TestFlow_HandCup1NewCupToReact(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{    
    return TestFlow_Api_HandCup1ToReact();
}

static int32_t TestFlow_HandCup1NewCupPut(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP1_PUT_REACT, TESTFLOW_UNIT_HANDCUP1, NULL, 0);
    return TestFlow_Api_HandCup1PutReactCup();
}

static uint8_t TestFlow_HandCup1NewCupCheckNewDiskResult(void *pStatus)
{
    uint8_t u8CheckResult = TestFlow_Api_HandCup1CheckResult();
    if (u8CheckResult == FSM_CHECK_OK)
    {
        if (((TESTFLOW_STATUS_T *)pStatus)->eCupStackState == TESTFLOW_CUPSTACK_STATE_UP || ((TESTFLOW_STATUS_T *)pStatus)->eCupStackState == TESTFLOW_CUPSTACK_STATE_READY)
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
        return u8CheckResult;
    }
}

static uint8_t TestFlow_HandCup1NewCupToReactCheckResult(void *pStatus)
{
    uint8_t u8CheckResult = TestFlow_Api_HandCup1CheckResult();
    if (u8CheckResult == FSM_CHECK_OK)
    {
        if (((TESTFLOW_STATUS_T *)pStatus)->eReactState == TESTFLOW_REACT_STATE_1_STOP)
        {
            if (TestFlow_Api_ReactHandCup1IsExist() == 1)
            {
                // 放杯前检测到有反应杯
                TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP1_NEWCUP_ABANDON, TESTFLOW_UNIT_HANDCUP1, NULL, 0);
                return FSM_CHECK_BRANCH2;
            }
            else
            {
                return FSM_CHECK_OK;
            }
        }
        else
        {
            return FSM_CHECK_WAIT;
        }
    }
    else
    {
        return u8CheckResult;
    }
}

static void TestFlow_HandCup1NewCupCatchOK(uint8_t u8Offset)
{
    //DebugMsg("HandCup1NewCupCatchOK\r\n");
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP1_NEWCUP_CATCH_OK, TESTFLOW_UNIT_HANDCUP1, NULL, 0);
}

static void TestFlow_HandCup1NewCupPutOK(uint8_t u8Offset)
{
    //DebugMsg("HandCup1NewCupPushOK\r\n");
//    uint8_t u8CupID = 0;
    
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP1_NEWCUP_PUT_OK, TESTFLOW_UNIT_HANDCUP1, NULL, 0);
}

static void TestFlow_HandCup1NewCupAbandonOK(uint8_t u8Offset)
{
	TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP1_NEWCUP_ABANDON_OK, TESTFLOW_UNIT_HANDCUP1, &g_u8GarbageNO, 1);
}

static void TestFlow_HandCup1NewCupCatchNG(uint8_t u8Offset)
{
    DebugMsg("W>HandCup1NewCupCatchNG:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP1_NEWCUP_CATCH_NG, TESTFLOW_UNIT_HANDCUP1, NULL, 0);
}

static void TestFlow_HandCup1NewCupCatchTimeout(uint8_t u8Offset)
{
    DebugMsg("E>HandCup1NewCupCatchTimeout:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP1_NEWCUP_CATCH_TIMEOUT, TESTFLOW_UNIT_HANDCUP1, NULL, 0);
}

static int32_t TestFlow_HandCup1NewCupAbandonToGarbge(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
	DebugMsg("NewCupAbandon:ghj,%d\r\n", u8Size);
	if (u8Size < 3) return TESTFLOW_RESULT_NG;
	g_u8GarbageNO = pu8Param[2];
    return TestFlow_Api_HandCup1ToGarbage(g_u8GarbageNO);  // 移动
}

static int32_t TestFlow_HandCup1NewCupAbandonPutGarbge(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    return TestFlow_Api_HandCup1PutGarbageCup(g_u8GarbageNO);  // 丢弃
}
//抓杯流程
const FSM_STATE_T g_stWorkHandCup1NewCupCatchTable[] = 
{
    /* State 0 */       {1,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup1NewCupCatchInit,       TestFlow_HandCup1NewCupCheckNewDiskResult,       NULL,                                  TestFlow_HandCup1NewCupCatchNG,            TestFlow_HandCup1NewCupCatchTimeout },
    /* State 1 */       {2,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup1NewCupCatch,           TestFlow_HandCup1CheckResult,                    TestFlow_HandCup1NewCupCatchOK,        TestFlow_HandCup1NewCupCatchNG,            TestFlow_HandCup1NewCupCatchTimeout },
    /* State 2 */       {3,                     FSM_STATE_FINISH,       4,       				0,      30000,      TestFlow_HandCup1NewCupToReact,         TestFlow_HandCup1NewCupToReactCheckResult,       NULL,                                  TestFlow_HandCup1NewCupCatchNG,            TestFlow_HandCup1NewCupCatchTimeout },
	/* State 3 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup1NewCupPut,             TestFlow_HandCup1CupPutNewCupResult,             TestFlow_HandCup1NewCupPutOK,          TestFlow_HandCup1NewCupCatchNG,            TestFlow_HandCup1NewCupCatchTimeout },
	/* State 4 */       {5,      				FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup1NewCupAbandonToGarbge, TestFlow_HandCup1CheckResult,             		 NULL,          						TestFlow_HandCup1NewCupCatchNG,            TestFlow_HandCup1NewCupCatchTimeout },
	/* State 5 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup1NewCupAbandonPutGarbge,TestFlow_HandCup1CheckResult,                    TestFlow_HandCup1NewCupAbandonOK,      TestFlow_HandCup1NewCupCatchNG,            TestFlow_HandCup1NewCupCatchTimeout },
};

static int32_t TestFlow_HandCup1CupAbandonInit(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    if (u8Size < 1) return TESTFLOW_RESULT_NG;
    g_u8GarbageNO = pu8Param[0];
    return TestFlow_Api_HandCup1ToReact();
}

static uint8_t TestFlow_HandCup1AbandonCupWaitReactResult(void *pStatus)
{
    uint8_t u8Result = TestFlow_Api_HandCup1CheckResult();
    if (u8Result == FSM_CHECK_OK)
    {
        if (((TESTFLOW_STATUS_T *)pStatus)->eReactState == TESTFLOW_REACT_STATE_2_STOP)
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

static int32_t TestFlow_HandCup1CupAbandonCatchReact(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP1_CUP_ABANDON_CATCH, TESTFLOW_UNIT_HANDCUP1, NULL, 0);
    return TestFlow_Api_HandCup1CatchReactCup();
}

static int32_t TestFlow_HandCup1CupAbandonToGarbge(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    return TestFlow_Api_HandCup1ToGarbage(g_u8GarbageNO);  // 移动
}

static int32_t TestFlow_HandCup1CupAbandonPutGarbge(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    return TestFlow_Api_HandCup1PutGarbageCup(g_u8GarbageNO);  // 丢弃
}
/*
static int32_t TestFlow_HandCup1CupAbandonToNewDisk(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    if (pu8Param == NULL && u8Size < 2)
    {
        return TESTFLOW_RESULT_ERR_PARAM;
    }
    return TestFlow_Api_HandCup1ToNewDisk(pu8Param[0], pu8Param[1]);
}
*/

static void TestFlow_HandCup1CupAbandonCatchOK(uint8_t u8Offset)
{
    //DebugMsg("HandCup1CupAbandonCatchOK\r\n");
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP1_CUP_ABANDON_CATCH_OK, TESTFLOW_UNIT_HANDCUP1, NULL, 0);
}

static void TestFlow_HandCup1CupAbandonOK(uint8_t u8Offset)
{
    uint8_t u8TxBuf[2];
    u8TxBuf[0] = g_u8GarbageNO;
    u8TxBuf[1] = 0;//主流程
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP1_CUP_ABANDON_OK, TESTFLOW_UNIT_HANDCUP1, u8TxBuf, 2);
}

static void TestFlow_HandCup1CupAbandonNG(uint8_t u8Offset)
{
    DebugMsg("E>HandCup1CupAbandonNG:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP1_CUP_ABANDON_NG, TESTFLOW_UNIT_HANDCUP1, NULL, 0);
}

static void TestFlow_HandCup1CupAbandonTimeout(uint8_t u8Offset)
{
    DebugMsg("E>HandCup1CupAbandonTimeout:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_HANDCUP1_CUP_ABANDON_TIMEOUT, TESTFLOW_UNIT_HANDCUP1, NULL, 0);
}

const FSM_STATE_T g_stWorkHandCup1CupAbandonTable[] = 
{
    /* State 0 */       {1,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup1CupAbandonInit,        TestFlow_HandCup1AbandonCupWaitReactResult,      NULL,                                  TestFlow_HandCup1CupAbandonNG,             TestFlow_HandCup1CupAbandonTimeout },
    /* State 1 */       {2,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup1CupAbandonCatchReact,  TestFlow_HandCup1CatchReactCupResult,            TestFlow_HandCup1CupAbandonCatchOK,    TestFlow_HandCup1CupAbandonNG,             TestFlow_HandCup1CupAbandonTimeout },
    /* State 2 */       {3,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup1CupAbandonToGarbge,    TestFlow_HandCup1CheckResult,                    NULL,                                  TestFlow_HandCup1CupAbandonNG,             TestFlow_HandCup1CupAbandonTimeout },
    /* State 3 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_HandCup1CupAbandonPutGarbge,   TestFlow_HandCup1CheckResult,                    TestFlow_HandCup1CupAbandonOK,         TestFlow_HandCup1CupAbandonNG,             TestFlow_HandCup1CupAbandonTimeout },
};


