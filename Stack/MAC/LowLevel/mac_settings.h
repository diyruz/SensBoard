/******************************************************************************

 @file  mac_settings.h

 @brief Describe the purpose and contents of the file.

 Group: WCS, LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2006-2021, Texas Instruments Incorporated
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
#ifndef MAC_SETTINGS_H
#define MAC_SETTINGS_H

#if defined(COMBO_MAC) || defined(FREQ_2_4G)
#include <driverlib/rf_ieee_mailbox.h>
#include <driverlib/rf_ieee_cmd.h>
#endif
#if defined(COMBO_MAC) || !defined(FREQ_2_4G)
#include <driverlib/rf_prop_cmd.h>
#endif
#include <driverlib/rf_mailbox.h>
#include <driverlib/rf_common_cmd.h>
#include <ti/drivers/rf/RF.h>
/* ------------------------------------------------------------------------------------------------
 *                                            Defines
 * ------------------------------------------------------------------------------------------------
 */
//To be removed when solution to RFCORE49 is available
#if defined(COMBO_MAC) || !defined(FREQ_2_4G)
#define RFCORE49
#endif
/**
 * (IEEE 802.15.4-2006 7.4.1) MAC constants.
 */
#define IEEE802154_A_UINT_BACKOFF_PERIOD  (20)

/**
 * (IEEE 802.15.4-2006 6.4.1) PHY constants.
 */
#define IEEE802154_A_TURNAROUND_TIME      (12)

/**
 * (IEEE 802.15.4-2006 6.4.2) PHY PIB attribute, specifically the O-QPSK PHY.
 */
#define IEEE802154_PHY_SHR_DURATION       (10)

/**
 * (IEEE 802.15.4-2006 6.4.2) PHY PIB attribute, specifically the O-QPSK PHY.
 */
#define IEEE802154_PHY_SYMBOLS_PER_OCTET  (2)

/**
 * (IEEE 802.15.4-2006 7.4.2) macAckWaitDuration PIB attribute.
 */
#define IEEE802154_MAC_ACK_WAIT_DURATION                                       \
        (IEEE802154_A_UINT_BACKOFF_PERIOD + IEEE802154_A_TURNAROUND_TIME       \
         + IEEE802154_PHY_SHR_DURATION                                         \
         + ( 6 * IEEE802154_PHY_SYMBOLS_PER_OCTET))
/**
 * (IEEE 802.15.4-2006 6.5.3.2) O-QPSK symbol rate.
 */
#define IEEE802154_SYMBOLS_PER_SEC        (62500)

/**
 * Frequency of the Radio Timer module.
 *
 * 4MHz clock.
 */
#define PLATFORM_RADIO_RAT_TICKS_PER_SEC  (4000000)
/* ------------------------------------------------------------------------------------------------
 *                                           Constants
 * ------------------------------------------------------------------------------------------------
 */
// RF Core variables

// RF Core API commands
#if defined(COMBO_MAC) || defined(FREQ_2_4G)
extern rfc_CMD_RADIO_SETUP_PA_t RF_cmdRadioSetup;
extern rfc_CMD_IEEE_TX_t RF_cmdIEEETx;
extern rfc_CMD_IEEE_RX_t RF_cmdIEEERx;
extern rfc_CMD_IEEE_CSMA_t RF_cmdIEEECsma;
extern rfc_CMD_IEEE_RX_ACK_t RF_cmdRxAckCmd;
#endif
#if defined(COMBO_MAC) || !defined(FREQ_2_4G)
extern rfc_CMD_PROP_RADIO_DIV_SETUP_PA_t RF_cmdPropRadioDivSetup;
extern rfc_CMD_PROP_TX_ADV_t RF_cmdPropTxAdv;
extern rfc_CMD_PROP_RX_ADV_t RF_cmdPropRxAdv;
extern rfc_CMD_PROP_CS_t RF_cmdPropCs;
extern rfc_CMD_PROP_CS_t RF_cmdPropCsSlotted;
#endif
extern rfc_CMD_FS_t RF_cmdFsRx;
extern rfc_CMD_FS_t RF_cmdFsTx;
extern rfc_CMD_NOP_t RF_cmdNop;
extern rfc_CMD_NOP_t RF_cmdNopFg;
extern rfc_CMD_SCH_IMM_t RF_cmdScheduleImmediate;
extern rfc_CMD_SET_RAT_CMP_t RF_cmdRat;
extern rfc_CMD_SET_RAT_CMP_t RF_cmdRat1;
#endif // MAC_SETTINGS_H
