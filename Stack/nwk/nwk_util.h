/**************************************************************************************************
  Filename:       nwk_util.h
  Revised:        $Date: 2015-02-16 12:23:39 -0800 (Mon, 16 Feb 2015) $
  Revision:       $Revision: 42629 $

  Description:    Network layer utility functions.


  Copyright 2004-2015 Texas Instruments Incorporated. All rights reserved.

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

#ifndef NWK_UTIL_H
#define NWK_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * INCLUDES
 */
#include "nl_mede.h"
#include "nwk.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * FRAME FORMAT CONSTANTS
 */

// Command identifiers
#define CMD_ID_RREQ                 0x01
#define CMD_ID_RREP                 0x02
#define CMD_ID_NETWORK_STATUS       0x03    // New name for Route Error (CMD_ID_RERR)
#define CMD_ID_LEAVE                0x04
#define CMD_ID_RREC                 0x05
#define CMD_ID_REJOIN_REQ           0x06
#define CMD_ID_REJOIN_RSP           0x07
#define CMD_ID_LINK_STATUS          0x08
#define CMD_ID_NETWORK_REPORT       0x09
#define CMD_ID_NETWORK_UPDATE       0x0A
#define CMD_ID_END_DEV_TIMEOUT_REQ  0x0B
#define CMD_ID_END_DEV_TIMEOUT_RSP  0x0C


#define CMD_ID_NWK_LAYER_ADD_WHITE_LIST_REQ     0xC0
#define CMD_ID_NWK_LAYER_REMOVE_WHITE_LIST_REQ  0xC1


// header fields
#define NWK_HDR_FRAME_CTRL_LSB 0
#define NWK_HDR_FRAME_CTRL_MSB 1
#define NWK_HDR_DST_ADDR_LSB   2
#define NWK_HDR_DST_ADDR_MSB   3
#define NWK_HDR_SRC_ADDR_LSB   4
#define NWK_HDR_SRC_ADDR_MSB   5
#define NWK_HDR_RADIUS         6
#define NWK_HDR_SEQ_NUM        7
#define NWK_HDR_LEN            8

// optional header fields
#define NWK_HDR_DST_EXTADDR_LEN        Z_EXTADDR_LEN
#define NWK_HDR_SRC_EXTADDR_LEN        Z_EXTADDR_LEN
#define NWK_HDR_MULTICAST_CTRL_LEN     1
#define NWK_HDR_SRC_ROUTE_SUBFRAME_LEN 0
#define NWK_HDR_SRCRTG_CNT_LEN         1

// frame control fields
#define NWK_FC_FRAME_TYPE   0
#define NWK_FC_PROT_VERSION 2
#define NWK_FC_DISC_ROUTE   6
#define NWK_FC_MULTICAST    8
#define NWK_FC_SECURE       9
#define NWK_FC_SRC_ROUTE    10
#define NWK_FC_DST_EXTADDR  11
#define NWK_FC_SRC_EXTADDR  12
#define NWK_FC_END_DEV_INIT 13
#define NWK_FC_RESERVED     14

// frame control field masks
#define NWK_FC_FRAME_TYPE_MASK   0x03
#define NWK_FC_PROT_VERSION_MASK 0x0F
#define NWK_FC_DISC_ROUTE_MASK   0x03
#define NWK_FC_MULTICAST_MASK    0x01
#define NWK_FC_SECURE_MASK       0x01
#define NWK_FC_SRC_ROUTE_MASK    0x01
#define NWK_FC_DST_EXTADDR_MASK  0x01
#define NWK_FC_SRC_EXTADDR_MASK  0x01
#define NWK_FC_END_DEV_INIT_MASK 0x01
#define NWK_FC_RESERVED_MASK     0x03

// Frame Type sub-field
#define DATA_FRAME_TYPE           0x00
#define CMD_FRAME_TYPE            0x01
#define STUB_NWK_FRAME_TYPE       0x03

#define NWK_FC_ROUTE_DISC_ENABLED 0x01

