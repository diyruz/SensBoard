/**************************************************************************************************
  Filename:       nwk_globals.h
  Revised:        $Date: 2015-01-08 16:32:12 -0800 (Thu, 08 Jan 2015) $
  Revision:       $Revision: 41678 $

  Description:    User definable Network Parameters.


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

#ifndef NWK_GLOBALS_H
#define NWK_GLOBALS_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * INCLUDES
 */

#include "zcomdef.h"
#include "nwk_bufs.h"
#include "assoc_list.h"
#include "binding_table.h"
#include "zglobals.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */
#if !defined ( CONFIG_ITEM )
  #define CONFIG_ITEM CONST
#endif

#if !defined ( NWK_LINK_STATUS_PERIOD )
  #define NWK_LINK_STATUS_PERIOD 15       // 15 seconds
#endif
#if !defined ( ZIGBEE_FRAGMENTATION )
  #define ZIGBEE_FRAGMENTATION
#endif

#if ( RFD_RCVC_ALWAYS_ON==TRUE ) || ( ZG_BUILD_RTR_TYPE )
  // The PANID Conflict feature is mandatory for both 2007 and 2007 PRO.
  // So, it will be ON all the time (except sleeping end devices).
  #if !defined ( ZIGBEE_PANID_CONFLICT )
    #define ZIGBEE_PANID_CONFLICT
  #endif
#endif

#if ( ZG_BUILD_RTR_TYPE )
  // The Frequency Agility feature is mandatory for both 2007 and 2007 PRO.
  // So, it will be ON all the time for routers.
  #if !defined ( ZIGBEE_FREQ_AGILITY )
    #define ZIGBEE_FREQ_AGILITY
  #endif
#endif

#if defined ( ZIGBEE_FREQ_AGILITY ) || defined ( ZBIT )
  // Make Frequency Agility to look for all MAC errors for Certification test
  // by setting the following to TRUE. The default value is FALSE meaning
  // that only ZMacChannelAccessFailure error code will trigger a scan.
  #if !defined ( NWK_FREQ_AGILITY_ALL_MAC_ERRS )
    #define NWK_FREQ_AGILITY_ALL_MAC_ERRS  FALSE
  #endif
#endif

// Controls various stack parameter settings
#define ZIGBEEPRO_PROFILE     2

// Channel mask
#define MAX_CHANNELS_868MHZ     0x00000001
#define MAX_CHANNELS_915MHZ     0x000007FE

#if !defined ( MAX_CHANNELS_24GHZ )
  #define MAX_CHANNELS_24GHZ    0x07FFF800
#endif

#define STACK_PROFILE_ID      ZIGBEEPRO_PROFILE

//Z-Stack does not support other than ZigbeePro_Profile.
//These parameters must not be modified to keep Zigbee Core compliance.
#if ( STACK_PROFILE_ID == ZIGBEEPRO_PROFILE )
    #define MAX_NODE_DEPTH      20
    #define USE_NWK_SECURITY    1   // true or false
    #define SECURITY_LEVEL      5
#endif

// Zigbee protocol version
#define ZB_PROT_V1_0                 1
#define ZB_PROT_V1_1                 2

#define ZB_PROT_VERS      ZB_PROT_V1_1
#define ZIGBEE_PROT_ID            0x00

// Status and error codes for extra information
#define NWK_STATUS_PING_RCVD            0x0001
#define NWK_STATUS_ASSOC_CNF            0x0002
#define NWK_STATUS_ED_ADDR              0x0003
#define NWK_STATUS_PARENT_ADDR          0x0004
#define NWK_STATUS_COORD_ADDR           0x0005
#define NWK_STATUS_ROUTER_ADDR          0x0006
#define NWK_STATUS_ORPHAN_RSP           0x0007

#define NWK_ERROR_ASSOC_RSP             0x1001
#define NWK_ERROR_ASSOC_RSP_MF          0x1002
#define NWK_ERROR_ASSOC_CNF_DENIED      0x1003
#define NWK_ERROR_ENERGY_SCAN_FAILED    0x1004

// Indicate whether incoming NWK frames must be all checked for freshness
// when the memory for incoming frame counts is exceeded
#if !defined( NWK_ALL_FRESH )
  #define NWK_ALL_FRESH TRUE  // Default to TRUE - Check all frames
#endif

// Indicate whether to use Multicast in NIB value
#if !defined( MULTICAST_ENABLED )
  #define MULTICAST_ENABLED TRUE  // Default to TRUE
