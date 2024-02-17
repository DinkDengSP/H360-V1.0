/******************************************************************
 * (c) Copyright 2018-2025, XXXX, All Rights Reserved.
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF XXXX, INC.
 * The copyright notice above does not evidence any actual or intended
 * publication of such source code. 
 *
 *  Subsystem:   TestFlow
 *  File:        TestFlow_dlist.c
 *  Author:      WWZHOU
 *  Description: Double List for C source files.
 *
 *  Function List:  
 *     1. ....
 *  History:        
 *                  
 *     1. Date:
 *        Author:
 *        Modification:
 *     2. ...
 *  
 ***************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "TestFlow_DList.h"

#define TESTFLOW_DLIST_NODE_SIZE                 (sizeof(TESTFLOW_DNODE_T))
#define TESTFLOW_DLIST_NODE_MAX                  (10000u)

uint8_t *g_pu8DListNodeMemBuf;
static OS_MEM g_stDListNodeMemPool;

/****************************************************************************
 *
 * Function Name:    TestFlow_dlistModuleInit
 * Input:            None
 *                   
 * Output:           None
 *                   
 * Returns:          TESTFLOW_RESULT_OK,TESTFLOW_RESULT_NG
 * Description:      alloc mem pool for double list module.
 * Note:             You have to call it once after Reset.
 ***************************************************************************/
int32_t TestFlow_DListModuleInit(void)
{
    OS_ERR os_err;

    g_pu8DListNodeMemBuf = TestFlow_AllocMemory(TESTFLOW_DLIST_NODE_MAX*TESTFLOW_DLIST_NODE_SIZE);
    if (g_pu8DListNodeMemBuf != NULL)
    {
        OSMemCreate(&g_stDListNodeMemPool, "OSMem_DListNode", g_pu8DListNodeMemBuf, 
                    TESTFLOW_DLIST_NODE_MAX, TESTFLOW_DLIST_NODE_SIZE, &os_err);
        if (os_err != OS_ERR_NONE)
        {
            DebugMsg("ERR>DListNodeMemPool:%d\r\n", os_err);
            return TESTFLOW_RESULT_NG;		
        }
    }
    else
    {
        DebugMsg("ERR>DListBuf\r\n");
    }

    return TESTFLOW_RESULT_OK;	
}

/****************************************************************************
 *
 * Function Name:    TestFlow_DListInit
 * Input:            pstList - double list for initialize;
 *                   
 * Output:           None
 *                   
 * Returns:          TESTFLOW_RESULT_OK,TESTFLOW_RESULT_ERR_PARAM
 * Description:      Initialize one double list.
 * Note:             
 ***************************************************************************/
int32_t TestFlow_DListInit(TESTFLOW_DLIST_T *pstList)
{
    if (pstList == NULL)
    {
        return TESTFLOW_RESULT_ERR_PARAM;
    }
    pstList->u32Size = 0;
    pstList->pstHead = NULL;
    pstList->pstTail = NULL;

    return TESTFLOW_RESULT_OK;	
}

/****************************************************************************
 *
 * Function Name:    TestFlow_DListAllocNode
 * Input:            pData - Node Data;
 *                   
 * Output:           None
 *                   
 * Returns:          new node
 * Description:      Alloc One Double List Node From Memary Pool.
 * Note:             
 ***************************************************************************/
static TESTFLOW_DNODE_T * TestFlow_DListAllocNode(void *pData)
{
    OS_ERR os_err;
    TESTFLOW_DNODE_T *pstNewNode;
	
    pstNewNode = (TESTFLOW_DNODE_T *)OSMemGet(&g_stDListNodeMemPool, &os_err);
    if (pstNewNode == NULL)
    {
        return NULL;
    }
    else
    {
        pstNewNode->next = NULL;
        pstNewNode->prev = NULL;
        pstNewNode->pData = pData;
        return pstNewNode;		
    }
}

/****************************************************************************
 *
 * Function Name:    TestFlow_DListInsertHead
 * Input:            pstList - The head of double list;
 *                   pData - list data
 * Output:           None
 *                   
 * Returns:          TESTFLOW_RESULT_OK,TESTFLOW_RESULT_NG,TESTFLOW_RESULT_ERR_PARAM
 * Description:      Get a new node, insert to pstList->next.
 * Note:             Double List Had a head, new Node insert to Head->next
 ***************************************************************************/
