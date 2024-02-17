#include "TestFlow_Config.h"
#include "TestFlow_Engine.h"
#include "TestFlow_Warning.h"
#include "TestFlow_PartWorkReagentDisk.h"
#include "TestFlow_ActionAPI.h"

static int32_t TestFlow_ReagentDiskResetInit(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    TestFlow_Api_ReagentDiskReset();
    return TESTFLOW_RESULT_OK;
}

static uint8_t TestFlow_ReagentDiskTrueResult(void *pStatus)
{
    return FSM_CHECK_OK;
}

static uint8_t TestFlow_ReagentDiskCheckResult(void *pStatus)
{
    return TestFlow_Api_ReagentDiskCheckResult();
}

static void TestFlow_ReagentDiskResetCheckOK(uint8_t u8Offset)
{
    DebugMsg("ReagentDiskResetOK\r\n");
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_REAGENTDISK_RESET_OK, TESTFLOW_UNIT_REAGENTDISK, NULL, 0);
}

static void TestFlow_ReagentDiskResetCheckNG(uint8_t u8Offset)
{
    DebugMsg("E>RD_ResetNG:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_REAGENTDISK_RESET_NG, TESTFLOW_UNIT_REAGENTDISK, NULL, 0);
}

static void TestFlow_ReagentDiskResetTimeout(uint8_t u8Offset)
{
    DebugMsg("E>RD_ResetTimeout:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_REAGENTDISK_RESET_TIMEOUT, TESTFLOW_UNIT_REAGENTDISK, NULL, 0);
}

const FSM_STATE_T g_stWorkReagentDiskResetTable[] = 
{
    /* State 0 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_ReagentDiskResetInit,          TestFlow_ReagentDiskCheckResult,       TestFlow_ReagentDiskResetCheckOK,         TestFlow_ReagentDiskResetCheckNG,          TestFlow_ReagentDiskResetTimeout },
};

static int32_t TestFlow_ReagentDiskRotateInit(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    if (pu8Param == NULL || u8Size < 2)
    {
        return TESTFLOW_RESULT_ERR_PARAM;
    }

    return TestFlow_Api_ReagentDiskRotateToPos(pu8Param[0], pu8Param[1]);
}

static void TestFlow_ReagentDiskRotateOK(uint8_t u8Offset)
{
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_REAGENTDISK_ROTATE_OK, TESTFLOW_UNIT_REAGENTDISK, NULL, 0);
}

static void TestFlow_ReagentDiskRotateNG(uint8_t u8Offset)
{
    DebugMsg("E>RD_RotateNG:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_REAGENTDISK_ROTATE_NG, TESTFLOW_UNIT_REAGENTDISK, NULL, 0);
}

static void TestFlow_ReagentDiskRotateTimeout(uint8_t u8Offset)
{
    DebugMsg("E>RD_RotateTimeout:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_REAGENTDISK_ROTATE_TIMEOUT, TESTFLOW_UNIT_REAGENTDISK, NULL, 0);
}

const FSM_STATE_T g_stWorkReagentDiskRotateTable[] = 
{
    /* State 0 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_ReagentDiskRotateInit,         TestFlow_ReagentDiskCheckResult,       TestFlow_ReagentDiskRotateOK,         TestFlow_ReagentDiskRotateNG,          TestFlow_ReagentDiskRotateTimeout },
};

static int32_t TestFlow_ReagentDiskMixInit(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    if (pu8Param == NULL || u8Size < 1)
    {
        return TESTFLOW_RESULT_ERR_PARAM;
    }

    return TestFlow_Api_ReagentDiskRotate(pu8Param[0]);
}

static void TestFlow_ReagentDiskMixOK(uint8_t u8Offset)
{
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_REAGENTDISK_MIX_OK, TESTFLOW_UNIT_REAGENTDISK, NULL, 0);
}

static void TestFlow_ReagentDiskMixNG(uint8_t u8Offset)
{
    DebugMsg("E>RD_MixNG:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_REAGENTDISK_MIX_NG, TESTFLOW_UNIT_REAGENTDISK, NULL, 0);
}

static void TestFlow_ReagentDiskMixTimeout(uint8_t u8Offset)
{
    DebugMsg("E>RD_MixTimeout:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_REAGENTDISK_MIX_TIMEOUT, TESTFLOW_UNIT_REAGENTDISK, NULL, 0);
}

// 磁珠摇匀
const FSM_STATE_T g_stWorkReagentDiskMixTable[] = 
{
    /* State 0 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_ReagentDiskMixInit,         TestFlow_ReagentDiskCheckResult,       TestFlow_ReagentDiskMixOK,         TestFlow_ReagentDiskMixNG,          TestFlow_ReagentDiskMixTimeout },
};

static uint8_t g_u8ReagentDiskMixQuantity = 0;      // 循环摇匀次数,正反算一次摇匀
static uint8_t g_u8ReagentDiskMixStep = 0;          // 正反旋转孔位数
static uint8_t g_u8ReagentDiskMixCount = 0;         // 摇匀计数
static int32_t TestFlow_ReagentDiskMixForTestInit(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    if (pu8Param == NULL || u8Size < 2)
    {
        return TESTFLOW_RESULT_ERR_PARAM;
    }

    g_u8ReagentDiskMixQuantity = pu8Param[0];
    g_u8ReagentDiskMixStep = pu8Param[1];
    g_u8ReagentDiskMixCount = 0;
    DebugMsg("ReagentMix:%d,%d\r\n", g_u8ReagentDiskMixQuantity, g_u8ReagentDiskMixStep);
    return TESTFLOW_RESULT_OK;
}

static int32_t TestFlow_ReagentDiskMixForTestRotate(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    return TestFlow_Api_ReagentDiskRotate(g_u8ReagentDiskMixStep);
}

static int32_t TestFlow_ReagentDiskMixForTestOppositeRotate(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    return TestFlow_Api_ReagentDiskRotate(TESTFLOW_REAGENT_POS_CONFIG-g_u8ReagentDiskMixStep);
}

static uint8_t TestFlow_ReagentDiskMixForTestCheckResult(void *pStatus)
{
    uint8_t  u8Result = TestFlow_Api_ReagentDiskCheckResult();
    if (u8Result == FSM_CHECK_OK)
    {
        g_u8ReagentDiskMixCount ++;
        DebugMsg("RD Mix:%d\r\n", g_u8ReagentDiskMixCount);
        if (g_u8ReagentDiskMixCount >= g_u8ReagentDiskMixQuantity)
        {
            return FSM_CHECK_OK;
        }
        else
        {
            return FSM_CHECK_BRANCH2;
        }
    }
    else
    {
        return u8Result;
    }
}

static void TestFlow_ReagentDiskMixForTestOK(uint8_t u8Offset)
{
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_REAGENTDISK_MIX_FOR_TEST_OK, TESTFLOW_UNIT_REAGENTDISK, NULL, 0);
}

static void TestFlow_ReagentDiskMixForTestNG(uint8_t u8Offset)
{
    DebugMsg("E>RD_MixForTestNG:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_REAGENTDISK_MIX_FOR_TEST_NG, TESTFLOW_UNIT_REAGENTDISK, NULL, 0);
}

static void TestFlow_ReagentDiskMixForTestTimeout(uint8_t u8Offset)
{
    DebugMsg("E>RD_MixForTestTimeout:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_REAGENTDISK_MIX_FOR_TEST_TIMEOUT, TESTFLOW_UNIT_REAGENTDISK, NULL, 0);
}

// 测试前磁珠摇匀
const FSM_STATE_T g_stWorkReagentDiskMixForTestTable[] = 
{
    /* State 0 */       {1,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_ReagentDiskMixForTestInit,             TestFlow_ReagentDiskTrueResult,                NULL,                                     TestFlow_ReagentDiskMixForTestNG,          TestFlow_ReagentDiskMixForTestTimeout },
    /* State 1 */       {2,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_ReagentDiskMixForTestRotate,           TestFlow_ReagentDiskCheckResult,               NULL,                                     TestFlow_ReagentDiskMixForTestNG,          TestFlow_ReagentDiskMixForTestTimeout },
    /* State 2 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       1,                      0,      30000,      TestFlow_ReagentDiskMixForTestOppositeRotate,   TestFlow_ReagentDiskMixForTestCheckResult,     TestFlow_ReagentDiskMixForTestOK,         TestFlow_ReagentDiskMixForTestNG,          TestFlow_ReagentDiskMixForTestTimeout },
};

