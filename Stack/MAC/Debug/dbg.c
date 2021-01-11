/******************************************************************************

 @file  dbg.c

 @brief Handles debug trace operations

 Group: WCS, LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2011-2020, Texas Instruments Incorporated
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

/// \addtogroup module_dbg
//@{
#include "dbg.h"
#include "rfctrc_regs.h"
//#include "interrupt.h"
#include <stdint.h>
#include "cpu.h"

#ifndef DBG_GLOBAL_DISABLE

typedef char cs_t;

#define ENTER_CRITICAL_SECTION(x)                                              \
  do { (x) = !CPUcpsid(); } while (0)

#define EXIT_CRITICAL_SECTION(x)                                               \
  do { if (x) { (void) CPUcpsie(); } } while (0)

#define __enable_irq()    asm("cpsie i");
#define __disable_irq()   asm("cpsid i");

/** \brief Function used to implement DBG_PRINTn
 *
 * Waits for the LVDS channel to be ready before storing the printf() parameters and transmitting the
 * LVDS packet.
 *
 * \param[in]  x
 *     Contains the following values:
 *     - [17:16] The channel number to use (1, 2 or 3)
 *     - [15:8] Trace packet header byte
 *     - [2:0] Number of 16-bit printf() arguments, 0-4
 * \param[in]  y
 *     Contains printf() argument 0 in LSW and argument 1 in MSW
 * \param[in]  z
 *     Contains printf() argument 2 in LSW and argument 3 in MSW
 */
void dbgPrintf(uint32_t x, uint32_t y, uint32_t z)
{
   int  ch = (x >> 16) - 1;
   cs_t cs;

   // Wait for previous packet to be transmitted
   while (1) {
      //__disable_irq();
      ENTER_CRITICAL_SECTION(cs);
      if ((SP_TRCCH1CMD[ch] & TRCCH1CMD_CH1PKTHDR_BM) == 0) {
         // Channel ready, transmit packet
         SP_TRCCH1PAR01[ch] = y;
         SP_TRCCH1PAR23[ch] = z;
         SP_TRCCH1CMD[ch] = x;
         //__enable_irq();
         EXIT_CRITICAL_SECTION(cs);
         break;
      }
      else {
         //__enable_irq();
         EXIT_CRITICAL_SECTION(cs);
      }
   }
} // dbgPrintf



/** \brief Function used to implement DBG_PRINTn, n=1,2
 *
 * Waits for the LVDS channel to be ready before storing the printf() parameters and transmitting the
 * LVDS packet.
 *
 * \param[in]  x
 *     Contains the following values:
 *     - [17:16] The channel number to use (1, 2 or 3)
 *     - [15:8] Trace packet header byte
 *     - [2:0] Number of 16-bit printf() arguments, 0-2
 * \param[in]  y
 *     Contains printf() argument 0 in LSW and argument 1 in MSW
 */
void dbgPrintf2(uint32_t x, uint32_t y)
{
   int  ch = (x >> 16) - 1;
   cs_t cs;

   // Wait for previous packet to be transmitted
   while (1) {
      //__disable_irq();
      ENTER_CRITICAL_SECTION(cs);
      if ((SP_TRCCH1CMD[ch] & TRCCH1CMD_CH1PKTHDR_BM) == 0) {
         // Channel ready, transmit packet
         SP_TRCCH1PAR01[ch] = y;
         SP_TRCCH1CMD[ch] = x;
         //__enable_irq();
         EXIT_CRITICAL_SECTION(cs);
         break;
      }
      else {
         //__enable_irq();
         EXIT_CRITICAL_SECTION(cs);
      }
   }
} // dbgPrintf2



/** \brief Function used to implement DBG_PRINT0
 *
 * Waits for the LVDS channel to be ready before storing the printf() parameters and transmitting the
 * LVDS packet.
 *
 * \param[in]  x
 *     Contains the following values:
 *     - [17:16] The channel number to use (1, 2 or 3)
 *     - [15:8] Trace packet header byte
 *     - [2:0] Number of 16-bit printf() arguments, 0
 */
void dbgPrintf0(uint32_t x)
{
   int  ch = (x >> 16) - 1;
   cs_t cs;

   // Wait for previous packet to be transmitted
   while (1) {
      //__disable_irq();
      ENTER_CRITICAL_SECTION(cs);
      if ((SP_TRCCH1CMD[ch] & TRCCH1CMD_CH1PKTHDR_BM) == 0) {
         // Channel ready, transmit packet
         SP_TRCCH1CMD[ch] = x;
         //__enable_irq();
         EXIT_CRITICAL_SECTION(cs);
         break;
      }
      else {
         //__enable_irq();
         EXIT_CRITICAL_SECTION(cs);
      }
   }
} // dbgPrintf0
#endif // !DBG_GLOBAL_DISABLE


//@}
