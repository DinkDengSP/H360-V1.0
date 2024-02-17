/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 10:07:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 12:04:34 +0800
************************************************************************************************/ 
#include "UserMemManage.h"

//函数指针类型定义
typedef void (*UserMemInitFuncPtr)();
typedef void* (*UserMemMallocFuncPtr)(uint32_t size);
typedef void (*UserMemFreeFuncPtr)(void* ptr);
typedef uint8_t (*UserMemPerUsedFuncPtr)(void);

/*--------------------------------------------AXI_SRAM内存-------------------------------------------------------*/
#pragma pack(push) //保存对齐状态 
#pragma pack(4)//四字节对齐
//内存池 
static uint8_t memAxiSramByteArray[MEM_MANAGE_BYTE_SIZE_AXI_SRAM]@".axi_sram";	
#pragma pack (pop)				
//管理内存池
static TX_BYTE_POOL bytePoolMemAxiSram;
//当内存池是否已经初始化
static uint8_t memRegionAxiSramReady = 0;

//AXI_SRAM内存初始化
void UserMemInitAXI_SRAM(void)
{
    //清空内存
    UserMemSet(memAxiSramByteArray,0,MEM_MANAGE_BYTE_SIZE_AXI_SRAM);
	//创建内存池
    tx_byte_pool_create(&bytePoolMemAxiSram,"bytePoolMemAxiSram",memAxiSramByteArray,MEM_MANAGE_BYTE_SIZE_AXI_SRAM);
    //初始化标记
    memRegionAxiSramReady = 1;
}

//申请内存
void* UserMemMallocAXI_SRAM(uint32_t size)
{
    void* memMallocResult = NULL;
    if(memRegionAxiSramReady == 0)
    {
        UserMemInitAXI_SRAM();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemAxiSram,(void**)(&memMallocResult),size,TX_NO_WAIT);
    //回传结果
    return memMallocResult;
}

