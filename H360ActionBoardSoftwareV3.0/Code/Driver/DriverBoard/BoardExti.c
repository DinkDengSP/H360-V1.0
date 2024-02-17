#include "BoardExti.h"
#include "os.h"

/**因为中断2被占用,所以2以后的都错一位**/

//指定通道的外部中断回调函数是否被设置
static BoardExtiTrigCallBackFuncPtr userExtiCallBackArray[15] = {NULL};


#define BOARD_EXTI_WITH_NONE_IN     255
//指定通道的外部中断当前关联的引脚
static uint8_t userExtiAssociatePinArray[15] = {BOARD_EXTI_WITH_NONE_IN};

//无关联
#define EXTI_Line_None          ((uint32_t)0x80000000)
#define EXTI_PORT_SRC_None      ((uint8_t)0xff)
#define EXTI_PIN_SRC_None       ((uint8_t)0xff)

//设定板上输入与外部中断线之间的关联数组
typedef struct BoardInWithExtiPort
{
    BOARD_IN_PIN inPin;//输入引脚
    uint32_t extiLine;//中断引脚
    uint8_t extiPortSource;//中断端口
    uint8_t extiPinSource;//中断线
    uint8_t callBackIndex;//回调函数以及关联引脚的序号
}BoardInWithExtiPort;

