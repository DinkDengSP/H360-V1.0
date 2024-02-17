/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-01-04 15:12:49
**LastEditors: DengXiaoJun
**LastEditTime: 2021-01-04 15:22:46
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "UserMemManage.h"
#include "os.h"

//设置内存块指定值
void UserMemSet(void *s,uint8_t c,uint32_t count)
{
    uint8_t *xs = s; 
    if(count == 0)
        return;
    while(count--)
		*xs++=c;
}

//拷贝内存块
void UserMemCopy(void *des,void *src,uint32_t n)
{
    uint8_t *xdes=des;
	uint8_t *xsrc=src;
    if(n == 0)
        return;
    while(n--)
		*xdes++=*xsrc++; 
}

//函数指针类型定义
typedef void (*UserMemInitFuncPtr)();
typedef void* (*UserMemMallocFuncPtr)(uint32_t size);
typedef void (*UserMemFreeFuncPtr)(void* ptr);
typedef void* (*UserMemReallocFuncPtr)(void *ptr,uint32_t size);
typedef uint8_t (*UserMemPerUsedFuncPtr)(void);


/*******************************MAIN************************************/
//内存池,可分配内存用一个大数组保存 
static uint8_t sramMAIN_Base[MAX_SIZE_MAIN];					
//内存管理表,用于管理维护内存池的状态
static uint16_t sramMAIN_MapBase[ALLOC_TABLE_SIZE_MAIN];
//当前内存区域的内存管理是否就绪	
static uint8_t sramMAIN_MemReady = 0;

//初始化内存块
void UserMemInitMemMAIN(void)
{
    UserMemSet(sramMAIN_Base,0,MAX_SIZE_MAIN);
    UserMemSet(sramMAIN_MapBase,0,ALLOC_TABLE_SIZE_MAIN*2);
	//内存管理初始化OK 
	sramMAIN_MemReady = 1;
}
//申请内存
void* UserMemMallocMemMAIN(uint32_t size)
{
    int32_t offset = 0;
    //需要的内存块数
    uint16_t nmemb;
    //连续空内存块数
    uint16_t cmemb = 0;
    //计数器
    uint16_t i = 0;
    if(sramMAIN_MemReady == 0)
    {
        UserMemInitMemMAIN();
    }
    //不需要分配
    if(size == 0)
    {
        return (void*)NULL;
    }
    //获取需要分配的连续内存块数
    nmemb = size / BLOCK_SIZE_MAIN;
    if (size % BLOCK_SIZE_MAIN)
    {
        //只能多不能少
        nmemb++;
    }
    //搜索整个内存控制区
    for (offset = ALLOC_TABLE_SIZE_MAIN - 1; offset >= 0; offset--)
    {
        if(0 == sramMAIN_MapBase[offset])
        {
            //本块内存未使用
            cmemb++;
        }
        else
        {
            //已经使用,连续内存块清零
            cmemb = 0;
        }
        if(cmemb == nmemb)
        {
            //已经得到指定数量的连续内存块
            //标注内存块非空
            for (i = 0; i < nmemb; i++)
            {
                //使用块数量标注,这样,在释放的时候就能连续释放比较快
                //例如标注为5,释放,就直接释放当前块以及之后的4个块
                sramMAIN_MapBase[offset + i] = nmemb;
            }
            offset = offset * BLOCK_SIZE_MAIN;
            return (void*)(sramMAIN_Base + offset);
        }
    }
    //没有找到连续内存块支持
    return (void*)NULL;
}
//释放内存
void UserMemFreeMemMAIN(void* ptr)
{
    uint32_t offset;
    int i = 0;
    offset=(uint32_t)ptr-(uint32_t)sramMAIN_Base;
    if(sramMAIN_MemReady == 0)
    {
        UserMemInitMemMAIN();
    }
    if(offset < MAX_SIZE_MAIN)
    {
        //偏移所在内存块号码
        int index = offset/BLOCK_SIZE_MAIN;	
		//内存块数量		
        int nmemb = sramMAIN_MapBase[index];	
		//内存块清零
        for(i=0;i<nmemb;i++)  						
        {  
            sramMAIN_MapBase[index+i]=0;  
        }  
    }
    else
    {
        //超区间
    }
}
//内存块扩容
void* UserMemReallocMemMAIN(void *ptr,uint32_t size)
{
    uint8_t* newPtr = UserMemMallocMemMAIN(size);
    if(newPtr != NULL)
    {
        UserMemCopy(newPtr,ptr,size);
        UserMemFreeMemMAIN(ptr);
        return (void*)newPtr;
    }
    else
    {
        return (void*)NULL;
    }
}
//获取空间使用率
uint8_t UserMemPerUsedMemMAIN(void)
{
    uint32_t used = 0;
    uint32_t i = 0;
    for(i=0;i< ALLOC_TABLE_SIZE_MAIN;i++)  
    {  
        if(sramMAIN_MapBase[i])
			used++; 
    }
    return (used*100)/(ALLOC_TABLE_SIZE_MAIN); 
}


