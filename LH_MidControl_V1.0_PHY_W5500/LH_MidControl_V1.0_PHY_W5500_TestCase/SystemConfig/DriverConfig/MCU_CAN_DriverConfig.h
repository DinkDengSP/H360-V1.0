/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2022-11-11 15:47:45 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2022-11-24 19:57:26 +0800
************************************************************************************************/ 
#ifndef __MCU_CAN_DRIVER_CONFIG_H_
#define __MCU_CAN_DRIVER_CONFIG_H_
#include "MCU_ClockDriverConfig.h"

/*----------------------------------------------芯片CAN--------------------------------------*/
//CAN模式
#define MCU_CAN_MODE_CLASSICAL                  0//经典模式
#define MCU_CAN_MODE_FD_NO_BRS                  1//FD CAN不带可变波特率
#define MCU_CAN_MODE_FD_BRS                     2//FD CAN带可变波特率

//CAN总线数据包最大长度
#define MCU_CAN_DATA_LEN_MAX_CLASSICAL          8
#define MCU_CAN_DATA_LEN_MAX_FD                 64

//定义CAN通讯模块过滤器最大数量,默认设置一个过滤器2个32位字
#define MCU_CAN_FILTER_NBR_MAX                  64
//定义CAN通讯模块收发FIFO总数最大值,一个64字节CAN包18个字,与过滤器加起来一共占了1280个32位字
#define MCU_CAN_FIFO_NBR_MAX                    64

//根据系统时钟定义CAN模块的时钟源
#if(CORE_CLOCK_MODE_USER == CORE_CLOCK_MODE_480M)
#define MCU_CAN_SRC_CLK_HZ                      120000000
#elif(CORE_CLOCK_MODE_USER == CORE_CLOCK_MODE_400M)
#define MCU_CAN_SRC_CLK_HZ                      100000000
#endif

/*----------------------------------------芯片CAN1配置讯息--------------------------------------*/
//设置CAN1模式
#define MCU_CAN1_MODE_USER                      MCU_CAN_MODE_CLASSICAL
//是否显示中断回调讯息
#define MCU_CAN1_ENANLE_SHOW_INT_MSG            0
//等待CAN FIFO最长时间,数据发包的时候如果超时还没有缓冲区就认为是异常
#define MCU_CAN1_WAIT_FIFO_SINGLE_PACK_MS       200
//自动重发配置
#define MCU_CAN1_AUTO_RETRANS                   ENABLE
//传输暂停模式,如果使能则FDCAN 在下一次开始之前和成功发送帧之间，暂停两个Tq发送暂停功能用于 CAN 消息标识
//符（永久性地）被指定为特定值、并且无法轻易更改的CAN 系统中。这些消息标识符的 CAN 仲裁优先级可能高于其他
//已定义的消息，而在特定的应用中，其相对仲裁优先级应反转。这样可能会导致如下情况：一个 ECU 发送的 CAN 消息    
//突发造成 CAN 仲裁优先级较低的另一个 ECU CAN 消息被延迟,此功能可使来自单个节点的突发发送变得松散一些，避免
//出现应用程序错误地请求过多传输的“胡乱指示”情况
#define MCU_CAN1_TRANS_PAUSE                    DISABLE
//协议异常处理模式,FD CAN当使能,发生异常的时候协议进入同步中
#define MCU_CAN1_PROTOCOL_EXCEPTION             ENABLE

//是否需要配置11位标准帧过滤器
#define MCU_CAN1_STD_FILTER_NBR_EXIST           0
//是否需要配置29位扩展帧过滤器
#define MCU_CAN1_EXT_FILTER_NBR_EXIST           1

//CAN口收发缓冲区的长度以及消息队列的长度
#define MCU_CAN1_MSG_FIFO_LENGTH                128
//发送消息的FIFO长度
#define MCU_CAN1_TRANS_FIFO_NBR_COUNT           16
//接收消息的FIFO长度
#define MCU_CAN1_RECV_FIFO_NBR_MAX              (MCU_CAN_FIFO_NBR_MAX - MCU_CAN1_TRANS_FIFO_NBR_COUNT)
//CAN 接收FIFO0的长度
#define MCU_CAN1_RECV_FIFO_0_NBR_COUNT          24
//CAN 接收FIFO1的长度
#define MCU_CAN1_RECV_FIFO_1_NBR_COUNT          (MCU_CAN1_RECV_FIFO_NBR_MAX - MCU_CAN1_RECV_FIFO_0_NBR_COUNT)