//板上输入与中断端口的配置文件
static const BoardInWithExtiPort BoardInWithExtiPortConfigArray[] = {
    /**输入端口号定义**********************外部中断信号定义*****外部中断端口定义*******外部中断引脚定义****数组序号*/
    {BOARD_IN_CPLD_NOP1                  ,EXTI_Line_None , EXTI_PORT_SRC_None,   EXTI_PIN_SRC_None,    255       },
    {BOARD_IN_CPLD_NOP2                  ,EXTI_Line_None , EXTI_PORT_SRC_None,   EXTI_PIN_SRC_None,    255       },
    {BOARD_IN_CPLD_NOP3                  ,EXTI_Line_None , EXTI_PORT_SRC_None,   EXTI_PIN_SRC_None,    255       },
    {BOARD_IN_CPLD_NOP4                  ,EXTI_Line_None , EXTI_PORT_SRC_None,   EXTI_PIN_SRC_None,    255       },
    {BOARD_IN_CPLD_NOP5                  ,EXTI_Line_None , EXTI_PORT_SRC_None,   EXTI_PIN_SRC_None,    255       },
    {BOARD_IN_CPLD_NOP6                  ,EXTI_Line_None , EXTI_PORT_SRC_None,   EXTI_PIN_SRC_None,    255       },
    {BOARD_IN_CPLD_NOP7                  ,EXTI_Line_None , EXTI_PORT_SRC_None,   EXTI_PIN_SRC_None,    255       },
    {BOARD_IN_CPLD_NOP8                  ,EXTI_Line_None , EXTI_PORT_SRC_None,   EXTI_PIN_SRC_None,    255       },
    {BOARD_IN_CPLD_NOP9                  ,EXTI_Line_None , EXTI_PORT_SRC_None,   EXTI_PIN_SRC_None,    255       },
    {BOARD_IN_CPLD_NOP10                 ,EXTI_Line_None , EXTI_PORT_SRC_None,   EXTI_PIN_SRC_None,    255       },
    {BOARD_IN_CPLD_NOP11                 ,EXTI_Line_None , EXTI_PORT_SRC_None,   EXTI_PIN_SRC_None,    255       },
    {BOARD_IN_CPLD_NOP12                 ,EXTI_Line_None , EXTI_PORT_SRC_None,   EXTI_PIN_SRC_None,    255       },
    {BOARD_IN_CPLD_NOP13                 ,EXTI_Line_None , EXTI_PORT_SRC_None,   EXTI_PIN_SRC_None,    255       },
    {BOARD_IN_MCU_ARM_SEN1_PI10          ,EXTI_Line10    , EXTI_PortSourceGPIOI, EXTI_PinSource10,     9         },
    {BOARD_IN_MCU_ARM_SEN2_PI9           ,EXTI_Line9     , EXTI_PortSourceGPIOI, EXTI_PinSource9,      8         },
    {BOARD_IN_MCU_ARM_SEN3_PC13          ,EXTI_Line13    , EXTI_PortSourceGPIOC, EXTI_PinSource13,     12        },
    {BOARD_IN_MCU_ARM_SEN4_PI8           ,EXTI_Line8     , EXTI_PortSourceGPIOI, EXTI_PinSource8,      7         },
    {BOARD_IN_MCU_ARM_SEN5_PE6           ,EXTI_Line6     , EXTI_PortSourceGPIOE, EXTI_PinSource6,      5         },
    {BOARD_IN_MCU_ARM_SEN6_PE5           ,EXTI_Line5     , EXTI_PortSourceGPIOE, EXTI_PinSource5,      4         },
    {BOARD_IN_MCU_ARM_SEN7_PE4           ,EXTI_Line4     , EXTI_PortSourceGPIOE, EXTI_PinSource4,      3         },
    {BOARD_IN_MCU_ARM_SEN8_PE3           ,EXTI_Line3     , EXTI_PortSourceGPIOE, EXTI_PinSource3,      2         },
    {BOARD_IN_MCU_ARM_SEN9_PE2           ,EXTI_Line_None , EXTI_PortSourceGPIOE, EXTI_PinSource2,      255       },
    {BOARD_IN_MCU_ARM_SEN10_PI7          ,EXTI_Line7     , EXTI_PortSourceGPIOI, EXTI_PinSource7,      6         },
    {BOARD_IN_MCU_ARM_SEN11_PI6          ,EXTI_Line6     , EXTI_PortSourceGPIOI, EXTI_PinSource6,      5         },
    {BOARD_IN_MCU_ARM_SEN12_PI5          ,EXTI_Line5     , EXTI_PortSourceGPIOI, EXTI_PinSource5,      4         },
    {BOARD_IN_MCU_ARM_SEN13_PI4          ,EXTI_Line4     , EXTI_PortSourceGPIOI, EXTI_PinSource4,      3         },
    {BOARD_IN_MCU_ARM_SEN14_PB9          ,EXTI_Line9     , EXTI_PortSourceGPIOB, EXTI_PinSource9,      8         },
    {BOARD_IN_MCU_ARM_SEN15_PB8          ,EXTI_Line8     , EXTI_PortSourceGPIOB, EXTI_PinSource8,      7         },
    {BOARD_IN_MCU_ARM_SEN16_PB7          ,EXTI_Line7     , EXTI_PortSourceGPIOB, EXTI_PinSource7,      6         },
    {BOARD_IN_MCU_SLAVE_ARM_SEN1_PH12    ,EXTI_Line12    , EXTI_PortSourceGPIOH, EXTI_PinSource12,     11        },
    {BOARD_IN_MCU_SLAVE_ARM_SEN2_PH11    ,EXTI_Line11    , EXTI_PortSourceGPIOH, EXTI_PinSource11,     10        },
    {BOARD_IN_MCU_SLAVE_ARM_SEN3_PH10    ,EXTI_Line10    , EXTI_PortSourceGPIOH, EXTI_PinSource10,     9         },
    {BOARD_IN_MCU_SLAVE_ARM_SEN4_PH9     ,EXTI_Line9     , EXTI_PortSourceGPIOH, EXTI_PinSource9,      8         },
    {BOARD_IN_MCU_SLAVE_ARM_SEN5_PH8     ,EXTI_Line8     , EXTI_PortSourceGPIOH, EXTI_PinSource8,      7         },
    {BOARD_IN_MCU_SLAVE_ARM_SEN6_PH7     ,EXTI_Line7     , EXTI_PortSourceGPIOH, EXTI_PinSource7,      6         },
    {BOARD_IN_MCU_SLAVE_ARM_SEN7_PH6     ,EXTI_Line6     , EXTI_PortSourceGPIOH, EXTI_PinSource6,      5         },
    {BOARD_IN_MCU_SLAVE_ARM_SEN8_PB11    ,EXTI_Line11    , EXTI_PortSourceGPIOB, EXTI_PinSource11,     10        },
    {BOARD_IN_MCU_SLAVE_ARM_SEN9_PB10    ,EXTI_Line10    , EXTI_PortSourceGPIOB, EXTI_PinSource10,     9         },
    {BOARD_IN_MCU_SLAVE_ARM_SEN10_PC0    ,EXTI_Line0     , EXTI_PortSourceGPIOC, EXTI_PinSource0,      0         },
    {BOARD_IN_MCU_SLAVE_ARM_SEN11_PC1    ,EXTI_Line1     , EXTI_PortSourceGPIOC, EXTI_PinSource1,      1         },
    {BOARD_IN_MCU_SLAVE_ARM_SEN12_PC2    ,EXTI_Line_None , EXTI_PortSourceGPIOC, EXTI_PinSource2,      255       },
    {BOARD_IN_MCU_SLAVE_ARM_SEN13_PC3    ,EXTI_Line3     , EXTI_PortSourceGPIOC, EXTI_PinSource3,      2         },
    {BOARD_IN_MCU_SLAVE_ARM_SEN14_PB1    ,EXTI_Line1     , EXTI_PortSourceGPIOB, EXTI_PinSource1,      1         },
    {BOARD_IN_MCU_SLAVE_ARM_SEN15_PB0    ,EXTI_Line0     , EXTI_PortSourceGPIOB, EXTI_PinSource0,      0         },
    {BOARD_IN_MCU_SLAVE_ARM_SEN16_PC5    ,EXTI_Line5     , EXTI_PortSourceGPIOC, EXTI_PinSource5,      4         },
    {BOARD_IN_MCU_FRUBBISH1_PH15         ,EXTI_Line15    , EXTI_PortSourceGPIOH, EXTI_PinSource15,     14        },
    {BOARD_IN_MCU_FRUBBISH2_PH14         ,EXTI_Line14    , EXTI_PortSourceGPIOH, EXTI_PinSource14,     13        },
    {BOARD_IN_MCU_FRUBBISH3_PH13         ,EXTI_Line13    , EXTI_PortSourceGPIOH, EXTI_PinSource13,     12        },
    {BOARD_IN_MCU_FRUBBISH4_PC9          ,EXTI_Line9     , EXTI_PortSourceGPIOC, EXTI_PinSource9,      8         },
    {BOARD_IN_MCU_FRUBBISH_OVER1_PI0     ,EXTI_Line0     , EXTI_PortSourceGPIOI, EXTI_PinSource0,      0         },
    {BOARD_IN_MCU_FRUBBISH_OVER2_PI1     ,EXTI_Line1     , EXTI_PortSourceGPIOI, EXTI_PinSource1,      1         },
    {BOARD_IN_MCU_PR1_ARM1_PG14          ,EXTI_Line14    , EXTI_PortSourceGPIOG, EXTI_PinSource14,     13        },
    {BOARD_IN_MCU_PR1_ARM2_PG13          ,EXTI_Line13    , EXTI_PortSourceGPIOG, EXTI_PinSource13,     12        },
    {BOARD_IN_MCU_PR2_ARM1_PG11          ,EXTI_Line11    , EXTI_PortSourceGPIOG, EXTI_PinSource11,     10        },
    {BOARD_IN_MCU_PR2_ARM2_PG10          ,EXTI_Line10    , EXTI_PortSourceGPIOG, EXTI_PinSource10,     9         },
    {BOARD_IN_MCU_YM1_ARM1_PB6           ,EXTI_Line6     , EXTI_PortSourceGPIOB, EXTI_PinSource6,      5         },
    {BOARD_IN_MCU_YM1_ARM2_PB5           ,EXTI_Line5     , EXTI_PortSourceGPIOB, EXTI_PinSource5,      4         },
};

