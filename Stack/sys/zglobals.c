/**************************************************************************************************
  Filename:       ZGlobals.c
  Revised:        $Date: 2015-10-05 14:56:09 -0700 (Mon, 05 Oct 2015) $
  Revision:       $Revision: 44517 $

  Description:    User definable Z-Stack parameters.


  Copyright 2007-2015 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
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
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */

#include "zcomdef.h"
#include "osal_nv.h"
#include "zd_object.h"
#include "zglobals.h"
#include "zd_nwk_mgr.h"
#include "zd_sec_mgr.h"
#include "bdb.h"
#include "ti_zstack_config.h"
#include "nwk_util.h"
#include "aps_mede.h"
#include "ssp.h"

#if !defined (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE)
#include "zcl_green_power.h"
#endif

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

typedef struct zgItem
{
  uint16_t id;
  uint16_t len;
  void *buf;
} zgItem_t;

/*********************************************************************
 * NWK GLOBAL VARIABLES
 */

// Transmission retries numbers
uint8_t zgMaxDataRetries = NWK_MAX_DATA_RETRIES;
uint8_t zgMaxMissingMacAckLinkFailure = MAX_MISSING_MAC_ACK_LINK_FAILURE;

// Default channel list
uint32_t zgDefaultChannelList = 0;

// Stack profile Id
uint8_t zgStackProfile = STACK_PROFILE_ID;

// Default indirect message holding timeout
uint8_t zgIndirectMsgTimeout = NWK_INDIRECT_MSG_TIMEOUT;

// Secure permit join
uint8_t zgSecurePermitJoin = TRUE;

// if a joiner is performing a trust center rejoin (unsecure), this flag will
// determine if the Trust Center is allowed to encrypt the outgoing NWK key
// using the default TC Link Key. This flag would be checked only in the case where
// the Trust Center does not recognize the device performing a trust center rejoin,
// or if it does recognize it but it does not already have an established unique
// TC Link Key. In this scenario, if this flag is TRUE, the Trust Center will
// encrypt the outgoing NWK Key with the default TC Link Key (ZigbeeAlliance09).
// If this flag is FALSE (default), the Trust Center will not send the NWK Key at all.
uint8_t zgAllowRejoinsWithWellKnownKey = FALSE;

//allowInstallCodes
uint8_t zgAllowInstallCodes = ZG_IC_SUPPORTED_NOT_REQUIRED;
//Allow other devices in the network to change the TC permit joining policy
uint8_t zgAllowRemoteTCPolicyChange = TRUE;

// Trust center address
uint8_t zgApsTrustCenterAddr[Z_EXTADDR_LEN] = { 0 };

// Route Discovery Time - amount of time that a route request lasts
uint8_t zgRouteDiscoveryTime = ROUTE_DISCOVERY_TIME;

// Route expiry
uint8_t zgRouteExpiryTime = ROUTE_EXPIRY_TIME;

// Extended PAN Id
uint8_t zgExtendedPANID[Z_EXTADDR_LEN] = ZDAPP_CONFIG_EPID;

// Broadcast parameters
uint8_t zgMaxBcastRetires   = MAX_BCAST_RETRIES;
uint8_t zgPassiveAckTimeout = PASSIVE_ACK_TIMEOUT;
uint8_t zgBcastDeliveryTime = BCAST_DELIVERY_TIME;

// Many-to-one values
uint8_t zgConcentratorEnable = CONCENTRATOR_ENABLE;
uint8_t zgConcentratorDiscoveryTime = CONCENTRATOR_DISCOVERY_TIME;
uint8_t zgConcentratorRadius = CONCENTRATOR_RADIUS;
uint8_t zgConcentratorRC = CONCENTRATOR_ROUTE_CACHE;   // concentrator with route cache (no memory constraints)
uint8_t zgNwkSrcRtgExpiryTime = SRC_RTG_EXPIRY_TIME;

// Cleanup Child Table according to routing traffic
uint8_t zgRouterOffAssocCleanup = FALSE;

// Determines whether or not a remote NWK leave request command frame received
// by the local device is accepted .
uint8_t zgNwkLeaveRequestAllowed = TRUE;

// Enable or disable processing a leave request confirm generated by local
// device when it sends a leave request to a remote device.
// NOTE: This is only used for testing purposes, do not disable
uint8_t zgNwkProcessLocalLeaveCnf = TRUE;


//=======    Child Aging PARENT ROUTER (ZR/ZC) configuration   ========
// You can setup a router to support Child Table Aging in 1 of 2 modes of
// operation.  The first mode is NWK_PARENT_INFO_END_DEVICE_TIMEOUT_MSG and it
// expects end devices to use End Device timeout message periodically as a means of a keep-alive
// notification to the parent.  The other mode is NWK_PARENT_INFO_MAC_DATA_POLL
// which uses the end device's MAC POLL request as the keep-alive notification.
// The first method is preferred for new devices that will be RxOnIdle = FALSE as
// it does not requires constantly sending data request frames other than needed by polling data from parent.
// The second method is compatible with older end devices without the need for
// specific child aging support.
//
// The method supported by the router (or coordinator) is determined at build time
// by setting zgNwkParentInformation to either NWK_PARENT_INFO_END_DEVICE_TIMEOUT_MSG
// or NWK_PARENT_INFO_MAC_DATA_POLL.
//
// End device built with Child Table Aging support both methods, the method is
// determined by the parent and communicated at run-time.
#if ( ZG_BUILD_RTR_TYPE )
uint8_t zgNwkParentInformation = NWK_PARENT_INFO_MAC_DATA_POLL;
#else
uint8_t zgNwkParentInformation = NWK_PARENT_INFO_UNDEFINED;
#endif

