/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2020-08-20 17:57:00
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-01-29 21:16:49
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef	_SOCKET_H_
#define	_SOCKET_H_
#include "Types.h"

/*Socket 端口选择，可按自己的习惯定义*/
#define SOCK_TCPS_MAIN             0
#define SOCK_TCPS_DEBUG            1

extern uint8 socket(SOCKET s, uint8 protocol, uint16 port, uint8 flag);                      // Opens a socket(TCP or UDP or IP_RAW mode)
extern void close(SOCKET s);                                                                 // Close socket
extern uint8 connect(SOCKET s, uint8 * addr, uint16 port); 																	 // Establish TCP connection (Active connection)
extern void disconnect(SOCKET s);                                                            // disconnect the connection
extern uint8 listen(SOCKET s);	                                                             // Establish TCP connection (Passive connection)
extern uint16 send(SOCKET s, const uint8 * buf, uint16 len);                                 // Send data (TCP)
extern uint16 recv(SOCKET s, uint8 * buf, uint16 len);	                                     // Receive data (TCP)
extern uint16 sendto(SOCKET s, const uint8 * buf, uint16 len, uint8 * addr, uint16 port);    // Send data (UDP/IP RAW)
extern uint16 recvfrom(SOCKET s, uint8 * buf, uint16 len, uint8 * addr, uint16  *port);      // Receive data (UDP/IP RAW)

#ifdef __MACRAW__
void macraw_open(void);
uint16 macraw_send( const uint8 * buf, uint16 len );                                         //Send data (MACRAW)
uint16 macraw_recv( uint8 * buf, uint16 len );                                               //Recv data (MACRAW)
#endif

#endif
/* _SOCKET_H_ */