// Network command fields
#define NWK_CMD_LEAVE_OPTIONS 1
#define NWK_CMD_LEAVE_SIZE    2
#define NWK_CMD_LEAVE_RJ      0x20 // rejoin
#define NWK_CMD_LEAVE_REQ     0x40 // request(1)/indication(0)
#define NWK_CMD_LEAVE_RC      0x80 // remove children
#define NWK_CMD_LEAVE_OPTIONS_RESERVE 0x1F

#define NWK_CMD_REJOIN_REQ_SIZE 2
#define NWK_CMD_REJOIN_RSP_SIZE 4

// Command lengths
#define NSDU_SIZE_RREQ  6
#define NSDU_SIZE_RREQ_EXT  NSDU_SIZE_RREQ + 8
#define NSDU_SIZE_RREP  8
#define NSDU_SIZE_NETWORK_STATUS  4
#define NSDU_SIZE_NETWORK_STATUS_INVALID_CMD  5
#define NSDU_SIZE_LINK_STATUS_DEFAULT 2
#define NSDU_SIZE_NETWORK_REPORT      10
#define NSDU_SIZE_NETWORK_UPDATE      11
#define NSDU_SIZE_END_DEV_TIMEOUT_REQ 3
#define NSDU_SIZE_END_DEV_TIMEOUT_RSP 3

#define NWK_AUX_HDR_LEN  14
// Status Codes for Network Status Command Frame
#define NWKSTAT_NO_ROUTE_AVAIL             0x00
#define NWKSTAT_TREE_LINK_FAILURE          0x01
#define NWKSTAT_NONTREE_LINK_FAILURE       0x02
#define NWKSTAT_LOW_BATTERY_LEVEL          0x03
#define NWKSTAT_NO_ROUTING_CAPACITY        0x04
#define NWKSTAT_NO_INDIRECT_CAPACITY       0x05
#define NWKSTAT_INDIRECT_TRANS_EXPIRY      0x06
#define NWKSTAT_TARGET_DEVICE_UNAVAIL      0x07
#define NWKSTAT_TARGET_ADDRESS_UNALLOCATED 0x08
#define NWKSTAT_PARENT_LINK_FAILURE        0x09
#define NWKSTAT_VALIDATE_ROUTE             0x0a
#define NWKSTAT_SOURCE_ROUTE_FAILURE       0x0b
#define NWKSTAT_MANY_TO_ONE_ROUTE_FAILURE  0x0c
#define NWKSTAT_ADDRESS_CONFLICT           0x0d
#define NWKSTAT_VERIFY_ADDRESSES           0x0e
#define NWKSTAT_PANID_UPDATE               0x0f
#define NWKSTAT_NWK_ADDRESS_UPDATE         0x10
#define NWKSTAT_BAD_FRAME_COUNT            0x11
#define NWKSTAT_BAD_KEY_SEQ_NUMBER         0x12
#define NWKSTAT_UNKNOWN_COMMAND            0x13

// Link Status Option Field definitions
#define LS_OPTION_LAST_FRAME                0x40
#define LS_OPTION_FIRST_FRAME               0x20
#define LS_OPTION_ENTRY_COUNT_MASK          0x1F
#define LS_OPTION_RESERVED                  0x80

#define LS_ENTRY_COST_MASK                  0x07
#define LS_ENTRY_TXCOST_SHIFT               4
#define LS_ENTRY_RESERVED                   0x88

#define LS_TABLE_SIZE                       3

// Multicast Control Fields
#define NWK_MC_MODE_MASK                    0x03
#define NWK_MC_MODE_NONMEMBER               0x00
#define NWK_MC_MODE_MEMBER                  0x01
#define NWK_MC_RADIUS_MASK                  0x1C
#define NWK_MC_RADIUS_SHIFT                 2
#define NWK_MC_MAXRADIUS_MASK               0xE0
#define NWK_MC_MAXRADIUS_SHIFT              5
#define NWK_MC_RESET_RADIUS_SHIFT           (NWK_MC_MAXRADIUS_SHIFT - NWK_MC_RADIUS_SHIFT)
#define NWK_MC_INFINITE_RADIUS              7

// Report Types for Network Report Command Frame
#define NWKREPORT_PANID_CONFLICT            0x00

