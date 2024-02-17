/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-09-15 10:08:27 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-09-16 20:13:33 +0800
************************************************************************************************/ 
#include "NetXDUO_Port.h"
#include "UserMemManage.h"

//IP接口保存的讯息
NX_IP lan8700NetX;
//执行LAN8700 IP协议栈的线程
TX_THREAD *netx_lan8700_thread_ptr; 
//NETX 驱动层保存的协议栈讯息
NETX_PORT_INFO nxPortInfo;
//内存池控制块
static NX_PACKET_POOL nxPacketPoolLan8700;


//初始化标记
static uint8_t nxPacketPoolLan8700InitFlag;
#pragma pack(push) //保存对齐状态 
#pragma pack(4)
//内存池
static uint8_t memNetX_Lan8700PoolBase[NETX_LAN8700_PACKET_POOL_SIZE]@".sdram1";
#pragma pack (pop)	

//LAN8700创建驱动缓存
NX_PACKET_POOL* NetX_DriverLAN8700_CreateMemoryPool(void)
{
    uint32_t nxResult= NX_SUCCESS;
    //创建内存池
    if(nxPacketPoolLan8700InitFlag != 0)
    {
        return &nxPacketPoolLan8700;
    }
    nxPacketPoolLan8700InitFlag = 1;
    nxResult = nx_packet_pool_create(&nxPacketPoolLan8700,              /* 内存池控制块 */
                                    "nxPacketPoolLan8700",              /* 内存池名 */
                                    NETX_LAN8700_PACKET_SIZE,           /* 内存池每个数据包大小，单位字节,此值必须至少为 40 个字节，并且还必须可以被 4 整除 */
                                    (ULONG *)(memNetX_Lan8700PoolBase), /* 内存池地址，此地址必须ULONG对齐 */
                                    NETX_LAN8700_PACKET_POOL_SIZE);     /* 内存池大小 */
    if (nxResult != NX_SUCCESS)
    {
        LowLevelPrintf("NetX_DriverLAN8700CreateMemoryPool Failed: %d \r\n", nxResult);
        nxPacketPoolLan8700InitFlag = 0;
        return NULL;
    }
    return &nxPacketPoolLan8700;
}

//初始化驱动保存信息为默认值
static void NetX_DriverLAN8700InfoReset(void)
{
    nxPortInfo.nxPortInfoIP_Ptr = NULL;
    nxPortInfo.nxPortInfoInterfacePtr = NULL;
    nxPortInfo.nxPortInfoMultiCastCount = 0;
    nxPortInfo.nxPortInfoState = NETX_PORT_STATE_IDLE;
    nxPortInfo.recvPackCount = 0;
    nxPortInfo.sendPackCount = 0;
    nxPortInfo.allocFailCount = 0;
    nxPortInfo.errorCount = 0;
    nxPortInfo.dataEvent = 0;
    nxPortInfo.netPortState = NET_PORT_STATE_UNKNOW;
    //速度和连接模式
    nxPortInfo.linkState = NX_FALSE;
    nxPortInfo.linkSpeed = 0XFFFFFFFF;
    nxPortInfo.deplexMode = 0XFFFFFFFF;
}


//驱动程序调用接口申明
static void NetX_DriverLAN8700_Initialize(NX_IP_DRIVER *nxLan8700ReqPtr);
static void NetX_DriverLAN8700_Enable(NX_IP_DRIVER *nxLan8700ReqPtr);
static void NetX_DriverLAN8700_Disable(NX_IP_DRIVER *nxLan8700ReqPtr);
static void NetX_DriverLAN8700_Uninitialize(NX_IP_DRIVER *nxLan8700ReqPtr);
static void NetX_DriverLAN8700_PacketSend(NX_IP_DRIVER *nxLan8700ReqPtr);
static void NetX_DriverLAN8700_MultiCastJoin(NX_IP_DRIVER *nxLan8700ReqPtr);
static void NetX_DriverLAN8700_MultiCastLeave(NX_IP_DRIVER *nxLan8700ReqPtr);
static void NetX_DriverLAN8700_InterfaceAttach(NX_IP_DRIVER *nxLan8700ReqPtr);
static void NetX_DriverLAN8700_InterfaceDetach(NX_IP_DRIVER *nxLan8700ReqPtr);
static void NetX_DriverLAN8700_GetStatus(NX_IP_DRIVER *nxLan8700ReqPtr);
static void NetX_DriverLAN8700_GetSpeed(NX_IP_DRIVER *nxLan8700ReqPtr);
static void NetX_DriverLAN8700_GetDuplexType(NX_IP_DRIVER *nxLan8700ReqPtr);
static void NetX_DriverLAN8700_GetErrorCount(NX_IP_DRIVER *nxLan8700ReqPtr);
static void NetX_DriverLAN8700_GetRxCount(NX_IP_DRIVER *nxLan8700ReqPtr);
static void NetX_DriverLAN8700_GetTxCount(NX_IP_DRIVER *nxLan8700ReqPtr);
static void NetX_DriverLAN8700_GetAllocErrorCount(NX_IP_DRIVER *nxLan8700ReqPtr);
static void NetX_DriverLAN8700_DeferredProcessing(NX_IP_DRIVER *nxLan8700ReqPtr);
static void NetX_DriverLAN8700_SetPhysicalAddress(NX_IP_DRIVER *nxLan8700ReqPtr);
static void NetX_DriverLAN8700_UserCommand(NX_IP_DRIVER *nxLan8700ReqPtr);


//检查数据请求是否合理,错误返回1
static uint8_t NetX_DriverLAN8700_RequestCheck(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    //IP匹配
    if(nxPortInfo.nxPortInfoIP_Ptr != nxLan8700ReqPtr->nx_ip_driver_ptr)
    {
        //IP不匹配
        nxLan8700ReqPtr->nx_ip_driver_status = NX_NOT_SUCCESSFUL;
        //错误打印
        LowLevelPrintf("MultiCastJoin IP Ptr NotMatch\r\n");
        return 1;
    }
    //接口匹配
    if(nxPortInfo.nxPortInfoInterfacePtr != nxLan8700ReqPtr->nx_ip_driver_interface)
    {
        //接口不匹配
        nxLan8700ReqPtr->nx_ip_driver_status = NX_NOT_SUCCESSFUL;
        //错误打印
        LowLevelPrintf("MultiCastJoin Interface Ptr NotMatch\r\n");
        return 1;
    }
    //和内部保存指针匹配
    if(nxPortInfo.nxPortInfoIP_Ptr != &lan8700NetX)
    {
        //IP不匹配
        nxLan8700ReqPtr->nx_ip_driver_status = NX_NOT_SUCCESSFUL;
        //错误打印
        LowLevelPrintf("MultiCastJoin IP Ptr NotLocal\r\n");
        return 1;
    }
    return 0;
}


