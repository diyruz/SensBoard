/**************************************************************************************************
  Filename:       zcl_genericapp_data.c
  Revised:        $Date: 2014-05-12 13:14:02 -0700 (Mon, 12 May 2014) $
  Revision:       $Revision: 38502 $


  Description:    Zigbee Cluster Library - sample device application.


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

/*********************************************************************
 * INCLUDES
 */
#include "zcomdef.h"
#include "af.h"
#include "zd_config.h"

#include "zcl.h"
#include "zcl_general.h"
#include "zcl_ha.h"

/* GENERICAPP_TODO: Include any of the header files below to access specific cluster data
#include "zcl_poll_control.h"
#include "zcl_electrical_measurement.h"
#include "zcl_diagnostic.h"
#include "zcl_meter_identification.h"
#include "zcl_appliance_identification.h"
#include "zcl_appliance_events_alerts.h"
#include "zcl_power_profile.h"
#include "zcl_appliance_control.h"
#include "zcl_appliance_statistics.h"
#include "zcl_hvac.h"
*/


#include "zcl_genericapp.h"

/*********************************************************************
 * CONSTANTS
 */

#define GENERICAPP_DEVICE_VERSION     0
#define GENERICAPP_FLAGS              0

#define GENERICAPP_HWVERSION          1
#define GENERICAPP_ZCLVERSION         BASIC_ZCL_VERSION

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// Global attributes
const uint16_t zclGenericApp_basic_clusterRevision = 0x0002;
int16_t zclGenericApp_deviceTemperature = 0;
uint8_t zclGenericApp_batteryVoltage = 0;
#ifdef ZCL_IDENTIFY
const uint16_t zclGenericApp_identify_clusterRevision = 0x0001;
#endif

// Basic Cluster
const uint8_t zclGenericApp_HWRevision = GENERICAPP_HWVERSION;
const uint8_t zclGenericApp_ZCLVersion = GENERICAPP_ZCLVERSION;
const uint8_t zclGenericApp_ManufacturerName[] = { 6, 'D', 'I', 'Y', 'R', 'u', 'Z'};
const uint8_t zclGenericApp_PowerSource = POWER_SOURCE_BATTERY;
uint8_t zclGenericApp_PhysicalEnvironment = PHY_UNSPECIFIED_ENV;
const uint8_t zclGenericApp_ModelName[] = { 15, 'D', 'I', 'Y', 'R', 'u', 'Z', '_', 'W', 'a', 't', 'e', 'r', 'M', 'o', 'n'};
const uint8_t zclGenericApp_DateCode[] = {4, '2', '0', '2', '0'};
const uint8_t zclGenericApp_ApplicationVersion = 1;
const uint8_t zclGenericApp_BuildId = 1;

#ifdef ZCL_IDENTIFY
// Identify Cluster
uint16_t zclGenericApp_IdentifyTime;
#endif

/* GENERICAPP_TODO: declare attribute variables here. If its value can change,
 * initialize it in zclGenericApp_ResetAttributesToDefaultValues. If its
 * value will not change, initialize it here.
 */

#if ZCL_DISCOVER
CONST zclCommandRec_t zclGenericApp_Cmds[] =
{
  {
    ZCL_CLUSTER_ID_GENERAL_BASIC,
    COMMAND_BASIC_RESET_TO_FACTORY_DEFAULTS,
    CMD_DIR_SERVER_RECEIVED
  },

};

CONST uint8_t zclCmdsArraySize = ( sizeof(zclGenericApp_Cmds) / sizeof(zclGenericApp_Cmds[0]) );
#endif // ZCL_DISCOVER

/*********************************************************************
 * ATTRIBUTE DEFINITIONS - Uses REAL cluster IDs
 */