// Network Report Command Options Field bit mask
#define NWKREPORT_INFO_CNT                  0x1F
#define NWKREPORT_CMD_ID                    0xE0

// Update Types for Network Update Command Frame
#define NWKUPDATE_PANID_UPDATE              0x00

// Network Update Command Options Field bit mask
#define NWKUPDATE_INFO_CNT                  0x1F
#define NWKUPDATE_CMD_ID                    0xE0

// End Device Timeout Request Timeout values
#define ENDDEV_REQ_MIN_TIMEOUT_VALUE        0x00
#define ENDDEV_REQ_MAX_TIMEOUT_VALUE        0x0E

// End Device Timeout Request Configuration mask, for R21 0x00 is the only valid value
#define ENDDEV_REQ_END_DEV_CFG_INVALID_MASK   0xFF

// End Device Timeout Response status values
#define ENDDEV_RSP_STATUS_SUCCESS           0x00
#define ENDDEV_RSP_STATUS_INCORRECT_VALUE   0x01


/*********************************************************************
 * TYPEDEFS
 */
typedef struct
{
  uint16_t dstAddr;
  uint8_t* extAddr;
  uint8_t  rejoin;
  uint8_t  request;
  uint8_t  removeChildren;
  uint8_t  cnf;
} NLME_LeaveCmd_t;

typedef struct
{
  uint16_t devAddr;
  uint8_t  txCost;
  uint8_t  rxCost;
} linkStatusListItem_t;

// Used for ED Scan Confirm message
typedef struct
{
  uint8_t status;
  uint32_t scannedChannels;
  uint8_t *energyDetectList;
} NLME_EDScanConfirm_t;

// Used for Channel Interference detection message
typedef struct
{
  uint16_t totalTransmissions;
  uint16_t txFailures;
} NLME_ChanInterference_t;



/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTION PROTOTYPES
 */

/*
 * Build a NWK data indication and send to the next higher layer.
 */
extern ZStatus_t NLDE_DataIndSend( NLDE_DataReq_t* req );

/*
 * Build a NWK data service frame and send to the MAC.
 */
extern ZStatus_t NLDE_DataReqSend( NLDE_DataReq_t* req );

/*
 * Send an msdu
 */
extern ZStatus_t NLDE_SendMsg( uint8_t* msdu, uint16_t nextHopAddr, uint16_t macSrcAddr,
                               uint8_t msduLength, uint8_t nsduHandle,
                               uint16_t nsduHandleOptions,
                               uint8_t apsRetries,
                               nwkDB_UserData_t* ud );

/*
 * Call this function to parse an incoming message.
 */
extern uint8_t NLDE_ParseMsg( byte *buf, byte bufLength, NLDE_FrameFormat_t *ff );

/*
 * Updates entry in the neighbor table
 */
extern void RTG_UpdateNeighborEntry( uint16_t nodeAddress, uint16_t panId,
                                     uint8_t linkQuality, uint8_t status, uint8_t sent);

/*
 * Remove all entries with short address only.
 */
extern void nwkNeighborRemoveAllStranded( void );

extern void NLME_SetAssocFlags( void );

extern void NLME_SetUpdateID( uint8_t updateID );
/*
 * Send a simulated MAC->NWK Data Confirm message
 */
extern ZStatus_t nwkBuildDataCnf( byte handle, byte status );

/*
 * Send the NWK LEAVE cmd
 */
extern ZStatus_t NLME_LeaveCmdSend( NLME_LeaveCmd_t* cmd );

/*
 * Process the NWK LEAVE cmd
 */
extern uint8_t NLME_LeaveCmdProcess( uint8_t handle, NLDE_FrameFormat_t *ff );

/*
 * Handle NWK commands during MACCB_DATA_CNF_CMD processing
 */
extern void NLME_CmdCnf( NLDE_DataCnf_t* cnf );

/*
 * Check for NWK commands that can legally be broadcast
 */
extern uint8_t NLME_CmdBcast( uint8_t cmdID );

/*
 * Allocate a NLDE_DatatReq_t buffer for NWK commands
 */