#if(NETX_LAN8700_DRIVER_DEBUG_SHOW == 1)
//打印NetX请求类型
static void NetX_DriverLAN8700_RequestShow(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    //打印IP,子网掩码
    #ifndef NX_DISABLE_IPV4
    NX_INTERFACE *interface_ptr = nxLan8700ReqPtr->nx_ip_driver_interface;
    ULONG ipAddress = interface_ptr->nx_interface_ip_address;
    ULONG netMask = interface_ptr->nx_interface_ip_network_mask;
    LowLevelPrintf("NetX_DriverLAN8700,IPName: %s,IP: %d.%d.%d.%d , NetMask: %d.%d.%d.%d Request: ",
                   nxLan8700ReqPtr->nx_ip_driver_ptr->nx_ip_name, DWORD_TO_BYTE_HH(ipAddress), DWORD_TO_BYTE_HL(ipAddress),
                   DWORD_TO_BYTE_LH(ipAddress), DWORD_TO_BYTE_LL(ipAddress), DWORD_TO_BYTE_HH(netMask),
                   DWORD_TO_BYTE_HL(netMask), DWORD_TO_BYTE_LH(netMask), DWORD_TO_BYTE_LL(netMask));
    #endif
    //处理上层协议栈对网卡的请求
    switch (nxLan8700ReqPtr->nx_ip_driver_command)
    {
    case NX_LINK_INITIALIZE:
        //打印请求类型
        LowLevelPrintf("NX_LINK_INITIALIZE \r\n");
        break;
    case NX_LINK_ENABLE:
        //打印请求类型
        LowLevelPrintf("NX_LINK_ENABLE \r\n");
        break;
    case NX_LINK_DISABLE:
        //打印请求类型
        LowLevelPrintf("NX_LINK_DISABLE \r\n");
        break;
    case NX_LINK_UNINITIALIZE:
        //打印请求类型
        LowLevelPrintf("NX_LINK_UNINITIALZE \r\n");
        break;
    case NX_LINK_PACKET_SEND:
        //打印请求类型
        LowLevelPrintf("NX_LINK_PACKET_SEND \r\n");
        break;
    case NX_LINK_PACKET_BROADCAST:
        //打印请求类型
        LowLevelPrintf("NX_LINK_PACKET_BROADCAST \r\n");
        break;
    case NX_LINK_ARP_SEND:
        //打印请求类型
        LowLevelPrintf("NX_LINK_ARP_SEND \r\n");
        break;
    case NX_LINK_ARP_RESPONSE_SEND:
        //打印请求类型
        LowLevelPrintf("NX_LINK_ARP_RESPONSE_SEND \r\n");
        break;
    case NX_LINK_RARP_SEND:
        //打印请求类型
        LowLevelPrintf("NX_LINK_RARP_SEND \r\n");
        break;
    case NX_LINK_MULTICAST_JOIN:
        //打印请求类型
        LowLevelPrintf("NX_LINK_MULTICAST_JOIN \r\n");
        break;
    case NX_LINK_MULTICAST_LEAVE:
        //打印请求类型
        LowLevelPrintf("NX_LINK_MULTICAST_LEAVE \r\n");
        break;
    case NX_LINK_INTERFACE_ATTACH:
        //打印请求类型
        LowLevelPrintf("NX_LINK_INTERFACE_ATTACH \r\n");
        break;
    case NX_LINK_INTERFACE_DETACH:
        //打印请求类型
        LowLevelPrintf("NX_LINK_INTERFACE_DETACH \r\n");
        break;
    case NX_LINK_GET_STATUS:
        //打印请求类型
        LowLevelPrintf("NX_LINK_GET_STATUS \r\n");
        break;
    case NX_LINK_GET_SPEED:
        //打印请求类型
        LowLevelPrintf("NX_LINK_GET_SPEED \r\n");
        break;
    case NX_LINK_GET_DUPLEX_TYPE:
        //打印请求类型
        LowLevelPrintf("NX_LINK_GET_DUPLEX_TYPE \r\n");
        break;
    case NX_LINK_GET_ERROR_COUNT:
        //打印请求类型
        LowLevelPrintf("NX_LINK_GET_ERROR_COUNT \r\n");
        break;
    case NX_LINK_GET_RX_COUNT:
        //打印请求类型
        LowLevelPrintf("NX_LINK_GET_RX_COUNT \r\n");
        break;
    case NX_LINK_GET_TX_COUNT:
        //打印请求类型
        LowLevelPrintf("NX_LINK_GET_TX_COUNT \r\n");
        break;
    case NX_LINK_GET_ALLOC_ERRORS:
        //打印请求类型
        LowLevelPrintf("NX_LINK_GET_ALLOC_ERRORS \r\n");
        break;
    case NX_LINK_DEFERRED_PROCESSING:
        //打印请求类型
        LowLevelPrintf("NX_LINK_DEFERRED_PROCESSING \r\n");
        break;
    case NX_LINK_SET_PHYSICAL_ADDRESS:
        //打印请求类型
        LowLevelPrintf("NX_LINK_SET_PHYSICAL_ADDRESS \r\n");
        break;
    case NX_LINK_USER_COMMAND:
        //打印请求类型
        LowLevelPrintf("NX_LINK_USER_COMMAND \r\n");
        break;
    case NX_INTERFACE_CAPABILITY_GET:
        //打印请求类型
        LowLevelPrintf("NX_ENABLE_INTERFACE_CAPABILITY \r\n");
        break;
    case NX_INTERFACE_CAPABILITY_SET:
        //打印请求类型
        LowLevelPrintf("NX_INTERFACE_CAPABILITY_SET \r\n");
        break;
    default:
        //未实现的指令
        LowLevelPrintf("UNSUPPORT Cmd: %d\r\n",nxLan8700ReqPtr->nx_ip_driver_command);
        break;
    }
}
//打印NetX请求结果
static void NetX_DriverLAN8700_ResultShow(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    if(nxLan8700ReqPtr->nx_ip_driver_status == NX_SUCCESS)
    {
        LowLevelPrintf("    NetX Result: SUCCESS\r\n ================\r\n");
    }
    else
    {
        LowLevelPrintf("    NetX Result: %d\r\n ================\r\n",nxLan8700ReqPtr->nx_ip_driver_status);
    }
}
#else
//打印NetX请求类型
static void NetX_DriverLAN8700_RequestShow(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    nxLan8700ReqPtr = nxLan8700ReqPtr;
}
//打印NetX请求结果
static void NetX_DriverLAN8700_ResultShow(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    nxLan8700ReqPtr = nxLan8700ReqPtr;
}
#endif

