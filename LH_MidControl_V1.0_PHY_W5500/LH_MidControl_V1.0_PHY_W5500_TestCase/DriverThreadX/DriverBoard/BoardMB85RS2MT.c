/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-24 18:06:52 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-25 10:56:23 +0800
************************************************************************************************/ 
#include "BoardMB85RS2MT.h"

//芯片选中
static void BoardMB85RS2MT_ChipSelect(void)
{
    MCU_PinWriteSingle(PIN_BOARD_MB85RS2MT_SELECT,GPIO_PIN_RESET);
    CoreDelayUs(10);
}

//芯片取消选中
static void BoardMB85RS2MT_ChipDiselect(void)
{
    MCU_PinWriteSingle(PIN_BOARD_MB85RS2MT_SELECT,GPIO_PIN_SET);
    CoreDelayUs(10);
}

//写使能
static ERROR_SUB BoardMB85RS2MT_WriteEnable(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t cmdArray[1] = {0};
    //锁定SPI接口
    MCU_SPI2_GetLock();
    //芯片选中
    BoardMB85RS2MT_ChipSelect();
    //写使能
    cmdArray[0] = MB85RS2MT_CMD_WREN;
    errorSub = MCU_SPI2_ReadWriteBuffer(cmdArray,SPI_PTR_NULL,1);
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        //释放芯片
        BoardMB85RS2MT_ChipDiselect();
        //释放锁
        MCU_SPI2_ReleaseLock();
        return errorSub;
    }
    //释放芯片
    BoardMB85RS2MT_ChipDiselect();
    //释放锁
    MCU_SPI2_ReleaseLock();
    return ERROR_SUB_OK;
}

//写禁止
ERROR_SUB BoardMB85RS2MT_WriteDisable(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t cmdArray[1] = {0};
    //锁定SPI接口
    MCU_SPI2_GetLock();
    //芯片选中
    BoardMB85RS2MT_ChipSelect();
    //写使能
    cmdArray[0] = MB85RS2MT_CMD_WRDI;
    errorSub = MCU_SPI2_ReadWriteBuffer(cmdArray,SPI_PTR_NULL,1);
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        //释放芯片
        BoardMB85RS2MT_ChipDiselect();
        //释放锁
        MCU_SPI2_ReleaseLock();
        return errorSub;
    }
    //释放芯片
    BoardMB85RS2MT_ChipDiselect();
    //释放锁
    MCU_SPI2_ReleaseLock();
    return ERROR_SUB_OK;
}

//写状态寄存器
static ERROR_SUB BoardMB85RS2MT_WriteStateReg(uint8_t regValue)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t cmdArray[2] = {0};
    //锁定SPI接口
    MCU_SPI2_GetLock();
    //写入使能
    errorSub = BoardMB85RS2MT_WriteEnable();
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        //释放锁
        MCU_SPI2_ReleaseLock();
        return errorSub;
    }
    //芯片选中
    BoardMB85RS2MT_ChipSelect();
    //写寄存器命令
    cmdArray[0] = MB85RS2MT_CMD_WRSR;
    cmdArray[1] = regValue;
    errorSub = MCU_SPI2_ReadWriteBuffer(cmdArray,SPI_PTR_NULL,2);
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        //释放芯片
        BoardMB85RS2MT_ChipDiselect();
        //释放锁
        MCU_SPI2_ReleaseLock();
        return errorSub;
    }
    //释放芯片
    BoardMB85RS2MT_ChipDiselect();
    //释放锁
    MCU_SPI2_ReleaseLock();
    return ERROR_SUB_OK;
}

