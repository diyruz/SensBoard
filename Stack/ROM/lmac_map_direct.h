/******************************************************************************

 @file lmac_map_direct.h

 @brief LMAC API directly map the function implementation (declaration)

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

#ifndef LMAC_MAP_DIRECT_H
#define LMAC_MAP_DIRECT_H

#include "mac.h"
#include "mac_rx_onoff.h"
#include "mac_low_level.h"
#include "mac_symbol_timer.h"
#include "mac_radio.h"
#include "mac_radio_tx.h"

extern uint8 rxFhRsl;
#if defined(COMBO_MAC) || defined(FREQ_2_4G)
extern bool macSrcMatchIsEnabled;
#endif

#define MAP_macRegisterSfdDetect                        macRegisterSfdDetect
#define MAP_macTxFrame                                  macTxFrame
#define MAP_macRadioSetTxPower                          macRadioSetTxPower
#define MAP_macRxSoftEnable                             macRxSoftEnable
#define MAP_macRxSoftDisable                            macRxSoftDisable
#define MAP_macRadioSetChannel                          macRadioSetChannel
#define MAP_macRadioSetChannelRx                        macRadioSetChannelRx

#define MAP_macRxOffRequest                             macRxOffRequest
#define MAP_macRxDisable                                macRxDisable
#define MAP_macRxEnable                                 macRxEnable
#define MAP_macRxHardDisable                            macRxHardDisable
#define MAP_macRadioSetPanCoordinator                   macRadioSetPanCoordinator

#define MAP_macRadioRandomByte                          macRadioRandomByte
#define MAP_MAC_SrcMatchCheckAllPending                 MAC_SrcMatchCheckAllPending
#define MAP_MAC_SrcMatchAckAllPending                   MAC_SrcMatchAckAllPending

#define MAP_macLowLevelDiags                            macLowLevelDiags

#define MAP_macRadioSwInit                              macRadioSwInit
#define MAP_macRadioSetPanID                            macRadioSetPanID
#define MAP_macRadioSetShortAddr                        macRadioSetShortAddr
#define MAP_macRadioSetIEEEAddr                         macRadioSetIEEEAddr

#define MAP_macSymbolTimerBCWakeUp                      macSymbolTimerBCWakeUp
#define MAP_macSymbolTimerInit                          macSymbolTimerInit

#define MAP_MAC_SrcMatchEnable                          MAC_SrcMatchEnable
#define MAP_MAC_SrcMatchAddEntry                        MAC_SrcMatchAddEntry
#define MAP_MAC_SrcMatchDeleteEntry                     MAC_SrcMatchDeleteEntry
#define MAP_macCanSleep                                 macCanSleep
#define MAP_macGetRadioTxPowerReg                       macGetRadioTxPowerReg
#define MAP_macLowLevelReset                            macLowLevelReset
#define MAP_macLowLevelResume                           macLowLevelResume
#define MAP_macMcuBitReverse                            macMcuBitReverse
#define MAP_macRadioEnergyDetectStop                    macRadioEnergyDetectStop

#define MAP_macRadioSetRE                               macRadioSetRE
#define MAP_macRadioStartScan                           macRadioStartScan
#define MAP_macRadioStopScan                            macRadioStopScan
#define MAP_macRadioTxEnhAckCb                          macRadioTxEnhAckCb
#define MAP_macRatDisableChannelB                       macRatDisableChannelB
#define MAP_macRxSequenceNum                            macRxSequenceNum
#define MAP_macTxFrameRetransmit                        macTxFrameRetransmit
#define MAP_macSymbolBCTimerPowerUp                     macSymbolBCTimerPowerUp

#define MAP_macRadioTxBOBoundary                        macRadioTxBOBoundary
#define MAP_macTxDecrypt                                macTxDecrypt
#define MAP_txCsmaDelay                                 txCsmaDelay
#define MAP_macGetRadioState                            macGetRadioState

//#define MAP_macRadioTxBackoffBoundary                   macRadioTxBackoffBoundary
//#define MAP_macRadioPowerDown                           macRadioPowerDown
//#define MAP_macRadioPowerUp                             macRadioPowerUp
//#define MAP_macRadioPowerUpWait                         macRadioPowerUpWait
//#define MAP_macRadioPowerUpBeaconMode                   macRadioPowerUpBeaconMode
//#define MAP_macSymbolTimerChkPowerDown                  macSymbolTimerChkPowerDown
//#define MAP_macSymbolTimerCount                         macSymbolTimerCount
//#define MAP_macSymbolTimerRealign                       macSymbolTimerRealign
//#define MAP_macSymbolTimerSetCount                      macSymbolTimerSetCount
//#define MAP_macSymbolTimerSetRollover                   macSymbolTimerSetRollover
//#define MAP_macSymbolTimerSetTrigger                    macSymbolTimerSetTrigger
//#define MAP_macSymbolTimerSetTriggerRollover            macSymbolTimerSetTriggerRollover
//#define MAP_macRadioInit                                macRadioInit

/*
**  LMAC API directly map to implementation
**  This is used in CC131X/CC135X project with using any TIMAC 15.4 ROM image
*/

//#define MAP_FHAPI_reset                                             FHAPI_reset
//#define MAP_FHAPI_start                                             FHAPI_start
//#define MAP_FHAPI_startBS                                           FHAPI_startBS

#endif
