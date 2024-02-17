#ifndef NX_USER_H
#define NX_USER_H
#include "tx_user.h"
#include "nx_user_ftp.h"
#include "nx_user_tftp.h"

/*--------------------------------------------------基础功能配置-----------------------------------------------------*/
//指定设备上物理网络接口的总数。 默认值为 1，在 nx_api.h 中定义；
//设备必须具有至少一个物理接口。 请注意，此数量不包括环回接口。
#define NX_MAX_PHYSICAL_INTERFACES                              1

//定义后，将禁用 NetX Duo 对环回接口的支持。127.0.0.1
//#define NX_DISABLE_LOOPBACK_INTERFACE

//定义后，将允许接口设备驱动程序指定额外的功能信息，如校验和卸载。 默认情况下，未定义此选项。
//#define NX_ENABLE_INTERFACE_CAPABILITY

//定义后，用于指定 ThreadX 计时器时钟周期的数量（以秒为单位）。 默认值派生自 ThreadX 符号 TX_TIMER_TICKS_PER_SECOND，
//默认情况下，此值设置为 100（10ms 计时器）。 应用程序应谨慎修改此值，因为其他 NetX Duo 模块将从 NX_IP_PERIODIC_RATE 派生计时信息。
#ifdef TX_TIMER_TICKS_PER_SECOND
#define NX_IP_PERIODIC_RATE                                     TX_TIMER_TICKS_PER_SECOND
#else
#define NX_IP_PERIODIC_RATE                                     100
#endif

//定义后，将启用 IP 原始数据包接收筛选器功能.需要更好地控制要接收的原始 IP 数据包类型的应用程序可以使用此功能。 
//IP 原始数据包筛选器功能还支持 BSD 兼容层中的原始套接字操作。 默认情况下，未定义此选项。
//#define NX_ENABLE_IP_RAW_PACKET_FILTER

//控制原始数据包接收队列中可以排队的原始 IP 数据包数量的符号。 默认值设置为 20。
#define NX_IP_RAW_MAX_QUEUE_DEPTH                               20

//定义后，将启用 IPv4 静态路由，可在其中向目标地址分配特定的下一跃点地址。 默认情况下，IPv4 静态路由处于禁用状态。
//#define NX_ENABLE_IP_STATIC_ROUTING

//定义后，将设置 IPv4 静态路由表中的最大条目数，这是传出接口以及给定目标地址的下一个跃点地址的列表。 默认值为 8，
//在 nx_api.h. 中定义。仅当定义了 NX_ENABLE_IP_STATIC_ROUTING 时才使用此符号。
//#define NX_IP_ROUTING_TABLE_SIZE                              8

//指定可以联接的多播组的最大数量。 默认值为 7，在 nx_api.h 中定义。 
//应用程序可以在包含 nx_api.h 之前定义该值，以替代默认值。
//#define NX_MAX_MULTICAST_GROUPS                               7


/*--------------------------------------------------IPV6功能配置-----------------------------------------------------*/
//构建 NetX Duo 库时禁用 IPv6 功能。 对于不需要 IPv6 的应用程序，这可以避免拉取支持 IPv6 所需的代码和额外的存储空间。
#define NX_DISABLE_IPV6

//指定 IPv6 地址池中的条目数。 在接口配置期间，NetX Duo 使用池中的 IPv6 条目。 默认值为 (NX_MAX_PHYSICAL_INTERFACES * 3)，
//以允许每个接口具有至少一个本地链接地址和两个全局地址。 请注意，所有接口共享 IPv6 地址池。
#ifdef NX_MAX_PHYSICAL_INTERFACES
#define NX_MAX_IPV6_ADDRESSES                                   (NX_MAX_PHYSICAL_INTERFACES * 3)
#endif

//定义后，将禁用 ICMPv6 重定向数据包处理。 默认情况下，NetX Duo 将处理重定向消息，并使用下一跃点 IP 地址信息更新目标表。
//#define NX_DISABLE_ICMPV6_REDIRECT_PROCESS

//定义后，将禁止 NetX Duo 处理在 IPv6 路由器播发数据包中接收到的信息。
//#define NX_DISABLE_ICMPV6_ROUTER_ADVERTISEMENT_PROCESS

//定义后，将禁止 NetX Duo 定期向路由器发送 IPv6 路由器请求消息。
//#define NX_DISABLE_ICMPV6_ROUTER_SOLICITATION

