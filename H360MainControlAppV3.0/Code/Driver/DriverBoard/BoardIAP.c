#include "BoardIAP.h"
#include "BoardRS.h"
#ifdef RUN_AT_APP
#include "ServiceImplFatFsLog.h"
#endif
//跳转函数原型
typedef void (*SystemJmpAddressFuncPtr)(void);

//主板IAP模块初始化,包括IAP专属内存的处理和初始化跳转的功能
BOARD_IAP_JUMP_TO_BOOT_CASE BoardIAP_Init(void)
{
    //检测是否需要IAP
    uint8_t iapFlag = 0XA5;
    uint8_t jumpFlag = 0x5A;
    //读取跳转标志位的数据
    BoardMB85RS2MT_ReadBuffer(MB85RS2MT_ADDR_JUMP,&jumpFlag,1);
    //强制跳转过来的,初始上电为0不影响
    if(jumpFlag == 0x5A)
    {
        //清除强制跳转标记
        BoardIAP_ClearJumpFlag();
        //不需要判断是否跳转到APP了
        return BOARD_IAP_JUMP_TO_BOOT_CASE_FORCE;
    }


    //读取IAP标志位的数据
    BoardMB85RS2MT_ReadBuffer(MB85RS2MT_ADDR_IAP,&iapFlag,1);
    //初次烧写的程序,没有数据
    if((iapFlag != 0)&&(iapFlag != 0XA5))
    {
        BoardIAP_ClearFlag();
        iapFlag = 0;
    }
    if(iapFlag == 0)
    {
        //此时没有IAP标志,切换进入APP
        //当APP程序跳转进入IAP流程的时候,将该位置的数据设置为1
        //那么将进入IAP程序 否则将直接进一步调度进入APP
        CoreDelayMs(3000);
        BoardIAP_Jump2Application();
    }
    else
    {
        //此时有IAP表示,切换进入IAP,此时需要上报准备完成
        return BOARD_IAP_JUMP_TO_BOOT_CASE_UPDATE;
    }
    //跳转到APP失败,等待进一步升级指示
    return BOARD_IAP_JUMP_TO_BOOT_CASE_NONE;
}

//检测APP是否存在
uint8_t BoardIAP_CheckAppExist(void)
{
    if(((*(volatile uint32_t*)FLASH_APP_ADDR)&0x1FFE0000)==0x20000000)
    {
        //APP存在
        return 1;
    }
    else
    {
        //APP不存在
        return 0;
    }
}

void jump_to_app(uint32_t app_address)
{
    uint32_t stackPointAddr = ((*(volatile uint32_t*)app_address)&0xF0000000);
    //检查栈顶地址是否合法.0x20000000是CCRAM的起始地址,也是程序的栈顶地址
    //如果这个位置的数据不对,那么说明没有APP
    if(stackPointAddr == 0x20000000)	
    {
        #ifdef RUN_AT_APP
        SystemPrintf("System Jmp APP\r\n");
        #else

        #endif
        //关闭中断
        INTX_DISABLE();
        //如果存在MPU,关闭MPU
        MPU->CTRL &= ~MPU_CTRL_ENABLE_Msk;
        //关闭并清除所有的系统中断
        for(uint8_t i = 0; i < 8; i++)
        {
            //关闭中断
            NVIC->ICER[i] = 0xFFFFFFFF;
            //清除中断
            NVIC->ICPR[i] = 0xFFFFFFFF;
        }
        //设置新的中断向量
        SCB->VTOR = app_address;
        //设置中断掩码寄存器的状态和错误状态寄存器的状态
        __set_BASEPRI(0);
        __set_FAULTMASK(1);
        //设置特权级和用户级下的堆栈指针
        __set_MSP(*(uint32_t*)app_address);
        __set_PSP(*(uint32_t*)app_address);
        //强制切换到特权级
        __set_CONTROL(0);
        //系统刷新,保证设置生效
        __ISB();
        //跳转
        ((SystemJmpAddressFuncPtr)(*(uint32_t*)(app_address + 4)))();
    }
    else
    {
        #ifdef RUN_AT_APP
        SystemPrintf("Application Not Exist\r\n");
        #else

        #endif
    }
}

void jump_to_iap(uint32_t app_address)
{
    uint32_t stackPointAddr = ((*(volatile uint32_t*)app_address)&0xF0000000);
    //检查栈顶地址是否合法.0x20000000是CCRAM的起始地址,也是程序的栈顶地址
    //如果这个位置的数据不对,那么说明没有APP
    if(stackPointAddr == 0x20000000)	
    {
        #ifdef RUN_AT_APP
        SystemPrintf("System Jmp IAP\r\n");
        #else

        #endif
        //关闭中断
        INTX_DISABLE();
        //如果存在MPU,关闭MPU
        MPU->CTRL &= ~MPU_CTRL_ENABLE_Msk;
        //关闭并清除所有的系统中断
        for(uint8_t i = 0; i < 8; i++)
        {
            //关闭中断
            NVIC->ICER[i] = 0xFFFFFFFF;
            //清除中断
            NVIC->ICPR[i] = 0xFFFFFFFF;
        }
        //设置新的中断向量
        SCB->VTOR = app_address;
        //设置中断掩码寄存器的状态和错误状态寄存器的状态
        __set_BASEPRI(0);
        __set_FAULTMASK(1);
        //系统复位
        HAL_NVIC_SystemReset();
    }
    else
    {
        #ifdef RUN_AT_APP
        SystemPrintf("Bootloader Not Exist\r\n");
        #else

        #endif
    }
}

//写入跳转标志,用于程序跳转
void BoardIAP_WriteJumpFlag()
{
    uint8_t iapFlag = 0X5A;
    BoardMB85RS2MT_WriteBuffer(MB85RS2MT_ADDR_JUMP,&iapFlag,1);
}

//清除跳转标志,用于程序跳转
void BoardIAP_ClearJumpFlag()
{
    uint8_t iapFlag = 0X00;
    BoardMB85RS2MT_WriteBuffer(MB85RS2MT_ADDR_JUMP,&iapFlag,1);
}

//读取强制跳转标记
uint8_t BoardIAP_ReadJumpFlag()
{
    uint8_t iapFlag = 0X00;
    BoardMB85RS2MT_ReadBuffer(MB85RS2MT_ADDR_JUMP,&iapFlag,1);
    return iapFlag;
}


//写入在线升级标识,从APP跳转到IAP使用
void BoardIAP_WriteFlag()
{
    uint8_t iapFlag = 0XA5;
    BoardMB85RS2MT_WriteBuffer(MB85RS2MT_ADDR_IAP,&iapFlag,1);
}

//清除在线升级标识,从IAP跳转到APP使用
void BoardIAP_ClearFlag()
{
    uint8_t iapFlag = 0X00;
    BoardMB85RS2MT_WriteBuffer(MB85RS2MT_ADDR_IAP,&iapFlag,1);
}



//应用程序跳转到引导程序中
void BoardIAP_Jump2BootLoader(void)
{
    jump_to_iap(FLASH_IAP_ADDR);//跳转到iap的复位向量地址
}

											

//引导程序跳转到应用中
void BoardIAP_Jump2Application(void)
{
    //跳转到app的复位向量地址
    jump_to_app(FLASH_APP_ADDR);
}
