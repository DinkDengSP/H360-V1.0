/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-07-28 15:17:54
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-05 20:26:23
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "UserMemManage.h"

//函数指针类型定义
typedef void (*UserMemInitFuncPtr)();
typedef void* (*UserMemMallocFuncPtr)(uint32_t size);
typedef void (*UserMemFreeFuncPtr)(void* ptr);
typedef uint8_t (*UserMemPerUsedFuncPtr)(void);

/*******************************AXI_SRAM************************************/
//内存池,可分配内存用一个大数组保存 
static uint8_t sramAXI_SRAM_Base[MAX_SIZE_AXI_SRAM]@".axi_sram";					
//内存管理表,用于管理维护内存池的状态
static uint16_t sramAXI_SRAM_MapBase[ALLOC_TABLE_SIZE_AXI_SRAM]@".axi_sram";
//当前内存区域的内存管理是否就绪	
static uint8_t sramAXI_SRAM_MemReady = 0;

//初始化内存块
void UserMemInitMemAxiSram(void)
{
    UserMemSet(sramAXI_SRAM_Base,0,MAX_SIZE_AXI_SRAM);
    UserMemSet(sramAXI_SRAM_MapBase,0,ALLOC_TABLE_SIZE_AXI_SRAM*2);
	//内存管理初始化OK 
	sramAXI_SRAM_MemReady = 1;
}
//申请内存
void* UserMemMallocMemAxiSram(uint32_t size)
{
    int32_t offset = 0;
    //需要的内存块数
    uint16_t nmemb;
    //连续空内存块数
    uint16_t cmemb = 0;
    //计数器
    uint16_t i = 0;
    if(sramAXI_SRAM_MemReady == 0)
    {
        UserMemInitMemAxiSram();
    }
    //不需要分配
    if(size == 0)
    {
        return (void*)NULL;
    }
    //获取需要分配的连续内存块数
    nmemb = size / BLOCK_SIZE_AXI_SRAM;
    if (size % BLOCK_SIZE_AXI_SRAM)
    {
        //只能多不能少
        nmemb++;
    }
    //搜索整个内存控制区
    for (offset = ALLOC_TABLE_SIZE_AXI_SRAM - 1; offset >= 0; offset--)
    {
        if(0 == sramAXI_SRAM_MapBase[offset])
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
                sramAXI_SRAM_MapBase[offset + i] = nmemb;
            }
            offset = offset * BLOCK_SIZE_AXI_SRAM;
            return (void*)(sramAXI_SRAM_Base + offset);
        }
    }
    //没有找到连续内存块支持
    return (void*)NULL;
}
//释放内存
void UserMemFreeMemAxiSram(void* ptr)
{
    uint32_t offset;
    int i = 0;
    offset=(uint32_t)ptr-(uint32_t)sramAXI_SRAM_Base;
    if(sramAXI_SRAM_MemReady == 0)
    {
        UserMemInitMemAxiSram();
    }
    if(offset < MAX_SIZE_AXI_SRAM)
    {
        //偏移所在内存块号码
        int index = offset/BLOCK_SIZE_AXI_SRAM;	
		//内存块数量		
        int nmemb = sramAXI_SRAM_MapBase[index];	
		//内存块清零
        for(i=0;i<nmemb;i++)  						
        {  
            sramAXI_SRAM_MapBase[index+i]=0;  
        }  
    }
    else
    {
        //超区间
    }
}

//获取空间使用率
uint8_t UserMemPerUsedMemAxiSram(void)
{
    uint32_t used = 0;
    uint32_t i = 0;
    for(i=0;i< ALLOC_TABLE_SIZE_AXI_SRAM;i++)  
    {  
        if(sramAXI_SRAM_MapBase[i])
			used++; 
    }
    return (used*100)/(ALLOC_TABLE_SIZE_AXI_SRAM); 
}


/*******************************D2_SRAM1************************************/
//内存池,可分配内存用一个大数组保存 
static uint8_t sramD2_SRAM1_Base[MAX_SIZE_D2_SRAM1]@".d2_sram1";					
//内存管理表,用于管理维护内存池的状态
static uint16_t sramD2_SRAM1_MapBase[ALLOC_TABLE_SIZE_D2_SRAM1]@".d2_sram1";
//当前内存区域的内存管理是否就绪	
static uint8_t sramD2_SRAM1_MemReady = 0;

