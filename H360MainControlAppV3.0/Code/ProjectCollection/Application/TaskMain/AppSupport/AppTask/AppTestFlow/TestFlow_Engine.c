#include "TestFlow_Config.h"
#include "TestFlow_Engine.h"

static FSM_RECODER_T *pstFSMachineRecoder;//@".ccram";

static uint8_t  g_u8FSM_Offset;//@".ccram";
static uint8_t  g_u8FSM_State;//@".ccram";
static uint32_t g_u32FSM_TimeStart;//@".ccram";
//static uint32_t g_u32FSM_TimeOut;
//static FSM_STATE_T *g_pstFSMachine;

extern FSM_STATE_T *pstWorkTable[];

void TestFlow_FSMWorkRegister(FSM_RECODER_T *pstRecoderTable)
{
    pstFSMachineRecoder = pstRecoderTable;
}

void TestFlow_ShowFSM(uint32_t u32Work, uint8_t u8Offset)
{
    //DebugMsg("Work %d State:%d\r\n", u32Work, g_u8FSM_State);
}

void TestFlow_FSMEngine(uint32_t u32SystemTick)
{
    uint32_t u32WorkID = 0;
    int32_t  s32Result = 0;
    
    while(1)
    {
        const FSM_STATE_T *pstFSMachine = pstWorkTable[u32WorkID];
        FSM_RECODER_T *pstRecoder = pstFSMachineRecoder+u32WorkID;
        //DebugMsg("  FSM:%d,%p,%p\r\n", u32WorkID, pstFSMachine, pstRecoder);
        if (pstFSMachine == NULL || pstRecoder == NULL)
        {
            break;
        }
        if (pstRecoder->u8Enable == TRUE)
        {
            g_u32FSM_TimeStart = pstRecoder->u32TimeStart;
            g_u8FSM_Offset = pstRecoder->u8Offset;
            g_u8FSM_State = pstRecoder->u8StateFSM;

            //DebugMsg("  FSM_1:%d,%p,%p\r\n", u32WorkID, pstFSMachine, pstRecoder);
            if (g_u8FSM_State&FSM_STATE_IDLE)  // Chech Result & Timeout
            {
                if ((pstFSMachine+g_u8FSM_Offset)->pCheck_Arbiter != NULL)
                {
                    uint8_t u8ArbiterResult = (pstFSMachine+g_u8FSM_Offset)->pCheck_Arbiter(pstRecoder->pStatus);
                    //if (u8ArbiterResult != FSM_CHECK_WAIT) DebugMsg("FSM_Arbiter %d,%d\r\n", u8ArbiterResult, (pstFSMachine+g_u8FSM_Offset)->u32TimeOut);
                    if (u8ArbiterResult == FSM_CHECK_OK) // OK
                    {
                        if ((pstFSMachine+g_u8FSM_Offset)->pCheck_OK != NULL)
                        {
                            (pstFSMachine+g_u8FSM_Offset)->pCheck_OK(g_u8FSM_Offset);
                        }
                        g_u8FSM_State = (pstFSMachine+g_u8FSM_Offset)->u8NextState;
                        g_u32FSM_TimeStart = TESTFLOW_INVALID_TIME;
                    }
                    else if (u8ArbiterResult == FSM_CHECK_BRANCH1)
                    {
                        if ((pstFSMachine+g_u8FSM_Offset)->pCheck_OK != NULL)
                        {
                            (pstFSMachine+g_u8FSM_Offset)->pCheck_OK(g_u8FSM_Offset);
                        }
                        g_u8FSM_State = (pstFSMachine+g_u8FSM_Offset)->u8Branch1State;
                        g_u32FSM_TimeStart = TESTFLOW_INVALID_TIME;
                    }
                    else if (u8ArbiterResult == FSM_CHECK_BRANCH2)
                    {
                        g_u8FSM_State = (pstFSMachine+g_u8FSM_Offset)->u8Branch2State;
                        g_u32FSM_TimeStart = TESTFLOW_INVALID_TIME;
                    }
                    else if (u8ArbiterResult == FSM_CHECK_NG)  // NG
                    {
                        if ((pstFSMachine+g_u8FSM_Offset)->pCheck_NG != NULL)
                        {
                            (pstFSMachine+g_u8FSM_Offset)->pCheck_NG(g_u8FSM_Offset);
                        }
                        g_u8FSM_State = FSM_STATE_FINISH;
                        g_u32FSM_TimeStart = TESTFLOW_INVALID_TIME;
                    }
                    else
                    {
                        if (u32SystemTick > g_u32FSM_TimeStart && (u32SystemTick - g_u32FSM_TimeStart) > (pstFSMachine+g_u8FSM_Offset)->u32TimeOut)
                        {
                            if ((pstFSMachine+g_u8FSM_Offset)->pCheck_TimeOut != NULL)
                            {
                                (pstFSMachine+g_u8FSM_Offset)->pCheck_TimeOut(g_u8FSM_Offset);
                            }
                            g_u8FSM_State = FSM_STATE_FINISH;
                            g_u32FSM_TimeStart = TESTFLOW_INVALID_TIME;
                            //DebugMsg("WorkTimeout:%d,%d,%d,%d\r\n", u32WorkID, g_u8FSM_Offset, u32SystemTick, g_u32FSM_TimeStart);
                        }
                    }

                    // Check Finih
                    if (g_u8FSM_State == FSM_STATE_FINISH)
                    {
                        //DebugMsg("WorkFinish:%d,%d,%d\r\n", u32WorkID, g_u8FSM_Offset, u8ArbiterResult);
                        g_u32FSM_TimeStart = TESTFLOW_INVALID_TIME;
                        pstRecoder->u8Offset = 0;
                        pstRecoder->u8Enable = FALSE;
                        pstRecoder->u8StateFSM = FSM_STATE_FINISH;
                        pstRecoder->u8ParamSize = 0;
                        TestFlow_MemClr(pstRecoder->u8Param, sizeof(pstRecoder->u8Param));
                    }
                }
                else
                {
                    DebugMsg("E>FsmProc_Arbiter:%p,%d\r\n", pstFSMachine, g_u8FSM_Offset);
                }
            }
            else
            {
                g_u8FSM_Offset = g_u8FSM_State;
                TestFlow_ShowFSM(u32WorkID, g_u8FSM_Offset); // Display Message
                g_u8FSM_State |= FSM_STATE_IDLE;
                g_u32FSM_TimeStart = u32SystemTick;
                s32Result = (pstFSMachine+g_u8FSM_Offset)->pStateInitial(pstRecoder->pStatus, pstRecoder->u8Param, pstRecoder->u8ParamSize);
                //DebugMsg("  FSM:%d,%d,%d\r\n", u32WorkID, u32SystemTick, s32Result);
                if (s32Result < 0)
                {
                    if ((pstFSMachine+g_u8FSM_Offset)->pCheck_NG != NULL)
                    {
                        (pstFSMachine+g_u8FSM_Offset)->pCheck_NG(g_u8FSM_Offset);
                    }
                    g_u32FSM_TimeStart = TESTFLOW_INVALID_TIME;
                    g_u8FSM_Offset = 0;
                    g_u8FSM_State = FSM_STATE_FINISH;
                    pstRecoder->u8Offset = 0;
                    pstRecoder->u8Enable = FALSE;
                    pstRecoder->u8StateFSM = FSM_STATE_FINISH;
                    pstRecoder->u8ParamSize = 0;
                    //TestFlow_MemClr(pstRecoder->u8Param, sizeof(pstRecoder->u8Param));
                }
            }

            pstRecoder->u32TimeStart = g_u32FSM_TimeStart;
            pstRecoder->u8Offset = g_u8FSM_Offset;
            pstRecoder->u8StateFSM = g_u8FSM_State;
        }
        u32WorkID ++;
    }
}



