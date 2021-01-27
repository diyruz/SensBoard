/**************************************************************************************************
  Filename:       touchlink_target_app.c
  Revised:        $Date: 2013-11-26 15:12:49 -0800 (Tue, 26 Nov 2013) $
  Revision:       $Revision: 36298 $

  Description:    Zigbee Cluster Library - Light Link Target.


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
#include "ti_zstack_config.h"
#if defined ( BDB_TL_TARGET )
/*********************************************************************
 * INCLUDES
 */
#include "zcomdef.h"
#include "rom_jt_154.h"
#include "osal_nv.h"
#include "af.h"
#include "zd_app.h"
#include "nwk_util.h"
#include "addr_mgr.h"
#include "zd_sec_mgr.h"
#include "zstacktask.h"
#include "zstackapi.h"

#include "stub_aps.h"

#include "zcl.h"
#include "zcl_port.h"
#include "zcl_general.h"
#include "bdb.h"
#include "bdb_tl_commissioning.h"
#include "bdb_touchlink.h"
#include "touchlink_target_app.h"
#include "bdb_touchlink_target.h"


/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
uint8_t tlAppEntity;
uint8_t touchlinkAppAllowStealing;


/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static ZStatus_t targetScanReqCB( afAddrType_t *srcAddr, bdbTLScanReq_t *pReq, uint8_t seqNum );
static ZStatus_t targetDeviceInfoReqCB( afAddrType_t *srcAddr, bdbTLDeviceInfoReq_t *pReq, uint8_t seqNum );
static ZStatus_t targetIdentifyReqCB( afAddrType_t *srcAddr, bdbTLIdentifyReq_t *pReq );
static ZStatus_t targetResetToFNReqCB( afAddrType_t *srcAddr, bdbTLResetToFNReq_t *pReq );
static ZStatus_t targetNwkStartReqCB( afAddrType_t *srcAddr, bdbTLNwkStartReq_t *pReq, uint8_t seqNum );
static ZStatus_t targetNwkJoinReqCB( afAddrType_t *srcAddr, bdbTLNwkJoinReq_t *pReq, uint8_t seqNum );
static ZStatus_t targetNwkUpdateReqCB( afAddrType_t *srcAddr, bdbTLNwkUpdateReq_t *pReq );
static ZStatus_t targetEndpointInfo(afAddrType_t *srcAddr, bdbTLEndpointInfo_t *pRsp);
static ZStatus_t targetGetGrpIDsRsp(afAddrType_t *srcAddr, bdbTLGetGrpIDsRsp_t *pRsp);
static ZStatus_t targetGetEPListRsp(afAddrType_t *srcAddr, bdbTLGetEPListRsp_t *pRsp);

/*********************************************************************
 * LOCAL VARIABLES
 */
// Info related to the received request
static afAddrType_t dstAddr;
static tl_BDBFindingBindingCb_t commissioningCb;

/*********************************************************************
 * TouchLink Target Callback Table
 */
// Target Command Callbacks table
static bdbTL_InterPANCallbacks_t touchLinkTarget_CmdCBs =
{
  // Received Server Commands
  targetScanReqCB,          // Scan Request command
  targetDeviceInfoReqCB,    // Device Information Request command
  targetIdentifyReqCB,      // Identify Request command
  targetResetToFNReqCB,     // Reset to Factory New Request command
  targetNwkStartReqCB,      // Network Start Request command
  #if ( ZSTACK_ROUTER_BUILD )
  targetNwkJoinReqCB,       // Network Join Router Request command
  NULL,                     // Network Join End Device Request command
#else
  NULL,                     // Network Join Router Request command
  targetNwkJoinReqCB,       // Network Join End Device Request command
#endif
  targetNwkUpdateReqCB,     // Network Update Request command

  // Received Client Commands
  NULL,                     // Scan Response command
  NULL,                     // Device Information Response command
  NULL,                     // Network Start Response command
  NULL,                     // Network Join Router Response command
  NULL                      // Network Join End Device Response command
};