//定义在接收到路由器响应之前主机发送的路由器请求的最大数量。 如果未接收到任何响应，主机将得出结论，不存在任何路由器。 默认值为 3。
#define NX_ICMPV6_MAX_RTR_SOLICITATIONS                         3

//指定两个路由器请求消息之间的间隔。 默认值为 4。
#define NX_ICMPV6_RTR_SOLICITATION_INTERVAL                     4

//指定初始路由器请求的最大延迟（以秒为单位）。
#define NX_ICMPV6_RTR_SOLICITATION_DELAY                        1

//定义后，NetX Duo 不会发送 ICMPv4 错误消息来响应错误情况，如 IPv4 标头格式不正确。 默认情况下，未定义此选项。
//#define NX_DISABLE_ICMPV4_ERROR_MESSAGE

//定义后，将禁止 NetX Duo 发送 ICMPv6 错误消息以响应从另一台主机接收到的问题数据包（例如，已弃用格式不当的标头或数据包标头类型）。
//#define NX_DISABLE_ICMPV6_ERROR_MESSAGE

//定义后，此选项将禁用 IPv6 地址分配期间的重复地址检测 (DAD)。 可以通过手动配置或无状态地址自动配置来设置地址。
//#define NX_DISABLE_IPV6_DAD

//定义后，允许 NetX Duo 控制无状态地址自动配置功能。 默认情况下，未启用此选项。
//#define NX_IPV6_STATELESS_AUTOCONFIG_CONTROL

//定义后，允许在更改 IPv6 地址时调用回调函数。 默认情况下，未启用此选项。
//#define NX_ENABLE_IPV6_ADDRESS_CHANGE_NOTIFY

//定义后，此选项可防止 NetX Duo 在其超时过期之前删除早期缓存表条目，以便在表已满时为新条目留出空间。 永远不会清除静态和路由器条目。
//#define NX_DISABLE_IPV6_PURGE_UNUSED_CACHE_ENTRIES

//定义后，支持 IPv6 多播加入/离开函数。 默认情况下，未启用此选项。
//#define NX_ENABLE_IPV6_MULTICAST

//定义后，支持 IPv6 路径 MTU 发现功能。 默认情况下，未启用此选项。
//#define NX_ENABLE_IPV6_PATH_MTU_DISCOVERY

//指定计时器时钟周期中用于重置目标表中特定目标的路径 MTU 的等待间隔。 如果定义了 NX_DISABLE_IPV6_PATH_MTU_DISCOVERY，则定义此符号不起作用。
//仅在定义了 NX_ENABLE_IPV6_PATH_MTU_DISCOVERY 时，此选项才有效。 默认情况下，此值设置为 600（秒）。
#define NX_PATH_MTU_INCREASE_WAIT_INTERVAL                      600

/*--------------------------------------------------邻居发现协议配置RFC2461-----------------------------------------------------*/
//指定在需要 IPv6 地址和 MAC 地址之间的映射时，NetX Duo 传输作为 IPv6 邻居发现协议的一部分的邻居请求消息数。 
//在 nx_nd_cache.h 中定义，默认值为 3。
#define NX_MAX_MULTICAST_SOLICIT                                3

//指定 NetX Duo 传输的邻居请求消息数，以确定特定邻居的可访问性。 在 nx_nd_cache.h 中定义，默认值为 3。
#define NX_MAX_UNICAST_SOLICIT                                  3

//指定缓存条目以“可访问”状态存在而不从缓存目标 IPv6 地址接收任何数据包的超时（以秒为单位）。 
//在 nx_nd_cache.h 中定义，默认值为 30。
#define NX_REACHABLE_TIME                                       30

//指定 NetX Duo 发送的每个请求数据包之间的延迟时长（以毫秒为单位）。 
//在 nx_nd_cache.h 中定义，默认值为 1000。
#define NX_RETRANS_TIMER                                        1000

//指定向处于过时状态的缓存条目发出第一个请求之前的延迟（以秒为单位）。 
//在 nx_nd_cache.h 中定义，默认值为 5。
#define NX_DELAY_FIRST_PROBE_TIME                               5

//用于定义为要解析的 ND 缓存排队的最大数据包数的符号。 默认情况下，此符号设置为 4。
#define NX_ND_MAX_QUEUE_DEPTH                                   4

