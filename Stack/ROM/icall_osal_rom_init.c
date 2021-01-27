/******************************************************************************

 @file  icall_osal_rom_init.c

 @brief This file contains the externs for ROM API initialization.

 Group: WCS, BTS
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2017-2021, Texas Instruments Incorporated
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

/*******************************************************************************
 * INCLUDES
 */

#ifndef OSAL_PORT2TIRTOS
#include "icall.h"
#include "osal.h"
#include "osal_pwrmgr.h"
#else
#include "osal_port.h"
#define ICall_Errno uint8_t
#define ICall_CSState uint32_t
#endif

#include "string.h"
#include "saddr.h"

#include "rom_jt_def_154.h"

#include <ti/drivers/dpl/ClockP.h>

/*******************************************************************************
 * EXTERNS
 */

extern ICall_CSState ICall_enterCSImpl(void);
extern void ICall_leaveCSImpl(ICall_CSState key);


/*******************************************************************************
 * PROTOTYPES
 */

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

// ROM Flash Jump Table

#if defined(USE_ICALL) && !defined(USE_DMM)
#if defined __TI_COMPILER_VERSION || defined __TI_COMPILER_VERSION__
#pragma DATA_ALIGN(ICALL_OSAL_ROM_Flash_JT, 4)
#elif defined(__GNUC__) || defined(__clang__)
__attribute__ ((aligned (4)))
#else
#pragma data_alignment=4
#endif