#endif

// Maximum number in tables
#if !defined( NWK_MAX_DEVICE_LIST )
#define NWK_MAX_DEVICE_LIST     20  // Maximum number of devices in the
                                    // Assoc/Device list.
#endif

// Don't change this value to set the number of devices.  Change
//  NWK_MAX_DEVICE_LIST above
#define NWK_MAX_DEVICES   ( NWK_MAX_DEVICE_LIST + 1 )    // One extra space for parent

// Number of End Devices that will be stored in the SrcMatch and NotMyChildList lists
// when aged out by the Child Table Management process
#define MAX_NOT_MYCHILD_DEVICES  5

// Number of reserved places for router and end device children, to be used in stochastic addressing.
#if !defined ( NWK_MIN_ROUTER_CHILDREN )
  #define NWK_MIN_ROUTER_CHILDREN          0
#endif
#if !defined ( NWK_MIN_ENDDEVICE_CHILDREN )
  #define NWK_MIN_ENDDEVICE_CHILDREN       0
#endif

#if !defined ( MAX_NEIGHBOR_ENTRIES )
  #if ( ZG_BUILD_RTR_TYPE )
    #define MAX_NEIGHBOR_ENTRIES    16
  #else
    #define MAX_NEIGHBOR_ENTRIES    4
  #endif
#endif

#if !defined ( APS_MAX_GROUPS )
  #define APS_MAX_GROUPS  10
#endif

#define NWK_MAX_REFLECTOR_ENTRIES ( NWK_MAX_BINDING_ENTRIES )

#if !defined( MAX_BCAST )
  #define MAX_BCAST 9
#endif

// Maxiumum number of secure partners(Commercial mode only).
#if ZG_BUILD_COORDINATOR_TYPE
// Adding 5 entries to allow up to 5 legacy devices join concurrently when the rest of the
// table is filled with ZigBee 3.0 devices, binding table related addresses, association
// table related addresses, etc. the usage of these 5 entries is just temporary during joining
// of the legacy devices. A few seconds (BDB_DEFAULT_TC_NODE_JOIN_TIMEOUT) after they joined,
// these entries are released and can be used for joining more legacy devices.
  #define NWK_MAX_SECURE_PARTNERS (5 + ZDSECMGR_TC_DEVICE_MAX)
#else
// Add 1 for the Trust Center(Coordinator) if it is not the parent.
  #define NWK_MAX_SECURE_PARTNERS 1
#endif

// Maximum number of addresses managed by the Address Manager
#define NWK_MAX_ADDRESSES (uint16_t)                          \
                          ( ( NWK_MAX_DEVICES           ) +   \
                            ( NWK_MAX_REFLECTOR_ENTRIES ) +   \
                            ( NWK_MAX_SECURE_PARTNERS   )   )

// Network PAN Coordinator Address
#define NWK_PAN_COORD_ADDR 0x0000

// Network Addressing modes
#define NWK_ADDRESSING_DISTRIBUTED    0x00
#define NWK_ADDRESSING_STOCHASTIC     0x02

#if !defined ( NWK_LINK_STATUS_PERIOD )
  #define NWK_LINK_STATUS_PERIOD 15       // 15 seconds
#endif

#if !defined ( LINK_STATUS_JITTER_MASK )
  #define LINK_STATUS_JITTER_MASK 0x007F   // 127 milliseconds
#endif

#if !defined ( NWK_ROUTE_AGE_LIMIT )
  #define NWK_ROUTE_AGE_LIMIT 3       // 3 missed link satus frames
#endif

#if !defined ( MAX_RTG_SRC_ENTRIES )
  #define MAX_RTG_SRC_ENTRIES 12
#endif

#if !defined ( SRC_RTG_EXPIRY_TIME )
  #define SRC_RTG_EXPIRY_TIME 10   // seconds before the source route entry expires
#endif

#if !defined ( MTO_RREQ_LIMIT_TIME )
  // in milliseconds. The time limited to one MTO RReq (Concentrator Announce)
  #define MTO_RREQ_LIMIT_TIME 1000
#endif

#if !defined ( MTO_ROUTE_EXPIRY_TIME )
  // The number of seconds a MTO routing entry will last. Default to not expiring.
  #define MTO_ROUTE_EXPIRY_TIME RTG_NO_EXPIRY_TIME
#endif