//读状态寄存器
static ERROR_SUB BoardMB85RS2MT_ReadStateReg(uint8_t* stateRegValuePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t cmdArray[1] = {0};
    //锁定SPI接口
    MCU_SPI2_GetLock();
    //芯片选中
    BoardMB85RS2MT_ChipSelect();
    //读取状态寄存器
    cmdArray[0] = MB85RS2MT_CMD_RDSR;
    errorSub = MCU_SPI2_ReadWriteBuffer(cmdArray,SPI_PTR_NULL,1);
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        //释放芯片
        BoardMB85RS2MT_ChipDiselect();
        //释放锁
        MCU_SPI2_ReleaseLock();
        return errorSub;
    }
    errorSub = MCU_SPI2_ReadWriteBuffer(SPI_PTR_NULL,stateRegValuePtr,1);
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        //释放芯片
        BoardMB85RS2MT_ChipDiselect();
        //释放锁
        MCU_SPI2_ReleaseLock();
        return errorSub;
    }
    //释放芯片
    BoardMB85RS2MT_ChipDiselect();
    //释放锁
    MCU_SPI2_ReleaseLock();
    return ERROR_SUB_OK;
}

//初始化铁电存储器IO口
void BoardMB85RS2MT_PortInit(void)
{
    //初始化选中端口
    MCU_PinInit(PIN_BOARD_MB85RS2MT_SELECT, MCU_PIN_MODE_OUTPUT_PP, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_HIGH,0);
    //设定不选中
    BoardMB85RS2MT_ChipDiselect();
    //初始化SPI
    MCU_SPI2_Init(MB85RS2MT_SPI_CLK,MCU_SPI_CPOL_HIGH,MB85RS2MT_SPI_CPHA);
}

//读取芯片ID
ERROR_SUB BoardMB85RS2MT_ReadID(uint32_t *id)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *id = 0XFFFFFFFF;
    uint8_t* idTempArray= (uint8_t*)id;
    uint8_t cmd = MB85RS2MT_CMD_RDID;
    //锁定SPI接口
    MCU_SPI2_GetLock();
    //芯片选中
    BoardMB85RS2MT_ChipSelect();
    //发送CMD
    errorSub = MCU_SPI2_ReadWriteBuffer(&cmd,SPI_PTR_NULL,1);
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        //释放芯片
        BoardMB85RS2MT_ChipDiselect();
        //释放锁
        MCU_SPI2_ReleaseLock();
        return errorSub;
    }
    //读取ID缓存,直到收到首个字节为ID匹配字节
    do
    {
        errorSub = MCU_SPI2_ReadWriteBuffer(SPI_PTR_NULL,idTempArray,1);
        if(errorSub != ERROR_SUB_OK)
        {
            //释放芯片
            BoardMB85RS2MT_ChipDiselect();
            //释放锁
            MCU_SPI2_ReleaseLock();
            return errorSub;
        }
    }while(idTempArray[0] != (uint8_t)(MB85RS2MT_ID));
    //读取剩下的ID
    errorSub = MCU_SPI2_ReadWriteBuffer(SPI_PTR_NULL,idTempArray+1,3);
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        //释放芯片
        BoardMB85RS2MT_ChipDiselect();
        //释放锁
        MCU_SPI2_ReleaseLock();
        return errorSub;
    }
    //释放芯片
    BoardMB85RS2MT_ChipDiselect();
    //释放锁
    MCU_SPI2_ReleaseLock();
    return ERROR_SUB_OK;
}

//清除保护
ERROR_SUB BoardMB85RS2MT_ClearProtect(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t stateRegValue = 0X00;
    //锁定SPI接口
    MCU_SPI2_GetLock();
    //读取状态
    errorSub = BoardMB85RS2MT_ReadStateReg(&stateRegValue);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放锁
        MCU_SPI2_ReleaseLock();
        return errorSub;
    }
    if((stateRegValue & MB85RS2MT_BLOCK_PORTECT_UPPER_ALL) != 0)
    {
        //存在保护,清除保护
        LowLevelPrintf("BoardMB85RS2MT_ClearProtect State: 0X%02X\r\n",stateRegValue);
        //清除写保护
        stateRegValue &= (~MB85RS2MT_BLOCK_PORTECT_UPPER_ALL);
        errorSub = BoardMB85RS2MT_WriteStateReg(stateRegValue);
        if(errorSub != ERROR_SUB_OK)
        {
            //释放锁
            MCU_SPI2_ReleaseLock();
            return errorSub;
        }
        //清除状态
        stateRegValue = 0X00;
        errorSub = BoardMB85RS2MT_ReadStateReg(&stateRegValue);
        if(errorSub != ERROR_SUB_OK)
        {
            //释放锁
            MCU_SPI2_ReleaseLock();
            return errorSub;
        }
        if((stateRegValue & MB85RS2MT_BLOCK_PORTECT_UPPER_ALL) != 0)
        {
            //如果任然有效,那么清除失败,释放锁
            MCU_SPI2_ReleaseLock();
            return ERROR_SUB_BOARD_FRAM_CLEAR_PROTECT;
        }
    }
    //释放锁
    MCU_SPI2_ReleaseLock();
    return ERROR_SUB_OK;
}