static bdbTL_AppCallbacks_t touchlinkTarget_AppCBs =
{
  // Received Server Commands
  NULL,  // Get Group Identifiers Request command
  NULL,  // Get Endpoint List Request command

  // Received Client Commands
  targetEndpointInfo,  // Endpoint Information command
  targetGetGrpIDsRsp,  // Get Group Identifiers Response command
  targetGetEPListRsp  // Get Endpoint List Response command
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn          touchLinkTargetApp_Init
 *
 * @brief       Initialize the touchlink target module.
 *              This function must be called by the application during its initialization.
 *
 * @param       zclSampleAppEvents - the events process by the sample application
 *
 * @return      none
 */
void touchLinkTargetApp_Init(uint8_t  zclSampleApp_Entity)
{
  tlAppEntity = zclSampleApp_Entity;
  zstack_bdbTouchlinkGetAllowStealingRsp_t getAllowStealingRsp;


  StubAPS_RegisterApp( &touchLink_EP );

  zclport_registerEndpoint(tlAppEntity, &touchLink_EP);

  bdbTL_RegisterInterPANCmdCallbacks(&touchLinkTarget_CmdCBs);

  Zstackapi_bdbTouchlinkGetAllowStealingReq(tlAppEntity,&getAllowStealingRsp);
  touchlinkAppAllowStealing = getAllowStealingRsp.allowStealing;

  bdbTL_RegisterCmdCallbacks(TOUCHLINK_INTERNAL_ENDPOINT, &touchlinkTarget_AppCBs);
}

/*********************************************************************
 * @fn          touchLinkApp_registerFindingBindingCb
 *
 * @brief       Register application finding and binding callback
 *
 * @param       comCb - bdb finding and binding callback function
 *
 * @return      none
 */
void touchLinkApp_registerFindingBindingCb(tl_BDBFindingBindingCb_t comCb)
{
  commissioningCb = comCb;
}

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      targetScanReqCB
 *
 * @brief   This callback is called to process a Scan Request command.
 *
 * @param   srcAddr - sender's address
 * @param   pReq - parsed command
 * @param   seqNum - command sequence number
 *
 * @return  ZStatus_t
 */
static ZStatus_t targetScanReqCB( afAddrType_t *srcAddr, bdbTLScanReq_t *pReq, uint8_t seqNum )
{
  ZStatus_t ret = ZSuccess;
  int8_t rssi;

  if( pReq->touchLinkInitiator == FALSE )
  {
    return ZFailure;
  }

  rssi = touchLink_GetMsgRssi();
  if( rssi > TOUCHLINK_WORST_RSSI )
  {
    dstAddr = *srcAddr;
    dstAddr.panId = 0xFFFF;

    zstack_tlScanReq_t *pScanReq;
    pScanReq = (zstack_tlScanReq_t*)zcl_mem_alloc( sizeof(zstack_tlScanReq_t) );

    zcl_memcpy(&(pScanReq->addr), &dstAddr, sizeof(afAddrType_t));
    pScanReq->transID = pReq->transID;
    pScanReq->seqNum = seqNum;

    Zstackapi_tlScanReqInd(tlAppEntity, pScanReq);
    zcl_mem_free(pScanReq);

    if ( ret == ZSuccess )
    {
      touchLinkTransID = pReq->transID;
    }
  }

  return ( ret );
}

/*********************************************************************
 * @fn      targetDeviceInfoReqCB
 *
 * @brief   This callback is called to process a Device Information
 *          Request command.
 *
 * @param   srcAddr - sender's address
 * @param   pReq - parsed command
 * @param   seqNum - command sequence number
 *
 * @return  ZStatus_t
 */
static ZStatus_t targetDeviceInfoReqCB( afAddrType_t *srcAddr, bdbTLDeviceInfoReq_t *pReq, uint8_t seqNum )
{
  if ( touchLink_IsValidTransID( pReq->transID ) == FALSE )
  {
    return ( ZFailure );
  }
  return ( touchLink_SendDeviceInfoRsp( TOUCHLINK_INTERNAL_ENDPOINT, srcAddr,
                                  pReq->startIndex, pReq->transID, seqNum ) );
}

/*********************************************************************
 * @fn      targetIdentifyReqCB
 *
 * @brief   This callback is called to process an Identify Request command.
 *
 * @param   srcAddr - sender's address
 * @param   pReq - parsed command
 *
 * @return  ZStatus_t
 */
static ZStatus_t targetIdentifyReqCB( afAddrType_t *srcAddr, bdbTLIdentifyReq_t *pReq )
{
  zstack_bdbZCLIdentifyCmdIndReq_t Req;
  zstack_bdbSetEpDescListToActiveEndpointRsp_t Rsp;

  if ( touchLink_IsValidTransID( pReq->transID ) == FALSE )
  {
    return ( ZFailure );
  }

  // The target should identify itself
  uint16_t identifyTime;

  // Values of the Identify Duration field:
  // - Exit identify mode: 0x0000
  // - Length of time to remain in identify mode: 0x0001-0xfffe
  // - Remain in identify mode for a default time known by the receiver: 0xffff
  if ( pReq->IdDuration == 0xffff )
  {
    identifyTime = TOUCHLINK_DEFAULT_IDENTIFY_TIME;
  }
  else
  {
    identifyTime = pReq->IdDuration;
  }

  Zstackapi_bdbSetEpDescListToActiveEndpoint(tlAppEntity,&Rsp);

  Req.identifyTime = identifyTime;
  Req.endpoint = Rsp.EndPoint;

  Zstackapi_bdbZclIdentifyCmdIndReq(tlAppEntity, &Req);

  return ( ZSuccess );
}

/*********************************************************************
 * @fn      targetResetToFNReqCB
 *
 * @brief   This callback is called to process a Reset to Factory New
 *          Request command.
 *
 * @param   srcAddr - sender's address
 * @param   pReq - parsed command
 *
 * @return  ZStatus_t
 */
static ZStatus_t targetResetToFNReqCB( afAddrType_t *srcAddr, bdbTLResetToFNReq_t *pReq )
{
  // If factory new, discard the request
  if ( ( touchLink_IsValidTransID( pReq->transID ) == FALSE ) )
  {
    return ( ZFailure );
  }

  Zstackapi_tlTargetResetToFNReqInd(tlAppEntity);

  return ( ZSuccess );
}

/*********************************************************************
 * @fn      targetNwkStartReqCB
 *
 * @brief   This callback is called to process a Network Start Request command.
 *
 * @param   srcAddr - sender's address
 * @param   pReq - parsed command
 * @param   seqNum - command sequence number
 *
 * @return  ZStatus_t
 */
static ZStatus_t targetNwkStartReqCB( afAddrType_t *srcAddr, bdbTLNwkStartReq_t *pReq, uint8_t seqNum )
{
#if ZSTACK_END_DEVICE_BUILD
  return ( ZFailure );
#else
  if ( touchLink_IsValidTransID( pReq->transID ) == FALSE )
  {
    return ( ZFailure );
  }

  dstAddr = *srcAddr;
  dstAddr.panId = 0xFFFF;

  zstack_tlTargetNwkStartReq_t *pTargetNwkStartReq;
  pTargetNwkStartReq = (zstack_tlTargetNwkStartReq_t*)zcl_mem_alloc( sizeof(zstack_tlTargetNwkStartReq_t) );

  zcl_memcpy(&(pTargetNwkStartReq->addr), &dstAddr, sizeof(afAddrType_t));
  zcl_memcpy(&(pTargetNwkStartReq->nwkStartReq), pReq, sizeof(bdbTLNwkStartReq_t));
  pTargetNwkStartReq->seqNum = seqNum;
  pTargetNwkStartReq->allowStealing = touchlinkAppAllowStealing;

  Zstackapi_tlTargetNwkStartReqInd(tlAppEntity, pTargetNwkStartReq);
  zcl_mem_free(pTargetNwkStartReq);

  return ( ZSuccess );
#endif
}

/*********************************************************************
 * @fn      targetNwkJoinReqCB
 *
 * @brief   This callback is called to process a Network Join
 *          Request command.
 *
 * @param   srcAddr - sender's address
 * @param   pReq - parsed command
 * @param   seqNum - command sequence number
 *
 * @return  ZStatus_t
 */
static ZStatus_t targetNwkJoinReqCB( afAddrType_t *srcAddr, bdbTLNwkJoinReq_t *pReq, uint8_t seqNum )
{
  if ( touchLink_IsValidTransID( pReq->transID ) == FALSE )
  {
    return ( ZFailure );
  }

  uint8_t newUpdateId = touchLink_NewNwkUpdateId( pReq->nwkUpdateId, _NIB.nwkUpdateId);
  if ( _NIB.nwkUpdateId != newUpdateId )
  {
    NLME_SetUpdateID(newUpdateId);
  }

  dstAddr = *srcAddr;
  dstAddr.panId = 0xFFFF;

  zstack_touchlinkNwkJointReq_t *pTargetNwkJoinReq;
  pTargetNwkJoinReq = (zstack_touchlinkNwkJointReq_t*)zcl_mem_alloc( sizeof(zstack_touchlinkNwkJointReq_t) );

  zcl_memcpy(&(pTargetNwkJoinReq->addr), &dstAddr, sizeof(afAddrType_t));
  zcl_memcpy(&(pTargetNwkJoinReq->nwkJoinReq), pReq, sizeof(bdbTLNwkJoinReq_t));
  pTargetNwkJoinReq->seqNum = seqNum;
  pTargetNwkJoinReq->allowStealing = touchlinkAppAllowStealing;

  Zstackapi_touchlinkNwkJoinReqInd(tlAppEntity, pTargetNwkJoinReq);
  zcl_mem_free(pTargetNwkJoinReq);

  if(commissioningCb)
  {
    commissioningCb();
  }

  return ( ZSuccess );
}

/*********************************************************************
 * @fn      targetNwkUpdateReqCB
 *
 * @brief   This callback is called to process a Network Update Request
 *          command.
 *
 * @param   srcAddr - sender's address
 * @param   pReq - parsed command
 *
 * @return  ZStatus_t
 */
static ZStatus_t targetNwkUpdateReqCB( afAddrType_t *srcAddr, bdbTLNwkUpdateReq_t *pReq )
{
    if ( touchLink_IsValidTransID( pReq->transID ) == FALSE )
    {
      return ( ZFailure );
    }

    zstack_touchlinkNwkUpdateReq_t *pTargetNwkUpdateReq;
    pTargetNwkUpdateReq = (zstack_touchlinkNwkUpdateReq_t*)zcl_mem_alloc( sizeof(zstack_touchlinkNwkUpdateReq_t) );

    pTargetNwkUpdateReq->transID = pReq->transID;
    zcl_cpyExtAddr(pTargetNwkUpdateReq->extendedPANID, pReq->extendedPANID);
    pTargetNwkUpdateReq->nwkUpdateId = pReq->nwkUpdateId;
    pTargetNwkUpdateReq->logicalChannel = pReq->logicalChannel;
    pTargetNwkUpdateReq->PANID = pReq->PANID;
    pTargetNwkUpdateReq->nwkAddr = pReq->nwkAddr;

    Zstackapi_touchlinkNwkUpdateReqInd(tlAppEntity, pTargetNwkUpdateReq);
    zcl_mem_free(pTargetNwkUpdateReq);

    return ( ZSuccess );
}

/*********************************************************************
 * @fn      targetEndpointInfo
 *
 * @brief   This callback is called to process an Endpoint Info Cmd
 *
 * @param   *srcAddr -- source address of the command
 * @param   *pRsp - parsed command payload
 *
 * @return  ZStatus_t
 */
static ZStatus_t targetEndpointInfo(afAddrType_t *srcAddr, bdbTLEndpointInfo_t *pRsp)
{
  // TODO: Implement application behavior
  return ZSuccess;
}

/*********************************************************************
 * @fn      targetGetGrpIDsRsp
 *
 * @brief   This callback is called to process a Get Group Identifiers
 *          Response
 *
 * @param   *srcAddr -- source address of the response
 * @param   *pRsp - parsed response payload
 *
 * @return  ZStatus_t
 */
static ZStatus_t targetGetGrpIDsRsp(afAddrType_t *srcAddr, bdbTLGetGrpIDsRsp_t *pRsp)
{
  // TODO: Implement application behavior
  return ZSuccess;
}

/*********************************************************************
 * @fn      targetGetEPListRsp
 *
 * @brief   This callback is called to process a Get Endpoint List
 *          Response
 *
 * @param   *srcAddr -- source address of the response
 * @param   *pRsp - parsed response payload
 *
 * @return  ZStatus_t
 */
static ZStatus_t targetGetEPListRsp(afAddrType_t *srcAddr, bdbTLGetEPListRsp_t *pRsp)
{
  // TODO: Implement application behavior
  return ZSuccess;
}

#endif // BDB_TL_TARGET

/*********************************************************************
*********************************************************************/