//释放内存
void UserMemFreeAXI_SRAM(void* ptr)
{
    if(memRegionAxiSramReady == 0)
    {
        UserMemInitAXI_SRAM();
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
//获取当前动态内存使用率
uint8_t UserMemPerUsedAXI_SRAM(void)
{   
    ULONG availableBytesCount = 0;
    float userFloat = 0.0;
    if(memRegionAxiSramReady == 0)
    {
        UserMemInitAXI_SRAM();
    }
    tx_byte_pool_info_get(&bytePoolMemAxiSram,TX_NULL,&availableBytesCount,TX_NULL,TX_NULL,TX_NULL,TX_NULL);
    userFloat = ((MEM_MANAGE_BYTE_SIZE_AXI_SRAM-availableBytesCount)*100)/(MEM_MANAGE_BYTE_SIZE_AXI_SRAM); 
    return (uint8_t)userFloat;
}

/*-------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------SDRAM1内存-------------------------------------------------------*/
#pragma pack(push) //保存对齐状态 
#pragma pack(4)//四字节对齐
//内存池 
static uint8_t memSDRam1ByteArray[MEM_MANAGE_BYTE_SIZE_SDRAM1]@".sdram1";	
#pragma pack (pop)				
//管理内存池
static TX_BYTE_POOL bytePoolMemSDRam1;
//当内存池是否已经初始化
static uint8_t memRegionSDRam1Ready = 0;

//SDRAM1内存初始化
void UserMemInitSDRAM1(void)
{
    //清空内存
    UserMemSet(memSDRam1ByteArray,0,MEM_MANAGE_BYTE_SIZE_SDRAM1);
	//创建内存池
    tx_byte_pool_create(&bytePoolMemSDRam1,"bytePoolMemSDRam1",memSDRam1ByteArray,MEM_MANAGE_BYTE_SIZE_SDRAM1);
    //初始化标记
    memRegionSDRam1Ready = 1;
}

//申请内存
void* UserMemMallocSDRAM1(uint32_t size)
{
    void* memMallocResult = NULL;
    if(memRegionSDRam1Ready == 0)
    {
        UserMemInitSDRAM1();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemSDRam1,(void**)(&memMallocResult),size,TX_NO_WAIT);
    //回传结果
    return memMallocResult;
}

//释放内存
void UserMemFreeSDRAM1(void* ptr)
{
    if(memRegionSDRam1Ready == 0)
    {
        UserMemInitSDRAM1();
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
//获取当前动态内存使用率
uint8_t UserMemPerUsedSDRAM1(void)
{   
    ULONG availableBytesCount = 0;
    float userFloat = 0.0;
    if(memRegionSDRam1Ready == 0)
    {
        UserMemInitSDRAM1();
    }
    tx_byte_pool_info_get(&bytePoolMemSDRam1,TX_NULL,&availableBytesCount,TX_NULL,TX_NULL,TX_NULL,TX_NULL);
    userFloat = ((MEM_MANAGE_BYTE_SIZE_SDRAM1-availableBytesCount)*100)/(MEM_MANAGE_BYTE_SIZE_SDRAM1); 
    return (uint8_t)userFloat;
}

/*-------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------SDRAM2内存-------------------------------------------------------*/
#pragma pack(push) //保存对齐状态 
#pragma pack(4)//四字节对齐
//内存池 
static uint8_t memSDRam2ByteArray[MEM_MANAGE_BYTE_SIZE_SDRAM2]@".sdram2";	
#pragma pack (pop)				
//管理内存池
static TX_BYTE_POOL bytePoolMemSDRam2;
//当内存池是否已经初始化
static uint8_t memRegionSDRam2Ready = 0;

//SDRAM2内存初始化
void UserMemInitSDRAM2(void)
{
    //清空内存
    UserMemSet(memSDRam2ByteArray,0,MEM_MANAGE_BYTE_SIZE_SDRAM2);
	//创建内存池
    tx_byte_pool_create(&bytePoolMemSDRam2,"bytePoolMemSDRam2",memSDRam2ByteArray,MEM_MANAGE_BYTE_SIZE_SDRAM2);
    //初始化标记
    memRegionSDRam2Ready = 1;
}

//申请内存
void* UserMemMallocSDRAM2(uint32_t size)
{
    void* memMallocResult = NULL;
    if(memRegionSDRam2Ready == 0)
    {
        UserMemInitSDRAM2();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemSDRam2,(void**)(&memMallocResult),size,TX_NO_WAIT);
    //回传结果
    return memMallocResult;
}

//释放内存
void UserMemFreeSDRAM2(void* ptr)
{
    if(memRegionSDRam2Ready == 0)
    {
        UserMemInitSDRAM2();
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
//获取当前动态内存使用率
uint8_t UserMemPerUsedSDRAM2(void)
{   
    ULONG availableBytesCount = 0;
    float userFloat = 0.0;
    if(memRegionSDRam2Ready == 0)
    {
        UserMemInitSDRAM2();
    }
    tx_byte_pool_info_get(&bytePoolMemSDRam2,TX_NULL,&availableBytesCount,TX_NULL,TX_NULL,TX_NULL,TX_NULL);
    userFloat = ((MEM_MANAGE_BYTE_SIZE_SDRAM2-availableBytesCount)*100)/(MEM_MANAGE_BYTE_SIZE_SDRAM2); 
    return (uint8_t)userFloat;
}

/*-------------------------------------------------------------------------------------------------------------------*/


static const UserMemInitFuncPtr UserMemInitFuncPtrArray[] = {
    UserMemInitAXI_SRAM,
    UserMemInitSDRAM1,
    UserMemInitSDRAM2,
};

static const UserMemMallocFuncPtr UserMemMallocFuncPtrArray[] = {
    UserMemMallocAXI_SRAM,
    UserMemMallocSDRAM1,
    UserMemMallocSDRAM2,
};

static const UserMemFreeFuncPtr UserMemFreeFuncPtrArray[] = {
    UserMemFreeAXI_SRAM,
    UserMemFreeSDRAM1,
    UserMemFreeSDRAM2,
};

static const UserMemPerUsedFuncPtr UserMemPerUsedFuncPtrArray[] = {
    UserMemPerUsedAXI_SRAM,
    UserMemPerUsedSDRAM1,
    UserMemPerUsedSDRAM2,
};


//动态内存初始化
void UserMemInit(MEM_REGION locate)
{
    UserMemInitFuncPtrArray[locate]();
}

//申请内存
void* UserMemMalloc(MEM_REGION locate,uint32_t size)
{
    void* targetPtr;
    targetPtr = UserMemMallocFuncPtrArray[locate](size);
    return targetPtr;
}

//释放内存
void UserMemFree(MEM_REGION locate,void* ptr)
{
    if(ptr == NULL)
    {
        return;
    }
    UserMemFreeFuncPtrArray[locate](ptr);
}

//申请内存
void* UserMemMallocWhileSuccess(MEM_REGION locate,uint32_t size)
{
    uint8_t* mallocResult = NULL;
    if(size == 0)
    {
        return NULL;
    }
    do
    {
        mallocResult = UserMemMalloc(locate,size);
        if(mallocResult == NULL)
        {
            CoreDelayMinTick();
        }
    }while(mallocResult == NULL);
    return (void*)(mallocResult);
}

//获取当前动态内存使用率
uint8_t UserMemPerUsed(MEM_REGION locate)
{
    uint8_t used;
    used = UserMemPerUsedFuncPtrArray[locate]();
    return used;
}
