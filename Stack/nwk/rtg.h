/**************************************************************************************************
  Filename:       rtg.h
  Revised:        $Date: 2014-11-18 14:45:15 -0800 (Tue, 18 Nov 2014) $
  Revision:       $Revision: 41167 $

  Description:    Interface to mesh routing functions


  Copyright 2004-2014 Texas Instruments Incorporated. All rights reserved.

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

#ifndef RTG_H
#define RTG_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

#include "zcomdef.h"
#include "nwk_util.h"
#include "nwk_bufs.h"
#include "zglobals.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define RTG_TIMER_INTERVAL    1000

//Route request command option
#define MTO_ROUTE           0x01       // Used in option of NLME_RouteDiscoveryRequest() and rtgTable[]
#define NO_ROUTE_CACHE      0x02       // Used in option of NLME_RouteDiscoveryRequest() and rtgTable[]
#define RTG_RECORD          0x04       // Used in option of rtgTable[]
#define MTO_ROUTE_RC        0x08       // Sender has route cache. Used in option of rtgTable[]
#define MTO_ROUTE_NRC       0x10       // Sender doesn't have route cache. Used in option of rtgTable[]
#define DEST_IEEE_ADDR      0x20       // Used in option of route request command frame
#define MULTICAST_ROUTE     0x40       // Ued in all three places
#define RREQ_OPTIONS_MASK   0x78       // b'0111,1000   0-2, 7 are reserved bits

#define RTG_MTO_DEST_ADDRESS   NWK_BROADCAST_SHORTADDR_DEVZCZR  //0xFFFC

#define RREP_ORI_IEEE       0x10
#define RREP_RES_IEEE       0x20
#define RREP_MULTICAST      0x40
#define RREP_OPTIONS_MASK   (RREP_ORI_IEEE | RREP_RES_IEEE | RREP_MULTICAST)

#define RTG_END_DEVICE_ADDR_TYPE 0
#define RTG_ROUTER_ADDR_TYPE     1

#define RTG_NO_EXPIRY_TIME  0xFF

/*********************************************************************
 * TYPEDEFS
 */

typedef enum
{
  RTG_SUCCESS,
  RTG_FAIL,
  RTG_TBL_FULL,
  RTG_HIGHER_COST,
  RTG_NO_ENTRY,
  RTG_INVALID_PATH,
  RTG_INVALID_PARAM,
  RTG_SRC_TBL_FULL
} RTG_Status_t;

// status values for routing entries
#define RT_INIT       0
#define RT_ACTIVE     1
#define RT_DISC       2
#define RT_LINK_FAIL  3
#define RT_REPAIR     4

// Routing table entry
//   Notice, if you change this structure, you must also change
//   rtgItem_t in zd_profile.h
typedef struct
{
  uint16_t  dstAddress;
  uint16_t  nextHopAddress;
  byte    expiryTime;
  byte    status;
  uint8_t   options;
} rtgEntry_t;

// Route discovery table entry
typedef struct
{
  byte    rreqId;
  uint16_t  srcAddress;
  uint16_t  previousNode;
  byte    forwardCost;
  byte    residualCost;
  byte    expiryTime;
} rtDiscEntry_t;

// Broadcast table entry.
typedef struct
{
  uint16_t srcAddr;
  uint8_t  bdt; // broadcast delivery time
  uint8_t  pat; // passive ack timeout
  uint8_t  mbr; // max broadcast retries
  uint8_t  handle;
  // Count of non-sleeping neighbors and router children.
  uint8_t  ackCnt;
  uint8_t  id;
} bcastEntry_t;

// Source routing table
typedef struct
{
  uint8_t    expiryTime;
  uint8_t    relayCount;
  uint16_t   dstAddress;
  uint16_t*  relayList;
} rtgSrcEntry_t;
/*********************************************************************
 * GLOBAL VARIABLES
 */

extern rtgEntry_t rtgTable[];
extern rtDiscEntry_t rtDiscTable[];

extern rtgSrcEntry_t rtgSrcTable[];

/*********************************************************************
 * FUNCTIONS
 */

extern void RTG_Init( void );