const uint32 ICALL_OSAL_ROM_Flash_JT[] =
{
     /* function pointer */
     (uint32)&ICall_enterCSImpl,                                        // ROM_ICALL_OSAL_JT_OFFSET[0]
     (uint32)&ICall_leaveCSImpl,                                        // ROM_ICALL_OSAL_JT_OFFSET[1]

     (uint32)&osal_mem_alloc,                                           // ROM_ICALL_OSAL_JT_OFFSET[2]
     (uint32)&osal_mem_free,                                            // ROM_ICALL_OSAL_JT_OFFSET[3]
     (uint32)&osal_memcmp,                                              // ROM_ICALL_OSAL_JT_OFFSET[4]
     (uint32)&osal_memcpy,                                              // ROM_ICALL_OSAL_JT_OFFSET[5]
     (uint32)&osal_memset,                                              // ROM_ICALL_OSAL_JT_OFFSET[6]

     (uint32)&osal_msg_allocate,                                        // ROM_ICALL_OSAL_JT_OFFSET[7]
     (uint32)&osal_msg_deallocate,                                      // ROM_ICALL_OSAL_JT_OFFSET[8]
     (uint32)&osal_msg_dequeue,                                         // ROM_ICALL_OSAL_JT_OFFSET[9]
     (uint32)&osal_msg_enqueue,                                          // ROM_ICALL_OSAL_JT_OFFSET[10]
     (uint32)&osal_msg_enqueue_max,                                     // ROM_ICALL_OSAL_JT_OFFSET[11]

     (uint32)&osal_msg_extract,                                         // ROM_ICALL_OSAL_JT_OFFSET[12]
     (uint32)&osal_msg_push,                                          	// ROM_ICALL_OSAL_JT_OFFSET[13]
     (uint32)&osal_msg_receive,                                         // ROM_ICALL_OSAL_JT_OFFSET[14]
     (uint32)&osal_msg_send,                                          	// ROM_ICALL_OSAL_JT_OFFSET[15]
     (uint32)&osal_pwrmgr_task_state,                                   // ROM_ICALL_OSAL_JT_OFFSET[16]
     (uint32)&osal_set_event,                                          	// ROM_ICALL_OSAL_JT_OFFSET[17]
     (uint32)&osal_strlen,                                              // ROM_ICALL_OSAL_JT_OFFSET[18]
     (uint32)&memcmp,                                          			// ROM_ICALL_OSAL_JT_OFFSET[19]

     (uint32)&ICall_getTicks,                                          	// ROM_ICALL_OSAL_JT_OFFSET[20]
     (uint32)&ICall_setTimer,                                          	// ROM_ICALL_OSAL_JT_OFFSET[21]
     (uint32)&ICall_stopTimer,                                          // ROM_ICALL_OSAL_JT_OFFSET[22]

     (uint32)&sAddrCmp,                                             // ROM_ICALL_OSAL_JT_OFFSET[23]
     (uint32)&sAddrCpy,                                             // ROM_ICALL_OSAL_JT_OFFSET[24]
     (uint32)&sAddrExtCpy,                                          // ROM_ICALL_OSAL_JT_OFFSET[25]
     (uint32)&sAddrExtCmp,                                          // ROM_ICALL_OSAL_JT_OFFSET[26]

};
#else
const uint32 ICALL_OSAL_ROM_Flash_JT[] =
{
     /* function pointer */
     (uint32)&OsalPort_enterCS,                                        // ROM_ICALL_OSAL_JT_OFFSET[0]
     (uint32)&OsalPort_leaveCS,                                        // ROM_ICALL_OSAL_JT_OFFSET[1]

     (uint32)&OsalPort_malloc,                                           // ROM_ICALL_OSAL_JT_OFFSET[2]
     (uint32)&OsalPort_free,                                            // ROM_ICALL_OSAL_JT_OFFSET[3]
     (uint32)&OsalPort_memcmp,                                              // ROM_ICALL_OSAL_JT_OFFSET[4]
     (uint32)&memcpy,                                              // ROM_ICALL_OSAL_JT_OFFSET[5]
     (uint32)&memset,                                              // ROM_ICALL_OSAL_JT_OFFSET[6]

     (uint32)&OsalPort_msgAllocate,                                        // ROM_ICALL_OSAL_JT_OFFSET[7]
     (uint32)&OsalPort_msgDeallocate,                                      // ROM_ICALL_OSAL_JT_OFFSET[8]
     (uint32)&OsalPort_msgDequeue,                                         // ROM_ICALL_OSAL_JT_OFFSET[9]
     (uint32)&OsalPort_msgEnqueue,                                          // ROM_ICALL_OSAL_JT_OFFSET[10]
     (uint32)&OsalPort_msgEnqueueMax,                                     // ROM_ICALL_OSAL_JT_OFFSET[11]

     (uint32)&OsalPort_msgExtract,                                         // ROM_ICALL_OSAL_JT_OFFSET[12]
     (uint32)&OsalPort_msgPush,                                            // ROM_ICALL_OSAL_JT_OFFSET[13]
     (uint32)&OsalPort_msgReceive,                                         // ROM_ICALL_OSAL_JT_OFFSET[14]
     (uint32)&OsalPort_msgSend,                                            // ROM_ICALL_OSAL_JT_OFFSET[15]
     (uint32)&OsalPort_pwrmgrTaskState,                                   // ROM_ICALL_OSAL_JT_OFFSET[16]
     (uint32)&OsalPort_setEvent,                                           // ROM_ICALL_OSAL_JT_OFFSET[17]
     (uint32)&strlen,                                              // ROM_ICALL_OSAL_JT_OFFSET[18]
     (uint32)&memcmp,                                                   // ROM_ICALL_OSAL_JT_OFFSET[19]

     (uint32)&ClockP_getSystemTicks,                                           // ROM_ICALL_OSAL_JT_OFFSET[20]
     (uint32)&OsalPort_setTimer,                                           // ROM_ICALL_OSAL_JT_OFFSET[21]
     (uint32)&OsalPort_stopTimer,                                          // ROM_ICALL_OSAL_JT_OFFSET[22]

     (uint32)&sAddrCmp,                                             // ROM_ICALL_OSAL_JT_OFFSET[23]
     (uint32)&sAddrCpy,                                             // ROM_ICALL_OSAL_JT_OFFSET[24]
     (uint32)&sAddrExtCpy,                                          // ROM_ICALL_OSAL_JT_OFFSET[25]
     (uint32)&sAddrExtCmp,                                          // ROM_ICALL_OSAL_JT_OFFSET[26]
};

#endif


void Icall_Osal_ROM_Init(void)
{
    /* assign the Icall Osal ROM JT table */
    RAM_MAC_BASE_ADDR[ROM_RAM_ICALL_OSAL_TABLE_INDEX] = (uint32)(ICALL_OSAL_ROM_Flash_JT);
}