//初始化内存块
void UserMemInitMemD2Sram1(void)
{
    UserMemSet(sramD2_SRAM1_Base,0,MAX_SIZE_D2_SRAM1);
    UserMemSet(sramD2_SRAM1_MapBase,0,ALLOC_TABLE_SIZE_D2_SRAM1*2);
	//内存管理初始化OK 
	sramD2_SRAM1_MemReady = 1;
}
//申请内存
void* UserMemMallocMemD2Sram1(uint32_t size)
{
    int32_t offset = 0;
    //需要的内存块数
    uint16_t nmemb;
    //连续空内存块数
    uint16_t cmemb = 0;
    //计数器
    uint16_t i = 0;
    if(sramD2_SRAM1_MemReady == 0)
    {
        UserMemInitMemD2Sram1();
    }
    //不需要分配
    if(size == 0)
    {
        return (void*)NULL;
    }
    //获取需要分配的连续内存块数
    nmemb = size / BLOCK_SIZE_D2_SRAM1;
    if (size % BLOCK_SIZE_D2_SRAM1)
    {
        //只能多不能少
        nmemb++;
    }
    //搜索整个内存控制区
    for (offset = ALLOC_TABLE_SIZE_D2_SRAM1 - 1; offset >= 0; offset--)
    {
        if(0 == sramD2_SRAM1_MapBase[offset])
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
                sramD2_SRAM1_MapBase[offset + i] = nmemb;
            }
            offset = offset * BLOCK_SIZE_D2_SRAM1;
            return (void*)(sramD2_SRAM1_Base + offset);
        }
    }
    //没有找到连续内存块支持
    return (void*)NULL;
}
//释放内存
void UserMemFreeMemD2Sram1(void* ptr)
{
    uint32_t offset;
    int i = 0;
    offset=(uint32_t)ptr-(uint32_t)sramD2_SRAM1_Base;
    if(sramD2_SRAM1_MemReady == 0)
    {
        UserMemInitMemD2Sram1();
    }
    if(offset < MAX_SIZE_D2_SRAM1)
    {
        //偏移所在内存块号码
        int index = offset/BLOCK_SIZE_D2_SRAM1;	
		//内存块数量		
        int nmemb = sramD2_SRAM1_MapBase[index];	
		//内存块清零
        for(i=0;i<nmemb;i++)  						
        {  
            sramD2_SRAM1_MapBase[index+i]=0;  
        }  
    }
    else
    {
        //超区间
    }
}

//获取空间使用率
uint8_t UserMemPerUsedMemD2Sram1(void)
{
    uint32_t used = 0;
    uint32_t i = 0;
    for(i=0;i< ALLOC_TABLE_SIZE_D2_SRAM1;i++)  
    {  
        if(sramD2_SRAM1_MapBase[i])
			used++; 
    }
    return (used*100)/(ALLOC_TABLE_SIZE_D2_SRAM1); 
}


/*******************************D2_SRAM2***********************************/
//内存池,可分配内存用一个大数组保存 
static uint8_t sramD2_SRAM2_Base[MAX_SIZE_D2_SRAM2]@".d2_sram2";					
//内存管理表,用于管理维护内存池的状态
static uint16_t sramD2_SRAM2_MapBase[ALLOC_TABLE_SIZE_D2_SRAM2]@".d2_sram2";
//当前内存区域的内存管理是否就绪	
static uint8_t sramD2_SRAM2_MemReady = 0;

