#include "UserMemManage.h"
#include "UserMemManageConfig.h"
#include "os.h"
#include "bget.h"

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

/*******************************内部内存****************************************/
//内部内存
//内部SRAM内存池 
static uint8_t sramIN_Base[SRAM_IN_MAX_SIZE];					
//内存管理表,内部SRAM内存池MAP
static uint16_t sramIN_MapBase[SRAM_IN_ALLOC_TABLE_SIZE];
//当前内存管理是否就绪	
static uint8_t sramIN_MemReady = 0;

//内部内存初始化
void UserMemInitSramIN(void)
{
    UserMemSet(sramIN_Base,0,SRAM_IN_MAX_SIZE);
    UserMemSet(sramIN_MapBase,0,SRAM_IN_ALLOC_TABLE_SIZE*2);
	//内存管理初始化OK 
	sramIN_MemReady = 1;
}

//申请内存
void* UserMemMallocSramIN(uint32_t size)
{
    int32_t offset = 0;
    //需要的内存块数
    uint16_t nmemb;
    //连续空内存块数
    uint16_t cmemb = 0;
    //计数器
    uint16_t i = 0;
    if(sramIN_MemReady == 0)
    {
        UserMemInitSramIN();
    }
    //不需要分配
    if(size == 0)
    {
        return (void*)NULL;
    }
    //获取需要分配的连续内存块数
    nmemb = size / SRAM_IN_BLOCK_SIZE;
    if (size % SRAM_IN_BLOCK_SIZE)
    {
        //只能多不能少
        nmemb++;
    }
    //搜索整个内存控制区
    for (offset = SRAM_IN_ALLOC_TABLE_SIZE - 1; offset >= 0; offset--)
    {
        if(0 == sramIN_MapBase[offset])
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
                sramIN_MapBase[offset + i] = nmemb;
            }
            offset = offset * SRAM_IN_BLOCK_SIZE;
            return (void*)(sramIN_Base + offset);
        }
    }
    //没有找到连续内存块支持
    return (void*)NULL;
}

//释放内存
void UserMemFreeSramIN(void* ptr)
{
    uint32_t offset;
    int i = 0;
    offset=(uint32_t)ptr-(uint32_t)sramIN_Base;
    if(sramIN_MemReady == 0)
    {
        UserMemInitSramIN();
    }
    if(offset < SRAM_IN_MAX_SIZE)
    {
        //偏移所在内存块号码
        int index = offset/SRAM_IN_BLOCK_SIZE;	
		//内存块数量		
        int nmemb = sramIN_MapBase[index];	
		//内存块清零
        for(i=0;i<nmemb;i++)  						
        {  
            sramIN_MapBase[index+i]=0;  
        }  
    }
    else
    {
        //超区间
    }
}

//重新申请内存
void* UserMemReallocSramIN(void *ptr,uint32_t size)
{
    uint8_t* newPtr = UserMemMallocSramIN(size);
    if(newPtr != NULL)
    {
        UserMemCopy(newPtr,ptr,size);
        UserMemFreeSramIN(ptr);
        return (void*)newPtr;
    }
    else
    {
        return (void*)NULL;
    }
}

//获取当前动态内存使用率
uint8_t UserMemPerUsedSramIN(void)
{   
    uint32_t used = 0;
    uint32_t i = 0;
    for(i=0;i< SRAM_IN_ALLOC_TABLE_SIZE;i++)  
    {  
        if(sramIN_MapBase[i])
			used++; 
    }
    return (used*100)/(SRAM_IN_ALLOC_TABLE_SIZE); 
}
/*******************************内部内存****************************************/


/*******************************CCM内存****************************************/
//CCM内存
//内部SRAM CCM内存池 
static uint8_t sramCCM_Base[SRAM_CCM_MAX_SIZE]@".ccram";					
//内存管理表,内部SRAM CCM内存池MAP
static uint16_t sramCCM_MapBase[SRAM_CCM_ALLOC_TABLE_SIZE]@".ccram";
//当前内存管理是否就绪	
static uint8_t sramCCM_MemReady = 0;

