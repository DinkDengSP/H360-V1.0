#include "BoardIAP.h"
#include "os.h"	
#include "BoardRS.h"
#include "BoardMB85RS2MT.h"

//跳转函数原型
typedef void (*SystemJmpAddressFuncPtr)(void);

//主板IAP模块初始化,包括IAP专属内存的处理和初始化跳转的功能
BOARD_IAP_JUMP_TO_BOOT_CASE BoardIAP_Init(void)
{
    //检测是否需要IAP
    uint8_t iapFlag = 1;
    uint8_t jumpFlag = 0x5A;
    //如果是跳转过来的,则停留在Boot里
    jumpFlag = BoardIAP_ReadJumpFlag();
    if(jumpFlag == 0x5A)
    {
        BoardIAP_ClearJumpFlag();
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
    if(((*(vu32*)FLASH_APP_ADDR)&0x1FFE0000)==0x10000000)
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
    //检查栈顶地址是否合法.0x10000000是CCRAM的起始地址,也是程序的栈顶地址
    //如果这个位置的数据不对,那么说明没有APP
    if(((*(vu32*)app_address)&0x1FFE0000)==0x10000000)	
    {
        SystemPrintf("System Jmp APP\r\n");
        CoreDelayMs(10);
        //关闭中断
        INTX_DISABLE();
        //初始化所有的GPIO,但是SWD和SWCLK不能初始化
        GPIO_InitTypeDef GPIO_InitStruct;

        GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
        GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;

        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
        GPIO_InitStruct.GPIO_Pin &= ~(GPIO_Pin_13 | GPIO_Pin_14); /* SWDIO/SWCLK */
        GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
        GPIO_Init(GPIOB, &GPIO_InitStruct);
        GPIO_Init(GPIOC, &GPIO_InitStruct);
        GPIO_Init(GPIOD, &GPIO_InitStruct);
        GPIO_Init(GPIOE, &GPIO_InitStruct);
        GPIO_Init(GPIOF, &GPIO_InitStruct);
        GPIO_Init(GPIOG, &GPIO_InitStruct);
        GPIO_Init(GPIOH, &GPIO_InitStruct);
        GPIO_Init(GPIOI, &GPIO_InitStruct);
        //系统滴答定时器复位
        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
        //关闭其余所有外设的时钟
        RCC->AHB1ENR = (1<<20); /* 20: F4 CCMDAT ARAMEN. */
        RCC->AHB2ENR = 0;
        RCC->AHB3ENR = 0;
        RCC->APB1ENR = 0;
        RCC->APB2ENR = 0;
        //切换成CPU默认时钟
        RCC->CFGR = 0;
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
        __set_FAULTMASK(0);
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
        SystemPrintf("Application Not Exist\r\n");
    }
}

void jump_to_iap(uint32_t app_address)
{
    //检查栈顶地址是否合法.0x10000000是CCRAM的起始地址,也是程序的栈顶地址
    //如果这个位置的数据不对,那么说明没有APP
    if(((*(vu32*)app_address)&0x1FFE0000)==0x10000000)	
    {
        SystemPrintf("System Jmp IAP\r\n");
        //关闭中断
        INTX_DISABLE();
        //初始化所有的GPIO,但是SWD和SWCLK不能初始化
        GPIO_InitTypeDef GPIO_InitStruct;

        GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
        GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;

        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
        GPIO_InitStruct.GPIO_Pin &= ~(GPIO_Pin_13 | GPIO_Pin_14); /* SWDIO/SWCLK */
        GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
        GPIO_Init(GPIOB, &GPIO_InitStruct);
        GPIO_Init(GPIOC, &GPIO_InitStruct);
        GPIO_Init(GPIOD, &GPIO_InitStruct);
        GPIO_Init(GPIOE, &GPIO_InitStruct);
        GPIO_Init(GPIOF, &GPIO_InitStruct);
        GPIO_Init(GPIOG, &GPIO_InitStruct);
        GPIO_Init(GPIOH, &GPIO_InitStruct);
        GPIO_Init(GPIOI, &GPIO_InitStruct);
        //系统滴答定时器复位
        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
        //关闭其余所有外设的时钟
        RCC->AHB1ENR = (1<<20); /* 20: F4 CCMDAT ARAMEN. */
        RCC->AHB2ENR = 0;
        RCC->AHB3ENR = 0;
        RCC->APB1ENR = 0;
        RCC->APB2ENR = 0;
        //切换成CPU默认时钟
        RCC->CFGR = 0;
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
        __set_FAULTMASK(0);
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
        SystemPrintf("Bootloader Not Exist\r\n");
    }
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
    uint8_t iapFlag = 0;
    BoardMB85RS2MT_WriteBuffer(MB85RS2MT_ADDR_IAP,&iapFlag,1);
}

//写入程序跳转标志
void BoardIAP_WriteJumpFlag()
{
    uint8_t jumpFlag = 0X5A;
    BoardMB85RS2MT_WriteBuffer(MB85RS2MT_ADDR_JUMP,&jumpFlag,1);
}

//读取程序跳转标志
uint8_t BoardIAP_ReadJumpFlag()
{
    uint8_t jumpFlag;
    BoardMB85RS2MT_ReadBuffer(MB85RS2MT_ADDR_JUMP,&jumpFlag,1);
    return jumpFlag;
}

//清除程序跳转标志
void BoardIAP_ClearJumpFlag()
{
    uint8_t jumpFlag = 0;
    BoardMB85RS2MT_WriteBuffer(MB85RS2MT_ADDR_JUMP,&jumpFlag,1);
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
