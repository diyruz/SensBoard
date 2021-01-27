/**************************************************************************************************
  Filename:       bdb_interface.h
  Revised:        $Date: 2016-02-25 11:51:49 -0700 (Thu, 25 Feb 2016) $
  Revision:       $Revision: - $

  Description:    This file contains the Base Device Behavior interface such as
                  MACRO configuration and API.


  Copyright 2006-2014 Texas Instruments Incorporated. All rights reserved.

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

#ifndef BDB_INTERFACE_H
#define BDB_INTERFACE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "bdb.h"
#include "zstack.h"

 /*********************************************************************
 * CONSTANTS
 */

#define BDB_INSTALL_CODE_USE_IC_CRC  1    // IC + CRC are expected to be introduced (18 bytes), CRC is validated and TC link key is calcuated from it
#define BDB_INSTALL_CODE_USE_KEY     2    // TC link key is expected (16 bytes), the buffer provided will be used as the key


//bdbTCLinkKeyExchangeMethod
#define BDB_TC_LINK_KEY_EXCHANGE_APS_KEY     0x00
#define BDB_TC_LINK_KEY_EXCHANGE_CBKE        0x01    //Not supported yet by spec

//Status notifications on APS TC Link exchange process for a joining device
#define BDB_TC_LK_EXCH_PROCESS_JOINING         0
#define BDB_TC_LK_EXCH_PROCESS_EXCH_SUCCESS    1
#define BDB_TC_LK_EXCH_PROCESS_EXCH_FAIL       2


#ifdef BDB_REPORTING
#define BDBREPORTING_REPORTOFF       0xFFFF //Use for MaxInterval, No periodic nor value changed reporting
#define BDBREPORTING_NOPERIODIC      0x0000 //Use for MaxInterval, No periodic but value changed is enabled
#define BDBREPORTING_NOLIMIT         0x0000 //Use for MinInterval, there can be consecutive value change reporting without enforcing wait time
#endif
 /*********************************************************************
 * CONFIGURATION MACROS
 */

 /**********************
 * General configuration
 */

//Constants used by all nodes
#ifdef PER_TEST_SET_FINDING_BINDING_1_MIN
#define BDBC_MIN_COMMISSIONING_TIME                    60    // Reduce the time for F&B to allow easy matching between intended devices
#else
#define BDBC_MIN_COMMISSIONING_TIME                    180    // 180 seconds
#endif
#define BDBC_REC_SAME_NETWORK_RETRY_ATTEMPS            3      //Maximum by BDB spec is 10

//Define if ZR devices will perform classical formation procedure or not (the network formed would be Distributed Network)
#define BDB_ROUTER_FORM_DISTRIBUTED_NWK_ENABLED     1

//Define how IC are introduced see
#define BDB_INSTALL_CODE_USE  BDB_INSTALL_CODE_USE_IC_CRC

//Time after which the device will reset itself after failining on
//TCLK exchange procedure (more than BDB_DEFAULT_TC_LINK_KEY_EXCHANGE_ATTEMPS_MAX
//attempts for the same request or Parent Lost during TCLK exchange process).
//This reset will perform a FN reset
#define BDB_TC_LINK_KEY_EXCHANGE_FAIL_LEAVE_TIMEOUT  5000

#define BDB_TL_IDENTIFY_TIME                         0xFFFF



#define BDB_DEFAULT_DEVICE_UNAUTH_TIMEOUT            3000  // Time joining device waits for nwk
                                                           // before attempt association again.
                                                           // In BDB is known as apsSecurityTimeOutPeriod

#define BDB_ALLOW_TL_STEALING                        TRUE


 /******************
 * F&B configuration
 */
//Your JOB: Set this value according to your application
//Initiator devices in F&B procedure may enable this macro to perfom the searching as a periodic task
//for up to 3 minutes with a period of FINDING_AND_BINDING_PERIODIC_TIME between discovery attempts
#define FINDING_AND_BINDING_PERIODIC_ENABLE          TRUE    // Boolean
#define FINDING_AND_BINDING_PERIODIC_TIME            15      // in seconds

