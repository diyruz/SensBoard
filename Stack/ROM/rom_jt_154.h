/******************************************************************************

 @file  rom_jt_154.h

 @brief Public interface file for ROM jump table

 Group: WCS, LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2005-2021, Texas Instruments Incorporated
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

#ifndef TIMAC_ROM_JUMP_TABLE_H
#define TIMAC_ROM_JUMP_TABLE_H

#if defined(TIMAC_ROM_IMAGE_BUILD)
/*
    If ROM_BUILD or COMMON_ROM_BUILD are defined then all MAP_ prefixed
    functions must be indirectly to any function address through a jump
    table. These indirect mappings are dependent on which ROM image is
    being built and are found in the following headers:
*/
#include "fh_rom_jt.h"
#include "icall_osal_rom_jt.h"
#include "lmac_rom_jt.h"
#include "hmac_rom_jt.h"

void TIMAC_ROM_Init( void );

#else
/*
    If this is not a ROM_BUILD then all MAP_ prefixed functions can be
    directly mapped and linked to the given function's address instead
    of a jump table entry. These mappings are not dependent on the ROM
    image and can be found in the following header:
*/

#include "fh_map_direct.h"
#include "hmac_map_direct.h"
#include "lmac_map_direct.h"
#include "icall_osal_map_direct.h"

#endif

#endif