//初始化内存块
void UserMemInitMemD2Sram2(void)
{
    UserMemSet(sramD2_SRAM2_Base,0,MAX_SIZE_D2_SRAM2);
    UserMemSet(sramD2_SRAM2_MapBase,0,ALLOC_TABLE_SIZE_D2_SRAM2*2);
	//内存管理初始化OK 
	sramD2_SRAM2_MemReady = 1;
}
//申请内存
void* UserMemMallocMemD2Sram2(uint32_t size)
{
    int32_t offset = 0;
    //需要的内存块数
    uint16_t nmemb;
    //连续空内存块数
    uint16_t cmemb = 0;
    //计数器
    uint16_t i = 0;
    if(sramD2_SRAM2_MemReady == 0)
    {
        UserMemInitMemD2Sram2();
    }
    //不需要分配
    if(size == 0)
    {
        return (void*)NULL;
    }
    //获取需要分配的连续内存块数
    nmemb = size / BLOCK_SIZE_D2_SRAM2;
    if (size % BLOCK_SIZE_D2_SRAM2)
    {
        //只能多不能少
        nmemb++;
    }
    //搜索整个内存控制区
    for (offset = ALLOC_TABLE_SIZE_D2_SRAM2 - 1; offset >= 0; offset--)
    {
        if(0 == sramD2_SRAM2_MapBase[offset])
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
                sramD2_SRAM2_MapBase[offset + i] = nmemb;
            }
            offset = offset * BLOCK_SIZE_D2_SRAM2;
            return (void*)(sramD2_SRAM2_Base + offset);
        }
    }
    //没有找到连续内存块支持
    return (void*)NULL;
}
//释放内存
void UserMemFreeMemD2Sram2(void* ptr)
{
    uint32_t offset;
    int i = 0;
    offset=(uint32_t)ptr-(uint32_t)sramD2_SRAM2_Base;
    if(sramD2_SRAM2_MemReady == 0)
    {
        UserMemInitMemD2Sram2();
    }
    if(offset < MAX_SIZE_D2_SRAM2)
    {
        //偏移所在内存块号码
        int index = offset/BLOCK_SIZE_D2_SRAM2;	
		//内存块数量		
        int nmemb = sramD2_SRAM2_MapBase[index];	
		//内存块清零
        for(i=0;i<nmemb;i++)  						
        {  
            sramD2_SRAM2_MapBase[index+i]=0;  
        }  
    }
    else
    {
        //超区间
    }
}

//获取空间使用率
uint8_t UserMemPerUsedMemD2Sram2(void)
{
    uint32_t used = 0;
    uint32_t i = 0;
    for(i=0;i< ALLOC_TABLE_SIZE_D2_SRAM2;i++)  
    {  
        if(sramD2_SRAM2_MapBase[i])
			used++; 
    }
    return (used*100)/(ALLOC_TABLE_SIZE_D2_SRAM2); 
}


/*******************************D3_SRAM4************************************/
//内存池,可分配内存用一个大数组保存 
static uint8_t sramD3_SRAM4_Base[MAX_SIZE_D3_SRAM4]@".d3_sram4";					
//内存管理表,用于管理维护内存池的状态
static uint16_t sramD3_SRAM4_MapBase[ALLOC_TABLE_SIZE_D3_SRAM4]@".d3_sram4";
//当前内存区域的内存管理是否就绪	
static uint8_t sramD3_SRAM4_MemReady = 0;

//初始化内存块
void UserMemInitMemD3Sram4(void)
{
    UserMemSet(sramD3_SRAM4_Base,0,MAX_SIZE_D3_SRAM4);
    UserMemSet(sramD3_SRAM4_MapBase,0,ALLOC_TABLE_SIZE_D3_SRAM4*2);
	//内存管理初始化OK 
	sramD3_SRAM4_MemReady = 1;
}
//申请内存
void* UserMemMallocMemD3Sram4(uint32_t size)
{
    int32_t offset = 0;
    //需要的内存块数
    uint16_t nmemb;
    //连续空内存块数
    uint16_t cmemb = 0;
    //计数器
    uint16_t i = 0;
    if(sramD3_SRAM4_MemReady == 0)
    {
        UserMemInitMemD3Sram4();
    }
    //不需要分配
    if(size == 0)
    {
        return (void*)NULL;
    }
    //获取需要分配的连续内存块数
    nmemb = size / BLOCK_SIZE_D3_SRAM4;
    if (size % BLOCK_SIZE_D3_SRAM4)
    {
        //只能多不能少
        nmemb++;
    }
    //搜索整个内存控制区
    for (offset = ALLOC_TABLE_SIZE_D3_SRAM4 - 1; offset >= 0; offset--)
    {
        if(0 == sramD3_SRAM4_MapBase[offset])
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
                sramD3_SRAM4_MapBase[offset + i] = nmemb;
            }
            offset = offset * BLOCK_SIZE_D3_SRAM4;
            return (void*)(sramD3_SRAM4_Base + offset);
        }
    }
    //没有找到连续内存块支持
    return (void*)NULL;
}
//释放内存
void UserMemFreeMemD3Sram4(void* ptr)
{
    uint32_t offset;
    int i = 0;
    offset=(uint32_t)ptr-(uint32_t)sramD3_SRAM4_Base;
    if(sramD3_SRAM4_MemReady == 0)
    {
        UserMemInitMemD3Sram4();
    }
    if(offset < MAX_SIZE_D3_SRAM4)
    {
        //偏移所在内存块号码
        int index = offset/BLOCK_SIZE_D3_SRAM4;	
		//内存块数量		
        int nmemb = sramD3_SRAM4_MapBase[index];	
		//内存块清零
        for(i=0;i<nmemb;i++)  						
        {  
            sramD3_SRAM4_MapBase[index+i]=0;  
        }  
    }
    else
    {
        //超区间
    }
}

