/**************************************************************************************************
  Filename:       touchlink_initiator_app.c
  Revised:        $Date: 2013-11-26 15:12:49 -0800 (Tue, 26 Nov 2013) $
  Revision:       $Revision: 36298 $

  Description:    Zigbee Cluster Library - Light Link Initiator.


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
#if defined ( BDB_TL_INITIATOR)
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
#include "touchlink_initiator_app.h"
#include "bdb_touchlink_initiator.h"

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

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static ZStatus_t initiatorScanReqCB( afAddrType_t *srcAddr, bdbTLScanReq_t *pReq, uint8_t seqNum );
static ZStatus_t initiatorDeviceInfoReqCB( afAddrType_t *srcAddr, bdbTLDeviceInfoReq_t *pReq, uint8_t seqNum );
static ZStatus_t initiatorIdentifyReqCB( afAddrType_t *srcAddr, bdbTLIdentifyReq_t *pReq );
static ZStatus_t initiatorNwkJoinReqCB( afAddrType_t *srcAddr, bdbTLNwkJoinReq_t *pReq, uint8_t seqNum );
static ZStatus_t initiatorNwkUpdateReqCB( afAddrType_t *srcAddr, bdbTLNwkUpdateReq_t *pReq );
static ZStatus_t initiatorScanRspCB( afAddrType_t *srcAddr, bdbTLScanRsp_t *pRsp );
static ZStatus_t initiatorDeviceInfoRspCB( afAddrType_t *srcAddr, bdbTLDeviceInfoRsp_t *pRsp );
static ZStatus_t initiatorNwkStartRspCB( afAddrType_t *srcAddr, bdbTLNwkStartRsp_t *pRsp );
static ZStatus_t initiatorNwkJoinRspCB( afAddrType_t *srcAddr, bdbTLNwkJoinRsp_t *pRsp );
/*********************************************************************
 * LOCAL VARIABLES
 */


/*********************************************************************
 * TOUCHLINK Initiator Callback Table
 */
