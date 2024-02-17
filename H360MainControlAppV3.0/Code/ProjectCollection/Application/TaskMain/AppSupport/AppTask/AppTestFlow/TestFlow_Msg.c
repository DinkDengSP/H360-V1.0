#include "TestFlow_Config.h"
#include "TestFlow_Msg.h"
#include "TestFlow_Result.h"

extern OS_TCB tcbTaskAppTest;

int32_t TestFlow_SendMsg(TESTFLOW_MSG_CMD_E eCmd, TESTFLOW_UNIT_E eSrcUnit, const uint8_t *pu8Param, uint16_t u16Size)
{
    int32_t s32Result = TESTFLOW_RESULT_OK;
    OS_ERR err = OS_ERR_NONE;
    TESTFLOW_MSM_T *pstMsg = TestFlow_AllocMemory(sizeof(TESTFLOW_MSM_T));
    
    if (pstMsg != NULL)
    {
        pstMsg->eCommand = eCmd;
        pstMsg->eSrcUnit = eSrcUnit;
        pstMsg->eDestUnit = TESTFLOW_UNIT_TESTFLOW;
        pstMsg->u16DataSize = u16Size;
        if (u16Size <= sizeof(pstMsg->u8Data))
        {
            TestFlow_MemCopy(pstMsg->u8Data, pu8Param, u16Size);
            pstMsg->pData = NULL;
        }
        else
        {
            pstMsg->pData = TestFlow_AllocMemory(u16Size);
            if (pstMsg->pData != NULL)
            {
                TestFlow_MemCopy(pstMsg->pData, pu8Param, u16Size);
            }
            else
            {
                s32Result = TESTFLOW_RESULT_ERR_MEM;
            }
        }
    }
    
    if (s32Result == TESTFLOW_RESULT_OK)
    {
        OSTaskQPost((OS_TCB *)&tcbTaskAppTest,
                    (void *)pstMsg,
                    (OS_MSG_SIZE)sizeof(TESTFLOW_MSM_T),
                    (OS_OPT)OS_OPT_POST_FIFO,
                    (OS_ERR *)&err);
        if (err != OS_ERR_NONE)
        {
            s32Result = TESTFLOW_RESULT_NG;
        }
    }
    
    if (s32Result != TESTFLOW_RESULT_OK)
    {
        //uint8_t u8BufTmp[4];
        DebugMsg("E>%d SendMsgNG:%d,%X,%d\r\n", eSrcUnit, s32Result, eCmd, err);
        //u8BufTmp[0] = eSrcUnit;
        //u8BufTmp[1] = (uint8_t)(0 - s32Result);
        //*(uint16_t *)&u8BufTmp[2] = eCmd;
        //TestProc_ReportWarning(&STATUS_system, TESTPROC_WARNING_DEBUG1_SEND_MSG, u8BufTmp, 4);
    }
    
    return s32Result;
}