//系统中断线
static const uint32_t EXTI_LineArray[] = {
    EXTI_Line0,
    EXTI_Line1,
    EXTI_Line3,
    EXTI_Line4,
    EXTI_Line5,
    EXTI_Line6,
    EXTI_Line7,
    EXTI_Line8,
    EXTI_Line9,
    EXTI_Line10,
    EXTI_Line11,
    EXTI_Line12,
    EXTI_Line13,
    EXTI_Line14,
    EXTI_Line15,
};

//板上中断全部初始化
void BoardExtiResetAll(void)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;
    uint8_t i = 0;
    //回调函数和引脚IO配置设置为0
    for(i = 0; i < 15; i++)
    {
        userExtiCallBackArray[i] = NULL;
    }
    for(i = 0; i < 15; i++)
    {
        userExtiAssociatePinArray[i] = BOARD_EXTI_WITH_NONE_IN;
    }

    //将所有的中断线都关闭掉
    for(i= 0; i < (sizeof(EXTI_LineArray)/sizeof(EXTI_LineArray[0]));i++)
    {
        EXTI_StructInit(&EXTI_InitStructure);
        //强行关闭外部中断
        EXTI_InitStructure.EXTI_Line = EXTI_LineArray[i];
        //中断线关闭
        EXTI_InitStructure.EXTI_LineCmd = DISABLE;
        EXTI_Init(&EXTI_InitStructure);
    }
    //将所有的外部中断都打开
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USER_EXT_INT_PREE_PRI;
	//子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USER_EXT_INT_SUB_PRI;
	//使能外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//配置
	NVIC_Init(&NVIC_InitStructure);

    //将所有的外部中断都打开
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USER_EXT_INT_PREE_PRI;
	//子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USER_EXT_INT_SUB_PRI;
	//使能外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//配置
	NVIC_Init(&NVIC_InitStructure);

    //将所有的外部中断都打开
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USER_EXT_INT_PREE_PRI;
	//子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USER_EXT_INT_SUB_PRI;
	//使能外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//配置
	NVIC_Init(&NVIC_InitStructure);

    //将所有的外部中断都打开
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USER_EXT_INT_PREE_PRI;
	//子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USER_EXT_INT_SUB_PRI;
	//使能外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//配置
	NVIC_Init(&NVIC_InitStructure);

    //将所有的外部中断都打开
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USER_EXT_INT_PREE_PRI;
	//子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USER_EXT_INT_SUB_PRI;
	//使能外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//配置
	NVIC_Init(&NVIC_InitStructure);

    //将所有的外部中断都打开
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USER_EXT_INT_PREE_PRI;
	//子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USER_EXT_INT_SUB_PRI;
	//使能外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//配置
	NVIC_Init(&NVIC_InitStructure);
}


