/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-12-11 19:43:10
**LastEditors: DengXiaoJun
**LastEditTime: 2020-12-11 20:32:26
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "ADT_DList.h"
#include "UserMemManage.h"

//单个节点内存空间尺寸
#define ADT_DLIST_NODE_SIZE                         (sizeof(ADT_DNODE_T))
//内存申请获取新节点的接口
#define ADT_DLIST_NodeAlloc()                       COMM_MemAlloc(ADT_DLIST_NODE_SIZE)
//内存释放节点的接口
#define ADT_DLIST_NodeFree(ptr)                     COMM_MemFree(ptr)

//初始化链表结构体
int32_t ADT_DListInit(ADT_DLIST_T *pstList)
{
    if (NULL == pstList)
    {
        //空指针异常
        return -1;
    }
    pstList->u32Size = 0;
    pstList->pstHead = NULL;
    pstList->pstTail = NULL;

    return 0;	
}

//为数据指针动态申请一个新的节点
static ADT_DNODE_T * ADT_DListAllocNode(void *pData)
{
    //定义一个指针
    ADT_DNODE_T* pstNewNode = NULL;
	//申请内存获得新的节点
    pstNewNode = (ADT_DNODE_T *)ADT_DLIST_NodeAlloc();
    if (pstNewNode != NULL)
    {
        //申请成功
        pstNewNode->next  = NULL;
        pstNewNode->prev  = NULL;
        //将申请到的节点的数据指针指向需要的数据指针
        pstNewNode->pData = pData;
        //返回节点
        return pstNewNode;		
    }
    else
    {
        //申请失败
        return NULL;
    }
}

//将一个新数据插入链表头部
int32_t ADT_DListInsertHead(ADT_DLIST_T *pstList, void * pData)
{
    //空节点指针
    ADT_DNODE_T *pstNewNode = NULL;
    //首先链表地址不能为空
    if (NULL == pstList)
    {
        return -1;
    }
	//为数据申请节点
    pstNewNode = ADT_DListAllocNode(pData);	
    if (NULL == pstNewNode)	
    {
        //内存申请失败
        return -2;
    }
    else
    {
        if (pstList->u32Size == 0)
        {
            //节点里面还没有任何数据,将链表的头指针尾指针都指向新生成的节点
            pstList->pstHead = pstNewNode;
            pstList->pstTail = pstNewNode;
            //节点的头指针尾指针都为空
            pstNewNode->next = NULL;
            pstNewNode->prev = NULL;
        }
        else
        {
            //节点里面已经存在数据,因为是插入头部,所以把节点的前向指针设置为空
            pstNewNode->prev = NULL;
            //之前的链表头节点的前向指针肯定是空,这时候之前的头节点的前向指针不再为空了,因为新
            //插入的节点到了头节点的前面,所以将头节点前向指针指向新插入的节点
			pstList->pstHead->prev = pstNewNode;
            //新插入的节点的后向指针指向原来的头节点
			pstNewNode->next = pstList->pstHead;
            //这个时候就可以将链表的头节点设置为新的节点了
            pstList->pstHead = pstNewNode;
        }
        //链表长度增加
        pstList->u32Size++;		
    }
    //返回完成
    return 0;
}

//将数据插入链表尾部
int32_t ADT_DListInsertTail(ADT_DLIST_T *pstList, void *pData)
{
    //新生成的节点
    ADT_DNODE_T *pstNewNode = NULL;
    //链表指针为空异常
    if (NULL == pstList)
    {
        return -1;
    }
	//申请节点内存
    pstNewNode = ADT_DListAllocNode(pData);
    if (NULL == pstNewNode)	
    {
        return -2;
    }
    else
    {
        //链表开始不存在数据
        if (pstList->u32Size == 0)
        {
            //头节点和尾节点都指向新的节点
            pstList->pstHead = pstNewNode;
            pstList->pstTail = pstNewNode;
            //节点的头尾都为空
            pstNewNode->next = NULL;
            pstNewNode->prev = NULL;
        }
        else
        {
            //链表已经有数据了,因为是插入尾部,所以新生成的节点的尾部肯定为空
            pstNewNode->next = NULL;
            //链表原先尾部节点指向的下一个节点的指针设置为新生成的节点
            pstList->pstTail->next = pstNewNode;
            //新生成的节点的前向节点指向原先链表的尾部节点
            pstNewNode->prev = pstList->pstTail;
            //此刻可以将链表的尾节点指向新生成的节点了
            pstList->pstTail = pstNewNode;
        }
        //链表长度增加
        pstList->u32Size++;		
    }
    return 0;
}

//从链表中查询指定的数据
ADT_DNODE_T *ADT_DListFindNodeByData(ADT_DLIST_T *pstList, void *pData)
{
    //遍历节点的临时缓存
    ADT_DNODE_T *pstNodeTmp;
	uint8_t findSuccessFlag = 0;
    //查询的数据和链表的指针不能为空
    if (NULL == pstList || NULL == pData)
    {
        return NULL;
    }
    //头节点遍历
    pstNodeTmp = pstList->pstHead;
    //直到查询到尾部
    while(NULL != pstNodeTmp)
    {
        //查看节点指针是否对的上
        if(pstNodeTmp->pData == pData)
        {
            //对的上,退出查找,并设置找到了的标志位
            findSuccessFlag = 1;
            break;
        }
        //每查询完一个,转移到下一个
        pstNodeTmp = pstNodeTmp->next;
    }
    //查询到了返回当前指向的节点,否则没查询到返回空
    return (findSuccessFlag == 0 ? NULL : pstNodeTmp);
}

//从链表中删除指定节点
int32_t ADT_DListDeleteNode(ADT_DLIST_T *pstList, ADT_DNODE_T *pstNode)
{
    //节点和链表都不能为空
    if (NULL == pstList || NULL == pstNode)
    {
        return -1;
    }
    if((NULL == pstNode->prev)&&(NULL == pstNode->next))
    {
        //该节点是链表中的唯一节点,那就相当于清除整个链表
        pstList->pstHead = NULL;
        pstList->pstTail = NULL;
    }
    else if(NULL == pstNode->prev)    
    {
        //删除的是头部节点
        //链表的头部节点指向原先头部节点的下一个节点
        pstList->pstHead = pstNode->next;
        //原先头部节点的下一个节点的前向节点设置为空,这样就标注成为了新的头部节点
        pstList->pstHead->prev = NULL;		
    }
    else if(NULL == pstNode->next)    
    {
        //删除的是尾部节点
        //链表的尾部节点指向原先尾部节点的上一个节点
        pstList->pstTail = pstNode->prev;
        //原先尾部节点的指向的下一个节点设置为空,这样就标注了节点成为了新的尾部节点
        pstList->pstTail->next = NULL;		
    }
    else                          
    {
        //删除的是中间节点
        //当前节点的下一个节点的上一个节点不再指向当前节点,而是指向当前节点的上一个节点
        pstNode->next->prev = pstNode->prev;
        //原先节点的上一个节点的下一个节点不再指向当前节点,而是只想当前节点的下一个节点
        //这样就把当前节点和链表的关系脱离
        pstNode->prev->next = pstNode->next;
    }
    //链表数量-1
    pstList->u32Size--;
    //链表指向的数据指针设置为空
    pstNode->pData = NULL;
    //释放节点本身的内存
    ADT_DLIST_NodeFree(pstNode);
	
    return 0;	
}
