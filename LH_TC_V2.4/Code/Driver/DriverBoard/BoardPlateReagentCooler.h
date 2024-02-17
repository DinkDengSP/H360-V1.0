#ifndef _BOARD_PLATE_REAGENT_COOLER_H_
#define _BOARD_PLATE_REAGENT_COOLER_H_
#include "BoardCommBase.h"
//RS485状态
typedef enum BOARD_RS485_STATE{
    BOARD_RS485_STATE_SEND,
    BOARD_RS485_STATE_SEND_END,
    BOARD_RS485_STATE_RECV,
}BOARD_RS485_STATE;
//RS485接收状态
typedef enum BOARD_RS485_RECV_STATE{
    RECV_STATE_HEAD = 0x00,
    RECV_STATE_LEN,
    RECV_STATE_ADDR,
    RECV_STATE_DATA,
    RECV_STATE_CRC,
    RECV_STATE_END,
    RECV_STATE_OK,
}BOARD_RS485_RECV_STATE;
//主板发送状态
typedef enum BOARD_COOOLER_SEND_STATE{
    BOARD_COOOLER_SEND_STATE_IDLE       = 0x00,
    BOARD_COOOLER_SEND_STATE_OPEN       = 0x01,
    BOARD_COOOLER_SEND_STATE_CLOSE      = 0x02,
    BOARD_COOOLER_SEND_STATE_INQUIRE    = 0x03,
    BOARD_COOOLER_SEND_STATE_SETTEMP    = 0x04,
}BOARD_COOOLER_SEND_STATE;
//设置命令
typedef struct BOARD_COOLER_SET_CMD{
    uint8_t head;       //帧头
    uint8_t len;        //帧长
    uint8_t addr;       //地址
    uint8_t cmdCode;    //命令码
    uint8_t paramH;     //参数高地址
    uint8_t paramL;     //参数低地址
    uint8_t crcCode;    //校验码
    uint8_t tail;       //帧尾
}BOARD_COOLER_SET_CMD;
//查询命令
typedef struct BOARD_COOLER_INQUIRE_CMD{
    uint8_t head;       //帧头
    uint8_t len;        //帧长
    uint8_t addr;       //地址
    uint8_t cmdCode;    //命令码
    uint8_t devState;   //设备状态
    uint8_t currenttempH;     //参数高地址
    uint8_t currenttempL;     //参数低地址
    uint8_t motorRPMH;     //参数高地址
    uint8_t motorRPML;     //参数低地址
    uint8_t setTempH;     //参数高地址
    uint8_t setTempL;     //参数低地址
    uint8_t crcCode;    //校验码
    uint8_t tail;       //帧尾
}BOARD_COOLER_INQUIRE_CMD;

//串口接收缓冲区大小
#define UART2_RECV_BUFF_MAX_LEN         100
//帧头设置
#define RS485_COOLER_COMM_STX           0XC0
//地址
#define RS485_COOLER_COMM_ADDR          0x01
//返回地址
#define RS485_COOLER_ACK_ADDR           0xFF
//设置命令长度
#define RS485_COOLER_COMM_SET_CMD_LEN       0x08
//结尾
#define RS485_COOLER_COMM_END           0x01

//串口接收数据长度
#define BOARD_COOLER_RECV_DATA_LENGTH           100
//制冷机接收状态
typedef enum BOARD_RECV_STATE
{
    BOARD_RECV_STATE_RECV   = 0,
    BOARD_RECV_STATE_OK     = 1,
    BOARD_RECV_STATE_ERR    = 2,
}BOARD_RECV_STATE;

//制冷机接收数据状态
typedef struct BOARD_COOLER_RECV_UNIT{
    uint16_t recvLen;
    uint16_t expectLen;
    uint8_t dataBuf[BOARD_COOLER_RECV_DATA_LENGTH];
    BOARD_RECV_STATE RecvState;
}BOARD_COOLER_RECV_UNIT;


//----------------------------------------函数接口-------------------------------------------
//RS485引脚初始化
void BoardRS485Init(void);
//设置制冷水箱温度,实际温度
LH_ERR BoardPlateReagentCoolerSetTemp(int16_t temp);
//设置制冷机开机
LH_ERR BoardPlateReagentCoolerOpen(void);
//设置制冷机关机
LH_ERR BoardPlateReagentCoolerClose(void);
//设置制冷机状态查询,返回
LH_ERR BoardPlateReagentCoolerStateInquire(uint8_t *systemStatePtr,int16_t *currentTempPtr,int16_t *motorRPMPtr,int16_t *setTempPtr);

#endif