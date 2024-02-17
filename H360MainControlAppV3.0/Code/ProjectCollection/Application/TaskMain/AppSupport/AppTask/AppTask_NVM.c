/******************************************************************
 * (c) Copyright 2018-2025, XXXX, All Rights Reserved.
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF XXXX, INC.
 * The copyright notice above does not evidence any actual or intended
 * publication of such source code. 
 *
 *  Subsystem:   AppTask
 *  File:        AppTask_NVM.c
 *  Author:      WWZHOU
 *  Description: 
 *
 *  Function List:  
 *     1. ....
 *  History:        
 *                  
 *     1. Date:
 *        Author:
 *        Modification:
 *     2. ...
 *  
 ***************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "AppTask_NVM.h"
#include "AppUtil_CRC.h"
#include "TestFlow_Typedef.h"

typedef struct
{
    uint32_t u32NewFlashFlag;
    uint8_t  u8CellNumber;  // 0~3
    uint8_t  u8ReportTime;
    uint8_t  u8CellType;
    uint8_t  u8BoardType;   // 电路板类型
    uint8_t  u8Invalid[56];
}NVM_CELL_T;

static NVM_CELL_T g_stCellConfig;

/****************************************************************************
 *
 * Function Name:    NVM_LoadConfig
 * Input:            None
 *                   
 * Output:           None
 *                   
 * Returns:          
 * Description:      
 * Note:             在CAN的初始化前被调用
 ***************************************************************************/
int32_t NVM_LoadConfig(void)
{
    uint32_t u32ReadCRC, u32CalcCRC;

    memcpy(&g_stCellConfig, (uint8_t *)FLASH_APP_CONFIG_ADDR, sizeof(NVM_CELL_T));
    
    if (FLASH_NEW_FLAG != g_stCellConfig.u32NewFlashFlag)
    {
        //MCU_Uart1Printf("##New Board##\r\n");
        memset(&g_stCellConfig, 0, sizeof(NVM_CELL_T));
        g_stCellConfig.u32NewFlashFlag = FLASH_NEW_FLAG;
        g_stCellConfig.u8CellNumber = 0; // Cell 1
        g_stCellConfig.u8ReportTime = 5;
        g_stCellConfig.u8CellType = TESTFLOW_CELL_TYPE_CI;
        g_stCellConfig.u8BoardType = 2;  // H200 MainBoard, STM32F407IGTx + 1MB SRAM + 2MB FRAM
        NVM_SaveConfig(NVM_TYPE_APP_CONFIG);
        //MCU_Uart1Printf("InitConfig:%d,%d,%d\r\n", g_stCellConfig.u8CellNumber, g_stCellConfig.u8ReportTime, g_stCellConfig.u8CellType);
    }
    
    u32CalcCRC = AppUtil_calcCRC32((uint8_t *)FLASH_APP_CONFIG_ADDR, FLASH_BOOT_CONFIG_SIZE-4);
    u32ReadCRC = BYTE_TO_WORD(*(uint8_t *)(FLASH_APP_CONFIG_ADDR+FLASH_APP_CONFIG_SIZE-1),
                              *(uint8_t *)(FLASH_APP_CONFIG_ADDR+FLASH_APP_CONFIG_SIZE-2),
                              *(uint8_t *)(FLASH_APP_CONFIG_ADDR+FLASH_APP_CONFIG_SIZE-3),
                              *(uint8_t *)(FLASH_APP_CONFIG_ADDR+FLASH_APP_CONFIG_SIZE-4));
    
    if (u32ReadCRC != u32CalcCRC)
    {
        //MCU_Uart1Printf("**ERR>LoadCellConfig_CRC:%04X,%04X\r\n", u32ReadCRC, u32CalcCRC);
        return -1;
    }
    else
    {
        //MCU_Uart1Printf("CellConfig:%d,%d,%d\r\n", g_stCellConfig.u8CellNumber, g_stCellConfig.u8ReportTime, g_stCellConfig.u8CellType);
        return 0;
    }
}