//LAN8700 NetX驱动接口
void NetX_DriverLAN8700(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    //首先设置结果为成功
    nxLan8700ReqPtr->nx_ip_driver_status = NX_SUCCESS;
    NetX_DriverLAN8700_RequestShow(nxLan8700ReqPtr);
    //处理上层协议栈对网卡的请求
    switch (nxLan8700ReqPtr->nx_ip_driver_command)
    {
    case NX_LINK_INITIALIZE:
        //驱动程序初始化
        NetX_DriverLAN8700_Initialize(nxLan8700ReqPtr);
        break;
    case NX_LINK_ENABLE:
        //启用链接
        NetX_DriverLAN8700_Enable(nxLan8700ReqPtr);
        break;
    case NX_LINK_DISABLE:
        //禁用链接
        NetX_DriverLAN8700_Disable(nxLan8700ReqPtr);
        break;
    case NX_LINK_UNINITIALIZE:
        //取消初始化链接
        NetX_DriverLAN8700_Uninitialize(nxLan8700ReqPtr);
        break;
    case NX_LINK_PACKET_SEND:
        //数据包发送
        NetX_DriverLAN8700_PacketSend(nxLan8700ReqPtr);
        break;
    case NX_LINK_PACKET_BROADCAST:
        //数据包广播（仅 IPv4 数据包）
        NetX_DriverLAN8700_PacketSend(nxLan8700ReqPtr);
        break;
    case NX_LINK_ARP_SEND:
        // ARP 发送
        NetX_DriverLAN8700_PacketSend(nxLan8700ReqPtr);
        break;
    case NX_LINK_ARP_RESPONSE_SEND:
        // ARP 响应发送
        NetX_DriverLAN8700_PacketSend(nxLan8700ReqPtr);
        break;
    case NX_LINK_RARP_SEND:
        // RARP 发送
        NetX_DriverLAN8700_PacketSend(nxLan8700ReqPtr);
        break;
    case NX_LINK_MULTICAST_JOIN:
        //多播组联接
        NetX_DriverLAN8700_MultiCastJoin(nxLan8700ReqPtr);
        break;
    case NX_LINK_MULTICAST_LEAVE:
        //多播组离开
        NetX_DriverLAN8700_MultiCastLeave(nxLan8700ReqPtr);
        break;
    case NX_LINK_INTERFACE_ATTACH:
        //附加接口
        NetX_DriverLAN8700_InterfaceAttach(nxLan8700ReqPtr);
        break;
    case NX_LINK_INTERFACE_DETACH:
        //分离接口
        NetX_DriverLAN8700_InterfaceDetach(nxLan8700ReqPtr);
        break;
    case NX_LINK_GET_STATUS:
        //获取链接状态
        NetX_DriverLAN8700_GetStatus(nxLan8700ReqPtr);
        break;
    case NX_LINK_GET_SPEED:
        //获取链接速度
        NetX_DriverLAN8700_GetSpeed(nxLan8700ReqPtr);
        break;
    case NX_LINK_GET_DUPLEX_TYPE:
        //获取双工类型
        NetX_DriverLAN8700_GetDuplexType(nxLan8700ReqPtr);
        break;
    case NX_LINK_GET_ERROR_COUNT:
        //获取错误计数
        NetX_DriverLAN8700_GetErrorCount(nxLan8700ReqPtr);
        break;
    case NX_LINK_GET_RX_COUNT:
        //获取接收数据包计数
        NetX_DriverLAN8700_GetRxCount(nxLan8700ReqPtr);
        break;
    case NX_LINK_GET_TX_COUNT:
        //获取传输数据包计数
        NetX_DriverLAN8700_GetTxCount(nxLan8700ReqPtr);
        break;
    case NX_LINK_GET_ALLOC_ERRORS:
        //获取分配错误
        NetX_DriverLAN8700_GetAllocErrorCount(nxLan8700ReqPtr);
        break;
    case NX_LINK_DEFERRED_PROCESSING:
        //驱动程序延迟处理
        NetX_DriverLAN8700_DeferredProcessing(nxLan8700ReqPtr);
        break;
    case NX_LINK_SET_PHYSICAL_ADDRESS:
        //设置物理地址
        NetX_DriverLAN8700_SetPhysicalAddress(nxLan8700ReqPtr);
        break;
    case NX_LINK_USER_COMMAND:
        //用户命令
        NetX_DriverLAN8700_UserCommand(nxLan8700ReqPtr);
        break;
#ifdef NX_ENABLE_INTERFACE_CAPABILITY
    case NX_INTERFACE_CAPABILITY_GET:
        //获取物理接口扩展功能
        NetX_DriverLAN8700_CapabilityGet(nxLan8700ReqPtr);
        break;
    case NX_INTERFACE_CAPABILITY_SET:
        //设置物理接口扩展功能
        NetX_DriverLAN8700_CapabilitySet(nxLan8700ReqPtr);
        break;
#endif
    default:
        //未实现的指令
        nxLan8700ReqPtr->nx_ip_driver_status = NX_UNHANDLED_COMMAND;
        break;
    }
    //打印请求结果
    NetX_DriverLAN8700_ResultShow(nxLan8700ReqPtr);
    return;
}


/*---------------------------------------NETX LAN8700驱动实现-------------------------------------------*/
//驱动程序初始化
static void NetX_DriverLAN8700_Initialize(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    LH_ERR errorSubCode = LH_ERR_NONE;
    //状态匹配,必须是驱动附着状态
    if(nxPortInfo.nxPortInfoState != NETX_PORT_STATE_ATTACH)
    {
        //接口未使能
        nxLan8700ReqPtr->nx_ip_driver_status = NX_NOT_SUCCESSFUL;
        return;
    }
    //驱动参数检查
    if(0 != NetX_DriverLAN8700_RequestCheck(nxLan8700ReqPtr))
    {
        return;
    }
    //获取当前任务指针
    netx_lan8700_thread_ptr = tx_thread_identify();
    //硬件初始化
    errorSubCode = Board_LAN8700_Init();
    if(errorSubCode != LH_ERR_NONE)
    {
        LowLevelPrintf("Board_LAN8700_Init Failed : 0X%08X\r\n",errorSubCode);
        //状态切换为已经接口附着
        nxPortInfo.nxPortInfoState = NETX_PORT_STATE_ATTACH;
        //回传失败
        nxLan8700ReqPtr->nx_ip_driver_status = NX_NOT_SUCCESSFUL;
        return;
    }
    //检测网线连接状态
    errorSubCode = Board_LAN8700_GetLinkState(&(nxPortInfo.netPortState));
    if(errorSubCode != LH_ERR_NONE)
    {
        //读取网口状态失败
        LowLevelPrintf("Board_LAN8700_GetLinkState Failed : 0X%08X\r\n",errorSubCode);
        //状态切换为已经接口附着
        nxPortInfo.nxPortInfoState = NETX_PORT_STATE_ATTACH;
        //回传失败
        nxLan8700ReqPtr->nx_ip_driver_status = NX_NOT_SUCCESSFUL;
        return;
    }
    //设置网线接口状态
    if((nxPortInfo.netPortState == NET_PORT_STATE_LINK_DOWN)||(nxPortInfo.netPortState == NET_PORT_STATE_AUTONEGO_NOTDONE)||
            (nxPortInfo.netPortState == NET_PORT_STATE_UNKNOW))
    {
        //网线未连接
        nxPortInfo.nxPortInfoInterfacePtr->nx_interface_link_up = NX_FALSE;
    }
    else
    {
        //网络已经联通
        nxPortInfo.nxPortInfoInterfacePtr->nx_interface_link_up = NX_TRUE;
    }
    //更新MAC层的状态
    MCU_ETH_ReflushModeSpeed(nxPortInfo.netPortState);
    //设置最大传输单元
    nxLan8700ReqPtr->nx_ip_driver_interface->nx_interface_ip_mtu_size = 
                        NX_DRIVER_ETHERNET_MTU - NX_DRIVER_ETHERNET_FRAME_SIZE; 
    //设置MAC地址物理接口
    nxLan8700ReqPtr->nx_ip_driver_interface->nx_interface_physical_address_msw =
        (ULONG)((ETH_Handler.Init.MACAddr[0] << 8) | (ETH_Handler.Init.MACAddr[1]));
    nxLan8700ReqPtr->nx_ip_driver_interface->nx_interface_physical_address_lsw =
        (ULONG)((ETH_Handler.Init.MACAddr[2] << 24) | (ETH_Handler.Init.MACAddr[3] << 16) |
                (ETH_Handler.Init.MACAddr[4] << 8) | (ETH_Handler.Init.MACAddr[5]));
    //设置IP与MAC地址绑定
    nxLan8700ReqPtr->nx_ip_driver_interface -> nx_interface_address_mapping_needed =  NX_TRUE;
    //状态切换为已经使能
    nxPortInfo.nxPortInfoState = NETX_PORT_STATE_INIT;
    //回传成功
    nxLan8700ReqPtr->nx_ip_driver_status = NX_SUCCESS;
}

