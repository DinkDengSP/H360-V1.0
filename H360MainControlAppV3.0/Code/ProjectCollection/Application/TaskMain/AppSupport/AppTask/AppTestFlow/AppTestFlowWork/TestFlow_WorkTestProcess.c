#include "TestFlow_Config.h"
#include "TestFlow_Engine.h"
#include "TestFlow_Warning.h"
#include "TestFlow_WorkTestProcess.h"
#include "TestFlow_WorkTable.h"

static int32_t TestFlow_WorkTestProcessStep1Init(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    TESTFLOW_STATUS_T *pstStatus = pStatus;
    uint8_t u8CupID = 0;

    if (pstStatus->eReactState == TESTFLOW_REACT_STATE_1_RUN)
    {
        u8CupID = TestFlow_H1CupID(pstStatus->u8H1CupID, TESTFLOW_CELL_REACT_ROTATE_1);
    }
    else if(pstStatus->eReactState == TESTFLOW_REACT_STATE_1_STOP)
    {
        u8CupID = pstStatus->u8H1CupID;
    }
    else
    {
        
    }    
    DebugMsg("TP_1:%d\r\n", u8CupID);
    
    return TESTFLOW_RESULT_OK;
}

static uint8_t TestFlow_WorkTestProcessStep1Result(void *pStatus)
{    
    TESTFLOW_STATUS_T *pstStatus = pStatus;
    if (pstStatus->eReactState == TESTFLOW_REACT_STATE_2_RUN || pstStatus->eReactState == TESTFLOW_REACT_STATE_2_STOP)
    {
        return FSM_CHECK_OK;
    }
    else
    {
        return FSM_CHECK_WAIT;
    }
}

static void TestFlow_WorkTestProcessStep1Timeout(uint8_t u8Offset)
{
    //uint8_t u8Result = 2;
    DebugMsg("TestProcessStep1Timeout\r\n");
    //TestFlow_SendMsg(TESTFLOW_MSG_CMD_RESET, TESTFLOW_UNIT_TESTFLOW, &u8Result, 1);
}

static int32_t TestFlow_WorkTestProcessStep2Init(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    TESTFLOW_STATUS_T *pstStatus = pStatus;
    uint8_t u8CupID;

    if (pstStatus->eReactState == TESTFLOW_REACT_STATE_2_RUN)
    {
        u8CupID = TestFlow_H1CupID(pstStatus->u8H1CupID, TESTFLOW_CELL_REACT_ROTATE_2);
    }
    else if(pstStatus->eReactState == TESTFLOW_REACT_STATE_2_STOP)
    {
        u8CupID = pstStatus->u8H1CupID;
    }
    else
    {
        
    }

    DebugMsg("TP_2:%d\r\n", u8CupID);

    return TESTFLOW_RESULT_OK;
}

static uint8_t TestFlow_WorkTestProcessStep2Result(void *pStatus)
{    
    TESTFLOW_STATUS_T *pstStatus = pStatus;
    if (pstStatus->eReactState == TESTFLOW_REACT_STATE_3_RUN || pstStatus->eReactState == TESTFLOW_REACT_STATE_3_STOP)
    {
        return FSM_CHECK_OK;
    }
    else
    {
        return FSM_CHECK_WAIT;
    }
}

static void TestFlow_WorkTestProcessStep2Timeout(uint8_t u8Offset)
{
    //uint8_t u8Result = 2;
    DebugMsg("TestProcessStep2Timeout\r\n");
    //TestFlow_SendMsg(TESTFLOW_MSG_CMD_RESET, TESTFLOW_UNIT_TESTFLOW, &u8Result, 1);
}

