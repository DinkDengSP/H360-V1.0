/***********************************************************************
 *
 * (c) Copyright 2001-2016, XXXX, All Rights Reserved.
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF XXXX, INC.
 * The copyright notice above does not evidence any actual or intended
 * publication of such source code. 
 *
 *  Subsystem:   AppUtil
 *  File:        AppUtil_CRC.h
 *  Author:      WWZHOU
 *  Description: Double List for C header files.
 *********************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APPUTIL_CRC_H__
#define __APPUTIL_CRC_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

extern uint16_t AppUtil_calcCRC16(const uint8_t *pu8Data, uint32_t u32Size);
extern uint32_t AppUtil_calcCRC32(const uint8_t *pu8Data, uint32_t u32Size);

#ifdef __cplusplus
}
#endif

#endif /* __APPUTIL_CRC_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