//启用链接
static void NetX_DriverLAN8700_Enable(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    LH_ERR errorSubCode = LH_ERR_NONE;
    //状态匹配,必须是使能状态或者初始化状态
    if((nxPortInfo.nxPortInfoState != NETX_PORT_STATE_INIT)&&
        (nxPortInfo.nxPortInfoState != NETX_PORT_STATE_ENABLE))
    {
        //接口未使能
        nxLan8700ReqPtr->nx_ip_driver_status = NX_NOT_SUCCESSFUL;
        return;
    }
    //驱动参数检查
    if(0 != NetX_DriverLAN8700_RequestCheck(nxLan8700ReqPtr))
    {
        return;
    }
    //已经使能
    if(nxPortInfo.nxPortInfoState == NETX_PORT_STATE_ENABLE)
    {
        nxLan8700ReqPtr->nx_ip_driver_status =  NX_ALREADY_ENABLED;
        return;
    }
    //检测网线连接状态
    errorSubCode = Board_LAN8700_GetLinkState(&(nxPortInfo.netPortState));
    if(errorSubCode != LH_ERR_NONE)
    {
        //读取网口状态失败
        LowLevelPrintf("Board_LAN8700_GetLinkState Failed : 0X%08X\r\n",errorSubCode);
        //回传失败
        nxLan8700ReqPtr->nx_ip_driver_status = NX_NOT_SUCCESSFUL;
        return;
    }
    //设置网线接口状态
    if((nxPortInfo.netPortState == NET_PORT_STATE_LINK_DOWN)||(nxPortInfo.netPortState == NET_PORT_STATE_AUTONEGO_NOTDONE)||
            (nxPortInfo.netPortState == NET_PORT_STATE_UNKNOW))
    {
        //网线未连接
        nxPortInfo.nxPortInfoInterfacePtr->nx_interface_link_up = NX_FALSE;
    }
    else
    {
        //网络已经联通
        nxPortInfo.nxPortInfoInterfacePtr->nx_interface_link_up = NX_TRUE;
    }
    //更新MAC层的状态
    MCU_ETH_ReflushModeSpeed(nxPortInfo.netPortState);
    //使能
    MCU_ETH_Start();
    //状态切换为已经使能
    nxPortInfo.nxPortInfoState = NETX_PORT_STATE_ENABLE;
    //设置已连接
    nxPortInfo.nxPortInfoIP_Ptr->nx_ip_driver_link_up =  NX_TRUE;
    //回传成功
    nxLan8700ReqPtr->nx_ip_driver_status = NX_SUCCESS;
    return;
}

//禁用链接
static void NetX_DriverLAN8700_Disable(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    if(nxPortInfo.nxPortInfoState != NETX_PORT_STATE_ENABLE)
    {
        //接口未使能
        nxLan8700ReqPtr->nx_ip_driver_status = NX_NOT_SUCCESSFUL;
        return;
    }
    //驱动参数检查
    if(0 != NetX_DriverLAN8700_RequestCheck(nxLan8700ReqPtr))
    {
        return;
    }
    //接口停止
    MCU_ETH_Stop();
    //状态切换为初始化
    nxPortInfo.nxPortInfoState = NETX_PORT_STATE_INIT;
    //设置断开链接
    nxPortInfo.nxPortInfoIP_Ptr->nx_ip_driver_link_up = NX_FALSE;
    //回传成功
    nxLan8700ReqPtr->nx_ip_driver_status = NX_SUCCESS;
    return;
}

//取消初始化链接
static void NetX_DriverLAN8700_Uninitialize(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    //保存信息初始化
    NetX_DriverLAN8700InfoReset();
    //关闭ETH
    MCU_ETH_Stop();
    //状态修改为空闲
    nxPortInfo.nxPortInfoState = NETX_PORT_STATE_IDLE;
    //返回成功
    nxLan8700ReqPtr->nx_ip_driver_status = NX_SUCCESS;
}

//0无错误,1有错误
uint8_t NetX_DriverLAN8700_PacketSendCheckLinkState(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    //检查网络连接状态
    LH_ERR errorSubCode = LH_ERR_NONE;
    //读取芯片接口状态
    errorSubCode = Board_LAN8700_GetLinkState(&(nxPortInfo.netPortState));
    if(errorSubCode != LH_ERR_NONE)
    {
        //读取网口状态失败
        LowLevelPrintf("Board_LAN8700_GetLinkState Failed : 0X%08X\r\n",errorSubCode);
        return 1;
    }
    //未连接状态,协商未完成状态,位置状态统一看作未连接
    if((nxPortInfo.netPortState == NET_PORT_STATE_LINK_DOWN)||(nxPortInfo.netPortState == NET_PORT_STATE_AUTONEGO_NOTDONE)||
        (nxPortInfo.netPortState == NET_PORT_STATE_UNKNOW))
    {
        //网线未连接
        nxPortInfo.nxPortInfoInterfacePtr->nx_interface_link_up = NX_FALSE;
    }
    else
    {
        //网络已经联通
        nxPortInfo.nxPortInfoInterfacePtr->nx_interface_link_up = NX_TRUE;
    }
    //解析速度和双工模式
    Board_LAN8700_GetModeSpeed(&(nxPortInfo.linkSpeed),&(nxPortInfo.deplexMode));
    //根据网络接口状态修正MAC层
    MCU_ETH_ReflushModeSpeed(nxPortInfo.netPortState);
    //决定是否可以发送
    if((nxPortInfo.netPortState == NET_PORT_STATE_LINK_DOWN)||(nxPortInfo.netPortState == NET_PORT_STATE_AUTONEGO_NOTDONE)||
        (nxPortInfo.netPortState == NET_PORT_STATE_UNKNOW))
    {
        //回传失败
        return 1;
    }
    return 0;
}

