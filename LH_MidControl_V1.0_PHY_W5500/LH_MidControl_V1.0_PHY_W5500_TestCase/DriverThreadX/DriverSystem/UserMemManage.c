/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:46:47 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:17:24 +0800
************************************************************************************************/ 
#include "UserMemManage.h"
#include "tx_byte_pool.h"


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

//AXI_SRAM内存初始化
void UserMemInitAXI_SRAM(void)
{
    if(bytePoolMemAxiSram.tx_byte_pool_id == TX_BYTE_POOL_ID)
    {
        //已经初始化了
        return;
    }
    //清空内存
    UserMemSet(memAxiSramByteArray,0,MEM_MANAGE_BYTE_SIZE_AXI_SRAM);
	//创建内存池
    tx_byte_pool_create(&bytePoolMemAxiSram,"bytePoolMemAxiSram",memAxiSramByteArray,MEM_MANAGE_BYTE_SIZE_AXI_SRAM);
}

//申请内存
void* UserMemMallocAXI_SRAM(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemAxiSram.tx_byte_pool_id != TX_BYTE_POOL_ID)
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
    if(bytePoolMemAxiSram.tx_byte_pool_id != TX_BYTE_POOL_ID)
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
    if(bytePoolMemAxiSram.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        UserMemInitAXI_SRAM();
    }
    tx_byte_pool_info_get(&bytePoolMemAxiSram,TX_NULL,&availableBytesCount,TX_NULL,TX_NULL,TX_NULL,TX_NULL);
    userFloat = ((MEM_MANAGE_BYTE_SIZE_AXI_SRAM-availableBytesCount)*100)/(MEM_MANAGE_BYTE_SIZE_AXI_SRAM); 
    return (uint8_t)userFloat;
}

/*-------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------D2_SRAM1内存-------------------------------------------------------*/
#pragma pack(push) //保存对齐状态 
#pragma pack(4)//四字节对齐
//内存池 
static uint8_t memD2_Sram1ByteArray[MEM_MANAGE_BYTE_SIZE_D2_SRAM1]@".d2_sram1";	
#pragma pack (pop)			
	
//管理内存池
static TX_BYTE_POOL bytePoolMemD2_Sram1;

//D2_SRAM1内存初始化
void UserMemInitD2_SRAM1(void)
{
    if(bytePoolMemD2_Sram1.tx_byte_pool_id == TX_BYTE_POOL_ID)
    {
        //已经初始化过了
        return;
    }
    //清空内存
    UserMemSet(memD2_Sram1ByteArray,0,MEM_MANAGE_BYTE_SIZE_D2_SRAM1);
	//创建内存池
    tx_byte_pool_create(&bytePoolMemD2_Sram1,"bytePoolMemD2_Sram1",memD2_Sram1ByteArray,MEM_MANAGE_BYTE_SIZE_D2_SRAM1);
}

//申请内存
void* UserMemMallocD2_SRAM1(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemD2_Sram1.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        UserMemInitD2_SRAM1();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemD2_Sram1,(void**)(&memMallocResult),size,TX_NO_WAIT);
    //回传结果
    return memMallocResult;
}