#if ( ZG_BUILD_ENDDEVICE_TYPE )
  #define APS_MAX_ENDDEVICE_BROADCAST_ENTRIES   ( 2 * MAX_BCAST )
#endif

#if !defined( NWK_CONFLICTED_ADDR_EXPIRY_TIME )
  #define NWK_CONFLICTED_ADDR_EXPIRY_TIME  4    // number of link status period after the
                                                // last received address conflict
                                                // report (network status command)
#endif

#if !defined ( LINK_DOWN_TRIGGER )
  #define LINK_DOWN_TRIGGER          3   // Link is down if txCounter exceeds this
#endif

#if !defined ( GOOD_LINK_COST )
  #define GOOD_LINK_COST             ( MAX_LINK_COST / 2 )   // 3 is considered good cost
#endif

// This the size of the conflicted address table
// Scale it up if the size of the network is over 100 nodes
#if !defined( CONFLICTED_ADDR_TABLE_SIZE )
  #define CONFLICTED_ADDR_TABLE_SIZE   3
#endif

// Maximum number of relays in source routing
#if !defined ( MAX_SOURCE_ROUTE )
  #define MAX_SOURCE_ROUTE             12
#endif

#if !defined ( MAX_BROADCAST_QUEUED )
  #define MAX_BROADCAST_QUEUED 10
#endif

#if !defined ( DEFAULT_ROUTE_REQUEST_RADIUS )
  #define DEFAULT_ROUTE_REQUEST_RADIUS DEF_NWK_RADIUS
#endif

#if !defined ( DEF_NWK_RADIUS )
  // the default network radius set twice the value of <nwkMaxDepth>
  #define DEF_NWK_RADIUS           ( 2 * BEACON_MAX_DEPTH )
#endif

#if !defined ( MAX_PASSIVE_ACK_CNT )
  #define MAX_PASSIVE_ACK_CNT 8
#endif

//Define the number of network security material entries that this device can have.
//The first MAX_NWK_SEC_MATERIAL_TABLE_ENTRIES-1 networks will be stored, while the last
//will be used for the remaining networks joined
#define MAX_NWK_SEC_MATERIAL_TABLE_ENTRIES   5

/*********************************************************************
 * TYPEDEFS
 */
#if ( MAX_NEIGHBOR_ENTRIES > 255 ) || defined ( LARGE_NEIGHBOR_TABLE )
  typedef uint16_t neighborTableIndex_t;
#else
  typedef uint8_t neighborTableIndex_t;
#endif

#if ( NWK_MAX_DEVICES > 255 ) || defined ( LARGE_DEVICE_TABLE )
  typedef uint16_t deviceTableIndex_t;
#else
  typedef uint8_t deviceTableIndex_t;
#endif

#if ( MAX_RTG_ENTRIES > 255 ) || defined ( LARGE_ROUTING_TABLE )
  typedef uint16_t rtgTableIndex_t;
#else
  typedef uint8_t rtgTableIndex_t;
#endif

#if ( MAX_RTG_SRC_ENTRIES > 255 ) || defined ( LARGE_SOURCE_ROUTE_TABLE )
  typedef uint16_t srcRtgTableIndex_t;
#else
  typedef uint8_t srcRtgTableIndex_t;
#endif

#if ( MAX_RREQ_ENTRIES > 255 ) || defined ( LARGE_ROUTE_REQUEST_TABLE )
  typedef uint16_t rreqTableIndex_t;
#else
  typedef uint8_t rreqTableIndex_t;
#endif

#if ( NWK_MAX_BINDING_ENTRIES > 255 ) || defined ( LARGE_BINDING_TABLE )
  typedef uint16_t bindTableIndex_t;
#else
  typedef uint8_t bindTableIndex_t;
#endif

#if ( MAX_BCAST > 255 ) || defined ( LARGE_BROADCAST_TABLE )
  typedef uint16_t bcastTableIndex_t;
#else
  typedef uint8_t bcastTableIndex_t;
#endif

#if defined ( FEATURE_MNP )
  #if !defined ( MAX_MNP_QUEUED )
    #define MAX_MNP_QUEUED 2
  #endif
#endif

/*********************************************************************
 * NWK GLOBAL VARIABLES
 */

// Variables for MAX data buffer levels
extern CONST byte gNWK_MAX_DATABUFS_WAITING;
extern CONST byte gNWK_MAX_DATABUFS_SCHEDULED;
extern CONST byte gNWK_MAX_DATABUFS_CONFIRMED;
extern CONST byte gNWK_MAX_DATABUFS_TOTAL;

