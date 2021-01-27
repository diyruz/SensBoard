/******************************************************************************

 @file  dbg.h

 @brief Provides data logging

 Group: WCS, LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2011-2021, Texas Instruments Incorporated
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

#ifndef _DBG_H
#define _DBG_H

#if defined(USE_ITM_DBG)
/**
 *
 * The tracer module enables ITM via a high speed single wire output.
 */
#include "itm.h"
#elif defined(USE_TIRTOS_DBG)
#include <stdint.h>
#include <xdc/runtime/Log.h>
#endif //USE_ITM_DBG

#define MAC_DBG_CH  0
#define APP_DBG_CH  1
#define DBGSYS      MAC_DBG_CH

#if USE_TIRTOS_DBG
/// Emulates DBG_PRINT with no arguments, but sends an ID instead of the format string
#define DBG_PRINT0(ch, str) \
   (void) ch; \
   Log_info0(str);

/// Emulates DBG_PRINT with 1 numeric argument (16-bit)
#define DBG_PRINT1(ch, str, a0) \
        (void) ch; \
        Log_info1(str, a0);

/// Emulates DBG_PRINT with 2 numeric arguments (16-bit)
#define DBG_PRINT2(ch, str, a0, a1) \
        (void) ch; \
        Log_info2(str, a0, a1);

/// Emulates DBG_PRINT with 3 numeric arguments (16-bit)
#define DBG_PRINT3(ch, str, a0, a1, a2) \
        (void) ch; \
        Log_info3(str, a0, a1, a2);

/// Emulates DBG_PRINT with 4 numeric arguments (16-bit)
#define DBG_PRINT4(ch, str, a0, a1, a2, a3) \
        (void) ch; \
        Log_info4(str, a0, a1, a2, a3);

/// Emulates DBG_PRINT with 1 long numeric argument (32-bit)
#define DBG_PRINTL1(ch, str, a0) \
        (void) ch; \
        Log_info1(str, a0);

/// Emulates DBG_PRINT with 2 long numeric arguments (32-bit)
#define DBG_PRINTL2(ch, str, a0, a1) \
        (void) ch; \
        Log_info2(str, a0, a1);

#elif defined(USE_ITM_DBG)
/// Emulates DBG_PRINT with no arguments, but sends an ID instead of the format string
#define DBG_PRINT0(ch, str) \
  {  \
      ITM_putStringBlocking(str, ch); \
      ITM_putNewLineBlocking(ch); \
  }

/// Emulates DBG_PRINT with 1 numeric argument (16-bit)
#define DBG_PRINT1(ch, str, a0) \
    { \
      ITM_putStringBlocking(str, ch); \
      ITM_putStringBlocking(" ,", ch); \
      ITM_putValueBlocking(a0, ch); \
      ITM_putNewLineBlocking(ch); \
    }

/// Emulates DBG_PRINT with 2 numeric arguments (16-bit)
#define DBG_PRINT2(ch, str, a0, a1) \
    { \
      ITM_putStringBlocking(str, ch); \
      ITM_putStringBlocking(", ", ch); \
      ITM_putValueBlocking(a0, ch); \
      ITM_putStringBlocking(", ", ch); \
      ITM_putValueBlocking(a1, ch); \
      ITM_putNewLineBlocking(ch); \
    }

/// Emulates DBG_PRINT with 3 numeric arguments (16-bit)
#define DBG_PRINT3(ch, str, a0, a1, a2) \
    { \
      ITM_putStringBlocking(str, ch); \
      ITM_putStringBlocking(", ", ch); \
      ITM_putValueBlocking(a0, ch); \
      ITM_putStringBlocking(", ", ch); \
      ITM_putValueBlocking(a1, ch); \
      ITM_putStringBlocking(", ", ch); \
      ITM_putValueBlocking(a2, ch); \
      ITM_putNewLineBlocking(ch); \
    }

/// Emulates DBG_PRINT with 4 numeric arguments (16-bit)
#define DBG_PRINT4(ch, str, a0, a1, a2, a3) \
    { \
      ITM_putStringBlocking(str, ch); \
      ITM_putStringBlocking(", ", ch); \
      ITM_putValueBlocking(a0, ch); \
      ITM_putStringBlocking(", ", ch); \
      ITM_putValueBlocking(a1, ch); \
      ITM_putStringBlocking(", ", ch); \
      ITM_putValueBlocking(a2, ch); \
      ITM_putNewLineBlocking(ch); \
    }

/// Emulates DBG_PRINT with 1 long numeric argument (32-bit)
#define DBG_PRINTL1(ch, str, a0) \
    { \
      ITM_putStringBlocking(str, ch); \
      ITM_putStringBlocking(", ", ch); \
      ITM_putValueBlocking(a0, ch); \
      ITM_putNewLineBlocking(ch); \
    }

/// Emulates DBG_PRINT with 2 long numeric arguments (32-bit)
#define DBG_PRINTL2(ch, str, a0, a1) \
    { \
      ITM_putStringBlocking(str, ch); \
      ITM_putStringBlocking(", ", ch); \
      ITM_putValueBlocking(a0, ch); \
      ITM_putStringBlocking(", ", ch); \
      ITM_putValueBlocking(a1, ch); \
      ITM_putNewLineBlocking(ch); \
    }

#else
/// Emulates DBG_PRINT with no arguments, but sends an ID instead of the format string
#define DBG_PRINT0(ch, str)
/// Emulates DBG_PRINT with 1 numeric argument (16-bit)
#define DBG_PRINT1(ch, str, a0)
/// Emulates DBG_PRINT with 2 numeric arguments (16-bit)
#define DBG_PRINT2(ch, str, a0, a1)
/// Emulates DBG_PRINT with 3 numeric arguments (16-bit)
#define DBG_PRINT3(ch, str, a0, a1, a2)
/// Emulates DBG_PRINT with 4 numeric arguments (16-bit)
#define DBG_PRINT4(ch, str, a0, a1, a2, a3)
/// Emulates DBG_PRINT with 1 long numeric argument (32-bit)
#define DBG_PRINTL1(ch, str, a0)
/// Emulates DBG_PRINT with 2 long numeric arguments (32-bit)
#define DBG_PRINTL2(ch, str, a0, a1)

#endif //#if USE_TIRTOS_DBG || USE_ITM_DEBUG

//@}
//-------------------------------------------------------------------------------------------------------




#endif
//@}
