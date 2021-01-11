/******************************************************************************

 @file  rom_init_154.c

 @brief This file contains the externs for FH ROM API initialization.

 Group: WCS, BTS
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2017-2020, Texas Instruments Incorporated
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

#include "rom_jt_def_154.h"

/*******************************************************************************
 * EXTERNS
 */


/*******************************************************************************
 * PROTOTYPES
 */

/*******************************************************************************
 * MACROS
 */

/* RAM address assigned */
#if defined __TI_COMPILER_VERSION__ && defined __TI_ARM__
#pragma DATA_SECTION(RAM_MAC_BASE_ADDR ,".data:RAM_MAC_BASE_ADDR")
uint32_t RAM_MAC_BASE_ADDR[ROM_RAM_TABLE_SIZE];

#elif defined(__IAR_SYSTEMS_ICC__)
#pragma location=".data_RAM_BASE_ADDR"
uint32_t RAM_MAC_BASE_ADDR[ROM_RAM_TABLE_SIZE];
#elif defined(__GNUC__) || defined(__clang__)
uint32_t RAM_MAC_BASE_ADDR[ROM_RAM_TABLE_SIZE] __attribute__((section(".data:RAM_MAC_BASE_ADDR")));
#else
#error "Unsupported platform or compiler"
#endif

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



/* ******************************************************************************
 * @fn          FH ROM Spinlock
 *
 * @brief       This routine is used to trap indexing errors in R2R JT.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void ROM_MAC_Spinlock( void )
{
  volatile uint8 i = 1;

  while(i);
}

/*******************************************************************************
 * @fn          BLE ROM Initialization
 *
 * @brief       This routine initializes the BLE Controller ROM software. First,
 *              the image's CRC is verified. Next, its C runtime is initialized.
 *              Then the ICall function pointers for dispatch, and enter/leave
 *              critical section are initialized. Finally, the location of the
 *              R2F and R2R flash jump tables are initialized.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void TIMAC_ROM_Init( void )
{
#if 0
  volatile uint8 i;

  // verify the Controller ROM image
  i = validChecksum(&__checksum_begin, &__checksum_end);

  // trap a checksum failure - what now?
  while( !i );
#endif // ENABLE_ROM_CHECKSUM_CHECK

  /*
  ** Controller ROM
  */
  Icall_Osal_ROM_Init();
  FH_ROM_Init();
  HMAC_ROM_Init();
  LMAC_ROM_Init();

  return;
}



