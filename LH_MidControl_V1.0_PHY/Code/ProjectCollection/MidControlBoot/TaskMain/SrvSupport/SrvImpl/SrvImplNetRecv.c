/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-08-04 19:39:42
**LastEditors: DengXiaoJun
**LastEditTime: 2021-08-10 10:09:01
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "SrvImplNetRecv.h"
#include "SrvTaskConfig.h"
#include "AppTaskConfig.h"

//处理通讯协议接收到的数据
LH_ERR SrvImplNetRecvWithProtocol(uint8_t* recvDataPtr, uint16_t size)
{
    LH_ERR errorCode = LH_ERR_NONE;
    NET_CMD_RECV_STATE* netCmdRecvStatePtr = NULL;
    uint32_t currentTimeTick = 0;
    //获取当前时间
    currentTimeTick = tx_time_get();
    //获取接收缓存指针
    netCmdRecvStatePtr = TcpBaseUtilGetRecvStatePtr();
    //判定一个数据包的两帧数据是否超时
    if(netCmdRecvStatePtr->packLastRecvTimeTick != 0 && (currentTimeTick-netCmdRecvStatePtr->packLastRecvTimeTick) > TCPIP_CMD_PACK_FRAME_TIMEOUT_MS)
    {
        //打印数据异常
        SystemPrintf("[MID]Boot E>TcpCmd Reset:%d,%d\r\n", currentTimeTick, netCmdRecvStatePtr->packLastRecvTimeTick);
        //清理接收状态,当成一包新数据接收
        TcpBaseUtilClearRecvState();
    }
    //正常数据接收处理
    if(netCmdRecvStatePtr->recvDataLength+size < TCP_CMD_HEADER_SIZE)
    {
        //接收到的数据不够一个指令头,将数据全部保存下来
        UserMemCopy(netCmdRecvStatePtr->recvDat+netCmdRecvStatePtr->recvDataLength, recvDataPtr, size);
        //储存数据量增加
        netCmdRecvStatePtr->recvDataLength += size;
        //记录接收到这一帧数据的时间
        netCmdRecvStatePtr->packLastRecvTimeTick = currentTimeTick;
    }
    else
    {
        //如果当前的数据和已经接受的数据加起来超过了一个指令头,就开始解析
        uint16_t recvDataProcessIndex = 0;//数据解析计数器,解析多少数据,这个计数器增加多少,直到解析完成
        uint16_t copySizeUtil = 0;//辅助拷贝,用于拷贝前计算
        uint16_t cmdLeaderCode = 0;//指令前导码计算辅助
        do
        {
            //还没收到数据头
            if (netCmdRecvStatePtr->recvDataLength < TCP_CMD_HEADER_SIZE)
            {
                //距离完成的数据头还剩下多少,就拷贝多少
                copySizeUtil = TCP_CMD_HEADER_SIZE-netCmdRecvStatePtr->recvDataLength;
                //如果剩下的的数据量不足不全全部数据头,以收到的数据量为准
                if (copySizeUtil > (size-recvDataProcessIndex)) 
                    copySizeUtil = size-recvDataProcessIndex;
                //拷贝指定长度的数据量
                UserMemCopy(netCmdRecvStatePtr->recvDat+netCmdRecvStatePtr->recvDataLength, recvDataPtr+recvDataProcessIndex, copySizeUtil);
                //已经拷贝的数据量增加
                recvDataProcessIndex += copySizeUtil;
                //新增拷贝的数据量增加
                netCmdRecvStatePtr->recvDataLength += copySizeUtil;
            }
            //经过一次处理后,如果已经收到了完成的数据头部
            if (netCmdRecvStatePtr->recvDataLength >= TCP_CMD_HEADER_SIZE)
            {
                //检查收到的指令头是不是一个有效的指令头
                cmdLeaderCode = TcpBaseUtilConvertBufferToUint16(netCmdRecvStatePtr->recvDat,0);
                //指令头前缀匹配
                if(TCP_CMD_LEADER_CODE == cmdLeaderCode)
                {
                    //计算该指令应该收到多少的包体数据
                    netCmdRecvStatePtr->recvBodyLength = TcpBaseUtilConvertBufferToUint16(netCmdRecvStatePtr->recvDat,TCP_CMD_HEADER_DLEN_OFFSET);
                    if (netCmdRecvStatePtr->recvBodyLength > 0)
                    {
                        //拷贝包体数据,先检查当前接收到的数据够不够填充包体,能填充全部拷贝,否则拷贝剩余部分
                        copySizeUtil = (netCmdRecvStatePtr->recvBodyLength+recvDataProcessIndex) <= size ? netCmdRecvStatePtr->recvBodyLength : (size-recvDataProcessIndex);
                        //如果当前已经拷贝了一部分的包体数据,那么这一次拷贝的数据就减去已经拷贝的包体数据
                        uint16_t copySizeUtilMax = 0;
                        if(netCmdRecvStatePtr->recvDataLength - TCP_CMD_HEADER_SIZE > 0)
                        {
                            //计算最大能拷贝的数据量,包体数据加包头数据,减去当前接收到的总数量,就是还缺的包体数据
                            copySizeUtilMax =  (netCmdRecvStatePtr->recvBodyLength + TCP_CMD_HEADER_SIZE) - netCmdRecvStatePtr->recvDataLength;
                            //如果拷贝数据大于最大能拷贝的数据,那么以最大拷贝数据为准
                            if(copySizeUtil > copySizeUtilMax)copySizeUtil = copySizeUtilMax;
                        }
                        UserMemCopy(netCmdRecvStatePtr->recvDat+netCmdRecvStatePtr->recvDataLength, recvDataPtr+recvDataProcessIndex, copySizeUtil);
                        //一定要记得将已经处理的数据量增加
                        recvDataProcessIndex += copySizeUtil;
                        netCmdRecvStatePtr->recvDataLength += copySizeUtil;
                    }
                    //如果一帧指令包接收完成,接收的数据和需要接受的数据匹配上
                    //如果没到达要求的数据量,等待下一次处理
                    if(netCmdRecvStatePtr->recvDataLength >= (netCmdRecvStatePtr->recvBodyLength+TCP_CMD_HEADER_SIZE))
                    {
                        //接收到完整一包指令,将数据转发到处理函数进行数据的处理
                        errorCode = SrvImplNetRecvWithCompletePack(netCmdRecvStatePtr);
                        //清理接收状态,后面的数据当成一包新数据接收
                        TcpBaseUtilClearRecvState();
                    }
                }
                else
                {
                    //指令头前缀不匹配,报错打印并清除指令缓冲
                    SystemPrintf("[MID]Boot E>LeaderCode:%04X\r\n", cmdLeaderCode);
                    for (uint16_t i=0; i<size; i++)
                    {
                        SystemPrintf("%02X ", recvDataPtr[i]);
                    }
                    SystemPrintf("\r\n=============\r\n");
                    //清理接收状态,后面的数据当成一包新数据接收
                    TcpBaseUtilClearRecvState();
                    return LH_ERR_BOARD_MAIN_COMM_NET_LEADER;                     
                }
            }
            else
            {
                //有数据没接收完,标识接收到本包数据的时间
                netCmdRecvStatePtr->packLastRecvTimeTick = currentTimeTick;
            }
        }while(recvDataProcessIndex < size);//循环处理,直到将接收到的全部数据处理完成
    }
    return errorCode;
}


