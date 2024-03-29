#include "MCU_Flash.h"
#include "os.h"

//读出字
uint32_t MCU_FlashReadWord(uint32_t faddr)
{
    return *(volatile uint32_t*)faddr;
}

//获取某个地址所在的flash扇区
//addr:flash地址
//返回值:0~11,即addr所在的扇区
static uint16_t MCU_FlashGetSector(uint32_t addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)
        return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)
        return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)
        return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)
        return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)
        return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)
        return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)
        return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)
        return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)
        return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)
        return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)
        return FLASH_Sector_10; 
	return FLASH_Sector_11;	
}

//从指定地址开始写入指定长度的数据
//特别注意:因为STM32F4的扇区实在太大,没办法本地保存扇区数据,所以本函数
//         写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以
//         写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
//         没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写. 
//该函数对OTP区域也有效!可以用来写OTP区!
//OTP区域地址范围:0X1FFF7800~0X1FFF7A0F
//WriteAddr:起始地址(此地址必须为4的倍数!!)
//pBuffer:数据指针
//NumToWrite:字(32位)数(就是要写入的32位数据的个数.) 
LH_ERR MCU_FlashWrite(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite)
{
    //flash状态
    FLASH_Status status = FLASH_COMPLETE;
    //打开HSI
    RCC_HSICmd(ENABLE);
    //延时10MS
    CoreDelayMs(10);
    //写入的起始地址
    uint32_t addrx=0;
    //写入的结束地址
    uint32_t endaddr=0;
    //非法地址校验,必须是整地址开始写入	
    if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)
    {
        return LH_ERR_MCU_FLASH_ADDR;
    }	
    /* 关中断 */
    __set_PRIMASK(1);  
    //FLASH擦除期间,必须禁止数据缓存								
    FLASH_DataCacheCmd(DISABLE);	
    //解锁 
    FLASH_Unlock();
    /* 清空所有标志位 */
    FLASH_ClearFlag( FLASH_FLAG_EOP|FLASH_FLAG_WRPERR|FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
    //写入的起始地址    
    addrx=WriteAddr;
    //写入的结束地址				
    endaddr=WriteAddr+NumToWrite*4;	
    //只有主存储区,才需要执行擦除操作!!
    if(addrx<0X1FFF0000)			
    {
        //扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
        while(addrx < endaddr)		
        {
            //有非0XFFFFFFFF的地方,要擦除这个扇区
            if(MCU_FlashReadWord(addrx)!=0XFFFFFFFF)
            {   
                //VCC=2.7~3.6V之间!!
                /* 清空所有标志位 */
                FLASH_ClearFlag( FLASH_FLAG_EOP|FLASH_FLAG_WRPERR|FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
                status=FLASH_EraseSector(MCU_FlashGetSector(addrx),VoltageRange_3);
                //发生错误了
                if(status!=FLASH_COMPLETE)
                {
                    //上锁
                    FLASH_Lock();
                    //开启数据缓存
                    FLASH_DataCacheCmd(ENABLE);	
                    /* 开中断 */
                    __set_PRIMASK(0); 
                    return LH_ERR_MCU_FLASH_ERASE;
                }
            }
            else
            {
                //读取完成,地址自增
                addrx+=4;
            } 
                
        } 
    }
    if(status==FLASH_COMPLETE)
    {
        //写数据
        while(WriteAddr<endaddr)
        {
            //写入数据
            if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)
            { 
                //写入异常
                //上锁
                FLASH_Lock();
                //开启数据缓存
                FLASH_DataCacheCmd(ENABLE);	
                /* 开中断 */
                __set_PRIMASK(0);
                return LH_ERR_MCU_FLASH_WRITE;
            }
            //地址增加
            WriteAddr+=4;
            //缓冲区增加
            pBuffer++;
        } 
    }
    //正常执行完成
    //上锁
    FLASH_Lock();
    //开启数据缓存
    FLASH_DataCacheCmd(ENABLE);	
    /* 开中断 */
    __set_PRIMASK(0); 
    return LH_ERR_NONE;
}

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToRead:字(4位)数
void MCU_FlashRead(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead)
{
    uint32_t i;
	for(i=0;i<NumToRead;i++)
	{
        //读取4个字节.
		pBuffer[i] = MCU_FlashReadWord(ReadAddr);
        //偏移4个字节.
		ReadAddr += 4;	
	}
}   		












