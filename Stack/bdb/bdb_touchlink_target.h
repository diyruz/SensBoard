/**************************************************************************************************
  Filename:       bdb_touchlink_target.h
  Revised:        $Date: 2013-05-10 15:34:14 -0700 (Fri, 10 May 2013) $
  Revision:       $Revision: 34239 $

  Description:    This file contains the ZCL Touch Link Target definitions.


  Copyright 2011-2013 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
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
**************************************************************************************************/

#ifndef TOUCHLINK_TARGET_H
#define TOUCHLINK_TARGET_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "zcl_general.h"
#include "bdb_tl_commissioning.h"

/*********************************************************************
 * CONSTANTS
 */

// Task Events
#define TOUCHLINK_NWK_DISC_CNF_EVT                               0x0001
#define TOUCHLINK_NWK_JOIN_IND_EVT                               0x0002
#define TOUCHLINK_RESET_TO_FN_EVT                                0x0004
#define TOUCHLINK_START_NWK_EVT                                  0x0008
#define TOUCHLINK_JOIN_ATTEMPT_EVT                               0x0010
#define TOUCHLINK_NWK_START_EVT                                  0x0020
#define TOUCHLINK_TRANS_LIFETIME_EXPIRED_EVT                     0x0040
#define TOUCHLINK_TARGET_ENABLE_TIMEOUT                          0x0080
#define TOUCHLINK_NWK_RESTORE_NWK_PARAMETERS_EVT                 0x0100
#define TOUCHLINK_NWK_FORMATION_SUCCESS_EVT                      0x0200
#define TOUCHLINK_NWK_ANNOUNCE_EVT                               0x0400

#define TARGET_PERPETUAL_OPERATION_ENABLED          FALSE
#define TOUCHLINK_TARGET_PERPETUAL                  86400001 //currently we decided that 24 hours is the maximum time. more than this - will be treated as perpetual

/*********************************************************************
 * TYPEDEFS
 */
typedef void (*tlGCB_TargetEnable_t)( uint8_t enable );

/*********************************************************************
 * VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

/*-------------------------------------------------------------------
 * TASK API - These functions must only be called by OSAL.
 */

/*
 * Initialization for the TouchLink Target task
 */
extern void touchLinkTarget_Init( uint8_t task_id );

/*
 * Event Process for the TouchLink Target task
 */
extern uint32_t touchLinkTarget_event_loop( uint8_t task_id, uint32_t events );

/*
 * @brief   This callback is called to process a Leave Confirmation message.
 */
extern void* targetZdoLeaveCnfCB( void *pParam );

/*
 * @brief   Send out a Scan Response command.
 */
extern ZStatus_t touchLink_TargetSendScanRsp( uint8_t srcEP, afAddrType_t *dstAddr, uint32_t transID, uint8_t seqNum );


#if (ZSTACK_ROUTER_BUILD)
/*
 * @brief   Send out a Network Start Response command.
 */
extern void targetSendNwkStartRsp( afAddrType_t *dstAddr, uint32_t transID, uint8_t status,
                                   bdbTLNwkParams_t *pNwkParams, uint8_t nwkUpdateId, uint8_t seqNum );

#if (defined (USE_ICALL) || defined (OSAL_PORT2TIRTOS))
/*
 * @brief   Send network start event to touchlink event loop
 */
extern void targetProcessNwkStartEvt(afAddrType_t *pAddr, bdbTLNwkStartReq_t *pNwkStartReq, uint8_t seqNum, bool allowStealing);
#endif
#endif  //  ZSTACK_ROUTER_BUILD

/*
 * @brief   Process ZDO device state change
 */
void targetProcessStateChange( devStates_t devState );

/*
 * @brief   Send network join event to touchlink event loop
 */
extern void targetProcessNwkJoinEvt(afAddrType_t *pAddr, bdbTLNwkJoinReq_t *pNwkJoinReq, uint8_t seqNum, bool allowStealing);

/*
 * @brief   Send factory new reset to event loop.
 */
extern void targetProcessResetToFNEvt( void );

/*
 * @brief   Send network Update event to touchlink event loop
 */
extern void targetProcessNwkUpdateEvt(bdbTLNwkUpdateReq_t *pNwkUpdateReq);

/*
 * @brief   General function to allow stealing when performing TL as target
 */
extern void bdb_TouchlinkSetAllowStealing( bool allow );

/*
 * @brief   General function to get the allow stealing value
 */
extern bool bdb_TouchlinkGetAllowStealing( void );

/*
 * @brief   Register an Application's Enable/Disable callback function.
 *          Refer to touchLinkTarget_EnableCommissioning to enable/disable TL as target
 */
extern void bdb_RegisterTouchlinkTargetEnableCB( tlGCB_TargetEnable_t pfnTargetEnableChange );

/*
 * @brief   This callback is called to process a Leave Confirmation message.
 *
 *          Note: this callback function returns a pointer if it has handled
 *                the confirmation message and no further action should be
 *                taken with it. It returns NULL if it has not handled the
 *                confirmation message and normal processing should take place.
 */
extern void *initiatorZdoLeaveCnfCB( void *pParam );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* TOUCHLINK_TARGET_H */