/******************************函数指针数组**********************************/
//内存初始化函数指针数组
static const UserMemInitFuncPtr UserMemInitFuncPtrArray[] = {
    UserMemInitMemMAIN,
};
//内存申请函数指针数组
static const UserMemMallocFuncPtr UserMemMallocFuncPtrArray[] = {
    UserMemMallocMemMAIN,
};
//内存释放函数指针数组
static const UserMemFreeFuncPtr UserMemFreeFuncPtrArray[] = {
    UserMemFreeMemMAIN,
};
//内存重新扩容函数指针数组
static const UserMemReallocFuncPtr UserMemReallocFuncPtrArray[] = {
    UserMemReallocMemMAIN,
};
//内存使用率计算函数指针数组
static const UserMemPerUsedFuncPtr UserMemPerUsedFuncPtrArray[] = {
    UserMemPerUsedMemMAIN,
};


//内存管理互斥信号量
static OS_MUTEX mutexUserMemManage;
static uint8_t mutexInitFlag = 0;

//内存管理初始化
void UserMemInit(USER_MEM_REGION region)
{
    OS_ERR err;
    //判断内存区域是否支持
        if(region > MEM_REGION_MAX)return;
    //内存管理初始化
        UserMemInitFuncPtrArray[region]();
    //互斥信号量初始化
        if(mutexInitFlag == 0)
        {
            mutexInitFlag = 1;
            //初始化互斥信号量
            OSMutexCreate((OS_MUTEX*	)&mutexUserMemManage,
                            (CPU_CHAR*	)"mutexUserMemManage",
                            (OS_ERR*		)&err);
        }
}

//申请内存
void* UserMemMalloc(USER_MEM_REGION region,uint32_t size)
{
    OS_ERR err;
    void* targetPtr;
    //判断内存区域是否支持
        if(region > MEM_REGION_MAX)return NULL;
    //申请互斥信号量
	    if (OSRunning)OSMutexPend(&mutexUserMemManage, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //申请内存
        targetPtr = UserMemMallocFuncPtrArray[region](size);
    //申请来的内存清空
        if(targetPtr != NULL)
        {
            UserMemSet(targetPtr,0,size);
        }
    //释放互斥信号量
	    if (OSRunning)OSMutexPost(&mutexUserMemManage, OS_OPT_POST_FIFO, &err);
    return targetPtr;
}

//释放内存
void UserMemFree(USER_MEM_REGION region,void* ptr)
{
    OS_ERR err;
    //判断内存区域是否支持
        if(region > MEM_REGION_MAX)return;
    //申请互斥信号量
	    if (OSRunning)OSMutexPend(&mutexUserMemManage, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //释放内存
        UserMemFreeFuncPtrArray[region](ptr);
    //释放互斥信号量
	    if (OSRunning)OSMutexPost(&mutexUserMemManage, OS_OPT_POST_FIFO, &err);
}

//重新申请内存
void* UserMemRealloc(USER_MEM_REGION region,void *ptr,uint32_t size)
{
    OS_ERR err;
    void* targetPtr;
    //检测内存区域是否支持
        if(region > MEM_REGION_MAX)return (void*)NULL;
    //申请互斥信号量
	    if (OSRunning)OSMutexPend(&mutexUserMemManage, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //重新申请内存
        targetPtr = UserMemReallocFuncPtrArray[region](ptr,size);
    //申请来的内存清空
        if(targetPtr != NULL)
        {
            UserMemSet(targetPtr,0,size);
        }
    //释放互斥信号量
	    if (OSRunning)OSMutexPost(&mutexUserMemManage, OS_OPT_POST_FIFO, &err);
    return targetPtr;
}

//获取当前动态内存使用率
uint8_t UserMemPerUsed(USER_MEM_REGION region)
{
    OS_ERR err;
    uint8_t used;
    //识别内存区域是否支持
        if(region > MEM_REGION_MAX)return 0;
    //申请互斥信号量
	    if (OSRunning)OSMutexPend(&mutexUserMemManage, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    //计算内存使用率
        used = UserMemPerUsedFuncPtrArray[region]();
    //释放互斥信号量
	    if (OSRunning)OSMutexPost(&mutexUserMemManage, OS_OPT_POST_FIFO, &err);
    return used;
}