//CCM内存初始化
void UserMemInitSramCCM(void)
{
    UserMemSet(sramCCM_Base,0,SRAM_CCM_MAX_SIZE);
    UserMemSet(sramCCM_MapBase,0,SRAM_CCM_ALLOC_TABLE_SIZE*2);
	//内存管理初始化OK 
	sramCCM_MemReady = 1;
}

//申请内存
void* UserMemMallocSramCCM(uint32_t size)
{
    int32_t offset = 0;
    //需要的内存块数
    uint16_t nmemb;
    //连续空内存块数
    uint16_t cmemb = 0;
    //计数器
    uint16_t i = 0;
    if(sramCCM_MemReady == 0)
    {
        UserMemInitSramCCM();
    }
    //不需要分配
    if(size == 0)
    {
        return (void*)NULL;
    }
    //获取需要分配的连续内存块数
    nmemb = size / SRAM_CCM_BLOCK_SIZE;
    if (size % SRAM_CCM_BLOCK_SIZE)
    {
        //只能多不能少
        nmemb++;
    }
    //搜索整个内存控制区
    for (offset = SRAM_CCM_ALLOC_TABLE_SIZE - 1; offset >= 0; offset--)
    {
        if(0 == sramCCM_MapBase[offset])
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
                sramCCM_MapBase[offset + i] = nmemb;
            }
            offset = offset * SRAM_CCM_BLOCK_SIZE;
            return (void*)(sramCCM_Base + offset);
        }
    }
    //没有找到连续内存块支持
    return (void*)NULL;
}

//释放内存
void UserMemFreeSramCCM(void* ptr)
{
    uint32_t offset;
    int i = 0;
    offset=(uint32_t)ptr-(uint32_t)sramCCM_Base;
    if(sramCCM_MemReady == 0)
    {
        UserMemInitSramCCM();
    }
    if(offset < SRAM_CCM_MAX_SIZE)
    {
        //偏移所在内存块号码
        int index = offset/SRAM_CCM_BLOCK_SIZE;	
		//内存块数量		
        int nmemb = sramCCM_MapBase[index];	
		//内存块清零
        for(i=0;i<nmemb;i++)  						
        {  
            sramCCM_MapBase[index+i]=0;  
        }  
    }
    else
    {
        //超区间
    }
}

//重新申请内存
void* UserMemReallocSramCCM(void *ptr,uint32_t size)
{
    uint8_t* newPtr = UserMemMallocSramCCM(size);
    if(newPtr != NULL)
    {
        UserMemCopy(newPtr,ptr,size);
        UserMemFreeSramCCM(ptr);
        return (void*)newPtr;
    }
    else
    {
        return (void*)NULL;
    }
}

//获取当前动态内存使用率
uint8_t UserMemPerUsedSramCCM(void)
{   
    uint32_t used = 0;
    uint32_t i = 0;
    for(i=0;i< SRAM_CCM_ALLOC_TABLE_SIZE;i++)  
    {  
        if(sramCCM_MapBase[i])
			used++; 
    }
    return (used*100)/(SRAM_CCM_ALLOC_TABLE_SIZE); 
}
/*******************************CCM内存****************************************/



/********************************IS62内存*************************************/
//IS62内存
static uint8_t sramIS62_Base[SRAM_IS62_MAX_SIZE]@".is62";					
//当前内存管理是否就绪	
static uint8_t sramIS62_MemReady = 0;

//is60内存初始化
void UserMemInitSramIS62(void)
{
    UserMemSet(sramIS62_Base,0,SRAM_IS62_MAX_SIZE);
    //初始化堆内存
    bpool(sramIS62_Base,SRAM_IS62_MAX_SIZE);
	//内存管理初始化OK 
	sramIS62_MemReady = 1;
}

//申请内存
void* UserMemMallocSramIS62(uint32_t size)
{
    if(sramIS62_MemReady == 0)
    {
        UserMemInitSramIS62();
    }
    //申请内存
    return bget(size);
}

//释放内存
void UserMemFreeSramIS62(void* ptr)
{
    if(sramIS62_MemReady == 0)
    {
        UserMemInitSramIS62();
    }
    //释放内存
    brel(ptr);
}

//重新申请内存
void* UserMemReallocSramIS62(void *ptr,uint32_t size)
{
    //重新申请
    uint8_t* newPtr = bgetr(ptr,size);
    if(newPtr != NULL)
    {
        //数据拷贝
        UserMemCopy(newPtr,ptr,size);
        //以前的数据释放
        brel(ptr);
        return (void*)newPtr;
    }
    else
    {
        return (void*)NULL;
    }
}