extern NLDE_DataReq_t* NLME_CmdDataReqAlloc
                         ( NLDE_DataReqAlloc_t* dra );

/*
 * Stub to load the next higher layer frame data
 */
extern void* NLDE_FrameDataLoad( NLDE_DataReq_t* req );

/*
 * Process Rejoin Rsp command pkt
 */
extern void NLME_RejoinRspCmdProcess( NLDE_FrameFormat_t* ff );

/*
 * Send Rejoin Rsp command pkt
 */
extern ZStatus_t NLME_RejoinRspSend( ZMacAssociateRsp_t *AssocRsp, byte relation, uint8_t type  );

/*
 * Process Rejoin Request command pkt
 */
extern void NLME_RejoinReqCmdProcess( NLDE_FrameFormat_t* ff );

/*
 * Build src routing header frame
 */
extern uint16_t NLDE_BuildSrcRtgFrame( NLDE_SrcFrameFormat_t* sff, uint16_t dstAddr );


/*********************************************************************
 * HELPERS FUNCTION PROTOTYPES
 */

/*
 * Returns a unique handle to identify a packet transaction
 */
extern byte NLDE_GetHandle( void );

extern byte NLME_GetProtocolVersion( void );

extern uint8_t NLME_GetEnergyThreshold( void );

extern void NLME_SetEnergyThreshold( uint8_t value );

extern void NLME_SetBroadcastFilter(byte capabilities);

extern addr_filter_t NLME_IsAddressBroadcast(uint16_t shortAddress);

extern void NLME_RemoveChild( uint8_t* extAddr, uint8_t dealloc );

extern uint8_t nwk_multicast( NLDE_FrameFormat_t* ff );

extern void NLME_NwkStatusAddrConflictProcess( uint16_t addr );

extern uint8_t NLME_ProcessAddressConflictLocal( uint16_t addr );

extern ZStatus_t NLDE_CheckForAddrConflict( NLDE_FrameFormat_t *ff );
extern ZStatus_t NLME_SendNetworkStatus( uint16_t dstAddr,
                            uint16_t statusAddr, uint8_t statusCode, uint8_t forceSeqNum );

extern ZStatus_t NLME_SendNetworkStatusInvalidCmd( uint16_t dstAddr, uint8_t unknownCmdId );

extern uint8_t NLME_ProcessNetworkStatus( NLDE_FrameFormat_t *ff, uint8_t handle );

extern void nwkHandleBrokenRoute( nwkDB_t *rec );

extern uint8_t NLDE_ParseFrameControl( uint16_t fc, NLDE_FrameFormat_t *ff );

// Functions for addressing schemes
extern void NLME_AddressConflictAssignNewStochastic( void );

// Functions pointers for addressing schemes
extern ZStatus_t NLME_SendNetworkReport( uint16_t dstAddr, uint8_t reportType, uint8_t *EPID,
                                         uint8_t reportInfoCnt, uint16_t *panIDs );
extern void NLME_ProcessNetworkReport( NLDE_FrameFormat_t *ff );
extern ZStatus_t NLME_SendNetworkUpdate( uint16_t dstAddr, uint8_t updateType, uint8_t *EPID,
                                         uint8_t updateId, uint16_t newPID );
extern uint8_t NLME_ProcessNetworkUpdate( uint8_t handle, NLDE_FrameFormat_t *ff );

extern ZStatus_t NLME_SendEndDevTimeoutReq( uint16_t dstAddr, uint8_t *destExtAddr,
                                            uint8_t reqTimeout, uint8_t endDevCfg );
extern void NLME_ProcessEndDevTimeoutReq( NLDE_FrameFormat_t *ff );
extern ZStatus_t NLME_SendEndDevTimeoutRsp( uint16_t dstAddr, uint8_t status, uint8_t parentInfo );
extern void NLME_ProcessEndDevTimeoutRsp( NLDE_FrameFormat_t *ff );

extern void nwkAddPanId( uint16_t panID );
extern void nwkProcessPanIdScan( void );
extern void nwkChangePanID( void );
extern void nwkFreePanIdList( void );

extern uint16_t nwkTransmissionFailures( uint8_t reset );

