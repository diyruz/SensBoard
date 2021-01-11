/******************************************************************************

 @file lmac_rom_jt.h

 @brief LMAC API directly map the function to function jump table

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

#ifndef LMAC_ROM_JT_H
#define LMAC_ROM_JT_H

#include "mac_rx_onoff.h"
#include "mac_low_level.h"
#include "mac_symbol_timer.h"
#include "mac_radio.h"
#include "mac_radio_tx.h"


#include "rom_jt_def_154.h"

/*
** LMAC API Proxy
** ROM-to ROM or ROM-to-Flash function
** if there is any patch function, replace the corresponding entries
** LMAC variables are put first in JT
*/

#define  macTxSlottedDelay                                  (*(uint8 * )                        ROM_LMAC_JT_OFFSET(0))
#define  macUnitBackoffPeriod                               (*(uint8 * )                        ROM_LMAC_JT_OFFSET(1))
#define  rxFhRsl                                            (*(uint8 * )                        ROM_LMAC_JT_OFFSET(2))
#define  bInSwi                                             (*(volatile bool * )                ROM_LMAC_JT_OFFSET(3))
#define  macPhyChannel                                      (*(uint8 * )                        ROM_LMAC_JT_OFFSET(4))
#define  macSrcMatchIsEnabled                               (*(bool * )                         ROM_LMAC_JT_OFFSET(5))
#define  macRadioYielded                                    (*(volatile uint8 * )               ROM_LMAC_JT_OFFSET(6))

#define LMAC_API_BASE_INDEX                   (7)
    /* function pointer */
#define MAP_macRegisterSfdDetect                        	((void     (*)(macSfdDetectCBack_t ))               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+0))
#define MAP_macTxFrame                                  	((void     (*)(uint8_t ))             		        ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+1))
#define MAP_macRadioSetTxPower                          	((uint8_t  (*)(int8_t ))             		        ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+2))
#define MAP_macRxSoftEnable                             	((void     (*)(uint8_t ))             			    ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+3))
#define MAP_macRxSoftDisable                            	((void     (*)(uint8_t ))             			    ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+4))
#define MAP_macRadioSetChannel                          	((bool     (*)(uint8_t ))             			    ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+5))
#define MAP_macRadioSetChannelRx                        	((bool     (*)(uint8_t ))             				ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+6))

#define MAP_macRxOffRequest                                 ((void     (*)(void ))                              ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+7))
#define MAP_macRxDisable                                    ((void     (*)(uint8_t ))                           ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+8))
#define MAP_macRxEnable                                     ((bool     (*)(uint8_t ))                           ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+9))
#define MAP_macRxHardDisable                                ((void     (*)(void ))                              ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+10))
#define MAP_macRadioSetPanCoordinator                       ((void     (*)(uint8_t ))                           ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+11))
#define MAP_macRadioRandomByte                              ((uint8    (*)(void ))                              ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+12))

#define MAP_MAC_SrcMatchCheckAllPending                     ((uint8    (*)(void ))                              ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+13))
#define MAP_MAC_SrcMatchAckAllPending                       ((void     (*)(uint8 ))                             ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+14))

#define MAP_macLowLevelDiags                                ((void    (*)(uint8 ))                              ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+15))
#define MAP_macRadioSwInit                                  ((void    (*)(void ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+16))
#define MAP_macRadioSetPanID                                ((void    (*)(uint16 ))                             ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+17))
#define MAP_macRadioSetShortAddr                            ((void    (*)(uint16 ))                             ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+18))
#define MAP_macRadioSetIEEEAddr                             ((void    (*)(uint8 * ))                            ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+19))
#define MAP_macSymbolTimerBCWakeUp                          ((void    (*)(void ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+20))
#define MAP_macSymbolTimerInit                              ((void    (*)(void ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+21))

#define MAP_MAC_SrcMatchAddEntry                            ((uint8    (*)(sAddr_t *, uint16))                  ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+22))
#define MAP_MAC_SrcMatchDeleteEntry                         ((uint8    (*)(sAddr_t *, uint16))                  ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+23))
#define MAP_macCanSleep                                     ((uint8    (*)(void ))                              ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+24))
#define MAP_macGetRadioTxPowerReg                           ((uint32    (*)(int8 ))                             ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+25))
#define MAP_macLowLevelReset                                ((void    (*)(void ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+26))
#define MAP_macLowLevelResume                               ((void    (*)(void ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+27))
#define MAP_macMcuBitReverse                                ((void    (*)(uint8 *, uint16))                     ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+28))
#define MAP_macRadioEnergyDetectStop                        ((uint8    (*)(void ))                              ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+29))
#define MAP_macRadioSetRE                                   ((void    (*)(uint32 ))                             ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+30))
#define MAP_macRadioStartScan                               ((void    (*)(uint8 ))                              ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+31))
#define MAP_macRadioStopScan                                ((void    (*)(void ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+32))
#define MAP_macRadioTxEnhAckCb                              ((void    (*)(macTx_t *pMsg ))                      ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+33))
#define MAP_macRatDisableChannelB                           ((void    (*)(void ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+34))
#define MAP_macRxSequenceNum                                ((uint8    (*)(void ))                              ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+35))
#define MAP_macTxFrameRetransmit                            ((void    (*)(void ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+36))
#define MAP_macSymbolBCTimerPowerUp                         ((void    (*)(void ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+37))
#define MAP_macRadioTxBOBoundary                            ((uint32  (*)(void ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+38))
#define MAP_macTxDecrypt                                    ((uint8   (*)(void ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+39))
#define MAP_txCsmaDelay                                     ((void    (*)(uint8 ))                              ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+40))
#define MAP_macGetRadioState                                ((bool    (*)(void ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+41))
#define MAP_MAC_SrcMatchEnable                              ((void    (*)(void ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+42))
#define MAP_macLowLevelYield                                ((bool    (*)(void ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+43))

//#define MAP_macRadioTxBackoffBoundary                       ((uint32    (*)(uint32 ))                           ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+15))

//#define MAP_macRadioPowerDown                               ((void    (*)(bool ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+6))
//#define MAP_macRadioPowerUp                                 ((void    (*)(bool ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+6))
//#define MAP_macRadioPowerUpWait                             ((void    (*)(void ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+6))
//#define MAP_macRadioPowerUpBeaconMode                       ((void    (*)(bool ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+6))

//#define MAP_macSymbolTimerChkPowerDown                      ((void    (*)(void ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+6))
//#define MAP_macSymbolTimerCount                             ((uint32  (*)(void ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+6))
//#define MAP_macSymbolTimerRealign                           ((int32   (*)(macRx_t *pMsg ))                      ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+6))
//#define MAP_macSymbolTimerSetCount                          ((void    (*)(uint32 ))                             ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+6))
//#define MAP_macSymbolTimerSetRollover                       ((void    (*)(uint32 ))                             ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+6))
//#define MAP_macSymbolTimerSetTrigger                        ((void    (*)(uint32 ))                             ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+6))
//#define MAP_macSymbolTimerSetTriggerRollover                ((void    (*)(void ))                               ROM_LMAC_JT_OFFSET(LMAC_API_BASE_INDEX+6))

#endif
