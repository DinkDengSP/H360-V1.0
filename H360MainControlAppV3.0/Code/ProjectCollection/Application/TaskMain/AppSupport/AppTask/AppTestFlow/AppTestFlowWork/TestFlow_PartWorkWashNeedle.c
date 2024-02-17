#include "TestFlow_Config.h"
#include "TestFlow_Engine.h"
#include "TestFlow_Warning.h"
#include "TestFlow_PartWorkWashNeedle.h"
#include "TestFlow_ActionAPI.h"

static int32_t TestFlow_WashNeedleResetInit(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    TestFlow_Api_WashNeedleReset();
    return TESTFLOW_RESULT_OK;
}

static uint8_t TestFlow_WashNeedleResetResult(void *pStatus)
{
    return TestFlow_Api_WashNeedleCheckResult();
}

static void TestFlow_WashNeedleResetCheckOK(uint8_t u8Offset)
{
    DebugMsg("WashNeedleResetOK\r\n");
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_WASHNEEDLE_RESET_OK, TESTFLOW_UNIT_WASHNEEDLE, NULL, 0);
}

static void TestFlow_WashNeedleResetCheckNG(uint8_t u8Offset)
{
    DebugMsg("WashNeedleResetNG\r\n");
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_WASHNEEDLE_RESET_NG, TESTFLOW_UNIT_WASHNEEDLE, NULL, 0);
}

static void TestFlow_WashNeedleResetTimeout(uint8_t u8Offset)
{
    DebugMsg("WashNeedleResetTimeout\r\n");
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_WASHNEEDLE_RESET_TIMEOUT, TESTFLOW_UNIT_WASHNEEDLE, NULL, 0);
}


const FSM_STATE_T g_stWorkWashNeedleResetTable[] = 
{
    /* State 1 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_WashNeedleResetInit,          TestFlow_WashNeedleResetResult,       TestFlow_WashNeedleResetCheckOK,         TestFlow_WashNeedleResetCheckNG,          TestFlow_WashNeedleResetTimeout },
};