//指定在 NetX Duo 将接口地址标记为有效之前要发送的邻居请求消息数。 如果定义了 NX_DISABLE_IPV6_DAD（禁用了 DAD），
//则设置此选项将不起作用。 或者，值 0 将关闭 DAD，但 NetX Duo 中会保留 DAD 功能。 在 nx_api.h 中定义，默认值为 3。
#define NX_IPV6_DAD_TRANSMITS                                   3

//指定 IPv6 邻居缓存表中的条目数。 在 nx_nd_cache.h 中定义，默认值为 16。
#define NX_IPV6_NEIGHBOR_CACHE_SIZE                             16

//指定 IPv6 目标表中的条目数。 这会存储有关 IPv6 地址的下一跃点地址的信息。 
//在 nx_api.h 中定义，默认值为 8。
#define NX_IPV6_DESTINATION_TABLE_SIZE                          8

//指定前缀表的大小。 前缀信息从路由器播发获取，是 IPv6 地址配置的一部分。 
//在 nx_api.h 中定义，默认值为 8。
#define NX_IPV6_PREFIX_LIST_TABLE_SIZE                          8


/*--------------------------------------------------IP安全传输,也就是一堆安全协议-----------------------------------------------------*/
//定义后，将允许 NetX Duo 库支持 IPsec 操作。 此功能需要可选的 NetX Duo IPsec 模块。 
//默认情况下，未启用此功能
//#define NX_IPSEC_ENABLE

/*--------------------------------------------------NAT网络地址转换协议配置-----------------------------------------------------*/
//定义后，NetX Duo 是通过 NAT 过程生成的。 默认情况下，未定义此选项。
//#define NX_NAT_ENABLE

/*--------------------------------------------------IGMP组策略控制协议配置-----------------------------------------------------*/
//定义后，将禁用 IGMPv2 支持，NetX Duo 仅支持 IGMPv1。 默认情况下，此选项未设置，且在 nx_api.h 中定义。
//#define NX_DISABLE_IGMPV2

/*--------------------------------------------------ARP地址解析协议配置-----------------------------------------------------*/
//定义后，允许 NetX Duo 通过发送 ARP 响应保护其 IP 地址。
//#define NX_ARP_DEFEND_BY_REPLY

//定义后，允许 ARP 调用回调通知函数来检测 MAC 地址是否已更新。
//#define  NX_ENABLE_ARP_MAC_CHANGE_NOTIFICATION

//指定 ARP 条目保持有效的秒数。 默认值为 0 时，将禁用 ARP 条目的过期或老化，在 nx_api.h 中定义。 
//应用程序可以在包含 nx_api.h 之前定义该值，以替代默认值。
#define NX_ARP_EXPIRATION_RATE                                  0

//指定 ARP 重试之间的秒数。 默认值为 10（表示 10 秒），在 nx_api.h 中定义。 
//应用程序可以在包含 nx_api.h 之前定义该值，以替代默认值。
#define NX_ARP_UPDATE_RATE                                      10

//指定在没有 ARP 响应的情况下进行 ARP 重试的最大次数。 默认值为 18，
//在 nx_api.h 中定义。 应用程序可以在包含 nx_api.h 之前定义该值，以替代默认值。
#define NX_ARP_MAXIMUM_RETRIES                                  18

//指定在等待 ARP 响应时可以排队的数据包的最大数量。 默认值为 4，
//在 nx_api.h 中定义。
#define NX_ARP_MAX_QUEUE_DEPTH                                  4

//定义后，此选项用于禁止在 ARP 缓存中输入 ARP 请求信息。
//#define NX_DISABLE_ARP_AUTO_ENTRY

//定义 ARP 模块发出下一个防御数据包以响应指示地址冲突的传入 ARP 消息的时间间隔（以秒为单位）。
//默认值为10
//#define NX_ARP_DEFEND_INTERVAL                                10



/*--------------------------------------------------TCP通讯协议配置-----------------------------------------------------*/
//指定如何划分系统时钟周期 (NX_IP_PERIODIC_RATE) 的数量，以计算 TCP 延迟 ACK 处理的计时器速率。 
//默认值为 5（表示 200ms），在 nx_tcp.h 中定义。 应用程序可以在包含 nx_api.h 之前定义该值，以替代默认值。
#define NX_TCP_ACK_TIMER_RATE                                   5