// This is an index into table Requested Timeout Enumerated Values.
// It is used by the parent router, it indicates the default timeout value
// for any end device that does not negotiate a different timeout value
uint8_t zgNwkEndDeviceTimeoutDefault = END_DEV_TIMEOUT_VALUE;

// Index into table Requested Timeout Enumerated Values.
// Used to keep the leave message into MAC queue for child devices that has expired
uint8_t zgNwkEndDeviceLeaveTimeoutDefault = NWK_END_DEVICE_LEAVE_TIMEOUT;

//Number of keep alive messages that the stack will send to parent per timeout, to ensure it wont get expired.
uint8_t zgEndDeviceKeepAliveMessagesPerTimeout = END_DEVICE_KEEP_ALIVE_MESSAGES_PER_TIMEOUT;
//=====================================================================

//==========    Child Aging END DEVICE configuration    ===============
// Values used by End Device when sending End Device Timeout Request
uint8_t zgEndDeviceTimeoutValue  = END_DEV_TIMEOUT_VALUE;
uint8_t zgEndDeviceConfiguration = END_DEV_CONFIGURATION;


//=====================================================================

// Determines if the Child Aging Table Management process is active or not.
// This feature is mandatory for certification with Zigbee PRO 2015+ and
// is enabled by default.
//
// NOTICE:  Before enabling Child Aging make sure to review all the related
// definitions in this file, especially zgNwkParentInformation.
uint8_t zgChildAgingEnable = TRUE;

//==========    TouchLink NWK configuration    ===============
// Values used by Router when starts a network as initiator
uint8_t zTouchLinkNwkStartRtr = FALSE;

//==========    Allow radio to be turned off    ===============
uint8_t zgAllowRadioRxOff = TRUE;

/*********************************************************************
 * APS GLOBAL VARIABLES
 */

// The maximum number of retries allowed after a transmission failure
uint8_t zgApscMaxFrameRetries = APSC_MAX_FRAME_RETRIES;

// The maximum number of seconds (milliseconds) to wait for an
// acknowledgement to a transmitted frame.

// This number is used by polled devices.
uint16_t zgApscAckWaitDurationPolled = APSC_ACK_WAIT_DURATION_POLLED;

// This number is used by non-polled devices in the following formula:
//   (100 mSec) * (_NIB.MaxDepth * zgApsAckWaitMultiplier)
uint8_t zgApsAckWaitMultiplier = 2;

// The maximum number of milliseconds for the end device binding
uint16_t zgApsDefaultMaxBindingTime = APS_DEFAULT_MAXBINDING_TIME;

// The 64-big identifier of the network to join or form.
// Default set to all zeros
uint8_t zgApsUseExtendedPANID[Z_EXTADDR_LEN] = {00,00,00,00,00,00,00,00};

// A boolean flag that indicates whether it is OK to use insecure join
// on startup. Default set to TRUE
uint8_t zgApsUseInsecureJoin = TRUE;

// The radius of broadcast multicast transmissions
uint8_t zgApsNonMemberRadius = APS_DEFAULT_NONMEMBER_RADIUS;

// Commissioned Network Address
uint16_t zgNwkCommissionedNwkAddr = INVALID_NODE_ADDR;

// APS Duplication Rejection table variables
uint16_t zgApscDupRejTimeoutInc = DEFAULT_APS_DUP_REJ_TIMEOUT_INCREMENT;
uint8_t  zgApscDupRejTimeoutCount = DEFAULT_APS_DUP_REJ_TIMEOUT;
uint16_t zgApsMinDupRejTableSize = APS_DUP_REJ_ENTRIES;



/*********************************************************************
 * SECURITY GLOBAL VARIABLES
 */

// If TRUE, preConfigKey should be configured on all devices on the network
// If false, it is configured only on the coordinator and sent to other
// devices upon joining.
uint8_t zgPreConfigKeys = FALSE;

// The type of link key in use.  This will determine the security
// policies associated with sending and receiving APS messages.
// If ZG_GLOBAL_LINK_KEY APS TCLK security may be used for specific APS commands
// If ZG_UNIQUE_LINK_KEY APS TCLK security is required for specific APS commands
uint8_t zgApsLinkKeyType = ZG_GLOBAL_LINK_KEY;

// With changes introduced in R20 of the ZigBee specification,
// boolean value of zgUseDefaultTCLK is set depending on zgApsLinkKeyType value.
//
// For zgApsLinkKeyType = ZG_GLOBAL_LINK_KEY, zgUseDefaultTCLK = TRUE
// For zgApsLinkKeyType = ZG_UNIQUE_LINK_KEY, different devices have
// different value:
//      ZC should have             zgUseDefaultTCLK = FALSE
//      Other devices should have  zgUseDefaultTCLK = TRUE
// This is initialized in zgInitItems()
// If ZG_UNIQUE_LINK_KEY, individual trust center link key between each device
// and the trust center should be manually configured via MT_SYS_OSAL_NV_WRITE
uint8_t zgUseDefaultTCLK;

#if defined ( APP_TP2_TEST_MODE )
uint8_t guTxApsSecON = TP_GU_BOTH;
uint8_t guEnforceRxApsSec = TP_GU_ALL;
#endif