//数据包发送
static void NetX_DriverLAN8700_PacketSend(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    if (nxPortInfo.nxPortInfoState != NETX_PORT_STATE_ENABLE)
    {
        //接口未使能
        nxLan8700ReqPtr->nx_ip_driver_status = NX_NOT_SUCCESSFUL;
        //释放内存
        nx_packet_transmit_release(nxLan8700ReqPtr->nx_ip_driver_packet);
        return;
    }
    //驱动参数检查
    if (0 != NetX_DriverLAN8700_RequestCheck(nxLan8700ReqPtr))
    {
        //数据不匹配
        nxLan8700ReqPtr->nx_ip_driver_status = NX_NOT_SUCCESSFUL;
        //释放内存
        nx_packet_transmit_release(nxLan8700ReqPtr->nx_ip_driver_packet);
        return;
    }
    if(0 != NetX_DriverLAN8700_PacketSendCheckLinkState(nxLan8700ReqPtr))
    {
        //网线未连接
        nxLan8700ReqPtr->nx_ip_driver_status = NX_NOT_SUCCESSFUL;
        //释放内存
        nx_packet_transmit_release(nxLan8700ReqPtr->nx_ip_driver_packet);
        return;
    }
    NX_IP *ip_ptr = nxLan8700ReqPtr->nx_ip_driver_ptr;
    //保存要发送的包数据
    NX_PACKET *packet_ptr = nxLan8700ReqPtr->nx_ip_driver_packet;
    //以太网帧,也就是前面SA+DA+TYPE
    ULONG *ethernet_frame_ptr;
    //后续操作是为了添加帧描述符,注意缓存长度必须是1536才能这么操作
    packet_ptr->nx_packet_prepend_ptr = packet_ptr->nx_packet_prepend_ptr - NX_DRIVER_ETHERNET_FRAME_SIZE;
    //长度增加帧描述符
    packet_ptr->nx_packet_length = packet_ptr->nx_packet_length + NX_DRIVER_ETHERNET_FRAME_SIZE;
    //帧类型符,注意32未对齐
    ethernet_frame_ptr = (ULONG *)(packet_ptr->nx_packet_prepend_ptr - 2);
    //设置目标MAC地址
    *ethernet_frame_ptr = nxLan8700ReqPtr->nx_ip_driver_physical_address_msw;
    //目标MAC 低四个字节
    *(ethernet_frame_ptr + 1) = nxLan8700ReqPtr->nx_ip_driver_physical_address_lsw;
    //设置源地址
    *(ethernet_frame_ptr + 2) = (ip_ptr->nx_ip_arp_physical_address_msw << 16) |
                                (ip_ptr->nx_ip_arp_physical_address_lsw >> 16);
    *(ethernet_frame_ptr + 3) = (ip_ptr->nx_ip_arp_physical_address_lsw << 16);
    //设置协议类型
    if ((nxLan8700ReqPtr->nx_ip_driver_command == NX_LINK_ARP_SEND) ||
        (nxLan8700ReqPtr->nx_ip_driver_command == NX_LINK_ARP_RESPONSE_SEND))
    {
        //ARP数据包
        *(ethernet_frame_ptr + 3) |= NX_DRIVER_ETHERNET_ARP;
    }
    else if (nxLan8700ReqPtr->nx_ip_driver_command == NX_LINK_RARP_SEND)
    {
        //RARP数据包
        *(ethernet_frame_ptr + 3) |= NX_DRIVER_ETHERNET_RARP;
    }
    else 
    {
        if (packet_ptr->nx_packet_ip_version == NX_IP_VERSION_V6)
        {
            //IPV6数据包
            *(ethernet_frame_ptr + 3) |= NX_DRIVER_ETHERNET_IPV6;
        }
        else
        {
            //IPV4数据包
            *(ethernet_frame_ptr + 3) |= NX_DRIVER_ETHERNET_IP;
        }
    }
    //修改大小端
    NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr));
    NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr + 1));
    NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr + 2));
    NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr + 3));
    //大于单包MTU最大值
    if (packet_ptr->nx_packet_length > NX_DRIVER_ETHERNET_MTU)
    {
        //移除添加的协议头
        NX_DRIVER_ETHERNET_HEADER_REMOVE(packet_ptr);
        //返回错误
        nxLan8700ReqPtr->nx_ip_driver_status = NX_NOT_SUCCESSFUL;
        //释放内存
        nx_packet_transmit_release(packet_ptr);
        return;
    }
    //计算出全部需要发送的数据包,发送出去
    //硬件发送
    ETH_MULTI_PACK* multiPackSendPtr = UserMemMallocWhileSuccess(NETX_LAN8700_MEM_REGION,((sizeof(ETH_MULTI_PACK))/(sizeof(uint8_t))));
    uint8_t indexUtil = 0;
    NX_PACKET* packet_Copyptr = packet_ptr;
    while(packet_Copyptr != NULL)
    {
        //写入地址和长度
        multiPackSendPtr->dataBufferPtrArray[indexUtil] = packet_Copyptr->nx_packet_prepend_ptr;
        multiPackSendPtr->dataLengthArray[indexUtil] = packet_Copyptr -> nx_packet_append_ptr - packet_Copyptr->nx_packet_prepend_ptr;
#if((NETX_LAN8700_DRIVER_DEBUG_SHOW_BUF == 1)||(NETX_LAN8700_DRIVER_DEBUG_SHOW == 1))
    LowLevelPrintf("LAN8700 Send Data Length: %d\r\n",multiPackSendPtr->dataLengthArray[indexUtil]);
#endif
#if(NETX_LAN8700_DRIVER_DEBUG_SHOW_BUF == 1)                
        //显示发送
        LowLevelShowArrayBuffer("LAN8700 Send DataBuffer:\r\n",multiPackSendPtr->dataBufferPtrArray[indexUtil],
                                    multiPackSendPtr->dataLengthArray[indexUtil]);
#endif
        indexUtil += 1;
        packet_Copyptr = packet_Copyptr->nx_packet_next;
        //发送数据包大于系统内部缓存包的时候,丢弃报错
        if(indexUtil >= ETH_TX_DESC_CNT)
        {
#if(NETX_LAN8700_DRIVER_DEBUG_SHOW == 1)
            LowLevelPrintf("NetX_DriverLAN8700_PacketSend,PackMoreThen ETH_TX_DESC_CNT: %d\r\n",ETH_TX_DESC_CNT);
#endif
            nxLan8700ReqPtr->nx_ip_driver_status = NX_NOT_SUCCESSFUL;
            //移除添加的协议头
            NX_DRIVER_ETHERNET_HEADER_REMOVE(packet_ptr);
            //释放内存
            nx_packet_transmit_release(nxLan8700ReqPtr->nx_ip_driver_packet);
            //释放内存
            UserMemFree(NETX_LAN8700_MEM_REGION,multiPackSendPtr);
            return;
        }
    }
    multiPackSendPtr->packCount = indexUtil;
    //发送数据
    MCU_ETH_WriteMultiPack(multiPackSendPtr);
    //移除添加的协议头
    NX_DRIVER_ETHERNET_HEADER_REMOVE(packet_ptr);
    //释放内存
    nx_packet_transmit_release(nxLan8700ReqPtr->nx_ip_driver_packet);
    nxLan8700ReqPtr->nx_ip_driver_packet = NULL;
    //释放内存
    UserMemFree(NETX_LAN8700_MEM_REGION,multiPackSendPtr);
    //设置发送成功
    nxLan8700ReqPtr->nx_ip_driver_status = NX_SUCCESS;
    return;
}