//指定如何划分 NetX Duo 的内部时钟周期 (NX_IP_PERIODIC_RATE) 的数量，以计算快速 TCP 计时器速率。 快速 TCP 计时器用于驱动各种 TCP 计时器，包括延迟 ACK 计时器。 
//默认值为 10，表示 100ms（假设 ThreadX 计时器在 10ms 时运行）。 此值在 nx_tcp.h 中定义。 
//应用程序可以在包含 nx_api.h 之前定义该值，以替代默认值。
#define NX_TCP_FAST_TIMER_RATE                                  10

//指定如何划分系统时钟周期 (NX_IP_PERIODIC_RATE) 的数量，以计算 TCP 传输重试处理的计时器速率。 
//默认值为 1，表示 1 秒，在 nx_tcp.h 中定义。 应用程序可以在包含 nx_api.h 之前定义该值，以替代默认值。
#define NX_TCP_TRANSMIT_TIMER_RATE                              1

//指定在 KeepAlive 计时器激活之前处于非活动状态的秒数。 默认值为 7200（表示 2 小时），
//在 nx_tcp.h 中定义。 应用程序可以在包含 nx_api.h 之前定义该值，以替代默认值。
#define NX_TCP_KEEPALIVE_INITIAL                                7200

//指定在连接的另一方没有响应时，KeepAlive 计时器重试之间间隔的秒数。 
//默认值为 75（表示重试之间间隔 75 秒），在 nx_tcp.h 中定义。 应用程序可以在包含 nx_api.h 之前定义该值，以替代默认值。
#define NX_TCP_KEEPALIVE_RETRY                                  75

//用于定义可在 TCP 套接字接收队列中保留的无序 TCP 数据包最大数量的符号。 此符号可用于限制在 TCP 接收套接字中排队的数据包数量，
//防止数据包池耗尽。 默认情况下未定义此符号，因此在 TCP 套接字中排队的无序数据包数没有限制。默认值为8
#define NX_TCP_MAX_OUT_OF_ORDER_PACKETS                         8

//指定服务器侦听请求的最大数量。 默认值为 10，在 nx_api.h 中定义。 应用程序可以在包含 nx_api.h 之前定义该值，以替代默认值。
#define NX_MAX_LISTEN_REQUESTS                                  10

//定义后，此选项用于启用可选的 TCP KeepAlive 计时器。 默认设置未启用。
#define NX_ENABLE_TCP_KEEPALIVE

//定义后，将启用可选的 TCP 即时 ACK 响应处理。 定义此符号等效于将 NX_TCP_ACK_EVERY_N_PACKETS 定义为 1。
//#define NX_TCP_IMMEDIATE_ACK

//指定发送 ACK 之前要接收的 TCP 数据包的数量。 注意，如果启用了 NX_TCP_IMMEDIATE_ACK，但未启用 NX_TCP_ACK_EVERY_N_PACKETS，
//则此值将自动设置为 1，以实现向后兼容性。
//#define NX_TCP_ACK_EVERY_N_PACKETS                            2
#if (defined(NX_TCP_IMMEDIATE_ACK) && !defined(NX_TCP_ACK_EVERY_N_PACKETS))
#define NX_TCP_ACK_EVERY_N_PACKETS 1
#endif

//指定在连接被视为中断之前允许的数据传输重试次数。 默认值为 10（表示 10 次重试），
//在 nx_tcp.h 中定义。 应用程序可以在包含 nx_api.h 之前定义该值，以替代默认值。
#define NX_TCP_MAXIMUM_RETRIES                                  10

//指定 TCP 发送请求被挂起或拒绝之前 TCP 传输队列的最大深度。 默认值为 20，这表示在任意给定时间，传输队列中最多可以有 20 个数据包。 请注意，数据包将保留在传输队列中，
//直到从连接的另一端接收到涵盖部分或全部数据包数据的 ACK。 此常量在 nx_tcp.h 中定义。 应用程序可以在包含 nx_api.h 之前定义该值，以替代默认值。
#define NX_TCP_MAXIMUM_TX_QUEUE                                 20

//指定重试之间的重新传输超时期限如何变化。 如果此值为 0，则初始重新传输超时与后续重新传输超时相同。 如果此值为 1，则每个连续重新传输时长为前一个的 2 倍。 如果此值为 2，
//则每个后续的重新传输超时时长为前一个的 4 倍。 默认值为 0，在 nx_tcp.h 中定义。 应用程序可以在包含 nx_api.h 之前定义该值，以替代默认值。
#define NX_TCP_RETRY_SHIFT                                      0

