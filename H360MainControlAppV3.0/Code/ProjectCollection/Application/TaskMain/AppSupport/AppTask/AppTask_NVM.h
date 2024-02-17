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
#ifndef __APPTASK_NVM_H__
#define __APPTASK_NVM_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "MCU_Flash.h"

typedef enum
{
    NVM_TYPE_NEW_APP = 0,
    NVM_TYPE_APP_CONFIG,
    NVM_TYPE_ERROR
}NVM_TYPE_E;

typedef struct
{
    uint8_t  u8Body[120];
    uint16_t u16BodySize;
    uint16_t u16RecvCrc;
    uint8_t  u8Cmd;
    uint8_t  u8Invalid1;
    uint8_t  u8Invalid2;
    uint8_t  u8Invalid3;
}NVM_COM_CMD_T;


extern int32_t NVM_LoadConfig(void);
extern int32_t NVM_SaveConfig(NVM_TYPE_E eType);

#ifdef __cplusplus
}
#endif

#endif /* __APPTASK_NVM_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