//初始化铁电存储器
ERROR_SUB BoardMB85RS2MT_Check(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //首先读取ID
    uint32_t chipID = 0X00000000;
    errorSub = BoardMB85RS2MT_ReadID(&chipID);
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //ID匹配
    if((chipID&0x000FFFFF) != MB85RS2MT_ID)
    {
        LowLevelShowErrorSubCode(ERROR_SUB_BOARD_FRAM_ID_MATCH);
        //ID不匹配
        return ERROR_SUB_BOARD_FRAM_ID_MATCH;
    }
    //取消写保护
    errorSub = BoardMB85RS2MT_ClearProtect();
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    uint32_t srcDat = 0;
    uint32_t writeDat = (uint32_t)MCU_RandomGetNextUnsigned(0,0X0FFFFFFF);
    uint32_t readDat = 0;
    uint32_t testAddr = (uint32_t)MCU_RandomGetNextUnsigned(MB85RS2MT_SELF_CHECK_REGION_ADDR_MIN,
                                                    MB85RS2MT_SELF_CHECK_REGION_ADDR_MAX);
    //自检开始
    LowLevelPrintf("%s Start,Addr: 0X%08X\r\n",__func__,testAddr);
    //锁定SPI接口
    MCU_SPI2_GetLock();
    //读取源
    errorSub = BoardMB85RS2MT_ReadBuffer(testAddr,(uint8_t*)(&srcDat),4);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放锁
        MCU_SPI2_ReleaseLock();
        //错误日志打印
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //写入新的数据
    errorSub = BoardMB85RS2MT_WriteBuffer(testAddr,(uint8_t*)(&writeDat),4);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放锁
        MCU_SPI2_ReleaseLock();
        //错误日志打印
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //读取出来
    errorSub = BoardMB85RS2MT_ReadBuffer(testAddr,(uint8_t*)(&readDat),4);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放锁
        MCU_SPI2_ReleaseLock();
        //错误日志打印
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //比较数据
    if(readDat != writeDat)
    {
        //释放锁
        MCU_SPI2_ReleaseLock();
        //错误日志打印
        LowLevelShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_FRAM_CHECK_FRIST;
    }
    //原始数据写回去
    errorSub = BoardMB85RS2MT_WriteBuffer(testAddr,(uint8_t*)(&srcDat),4);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放锁
        MCU_SPI2_ReleaseLock();
        //错误日志打印
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //设置异常值
    readDat = 0XAA5555AA;
    //读取出来
    errorSub = BoardMB85RS2MT_ReadBuffer(testAddr,(uint8_t*)(&readDat),4);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放锁
        MCU_SPI2_ReleaseLock();
        //错误日志打印
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    //比较数据
    if(readDat != srcDat)
    {
        //释放锁
        MCU_SPI2_ReleaseLock();
        //错误日志打印
        LowLevelShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_FRAM_CHECK_SECOND;
    }
    //释放锁
    MCU_SPI2_ReleaseLock();
    //成功
    LowLevelPrintf("%s Success Over\r\n",__func__);
    return ERROR_SUB_OK;
}