//外部中断注册
LH_ERR BoardExtiRegister(BOARD_IN_PIN inPin,EXTITrigger_TypeDef trigMode,BoardExtiTrigCallBackFuncPtr callBack)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    BoardInWithExtiPort* extiMapPtr;
    //回调函数不能为空
    if(callBack == NULL)
    {
        return LH_ERR_NONE;
    }
    //引脚要在指定范围内
    if(inPin >= BOARD_IN_PIN_COUNT)
    {
        return LH_ERR_EXT_INT_UNSUPPORT;
    }
    //查看输入引脚的状态
    if(inPin == BoardInWithExtiPortConfigArray[inPin].inPin)
    {
        //取出配置信息
        extiMapPtr = (BoardInWithExtiPort*)(&(BoardInWithExtiPortConfigArray[inPin]));
        //首先查看是否能注册
        if(EXTI_Line_None == extiMapPtr->extiLine)
        {
            return LH_ERR_EXT_INT_UNSUPPORT;
        }
        //然后看其注册讯息是否已经被占用
        if((NULL == userExtiCallBackArray[extiMapPtr->callBackIndex])&&(BOARD_EXTI_WITH_NONE_IN == 
                    userExtiAssociatePinArray[extiMapPtr->callBackIndex]))
        {
            //先屏蔽中断
            CPU_IntDis();
            //可以注册
            userExtiCallBackArray[extiMapPtr->callBackIndex] = callBack;
            userExtiAssociatePinArray[extiMapPtr->callBackIndex] = inPin;
            //开始注册
            SYSCFG_EXTILineConfig(extiMapPtr->extiPortSource, extiMapPtr->extiPinSource);
            //中断线设定
            EXTI_InitStructure.EXTI_Line = extiMapPtr->extiLine;
            //中断事件
            EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
            //下降沿触发
            EXTI_InitStructure.EXTI_Trigger = trigMode; 
            //中断线使能
            EXTI_InitStructure.EXTI_LineCmd = ENABLE;
            //配置
            EXTI_Init(&EXTI_InitStructure);
            //然后打开中断
            CPU_IntEn();

            return LH_ERR_NONE;
        }
        else
        {
            //被占用了
            return LH_ERR_EXT_INT_CHANNEL_BUSY;
        }
    }
    else
    {
        return LH_ERR_EXT_INT_UNSUPPORT;
    }
}