//获取空间使用率
uint8_t UserMemPerUsedMemD3Sram4(void)
{
    uint32_t used = 0;
    uint32_t i = 0;
    for(i=0;i< ALLOC_TABLE_SIZE_D3_SRAM4;i++)  
    {  
        if(sramD3_SRAM4_MapBase[i])
			used++; 
    }
    return (used*100)/(ALLOC_TABLE_SIZE_D3_SRAM4); 
}


/*******************************SDRAM1************************************/
//内存池,可分配内存用一个大数组保存 
static uint8_t sdram1ByteArray[MAX_SIZE_SDRAM1]@".sdram1";					
static TX_BYTE_POOL bytePoolSdram1;
//当前内存区域的内存管理是否就绪	
static uint8_t sdram1MemReady = 0;

//初始化内存块
void UserMemInitMemSdram1(void)
{
    tx_byte_pool_create(&bytePoolSdram1,"bytePoolSdram1",sdram1ByteArray,MAX_SIZE_SDRAM1);
	//内存管理初始化OK 
	sdram1MemReady = 1;
}
//申请内存
void* UserMemMallocMemSdram1(uint32_t size)
{
    void* memMallocResult = NULL;
    if(sdram1MemReady == 0)
    {
        UserMemInitMemSdram1();
    }
    //申请内存
    tx_byte_allocate(&bytePoolSdram1,(void**)(&memMallocResult),size,TX_NO_WAIT);
    //回传结果
    return memMallocResult;
}
//释放内存
void UserMemFreeMemSdram1(void* ptr)
{
    if(sdram1MemReady == 0)
    {
        UserMemInitMemSdram1();
    }
    if(ptr == NULL)
    {
        return;
    }
    else
    {
        //释放内存
        tx_byte_release(ptr);
    }
}

//获取空间使用率
uint8_t UserMemPerUsedMemSdram1(void)
{
    ULONG availableBytesCount = 0;
    ULONG fragmentByteCount = 0;
    if(sdram1MemReady == 0)
    {
        UserMemInitMemSdram1();
    }
    tx_byte_pool_info_get(&bytePoolSdram1,TX_NULL,&availableBytesCount,&fragmentByteCount,
                            TX_NULL,TX_NULL,TX_NULL);
    return ((fragmentByteCount-availableBytesCount)*100)/(fragmentByteCount); 
}

/*******************************SDRAM2************************************/
//内存池,可分配内存用一个大数组保存 
static uint8_t sdram2ByteArray[MAX_SIZE_SDRAM2]@".sdram2";					
static TX_BYTE_POOL bytePoolSdram2;
//当前内存区域的内存管理是否就绪	
static uint8_t sdram2MemReady = 0;

//初始化内存块
void UserMemInitMemSdram2(void)
{
    tx_byte_pool_create(&bytePoolSdram2,"bytePoolSdram2",sdram2ByteArray,MAX_SIZE_SDRAM2);
	//内存管理初始化OK 
	sdram2MemReady = 1;
}
//申请内存
void* UserMemMallocMemSdram2(uint32_t size)
{
    void* memMallocResult = NULL;
    if(sdram2MemReady == 0)
    {
        UserMemInitMemSdram2();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolSdram2,(void**)(&memMallocResult),size,TX_WAIT_FOREVER);
    
    //回传结果
    return memMallocResult;
}
//释放内存
void UserMemFreeMemSdram2(void* ptr)
{
    if(sdram2MemReady == 0)
    {
        UserMemInitMemSdram2();
    }
    if(ptr == NULL)
    {
        return;
    }
    else
    {
        //释放内存
        tx_byte_release(ptr);
    }
}

//获取空间使用率
uint8_t UserMemPerUsedMemSdram2(void)
{
    ULONG availableBytesCount = 0;
    ULONG fragmentByteCount = 0;
    if(sdram2MemReady == 0)
    {
        UserMemInitMemSdram2();
    }
    tx_byte_pool_info_get(&bytePoolSdram2,TX_NULL,&availableBytesCount,&fragmentByteCount,
                            TX_NULL,TX_NULL,TX_NULL);
    return ((fragmentByteCount-availableBytesCount)*100)/(fragmentByteCount); 
}