//指定在连接被视为中断之前允许 KeepAlive 重试的次数。 默认值为 10（表示 10 次重试），在 nx_tcp.h 中定义。 应用程序可以在包含 nx_api.h 之前定义该值，以替代默认值。
#define NX_TCP_KEEPALIVE_RETRIES                                10

//启用 TCP 应用程序的窗口缩放选项。 定义后，在 TCP 连接阶段会协商窗口缩放选项，且应用程序可以指定大于 64 K 的窗口大小。 未启用默认设置（未定义）。
//#define NX_ENABLE_TCP_WINDOW_SCALING

//定义后，当提供的超时值指定为 NX_NO_WAIT 时，将禁止在断开连接期间进行重置处理。
//#define NX_DISABLE_RESET_DISCONNECT

//定义后，可以在接受 TCP 连接之前验证对等 MSS 的最小值。 若要使用此功能，必须定义符号 NX_ENABLE_TCP_MSS_MINIMUM 默认情况下不启用此选项。
//#define NX_ENABLE_TCP_MSS_CHECK

//	定义 NetX Duo TCP 模块接受的最小 MSS 值的符号。 此功能由 NX_ENABLE_TCP_MSS_CHECK 启用。
//#define NX_TCP_MSS_MINIMUM                                    128

//定义后，允许应用程序安装一个回调函数，当 TCP 传输队列深度不再为最大值时，将调用该函数。 
//此回调可指示 TCP 套接字已准备好传输更多数据。 默认情况下，未启用此选项。
//#define NX_ENABLE_TCP_QUEUE_DEPTH_UPDATE_NOTIFY

//定义后，将启用 NetX Duo 数据包池低水位线功能。 应用程序设置低水位线值。 在收到 TCP 数据包时，
//如果达到了数据包池低水位线，NetX Duo 将通过释放数据包以静默方式丢弃数据包，防止数据包池耗尽。 默认情况下，未启用此功能。
//#define NX_ENABLE_LOW_WATERMARK

//用于定义 TCP 套接字最大接收队列的符号。 此功能由 NX_ENABLE_LOW_WATERMARK 启用。
#ifdef NX_ENABLE_LOW_WATERMARK
#define NX_TCP_MAXIMUM_RX_QUEUE                                 20
#endif

/*--------------------------------------------------IP分片协议配置-----------------------------------------------------*/
//定义后，将同时禁用 IPv4 和 IPv6 拆分和重组逻辑。
//#define NX_DISABLE_FRAGMENTATION

//定义后，将允许在接收 IP 片段后立即执行 IPv4 和 IPv6 重组逻辑。 默认情况下，未定义此选项。
//#define NX_FRAGMENT_IMMEDIATE_ASSEMBLY

//用于控制允许重组 IPv4 片段和 IPv6 片段的最长时间的符号。 默认值60
//请注意，此处定义的值将覆盖 NX_IPV4_MAX_REASSEMBLY_TIME 和 NX_IPV6_MAX_REASSEMBLY_TIME。
//#define NX_IP_MAX_REASSEMBLY_TIME                             60

//用于控制允许重组 IPv4 片段的最长时间的符号。 请注意，在 NX_IP_MAX_REASSEMBLY_TIME 中定义的值会覆盖此值。
//#define NX_IPV4_MAX_REASSEMBLY_TIME                           15

//控制允许重组 IPv6 片段的最长时间的符号。
//#define NX_IPV6_MAX_REASSEMBLY_TIME                           60

/*--------------------------------------------------通讯数据校验配置-----------------------------------------------------*/
//定义后，将禁用接收到的 ICMP 数据包上的 ICMPv4 校验和计算。 
//如果定义了 NX_DISABLE_ICMP_RX_CHECKSUM，将自动定义此选项。 默认情况下，未定义此选项。
//#define NX_DISABLE_ICMPV4_RX_CHECKSUM

//定义后，将禁用接收到的 ICMP 数据包上的 ICMPv6 校验和计算。
//已重命名为 NX_DISABLE_ICMPV6_RX_CHECKSUM。 尽管它仍受支持，但建议使用新设计以使用 NX_DISABLE_CMPV6_RX_CHECKSUM。
//#define NX_DISABLE_ICMPV6_RX_CHECKSUM