//接收到完整的一包数据之后,进行数据打包和数据转发
LH_ERR SrvImplNetRecvWithCompletePack(NET_CMD_RECV_STATE* recvCompletePack)
{
    LH_ERR errorCode = LH_ERR_NONE;
    uint16_t indexUtil = 0;
    uint32_t checkSumUtil = 0;
    NET_COMMAND_PACK* netCommandPackPtr = NULL; 
    //申请内存,存放指令包
    netCommandPackPtr = UserMemMalloc(MEM_AXI_SRAM,(sizeof(NET_COMMAND_PACK)/sizeof(uint8_t)));
    if(netCommandPackPtr == NULL)
    {
        SystemPrintf("[MID]Boot E>Func:%s,Line:%d ,UserMemMalloc Failed\r\n",__FUNCTION__,__LINE__);
        return LH_ERR_BOARD_MAIN_COMM_MALLOC_FAILED;
    }
    //指令包初始化
    TcpBaseUtilNetCmdPackInit(netCommandPackPtr);
    //从完整数据包中进行解析
    //指令
    netCommandPackPtr->netCommand = TcpBaseUtilConvertBufferToUint16(recvCompletePack->recvDat,TCP_CMD_HEADER_CMD_OFFSET);
    //头校验
    netCommandPackPtr->headerDataCheckSum = TcpBaseUtilConvertBufferToUint16(recvCompletePack->recvDat,TCP_CMD_HEADER_SUM_OFFSET);
    //数据校验和
    netCommandPackPtr->bodyDataCheckSum = TcpBaseUtilConvertBufferToUint32(recvCompletePack->recvDat,TCP_CMD_DATA_SUM_OFFSET);
    //数据长度
    netCommandPackPtr->bodyDataLength = recvCompletePack->recvBodyLength;
    //进行CRC校验码计算
    //获取数据头的校验和
    checkSumUtil = 0;
    for (indexUtil=0; indexUtil<(TCP_CMD_HEADER_SIZE-2); indexUtil++)
    {
        checkSumUtil += recvCompletePack->recvDat[indexUtil];
    }
    //匹配校验和,异常则报错
    if ((uint16_t)checkSumUtil != netCommandPackPtr->headerDataCheckSum)
    {
        SystemPrintf("[MID]Boot E>Func:%s,Line:%d ,headerDataCheckSum:%04X,%04X\r\n",__FUNCTION__,__LINE__,checkSumUtil, netCommandPackPtr->headerDataCheckSum);
        //释放内存
        UserMemFree(MEM_AXI_SRAM,netCommandPackPtr);
        //回传NACK,让上位机重发
        TcpBaseSendAck(TCP_NET_ACK_ERROR);
        return LH_ERR_BOARD_MAIN_COMM_NET_CRC_HEAD;
    }
    //进行数据体的校验
    //计算数据包体校验和
    checkSumUtil = 0;    
    for (indexUtil = TCP_CMD_HEADER_SIZE; indexUtil < recvCompletePack->recvDataLength; indexUtil++)
    {
        checkSumUtil += recvCompletePack->recvDat[indexUtil];
    }
    //匹配包体校验和,如果不匹配则报错
    if (checkSumUtil != netCommandPackPtr->bodyDataCheckSum)
    {
        SystemPrintf("[MID]Boot E>Func:%s,Line:%d ,bodyDataCheckSum :%08X,%08X\r\n",__FUNCTION__,__LINE__,checkSumUtil, netCommandPackPtr->bodyDataCheckSum);
        //释放内存
        UserMemFree(MEM_AXI_SRAM,netCommandPackPtr);
        //回传NACK,让上位机重发
        TcpBaseSendAck(TCP_NET_ACK_ERROR);
        return LH_ERR_BOARD_MAIN_COMM_NET_CRC_BODY;
    }
    //申请数据域指针
    if(netCommandPackPtr->bodyDataLength != 0)
    {
        netCommandPackPtr->bodyData = UserMemMalloc(MEM_AXI_SRAM,netCommandPackPtr->bodyDataLength);
        if(netCommandPackPtr->bodyData == NULL)
        {
            SystemPrintf("[MID]Boot E>Func:%s,Line:%d ,UserMemMalloc Failed\r\n",__FUNCTION__,__LINE__);
            return LH_ERR_BOARD_MAIN_COMM_MALLOC_FAILED;
        }
    }
    //校验和过关,进行数据体拷贝
    UserMemCopy(netCommandPackPtr->bodyData,recvCompletePack->recvDat+TCP_CMD_HEADER_SIZE,netCommandPackPtr->bodyDataLength);
    //包体数据构造完成,现在需要转发
    errorCode = SrvImplNetRecvDispatchCommand(netCommandPackPtr);
    if(errorCode != LH_ERR_NONE)
    {
        //转发失败,释放内存
        if(netCommandPackPtr->bodyDataLength != 0)
        {
            UserMemFree(MEM_AXI_SRAM,netCommandPackPtr->bodyData);
        }
        UserMemFree(MEM_AXI_SRAM,netCommandPackPtr);
    }
    //转发完成,去除操作数据包指向,防止野指针
    netCommandPackPtr = NULL;
    return errorCode;
}