//多播组联接
static void NetX_DriverLAN8700_MultiCastJoin(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    //状态匹配,必须是使能状态或者初始化状态
    if((nxPortInfo.nxPortInfoState != NETX_PORT_STATE_INIT)&&
        (nxPortInfo.nxPortInfoState !=NETX_PORT_STATE_ENABLE))
    {
        //接口未使能
        nxLan8700ReqPtr->nx_ip_driver_status = NX_NOT_ENABLED;
        return;
    }
    //驱动参数检查
    if(0 != NetX_DriverLAN8700_RequestCheck(nxLan8700ReqPtr))
    {
        return;
    }
    //使能多播接收
    MCU_ETH_EnableMultiCast();
    nxPortInfo.nxPortInfoMultiCastCount += 1;
    //结果返回
    nxLan8700ReqPtr->nx_ip_driver_status =  NX_SUCCESS;
}

//多播组离开
static void NetX_DriverLAN8700_MultiCastLeave(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    //状态匹配,必须是使能状态或者初始化状态
    if((nxPortInfo.nxPortInfoState != NETX_PORT_STATE_INIT)&&
        (nxPortInfo.nxPortInfoState !=NETX_PORT_STATE_ENABLE))
    {
        //接口未使能
        nxLan8700ReqPtr->nx_ip_driver_status = NX_NOT_ENABLED;
        return;
    }
    //驱动参数检查
    if(0 != NetX_DriverLAN8700_RequestCheck(nxLan8700ReqPtr))
    {
        return;
    }
    //计数递减
    nxPortInfo.nxPortInfoMultiCastCount -= 1;
    if(nxPortInfo.nxPortInfoMultiCastCount == 0)
    {
        //关闭多播
        MCU_ETH_DisableMultiCast();
    }
    //结果返回
    nxLan8700ReqPtr->nx_ip_driver_status =  NX_SUCCESS;
}

//附加接口
//此请求从 NetX Duo 调用到设备驱动程序，
//允许驱动程序将驱动程序实例与 IP 中的相应 IP 实例和物理接口实例相关联
static void NetX_DriverLAN8700_InterfaceAttach(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    //保存信息初始化
    NetX_DriverLAN8700InfoReset();
    //保存IP 实例和接口实例
    nxPortInfo.nxPortInfoIP_Ptr = nxLan8700ReqPtr->nx_ip_driver_ptr;
    nxPortInfo.nxPortInfoInterfacePtr = nxLan8700ReqPtr->nx_ip_driver_interface;
    //状态修改为IP附着
    nxPortInfo.nxPortInfoState = NETX_PORT_STATE_ATTACH;
    //返回成功
    nxLan8700ReqPtr->nx_ip_driver_status = NX_SUCCESS;
}

//分离接口
static void NetX_DriverLAN8700_InterfaceDetach(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    //保存信息初始化
    NetX_DriverLAN8700InfoReset();
    //关闭ETH
    MCU_ETH_Stop();
    //状态修改为空闲
    nxPortInfo.nxPortInfoState = NETX_PORT_STATE_IDLE;
    //返回成功
    nxLan8700ReqPtr->nx_ip_driver_status = NX_SUCCESS;
}

//获取链接状态
static void NetX_DriverLAN8700_GetStatus(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    LH_ERR errorSubCode = LH_ERR_NONE;
    //读取芯片接口状态
    errorSubCode = Board_LAN8700_GetLinkState(&(nxPortInfo.netPortState));
    if(errorSubCode != LH_ERR_NONE)
    {
        //读取网口状态失败
        LowLevelPrintf("Board_LAN8700_GetLinkState Failed : 0X%08X\r\n",errorSubCode);
        //回传失败
        nxLan8700ReqPtr->nx_ip_driver_status = NX_NOT_SUCCESSFUL;
        return;
    }
    //未连接状态,协商未完成状态,位置状态统一看作未连接
    if((nxPortInfo.netPortState == NET_PORT_STATE_LINK_DOWN)||(nxPortInfo.netPortState == NET_PORT_STATE_AUTONEGO_NOTDONE)||
        (nxPortInfo.netPortState == NET_PORT_STATE_UNKNOW))
    {
        //网线未连接
        nxPortInfo.nxPortInfoInterfacePtr->nx_interface_link_up = NX_FALSE;
        *(nxLan8700ReqPtr->nx_ip_driver_return_ptr) = NX_FALSE;
    }
    else
    {
        //网络已经联通
        nxPortInfo.nxPortInfoInterfacePtr->nx_interface_link_up = NX_TRUE;
        *(nxLan8700ReqPtr->nx_ip_driver_return_ptr) = NX_TRUE;
    }
    //解析速度和双工模式
    Board_LAN8700_GetModeSpeed(&(nxPortInfo.linkSpeed),&(nxPortInfo.deplexMode));
    //根据网络接口状态修正MAC层
    MCU_ETH_ReflushModeSpeed(nxPortInfo.netPortState);
    //返回成功
    nxLan8700ReqPtr->nx_ip_driver_status = NX_SUCCESS;
    return; 
}

//获取链接速度
static void NetX_DriverLAN8700_GetSpeed(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    LH_ERR errorSubCode = LH_ERR_NONE;
    //读取芯片接口状态
    errorSubCode = Board_LAN8700_GetLinkState(&(nxPortInfo.netPortState));
    if(errorSubCode != LH_ERR_NONE)
    {
        //读取网口状态失败
        LowLevelPrintf("Board_LAN8700_GetLinkState Failed : 0X%08X\r\n",errorSubCode);
        //回传失败
        nxLan8700ReqPtr->nx_ip_driver_status = NX_NOT_SUCCESSFUL;
        return;
    }
    //解析速度和双工模式
    Board_LAN8700_GetModeSpeed(&(nxPortInfo.linkSpeed),&(nxPortInfo.deplexMode));
    //未连接状态,协商未完成状态,位置状态统一看作未连接
    if((nxPortInfo.netPortState == NET_PORT_STATE_LINK_DOWN)||(nxPortInfo.netPortState == NET_PORT_STATE_AUTONEGO_NOTDONE)||
        (nxPortInfo.netPortState == NET_PORT_STATE_UNKNOW))
    {
        //网线未连接
        nxPortInfo.nxPortInfoInterfacePtr->nx_interface_link_up = NX_FALSE;
        *(nxLan8700ReqPtr->nx_ip_driver_return_ptr) = nxPortInfo.linkSpeed;
    }
    else
    {
        //网络已经联通
        nxPortInfo.nxPortInfoInterfacePtr->nx_interface_link_up = NX_TRUE;
        *(nxLan8700ReqPtr->nx_ip_driver_return_ptr) = nxPortInfo.linkSpeed;
    }
    //根据网络接口状态修正MAC层
    MCU_ETH_ReflushModeSpeed(nxPortInfo.netPortState);
    //设置结果
    nxLan8700ReqPtr->nx_ip_driver_return_ptr = (ULONG*)&(nxPortInfo.linkSpeed);
    //返回成功
    nxLan8700ReqPtr->nx_ip_driver_status = NX_SUCCESS;
    return; 
}