//定义后，将在收到的 ICMP 数据包上禁用 ICMPv4 和 ICMPv6 校验和计算。 当网络接口驱动程序能够验证 ICMPv4 和 ICMPv6 校验和，
//且应用程序不使用 IP 拆分功能或 IPsec 功能时，此选项很有用。 默认情况下，未定义此选项。
//#define NX_DISABLE_ICMP_RX_CHECKSUM

//定义后，将禁用传输的 ICMP 数据包上的 ICMPv4 校验和计算。 
//如果定义了 NX_DISABLE_ICMP_TX_CHECKSUM，将自动定义此选项。 默认情况下，未定义此选项。
//#define NX_DISABLE_ICMPV4_TX_CHECKSUM

//定义后，将禁用传输的 ICMP 数据包上的 ICMPv6 校验和计算。
//#define NX_DISABLE_ICMPV6_TX_CHECKSUM

//定义后，将在传输的 ICMP 数据包上禁用 ICMPv4 和 ICMPv6 校验和计算。 当网络接口驱动程序能够计算 ICMPv4 和 ICMPv6 校验和，
//且应用程序不使用 IP 拆分功能或 IPsec 功能，则此选项很有用。 默认情况下，未定义此选项。
//#define NX_DISABLE_ICMP_TX_CHECKSUM

//定义后，将禁用接收到的 IPv4 数据包上的校验和逻辑。 当网络设备能够验证 IPv4 校验和，且应用程序不应使用 IP 拆分或 IPsec 时，此选项很有用。
//#define NX_DISABLE_IP_RX_CHECKSUM

//定义后，将禁用发送的 IPv4 数据包上的校验和逻辑。 当基础网络设备能够生成 IPv4 标头校验和，且应用程序不应使用 IP 拆分或 IPsec 时，此选项很有用。
//#define NX_DISABLE_IP_TX_CHECKSUM

//定义后，将禁用接收到的 TCP 数据包上的校验和逻辑。 仅当链路层具有可靠的校验和或 CRC 处理，
//或者接口驱动程序能够验证硬件中的 TCP 校验和，且应用程序不使用 IPsec 时，此选项才很有用。
//#define NX_DISABLE_TCP_RX_CHECKSUM

//定义后，将禁用用于发送 TCP 数据包的校验和逻辑。 仅当接收网络节点接收到禁用了 TCP 校验和逻辑，
//或者基础网络驱动程序能够生成 TCP 校验和，且应用程序不使用 IPsec 时，此选项才很有用。
//#define NX_DISABLE_TCP_TX_CHECKSUM

//定义后，将禁用传入 UDP 数据包上的 UDP 校验和计算。 当网络接口驱动程序能够验证硬件中的 UDP 标头校验和，且应用程序不启用 IPsec 或 IP 拆分逻辑时，此选项很有用。
//#define NX_DISABLE_UDP_RX_CHECKSUM

//M	定义后，将禁用传出 UDP 数据包上的 UDP 校验和计算。 当网络接口驱动程序能够计算 UDP 标头校验和，
//并在传输数据之前将值插入 IP 标头，且应用程序不启用 IPsec 或 IP 拆分逻辑时，此选项很有用。
//#define NX_DISABLE_UDP_TX_CHECKSUM


/*--------------------------------------------------通讯讯息收集配置-----------------------------------------------------*/
//定义后，此选项用于禁用 ARP 信息收集。
//#define NX_DISABLE_ARP_INFO

//定义后，此选项用于禁用 IP 信息收集。
//#define NX_DISABLE_IP_INFO

//定义后，此选项用于禁用 ICMP 信息收集。
//#define NX_DISABLE_ICMP_INFO

//定义后，此选项用于禁用 IGMP 信息收集。
//#define NX_DISABLE_IGMP_INFO

//定义后，将禁用数据包池信息收集。
//#define NX_DISABLE_PACKET_INFO

//定义后，此选项用于禁用 RARP 信息收集。
//#define NX_DISABLE_RARP_INFO

//定义后，此选项用于禁用 TCP 信息收集。
//#define NX_DISABLE_TCP_INFO

//定义后，此选项用于禁用 UDP 信息收集。
//#define NX_DISABLE_UDP_INFO


/*--------------------------------------------------数据包内存池配置-----------------------------------------------------*/
//指定帧的物理标头的大小（以字节为单位）。 默认值为 16（基于与 32 位边界对齐的典型 14 字节以太网帧），
//在 nx_api.h 中定义。 应用程序可以在包含 nx_api.h（例如 nx_user.h）之前定义该值，以替代默认值。
#define NX_PHYSICAL_HEADER                                      16