//对处理完成的TCIIP数据包进行转发处理
LH_ERR  SrvImplNetRecvDispatchCommand(NET_COMMAND_PACK* commandPackPtr)
{
    NET_COMMAND_PACK* commandPackLocal = commandPackPtr;
    uint32_t txResult = TX_SUCCESS;
    if(commandPackPtr->netCommand == TCP_CMD_COMMUNICATE_ACK)
    {
        //如果是ACK,立即处理,ACK不需要ACK
        tx_semaphore_ceiling_put(TcpBaseUtilGetNetAckSemPtr(),1);
        //释放内存
        if(commandPackPtr->bodyData != NULL)
        {
            //释放数据包
            UserMemFree(MEM_AXI_SRAM,commandPackPtr->bodyData);
        }
        UserMemFree(MEM_AXI_SRAM,commandPackPtr);
        return LH_ERR_NONE;
    }
    if(commandPackPtr->netCommand != TCP_CMD_OFF_LINE)
    {
        //只要不是脱机指令,回传ACK
        TcpBaseSendAck(TCP_NET_ACK_OK);
    }
    //转发到网口数据处理线程
    txResult = tx_queue_send(&queueAppTaskTcpMsg,(void*)(&commandPackLocal),TX_NO_WAIT);
    //检测是否发送成功
    if (txResult != TX_SUCCESS)
    {
        SystemPrintf("[MID]Boot E>Func:%s,Line:%d ,QueueSend Failed,txResult: %d\r\n",__FUNCTION__,__LINE__,txResult);
        return LH_ERR_BOARD_MAIN_COMM_NET_DISPATCH_ERR;
    }
    return LH_ERR_NONE;
}















































