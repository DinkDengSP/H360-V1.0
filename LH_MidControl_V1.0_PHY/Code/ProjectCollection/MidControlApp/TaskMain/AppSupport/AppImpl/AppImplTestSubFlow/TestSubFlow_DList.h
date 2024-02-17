/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: ZWT
**Date: 2021-03-09 11:03:59
**LastEditors: ZWT
**LastEditTime: 2021-03-09 14:36:57
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#ifndef __TESTFLOW_DLIST_H__
#define __TESTFLOW_DLIST_H__

//#ifdef __cplusplus
// extern "C" {
//#endif
#include <stdint.h>
#include "AppIpcBase.h"
#include "CoreUtil.h"
#include "TestSubFlow_Typedef.h"


typedef struct LNode
{ 
	void const *pData;				// 数据
	struct LNode *prev;			// 前指针
	struct LNode *next;			// 后指针
}TESTSUBFLOW_LNODE;



typedef struct List
{
	uint32_t u32Len;			// 链表长度
	TESTSUBFLOW_LNODE *pstHead;				// 头指针
	TESTSUBFLOW_LNODE *pstTail;				// 尾指针
}TESTSUBFLOW_LIST;


//#include "TestSubFlow_Typedef.h"




//#ifdef __cplusplus
//}
//#endif
//分配内存池
uint32_t TestSubFlow_CreatNodePool();

// 分配链表存储空间
extern int32_t TestSubFlow_DListModuleInit(void);

// 链表初始化
extern int32_t TestSubFlow_DListInit(TESTSUBFLOW_LIST *pstList);

// 新建节点
extern TESTSUBFLOW_LNODE * TestSubFlow_DListAllocNode(void const *pData);

// 链表前插入新节点
extern int32_t TestSubFlow_DListInsertHead(TESTSUBFLOW_LIST *pstList, void const * pData);
// 链表后插入新节点
extern int32_t TestSubFlow_DListInsertTail(TESTSUBFLOW_LIST *pstList, void const * pData);
// 查找节点
extern TESTSUBFLOW_LNODE *TestSubFlow_DListFindNodeByData(TESTSUBFLOW_LIST *pstList, void *pData);

// 删除节点,放回池
extern int32_t TestSubFlow_DListDeleteNode(TESTSUBFLOW_LIST *pstList, TESTSUBFLOW_LNODE *pstNode);

// 删除节点，释放内存
extern int32_t TestSubFlow_DListDeleteNodeFreeMem(TESTSUBFLOW_LIST *pstList, TESTSUBFLOW_LNODE *pstNode);


#endif /* __TESTFLOW_DLIST_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