int32_t NVM_SaveConfig(NVM_TYPE_E eType)
{
    uint8_t  *pu8BootConfigBuff;
    uint8_t  *pu8AppConfigBuff;
    int s32Result = 0;
    uint32_t crc32;
    int i;

    pu8BootConfigBuff = UserMemMalloc(MEM_SRAM,FLASH_BOOT_CONFIG_SIZE);
    if (NULL == pu8BootConfigBuff)
    {
        UserMemFree(MEM_SRAM, pu8BootConfigBuff);
        return -1;
    }
    pu8AppConfigBuff = UserMemMalloc(MEM_SRAM,FLASH_BOOT_CONFIG_SIZE);
    if (NULL == pu8AppConfigBuff)
    {
        UserMemFree(MEM_SRAM, pu8BootConfigBuff);
        UserMemFree(MEM_SRAM, pu8AppConfigBuff);
        return -2;
    }
    //Boot
    for (i=0; i<FLASH_BOOT_CONFIG_SIZE; i++)
    {
        pu8BootConfigBuff[i] = *(uint8_t *)(FLASH_BOOT_CONFIG_ADDR+i);
    }
    //App
    for (i=0; i<FLASH_APP_CONFIG_SIZE; i++)
    {
        pu8AppConfigBuff[i] = *(uint8_t *)(FLASH_APP_CONFIG_ADDR+i);
    }
    switch(eType)
    {
        case NVM_TYPE_NEW_APP:
            *(uint32_t *)&pu8BootConfigBuff[FLASH_BOOT_CONFIG_NEW_APP_ADDR-FLASH_BOOT_CONFIG_ADDR] = ~FLASH_PROGRAM_NEW_FUNC;
            break;
        case NVM_TYPE_APP_CONFIG:
            memcpy(pu8AppConfigBuff, (uint8_t *)&g_stCellConfig, sizeof(NVM_CELL_T));
            break;
        default:
            s32Result = -3;
            break;
    }
    if (0 == s32Result)
    {
        MCU_FlashErase(FLASH_BOOT_CONFIG_ADDR, 1);
        
        crc32 = AppUtil_calcCRC32(pu8BootConfigBuff, FLASH_BOOT_CONFIG_SIZE-4);
        pu8BootConfigBuff[FLASH_BOOT_CONFIG_SIZE-4] = WORD_TO_BYTE_LL(crc32);
        pu8BootConfigBuff[FLASH_BOOT_CONFIG_SIZE-3] = WORD_TO_BYTE_LH(crc32);
        pu8BootConfigBuff[FLASH_BOOT_CONFIG_SIZE-2] = WORD_TO_BYTE_HL(crc32);
        pu8BootConfigBuff[FLASH_BOOT_CONFIG_SIZE-1] = WORD_TO_BYTE_HH(crc32);
        MCU_FlashProgram(FLASH_BOOT_CONFIG_ADDR, pu8BootConfigBuff, FLASH_BOOT_CONFIG_SIZE);
        
        crc32 = AppUtil_calcCRC32(pu8AppConfigBuff, FLASH_APP_CONFIG_SIZE-4);
        pu8AppConfigBuff[FLASH_APP_CONFIG_SIZE-4] = WORD_TO_BYTE_LL(crc32);
        pu8AppConfigBuff[FLASH_APP_CONFIG_SIZE-3] = WORD_TO_BYTE_LH(crc32);
        pu8AppConfigBuff[FLASH_APP_CONFIG_SIZE-2] = WORD_TO_BYTE_HL(crc32);
        pu8AppConfigBuff[FLASH_APP_CONFIG_SIZE-1] = WORD_TO_BYTE_HH(crc32);
        MCU_FlashProgram(FLASH_APP_CONFIG_ADDR, pu8AppConfigBuff, FLASH_APP_CONFIG_SIZE);
    }
    UserMemFree(MEM_SRAM, pu8BootConfigBuff);
    UserMemFree(MEM_SRAM, pu8AppConfigBuff);

    return s32Result;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