uint8_t zgApsAllowR19Sec = FALSE;
uint8_t zgSwitchCoordKey = FALSE;
uint8_t zgSwitchCoordKeyIndex = 0;

/*********************************************************************
 * ZDO GLOBAL VARIABLES
 */

// Configured PAN ID
uint16_t zgConfigPANID = ZDAPP_CONFIG_PAN_ID;

// Device Logical Type
uint8_t zgDeviceLogicalType = DEVICE_LOGICAL_TYPE;

// Startup Delay
uint8_t zgStartDelay = START_DELAY;

#if !defined MT_TASK
// Flag to use verbose (i.e. "cc2480-style") direct MT callbacks in ZDProfile.c, ZDP_IncomingData().
uint8_t zgZdoDirectCB = TRUE;
#endif

// Min number of attempted transmissions for Channel Interference detection
uint8_t zgNwkMgrMinTransmissions = ZDNWKMGR_MIN_TRANSMISSIONS;

/*********************************************************************
 * BDB GLOBAL VARIABLES
 */

// Determines whether or not we will attempt to perform unsecure rejoins
uint8_t zgBdbAttemptUnsecureRejoin = BDB_ATTEMPT_UNSECURE_REJOIN;

// number of secure rejoin attempts will perform before trying
// unseure/trust center rejoin
uint8_t zgBdbMaxSecureRejoinAttempts = BDB_MAX_SECURE_REJOIN_ATTEMPTS;

/*********************************************************************
 * APPLICATION GLOBAL VARIABLES
 */

// Network Manager Mode
uint8_t zgNwkMgrMode = ZDNWKMGR_ENABLE;

/*********************************************************************
 * NON-STANDARD GLOBAL VARIABLES
 */

// Simple API Endpoint
uint8_t zgSapiEndpoint = SAPI_ENDPOINT;

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * ZGlobal Item Table
 */