extern rtgEntry_t *RTG_GetRtgEntry( uint16_t DstAddress, uint8_t options);

extern RTG_Status_t RTG_RemoveRtgEntry( uint16_t DstAddress, uint8_t options );

extern uint16_t RTG_GetNextHop( uint16_t DstAddress, uint16_t avoidAddr,
                             uint16_t avoidAddr2, uint16_t avoidAddr3, uint8_t options );

extern byte RTG_ProcessRreq(
           NLDE_FrameFormat_t *ff, uint16_t macSrcAddress, uint16_t *nextHopAddr );

extern void RTG_ProcessRrep( NLDE_FrameFormat_t *ff, uint16_t macSrcAddress );

extern void RTG_ProcessRrec( NLDE_FrameFormat_t *ff );

extern uint8_t RTG_ProcessRErr( NLDE_FrameFormat_t *ff );
extern void RTG_TimerEvent( void );

extern uint16_t RTG_AllocNewAddress( byte deviceType );

extern void RTG_DeAllocTreeAddress( uint16_t shortAddr );

extern void RTG_DeAllocStochasticAddress( uint16_t shortAddr );

extern void RTG_BcastTimerHandler( void );

extern byte RTG_BcastChk( NLDE_FrameFormat_t *ff, uint16_t macSrcAddr );

extern byte RTG_BcastAdd(NLDE_FrameFormat_t*ff, uint16_t macSrcAddr, byte handle);

extern void RTG_BcastDel( byte handle );

extern void RTG_DataReq( OsalPort_EventHdr *inMsg );

extern byte RTG_PoolAdd( NLDE_FrameFormat_t *ff );

extern uint16_t RTG_GetTreeRoute( uint16_t dstAddress );

extern uint16_t RTG_SrcGetNextHop( uint8_t rtgIndex, uint16_t* rtgList);

extern uint8_t RTG_ValidateSrcRtg(uint8_t relayCnt, uint8_t relayIdx, uint16_t* relayList );

extern uint8_t RTG_RtgRecordInitiation( uint16_t DstAddress, uint16_t SrcAddress, uint8_t options);

extern RTG_Status_t RTG_GetRtgSrcEntry( uint16_t dstAddr, uint8_t* pRelayCnt, uint16_t** ppRelayList);
extern RTG_Status_t RTG_CheckRtStatus( uint16_t DstAddress, byte RtStatus, uint8_t options );

extern uint8_t RTG_ProcessRtDiscBits( uint8_t rtDiscFlag, uint16_t dstAddress, uint8_t* pSrcRtgSet, uint8_t options );

extern uint8_t RTG_RouteMaintanence( uint16_t DstAddress, uint16_t SrcAddress, uint8_t options );

extern void RTG_FillCSkipTable( byte *children, byte *routers,
                                byte depth, uint16_t *pTbl );


extern void RTG_SendBrokenRoute( uint16_t nwkSrcAddr, uint16_t nwkDstAddr,
                   uint8_t srcRouteSet, uint16_t macSrcAddr, uint16_t macDstAddr );

extern uint16_t RTG_CalcTreeAddress(  byte deviceType );

extern uint16_t RTG_GetStochastic( byte deviceType );

extern uint16_t RTG_GetNextTreeHop( uint16_t dstAddress );

extern uint16_t RTG_ChildGetNextHop( uint16_t DstAddr );

extern uint8_t RTG_GetAncestors( uint16_t dstAddr, uint16_t ancestorAddr, uint16_t *pRtgDst );

extern void RTG_nextHopIsBad( uint16_t nextHop );

extern ZStatus_t RTG_SendRErr( uint16_t SrcAddress, uint16_t DstAddress, byte ErrorCode );

extern RTG_Status_t RTG_AddSrcRtgEntry_Guaranteed( uint16_t srcAddr, uint8_t relayCnt, uint16_t* pRelayList );

extern void RTG_initRtgTable( void );

extern void RTG_MTORouteReq(void);
extern void RTG_SendNextEDMsg( uint16_t shortAddr, uint8_t sendBroadcast, uint8_t sendNoData );

/*********************************************************************
*********************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* RTG_H */