extern CONST byte gNWK_INDIRECT_CNT_RTG_TMR;
extern CONST byte gNWK_INDIRECT_MSG_MAX_PER;
extern CONST byte gNWK_INDIRECT_MSG_MAX_ALL;

extern CONFIG_ITEM neighborTableIndex_t gMAX_NEIGHBOR_ENTRIES;

extern CONFIG_ITEM rtgTableIndex_t gMAX_RTG_ENTRIES;
extern CONFIG_ITEM srcRtgTableIndex_t gMAX_RTG_SRC_ENTRIES;
extern CONFIG_ITEM rreqTableIndex_t gMAX_RREQ_ENTRIES;

extern CONST uint16_t gMTO_RREQ_LIMIT_TIME;
extern CONST uint8_t gMTO_ROUTE_EXPIRY_TIME;

extern CONST uint8_t gMAX_PASSIVE_ACK_CNT;

// Variables for MAX list size
extern CONST deviceTableIndex_t gNWK_MAX_DEVICE_LIST;

extern CONST uint8_t gNWK_TREE_ALLOCATE_ROUTERADDR_FOR_ENDDEVICE;

extern CONST uint16_t gNWK_MIN_ROUTER_CHILDREN;
extern CONST uint16_t gNWK_MIN_ENDDEVICE_CHILDREN;

extern byte gMIN_TREE_LQI;

extern CONFIG_ITEM byte defaultKey[];

#if (ZG_BUILD_JOINING_TYPE)
extern CONFIG_ITEM byte distributedDefaultKey[];
#endif

extern CONST byte defaultTCLinkKey[];

extern CONST uint8_t gMAX_SOURCE_ROUTE;
extern uint16_t rtgSrcRelayList[];

extern CONFIG_ITEM bcastTableIndex_t gMAX_BCAST;

extern CONST byte gNWK_CONFLICTED_ADDR_EXPIRY_TIME;

#if defined ( ZIGBEE_FREQ_AGILITY ) || defined ( ZBIT )
extern CONST uint8_t gNWK_FREQ_AGILITY_ALL_MAC_ERRS;
#endif
extern CONST uint8_t gMAX_BROADCAST_QUEUED;

extern CONST uint8_t gLINK_DOWN_TRIGGER;

extern CONST uint8_t gGOOD_LINK_COST;

extern CONST uint8_t gDEFAULT_ROUTE_REQUEST_RADIUS;

extern CONST uint8_t gDEF_NWK_RADIUS;

#if ( ZSTACK_ROUTER_BUILD ) || defined ( ZBIT )
extern CONST uint16_t gLINK_STATUS_JITTER_MASK;
#endif

extern CONST uint8_t gMAX_NOT_MYCHILD_DEVICES;

extern CONST uint32_t timeoutValue[];

extern CONST uint32_t gMAX_NWK_FRAMECOUNTER_CHANGES;

#if defined ( FEATURE_MNP )
  extern uint8_t gMAX_MNP_QUEUED;
#endif


/*********************************************************************
 * APS GLOBAL VARIABLES
 */

// Variables for Binding Table
extern CONFIG_ITEM bindTableIndex_t gNWK_MAX_BINDING_ENTRIES;
extern CONFIG_ITEM uint8_t gMAX_BINDING_CLUSTER_IDS;
extern CONST uint16_t gBIND_REC_SIZE;

extern CONFIG_ITEM uint8_t gAPS_MAX_GROUPS;

extern uint8_t gAPS_MAX_ENDDEVICE_BROADCAST_ENTRIES;

/*********************************************************************
 * GLOBAL VARIABLES - Statistics
 */

#if defined ( FEATURE_PACKET_FILTER_STATS )
  extern uint32_t apsInvalidPackets;
  extern uint32_t apsSecurityFailures;
  extern uint32_t nwkInvalidPackets;
  extern uint32_t nwkSecurityFailures;
#endif

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Init Global Variables
 */
extern void nwk_globals_init( void );
extern void NIB_init( void );

extern void nwk_Status( uint16_t statusCode, uint16_t statusValue );

extern uint16_t nwk_adjustDelay( uint16_t existingDelay, uint8_t confirmStatus, uint16_t bufOptions );

/*********************************************************************
*********************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* NWK_GLOBALS_H */


