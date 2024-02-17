#include "ServiceImplSerialUserRS1.h"


//调试串口输出一帧数据
void ServiceImplSerialUserRS1WriteBuffer(uint8_t* bufferStartPtr,uint16_t bufferLength)
{
    BoardRS_SendBuffer(BOARD_RS_PORT_1,bufferStartPtr,bufferLength);
}