//CAN控制器发送信号时，是经过收发器后发往CAN总线后，再经过收发器反馈总线信号。那么发送过程中，控制器发送位信号到接收位信号就不可避免地存在环路延迟
//CAN协议中规定：发送方发送位时，需检测接收到的位与发送是否一致，若不一致则产生错误帧（位错误）。如果发送延迟过长，则将直接导致发送与接收位不一致而产生错误帧
//由于传统CAN协议规定最高波特率为1Mbps，即位宽1us，正常情况下，传输延迟不会超过位宽的采样点（当然具体延迟取决于收发器环路延迟、传输距离、传输线缆质量等），
//因此不会因为发送延迟而产生错误
//TDC实际上就是在发送BRS位为隐性的CANFD报文时（BRS隐性即开启数据域波特率），在发送时延迟一定时间后，在第二采样点采样接收位，以正确采样到发送位对应的接收位
//那么延迟采样的延迟时间是多久呢？实际上，开启TDC后，控制器将自动测量Tx信号线上FDF位到r0位下降沿与Rx信号线上FDF位到r0位边沿的之间的延迟时间
//发送延迟测量的时间单位为CAN控制器时钟
//当CANFD数据域波特率大于1Mbps时，应开启TDC；数据段波特率的预分频值（DBRP）建议设置为1或2
//ISO11898-1:2015中还对TDC做了一些规定：开启TDC后，数据段波特率的预分频值（DBRP）应设置为1或2；控制器实现TDC机制应能补偿至少两个位时间
//TDCF 发送器延迟补偿过滤器时间,通过引脚 FDCAN_TX 进行发送时，协议控制器会通过引脚 FDCAN_RX 接    
//收到其本地 CAN 收发器发送的数据。接收数据的延迟为 CAN 收发器环路延迟。如果该延迟    
//大于 TSEG1（采样点之前的时间段），则会检测到位错误。如果不进行收发器延迟补偿， 
//FDCAN 帧数据阶段的比特率会受到收发器环路延迟的限制(为了适配长总线网络而设置的延迟)
#define MCU_CAN1_TDC_FILTER                     2


//发送FIFO为0,不允许
#if(MCU_CAN1_TRANS_FIFO_NBR_COUNT == 0)
#error "MCU CAN1 Trans Fifo Length Limit,Not Equal Zero!!!"
#endif
//发送FIFO大于了总FIFO长度,没有地方放接收FIFO了
#if(MCU_CAN1_TRANS_FIFO_NBR_COUNT >= MCU_CAN_FIFO_NBR_MAX)
#error "MCU CAN1 Trans Fifo Length Max,No Space For Recv!!!"
#endif
//接收FIFO0比总的接收FIFO还长,没有那么多空间放
#if(MCU_CAN1_RECV_FIFO_0_NBR_COUNT > MCU_CAN1_RECV_FIFO_NBR_MAX)
#error "MCU CAN1 Recv Fifo1 Max Then MCU_CAN1_RECV_FIFO_NBR_MAX,No Enough Space!!!"
#endif
//至少配置一个过滤器
#if((MCU_CAN1_STD_FILTER_NBR_EXIST == 0)&&(MCU_CAN1_EXT_FILTER_NBR_EXIST == 0))
#error "MCU CAN1 Must Config One Filter"
#endif


/*----------------------------------------芯片CAN2配置讯息--------------------------------------*/
//设置CAN2模式
#define MCU_CAN2_MODE_USER                      MCU_CAN_MODE_CLASSICAL
//是否显示中断回调讯息
#define MCU_CAN2_ENANLE_SHOW_INT_MSG            0
//等待CAN FIFO最长时间,数据发包的时候如果超时还没有缓冲区就认为是异常
#define MCU_CAN2_WAIT_FIFO_SINGLE_PACK_MS       200
//自动重发配置
#define MCU_CAN2_AUTO_RETRANS                   ENABLE
//传输暂停模式,如果使能则FDCAN 在下一次开始之前和成功发送帧之间，暂停两个Tq发送暂停功能用于 CAN 消息标识
//符（永久性地）被指定为特定值、并且无法轻易更改的CAN 系统中。这些消息标识符的 CAN 仲裁优先级可能高于其他
//已定义的消息，而在特定的应用中，其相对仲裁优先级应反转。这样可能会导致如下情况：一个 ECU 发送的 CAN 消息    
//突发造成 CAN 仲裁优先级较低的另一个 ECU CAN 消息被延迟,此功能可使来自单个节点的突发发送变得松散一些，避免
//出现应用程序错误地请求过多传输的“胡乱指示”情况
#define MCU_CAN2_TRANS_PAUSE                    DISABLE
//协议异常处理模式,FD CAN当使能,发生异常的时候协议进入同步中
#define MCU_CAN2_PROTOCOL_EXCEPTION             ENABLE