//获取双工类型
static void NetX_DriverLAN8700_GetDuplexType(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    LH_ERR errorSubCode = LH_ERR_NONE;
    //读取芯片接口状态
    errorSubCode = Board_LAN8700_GetLinkState(&(nxPortInfo.netPortState));
    if(errorSubCode != LH_ERR_NONE)
    {
        //读取网口状态失败
        LowLevelPrintf("Board_LAN8700_GetLinkState Failed : 0X%08X\r\n",errorSubCode);
        //回传失败
        nxLan8700ReqPtr->nx_ip_driver_status = NX_NOT_SUCCESSFUL;
        return;
    }
    //解析速度和双工模式
    Board_LAN8700_GetModeSpeed(&(nxPortInfo.linkSpeed),&(nxPortInfo.deplexMode));
    //未连接状态,协商未完成状态,位置状态统一看作未连接
    if((nxPortInfo.netPortState == NET_PORT_STATE_LINK_DOWN)||(nxPortInfo.netPortState == NET_PORT_STATE_AUTONEGO_NOTDONE)||
        (nxPortInfo.netPortState == NET_PORT_STATE_UNKNOW))
    {
        //网线未连接
        nxPortInfo.nxPortInfoInterfacePtr->nx_interface_link_up = NX_FALSE;
        *(nxLan8700ReqPtr->nx_ip_driver_return_ptr) = nxPortInfo.deplexMode;
    }
    else
    {
        //网络已经联通
        nxPortInfo.nxPortInfoInterfacePtr->nx_interface_link_up = NX_TRUE;
        *(nxLan8700ReqPtr->nx_ip_driver_return_ptr) = nxPortInfo.deplexMode;
    }
    //根据网络接口状态修正MAC层
    MCU_ETH_ReflushModeSpeed(nxPortInfo.netPortState);
    //设置结果
    nxLan8700ReqPtr->nx_ip_driver_return_ptr = (ULONG*)&(nxPortInfo.deplexMode);
    //返回成功
    nxLan8700ReqPtr->nx_ip_driver_status = NX_SUCCESS;
    return; 
}

//获取错误计数
static void NetX_DriverLAN8700_GetErrorCount(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    nxLan8700ReqPtr->nx_ip_driver_return_ptr = (ULONG*)&(nxPortInfo.errorCount);
    //返回成功
    nxLan8700ReqPtr->nx_ip_driver_status = NX_SUCCESS;
    return; 
}

//获取接收数据包计数
static void NetX_DriverLAN8700_GetRxCount(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    *(nxLan8700ReqPtr->nx_ip_driver_return_ptr) = nxPortInfo.recvPackCount;
    //返回成功
    nxLan8700ReqPtr->nx_ip_driver_status = NX_SUCCESS;
    return; 
}

//获取传输数据包计数
static void NetX_DriverLAN8700_GetTxCount(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    *(nxLan8700ReqPtr->nx_ip_driver_return_ptr) = nxPortInfo.sendPackCount;
    //返回成功
    nxLan8700ReqPtr->nx_ip_driver_status = NX_SUCCESS;
    return; 
}

//获取分配错误
static void NetX_DriverLAN8700_GetAllocErrorCount(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    *(nxLan8700ReqPtr->nx_ip_driver_return_ptr) = nxPortInfo.allocFailCount;
    //返回成功
    nxLan8700ReqPtr->nx_ip_driver_status = NX_SUCCESS;
    return; 
}

//数据处理调用相应的数据包到相应的处理线程
static void NetX_DriverLAN8700_SendDataToNetX(NX_IP *ip_ptr, NX_PACKET *packet_ptr)
{
    uint16_t packet_type;
    //设置发送接口
    packet_ptr->nx_packet_ip_interface = nxPortInfo.nxPortInfoInterfacePtr;
    //获取数据包的类型 6DA+6SA+2TYPE
    packet_type = (USHORT)(((UINT)(*(packet_ptr->nx_packet_prepend_ptr + 12))) << 8) |
                    ((UINT)(*(packet_ptr->nx_packet_prepend_ptr + 13)));
    //IPV4与IPV6
    if (packet_type == NX_DRIVER_ETHERNET_IP || packet_type == NX_DRIVER_ETHERNET_IPV6)
    {
        //去除帧头
        NX_DRIVER_ETHERNET_HEADER_REMOVE(packet_ptr);
        //调用IP包处理
        _nx_ip_packet_deferred_receive(ip_ptr, packet_ptr);
#if(NETX_LAN8700_DRIVER_DEBUG_SHOW == 1)
        //日志打印
        LowLevelPrintf("_nx_ip_packet_deferred_receive\r\n");
#endif
    }
    else if(packet_type == NX_DRIVER_ETHERNET_ARP)
    {
        //ARP包,去除帧头
        NX_DRIVER_ETHERNET_HEADER_REMOVE(packet_ptr);
        //调用ARP包处理
        _nx_arp_packet_deferred_receive(ip_ptr, packet_ptr);
#if(NETX_LAN8700_DRIVER_DEBUG_SHOW == 1)
        //日志打印
        LowLevelPrintf("_nx_arp_packet_deferred_receive\r\n");
#endif
    }
    else if (packet_type == NX_DRIVER_ETHERNET_RARP)
    {
        //RARP包去除帧头
        NX_DRIVER_ETHERNET_HEADER_REMOVE(packet_ptr);
        //调用RARP包处理
        _nx_rarp_packet_deferred_receive(ip_ptr, packet_ptr);
#if(NETX_LAN8700_DRIVER_DEBUG_SHOW == 1)
        //日志打印
        LowLevelPrintf("_nx_rarp_packet_deferred_receive\r\n");
#endif
    }
    else
    {
        //不支持的包类型,释放掉
        nx_packet_release(packet_ptr);
        //日志打印
        LowLevelPrintf("Unknow DataPack Type\r\n");
    }
}
#ifndef NX_DRIVER_DEFERRED_PROCESSING
//NETX数据快速提交
void NetX_DriverLAN8700_RecvDataProcess(void)
{
    uint8_t* readDataPtr = NULL;
    uint32_t dataLength = 0;
    uint32_t nxResult = NX_SUCCESS;
    //清除事件
    uint32_t dataEvent = 0;
    //关闭中断
    uint32_t lastIntState = CoreLockIntBasePri();
    //加载和处理与中断相关的变量
    dataEvent = nxPortInfo.dataEvent;
    nxPortInfo.dataEvent = 0;
    //打开中断
    CoreUnlockIntBasePri(lastIntState);
    NX_PACKET *nxRecvPackPtr = NULL;;
    if(dataEvent & NX_DRIVER_DEFERRED_PACKET_RECEIVED)
    {
        //把数据都收光
        do
        {
            //读取数据
            MCU_ETH_ReadRecvData(&readDataPtr,&dataLength);
            //处理数据
            if(dataLength != 0)
            {
#if((NETX_LAN8700_DRIVER_DEBUG_SHOW_BUF == 1)||(NETX_LAN8700_DRIVER_DEBUG_SHOW == 1))                
                //打印接收数据长度
                LowLevelPrintf("NetX_DriverLAN8700_RecvDataProcess DataLength: %d\r\n",dataLength);
#endif
                //增加一次数据包接收
                nxPortInfo.recvPackCount += 1;
                //申请内存数据包
                nxResult = nx_packet_allocate(&nxPacketPoolLan8700, &nxRecvPackPtr,NX_RECEIVE_PACKET, NX_NO_WAIT);
                if (nxResult == NX_SUCCESS)
                {
                    //参数赋值
                    //偏移两个字节,实现4字节对齐的IP包头
                    nxRecvPackPtr->nx_packet_prepend_ptr += 2;
                    //设备包尾
                    nxRecvPackPtr->nx_packet_append_ptr = nxRecvPackPtr->nx_packet_prepend_ptr + dataLength;
                    nxRecvPackPtr->nx_packet_length = dataLength;
                    //单包数据长度可以放下,就不用再加数据了
                    nxRecvPackPtr->nx_packet_next = NULL;
                    //数据拷贝
                    UserMemCopy(nxRecvPackPtr->nx_packet_prepend_ptr,readDataPtr,dataLength);
                    //发送到协议层
                    NetX_DriverLAN8700_SendDataToNetX(nxPortInfo.nxPortInfoIP_Ptr, nxRecvPackPtr);
#if(NETX_LAN8700_DRIVER_DEBUG_SHOW_BUF == 1)                
                    //显示接收
                    LowLevelShowArrayBuffer("LAN8700 Recv DataBuffer:\r\n",readDataPtr,dataLength);
#endif
                }
                else
                {
                    //没有申请到数据包
                    LowLevelPrintf("NetX_DriverLAN8700_RecvDataProcess nx_packet_allocate Failed: %d\r\n",nxResult);
                    //申请失败计数
                    nxPortInfo.allocFailCount += 1;
                }
                //释放数据
                MCU_ETH_ReleaseRecvBuf(readDataPtr);
                readDataPtr = NULL;
            }
            else
            {
                //没读取到数据数据,退出
                break;
            }
        }while(1);
    }
}
#endif