/******************************函数指针数组**********************************/
//内存初始化函数指针数组
static const UserMemInitFuncPtr UserMemInitFuncPtrArray[] = {
    UserMemInitMemAxiSram,
    UserMemInitMemD2Sram1,
    UserMemInitMemD2Sram2,
    UserMemInitMemD3Sram4,
    UserMemInitMemSdram1,
    UserMemInitMemSdram2,
};
//内存申请函数指针数组
static const UserMemMallocFuncPtr UserMemMallocFuncPtrArray[] = {
    UserMemMallocMemAxiSram,
    UserMemMallocMemD2Sram1,
    UserMemMallocMemD2Sram2,
    UserMemMallocMemD3Sram4,
    UserMemMallocMemSdram1,
    UserMemMallocMemSdram2,
};
//内存释放函数指针数组
static const UserMemFreeFuncPtr UserMemFreeFuncPtrArray[] = {
    UserMemFreeMemAxiSram,
    UserMemFreeMemD2Sram1,
    UserMemFreeMemD2Sram2,
    UserMemFreeMemD3Sram4,
    UserMemFreeMemSdram1,
    UserMemFreeMemSdram2,
};

//内存使用率计算函数指针数组
static const UserMemPerUsedFuncPtr UserMemPerUsedFuncPtrArray[] = {
    UserMemPerUsedMemAxiSram,
    UserMemPerUsedMemD2Sram1,
    UserMemPerUsedMemD2Sram2,
    UserMemPerUsedMemD3Sram4,
    UserMemPerUsedMemSdram1,
    UserMemPerUsedMemSdram2,
};

//内存管理互斥信号量
static TX_MUTEX mutexUserMemManage;
static uint8_t mutexInitFlag = 0;

//内存管理初始化
void UserMemInit(USER_MEM_REGION region)
{
    //判断内存区域是否支持
    if(region > MEM_SDRAM2)return;
    //内存管理初始化
    UserMemInitFuncPtrArray[region]();
    //互斥信号量初始化
    if(mutexInitFlag == 0)
    {
        mutexInitFlag = 1;
        //初始化互斥信号量,启用优先级继承
        tx_mutex_create(&mutexUserMemManage,"mutexUserMemManage",TX_INHERIT);
    }
}

//申请内存
void* UserMemMalloc(USER_MEM_REGION region,uint32_t size)
{
    void* targetPtr;
    //判断内存区域是否支持
    if(region > MEM_SDRAM2)return NULL;
    //申请互斥信号量
	tx_mutex_get(&mutexUserMemManage,TX_WAIT_FOREVER);
    //申请内存
    targetPtr = UserMemMallocFuncPtrArray[region](size);
    //申请来的内存清空
    if(targetPtr != NULL)
    {
        UserMemSet(targetPtr,0,size);
    }
    //释放互斥信号量
	tx_mutex_put(&mutexUserMemManage);
    return targetPtr;
}

//申请内存直到成功
void* UserMemMallocWhileSuccess(USER_MEM_REGION region,uint32_t size)
{
    void* dstMemPtr = NULL;
    do
    {
        dstMemPtr = UserMemMalloc(region,size);
        if(dstMemPtr == NULL)
        {
            CoreDelayMinTick();
        }
    }while(dstMemPtr == NULL);
    return dstMemPtr;
}

//释放内存
void UserMemFree(USER_MEM_REGION region,void* ptr)
{
    //判断内存区域是否支持
    if(region > MEM_SDRAM2)return;
    //申请互斥信号量
	tx_mutex_get(&mutexUserMemManage,TX_WAIT_FOREVER);
    //释放内存
    UserMemFreeFuncPtrArray[region](ptr);
    //释放互斥信号量
	tx_mutex_put(&mutexUserMemManage);
}

//获取当前动态内存使用率
uint8_t UserMemPerUsed(USER_MEM_REGION region)
{
    uint8_t used;
    //识别内存区域是否支持
    if(region > MEM_SDRAM2)return 0;
    //申请互斥信号量
	tx_mutex_get(&mutexUserMemManage,TX_WAIT_FOREVER);
    //计算内存使用率
    used = UserMemPerUsedFuncPtrArray[region]();
    //释放互斥信号量
	tx_mutex_put(&mutexUserMemManage);
    return used;
}
