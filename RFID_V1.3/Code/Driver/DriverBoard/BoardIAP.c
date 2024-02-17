#include "BoardIAP.h"
#include "BoardMB85RS256.h"
//跳转函数原型
typedef void (*SystemJmpAddressFuncPtr)(void);

//跳转到APP
void jump_to_app(uint32_t app_address)
{
    //检查栈顶地址是否合法.0x20000000是SRAM的起始地址,也是程序的栈顶地址
    //如果这个位置的数据不对,那么说明没有APP
    if(((*(__IO uint32_t*)app_address)&0x2FFE0000)==0x20000000)	
    {
       // ServicePrintf("System Jmp IAP\r\n");
        //关闭中断
        INTX_DISABLE();
        //设置特权级和用户级下的堆栈指针
        __set_MSP(*(uint32_t*)app_address);
        __set_PSP(*(uint32_t*)app_address);
        //强制切换到特权级
        __set_CONTROL(0);
        //系统刷新,保证设置生效
        __ISB();
        // //中断向量表赋值到RAM区
        // UserMemCopy((void *)RAM_START_ADDR,(void *)app_address,VECTOR_SIZE);
        // //设置RAM映射到地址0x00000000
        // SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
        //跳转
        ((SystemJmpAddressFuncPtr)(*(uint32_t*)(app_address + 4)))();
    }
}

//跳转到Boot
void jump_to_boot(uint32_t app_address)
{
    //检查栈顶地址是否合法.0x20000000是SRAM的起始地址,也是程序的栈顶地址
    //如果这个位置的数据不对,那么说明没有APP
    if(((*(__IO uint32_t*)app_address)&0x2FFE0000)==0x20000000)	
    {
       // ServicePrintf("System Jmp IAP\r\n");
        //关闭中断
        INTX_DISABLE();
        //设置特权级和用户级下的堆栈指针
        __set_MSP(*(uint32_t*)app_address);
        __set_PSP(*(uint32_t*)app_address);
        //强制切换到特权级
        __set_CONTROL(0);
        //系统刷新,保证设置生效
        __ISB();
        //设置RAM映射到flash起始地址0x08000000
        SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_Flash);
        //跳转
        ((SystemJmpAddressFuncPtr)(*(uint32_t*)(app_address + 4)))();
    }
}

//跳转到APP
void BoardIapJumpApp(void)
{
    jump_to_app(FLASH_APP_ADDR);
}

//跳转到Boot
void BoardIapJumpBoot(void)
{
    uint8_t iapFlag = 0xA5;
    BoardMB85RS256_WriteBuffer(APP_UPDATE_FLAG_ADDR,&iapFlag,1);
    jump_to_boot(FLASH_IAP_ADDR);
}

//设置中断向量表在RAM区
void BoardIapSetVectorTableInSRAM(void)
{
    //中断向量表赋值到RAM区
    UserMemCopy((void *)RAM_START_ADDR,(void *)FLASH_APP_ADDR,VECTOR_SIZE);
    //设置RAM映射到地址0x00000000
    SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
}

//写入升级标志
void BoardIapWriteUpdateFlag(void)
{
    uint8_t iapFlag = 0xA5;
    BoardMB85RS256_WriteBuffer(APP_UPDATE_FLAG_ADDR,&iapFlag,1);
}

//清空升级标志
void BoardIapClearUpdateFlag(void)
{
    uint8_t iapFlag = 0x00;
    BoardMB85RS256_WriteBuffer(APP_UPDATE_FLAG_ADDR,&iapFlag,1);   
}

//检查升级标志
uint8_t BoardIapCheckUpdateFlag(void)
{
    uint8_t iapFlag = 0;
    //读Iap标志
    BoardMB85RS256_ReadBuffer(APP_UPDATE_FLAG_ADDR,&iapFlag,1);
    if(iapFlag == 0XA5)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//IAP初始化
void BoardIapInit(void)
{
    uint8_t iapFlag = 1;
    BoardMB85RS256_ReadBuffer(APP_UPDATE_FLAG_ADDR,&iapFlag,1);
    //跳转标志错误
    if((iapFlag != 0XA5) && (iapFlag != 0))
    {
        BoardIapClearUpdateFlag();
        BoardIapJumpApp();
    }
    else if(iapFlag == 0)
    {
        BoardIapJumpApp();
    }
    else
    {
        //有升级标志进入Boot
        return;
    }
}