//指定物理数据包包尾的大小（以字节为单位），通常用于为以太网 CRC 等项保留存储。默认值为 4，在 nx_api.h 中定义。
#define NX_PHYSICAL_TRAILER                                     4

//定义后，将禁用对接收到的数据包的大小检查。
//#define NX_DISABLE_RX_SIZE_CHECKING

//定义后，用于记录数据包调试信息。
//#define NX_ENABLE_PACKET_DEBUG_INFO

//定义后，可以在 NX_PACKET 控制块的末尾处进行填充。 要填充的 ULONG 字词的数量由 NX_PACKET_HEADER_PAD_SIZE 定义。 
//请注意，此选项已被 NX_PACKET_ALIGNMENT 弃用
//#define NX_PACKET_HEADER_PAD

//设置要填充到 NX_PACKET 结构中的 ULONG 字词的数量，允许数据包有效负载区从所需的对齐位置开始。 当缓冲区描述符点直接传入 NX_PACKET 有效负载区，且网络接口接收逻辑或缓存操作逻辑要求缓冲区起始地址满足特定的对齐要求时，此功能很有用。 
//仅当定义了 NX_PACKET_HEADER_PAD 时，此值才有效。 请注意，此选项已被 NX_PACKET_ALIGNMENT 弃用。
//#define NX_PACKET_HEADER_PAD_SIZE                             1

//定义，指定数据包有效负载区的起始地址的对齐需求（以字节为单位）。 
//此选项弃用了 NX_PACKET_HEADER_PAD 和 NX_PACKET_HEADER_PAD_SIZE。 默认情况下，此选项定义为 4，使有效负载区的起始地址与 4 字节对齐。
#define NX_PACKET_ALIGNMENT     sizeof(ULONG)

//定义后，将禁用数据包链逻辑。 默认情况下，未定义此选项。
//#define NX_DISABLE_PACKET_CHAIN

//定义后，将允许堆栈使用两个数据包池，一个具有较大的负载大小，一个具有较小的负载大小。 默认情况下，未启用此选项。
//#define NX_ENABLE_DUAL_PACKET_POOL

/* Configuration options for Others */

//定义后，将删除基本 NetX Duo 错误检查 API 并提高性能。 不受禁用错误检查影响的 API 返回代码将在 API 定义中以粗体字样列出。 
//此定义通常在应用程序得到充分调试之后使用，使用它可提高性能并减少代码大小。
//#define NX_DISABLE_ERROR_CHECKING

//定义后，将启用延迟网络驱动程序数据包处理。 其允许网络驱动程序将数据包放置在 IP 实例上，
//并从 NetX Duo 内部 IP 帮助程序线程中调用真实处理例程。
#define NX_DRIVER_DEFERRED_PROCESSING

//定义后，此选项允许检查传入数据包的源地址。 默认已禁用此选项。
//#define NX_ENABLE_SOURCE_ADDRESS_CHECK

//定义后，将允许堆栈中有更多回调挂钩。 BSD 包装层使用这些回调函数。 默认情况下，未定义此选项。
//#define NX_ENABLE_EXTENDED_NOTIFY_SUPPORT

//定义后，将禁用源代码中的 ASSERT 检查。 默认情况下，未定义此选项。
//#define NX_DISABLE_ASSERT

//定义断言失败时要使用的 debug 语句的符号。
//#define NX_ASSERT_FAIL          while (1) tx_thread_sleep(NX_WAIT_FOREVER);

//定义后，将禁用 IPv4 功能。 此选项可用于将 NetX Duo 构建为仅支持 IPv6。 默认情况下，未定义此选项。
//#define NX_DISABLE_IPV4

//定义后，将检查 ICMP 数据包的目标地址。默认为禁用发往 IP 广播或 IP 多播地址的 ICMP 回显请求将以静默方式丢弃。
//#define NX_ENABLE_ICMP_ADDRESS_CHECK

//最大字符串长度,默认1024
#define NX_MAX_STRING_LENGTH                                    1024

//启用 TCP/IP 卸载功能的符号。 请注意，必须定义 NX_ENABLE_INTERFACE_CAPABILITY 才能启用此功能。
//#define NX_ENABLE_TCPIP_OFFLOAD

#endif