// Initiator Command Callbacks table
static bdbTL_InterPANCallbacks_t touchLinkInitiator_CmdCBs =
{
  // Received Server Commands
  initiatorScanReqCB,       // Scan Request command
  initiatorDeviceInfoReqCB, // Device Information Request command
  initiatorIdentifyReqCB,   // Identify Request command
  NULL,                     // Reset to Factory New Request command
  NULL,                     // Network Start Request command
#if ( ZSTACK_ROUTER_BUILD )
  initiatorNwkJoinReqCB,    // Network Join Router Request command
  NULL,                     // Network Join End Device Request command
#else
  NULL,                     // Network Join Router Request command
  initiatorNwkJoinReqCB,    // Network Join End Device Request command
#endif
  initiatorNwkUpdateReqCB,  // Network Update Request command

  // Received Client Commands
  initiatorScanRspCB,       // Scan Response command
  initiatorDeviceInfoRspCB, // Device Information Response command
  initiatorNwkStartRspCB,   // Network Start Response command
  initiatorNwkJoinRspCB,    // Network Join Router Response command
  initiatorNwkJoinRspCB     // Network Join End Device Response command
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn          touchLinkInitiatorApp_Init
 *
 * @brief       Initialize the touchlink initiator module.
 *              This function must be called by the application during its initialization.
 *
 * @param       zclSampleAppEvents - the events process by the sample application
 *
 * @return      none
 */
void touchLinkInitiatorApp_Init(uint8_t zclSampleApp_Entity)
{
  tlAppEntity = zclSampleApp_Entity;

  StubAPS_RegisterApp( &touchLink_EP );

  zclport_registerEndpoint(tlAppEntity, &touchLink_EP);

  bdbTL_RegisterInterPANCmdCallbacks(&touchLinkInitiator_CmdCBs);
}

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      initiatorScanReqCB
 *
 * @brief   This callback is called to process a Scan Request command.
 *
 * @param   srcAddr - sender's address
 * @param   pReq - parsed command
 * @param   seqNum - command sequence number
 *
 * @return  ZStatus_t
 */
static ZStatus_t initiatorScanReqCB( afAddrType_t *srcAddr, bdbTLScanReq_t *pReq, uint8_t seqNum )
{
  int8_t rssi;
  rssi = touchLink_GetMsgRssi();
  if( ( rssi > TOUCHLINK_WORST_RSSI ) && ( pReq->touchLinkInitiator == TRUE ) )
  {
    // response to the originator, but switch to dst PAN 0xFFFF
    afAddrType_t dstAddr;
    OsalPort_memcpy(&dstAddr, srcAddr, sizeof(afAddrType_t));
    dstAddr.panId = 0xFFFF;

    if ( pReq->touchLinkFactoryNew == TRUE )
    {
      // If we are factory new and revice a Scan Request from other factory new
      // device then drop the request.
      if ( bdbAttributes.bdbNodeIsOnANetwork == FALSE )
       {
         return ( ZSuccess );
       }
       // If, during its scan, a non factory new initiator receives another scan
       // request inter-PAN command frame from a factory new target, it shall be ignored.
      else
      {
        uint16_t time;
        zstack_touchlinkGetScanBaseTime_t getScanBaseTime;

        getScanBaseTime.stopTimer = FALSE;
        Zstackapi_touchlinkGetScanBaseTime(tlAppEntity, &getScanBaseTime);
        time = getScanBaseTime.time;

        if (time > 0)
        {
          return ( ZSuccess );
        }
      }
    }

    zstack_tlScanReq_t *pScanReq;
    pScanReq = (zstack_tlScanReq_t*)zcl_mem_alloc( sizeof(zstack_tlScanReq_t) );

    zcl_memcpy(&(pScanReq->addr), &dstAddr, sizeof(afAddrType_t));
    pScanReq->transID = pReq->transID;
    pScanReq->seqNum = seqNum;

    Zstackapi_tlScanReqInd(tlAppEntity, pScanReq);
    zcl_mem_free(pScanReq);
  }

  return ( ZSuccess );
}

/*********************************************************************
 * @fn      initiatorDeviceInfoReqCB
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
static ZStatus_t initiatorDeviceInfoReqCB( afAddrType_t *srcAddr, bdbTLDeviceInfoReq_t *pReq, uint8_t seqNum )
{
  if ( touchLink_IsValidTransID( pReq->transID ) == FALSE )
  {
    return ( ZFailure );
  }
  return ( touchLink_SendDeviceInfoRsp( TOUCHLINK_INTERNAL_ENDPOINT, srcAddr,
                                  pReq->startIndex, pReq->transID, seqNum ) );
}

/*********************************************************************
 * @fn      initiatorIdentifyReqCB
 *
 * @brief   This callback is called to process an Identify Request command.
 *
 * @param   srcAddr - sender's address
 * @param   pReq - parsed command
 *
 * @return  ZStatus_t
 */
static ZStatus_t initiatorIdentifyReqCB( afAddrType_t *srcAddr, bdbTLIdentifyReq_t *pReq )
{
  zstack_bdbZCLIdentifyCmdIndReq_t Req;
  zstack_bdbSetEpDescListToActiveEndpointRsp_t Rsp;

  if ( touchLink_IsValidTransID( pReq->transID ) == FALSE )
  {
    return ( ZFailure );
  }

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
 * @fn      initiatorNwkJoinReqCB
 *
 * @brief   This callback is called to process Network Join
 *          Request and Network Join End Device Request commands.
 *
 * @param   srcAddr - sender's address
 * @param   pReq - parsed command
 * @param   seqNum - command sequence number
 *
 * @return  ZStatus_t
 */
static ZStatus_t initiatorNwkJoinReqCB( afAddrType_t *srcAddr, bdbTLNwkJoinReq_t *pReq, uint8_t seqNum )
{
  if ( touchLink_IsValidTransID( pReq->transID ) == FALSE )
  {
    return ( ZFailure );
  }

  zstack_touchlinkNwkJointReq_t *pInitiatorNwkJoinReq;
  pInitiatorNwkJoinReq = (zstack_touchlinkNwkJointReq_t*)zcl_mem_alloc( sizeof(zstack_touchlinkNwkJointReq_t) );

  zcl_memcpy(&(pInitiatorNwkJoinReq->addr), srcAddr, sizeof(afAddrType_t));
  zcl_memcpy(&(pInitiatorNwkJoinReq->nwkJoinReq), pReq, sizeof(bdbTLNwkJoinReq_t));
  pInitiatorNwkJoinReq->seqNum = seqNum;

  Zstackapi_touchlinkNwkJoinReqInd(tlAppEntity, pInitiatorNwkJoinReq);
  zcl_mem_free(pInitiatorNwkJoinReq);

  return ( ZSuccess );
}

/*********************************************************************
 * @fn      initiatorNwkUpdateReqCB
 *
 * @brief   This callback is called to process a Network Update Request
 *          command.
 *
 * @param   srcAddr - sender's address
 * @param   pReq - parsed command
 *
 * @return  ZStatus_t
 */
static ZStatus_t initiatorNwkUpdateReqCB( afAddrType_t *srcAddr, bdbTLNwkUpdateReq_t *pReq )
{
  if ( touchLink_IsValidTransID( pReq->transID ) == FALSE )
  {
    return ( ZFailure );
  }

  zstack_touchlinkNwkUpdateReq_t *pInitiatorNwkUpdateReq;
  pInitiatorNwkUpdateReq = (zstack_touchlinkNwkUpdateReq_t*)zcl_mem_alloc( sizeof(zstack_touchlinkNwkUpdateReq_t) );

  pInitiatorNwkUpdateReq->transID = pReq->transID;
  zcl_cpyExtAddr(pInitiatorNwkUpdateReq->extendedPANID, pReq->extendedPANID);
  pInitiatorNwkUpdateReq->nwkUpdateId = pReq->nwkUpdateId;
  pInitiatorNwkUpdateReq->logicalChannel = pReq->logicalChannel;
  pInitiatorNwkUpdateReq->PANID = pReq->PANID;
  pInitiatorNwkUpdateReq->nwkAddr = pReq->nwkAddr;

  Zstackapi_touchlinkNwkUpdateReqInd(tlAppEntity, pInitiatorNwkUpdateReq);
  zcl_mem_free(pInitiatorNwkUpdateReq);

  return ( ZSuccess );
}

/*********************************************************************
 * @fn      initiatorScanRspCB
 *
 * @brief   This callback is called to process a Scan Response command.
 *
 * @param   srcAddr - sender's address
 * @param   pRsp - parsed command
 *
 * @return  ZStatus_t
 */
static ZStatus_t initiatorScanRspCB( afAddrType_t *srcAddr, bdbTLScanRsp_t *pRsp )
{
  if  ( touchLink_IsValidTransID( pRsp->transID ) &&
      ( pRsp->keyBitmask & touchLink_GetNwkKeyBitmask() ) )
  {
    zstack_touchlinkScanRsp_t *pInitiatorScanRsp;
    pInitiatorScanRsp = (zstack_touchlinkScanRsp_t*)zcl_mem_alloc( sizeof(zstack_touchlinkScanRsp_t) );

    zcl_memcpy(&pInitiatorScanRsp->addr, srcAddr, sizeof(afAddrType_t));
    pInitiatorScanRsp->scanRsp.transID = pRsp->transID;
    pInitiatorScanRsp->scanRsp.rssiCorrection = pRsp->rssiCorrection;
    zcl_memcpy(&pInitiatorScanRsp->scanRsp.zInfo, &pRsp->zInfo, sizeof(zInfo_t));
    zcl_memcpy(&pInitiatorScanRsp->scanRsp.touchLinkInfo, &pRsp->touchLinkInfo, sizeof(touchLinkInfo_t));
    pInitiatorScanRsp->scanRsp.keyBitmask = pRsp->keyBitmask;
    pInitiatorScanRsp->scanRsp.responseID = pRsp->responseID;
    zcl_cpyExtAddr(pInitiatorScanRsp->scanRsp.extendedPANID, pRsp->extendedPANID);
    pInitiatorScanRsp->scanRsp.nwkUpdateId = pRsp->nwkUpdateId;
    pInitiatorScanRsp->scanRsp.logicalChannel = pRsp->logicalChannel;
    pInitiatorScanRsp->scanRsp.PANID = pRsp->PANID;
    pInitiatorScanRsp->scanRsp.nwkAddr = pRsp->nwkAddr;
    pInitiatorScanRsp->scanRsp.numSubDevices = pRsp->numSubDevices;
    pInitiatorScanRsp->scanRsp.totalGrpIDs = pRsp->totalGrpIDs;
    zcl_memcpy(&pInitiatorScanRsp->scanRsp.deviceInfo, &pRsp->deviceInfo, sizeof(bdbTLDeviceInfo_t));

    Zstackapi_tlInitiatorScanRspInd(tlAppEntity, pInitiatorScanRsp);
    zcl_mem_free(pInitiatorScanRsp);
    return ( ZSuccess );
  }
  return ( ZFailure );
}

/*********************************************************************
 * @fn      initiatorDeviceInfoRspCB
 *
 * @brief   This callback is called to process a Device Information
 *          Response command.
 *          If sub-device is selected, selectedTarget data is updated.
 *
 * @param   srcAddr - sender's address
 * @param   pRsp - parsed command
 *
 * @return  ZStatus_t
 */
static ZStatus_t initiatorDeviceInfoRspCB( afAddrType_t *srcAddr, bdbTLDeviceInfoRsp_t *pRsp )
{
  if ( touchLink_IsValidTransID( pRsp->transID ) )
  {
    zstack_touchlinkDevInfoRsp_t *pDevInfoRsp;
    pDevInfoRsp = (zstack_touchlinkDevInfoRsp_t*)zcl_mem_alloc( sizeof(zstack_touchlinkDevInfoRsp_t) + ( pRsp->cnt * sizeof(devInfoRec_t) ) );

    zcl_memcpy(&pDevInfoRsp->addr, srcAddr, sizeof(afAddrType_t));
    pDevInfoRsp->devInfoRsp.transID = pRsp->transID;
    pDevInfoRsp->devInfoRsp.numSubDevices = pRsp->numSubDevices;
    pDevInfoRsp->devInfoRsp.startIndex = pRsp->startIndex;
    pDevInfoRsp->devInfoRsp.cnt = pRsp->cnt;
    zcl_memcpy(&pDevInfoRsp->devInfoRsp.devInfoRec, &pRsp->devInfoRec, (pRsp->cnt * sizeof(devInfoRec_t)));

    Zstackapi_tlInitiatorDevInfoRspInd(tlAppEntity, pDevInfoRsp);
    zcl_mem_free(pDevInfoRsp);
    return ( ZSuccess );
  }
  return ( ZFailure );
}

/*********************************************************************
 * @fn      initiatorNwkStartRspCB
 *
 * @brief   This callback is called to process a Network Start Response command.
 *
 * @param   srcAddr - sender's address
 * @param   pRsp - parsed command
 *
 * @return  ZStatus_t
 */
static ZStatus_t initiatorNwkStartRspCB( afAddrType_t *srcAddr, bdbTLNwkStartRsp_t *pRsp )
{
  if ( touchLink_IsValidTransID( pRsp->transID ) == FALSE )
  {
    return ( ZFailure );
  }

  zstack_touchlinkNwkStartRsp_t *pNwkStartRsp;
  pNwkStartRsp = (zstack_touchlinkNwkStartRsp_t*)zcl_mem_alloc( sizeof(zstack_touchlinkNwkStartRsp_t) );

  pNwkStartRsp->nwkStartRsp.transID = pRsp->transID;
  pNwkStartRsp->nwkStartRsp.status = pRsp->status;
  zcl_cpyExtAddr(pNwkStartRsp->nwkStartRsp.extendedPANID, pRsp->extendedPANID);
  pNwkStartRsp->nwkStartRsp.nwkUpdateId = pRsp->nwkUpdateId;
  pNwkStartRsp->nwkStartRsp.logicalChannel = pRsp->logicalChannel;
  pNwkStartRsp->nwkStartRsp.panId = pRsp->panId;

  Zstackapi_tlInitiatorNwkStartRspInd(tlAppEntity, pNwkStartRsp);
  zcl_mem_free(pNwkStartRsp);
  return ( ZSuccess );
}

/*********************************************************************
 * @fn      initiatorNwkJoinRspCB
 *
 * @brief   This callback is called to process a Network Join Router
 *          Response command.
 *
 * @param   srcAddr - sender's address
 * @param   pRsp - parsed command
 *
 * @return  ZStatus_t
 */
static ZStatus_t initiatorNwkJoinRspCB( afAddrType_t *srcAddr, bdbTLNwkJoinRsp_t *pRsp )
{
  if ( ( touchLink_IsValidTransID( pRsp->transID ) == FALSE ) || ( ( srcAddr->addrMode != afAddr64Bit ) ) )
  {
    return ( ZFailure );
  }

  zstack_touchlinkNwkJoinRsp_t *pNwkJoinRsp;
  pNwkJoinRsp = (zstack_touchlinkNwkJoinRsp_t*)zcl_mem_alloc( sizeof(zstack_touchlinkNwkJoinRsp_t) );

  zcl_memcpy(&pNwkJoinRsp->addr, srcAddr, sizeof(afAddrType_t));
  pNwkJoinRsp->nwkJoinRsp.transID = pRsp->transID;
  pNwkJoinRsp->nwkJoinRsp.status = pRsp->status;

  Zstackapi_tlInitiatorNwkJoinRspInd(tlAppEntity, pNwkJoinRsp);
  zcl_mem_free(pNwkJoinRsp);
  return ( ZSuccess );
}

#endif
/*********************************************************************
*********************************************************************/