//写缓存
ERROR_SUB BoardMB85RS2MT_WriteBuffer(uint32_t address, uint8_t *pdata, uint32_t length)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t cmdArray[4] = {0};
    //锁定SPI接口
    MCU_SPI2_GetLock();
    //写入使能
    errorSub = BoardMB85RS2MT_WriteEnable();
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        //释放锁
        MCU_SPI2_ReleaseLock();
        return errorSub;
    }
    //芯片选中
    BoardMB85RS2MT_ChipSelect();
    //设置指令和地址
    cmdArray[0] = MB85RS2MT_CMD_WRITE;
    cmdArray[1] = (uint8_t)(address>>16);
    cmdArray[2] = (uint8_t)(address>>8);
    cmdArray[3] = (uint8_t)(address);
    //写入指令和地址
    errorSub = MCU_SPI2_ReadWriteBuffer(cmdArray,SPI_PTR_NULL,4);
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        //释放芯片
        BoardMB85RS2MT_ChipDiselect();
        //释放锁
        MCU_SPI2_ReleaseLock();
        return errorSub;
    }   
    //写入数据
    errorSub = MCU_SPI2_ReadWriteBuffer(pdata,SPI_PTR_NULL,length);
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        //释放芯片
        BoardMB85RS2MT_ChipDiselect();
        //释放锁
        MCU_SPI2_ReleaseLock();
        return errorSub;
    }     
    //释放芯片
    BoardMB85RS2MT_ChipDiselect();
    //释放锁
    MCU_SPI2_ReleaseLock();
    return ERROR_SUB_OK;  
}

//读缓存
ERROR_SUB BoardMB85RS2MT_ReadBuffer(uint32_t address, uint8_t *pdata, uint32_t length)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t cmdArray[4] = {0};
    //锁定SPI接口
    MCU_SPI2_GetLock();
    //芯片选中
    BoardMB85RS2MT_ChipSelect();
    //设置指令和地址
    cmdArray[0] = MB85RS2MT_CMD_READ;
    cmdArray[1] = (uint8_t)(address>>16);
    cmdArray[2] = (uint8_t)(address>>8);
    cmdArray[3] = (uint8_t)(address);
    //写入指令和地址
    errorSub = MCU_SPI2_ReadWriteBuffer(cmdArray,SPI_PTR_NULL,4);
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        //释放芯片
        BoardMB85RS2MT_ChipDiselect();
        //释放锁
        MCU_SPI2_ReleaseLock();
        return errorSub;
    }  
    //数据读取
    errorSub = MCU_SPI2_ReadWriteBuffer(SPI_PTR_NULL,pdata,length);
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        //释放芯片
        BoardMB85RS2MT_ChipDiselect();
        //释放锁
        MCU_SPI2_ReleaseLock();
        return errorSub;
    }   
    //释放芯片
    BoardMB85RS2MT_ChipDiselect();
    //释放锁
    MCU_SPI2_ReleaseLock();
    return ERROR_SUB_OK;
}

//读取SDRAM自检标记并检查是否已经被设置
FlagStatus BoardMB85RS2MT_ReadSDRAM_CheckPassFlag(void)
{
    uint32_t flagValue = 0;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //读取标记
    errorSub = BoardMB85RS2MT_ReadBuffer(MB85RS2MT_SDRAM_CHECK_PASS_FLAG_ADDR,(uint8_t*)(&flagValue),4);
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        return RESET;
    }
    //检查标记是否有效
    if(flagValue == MB85RS2MT_SDRAM_CHECK_PASS_FLAG_VAL)
    {
        return SET;
    }
    return RESET;
}

//写入SDRAM自检成功标记
ERROR_SUB BoardMB85RS2MT_WriteSDRAM_CheckPassFlag(void)
{
    uint32_t flagValue = MB85RS2MT_SDRAM_CHECK_PASS_FLAG_VAL;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = BoardMB85RS2MT_WriteBuffer(MB85RS2MT_SDRAM_CHECK_PASS_FLAG_ADDR,(uint8_t*)(&flagValue),4);
    if(errorSub != ERROR_SUB_OK)
    {
        LowLevelShowErrorSubCode(errorSub);
        return errorSub;
    }
    return ERROR_SUB_OK;
}