// Number of attemtps that will be done to retrieve the simple desc from a target
// device or the IEEE Address if this is unknown. The number of attempts cannot
// be greater than 36
#define FINDING_AND_BINDING_MAX_ATTEMPTS             4

//Default values for BDB attributes
#define BDB_DEFAULT_COMMISSIONING_GROUP_ID          0xFFFF
#define BDB_DEFAULT_JOIN_USES_INSTALL_CODE_KEY      FALSE
#define BDB_DEFAULT_PRIMARY_CHANNEL_SET             DEFAULT_CHANLIST //BDB specification default is: 0x02108800
#define BDB_DEFAULT_SCAN_DURATION                   0x04
#define BDB_DEFAULT_SECONDARY_CHANNEL_SET           SECONDARY_CHANLIST //BDB specification default is: (0x07FFF800 ^ 0x02108800)
#define BDB_DEFAULT_TC_LINK_KEY_EXCHANGE_METHOD     BDB_TC_LINK_KEY_EXCHANGE_APS_KEY
#define BDB_DEFAULT_TC_NODE_JOIN_TIMEOUT            0x0F
#define BDB_DEFAULT_TC_REQUIRE_KEY_EXCHANGE         FALSE


//Your JOB: Set this value according to your application
//This defines the time that initiator device will wait for Indentify query response
//and simple descriptor from target devices. Consider Identify Query is broadcast while Simple Desc is unicast
//Consider that ZED will have to wait longer since their responses will need to
//be pooled and will be dependent of the number of targets that is expected to be found
#if ZG_BUILD_RTR_TYPE
#define IDENTIFY_QUERY_RSP_TIMEOUT                     5000
#define SIMPLEDESC_RESPONSE_TIMEOUT_RTR                (ROUTE_DISCOVERY_TIME * 1000)  // Timeout for ZR
#else
#define IDENTIFY_QUERY_RSP_TIMEOUT                     10000
#define SIMPLEDESC_RESPONSE_TIMEOUT_RX_ALWAYS_ON       3000
#define SIMPLEDESC_RESPONSE_TIMEOUT_SLEEPY             (3 * nwk_GetConfigPollRate(POLL_RATE_TYPE_DEFAULT))      // Timeout for ZED  *Don't use time smaller than POLL_RATE
#endif


 /******************
 * Touchlink configuration
 */

/** Received signal strength threshold **/
// Manufacturer specific threshold (greater than -128),
// do not respond to Touch-link scan request if reached
#ifndef TOUCHLINK_WORST_RSSI
#define TOUCHLINK_WORST_RSSI               -85 // dBm
#endif

// Pre-programmed RSSI correction offset (0x00-0x20)
#ifndef TOUCHLINK_RSSI_CORRECTION
#define TOUCHLINK_RSSI_CORRECTION          0x00
#endif

/** Pre-Installed Keys **/
#define TOUCHLINK_CERTIFICATION_ENC_KEY    { 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,\
                                             0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf }

#define TOUCHLINK_CERTIFICATION_LINK_KEY   { 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,\
                                             0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf }

#define TOUCHLINK_DEFAULT_AES_KEY          { 0x50, 0x68, 0x4c, 0x69, 0xea, 0x9c, 0xd1, 0x38,\
                                             0x43, 0x4c, 0x53, 0x4e, 0x8f, 0x8d, 0xba, 0xb4 }

#define TOUCHLINK_KEY_INDEX_DEV         0
#define TOUCHLINK_KEY_INDEX_MASTER      4
#define TOUCHLINK_KEY_INDEX_CERT        15

// For certification only:
#define TOUCHLINK_ENC_KEY    TOUCHLINK_CERTIFICATION_ENC_KEY
#define TOUCHLINK_LINK_KEY   TOUCHLINK_CERTIFICATION_LINK_KEY
#define TOUCHLINK_KEY_INDEX  TOUCHLINK_KEY_INDEX_CERT