//获取当前动态内存使用率
uint8_t UserMemPerUsedSramIS62(void)
{   
    bufsize util1,util2,util3,util4,util5;
    //获取状态,已经分配的堆和空闲的堆加起来是总的堆大小
    bstats(&util1,&util2,&util3,&util4,&util5);
    //计算空闲
    return (uint8_t)((util1/(util1+util2))*100);
}
/********************************IS62内存*************************************/

static const UserMemInitFuncPtr UserMemInitFuncPtrArray[] = {
    UserMemInitSramIN,
    UserMemInitSramCCM,
    UserMemInitSramIS62,
};

static const UserMemMallocFuncPtr UserMemMallocFuncPtrArray[] = {
    UserMemMallocSramIN,
    UserMemMallocSramCCM,
    UserMemMallocSramIS62,
};

static const UserMemFreeFuncPtr UserMemFreeFuncPtrArray[] = {
    UserMemFreeSramIN,
    UserMemFreeSramCCM,
    UserMemFreeSramIS62,
};

static const UserMemReallocFuncPtr UserMemReallocFuncPtrArray[] = {
    UserMemReallocSramIN,
    UserMemReallocSramCCM,
    UserMemReallocSramIS62,
};

static const UserMemPerUsedFuncPtr UserMemPerUsedFuncPtrArray[] = {
    UserMemPerUsedSramIN,
    UserMemPerUsedSramCCM,
    UserMemPerUsedSramIS62,
};

//内存管理互斥信号量
static OS_MUTEX mutexUserMemManage;
static uint8_t mutexInitFlag = 0;

//动态内存初始化
void UserMemInit(SRAM_LOCATE locate)
{
    OS_ERR err;
    if(locate > SRAM_IS62)
    {
        return;
    }
    UserMemInitFuncPtrArray[locate]();
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
void* UserMemMalloc(SRAM_LOCATE locate,uint32_t size)
{
    OS_ERR err;
    if(locate > SRAM_IS62)
    {
        return (void*)NULL;
    }
    void* targetPtr;
    //申请互斥信号量
	if (OSRunning)OSMutexPend(&mutexUserMemManage, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    targetPtr = UserMemMallocFuncPtrArray[locate](size);
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
void UserMemFree(SRAM_LOCATE locate,void* ptr)
{
    OS_ERR err;
    if(locate > SRAM_IS62)
    {
        return;
    }
    //申请互斥信号量
	if(OSRunning)OSMutexPend(&mutexUserMemManage, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    UserMemFreeFuncPtrArray[locate](ptr);
    //释放互斥信号量
	if(OSRunning)OSMutexPost(&mutexUserMemManage, OS_OPT_POST_FIFO, &err);
}

//重新申请内存
void* UserMemRealloc(SRAM_LOCATE locate,void *ptr,uint32_t size)
{
    OS_ERR err;
    if(locate > SRAM_IS62)
    {
        return (void*)NULL;
    }
    void* targetPtr;
    //申请互斥信号量
	if(OSRunning)OSMutexPend(&mutexUserMemManage, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    targetPtr = UserMemReallocFuncPtrArray[locate](ptr,size);
    //申请来的内存清空
    if(targetPtr != NULL)
    {
        UserMemSet(targetPtr,0,size);
    }
    //释放互斥信号量
	if(OSRunning)OSMutexPost(&mutexUserMemManage, OS_OPT_POST_FIFO, &err);
    return targetPtr;
}

//获取当前动态内存使用率
uint8_t UserMemPerUsed(SRAM_LOCATE locate)
{
    OS_ERR err;
    if(locate > SRAM_IS62)
    {
        return 0;
    }
    uint8_t used;
    //申请互斥信号量
	if(OSRunning)OSMutexPend(&mutexUserMemManage, 0, OS_OPT_PEND_BLOCKING, 0, &err);
    used = UserMemPerUsedFuncPtrArray[locate]();
    //释放互斥信号量
	if(OSRunning)OSMutexPost(&mutexUserMemManage, OS_OPT_POST_FIFO, &err);
    return used;
}


