//是否需要配置11位标准帧过滤器
#define MCU_CAN2_STD_FILTER_NBR_EXIST           0
//是否需要配置29位扩展帧过滤器
#define MCU_CAN2_EXT_FILTER_NBR_EXIST           1

//CAN口收发缓冲区的长度以及消息队列的长度
#define MCU_CAN2_MSG_FIFO_LENGTH                128
//发送消息的FIFO长度
#define MCU_CAN2_TRANS_FIFO_NBR_COUNT           16
//接收消息的FIFO长度
#define MCU_CAN2_RECV_FIFO_NBR_MAX              (MCU_CAN_FIFO_NBR_MAX - MCU_CAN2_TRANS_FIFO_NBR_COUNT)
//CAN 接收FIFO0的长度
#define MCU_CAN2_RECV_FIFO_0_NBR_COUNT          24
//CAN 接收FIFO1的长度
#define MCU_CAN2_RECV_FIFO_1_NBR_COUNT          (MCU_CAN2_RECV_FIFO_NBR_MAX - MCU_CAN2_RECV_FIFO_0_NBR_COUNT)

//CAN控制器发送信号时，是经过收发器后发往CAN总线后，再经过收发器反馈总线信号。那么发送过程中，控制器发送位信号到接收位信号就不可避免地存在环路延迟
//CAN协议中规定：发送方发送位时，需检测接收到的位与发送是否一致，若不一致则产生错误帧（位错误）。如果发送延迟过长，则将直接导致发送与接收位不一致而产生错误帧
//由于传统CAN协议规定最高波特率为1Mbps，即位宽1us，正常情况下，传输延迟不会超过位宽的采样点（当然具体延迟取决于收发器环路延迟、传输距离、传输线缆质量等），
//因此不会因为发送延迟而产生错误
//TDC实际上就是在发送BRS位为隐性的CANFD报文时（BRS隐性即开启数据域波特率），在发送时延迟一定时间后，在第二采样点采样接收位，以正确采样到发送位对应的接收位
//那么延迟采样的延迟时间是多久呢？实际上，开启TDC后，控制器将自动测量Tx信号线上FDF位到r0位下降沿与Rx信号线上FDF位到r0位边沿的之间的延迟时间
//发送延迟测量的时间单位为CAN控制器时钟
//当CANFD数据域波特率大于1Mbps时，应开启TDC；数据段波特率的预分频值（DBRP）建议设置为1或2
//ISO11898-1:2015中还对TDC做了一些规定：开启TDC后，数据段波特率的预分频值（DBRP）应设置为1或2；控制器实现TDC机制应能补偿至少两个位时间
//TDCF 发送器延迟补偿过滤器时间,通过引脚 FDCAN_TX 进行发送时，协议控制器会通过引脚 FDCAN_RX 接    
//收到其本地 CAN 收发器发送的数据。接收数据的延迟为 CAN 收发器环路延迟。如果该延迟    
//大于 TSEG1（采样点之前的时间段），则会检测到位错误。如果不进行收发器延迟补偿， 
//FDCAN 帧数据阶段的比特率会受到收发器环路延迟的限制(为了适配长总线网络而设置的延迟)
#define MCU_CAN2_TDC_FILTER                     2

//发送FIFO为0,不允许
#if(MCU_CAN2_TRANS_FIFO_NBR_COUNT == 0)
#error "MCU CAN2 Trans Fifo Length Limit,Not Equal Zero!!!"
#endif
//发送FIFO大于了总FIFO长度,没有地方放接收FIFO了
#if(MCU_CAN2_TRANS_FIFO_NBR_COUNT >= MCU_CAN_FIFO_NBR_MAX)
#error "MCU CAN2 Trans Fifo Length Max,No Space For Recv!!!"
#endif
//接收FIFO0比总的接收FIFO还长,没有那么多空间放
#if(MCU_CAN2_RECV_FIFO_0_NBR_COUNT > MCU_CAN2_RECV_FIFO_NBR_MAX)
#error "MCU CAN2 Recv Fifo1 Max Then MCU_CAN2_RECV_FIFO_NBR_MAX,No Enough Space!!!"
#endif
//至少配置一个过滤器
#if((MCU_CAN2_STD_FILTER_NBR_EXIST == 0)&&(MCU_CAN2_EXT_FILTER_NBR_EXIST == 0))
#error "MCU CAN2 Must Config One Filter"
#endif










#endif




