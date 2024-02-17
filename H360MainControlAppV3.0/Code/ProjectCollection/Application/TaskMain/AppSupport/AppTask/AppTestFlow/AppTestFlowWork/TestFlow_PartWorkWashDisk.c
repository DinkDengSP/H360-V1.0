#include "TestFlow_Config.h"
#include "TestFlow_Engine.h"
#include "TestFlow_Warning.h"
#include "TestFlow_PartWorkWashDisk.h"
#include "TestFlow_ActionAPI.h"

static uint8_t g_u8WorkMask;

static int32_t TestFlow_WashDiskResetInit(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    DebugMsg("WashDiskResetStart\r\n");
    return TestFlow_Api_WashDiskReset();    
}

static uint8_t TestFlow_WashDiskCheckResult(void *pStatus)
{
    return TestFlow_Api_WashDiskCheckResult();
}

static void TestFlow_WashDiskResetCheckOK(uint8_t u8Offset)
{
    DebugMsg("WashDiskResetOK\r\n");
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_WASHDISK_RESET_OK, TESTFLOW_UNIT_WASHDISK, NULL, 0);
}

static void TestFlow_WashDiskResetCheckNG(uint8_t u8Offset)
{
    DebugMsg("WashDiskResetNG\r\n");
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_WASHDISK_RESET_NG, TESTFLOW_UNIT_WASHDISK, NULL, 0);
}

static void TestFlow_WashDiskResetTimeout(uint8_t u8Offset)
{
    DebugMsg("WashDiskResetTimeout\r\n");
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_WASHDISK_RESET_TIMEOUT, TESTFLOW_UNIT_WASHDISK, NULL, 0);
}


const FSM_STATE_T g_stWorkWashDiskResetTable[] = 
{
    /* State 1 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_WashDiskResetInit,          TestFlow_WashDiskCheckResult,       TestFlow_WashDiskResetCheckOK,         TestFlow_WashDiskResetCheckNG,          TestFlow_WashDiskResetTimeout },
};

static uint8_t* g_pu8WashDiskRotateWarningInfo = NULL;
static int32_t TestFlow_WashDiskRotateInit(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    if (pu8Param == NULL || u8Size < 61)
    {
        return TESTFLOW_RESULT_ERR_PARAM;
    }
    g_pu8WashDiskRotateWarningInfo = &pu8Param[5];
    //DebugMsg("WashDiskRotateStart\r\n");
    return TestFlow_Api_WashDiskRotate(pu8Param[0]);
}

static int32_t TestFlow_WashDiskMixAndClean(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    int32_t Result= TESTFLOW_RESULT_OK;
    uint8_t  u8MixMask = pu8Param[1];
    //uint8_t  u8TxBuf[4];

    g_u8WorkMask = 0;
    if (pu8Param[1] > 0)
    {
        u8MixMask &= ~0x20;
        Result = TestFlow_Api_WashNeedleClean(pu8Param[1]);
        g_u8WorkMask |= 0x01;
    }
    if (pu8Param[2] > 0)
    {
        if(pu8Param[2] == 1) // Bottle A
        {
            Result = TestFlow_Api_LiquidAInject(1);
            /*
            if (TestFlow_Api_LiquidAGetBottleAQuantity() == TESTFLOW_API_LIQUIDA_CHANGE_LEVEL)
            {
                u8TxBuf[0] = 0;
                u8TxBuf[1] = TestFlow_Api_LiquidAIsBottleAExist();
                u8TxBuf[2] = TestFlow_Api_LiquidAGetBottleBQuantity();
                u8TxBuf[3] = TestFlow_Api_LiquidAIsBottleBExist();
                if (u8TxBuf[2] == 0 || u8TxBuf[3] == 0)
                {
                    TestFlow_WarningReport(TESTFLOW_WARNING_FLOW_LIQUID_A_OUT, &u8TxBuf[0], 4);
                }
                else
                {
                    TestFlow_SendMsg(TESTFLOW_MSG_CMD_WASHDISK_LIQUIDA_A_OUT, TESTFLOW_UNIT_WASHDISK, NULL, 0);
                }
            }
            */
        }
        else if (pu8Param[2] == 2) // Bottle B
        {
            Result = TestFlow_Api_LiquidAInject(2);
            /*
            if (TestFlow_Api_LiquidAGetBottleBQuantity() == TESTFLOW_API_LIQUIDA_CHANGE_LEVEL)
            {
                u8TxBuf[0] = TestFlow_Api_LiquidAGetBottleAQuantity();
                u8TxBuf[1] = TestFlow_Api_LiquidAIsBottleAExist();
                u8TxBuf[2] = 0;
                u8TxBuf[3] = TestFlow_Api_LiquidAIsBottleBExist();
                if (u8TxBuf[0] == 0 || u8TxBuf[1] == 0)
                {
                    TestFlow_WarningReport(TESTFLOW_WARNING_FLOW_LIQUID_A_OUT, &u8TxBuf[0], 4);
                }
                else
                {
                    TestFlow_SendMsg(TESTFLOW_MSG_CMD_WASHDISK_LIQUIDA_B_OUT, TESTFLOW_UNIT_WASHDISK, NULL, 0);
                }
            }
            */
        }
        else
        {
            DebugMsg("E>No LiquidA Bottle\r\n");
            Result = TESTFLOW_RESULT_NG;
        }
        g_u8WorkMask |= 0x02;
    }
    if (pu8Param[3] > 0)
    {
        Result = TestFlow_Api_WashMixAction(pu8Param[4]);
        g_u8WorkMask |= 0x04;
    }
    if (Result == TESTFLOW_RESULT_OK)
    {
        TestFlow_SendMsg(TESTFLOW_MSG_CMD_WASHDISK_CLEANINJECTMIX, TESTFLOW_UNIT_WASHDISK, &g_u8WorkMask, 1);
    }

    return Result;
}