int32_t TestFlow_DListInsertHead(TESTFLOW_DLIST_T *pstList, void * pData)
{
    TESTFLOW_DNODE_T *pstNewNode;

    if (pstList == NULL)
    {
        return TESTFLOW_RESULT_ERR_PARAM;
    }
	
    pstNewNode = TestFlow_DListAllocNode(pData);	
    if (pstNewNode == NULL)	
    {
        return TESTFLOW_RESULT_NG;
    }
    else
    {
        if (pstList->u32Size == 0)
        {
            pstList->pstHead = pstNewNode;
            pstList->pstTail = pstNewNode;
            pstNewNode->next = NULL;
            pstNewNode->prev = NULL;
        }
        else
        {
            pstNewNode->prev = NULL;
			if (pstList->pstHead != NULL)
            {
                pstList->pstHead->prev = pstNewNode;
            }
            else
            {
                DebugMsg("E>DL_InsertHead\r\n");
            }
			pstNewNode->next = pstList->pstHead;
            pstList->pstHead = pstNewNode;
        }
        pstList->u32Size++;		
    }

    return TESTFLOW_RESULT_OK;
}

/****************************************************************************
 *
 * Function Name:    TestFlow_DListInsertTail
 * Input:            pstList - The head of double list;
 *                   pData - list data
 * Output:           None
 *                   
 * Returns:          TESTFLOW_RESULT_OK,TESTFLOW_RESULT_NG
 * Description:      Get a new node, insert to the tail of the dlist.
 * Note:             Double List Had a head, new Node insert to Head->next
 ***************************************************************************/
int32_t TestFlow_DListInsertTail(TESTFLOW_DLIST_T *pstList, void *pData)
{
    TESTFLOW_DNODE_T *pstNewNode;

    if (pstList == NULL)
    {
        return TESTFLOW_RESULT_ERR_PARAM;
    }
	
    pstNewNode = TestFlow_DListAllocNode(pData);	
    if (pstNewNode == NULL)	
    {
        return TESTFLOW_RESULT_NG;
    }
    else
    {
        if (pstList->u32Size == 0)
        {
            pstList->pstHead = pstNewNode;
            pstList->pstTail = pstNewNode;
            pstNewNode->next = NULL;
            pstNewNode->prev = NULL;
        }
        else
        {
            pstNewNode->next = NULL;
            if (pstList->pstTail != NULL)
            {
                pstList->pstTail->next = pstNewNode;
            }
            else
            {
                DebugMsg("E>DL_InsertHead\r\n");
            }
            pstNewNode->prev = pstList->pstTail;
            pstList->pstTail = pstNewNode;
        }
        pstList->u32Size++;
    }

    return TESTFLOW_RESULT_OK;
}

/****************************************************************************
 *
 * Function Name:    TestFlow_DListFindNodeByData
 * Input:            pstList - The double list;
 *                   pData - the node of pData
 * Output:           None
 *                   
 * Returns:          TESTFLOW_RESULT_OK,TESTFLOW_RESULT_ERR_PARAM
 * Description:      find one node which ->pData == pData.
 * Note:             
 ***************************************************************************/
TESTFLOW_DNODE_T *TestFlow_DListFindNodeByData(TESTFLOW_DLIST_T *pstList, void *pData)
{
    TESTFLOW_DNODE_T *pstNodeTmp;
    uint8_t u8FindNodeOk = FALSE;

    if (pstList == NULL || pData == NULL)
    {
        return NULL;
    }
    pstNodeTmp = pstList->pstHead;
    while(NULL != pstNodeTmp)
    {
        if(pstNodeTmp->pData == pData)
        {
            u8FindNodeOk = TRUE;
            break;
        }
        pstNodeTmp = pstNodeTmp->next;
    }

    return (u8FindNodeOk == FALSE ? NULL : pstNodeTmp);
}

/****************************************************************************
 *
 * Function Name:    TestFlow_DListDeleteNode
 * Input:            pstList - The double list;
 *                   pstNode - the node of to delete
 * Output:           None
 *                   
 * Returns:          TESTFLOW_RESULT_OK,TESTFLOW_RESULT_ERR_PARAM
 * Description:      delete node from dlist.
 * Note:             
 ***************************************************************************/
int32_t TestFlow_DListDeleteNode(TESTFLOW_DLIST_T *pstList, TESTFLOW_DNODE_T *pstNode)
{
    OS_ERR os_err;

    if (pstList == NULL || pstNode == NULL)
    {
        return TESTFLOW_RESULT_ERR_PARAM;
    }

    if (pstNode->prev == NULL && pstNode->next == NULL)
    {
        pstList->pstHead = NULL;
        pstList->pstTail = NULL;
    }
    else if (pstNode->prev == NULL)    //删除的是头部节点
    {
        pstList->pstHead = pstNode->next;
        pstList->pstHead->prev = NULL;
    }
    else if (pstNode->next == NULL)    //删除的是尾部节点
    {
        pstList->pstTail = pstNode->prev;
        pstList->pstTail->next = NULL;
    }
    else                          //删除的是中间节点
    {
        pstNode->next->prev = pstNode->prev;
        pstNode->prev->next = pstNode->next;
    }
    pstList->u32Size--; 	

    OSMemPut(&g_stDListNodeMemPool, pstNode, &os_err);
    if(os_err != OS_ERR_NONE)
    {
         DebugMsg("E>DListFree:%d\r\n", os_err);
    }
	
    return TESTFLOW_RESULT_OK;	
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
