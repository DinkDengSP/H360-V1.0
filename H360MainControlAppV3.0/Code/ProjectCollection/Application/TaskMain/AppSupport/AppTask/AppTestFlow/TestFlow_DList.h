/***********************************************************************
 *
 * (c) Copyright 2001-2016, XXXX, All Rights Reserved.
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF XXXX, INC.
 * The copyright notice above does not evidence any actual or intended
 * publication of such source code. 
 *
 *  Subsystem:   TestFlow
 *  File:        TestFlow_dlist.h
 *  Author:      WWZHOU
 *  Description: Double List for C header files.
 *********************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TESTFLOW_DLIST_H__
#define __TESTFLOW_DLIST_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include "os.h"
#include "TestFlow_Typedef.h"

typedef struct _DNode
{
    void * pData;                     // 节点存储的数据地址
    struct _DNode *prev;              // 指向上一个节点
    struct _DNode *next;              // 指向下一个节点
}TESTFLOW_DNODE_T;

typedef struct _DList
{
    uint32_t u32Size;                  // 链表的大小
    TESTFLOW_DNODE_T *pstHead;              // 存储链表的第一个节点
    TESTFLOW_DNODE_T *pstTail;              // 存储链表的最后一个节点
}TESTFLOW_DLIST_T;

extern int32_t TestFlow_DListModuleInit(void);
extern int32_t TestFlow_DListInit(TESTFLOW_DLIST_T *pstList);
extern int32_t TestFlow_DListInsertHead(TESTFLOW_DLIST_T *pstList, void *pData);
extern int32_t TestFlow_DListInsertTail(TESTFLOW_DLIST_T *pstList, void *pData);
extern TESTFLOW_DNODE_T *TestFlow_DListFindNodeByData(TESTFLOW_DLIST_T *pstList, void *pData);
extern int32_t TestFlow_DListDeleteNode(TESTFLOW_DLIST_T *pstList, TESTFLOW_DNODE_T *pstNode);

#ifdef __cplusplus
}
#endif

#endif /* __TESTFLOW_DLIST_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