CONST zclAttrRec_t zclGenericApp_Attrs[] =
        {
          // *** General Basic Cluster Attributes ***
          {
            ZCL_CLUSTER_ID_GENERAL_BASIC,
            { // Attribute record
              ATTRID_BASIC_ZCL_VERSION,
              ZCL_DATATYPE_UINT8,
              ACCESS_CONTROL_READ,
              (void *)&zclGenericApp_ZCLVersion
            }
          },
          {
            ZCL_CLUSTER_ID_GENERAL_BASIC,
            { // Attribute record
              ATTRID_BASIC_APPLICATION_VERSION,
              ZCL_DATATYPE_UINT8,
              ACCESS_CONTROL_READ,
              (void *)&zclGenericApp_ApplicationVersion
            }
          },
          {
            ZCL_CLUSTER_ID_GENERAL_BASIC,
            { // Attribute record
              ATTRID_BASIC_STACK_VERSION,
              ZCL_DATATYPE_UINT8,
              ACCESS_CONTROL_READ,
              (void *)&zclGenericApp_ZCLVersion
            }
          },
          {
            ZCL_CLUSTER_ID_GENERAL_BASIC,          // Cluster IDs - defined in the foundation (ie. zcl.h)
            {                                      // Attribute record
              ATTRID_BASIC_HW_VERSION,             // Attribute ID - Found in Cluster Library header (ie. zcl_general.h)
              ZCL_DATATYPE_UINT8,                  // Data Type - found in zcl.h
              ACCESS_CONTROL_READ,                 // Variable access control - found in zcl.h
              (void *)&zclGenericApp_HWRevision      // Pointer to attribute variable
            }
          },
          {
            ZCL_CLUSTER_ID_GENERAL_BASIC,
            { // Attribute record
              ATTRID_BASIC_MANUFACTURER_NAME,
              ZCL_DATATYPE_CHAR_STR,
              ACCESS_CONTROL_READ,
              (void *)zclGenericApp_ManufacturerName
            }
          },
          {
            ZCL_CLUSTER_ID_GENERAL_BASIC,
            { // Attribute record
              ATTRID_BASIC_MODEL_IDENTIFIER,
              ZCL_DATATYPE_CHAR_STR,
              ACCESS_CONTROL_READ,
              (void *)zclGenericApp_ModelName
            }
          },
            {
            ZCL_CLUSTER_ID_GENERAL_BASIC,
            { // Attribute record
              ATTRID_BASIC_DATE_CODE,
              ZCL_DATATYPE_CHAR_STR,
              ACCESS_CONTROL_READ,
              (void *)zclGenericApp_DateCode
            }
          },
          {
            ZCL_CLUSTER_ID_GENERAL_BASIC,
            { // Attribute record
              ATTRID_BASIC_POWER_SOURCE,
              ZCL_DATATYPE_ENUM8,
              ACCESS_CONTROL_READ,
              (void *)&zclGenericApp_PowerSource
            }
          },
           {
            ZCL_CLUSTER_ID_GENERAL_BASIC,
            { // Attribute record
              ATTRID_BASIC_PHYSICAL_ENVIRONMENT,
              ZCL_DATATYPE_ENUM8,
              (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
              (void *)&zclGenericApp_PhysicalEnvironment
            }
          },

          {
            ZCL_CLUSTER_ID_GENERAL_BASIC,
            { // Attribute record
              ATTRID_BASIC_SW_BUILD_ID,
              ZCL_DATATYPE_UINT8,
              ACCESS_CONTROL_READ,
              (void *)&zclGenericApp_BuildId
            }
          },
          {
            ZCL_CLUSTER_ID_GENERAL_BASIC,
            {  // Attribute record
              ATTRID_CLUSTER_REVISION,
              ZCL_DATATYPE_UINT16,
              ACCESS_CONTROL_READ,
              (void *)&zclGenericApp_basic_clusterRevision
            }
          },
          {
            ZCL_CLUSTER_ID_GENERAL_POWER_CFG,
            {
              ATTRID_POWER_CONFIGURATION_BATTERY_VOLTAGE,
              ZCL_DATATYPE_UINT8,
              ACCESS_CONTROL_READ | ACCESS_REPORTABLE,
              (void *)&zclGenericApp_batteryVoltage
            }
          },
          {
            ZCL_CLUSTER_ID_GENERAL_DEVICE_TEMP_CONFIG,
            {
              ATTRID_DEVICE_TEMPERATURE_CONFIGURATION_CURRENT_TEMPERATURE,
              ZCL_DATATYPE_INT16,
              ACCESS_CONTROL_READ | ACCESS_REPORTABLE,
              (void *)&zclGenericApp_deviceTemperature
            }
          }
        };

uint8_t CONST zclGenericApp_NumAttributes = ( sizeof(zclGenericApp_Attrs) / sizeof(zclGenericApp_Attrs[0]) );

#define ATTR(CLUSTER, NAME, TYPE, ACCESS, LOCATION) \
    {CLUSTER,{NAME, TYPE, ACCESS, (void*) LOCATION}}

uint16_t zclGenericApp_MutistateInputValues[GENERICAPP_CHANNELS_COUNT];
float zclGenericApp_ADCValues[GENERICAPP_CHANNELS_COUNT];


CONST zclAttrRec_t zclGenericApp_ChannelAttrs[][GENERICAPP_CHANNEL_ATTRS_COUNT] = {
   // ================================ CH1 [0] =========================================
   {
    ATTR(ZCL_CLUSTER_ID_GENERAL_ANALOG_INPUT_BASIC, ATTRID_IOV_BASIC_PRESENT_VALUE, ZCL_DATATYPE_SINGLE_PREC, (ACCESS_CONTROL_READ | ACCESS_REPORTABLE), &zclGenericApp_ADCValues[0]),
    ATTR(ZCL_CLUSTER_ID_GENERAL_MULTISTATE_INPUT_BASIC, ATTRID_IOV_BASIC_PRESENT_VALUE, ZCL_DATATYPE_UINT16, (ACCESS_CONTROL_READ | ACCESS_REPORTABLE | ACCESS_CONTROL_WRITE), &zclGenericApp_MutistateInputValues[0])
   },
   // ============================== END  CH1 =======================================

   // ================================ CH2 [1] =========================================
   {
    ATTR(ZCL_CLUSTER_ID_GENERAL_ANALOG_INPUT_BASIC, ATTRID_IOV_BASIC_PRESENT_VALUE, ZCL_DATATYPE_SINGLE_PREC, (ACCESS_CONTROL_READ | ACCESS_REPORTABLE), &zclGenericApp_ADCValues[1]),
    ATTR(ZCL_CLUSTER_ID_GENERAL_MULTISTATE_INPUT_BASIC, ATTRID_IOV_BASIC_PRESENT_VALUE, ZCL_DATATYPE_UINT16, (ACCESS_CONTROL_READ | ACCESS_REPORTABLE | ACCESS_CONTROL_WRITE), &zclGenericApp_MutistateInputValues[1])
   },
   // ============================== END  CH2 =======================================

   // ================================ CH3 [2] =========================================
   {
    ATTR(ZCL_CLUSTER_ID_GENERAL_ANALOG_INPUT_BASIC, ATTRID_IOV_BASIC_PRESENT_VALUE, ZCL_DATATYPE_SINGLE_PREC, (ACCESS_CONTROL_READ | ACCESS_REPORTABLE), &zclGenericApp_ADCValues[2]),
    ATTR(ZCL_CLUSTER_ID_GENERAL_MULTISTATE_INPUT_BASIC, ATTRID_IOV_BASIC_PRESENT_VALUE, ZCL_DATATYPE_UINT16, (ACCESS_CONTROL_READ | ACCESS_REPORTABLE | ACCESS_CONTROL_WRITE), &zclGenericApp_MutistateInputValues[2])
   }
   // ============================== END  CH3 =======================================
};



/*********************************************************************
 * SIMPLE DESCRIPTOR
 */
// This is the Cluster ID List and should be filled with Application
// specific cluster IDs.
const cId_t zclGenericApp_InClusterList[] =
{
  ZCL_CLUSTER_ID_GENERAL_BASIC
};
#define ZCLGENERICAPP_MAX_INCLUSTERS   (sizeof(zclGenericApp_InClusterList) / sizeof(zclGenericApp_InClusterList[0]))


const cId_t zclGenericApp_OutClusterList[] =
{
  ZCL_CLUSTER_ID_GENERAL_BASIC,
  ZCL_CLUSTER_ID_GENERAL_POWER_CFG,
  ZCL_CLUSTER_ID_GENERAL_DEVICE_TEMP_CONFIG
};
#define ZCLGENERICAPP_MAX_OUTCLUSTERS  (sizeof(zclGenericApp_OutClusterList) / sizeof(zclGenericApp_OutClusterList[0]))


const cId_t zclGenericApp_ChannelsInClusterList[] =
{
};
#define ZCLGENERICAPP_MAX_CHANNELS_INCLUSTERS   (sizeof(zclGenericApp_ChannelsInClusterList) / sizeof(zclGenericApp_ChannelsInClusterList[0]))


const cId_t zclGenericApp_ChannelsOutClusterList[] =
{
 ZCL_CLUSTER_ID_GENERAL_MULTISTATE_INPUT_BASIC,
 ZCL_CLUSTER_ID_GENERAL_ANALOG_INPUT_BASIC,
 ZCL_CLUSTER_ID_SS_IAS_ZONE
};

#define ZCLGENERICAPP_MAX_CHANNELS_OUTCLUSTERS  (sizeof(zclGenericApp_ChannelsOutClusterList) / sizeof(zclGenericApp_ChannelsOutClusterList[0]))

SimpleDescriptionFormat_t zclGenericApp_ChannelsSimpleDesc[GENERICAPP_CHANNELS_COUNT];

SimpleDescriptionFormat_t zclGenericApp_SimpleDesc =
{
  GENERICAPP_ENDPOINT,                  //  int Endpoint;
  ZCL_HA_PROFILE_ID,                     //  uint16_t AppProfId;
  ZCL_DEVICEID_TEST_DEVICE,          //  uint16_t AppDeviceId;
  GENERICAPP_DEVICE_VERSION,            //  int   AppDevVer:4;
  GENERICAPP_FLAGS,                     //  int   AppFlags:4;
  ZCLGENERICAPP_MAX_INCLUSTERS,         //  byte  AppNumInClusters;
  (cId_t *)zclGenericApp_InClusterList, //  byte *pAppInClusterList;
  ZCLGENERICAPP_MAX_OUTCLUSTERS,        //  byte  AppNumInClusters;
  (cId_t *)zclGenericApp_OutClusterList //  byte *pAppInClusterList;
};


void zclGenericApp_InitChannelsClusters(void) {
    for (uint8_t i = 0; i < GENERICAPP_CHANNELS_COUNT; i++) {
        zclGenericApp_ChannelsSimpleDesc[i] = (SimpleDescriptionFormat_t){
          GENERICAPP_CHANNELS_STARTING_ENDPOINT + i,                              //  int Endpoint;
          ZCL_HA_PROFILE_ID,                  //  uint16_t AppProfId[2];
          ZCL_DEVICEID_GENERIC,               //  uint16_t AppDeviceId[2];
          GENERICAPP_DEVICE_VERSION,            //  int   AppDevVer:4;
          GENERICAPP_FLAGS,                     //  int   AppFlags:4;
          ZCLGENERICAPP_MAX_CHANNELS_INCLUSTERS,         //  byte  AppNumInClusters;
          (cId_t *)zclGenericApp_ChannelsInClusterList, //  byte *pAppInClusterList;
          ZCLGENERICAPP_MAX_CHANNELS_OUTCLUSTERS,        //  byte  AppNumInClusters;
          (cId_t *)zclGenericApp_ChannelsOutClusterList //  byte *pAppInClusterList;
        };
    }
}

// Added to include Touchlink Target functionality
#if defined ( BDB_TL_INITIATOR ) || defined ( BDB_TL_TARGET )
bdbTLDeviceInfo_t tlGenericApp_DeviceInfo =
{
  GENERICAPP_ENDPOINT,                  //uint8_t endpoint;
  ZCL_HA_PROFILE_ID,                        //uint16_t profileID;
  ZCL_DEVICEID_GENERIC,          //uint16_t deviceID;
  GENERICAPP_DEVICE_VERSION,                    //uint8_t version;
  GENERICAPP_NUM_GRPS                   //uint8_t grpIdCnt;
};
#endif

/*********************************************************************
 * GLOBAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */

void zclGenericApp_ResetAttributesToDefaultValues(void)
{

  zclGenericApp_PhysicalEnvironment = PHY_UNSPECIFIED_ENV;
  for (uint8_t i = 0; i < GENERICAPP_CHANNELS_COUNT; i++) {
      zclGenericApp_MutistateInputValues[i] = 0;
      zclGenericApp_ADCValues[i] = 0.0f;
  }

#ifdef ZCL_IDENTIFY
  zclGenericApp_IdentifyTime = 0;
#endif

  /* GENERICAPP_TODO: initialize cluster attribute variables. */
}

/****************************************************************************
****************************************************************************/


