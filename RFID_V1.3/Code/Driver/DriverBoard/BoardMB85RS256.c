#include "BoardMB85RS256.h"

//FRAM的片选信号
#define MB85RS256_CS_SELECT()            MCU_PortWriteSingle(MCU_PIN_B_12,Bit_RESET)
#define MB85RS256_CS_RELEASE()           MCU_PortWriteSingle(MCU_PIN_B_12,Bit_SET)

//检查芯片读写过程,查看芯片是否正常
LH_ERR BoardMB85RS256_Check(void)
{
    uint8_t dat = 0;
    uint8_t dat2 = 0;
    uint8_t dataCopy = 0;
    // uint32_t BoardMB85RS256_id = 0;
    // //读取芯片ID
    // BoardMB85RS256_ReadID(&BoardMB85RS256_id);
    // if(BoardMB85RS256_id != MB85RS256_ID)
    // {
    //     return LH_ERR_BOARD_MAIN_COMM_FRAM_ID;
    // }
    
    //读取地址0数据
    BoardMB85RS256_ReadBuffer(0,&dat2,1);
    //要把原来的值保存起来
    dataCopy = dat2;
    //对读取到的数据进行操作
    dat = dat2+111;
    //忘记原来读取到的数据
    dat2 = 0;
    //将地址0数据加上指定数据写入flash
    BoardMB85RS256_WriteBuffer(0,&dat,1);
    //从地址0数据处读取刚写入的数据
    BoardMB85RS256_ReadBuffer(0,&dat2,1);
    //把原来的值写会去
    BoardMB85RS256_WriteBuffer(0,&dataCopy,1);
    //dat2是读到的处理后的数据 dat是写入之前的数据
    if(dat != dat2)
    {
        //两者不等,说明要么写入要么读取失败
        return LH_ERR_BOARD_MAIN_COMM_FRAM_CHECK;
    }
    else
    {
        //两者相等,说明校验通过
        return LH_ERR_NONE;
    }
}

//铁电存储器初始化
void BoardMB85RS256_Init(void)
{
    //初始化片选CS
    MCU_PortInit(MCU_PIN_B_12, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP,GPIO_Speed_Level_3);
    MB85RS256_CS_RELEASE();
    //初始化SPI
    MCU_SPI2_Init();
}

void BoardMB85RS256_ReadID(uint32_t *id)
{
    OS_ERR err;
    uint8_t retryCount = 0;
    //申请互斥信号量,防止多任务抢占
    if(OSRunning)OSMutexPend (&mutexMCU_SPI2,0,OS_OPT_PEND_BLOCKING,0,&err);
    uint8_t* buf= (uint8_t*)id;
    //片选
	MB85RS256_CS_SELECT();
    CoreDelayUs(10);
    //写指令
	MCU_SPI2_ReadWriteByteMB85(MB85RS256_CMD_RDID);              
    uint8_t temp = 0;
    do
    {
        temp = MCU_SPI2_ReadWriteByteMB85(0XFF);
        retryCount++;
        if(retryCount > 100)
        {
            *id = 0xFFFFFFFF;
            MB85RS256_CS_RELEASE();
            //释放互斥信号量
            if(OSRunning)OSMutexPost(&mutexMCU_SPI2,OS_OPT_POST_FIFO,&err);
            return;
        }
    }while(temp != (uint8_t)(MB85RS256_ID));
	buf[0] = temp;
	buf[1] = MCU_SPI2_ReadWriteByteMB85(0XFF);
	buf[2] = MCU_SPI2_ReadWriteByteMB85(0XFF);
    buf[3] = MCU_SPI2_ReadWriteByteMB85(0XFF);
	MB85RS256_CS_RELEASE();
    //释放互斥信号量
    if(OSRunning)OSMutexPost(&mutexMCU_SPI2,OS_OPT_POST_FIFO,&err);
}

void BoardMB85RS256_WriteBuffer(uint32_t address, uint8_t *pdata, uint32_t length)
{
    OS_ERR err;
	//申请互斥信号量,防止多任务抢占
    if(OSRunning)OSMutexPend (&mutexMCU_SPI2,0,OS_OPT_PEND_BLOCKING,0,&err);
    //片选使能.
	MB85RS256_CS_SELECT();           
    //写使能                          
	MCU_SPI2_ReadWriteByteMB85(MB85RS256_CMD_WREN);               
    MB85RS256_CS_RELEASE();
    CoreDelayUs(50);
    MB85RS256_CS_SELECT();
    //写指令
	MCU_SPI2_ReadWriteByteMB85(MB85RS256_CMD_WRITE);  
    // //写地址高两位            
	// MCU_SPI2_ReadWriteByteMB85((uint8_t)(address>>16));    
    //写地址中间字节  
	MCU_SPI2_ReadWriteByteMB85((uint8_t)(address>>8));   
    //写地址低字节   
	MCU_SPI2_ReadWriteByteMB85((uint8_t)(address));          
	/*写入数据*/
	for(uint32_t i=0;i<length;i++)
		MCU_SPI2_ReadWriteByteMB85(pdata[i]);
    //片选禁止写保护
	MB85RS256_CS_RELEASE();   
    //释放互斥信号量                             
    if(OSRunning)OSMutexPost(&mutexMCU_SPI2,OS_OPT_POST_FIFO,&err);
}

void BoardMB85RS256_ReadBuffer(uint32_t address, uint8_t *pdata, uint32_t length)
{
    OS_ERR err;
	//申请互斥信号量,防止多任务抢占
    if(OSRunning)OSMutexPend (&mutexMCU_SPI2,0,OS_OPT_PEND_BLOCKING,0,&err);
    //片选使能
	MB85RS256_CS_SELECT();                   
    //读指令             
	MCU_SPI2_ReadWriteByteMB85(MB85RS256_CMD_READ);     
    // //读地址高两位          
	// MCU_SPI2_ReadWriteByteMB85((uint8_t)(address>>16));    
    //读地址中间字节  
	MCU_SPI2_ReadWriteByteMB85((uint8_t)(address>>8)); 
    //读地址低字节      
	MCU_SPI2_ReadWriteByteMB85((uint8_t)(address));          
	/*读出数据*/
	for(uint32_t i=0;i<length;i++)
		pdata[i]=MCU_SPI2_ReadWriteByteMB85(0XFF);
    //片选禁止
	MB85RS256_CS_RELEASE();  
    //释放互斥信号量               
    if(OSRunning)OSMutexPost(&mutexMCU_SPI2,OS_OPT_POST_FIFO,&err);
}