//外部中断解除注册
void BoardExtiUnRegister(BOARD_IN_PIN inPin)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    BoardInWithExtiPort* extiMapPtr;
    //引脚要在指定范围内
    if(inPin >= BOARD_IN_PIN_COUNT)
    {
        return ;
    }
    //查看输入引脚的状态
    if(inPin == BoardInWithExtiPortConfigArray[inPin].inPin)
    {
        //取出配置信息
        extiMapPtr = (BoardInWithExtiPort*)(&(BoardInWithExtiPortConfigArray[inPin]));
        //首先查看是否能注册
        if(EXTI_Line_None == extiMapPtr->extiLine)
        {
            return ;
        }
        //然后看其注册讯息与当前引脚是否匹配
        if(inPin == userExtiAssociatePinArray[extiMapPtr->callBackIndex])
        {
            //先屏蔽中断
            CPU_IntDis();
            //清除内部状态讯息
            userExtiCallBackArray[extiMapPtr->callBackIndex] = NULL;
            userExtiAssociatePinArray[extiMapPtr->callBackIndex] = BOARD_EXTI_WITH_NONE_IN;
            //中断线设定
            EXTI_InitStructure.EXTI_Line = extiMapPtr->extiLine;
            //中断事件
            EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
            //下降沿触发
            EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; 
            //中断线关闭
            EXTI_InitStructure.EXTI_LineCmd = DISABLE;
            //配置
            EXTI_Init(&EXTI_InitStructure);
            //然后打开中断
            CPU_IntEn();
        }
        else
        {
            //被占用了
            return;
        }
    }
    else
    {
        return;
    }
}



//外部中断向量
void EXTI0_IRQHandler()
{
    //中断处理函数写在这里
    if(userExtiCallBackArray[0] != NULL)
    {
        userExtiCallBackArray[0]();
    }

    //清除线上的中断标志位 
    EXTI_ClearITPendingBit(EXTI_Line0); 
}

void EXTI1_IRQHandler()
{
    //中断处理函数写在这里
    if(userExtiCallBackArray[1] != NULL)
    {
        userExtiCallBackArray[1]();
    }

    //清除线上的中断标志位 
    EXTI_ClearITPendingBit(EXTI_Line1); 
}

void EXTI3_IRQHandler()
{
    //中断处理函数写在这里
    if(userExtiCallBackArray[2] != NULL)
    {
        userExtiCallBackArray[2]();
    }

    //清除线上的中断标志位 
    EXTI_ClearITPendingBit(EXTI_Line3); 
}