//释放内存
void UserMemFreeD2_SRAM1(void* ptr)
{
    if(bytePoolMemD2_Sram1.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        UserMemInitD2_SRAM1();
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
uint8_t UserMemPerUsedD2_SRAM1(void)
{   
    ULONG availableBytesCount = 0;
    float userFloat = 0.0;
    if(bytePoolMemD2_Sram1.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        UserMemInitD2_SRAM1();
    }
    tx_byte_pool_info_get(&bytePoolMemD2_Sram1,TX_NULL,&availableBytesCount,TX_NULL,TX_NULL,TX_NULL,TX_NULL);
    userFloat = ((MEM_MANAGE_BYTE_SIZE_D2_SRAM1-availableBytesCount)*100)/(MEM_MANAGE_BYTE_SIZE_D2_SRAM1); 
    return (uint8_t)userFloat;
}

/*-------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------D3_SRAM4内存-------------------------------------------------------*/
#pragma pack(push) //保存对齐状态 
#pragma pack(4)//四字节对齐
//内存池 
static uint8_t memD3_Sram4ByteArray[MEM_MANAGE_BYTE_SIZE_D3_SRAM4]@".d3_sram4";	
#pragma pack (pop)			
	
//管理内存池
static TX_BYTE_POOL bytePoolMemD3_Sram4;

//D3_SRAM4内存初始化
void UserMemInitD3_SRAM4(void)
{
    if(bytePoolMemD3_Sram4.tx_byte_pool_id == TX_BYTE_POOL_ID)
    {
        //已经初始化过了
        return;
    }
    //清空内存
    UserMemSet(memD3_Sram4ByteArray,0,MEM_MANAGE_BYTE_SIZE_D3_SRAM4);
	//创建内存池
    tx_byte_pool_create(&bytePoolMemD3_Sram4,"bytePoolMemD3_Sram4",memD3_Sram4ByteArray,MEM_MANAGE_BYTE_SIZE_D3_SRAM4);
}

//申请内存
void* UserMemMallocD3_SRAM4(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemD3_Sram4.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        UserMemInitD3_SRAM4();
    }
    //申请内存
    int32_t temp = tx_byte_allocate(&bytePoolMemD3_Sram4,(void**)(&memMallocResult),size,TX_NO_WAIT);
    //回传结果
    return memMallocResult;
}

//释放内存
void UserMemFreeD3_SRAM4(void* ptr)
{
    if(bytePoolMemD3_Sram4.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        UserMemInitD3_SRAM4();
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
uint8_t UserMemPerUsedD3_SRAM4(void)
{   
    ULONG availableBytesCount = 0;
    float userFloat = 0.0;
    if(bytePoolMemD3_Sram4.tx_byte_pool_id != TX_BYTE_POOL_ID)
    {
        UserMemInitD3_SRAM4();
    }
    tx_byte_pool_info_get(&bytePoolMemD3_Sram4,TX_NULL,&availableBytesCount,TX_NULL,TX_NULL,TX_NULL,TX_NULL);
    userFloat = ((MEM_MANAGE_BYTE_SIZE_D3_SRAM4-availableBytesCount)*100)/(MEM_MANAGE_BYTE_SIZE_D3_SRAM4); 
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

//SDRAM内存初始化
void UserMemInitSDRAM1(void)
{
    if(bytePoolMemSDRam1.tx_byte_pool_id == TX_BYTE_POOL_ID)
    {
        return;
    }
    //清空内存
    UserMemSet(memSDRam1ByteArray,0,MEM_MANAGE_BYTE_SIZE_SDRAM1);
	//创建内存池
    tx_byte_pool_create(&bytePoolMemSDRam1,"bytePoolMemSDRam1",memSDRam1ByteArray,MEM_MANAGE_BYTE_SIZE_SDRAM1);
}

//申请内存
void* UserMemMallocSDRAM1(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemSDRam1.tx_byte_pool_id != TX_BYTE_POOL_ID)
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
    if(bytePoolMemSDRam1.tx_byte_pool_id != TX_BYTE_POOL_ID)
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
    if(bytePoolMemSDRam1.tx_byte_pool_id != TX_BYTE_POOL_ID)
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

//SDRAM内存初始化
void UserMemInitSDRAM2(void)
{
    if(bytePoolMemSDRam2.tx_byte_pool_id == TX_BYTE_POOL_ID)
    {
        return;
    }
    //清空内存
    UserMemSet(memSDRam2ByteArray,0,MEM_MANAGE_BYTE_SIZE_SDRAM2);
	//创建内存池
    tx_byte_pool_create(&bytePoolMemSDRam2,"bytePoolMemSDRam2",memSDRam2ByteArray,MEM_MANAGE_BYTE_SIZE_SDRAM2);
}

//申请内存
void* UserMemMallocSDRAM2(uint32_t size)
{
    void* memMallocResult = NULL;
    if(bytePoolMemSDRam2.tx_byte_pool_id != TX_BYTE_POOL_ID)
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
    if(bytePoolMemSDRam2.tx_byte_pool_id != TX_BYTE_POOL_ID)
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
    if(bytePoolMemSDRam2.tx_byte_pool_id != TX_BYTE_POOL_ID)
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
    UserMemInitD2_SRAM1,
    UserMemInitD3_SRAM4,
    UserMemInitSDRAM1,
    UserMemInitSDRAM2,
};

static const UserMemMallocFuncPtr UserMemMallocFuncPtrArray[] = {
    UserMemMallocAXI_SRAM,
    UserMemMallocD2_SRAM1,
    UserMemMallocD3_SRAM4,
    UserMemMallocSDRAM1,
    UserMemMallocSDRAM2,
};

static const UserMemFreeFuncPtr UserMemFreeFuncPtrArray[] = {
    UserMemFreeAXI_SRAM,
    UserMemFreeD2_SRAM1,
    UserMemFreeD3_SRAM4,
    UserMemFreeSDRAM1,
    UserMemFreeSDRAM2,
};

static const UserMemPerUsedFuncPtr UserMemPerUsedFuncPtrArray[] = {
    UserMemPerUsedAXI_SRAM,
    UserMemPerUsedD2_SRAM1,
    UserMemPerUsedD3_SRAM4,
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