extern uint8_t nwk_ExtPANIDValid( byte *panID );


/****************************************************************************
 * Link Status Functions
 ****************************************************************************/
extern void NLME_InitLinkStatus( void );
extern void NLME_UpdateLinkStatus( void );
extern void NLME_SetLinkStatusTimer( void );
extern ZStatus_t nwkSendLinkStatus( void );
extern ZStatus_t NLME_SendLinkStatus( uint8_t neighbors, uint8_t options,
                              linkStatusListItem_t *pList );
extern void NLME_ProcessLinkStatus( NLDE_FrameFormat_t *ff );

extern uint8_t nwkConvertLqiToCost( uint8_t lqi );






/****************************************************************************
 * Poll Rate management functions
 ****************************************************************************/
//PollRateType

#define POLL_RATE_TYPE_DEFAULT          0x0001  //Default pollrate to be used when no other stack operation is done.
#define POLL_RATE_TYPE_APP_1            0x0002  //Application defined poll rate.
#define POLL_RATE_TYPE_APP_2            0x0004  //Application defined poll rate.
#define POLL_RATE_TYPE_JOIN_REJOIN      0x0008  //Poll rate used by the stack during join/rejoin operations
#define POLL_RATE_TYPE_QUEUED           0x0010  //Poll rate used by the stack when expecting a response or retrieving all queued messages from parent.
#define POLL_RATE_TYPE_RESPONSE         0x0020  //Poll rate used by the stack when expecting a response.

#ifdef ENABLE_POLL_RATE_DEBUG
#define POLL_RATE_DEBUG_1               0x0100
#define POLL_RATE_DEBUG_2               0x0200
#endif
#define POLL_RATE_TYPE_GENERIC_1_SEC    0x1000  //1 second poll rate used by the stack (cannot be modified)
#define POLL_RATE_DISABLED              0x2000  //State to not perform poll rate. This is used by the stack (cannot be modified)
#define POLL_RATE_KEEP_ALIVE            0x4000  //Indicates parent device uses Data Request as keep alive method, so poll still required
#define POLL_RATE_RX_ON_TRUE            0x8000  //Indicates if ZED is RxOnIdle, device will not poll unless Parent request it as keep alive method (cannot be modified)

//Generic Poll rate 1 second
#define GENERIC_POLL_RATE_1_SEC           1000

// to disable a specific poll rate, set it to the MAX value
#define POLL_RATE_MAX                   0xFFFFFFFF

//Structure to control PollRates
typedef struct
{
uint32_t PollRateDefault;
uint32_t PollRateRejoin;
uint32_t PollRateQueued;
uint32_t PollRateResponse;
uint32_t PollRateKeepAlive;
uint32_t PollRateGeneric1;
uint32_t PollRateApp1;
uint32_t PollRateApp2;
#ifdef ENABLE_POLL_RATE_DEBUG
uint32_t PollRateDebug1;
uint32_t PollRateDebug2;
#endif
uint32_t *pCurrentPollRate;
uint16_t PollRateControl;
}nwk_pollRateTable_t;

extern nwk_pollRateTable_t nwk_pollRateTable;


/*
 * @brief       Initialize the default values of the poll rates used by the stack
 */
void nwk_InitializeDefaultPollRates(void);

/*
 * @brief       Configure the pollRate values per procedure requirements.
 */
void nwk_SetConfigPollRate(uint16_t pollRateType, uint32_t Rate );

/*
 * @brief       Get the pollrate configured for the first pollrate type specified in pollRateType.
 */
uint32_t nwk_GetConfigPollRate(uint16_t pollRateType );

/*
 * @brief       Get the current pollrate in use and the bitmask of pollrates active.
 */
uint16_t nwk_GetCurrentPollRateType(uint32_t *pollRate);

/*
 * @brief       Enable/Disable pollRate for certain procedures.
 */
void nwk_SetCurrentPollRateType(uint16_t pollRateType, uint8_t Enable);


/****************************************************************************
 * Neighbor Table Functions
 ****************************************************************************/