static uint8_t TestFlow_WashDiskMixAndCleanResult(void *pStatus)
{
    uint8_t u8ExeResult;
    if ((g_u8WorkMask&0x01) >0)
    {
        u8ExeResult = TestFlow_Api_WashNeedleCheckResult();
        if (u8ExeResult == FSM_CHECK_OK)
        {
            g_u8WorkMask &= ~0x01;
            TestFlow_SendMsg(TESTFLOW_MSG_CMD_WASHDISK_CLEAN_OK, TESTFLOW_UNIT_WASHDISK, NULL, 0);
        }
        else if (u8ExeResult == FSM_CHECK_NG)
        {
            g_u8WorkMask &= ~0x01;
            TestFlow_SendMsg(TESTFLOW_MSG_CMD_WASHDISK_CLEAN_NG, TESTFLOW_UNIT_WASHDISK, NULL, 0);
        }
    }
    if ((g_u8WorkMask&0x02) >0)
    {
        u8ExeResult = TestFlow_Api_LiquidAActionResult(&g_pu8WashDiskRotateWarningInfo[53], 8);
        if (u8ExeResult == FSM_CHECK_OK)
        {
            g_u8WorkMask &= ~0x02;
            TestFlow_SendMsg(TESTFLOW_MSG_CMD_WASHDISK_INJECT_OK, TESTFLOW_UNIT_WASHDISK, NULL, 0);
        }
        else if (u8ExeResult == FSM_CHECK_NG)
        {
            g_u8WorkMask &= ~0x02;
            TestFlow_SendMsg(TESTFLOW_MSG_CMD_WASHDISK_INJECT_NG, TESTFLOW_UNIT_WASHDISK, NULL, 0);
        }
    }
    if ((g_u8WorkMask&0x04) >0)
    {
        u8ExeResult = TestFlow_Api_WashMixActionResult(&g_pu8WashDiskRotateWarningInfo[61], 8);
        if (u8ExeResult == FSM_CHECK_OK)
        {
            g_u8WorkMask &= ~0x04;
            TestFlow_SendMsg(TESTFLOW_MSG_CMD_WASHDISK_MIX_OK, TESTFLOW_UNIT_WASHDISK, NULL, 0);
        }
        else if (u8ExeResult == FSM_CHECK_NG)
        {
            g_u8WorkMask &= ~0x04;
            TestFlow_SendMsg(TESTFLOW_MSG_CMD_WASHDISK_MIX_NG, TESTFLOW_UNIT_WASHDISK, NULL, 0);
        }
    }

    if (g_u8WorkMask == 0)
    {
        return FSM_CHECK_OK;
    }
    else 
    {
        return FSM_CHECK_WAIT;
    }
}

static void TestFlow_WashDiskRotateOK(uint8_t u8Offset)
{
    //DebugMsg("WashDiskRotateOK\r\n");
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_WASHDISK_ROTATE_OK, TESTFLOW_UNIT_WASHDISK, NULL, 0);
}

static void TestFlow_WashDiskRotateNG(uint8_t u8Offset)
{
    DebugMsg("E>WashDiskRotateNG:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_WASHDISK_ROTATE_NG, TESTFLOW_UNIT_WASHDISK, NULL, 0);
}

static void TestFlow_WashDiskRotateTimeout(uint8_t u8Offset)
{
    DebugMsg("E>WashDiskRotateTimeout:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_WASHDISK_ROTATE_TIMEOUT, TESTFLOW_UNIT_WASHDISK, NULL, 0);
}

static void TestFlow_WashDiskMixAndCleanTimeout(uint8_t u8Offset)
{
    DebugMsg("E>WashDiskMixAndCleanTimeout:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_WASHDISK_MIXANDCLEAN_TIMEOUT, TESTFLOW_UNIT_WASHDISK, NULL, 0);
}

const FSM_STATE_T g_stWorkWashDiskRotateTable[] = 
{
    /* State 0 */       {1,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_WashDiskRotateInit,          TestFlow_WashDiskCheckResult,        TestFlow_WashDiskRotateOK,         TestFlow_WashDiskRotateNG,          TestFlow_WashDiskRotateTimeout },
    /* State 1 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      9500,      	TestFlow_WashDiskMixAndClean,         TestFlow_WashDiskMixAndCleanResult,  NULL,                              TestFlow_WashDiskRotateNG,          TestFlow_WashDiskMixAndCleanTimeout },
};

