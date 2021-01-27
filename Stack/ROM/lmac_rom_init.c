/******************************************************************************

 @file  lmac_rom_init.c

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

#include "mac.h"
#include "mac_tx.h"
#include "mac_radio.h"
#include "mac_radio_tx.h"
#include "mac_rx_onoff.h"
#include "mac_symbol_timer.h"
#include "mac_low_level.h"

#include "rom_jt_def_154.h"

/*******************************************************************************
 * EXTERNS
 */

extern uint8 rxFhRsl;
#if defined(COMBO_MAC) || defined(FREQ_2_4G)
extern bool macSrcMatchIsEnabled;
#endif
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

#if defined __TI_COMPILER_VERSION || defined __TI_COMPILER_VERSION__
#pragma DATA_ALIGN(LMAC_ROM_Flash_JT, 4)
#elif defined(__GNUC__) || defined(__clang__)
__attribute__ ((aligned (4)))
#else
#pragma data_alignment=4
#endif

const uint32 LMAC_ROM_Flash_JT[] =
{
    /* global variables */
    (uint32)&macTxSlottedDelay,
    (uint32)&macUnitBackoffPeriod,
    (uint32)&rxFhRsl,
    (uint32)&bInSwi,
    (uint32)&macPhyChannel,
#if defined(COMBO_MAC) || defined(FREQ_2_4G)
    (uint32)&macSrcMatchIsEnabled,
#else
    (uint32)NULL,
#endif
    (uint32)&macRadioYielded,

    /* function pointer */
    (uint32)&macRegisterSfdDetect,                                       // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+0]
    (uint32)&macTxFrame,                                                 // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+1]
    (uint32)&macRadioSetTxPower,                                         // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+2]
    (uint32)&macRxSoftEnable,                                            // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+3]
    (uint32)&macRxSoftDisable,                                           // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+4]
    (uint32)&macRadioSetChannel,                                         // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+5]
    (uint32)&macRadioSetChannelRx,                                       // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+6]

    (uint32)&macRxOffRequest,                                            // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+7]
    (uint32)&macRxDisable,                                               // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+8]
    (uint32)&macRxEnable,                                                // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+9]
    (uint32)&macRxHardDisable,                                           // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+10]
    (uint32)&macRadioSetPanCoordinator,                                  // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+11]
    (uint32)&macRadioRandomByte,                                         // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+12]

    (uint32)&MAC_SrcMatchCheckAllPending,                                // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+13]
    (uint32)&MAC_SrcMatchAckAllPending,                                  // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+14]

    (uint32)&macLowLevelDiags,                                           // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+15]
    (uint32)&macRadioSwInit,                                             // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+16]
    (uint32)&macRadioSetPanID,                                           // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+17]
    (uint32)&macRadioSetShortAddr,                                       // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+18]
    (uint32)&macRadioSetIEEEAddr,                                        // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+19]
    (uint32)&macSymbolTimerBCWakeUp,                                     // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+20]
    (uint32)&macSymbolTimerInit,                                         // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+21]

    (uint32)&MAC_SrcMatchAddEntry,                                       // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+22]
    (uint32)&MAC_SrcMatchDeleteEntry,                                    // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+23]
    (uint32)&macCanSleep,                                                // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+24]
    (uint32)&macGetRadioTxPowerReg,                                      // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+25]
    (uint32)&macLowLevelReset,                                           // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+26]
    (uint32)&macLowLevelResume,                                          // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+27]
#if defined (COMBO_MAC) || (!defined (FREQ_2_4G))
    (uint32)&macMcuBitReverse,                                           // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+28]
#else
    (uint32)NULL,
#endif
    (uint32)&macRadioEnergyDetectStop,                                   // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+29]
    (uint32)&macRadioSetRE,                                              // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+30]
    (uint32)&macRadioStartScan,                                          // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+31]
    (uint32)&macRadioStopScan,                                           // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+32]
#if defined (FEATURE_ENHANCED_ACK) && (defined (COMBO_MAC) || (!defined (FREQ_2_4G)))
    (uint32)&macRadioTxEnhAckCb,                                         // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+33]
#else
    (uint32)NULL,
#endif
    (uint32)&macRatDisableChannelB,                                      // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+34]
    (uint32)&macRxSequenceNum,                                           // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+35]
    (uint32)&macTxFrameRetransmit,                                       // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+36]
    (uint32)&macSymbolBCTimerPowerUp,                                    // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+37]
#if defined( FEATURE_BEACON_MODE )
    (uint32)&macRadioTxBOBoundary,                                       // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+38]
#else
    (uint32)NULL,
#endif

#if defined (FEATURE_MAC_SECURITY)
    (uint32)&macTxDecrypt,                                               // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+39]
#else
    (uint32)NULL,
#endif
    (uint32)&txCsmaDelay,                                                // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+40]
    (uint32)&macGetRadioState,                                           // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+41]
    (uint32)&MAC_SrcMatchEnable,                                         // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+42]
    (uint32)&macLowLevelYield,                                           // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+43]

    //    (uint32)&macRadioTxBackoffBoundary,                                // ROM_LMAC_JT_OFFSET[LMAC_API_BASE_INDEX+2]
    //(uint32)&macRadioPowerDown,
    //(uint32)&macRadioPowerUp,
    //(uint32)&macRadioPowerUpWait,
    //(uint32)&macRadioPowerUpBeaconMode,

    //(uint32)&macSymbolTimerChkPowerDown,
    //(uint32)&macSymbolTimerCount,
    //(uint32)&macSymbolTimerRealign,
    //(uint32)&macSymbolTimerSetCount,
    //(uint32)&macSymbolTimerSetRollover,
    //(uint32)&macSymbolTimerSetTrigger,
    //(uint32)&macSymbolTimerSetTriggerRollover,

};  

void LMAC_ROM_Init(void)
{   
    /* assign the FH ROM JT table */
    RAM_MAC_BASE_ADDR[ROM_RAM_LMAC_TABLE_INDEX] = (uint32)(LMAC_ROM_Flash_JT);
}