static CONST zgItem_t zgItemTable[] =
{
#if defined ( NV_INIT )
#if !defined MT_TASK
  {
    ZCD_NV_ZDO_DIRECT_CB, sizeof(zgZdoDirectCB), &zgZdoDirectCB
  },
#endif
  {
    ZCD_NV_LOGICAL_TYPE, sizeof(zgDeviceLogicalType), &zgDeviceLogicalType
  },
  {
    ZCD_NV_POLL_RATE, sizeof(nwk_pollRateTable), &nwk_pollRateTable
  },
  {
    ZCD_NV_DATA_RETRIES, sizeof(zgMaxDataRetries), &zgMaxDataRetries
  },
  {
    ZCD_NV_POLL_FAILURE_RETRIES, sizeof(zgMaxMissingMacAckLinkFailure), &zgMaxMissingMacAckLinkFailure
  },
  {
    ZCD_NV_CHANLIST, sizeof(zgDefaultChannelList), &zgDefaultChannelList
  },
  {
    ZCD_NV_SCAN_DURATION, sizeof(zgDefaultStartingScanDuration), &zgDefaultStartingScanDuration
  },
  {
    ZCD_NV_STACK_PROFILE, sizeof(zgStackProfile), &zgStackProfile
  },
  {
    ZCD_NV_INDIRECT_MSG_TIMEOUT, sizeof(zgIndirectMsgTimeout), &zgIndirectMsgTimeout
  },
  {
    ZCD_NV_ROUTE_EXPIRY_TIME, sizeof(zgRouteExpiryTime), &zgRouteExpiryTime
  },
  {
    ZCD_NV_EXTENDED_PAN_ID, Z_EXTADDR_LEN, zgExtendedPANID
  },
  {
    ZCD_NV_BCAST_RETRIES, sizeof(zgMaxBcastRetires), &zgMaxBcastRetires
  },
  {
    ZCD_NV_PASSIVE_ACK_TIMEOUT, sizeof(zgPassiveAckTimeout), &zgPassiveAckTimeout
  },
  {
    ZCD_NV_BCAST_DELIVERY_TIME, sizeof(zgBcastDeliveryTime), &zgBcastDeliveryTime
  },
  {
    ZCD_NV_CONCENTRATOR_ENABLE, sizeof(zgConcentratorEnable), &zgConcentratorEnable
  },
  {
    ZCD_NV_CONCENTRATOR_DISCOVERY, sizeof(zgConcentratorDiscoveryTime), &zgConcentratorDiscoveryTime
  },
  {
    ZCD_NV_CONCENTRATOR_RADIUS, sizeof(zgConcentratorRadius), &zgConcentratorRadius
  },
  {
    ZCD_NV_CONCENTRATOR_RC, sizeof(zgConcentratorRC), &zgConcentratorRC
  },
  {
    ZCD_NV_SRC_RTG_EXPIRY_TIME, sizeof(zgNwkSrcRtgExpiryTime), &zgNwkSrcRtgExpiryTime
  },
  {
    ZCD_NV_ROUTE_DISCOVERY_TIME, sizeof(zgRouteDiscoveryTime), &zgRouteDiscoveryTime
  },
#ifndef NONWK
  {
    ZCD_NV_PANID, sizeof(zgConfigPANID), &zgConfigPANID
  },
  {
    ZCD_NV_PRECFGKEYS_ENABLE, sizeof(zgPreConfigKeys), &zgPreConfigKeys
  },
  {
    ZCD_NV_SECURE_PERMIT_JOIN, sizeof(zgSecurePermitJoin), &zgSecurePermitJoin
  },
  {
    ZCD_NV_USE_DEFAULT_TCLK, sizeof(zgUseDefaultTCLK), &zgUseDefaultTCLK
  },
  {
    ZCD_NV_TRUSTCENTER_ADDR, Z_EXTADDR_LEN, zgApsTrustCenterAddr
  },
  {
    ZCD_NV_APS_LINK_KEY_TYPE, sizeof(zgApsLinkKeyType), &zgApsLinkKeyType
  },
#endif // NONWK
  {
    ZCD_NV_APS_FRAME_RETRIES, sizeof(zgApscMaxFrameRetries), &zgApscMaxFrameRetries
  },
  {
    ZCD_NV_APS_ACK_WAIT_DURATION, sizeof(zgApscAckWaitDurationPolled), &zgApscAckWaitDurationPolled
  },
  {
    ZCD_NV_APS_ACK_WAIT_MULTIPLIER, sizeof(zgApsAckWaitMultiplier), &zgApsAckWaitMultiplier
  },
  {
    ZCD_NV_BINDING_TIME, sizeof(zgApsDefaultMaxBindingTime), &zgApsDefaultMaxBindingTime
  },
  {
    ZCD_NV_APS_USE_EXT_PANID, Z_EXTADDR_LEN, zgApsUseExtendedPANID
  },
  {
    ZCD_NV_APS_USE_INSECURE_JOIN, sizeof(zgApsUseInsecureJoin), &zgApsUseInsecureJoin
  },
  {
    ZCD_NV_APS_NONMEMBER_RADIUS, sizeof(zgApsNonMemberRadius), &zgApsNonMemberRadius
  },
  {
    ZCD_NV_START_DELAY, sizeof(zgStartDelay), &zgStartDelay
  },
  {
    ZCD_NV_SAPI_ENDPOINT, sizeof(zgSapiEndpoint), &zgSapiEndpoint
  },
  {
    ZCD_NV_NWK_MGR_MODE, sizeof(zgNwkMgrMode), &zgNwkMgrMode
  },
  {
    ZCD_NV_NWKMGR_MIN_TX, sizeof(zgNwkMgrMinTransmissions), &zgNwkMgrMinTransmissions
  },
  {
    ZCD_NV_ROUTER_OFF_ASSOC_CLEANUP, sizeof(zgRouterOffAssocCleanup), &zgRouterOffAssocCleanup
  },
  {
    ZCD_NV_NWK_LEAVE_REQ_ALLOWED, sizeof(zgNwkLeaveRequestAllowed), &zgNwkLeaveRequestAllowed
  },
  {
    ZCD_NV_COMMISSIONED_NWK_ADDR, sizeof(zgNwkCommissionedNwkAddr), &zgNwkCommissionedNwkAddr
  },
  {
    ZCD_NV_APS_ALLOW_R19_SECURITY, sizeof(zgApsAllowR19Sec), &zgApsAllowR19Sec
  },
  {
    ZCD_NV_APS_DUPREJ_TIMEOUT_INC, sizeof(zgApscDupRejTimeoutInc), &zgApscDupRejTimeoutInc
  },
  {
    ZCD_NV_APS_DUPREJ_TIMEOUT_COUNT, sizeof(zgApscDupRejTimeoutCount), &zgApscDupRejTimeoutCount
  },
  {
    ZCD_NV_APS_DUPREJ_TABLE_SIZE, sizeof(zgApsMinDupRejTableSize), &zgApsMinDupRejTableSize
  },
  {
    ZCD_NV_NWK_CHILD_AGE_ENABLE, sizeof(zgChildAgingEnable), &zgChildAgingEnable
  },
  {
    ZCD_NV_NWK_PARENT_INFO, sizeof(zgNwkParentInformation), &zgNwkParentInformation
  },
  {
    ZCD_NV_NWK_ENDDEV_TIMEOUT_DEF, sizeof(zgNwkEndDeviceTimeoutDefault), &zgNwkEndDeviceTimeoutDefault
  },
  {
    ZCD_NV_END_DEV_TIMEOUT_VALUE, sizeof(zgEndDeviceTimeoutValue), &zgEndDeviceTimeoutValue
  },
  {
    ZCD_NV_END_DEV_CONFIGURATION, sizeof(zgEndDeviceConfiguration), &zgEndDeviceConfiguration
  },
#endif // NV_INIT
  // Last item -- DO NOT MOVE IT!
  {
    0x00, 0, NULL
  }
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static uint8_t zgItemInit( uint16_t id, uint16_t len, void *buf, uint8_t setDefault );

#ifdef ZSTACK_NVOCMP_MIGRATION
static void zgUpgradeNVDriver( void );
#endif

#ifndef NONWK
static uint8_t zgPreconfigKeyInit( uint8_t setDefault );
#endif

/*********************************************************************
 * @fn       zgItemInit()
 *
 * @brief
 *
 *   Initialize a global item. If the item doesn't exist in NV memory,
 *   write the system default (value passed in) into NV memory. But if
 *   it exists, set the item to the value stored in NV memory.
 *
 *   Also, if setDefault is TRUE and the item exists, we will write
 *   the default value to NV space.
 *
 * @param   id - item id
 * @param   len - item len
 * @param   buf - pointer to the item
 * @param   setDefault - TRUE to set default, not read
 *
 * @return  ZSUCCESS if successful, NV_ITEM_UNINIT if item did not
 *          exist in NV, NV_OPER_FAILED if failure.
 */
static uint8_t zgItemInit( uint16_t id, uint16_t len, void *buf, uint8_t setDefault )
{
  uint8_t status;

  // If the item doesn't exist in NV memory, create and initialize
  // it with the value passed in.
  status = osal_nv_item_init( id, len, buf );
  if ( status == ZSUCCESS )
  {
    if ( setDefault )
    {
      // Write the default value back to NV
      status = osal_nv_write( id, len, buf );
    }
    else
    {
      // The item exists in NV memory, read it from NV memory
      status = osal_nv_read( id, 0, len, buf );
    }
  }

  return (status);
}

/*********************************************************************
 * API FUNCTIONS
 */

/*********************************************************************
 * @fn          zgInit
 *
 * @brief
 *
 *   Initialize the Z-Stack Globals. If an item doesn't exist in
 *   NV memory, write the system default into NV memory. But if
 *   it exists, set the item to the value stored in NV memory.
 *
 * NOTE: The Startup Options (ZCD_NV_STARTUP_OPTION) indicate
 *       that the Config state items (zgItemTable) need to be
 *       set to defaults (ZCD_STARTOPT_DEFAULT_CONFIG_STATE). The
 *
 * @param       none
 *
 * @return      ZSUCCESS if successful, NV_ITEM_UNINIT if item did not
 *              exist in NV, NV_OPER_FAILED if failure.
 */
uint8_t zgInit( void )
{
  uint8_t  setDefault = FALSE;
  uint8_t  status;

#ifdef NV_RESTORE
  // Do we want to default the Config state values
  if ( zgReadStartupOptions() & ZCD_STARTOPT_DEFAULT_CONFIG_STATE )
  {
    setDefault = TRUE;
  }
#else
    setDefault = TRUE;
#endif

  status = osal_nv_item_init(ZCD_NV_BDBNODEISONANETWORK,sizeof(bdbAttributes.bdbNodeIsOnANetwork),&bdbAttributes.bdbNodeIsOnANetwork);

  //Force to reset state if device is forced to FN
  if((status == SUCCESS) && setDefault)
  {
    bdb_setNodeIsOnANetwork(false);
  }

#if defined (FEATURE_SYSTEM_STATS)
  {
    // This sections tracks the number of resets
    uint16_t bootCnt = 0;

    // Update the Boot Counter
    if ( osal_nv_item_init( ZCD_NV_BOOTCOUNTER, sizeof(bootCnt), &bootCnt ) == ZSUCCESS )
    {
      // Get the old value from NV memory
      osal_nv_read( ZCD_NV_BOOTCOUNTER, 0, sizeof(bootCnt), &bootCnt );
    }

    // Increment the Boot Counter and store it into NV memory
    if ( setDefault )
    {
      bootCnt = 0;
    }
    else
    {
      bootCnt++;
    }

    osal_nv_write( ZCD_NV_BOOTCOUNTER, sizeof(bootCnt), &bootCnt );
  }
#endif  // FEATURE_SYSTEM_STATS

  if (SET_EPID_AS_EXT_ADDR)
  {
    // Initialize the Extended PAN ID as my own extended address
    ZMacGetReq( ZMacExtAddr, zgExtendedPANID );
  }

#ifdef ZSTACK_NVOCMP_MIGRATION
  if(setDefault == FALSE)
  {
    zgUpgradeNVDriver();
  }
#endif

  // Initialize the items table
  zgInitItems( setDefault );

#ifndef NONWK
    // Initialize the Pre-Configured Key to the default key
    zgPreconfigKeyInit( setDefault );

    // Initialize NV items for all Keys: NWK, APS, TCLK and Master
    ZDSecMgrInitNVKeyTables( setDefault );
#endif // NONWK

  // Clear the Config State default
  if ( setDefault )
  {
    zgWriteStartupOptions( ZG_STARTUP_CLEAR, ZCD_STARTOPT_DEFAULT_CONFIG_STATE );
  }

  return ( ZSUCCESS );
}

/*********************************************************************
 * @fn          zgInitItems
 *
 * @brief       Initializes RAM variables from NV.  If NV items don't
 *              exist, then the NV is initialize with what is in RAM
 *              variables.
 *
 * @param       none
 *
 * @return      none
 */
void zgInitItems( uint8_t setDefault )
{
  uint8_t  i = 0;

  if ( ZG_BUILD_COORDINATOR_TYPE && ( zgApsLinkKeyType == ZG_UNIQUE_LINK_KEY ) )
  {
    zgUseDefaultTCLK = FALSE;
  }
  else
  {
    // Most of the time default TCLK will be used
    zgUseDefaultTCLK = TRUE;
  }

  while ( zgItemTable[i].id != 0x00 )
  {
    // Initialize the item
    zgItemInit( zgItemTable[i].id, zgItemTable[i].len, zgItemTable[i].buf, setDefault  );

    // Move on to the next item
    i++;
  }
}

/*********************************************************************
 * @fn          zgReadStartupOptions
 *
 * @brief       Reads the ZCD_NV_STARTUP_OPTION NV Item.
 *
 * @param       none
 *
 * @return      the ZCD_NV_STARTUP_OPTION NV item
 */
uint8_t zgReadStartupOptions( void )
{
  // Default to Use Config State and Use Network State
  uint8_t startupOption = 0;

  // This should have been done in ZMain.c, but just in case.
  if ( osal_nv_item_init( ZCD_NV_STARTUP_OPTION,
                              sizeof(startupOption),
                              &startupOption ) == ZSUCCESS )
  {
    // Read saved startup control
    osal_nv_read( ZCD_NV_STARTUP_OPTION,
                  0,
                  sizeof( startupOption ),
                  &startupOption);
  }
  return ( startupOption );
}

/*********************************************************************
 * @fn          zgWriteStartupOptions
 *
 * @brief       Writes bits into the ZCD_NV_STARTUP_OPTION NV Item.
 *
 * @param       action - ZG_STARTUP_SET set bit, ZG_STARTUP_CLEAR to
 *               clear bit. The set bit is an OR operation, and the
 *               clear bit is an AND ~(bitOptions) operation.
 *
 * @param       bitOptions - which bits to perform action on:
 *                      ZCD_STARTOPT_DEFAULT_CONFIG_STATE
 *                      ZCD_STARTOPT_DEFAULT_NETWORK_STATE
 *
 * @return      ZSUCCESS if successful
 */
uint8_t zgWriteStartupOptions( uint8_t action, uint8_t bitOptions )
{
  uint8_t status;
  uint8_t startupOptions = 0;

  status = osal_nv_read( ZCD_NV_STARTUP_OPTION,
                0,
                sizeof( startupOptions ),
                &startupOptions );

  if ( status == ZSUCCESS )
  {
    if ( action == ZG_STARTUP_SET )
    {
      // Set bits
      startupOptions |= bitOptions;
    }
    else
    {
      // Clear bits
      startupOptions &= (bitOptions ^ 0xFF);
    }

    // Changed?
    status = osal_nv_write( ZCD_NV_STARTUP_OPTION,
                 sizeof( startupOptions ),
                 &startupOptions );
  }

  return ( status );
}

/*********************************************************************
 * @fn          zgSetItem
 *
 * @brief       Set RAM variables from set-NV, if it exist in the zgItemTable
 *
 * @param       id - NV ID
 *              len - NV item length
 *              buf - pointer to the input buffer
 *
 * @return      none
 */
void zgSetItem( uint16_t id, uint16_t len, void *buf )
{

  uint8_t  i = 0;

  // Look up the NV item table
  while ( zgItemTable[i].id != 0x00 )
  {
    if( zgItemTable[i].id == id )
    {
      if ( zgItemTable[i].len == len )
      {
        OsalPort_memcpy( zgItemTable[i].buf, buf, len );
      }
      break;
    }
    // Move on to the next item
    i++;
  }
}

#ifndef NONWK
/*********************************************************************
 * @fn       zgPreconfigKeyInit()
 *
 * @brief
 *
 *   Initialize ZCD_NV_PRECFGKEY NV item. If the item doesn't exist in NV memory,
 *   write the system default (value passed in) into NV memory. But if
 *   it exists do not overwrite it.
 *
 *   Also, if setDefault is TRUE and the item exists, we will write
 *   the default value to NV space.
 *
 * @param   setDefault - TRUE to set default
 *
 * @return  ZSUCCESS if successful, NV_ITEM_UNINIT if item did not
 *          exist in NV, NV_OPER_FAILED if failure.
 */
static uint8_t zgPreconfigKeyInit( uint8_t setDefault )
{
  uint8_t              zgPreConfigKey[SEC_KEY_LEN];
  uint8_t              status;

  //NWK KEY
  //if nwk key is set to zeros, then generate a random key and use it
  memset(zgPreConfigKey,0,SEC_KEY_LEN);

#if (ZG_BUILD_RTR_TYPE)
  if(ZG_DEVICE_RTR_TYPE)
  {
    if(OsalPort_memcmp(defaultKey, zgPreConfigKey,SEC_KEY_LEN))
    {
      ZDSecMgrGenerateRndKey(zgPreConfigKey);
    }
    else
    {
      // Initialize the Pre-Configured Key to the default key
      OsalPort_memcpy( zgPreConfigKey, defaultKey, SEC_KEY_LEN );
    }
  }
#endif


  // If the item doesn't exist in NV memory, create and initialize it
  status = osal_nv_item_init( ZCD_NV_PRECFGKEY, SEC_KEY_LEN, zgPreConfigKey );
  if ( status == ZSUCCESS )
  {
    if ( setDefault )
    {
      // Write the default value back to NV
      status =  osal_nv_write( ZCD_NV_PRECFGKEY, SEC_KEY_LEN, zgPreConfigKey );
    }
  }

  // clear local copy of default key
  memset(zgPreConfigKey, 0x00, SEC_KEY_LEN);

  return (status);
}
#endif

#ifdef ZSTACK_NVOCMP_MIGRATION
/*********************************************************************
 * @fn       zgUpgradeNVDriver()
 *
 * @brief
 *
 *   Migrate from the two page (NVOCTP) to the multi page (NVOCMP)
 *   NV driver
 *
 * @param   none
 *
 * @return  none
 */
static void zgUpgradeNVDriver( void )
{
  uint8_t upgradeComplete = 0;
  // check if USE NVOCMP flag exists. If not, we must do a one-time
  // NV migration from NVOCTP to NVOCMP
  if( NV_ITEM_UNINIT == osal_nv_item_init( ZCD_NV_USE_NVOCMP, sizeof(uint8_t), &upgradeComplete ) )
  {
    uint16_t i = 0;

    // NOTICE: Developers should ensure that table sizes constants
    // (e.g. ZDSECMGR_TC_DEVICE_MAX) should not change sizes
    // between 3.20 and 3.30 SDK image. With the logic below
    // These constants will be used to index both the old NV tables
    // and the new NV tables. If you wish to change table sizes as well,
    // this logic will need to be updated to account for that.

    for( i = 0; i < gZDSECMGR_TC_DEVICE_MAX; i++ )
    {
      APSME_TCLinkKeyNVEntry_t defaultTCLinkKeyNVEntry = {0};
      APSME_TCLinkKeyNVEntry_t TCLinkKeyNVEntry = {0};
      // initialize the old TCLK NV entry
      if( SUCCESS == osal_nv_item_init( (ZCD_NV_LEGACY_TCLK_TABLE_START + i),
                        sizeof(APSME_TCLinkKeyNVEntry_t), &defaultTCLinkKeyNVEntry) )
      {
        // read the old TCLK NV entry into local variable
        if( SUCCESS == osal_nv_read( ZCD_NV_LEGACY_TCLK_TABLE_START + i, 0,
                      sizeof(APSME_TCLinkKeyNVEntry_t), &TCLinkKeyNVEntry) )
        {
          // initialize the new TCLK NV entry
          if( NV_ITEM_UNINIT == osal_nv_item_init_ex( ZCD_NV_EX_TCLK_TABLE, i,
                                               sizeof(APSME_TCLinkKeyNVEntry_t),
                                               &defaultTCLinkKeyNVEntry) )
          {
            // move the entry from the old table into the new table
            if( SUCCESS == osal_nv_write_ex( ZCD_NV_EX_TCLK_TABLE, i,
                                             sizeof(APSME_TCLinkKeyNVEntry_t),
                                             &TCLinkKeyNVEntry ) )
            {
              // delete the entry from the old table
              osal_nv_delete(ZCD_NV_LEGACY_TCLK_TABLE_START + i,
                             sizeof(APSME_TCLinkKeyNVEntry_t) );
            }
          }
        }
      }
    }

    for( i = 0; i < gZDSECMGR_TC_DEVICE_IC_MAX; i++ )
    {
      uint8_t defaultTCLinkKeyICNVEntry[SEC_KEY_LEN] = {0};
      uint8_t TCLinkKeyICNVEntry[SEC_KEY_LEN] = {0};
      // initialize the old TCLK IC NV entry
      if( SUCCESS == osal_nv_item_init( (ZCD_NV_LEGACY_TCLK_IC_TABLE_START + i),
                                      SEC_KEY_LEN, &defaultTCLinkKeyICNVEntry) )
      {
        // read the old TCLK IC NV entry into local variable
        if( SUCCESS == osal_nv_read( ZCD_NV_LEGACY_TCLK_IC_TABLE_START + i, 0,
                                     SEC_KEY_LEN, &TCLinkKeyICNVEntry) )
        {
          // initialize the new TCLK IC NV entry
          if( NV_ITEM_UNINIT == osal_nv_item_init_ex( ZCD_NV_EX_TCLK_IC_TABLE, i,
                                               SEC_KEY_LEN,
                                               &defaultTCLinkKeyICNVEntry) )
          {
            // move the entry from the old table into the new table
            if( SUCCESS == osal_nv_write_ex( ZCD_NV_EX_TCLK_IC_TABLE, i,
                                             SEC_KEY_LEN,
                                             &TCLinkKeyICNVEntry ) )
            {
              // delete the entry from the old table
              osal_nv_delete(ZCD_NV_LEGACY_TCLK_IC_TABLE_START + i,
                             SEC_KEY_LEN );
            }
          }
        }
      }
    }

    for( i = 0; i < gZDSECMGR_ENTRY_MAX; i++ )
    {
      APSME_ApsLinkKeyNVEntry_t defaultAPSLinkKeyData = {0};
      APSME_ApsLinkKeyNVEntry_t APSLinkKeyData = {0};
      // initialize the old APS Link Key NV entry
      if( SUCCESS == osal_nv_item_init( (ZCD_NV_LEGACY_APS_LINK_KEY_DATA_START + i),
                              sizeof(APSME_ApsLinkKeyNVEntry_t), &defaultAPSLinkKeyData) )
      {
        // read the old APS Link Key NV entry into local variable
        if( SUCCESS == osal_nv_read( ZCD_NV_LEGACY_APS_LINK_KEY_DATA_START + i, 0,
                                   sizeof(APSME_ApsLinkKeyNVEntry_t), &APSLinkKeyData) )
        {
          // initialize the new APS Link Key NV entry
          if( NV_ITEM_UNINIT == osal_nv_item_init_ex( ZCD_NV_EX_APS_KEY_DATA_TABLE, i,
                                               sizeof(APSME_ApsLinkKeyNVEntry_t),
                                               &defaultAPSLinkKeyData) )
          {
            // move the entry from the old table into the new table
            if( SUCCESS == osal_nv_write_ex( ZCD_NV_EX_APS_KEY_DATA_TABLE, i,
                                             sizeof(APSME_ApsLinkKeyNVEntry_t),
                                             &APSLinkKeyData ) )
            {
              // delete the entry from the old table
              osal_nv_delete(ZCD_NV_LEGACY_APS_LINK_KEY_DATA_START + i,
                             sizeof(APSME_ApsLinkKeyNVEntry_t) );
            }
          }
        }
      }
    }

    for( i = 0; i < MAX_NWK_SEC_MATERIAL_TABLE_ENTRIES; i++ )
    {
      nwkSecMaterialDesc_t defaultNwkSecMaterial = {0};
      nwkSecMaterialDesc_t nwkSecMaterial = {0};
      // initialize the old NWK Sec Material NV entry
      if( SUCCESS == osal_nv_item_init( (ZCD_NV_LEGACY_NWK_SEC_MATERIAL_TABLE_START + i),
                              sizeof(nwkSecMaterialDesc_t), &defaultNwkSecMaterial) )
      {
        // read the old NWK Sec Material NV entry into local variable
        if( SUCCESS == osal_nv_read( ZCD_NV_LEGACY_NWK_SEC_MATERIAL_TABLE_START + i, 0,
                                   sizeof(nwkSecMaterialDesc_t), &nwkSecMaterial) )
        {
          // initialize the new NWK Sec Material NV entry
          if( NV_ITEM_UNINIT == osal_nv_item_init_ex( ZCD_NV_EX_NWK_SEC_MATERIAL_TABLE, i,
                                               sizeof(nwkSecMaterialDesc_t),
                                               &defaultNwkSecMaterial) )
          {
            // move the entry from the old table into the new table
            if( SUCCESS == osal_nv_write_ex( ZCD_NV_EX_NWK_SEC_MATERIAL_TABLE, i,
                                             sizeof(nwkSecMaterialDesc_t),
                                             &nwkSecMaterial ) )
            {
              // delete the entry from the old table
              osal_nv_delete(ZCD_NV_LEGACY_NWK_SEC_MATERIAL_TABLE_START + i,
                             sizeof(nwkSecMaterialDesc_t) );
            }
          }
        }
      }
    }

#if !defined (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE)
    for( i = 0; i < GPP_MAX_PROXY_TABLE_ENTRIES; i++ )
    {
      uint8_t defaultProxyTableEntry[PROXY_TBL_LEN] = {0};
      uint8_t proxyTableEntry[PROXY_TBL_LEN] = {0};
      // initialize the old GPP Table NV entry
      if( SUCCESS == osal_nv_item_init_ex( ZCL_PORT_PROXY_TABLE_NV_ID,
                                           (ZCD_NV_LEGACY_PROXY_TABLE_START + i),
                                           PROXY_TBL_LEN, &defaultProxyTableEntry) )
      {
        // read the old GPP Table NV entry into local variable
        if( SUCCESS == osal_nv_read_ex( ZCL_PORT_PROXY_TABLE_NV_ID,
                                        (ZCD_NV_LEGACY_PROXY_TABLE_START + i), 0,
                                        PROXY_TBL_LEN, &proxyTableEntry) )
        {
          // initialize the new GPP Table NV entry
          if( NV_ITEM_UNINIT == osal_nv_item_init_ex( ZCL_PORT_PROXY_TABLE_NV_ID, i,
                                                      PROXY_TBL_LEN,
                                                      &defaultProxyTableEntry) )
          {
            // move the entry from the old table into the new table
            if( SUCCESS == osal_nv_write_ex( ZCL_PORT_PROXY_TABLE_NV_ID, i,
                                             PROXY_TBL_LEN,
                                             &proxyTableEntry ) )
            {
              // delete the entry from the old table
              osal_nv_delete_ex( ZCL_PORT_PROXY_TABLE_NV_ID,
                                 (ZCD_NV_LEGACY_PROXY_TABLE_START + i),
                                 PROXY_TBL_LEN );
            }
          }
        }
      }
    }
#endif
#if defined (ENABLE_GREENPOWER_COMBO_BASIC)
    for( i = 0; i < GPS_MAX_SINK_TABLE_ENTRIES; i++ )
    {
      uint8_t defaultSinkTableEntry[SINK_TBL_ENTRY_LEN] = {0};
      uint8_t sinkTableEntry[SINK_TBL_ENTRY_LEN] = {0};
      // initialize the old GPS Table NV entry
      if( SUCCESS == osal_nv_item_init_ex( ZCL_PORT_SINK_TABLE_NV_ID,
                                           (ZCD_NV_LEGACY_SINK_TABLE_START + i),
                                           SINK_TBL_ENTRY_LEN, &defaultSinkTableEntry) )
      {
        // read the old GPS Table NV entry into local variable
        if( SUCCESS == osal_nv_read_ex( ZCL_PORT_SINK_TABLE_NV_ID,
                                        (ZCD_NV_LEGACY_SINK_TABLE_START + i), 0,
                                        SINK_TBL_ENTRY_LEN, &sinkTableEntry) )
        {
          // initialize the new GPS Table NV entry
          if( NV_ITEM_UNINIT == osal_nv_item_init_ex( ZCL_PORT_SINK_TABLE_NV_ID, i,
                                                      SINK_TBL_ENTRY_LEN,
                                                      &defaultSinkTableEntry) )
          {
            // move the entry from the old table into the new table
            if( SUCCESS == osal_nv_write_ex( ZCL_PORT_SINK_TABLE_NV_ID, i,
                                             SINK_TBL_ENTRY_LEN,
                                             &sinkTableEntry ) )
            {
              // delete the entry from the old table
              osal_nv_delete_ex( ZCL_PORT_SINK_TABLE_NV_ID,
                                 (ZCD_NV_LEGACY_SINK_TABLE_START + i),
                                 SINK_TBL_ENTRY_LEN );
            }
          }
        }
      }
    }
#endif

    upgradeComplete = TRUE;
    osal_nv_write( ZCD_NV_USE_NVOCMP, sizeof(uint8_t), &upgradeComplete );
  }
}
#endif

/*********************************************************************
*********************************************************************/
