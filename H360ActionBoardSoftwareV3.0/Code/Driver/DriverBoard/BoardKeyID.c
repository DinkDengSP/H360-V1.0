#include "BoardKeyID.h"

//主板ID
static volatile BOARD_ID boardID;

//主板ID获取初始化
BOARD_ID BoardKeyID_Init(void)
{
    //board2 bit0 PF7
    MCU_PortInit(MCU_PIN_F_7, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Low_Speed);

    //board1 bit1 PF6
    MCU_PortInit(MCU_PIN_F_6, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Low_Speed);

    //board0 bit2 PI11
    MCU_PortInit(MCU_PIN_I_11, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Low_Speed);

    CoreDelayMs(10);

    boardID = BoardKeyID_GetFlush();
    
    return boardID;
}

//获取当前主板ID,变量获取
BOARD_ID BoardKeyID_GetCurrent(void)
{
    return boardID;
}

//获取当前主板ID,实际获取
BOARD_ID BoardKeyID_GetFlush(void)
{
    uint8_t boardIdLocal;
    BitAction bitValue;
    bitValue = MCU_PortReadSingle(MCU_PIN_I_11);
    boardIdLocal = (uint8_t)bitValue;

    boardIdLocal <<= 1;
    bitValue = MCU_PortReadSingle(MCU_PIN_F_6);
    boardIdLocal += (uint8_t)bitValue;

    boardIdLocal <<= 1;
    bitValue = MCU_PortReadSingle(MCU_PIN_F_7);
    boardIdLocal += (uint8_t)bitValue;
    
    //主板ID号从1开始
    boardIdLocal += 1;
    boardID = (BOARD_ID)boardIdLocal;
    //返回当前板号
    return boardID;
}