#define NWKNEIGHBOR_NOT_FOUND   0x00   // device not found
#define NWKNEIGHBOR_FOUND_SHORT 0x0A   // found matching short, but
                                       //   extended address doesn't match
#define NWKNEIGHBOR_FOUND_IEEE  0xA0   // found matching extended, but
                                       //   short address doesn't match
#define NWKNEIGHBOR_FOUND       0xAA   // found matching extended and short

// Neighbor table entry
typedef struct
{
  uint16_t  neighborAddress;
  uint8_t   neighborExtAddr[Z_EXTADDR_LEN];
  uint16_t  panId;
  uint8_t   age;          // number of nwkLinkStatusPeriod since last link status
  linkInfo_t linkInfo;
} neighborEntry_t;

extern neighborEntry_t  neighborTable[];

extern neighborEntry_t *nwkNeighborFind( uint16_t nwkAddr, uint16_t panID );
extern neighborEntry_t *nwkNeighborFindEmptySlot( void );
extern uint16_t nwkNeighborCount( void );
extern void nwkNeighborRemove( uint16_t NeighborAddress, uint16_t PanId );
extern neighborEntry_t *nwkNeighborGetWithIndex( uint16_t Index );
extern void nwkNeighborInitTable( void );
extern void nwkNeighborClearEntry( neighborEntry_t *entry );
extern void nwkNeighborKeepPANID( uint16_t PanID );
extern linkInfo_t *nwkNeighborGetLinkInfo( uint16_t NeighborAddress, uint16_t panId );
extern byte nwkNeighborCheckAddr( uint16_t nwkAddr, uint8_t *extAddr );
extern void nwkNeighborUpdateAge( void );
extern uint16_t nwkNeighborFindRandom( uint16_t skipAddr, uint16_t skipAddr2, uint16_t skipAddr3 );
extern void nwkNeighborUpdateNwkAddr( uint16_t nwkAddr, uint8_t *ieeeAddr );
extern void nwkNeighborAdd( uint16_t nwkAddr, uint16_t panId, uint8_t linkQuality );
extern uint8_t nlmeConflictedAddr( uint16_t addr );


/*********************************************************************
 * Frequency Agility Functions
 ********************************************************************/

extern void (*pNwkFreqAgilityScanCfg)( ZMacScanCnf_t *param );
extern void (*pNwkFreqAgilityIncTotalTx)( void );
extern void (*pNwkFreqAgilityIncErrorCnt)( linkInfo_t *linkInfo, uint8_t status );

extern void NwkFreqAgilityInit( void );

/****************************************************************************
 * Not My Child Table Functions
 ****************************************************************************/
// Child Table Mgmt message
typedef struct
{
  OsalPort_EventHdr hdr;
  uint16_t shortAddress;
} nwkChildMgmtMsg_t;

// Not My Child structure
typedef struct
{
uint16_t shortAddr;
uint16_t timeoutCounter;
} nwkNotMyChild_t;

extern void NwkInitChildAging( void );

extern nwkNotMyChild_t notMyChildList[];

extern void NwkNotMyChildListInit( void );

extern void NwkNotMyChildListAging( void );

extern uint8_t NwkNotMyChildListAdd( uint16_t childAddr, uint32_t timeoutValue );

extern void NwkNotMyChildListDelete( uint16_t devAddr );

extern void NwkNotMyChildSendLeave( uint16_t dstAddr );

extern void (*pAssocChildAging)( void );
extern uint8_t (*pAssocChildTableUpdateTimeout)( uint16_t nwkAddr );
extern void (*pNwkNotMyChildListInit)( void );
extern void (*pNwkNotMyChildListAging)( void );
extern uint8_t (*pNwkNotMyChildListAdd)( uint16_t devAddr, uint32_t timeoutValue );
extern void (*pNwkNotMyChildListDelete)( uint16_t devAddr );
extern void (*pNwkNotMyChildSendLeave)( uint16_t dstAddr );

/****************************************************************************
 * Utility function to copy NV items
 ****************************************************************************/
extern uint8_t nwkCreateDuplicateNV( uint16_t srcId, uint16_t dstId );

/****************************************************************************
****************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* NWK_UTIL_H */