// For internal EP's simple descriptor
#define TOUCHLINK_INTERNAL_ENDPOINT                 13
#define TOUCHLINK_INTERNAL_DEVICE_ID                0xE15E
#define TOUCHLINK_INTERNAL_FLAGS                    0
#define TOUCHLINK_DEFAULT_IDENTIFY_TIME             3

 /******************
 * Reporting attributes configuration
 */
#ifdef BDB_REPORTING
//Your JOB: Set this value according to your application
//Maximum size in bytes used by reportable attributes registered in any
//endpoint for the application (for analog attributes)
#ifndef BDBREPORTING_MAX_ANALOG_ATTR_SIZE
#define BDBREPORTING_MAX_ANALOG_ATTR_SIZE    4
#endif

//Your JOB: Set this value according to your application
//Max num of cluster with reportable attributes in any endpoint
//(eg. 2 endpoints with same cluster with reportable attributes counts as 2,
//regardless of the number of reportable attributes in the cluster)
#ifndef BDB_MAX_CLUSTERENDPOINTS_REPORTING
#define BDB_MAX_CLUSTERENDPOINTS_REPORTING    5
#endif

//Default values contants used in the bdb reporting code
#define BDBREPORTING_DEFAULTMAXINTERVAL    BDBREPORTING_REPORTOFF
#define BDBREPORTING_DEFAULTMININTERVAL    0x000A

//Define the DISABLE_DEFAULT_RSP flag for reporting attributes
#define BDB_REPORTING_DISABLE_DEFAULT_RSP  FALSE
#endif

/*********************************************************************
 * ENUM
 */


enum
{
  BDB_COMMISSIONING_INITIALIZATION,
  BDB_COMMISSIONING_NWK_STEERING,
  BDB_COMMISSIONING_FORMATION,
  BDB_COMMISSIONING_FINDING_BINDING,
  BDB_COMMISSIONING_TOUCHLINK,
  BDB_COMMISSIONING_PARENT_LOST,
};


enum
{
  BDB_COMMISSIONING_SUCCESS,
  BDB_COMMISSIONING_IN_PROGRESS,
  BDB_COMMISSIONING_NO_NETWORK,
  BDB_COMMISSIONING_TL_TARGET_FAILURE,
  BDB_COMMISSIONING_TL_NOT_AA_CAPABLE,
  BDB_COMMISSIONING_TL_NO_SCAN_RESPONSE,
  BDB_COMMISSIONING_TL_NOT_PERMITTED,
  BDB_COMMISSIONING_TCLK_EX_FAILURE,              //Many attempts made and failed, or parent lost during the TCLK exchange
  BDB_COMMISSIONING_FORMATION_FAILURE,
  BDB_COMMISSIONING_FB_TARGET_IN_PROGRESS,        //No callback from bdb when the identify time runs out, the application can figure out from Identify time callback
  BDB_COMMISSIONING_FB_INITITATOR_IN_PROGRESS,
  BDB_COMMISSIONING_FB_NO_IDENTIFY_QUERY_RESPONSE,
  BDB_COMMISSIONING_FB_BINDING_TABLE_FULL,
  BDB_COMMISSIONING_NETWORK_RESTORED,
  BDB_COMMISSIONING_FAILURE,              //Generic failure status for no commissioning mode supported, not allowed, invalid state to perform commissioning
};



 /*********************************************************************
 * TYPEDEFS
 */







/*********************************************************************
 * GLOBAL VARIABLES
 */


 /*********************************************************************
 * FUNCTION MACROS
 */

/*
 * Initialize the device with persistant data. Restore nwk (Silent rejoin for ZC and ZR, Rejoin for ZED), and resume reporting attributes.
 */
#define bdb_initialize() bdb_StartCommissioning(0)


 /*********************************************************************
 * FUNCTIONS
 */


/*************************************************************************/

#ifdef __cplusplus
}
#endif

#endif //BDB_INTERFACE_H
