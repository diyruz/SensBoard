/******************************************************************************

 @file rom_jt_def_154.h

 @brief ROM jump table definition

 Group: WCS LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2016-2020, Texas Instruments Incorporated
 All rights reserved.

 IMPORTANT: Your use of this Software is limited to those specific rights
 granted under the terms of a software license agreement between the user
 who downloaded the software, his/her employer (which must be your employer)
 and Texas Instruments Incorporated (the "License"). You may not use this
 Software unless you agree to abide by the terms of the License. The License
 limits your use, and you acknowledge, that the Software may not be modified,
 copied or distributed unless embedded on a Texas Instruments microcontroller
 or used solely and exclusively in conjunction with a Texas Instruments radio
 frequency transceiver, which is integrated into your product. Other than for
 the foregoing purpose, you may not use, reproduce, copy, prepare derivative
 works of, modify, distribute, perform, display or sell this Software and/or
 its documentation for any purpose.

 YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
 PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
 NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
 TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
 NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
 LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
 INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
 OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
 OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
 (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

 Should you have any questions regarding your right to use this Software,
 contact Texas Instruments Incorporated at www.TI.com.

 ******************************************************************************
 
 
 *****************************************************************************/

#ifndef ROM_JT_DEF_H
#define ROM_JT_DEF_H

#include "hal_types.h"

/*******************************************************************************
 * EXTERNS
 */

// Common ROM's RAM table for pointers to ICall functions and flash jump tables.
// Note: This linker imported symbol is treated as a variable by the compiler.
// 0: iCall/OSCAL Dispatch Function Pointer
// 1: FH ROM Flash jump table
// 2: HMAC ROM Flash Jump table
// 3: LMAC ROM Flash Jump table 
// 

// ROM's RAM table offset to ROM Flash Jump Table pointer.
#define ROM_RAM_ICALL_OSAL_TABLE_INDEX                  0
#define ROM_RAM_FH_TABLE_INDEX                          1
#define ROM_RAM_HMAC_TABLE_INDEX                        2
#define ROM_RAM_LMAC_TABLE_INDEX                        3

#define ROM_RAM_TABLE_SIZE                              4

extern uint32_t RAM_MAC_BASE_ADDR[];

// Defines used for the flash jump table routines that are not part of build.
// Note: Any change to this table must accompany a change to ROM_Flash_JT[]!

#define ROM_ICALL_OSAL_JT_LOCATION                      (&RAM_MAC_BASE_ADDR[ROM_RAM_ICALL_OSAL_TABLE_INDEX])
#define ROM_ICALL_OSAL_JT_BASE                          (*((uint32 **)ROM_ICALL_OSAL_JT_LOCATION))
#define ROM_ICALL_OSAL_JT_OFFSET(index)                 (*(ROM_ICALL_OSAL_JT_BASE+(index)))

#define ROM_FH_JT_LOCATION                              (&RAM_MAC_BASE_ADDR[ROM_RAM_FH_TABLE_INDEX])
#define ROM_FH_JT_BASE                                  (*((uint32 **)ROM_FH_JT_LOCATION))
#define ROM_FH_JT_OFFSET(index)                         (*(ROM_FH_JT_BASE+(index)))

#define ROM_HMAC_JT_LOCATION                            (&RAM_MAC_BASE_ADDR[ROM_RAM_HMAC_TABLE_INDEX])
#define ROM_HMAC_JT_BASE                                (*((uint32 **)ROM_HMAC_JT_LOCATION))
#define ROM_HMAC_JT_OFFSET(index)                       (*(ROM_HMAC_JT_BASE+(index)))

#define ROM_LMAC_JT_LOCATION                            (&RAM_MAC_BASE_ADDR[ROM_RAM_LMAC_TABLE_INDEX])
#define ROM_LMAC_JT_BASE                                (*((uint32 **)ROM_LMAC_JT_LOCATION))
#define ROM_LMAC_JT_OFFSET(index)                       (*(ROM_LMAC_JT_BASE+(index)))

void Icall_Osal_ROM_Init(void);
void FH_ROM_Init(void);
void HMAC_ROM_Init(void);
void LMAC_ROM_Init(void);

#endif
