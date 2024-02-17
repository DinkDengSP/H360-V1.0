#include "TestFlow_Config.h"
#include "TestFlow_Engine.h"
#include "TestFlow_Warning.h"
#include "TestFlow_PartWorkReact.h"
#include "TestFlow_ActionAPI.h"


int TestFlow_PartWorkReactInit(void)
{    
    return TESTFLOW_RESULT_OK;
}

static uint8_t TestFlow_ReactCheckResult(void *pStatus)
{
    return TestFlow_Api_ReactCheckResult();
}

// Reset
static int32_t TestFlow_ReactResetInit(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    TestFlow_Api_ReactReset();
    
    return TESTFLOW_RESULT_OK;
}

static void TestFlow_ReactResetCheckOK(uint8_t u8Offset)
{
    DebugMsg("ReactResetOK\r\n");
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_REACT_RESET_OK, TESTFLOW_UNIT_REACT, NULL, 0);
}

static void TestFlow_ReactResetCheckNG(uint8_t u8Offset)
{
    DebugMsg("E>ReactResetNG:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_REACT_RESET_NG, TESTFLOW_UNIT_REACT, NULL, 0);
}

static void TestFlow_ReactResetTimeout(uint8_t u8Offset)
{
    DebugMsg("E>ReactResetTimeout:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_REACT_RESET_TIMEOUT, TESTFLOW_UNIT_REACT, NULL, 0);
}


const FSM_STATE_T g_stWorkReactResetTable[] = 
{
    /* State 0 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_ReactResetInit,          TestFlow_ReactCheckResult,       TestFlow_ReactResetCheckOK,         TestFlow_ReactResetCheckNG,          TestFlow_ReactResetTimeout },
};
    
// React Start & Stop
static int32_t TestFlow_ReactStartInit(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    if (pu8Param != NULL)
    {
        DebugMsg("PartReactStart:%d\r\n", pu8Param[0]);
        return TESTFLOW_RESULT_OK;
    }
    else
    {
        DebugMsg("E>PartReactStart\r\n");
        return TESTFLOW_RESULT_ERR_PARAM;
    }
}

static uint8_t TestFlow_ReactStartResult(void *pStatus)
{
    return FSM_CHECK_OK;
}


const FSM_STATE_T g_stWorkReactStartTable[] = 
{
    /* State 0 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_ReactStartInit,          TestFlow_ReactStartResult,       NULL,         NULL,          NULL },
};


// React Rotate
static uint8_t u8RotateQuantity = 0;
static uint8_t u8OppositeRotate = 0;
static int32_t TestFlow_ReactRotateInit(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    int s32Result = TESTFLOW_RESULT_OK;
    
    if (pu8Param == NULL || u8Size < 2)
    {
        DebugMsg("E>ReactRotate:%p,%d\r\n", pu8Param, u8Size);
        return TESTFLOW_RESULT_ERR_PARAM;
    }

    u8RotateQuantity = pu8Param[0];
    u8OppositeRotate = pu8Param[1];
    if (u8RotateQuantity > TESTFLOW_CELL_CUP_REACT_MAX)
    {
        DebugMsg("E>ReactRotateQuantity:%p,%d\r\n", u8RotateQuantity);
        return TESTFLOW_RESULT_ERR_LEN;
    }

    if (u8OppositeRotate == 1) s32Result = TestFlow_Api_ReactOppositeRotate(u8RotateQuantity);
    else s32Result = TestFlow_Api_ReactRotate(u8RotateQuantity);
    if (s32Result != TESTFLOW_RESULT_OK)
    {
        DebugMsg("E>ReactRotateExec:%d\r\n", s32Result);
    }
    
    return TESTFLOW_RESULT_OK;
}

static void TestFlow_ReactRotateCheckOK(uint8_t u8Offset)
{
    TESTFLOW_MSG_CMD_E eCmd = TESTFLOW_MSG_CMD_REACT_ROTATE_NG;
    if (u8RotateQuantity == TESTFLOW_CELL_REACT_ROTATE_1 || (u8OppositeRotate == 1 && u8RotateQuantity == TESTFLOW_CELL_REACT_ROTATE_STEP2_1) ||
		(u8OppositeRotate == 0 && u8RotateQuantity == TESTFLOW_CELL_REACT_ROTATE_STEP1_1) ||
		u8RotateQuantity == TESTFLOW_CELL_REACT_ROTATE_STEP1_2 ||
		(u8OppositeRotate == 0 && u8RotateQuantity == TESTFLOW_CELL_REACT_ROTATE_STEP1_3) ||
		u8RotateQuantity == TESTFLOW_CELL_REACT_ROTATE_STEP1_4)
    {
        eCmd = TESTFLOW_MSG_CMD_REACT_TRIGGER_1_STOP;
    }
    else if (u8RotateQuantity == TESTFLOW_CELL_REACT_ROTATE_2 || (u8OppositeRotate == 1 && u8RotateQuantity == TESTFLOW_CELL_REACT_ROTATE_STEP2_2)||
		u8RotateQuantity == TESTFLOW_CELL_REACT_ROTATE_STEP2_3 || u8RotateQuantity == TESTFLOW_CELL_REACT_ROTATE_STEP2_4)
    {
        eCmd = TESTFLOW_MSG_CMD_REACT_TRIGGER_2_STOP;
    }
    else if (u8RotateQuantity == TESTFLOW_CELL_REACT_ROTATE_3 || u8RotateQuantity == TESTFLOW_CELL_REACT_ROTATE_STEP3_1 ||
		(u8OppositeRotate == 1 && u8RotateQuantity == TESTFLOW_CELL_REACT_ROTATE_STEP3_2) ||
		(u8OppositeRotate == 1 && u8RotateQuantity == TESTFLOW_CELL_REACT_ROTATE_STEP3_3) )
    {
        eCmd = TESTFLOW_MSG_CMD_REACT_TRIGGER_3_STOP;
    }
    else if (u8RotateQuantity == TESTFLOW_CELL_REACT_ROTATE_4 || u8RotateQuantity == TESTFLOW_CELL_REACT_ROTATE_STEP4_1)
    {
        eCmd = TESTFLOW_MSG_CMD_REACT_TRIGGER_4_STOP;
    }
    else
    {
        DebugMsg("E>ReactStop:%d\r\n", u8RotateQuantity);
    }
    DebugMsg("ReactRotateOK:%04X,%d,%d\r\n", eCmd, u8RotateQuantity, u8OppositeRotate);
    TestFlow_SendMsg(eCmd, TESTFLOW_UNIT_REACT, NULL, 0);
}

static void TestFlow_ReactRotateCheckNG(uint8_t u8Offset)
{
    DebugMsg("E>ReactRotateNG:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_REACT_ROTATE_NG, TESTFLOW_UNIT_REACT, NULL, 0);
}

static void TestFlow_ReactRotateTimeout(uint8_t u8Offset)
{
    DebugMsg("E>ReactRotateTimeout:%d\r\n", u8Offset);
    TestFlow_SendMsg(TESTFLOW_MSG_CMD_REACT_ROTATE_NG, TESTFLOW_UNIT_REACT, NULL, 0);
}


const FSM_STATE_T g_stWorkReactRotateTable[] = 
{
    /* State 0 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_ReactRotateInit,          TestFlow_ReactCheckResult,       TestFlow_ReactRotateCheckOK,         TestFlow_ReactRotateCheckNG,          TestFlow_ReactRotateTimeout },
};

uint8_t g_u8MixMask = 0;
static uint8_t g_u8MMixCupID;
static int32_t TestFlow_ReactMixInit(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    if (pu8Param == NULL || u8Size < 1)
    {
        return TESTFLOW_RESULT_ERR_PARAM;
    }

    g_u8MixMask = pu8Param[0];
	g_u8MMixCupID = pu8Param[3];
    if ((g_u8MixMask&0x01) > 0)
    {
        TestFlow_Api_ReagentMixAction(pu8Param[1]);
    }
    if ((g_u8MixMask&0x02) > 0)
    {
        TestFlow_Api_MNeedleMixAction(0);
    }
    
    return TESTFLOW_RESULT_OK;
}

static uint8_t TestFlow_ReactMixResult(void *pStatus)
{
    uint8_t u8Result1 = FSM_CHECK_OK, u8Result2 = FSM_CHECK_OK;
	uint8_t u8TxBuf[4] = {0};
    if ((g_u8MixMask&0x01) > 0)
    {
        u8Result1 = TestFlow_Api_ReagentMixCheckResult();
        if (u8Result1 == FSM_CHECK_OK)
        {
            g_u8MixMask &= ~0x01;
            TestFlow_SendMsg(TESTFLOW_MSG_CMD_REACT_RM_MIX_OK, TESTFLOW_UNIT_REACT, NULL, 0);
        }
        else if (u8Result1 == FSM_CHECK_NG)
        {
            g_u8MixMask &= ~0x01;
            TestFlow_SendMsg(TESTFLOW_MSG_CMD_REACT_RM_MIX_NG, TESTFLOW_UNIT_REACT, NULL, 0);
        }
    }
    if ((g_u8MixMask&0x02) > 0)
    {
    	u8TxBuf[0] = g_u8MMixCupID;
        u8Result2 = TestFlow_Api_MNeedleMixCheckResult();
        if (u8Result2 == FSM_CHECK_OK)
        {
            g_u8MixMask &= ~0x02;
            TestFlow_SendMsg(TESTFLOW_MSG_CMD_REACT_MM_MIX_OK, TESTFLOW_UNIT_REACT, &u8TxBuf[0], 1);
        }
        else if (u8Result2 == FSM_CHECK_NG)
        {
            g_u8MixMask &= ~0x02;
            TestFlow_SendMsg(TESTFLOW_MSG_CMD_REACT_MM_MIX_NG, TESTFLOW_UNIT_REACT, NULL, 0);
        }
    }

    if ((g_u8MixMask & 0x03) == 0) return FSM_CHECK_OK;
    else return FSM_CHECK_WAIT;
}

static void TestFlow_ReactMixTimeout(uint8_t u8Offset)
{
    DebugMsg("E>ReactMixTimeout:%d,%02X\r\n", u8Offset, g_u8MixMask);
    if ((g_u8MixMask&0x01) > 0) TestFlow_SendMsg(TESTFLOW_MSG_CMD_REACT_RM_MIX_TIMEOUT, TESTFLOW_UNIT_REACT, NULL, 0);
    if ((g_u8MixMask&0x02) > 0) TestFlow_SendMsg(TESTFLOW_MSG_CMD_REACT_MM_MIX_TIMEOUT, TESTFLOW_UNIT_REACT, NULL, 0);
}


const FSM_STATE_T g_stWorkReactMixTable[] = 
{
    /* State 0 */       {FSM_STATE_FINISH,      FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      30000,      TestFlow_ReactMixInit,          TestFlow_ReactMixResult,       NULL,         NULL,          TestFlow_ReactMixTimeout },
};