static int32_t TestFlow_WorkTestProcessStep3Init(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    TESTFLOW_STATUS_T *pstStatus = pStatus;
    uint8_t u8CupID;

    if (pstStatus->eReactState == TESTFLOW_REACT_STATE_3_RUN)
    {
        u8CupID = TestFlow_H1CupID(pstStatus->u8H1CupID, TESTFLOW_CELL_REACT_ROTATE_3);
    }
    else if(pstStatus->eReactState == TESTFLOW_REACT_STATE_3_STOP)
    {
        u8CupID = pstStatus->u8H1CupID;
    }
    else
    {
        
    }

    DebugMsg("TP_3:%d\r\n", u8CupID);
    
    return TESTFLOW_RESULT_OK;
}

static uint8_t TestFlow_WorkTestProcessStep3Result(void *pStatus)
{    
    TESTFLOW_STATUS_T *pstStatus = pStatus;

    
    if (pstStatus->eReactState == TESTFLOW_REACT_STATE_3_RUN || pstStatus->eReactState == TESTFLOW_REACT_STATE_3_STOP)
    {
        return FSM_CHECK_OK;
    }
    else
    {
        return FSM_CHECK_WAIT;
    }
}

static void TestFlow_WorkTestProcessStep3Timeout(uint8_t u8Offset)
{
    //uint8_t u8Result = 2;
    DebugMsg("TestProcessStep3Timeout\r\n");
    //TestFlow_SendMsg(TESTFLOW_MSG_CMD_RESET, TESTFLOW_UNIT_TESTFLOW, &u8Result, 1);
}

static int32_t TestFlow_WorkTestProcessStep4Init(void *pStatus, uint8_t *pu8Param, uint8_t u8Size)
{
    TESTFLOW_STATUS_T *pstStatus = pStatus;
    uint8_t u8CupID;

    if (pstStatus->eReactState == TESTFLOW_REACT_STATE_4_RUN)
    {
        u8CupID = TestFlow_H1CupID(pstStatus->u8H1CupID, TESTFLOW_CELL_REACT_ROTATE_4);
    }
    else if(pstStatus->eReactState == TESTFLOW_REACT_STATE_4_STOP)
    {
        u8CupID = pstStatus->u8H1CupID;
    }
    else
    {
        
    }

    DebugMsg("TP_4:%d\r\n", u8CupID);

    return TESTFLOW_RESULT_OK;
}

static uint8_t TestFlow_WorkTestProcessStep4Result(void *pStatus)
{    
    TESTFLOW_STATUS_T *pstStatus = pStatus;
    if (pstStatus->eReactState == TESTFLOW_REACT_STATE_1_RUN || pstStatus->eReactState == TESTFLOW_REACT_STATE_1_STOP)
    {
        return FSM_CHECK_OK;
    }
    else
    {
        return FSM_CHECK_WAIT;
    }
}

static void TestFlow_WorkTestProcessStep4Timeout(uint8_t u8Offset)
{
    //uint8_t u8Result = 2;
    DebugMsg("TestProcessStep4Timeout\r\n");
    //TestFlow_SendMsg(TESTFLOW_MSG_CMD_RESET, TESTFLOW_UNIT_TESTFLOW, &u8Result, 1);
}

const FSM_STATE_T g_stWorkTestProcessTable[] = 
{
    /* State 0 */       {1,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      10000,      TestFlow_WorkTestProcessStep1Init,        TestFlow_WorkTestProcessStep1Result,       NULL,       NULL,          TestFlow_WorkTestProcessStep1Timeout },
    /* State 1 */       {2,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      10000,      TestFlow_WorkTestProcessStep2Init,        TestFlow_WorkTestProcessStep2Result,       NULL,       NULL,          TestFlow_WorkTestProcessStep2Timeout },
    /* State 2 */       {3,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      10000,      TestFlow_WorkTestProcessStep3Init,        TestFlow_WorkTestProcessStep3Result,       NULL,       NULL,          TestFlow_WorkTestProcessStep3Timeout },
    /* State 3 */       {0,                     FSM_STATE_FINISH,       FSM_STATE_FINISH,       0,      10000,      TestFlow_WorkTestProcessStep4Init,        TestFlow_WorkTestProcessStep4Result,       NULL,       NULL,          TestFlow_WorkTestProcessStep4Timeout },
};


