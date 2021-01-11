/******************************************************************************

 @file  mac_activity.h

 @brief This file contains the data structures and APIs the MAC Activity
        tracking module.
        Note: Only utilized in DMM applications.

 Group: WCS, LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2019-2020, Texas Instruments Incorporated
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

#ifndef MAC_ACTIVITY_H
#define MAC_ACTIVITY_H

/*******************************************************************************
 * INCLUDES
 */
#include "mac_user_config.h"

/*******************************************************************************
 * CONSTANTS
 */
/* MAC Activity priorities */
#define MAC_ACTIVITY_PRI_NORMAL_INDEX  (0)
#define MAC_ACTIVITY_PRI_HIGH_INDEX    (1)
#define MAC_ACTIVITY_PRI_URGENT_INDEX  (2)

#define MAC_ACTIVITY_PRI_NORMAL (0x0000)
#define MAC_ACTIVITY_PRI_HIGH   (0x0001)
#define MAC_ACTIVITY_PRI_URGENT (0x0002)
#define MAC_ACTIVITY_NUM_PRI    (3)
#define MAC_ACTIVITY_COUNT      (6)

#define MAC_ACTIVITY_THRESHOLD_PRI_NORMAL (1)
#define MAC_ACTIVITY_THRESHOLD_PRI_HIGH   (2)
#define MAC_ACTIVITY_THRESHOLD_PRI_URGENT (3)

/* Priority threshold for RX BEACON Activity */
#define MAC_ACTIVITY_THRESHOLD_REM_BEACON_NORMAL_PRI (3)
#define MAC_ACTIVITY_THRESHOLD_REM_BEACON_HIGH_PRI (2)

/* Default increment for preemption tracking */
#define MAC_ACTIVITY_DEFAULT_INC (1)

/* MAC Activity types */
typedef enum
{
    MAC_ACTIVITY_LINK_EST     = 0x0001,
    MAC_ACTIVITY_TX_BEACON    = 0x0002,
    MAC_ACTIVITY_RX_BEACON    = 0x0003,
    MAC_ACTIVITY_FH           = 0x0004,
    MAC_ACTIVITY_SCAN         = 0x0005,
    MAC_ACTIVITY_DATA         = 0x0006,
    MAC_ACTIVITY_RX_ALWAYS_ON = 0x0007,
} macActivity_t;


typedef struct {
    uint8_t numMissedAssocTxFrames; /* Number of missed association packets. */
    uint8_t numMissedBeaconTxFrames; /* Number of missed broadcasts */
    uint8_t numMissedFHTxFrames; /* Number of missed FH frames */
    uint8_t numMissedDataTxFrames; /* Number of missed data frames */
    uint8_t prevTxFrameType; /* Last successfully transmitted frame */
    macActivity_t txActivity; /* Current activity associated with the Transmitter */
} macTxIntActivityData_t;

typedef struct {
    uint8_t numRxAssocAbort; /* Number of missed association packets. */
    uint8_t numRxBCAbort; /* Number of missed broadcasts */
    uint8_t numRxScanAbort; /* Number of aborted scans */
    uint8_t numRxDataAbort; /* Number of MAC data Rx aborts */
    uint8_t prevRxFrameType; /* Last successfully received command frame type */
    macActivity_t rxActivity; /* Current activity associated with the Receiver */
} macRxIntActivityData_t;

/* Activity tracking module function pointer typedef's. */
typedef void (*setActivityTrackingTx_t)(macTxIntData_t *txData, uint16_t cmdStatus, RF_EventMask rfEvent);
typedef void (*setActivityTrackingRx_t)(macRx_t *pRxBuf, bool resetCount);
typedef uint32_t (*getActivityPriorityTx_t)(void);
typedef uint32_t (*getActivityPriorityRx_t)(void);
typedef void (*setActivityTx_t)(macTxIntData_t *txData);
typedef void (*setActivityRx_t)(uint8_t rxEna_tbleFlags);
typedef macActivity_t (*getActivityTx_t)(void);
typedef macActivity_t (*getActivityRx_t)(void);

#ifdef MAC_ACTIVITY_PROFILING
typedef void (*startActivityProfilingTimer_t)(RF_Handle handle);
typedef void (*printActivityInfo_t)(uint32_t activity, uint32_t priority, uint8_t frameType, sAddr_t destAddr);
#endif

/* Activity tracking object type definition */
typedef const struct _activityObject
{
    setActivityTrackingTx_t pSetActivityTrackingTxFn;
    setActivityTrackingRx_t pSetActivityTrackingRxFn;
    getActivityPriorityTx_t pGetActivityPriorityTxFn;
    getActivityPriorityRx_t pGetActivityPriorityRxFn;
    setActivityTx_t pSetActivityTxFn;
    setActivityRx_t pSetActivityRxFn;
    getActivityTx_t pGetActivityTxFn;
    getActivityRx_t pGetActivityRxFn;

#ifdef MAC_ACTIVITY_PROFILING
    startActivityProfilingTimer_t pStartActivityProfilingTimerFn;
    printActivityInfo_t pPrintActivityInfoFn;
#endif
} activityObject_t;


/* ------------------------------------------------------------------------------------------------
 *                                          Macros
 * ------------------------------------------------------------------------------------------------
 */
/* MAC Activity table priority MACRO */
#define CALC_ACTIVITY_PRIORITY(ACTIVITY, PRIORITY) ((ACTIVITY << 16) | PRIORITY)

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */
extern activityObject_t activityObject;

/*******************************************************************************
 * APIs
 */

/*******************************************************************************
 */

#endif /* MAC_ACTIVITY_H */