//驱动程序延迟处理,主要是读取网口数据包
static void NetX_DriverLAN8700_DeferredProcessing(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    uint8_t* readDataPtr = NULL;
    uint32_t dataLength = 0;
    uint32_t nxResult = NX_SUCCESS;
    //清除事件
    uint32_t dataEvent = 0;
    //关闭中断
    uint32_t lastIntState = CoreLockIntBasePri();
    //加载和处理与中断相关的变量
    dataEvent = nxPortInfo.dataEvent;
    nxPortInfo.dataEvent = 0;
    //打开中断
    CoreUnlockIntBasePri(lastIntState);
    NX_PACKET *nxRecvPackPtr = NULL;;
    if(dataEvent & NX_DRIVER_DEFERRED_PACKET_RECEIVED)
    {
        //把数据都收光
        do
        {
            //读取数据
            MCU_ETH_ReadRecvData(&readDataPtr,&dataLength);
            //处理数据
            if(dataLength != 0)
            {
#if((NETX_LAN8700_DRIVER_DEBUG_SHOW_BUF == 1)||(NETX_LAN8700_DRIVER_DEBUG_SHOW == 1))                
                //打印接收数据长度
                LowLevelPrintf("NetX_DriverLAN8700_DeferredProcessing DataLength: %d\r\n",dataLength);
#endif
                //增加一次数据包接收
                nxPortInfo.recvPackCount += 1;
                //申请内存数据包
                nxResult = nx_packet_allocate(&nxPacketPoolLan8700, &nxRecvPackPtr,NX_RECEIVE_PACKET, NX_NO_WAIT);
                if (nxResult == NX_SUCCESS)
                {
                    //参数赋值
                    //偏移两个字节,实现4字节对齐的IP包头
                    nxRecvPackPtr->nx_packet_prepend_ptr += 2;
                    //设备包尾
                    nxRecvPackPtr->nx_packet_append_ptr = nxRecvPackPtr->nx_packet_prepend_ptr + dataLength;
                    nxRecvPackPtr->nx_packet_length = dataLength;
                    //单包数据长度可以放下,就不用再加数据了
                    nxRecvPackPtr->nx_packet_next = NULL;
                    //数据拷贝
                    UserMemCopy(nxRecvPackPtr->nx_packet_prepend_ptr,readDataPtr,dataLength);
                    //发送到协议层
                    NetX_DriverLAN8700_SendDataToNetX(nxLan8700ReqPtr->nx_ip_driver_ptr, nxRecvPackPtr);
                    
#if(NETX_LAN8700_DRIVER_DEBUG_SHOW_BUF == 1)                
                //显示接收
                LowLevelShowArrayBuffer("LAN8700 Recv DataBuffer:\r\n",readDataPtr,dataLength);
#endif
                }
                else
                {
                    //没有申请到数据包
                    LowLevelPrintf("NetX_DriverLAN8700_DeferredProcessing nx_packet_allocate Failed: %d\r\n",nxResult);
                    //申请失败计数
                    nxPortInfo.allocFailCount += 1;
                }
                //释放数据
                MCU_ETH_ReleaseRecvBuf(readDataPtr);
                readDataPtr = NULL;
            }
            else
            {
                //没读取到数据数据,退出
                break;
            }
        }while(1);
    }
    //处理完成,返回成功
    nxLan8700ReqPtr->nx_ip_driver_status = NX_SUCCESS;
}


//设置物理地址
static void NetX_DriverLAN8700_SetPhysicalAddress(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    asm("nop");
}

//用户命令
static void NetX_DriverLAN8700_UserCommand(NX_IP_DRIVER *nxLan8700ReqPtr)
{
    asm("nop");
}

/*-----------------------------------------系统接收中断回调函数-------------------------------------------*/
//ETH中断回调函数
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
    if(heth->Instance == ETH)
    {
        uint32_t dataEvent = nxPortInfo.dataEvent;
        nxPortInfo.dataEvent |= NX_DRIVER_DEFERRED_PACKET_RECEIVED;
#ifdef NX_DRIVER_DEFERRED_PROCESSING
        if (!dataEvent)
        {
            //中断延迟处理事件
            _nx_ip_driver_deferred_processing(nxPortInfo.nxPortInfoIP_Ptr);
        }
#endif
    }
}