void EXTI4_IRQHandler()
{
    //中断处理函数写在这里
    if(userExtiCallBackArray[3] != NULL)
    {
        userExtiCallBackArray[3]();
    }

    //清除线上的中断标志位 
    EXTI_ClearITPendingBit(EXTI_Line4); 
}

void EXTI9_5_IRQHandler()
{
    //中断处理函数写在这里
    if(EXTI_GetITStatus(EXTI_Line5) == SET)
    {
        if(userExtiCallBackArray[4] != NULL)
        {
            userExtiCallBackArray[4]();
        }
        //清除线上的中断标志位 
        EXTI_ClearITPendingBit(EXTI_Line5); 
    }

    //中断处理函数写在这里
    if(EXTI_GetITStatus(EXTI_Line6) == SET)
    {
        if(userExtiCallBackArray[5] != NULL)
        {
            userExtiCallBackArray[5]();
        }
        //清除线上的中断标志位 
        EXTI_ClearITPendingBit(EXTI_Line6); 
    }

    //中断处理函数写在这里
    if(EXTI_GetITStatus(EXTI_Line7) == SET)
    {
        if(userExtiCallBackArray[6] != NULL)
        {
            userExtiCallBackArray[6]();
        }
        //清除线上的中断标志位 
        EXTI_ClearITPendingBit(EXTI_Line7); 
    }

    //中断处理函数写在这里
    if(EXTI_GetITStatus(EXTI_Line8) == SET)
    {
        if(userExtiCallBackArray[7] != NULL)
        {
            userExtiCallBackArray[7]();
        }
        //清除线上的中断标志位 
        EXTI_ClearITPendingBit(EXTI_Line8); 
    }

    //中断处理函数写在这里
    if(EXTI_GetITStatus(EXTI_Line9) == SET)
    {
        if(userExtiCallBackArray[8] != NULL)
        {
            userExtiCallBackArray[8]();
        }
        //清除线上的中断标志位 
        EXTI_ClearITPendingBit(EXTI_Line9); 
    }
}

void EXTI15_10_IRQHandler()
{
    //中断处理函数写在这里
    if(EXTI_GetITStatus(EXTI_Line10) == SET)
    {
        if(userExtiCallBackArray[9] != NULL)
        {
            userExtiCallBackArray[9]();
        }
        //清除线上的中断标志位 
        EXTI_ClearITPendingBit(EXTI_Line10);
    }

    //中断处理函数写在这里
    if(EXTI_GetITStatus(EXTI_Line11) == SET)
    {   
        if(userExtiCallBackArray[10] != NULL)
        {
            userExtiCallBackArray[10]();
        }
        //清除线上的中断标志位 
        EXTI_ClearITPendingBit(EXTI_Line11);
    }

    //中断处理函数写在这里
    if(EXTI_GetITStatus(EXTI_Line12) == SET)
    {
        if(userExtiCallBackArray[11] != NULL)
        {
            userExtiCallBackArray[11]();
        }
        //清除线上的中断标志位 
        EXTI_ClearITPendingBit(EXTI_Line12);
    }

    //中断处理函数写在这里
    if(EXTI_GetITStatus(EXTI_Line13) == SET)
    {
        if(userExtiCallBackArray[12] != NULL)
        {
            userExtiCallBackArray[12]();
        }
        //清除线上的中断标志位 
        EXTI_ClearITPendingBit(EXTI_Line13);
    }

    //中断处理函数写在这里
    if(EXTI_GetITStatus(EXTI_Line14) == SET)
    {
        if(userExtiCallBackArray[13] != NULL)
        {
            userExtiCallBackArray[13]();
        }
        //清除线上的中断标志位 
        EXTI_ClearITPendingBit(EXTI_Line14);
    }

    //中断处理函数写在这里
    if(EXTI_GetITStatus(EXTI_Line15) == SET)
    {
        if(userExtiCallBackArray[14] != NULL)
        {
            userExtiCallBackArray[14]();
        }
        //清除线上的中断标志位 
        EXTI_ClearITPendingBit(EXTI_Line15);
    }
}






