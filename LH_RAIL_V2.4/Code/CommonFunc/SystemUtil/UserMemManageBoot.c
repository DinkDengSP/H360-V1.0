/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2020-06-19 15:37:18
**LastEditors: DengXiaoJun
**LastEditTime: 2020-07-04 15:43:33
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


/*******************************DTCM************************************/
//内存池,可分配内存用一个大数组保存 
static uint8_t sramDTCM_Base[MAX_SIZE_DTCM]@".dtcm_ram";					
//内存管理表,用于管理维护内存池的状态
static uint16_t sramDTCM_MapBase[ALLOC_TABLE_SIZE_DTCM]@".dtcm_ram";
//当前内存区域的内存管理是否就绪	
static uint8_t sramDTCM_MemReady = 0;

//初始化内存块
void UserMemInitMemDTCM(void)
{
    UserMemSet(sramDTCM_Base,0,MAX_SIZE_DTCM);
    UserMemSet(sramDTCM_MapBase,0,ALLOC_TABLE_SIZE_DTCM*2);
	//内存管理初始化OK 
	sramDTCM_MemReady = 1;
}
//申请内存
void* UserMemMallocMemDTCM(uint32_t size)
{
    int32_t offset = 0;
    //需要的内存块数
    uint16_t nmemb;
    //连续空内存块数
    uint16_t cmemb = 0;
    //计数器
    uint16_t i = 0;
    if(sramDTCM_MemReady == 0)
    {
        UserMemInitMemDTCM();
    }
    //不需要分配
    if(size == 0)
    {
        return (void*)NULL;
    }
    //获取需要分配的连续内存块数
    nmemb = size / BLOCK_SIZE_DTCM;
    if (size % BLOCK_SIZE_DTCM)
    {
        //只能多不能少
        nmemb++;
    }
    //搜索整个内存控制区
    for (offset = ALLOC_TABLE_SIZE_DTCM - 1; offset >= 0; offset--)
    {
        if(0 == sramDTCM_MapBase[offset])
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
                sramDTCM_MapBase[offset + i] = nmemb;
            }
            offset = offset * BLOCK_SIZE_DTCM;
            return (void*)(sramDTCM_Base + offset);
        }
    }
    //没有找到连续内存块支持
    return (void*)NULL;
}
//释放内存
void UserMemFreeMemDTCM(void* ptr)
{
    uint32_t offset;
    int i = 0;
    offset=(uint32_t)ptr-(uint32_t)sramDTCM_Base;
    if(sramDTCM_MemReady == 0)
    {
        UserMemInitMemDTCM();
    }
    if(offset < MAX_SIZE_DTCM)
    {
        //偏移所在内存块号码
        int index = offset/BLOCK_SIZE_DTCM;	
		//内存块数量		
        int nmemb = sramDTCM_MapBase[index];	
		//内存块清零
        for(i=0;i<nmemb;i++)  						
        {  
            sramDTCM_MapBase[index+i]=0;  
        }  
    }
    else
    {
        //超区间
    }
}
//内存块扩容
void* UserMemReallocMemDTCM(void *ptr,uint32_t size)
{
    uint8_t* newPtr = UserMemMallocMemDTCM(size);
    if(newPtr != NULL)
    {
        UserMemCopy(newPtr,ptr,size);
        UserMemFreeMemDTCM(ptr);
        return (void*)newPtr;
    }
    else
    {
        return (void*)NULL;
    }
}
//获取空间使用率
uint8_t UserMemPerUsedMemDTCM(void)
{
    uint32_t used = 0;
    uint32_t i = 0;
    for(i=0;i< ALLOC_TABLE_SIZE_DTCM;i++)  
    {  
        if(sramDTCM_MapBase[i])
			used++; 
    }
    return (used*100)/(ALLOC_TABLE_SIZE_DTCM); 
}

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
//内存块扩容
void* UserMemReallocMemAxiSram(void *ptr,uint32_t size)
{
    uint8_t* newPtr = UserMemMallocMemAxiSram(size);
    if(newPtr != NULL)
    {
        UserMemCopy(newPtr,ptr,size);
        UserMemFreeMemAxiSram(ptr);
        return (void*)newPtr;
    }
    else
    {
        return (void*)NULL;
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
//内存块扩容
void* UserMemReallocMemD2Sram1(void *ptr,uint32_t size)
{
    uint8_t* newPtr = UserMemMallocMemD2Sram1(size);
    if(newPtr != NULL)
    {
        UserMemCopy(newPtr,ptr,size);
        UserMemFreeMemD2Sram1(ptr);
        return (void*)newPtr;
    }
    else
    {
        return (void*)NULL;
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
//内存块扩容
void* UserMemReallocMemD2Sram2(void *ptr,uint32_t size)
{
    uint8_t* newPtr = UserMemMallocMemD2Sram2(size);
    if(newPtr != NULL)
    {
        UserMemCopy(newPtr,ptr,size);
        UserMemFreeMemD2Sram2(ptr);
        return (void*)newPtr;
    }
    else
    {
        return (void*)NULL;
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
//内存块扩容
void* UserMemReallocMemD3Sram4(void *ptr,uint32_t size)
{
    uint8_t* newPtr = UserMemMallocMemD3Sram4(size);
    if(newPtr != NULL)
    {
        UserMemCopy(newPtr,ptr,size);
        UserMemFreeMemD3Sram4(ptr);
        return (void*)newPtr;
    }
    else
    {
        return (void*)NULL;
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


/*******************************IS62************************************/

//初始化内存块
void UserMemInitMemIS62(void)
{
    return;
}
//申请内存
void* UserMemMallocMemIS62(uint32_t size)
{
    return NULL;
}
//释放内存
void UserMemFreeMemIS62(void* ptr)
{
    return;
}
//内存块扩容
void* UserMemReallocMemIS62(void *ptr,uint32_t size)
{
    return NULL;
}
//获取空间使用率
uint8_t UserMemPerUsedMemIS62(void)
{
    return 0;
}

/******************************函数指针数组**********************************/
//内存初始化函数指针数组
static const UserMemInitFuncPtr UserMemInitFuncPtrArray[] = {
    UserMemInitMemDTCM,
    UserMemInitMemAxiSram,
    UserMemInitMemD2Sram1,
    UserMemInitMemD2Sram2,
    UserMemInitMemD3Sram4,
    UserMemInitMemIS62,
};
//内存申请函数指针数组
static const UserMemMallocFuncPtr UserMemMallocFuncPtrArray[] = {
    UserMemMallocMemDTCM,
    UserMemMallocMemAxiSram,
    UserMemMallocMemD2Sram1,
    UserMemMallocMemD2Sram2,
    UserMemMallocMemD3Sram4,
    UserMemMallocMemIS62,
};
//内存释放函数指针数组
static const UserMemFreeFuncPtr UserMemFreeFuncPtrArray[] = {
    UserMemFreeMemDTCM,
    UserMemFreeMemAxiSram,
    UserMemFreeMemD2Sram1,
    UserMemFreeMemD2Sram2,
    UserMemFreeMemD3Sram4,
    UserMemFreeMemIS62,
};
//内存重新扩容函数指针数组
static const UserMemReallocFuncPtr UserMemReallocFuncPtrArray[] = {
    UserMemReallocMemDTCM,
    UserMemReallocMemAxiSram,
    UserMemReallocMemD2Sram1,
    UserMemReallocMemD2Sram2,
    UserMemReallocMemD3Sram4,
    UserMemReallocMemIS62,
};
//内存使用率计算函数指针数组
static const UserMemPerUsedFuncPtr UserMemPerUsedFuncPtrArray[] = {
    UserMemPerUsedMemDTCM,
    UserMemPerUsedMemAxiSram,
    UserMemPerUsedMemD2Sram1,
    UserMemPerUsedMemD2Sram2,
    UserMemPerUsedMemD3Sram4,
    UserMemPerUsedMemIS62,
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
