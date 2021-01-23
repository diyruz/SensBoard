/**************************************************************************************************
  Filename:       zcl_genericapp.h
  Revised:        $Date: 2014-06-19 08:38:22 -0700 (Thu, 19 Jun 2014) $
  Revision:       $Revision: 39101 $

  Description:    This file contains the ZigBee Cluster Library Home
                  Automation Sample Application.


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

#ifndef ZCL_GENERICAPP_H
#define ZCL_GENERICAPP_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * INCLUDES
 */
#include "nvintf.h"
#include "zcl.h"

// Added to include Touchlink Target functionality
#if defined(BDB_TL_INITIATOR) || defined(BDB_TL_TARGET)
#include "bdb_tl_commissioning.h"
#include "zcl_general.h"

#endif

/*********************************************************************
 * CONSTANTS
 */
#define GENERICAPP_ENDPOINT 1
#define GENERICAPP_CHANNELS_COUNT 3
#define GENERICAPP_CHANNELS_STARTING_ENDPOINT 2
#define GENERICAPP_CHANNEL_ATTRS_COUNT 2

// Added to include Touchlink Target functionality
#define GENERICAPP_NUM_GRPS 2

// Application Events
#define GENERICAPP_MAIN_SCREEN_EVT 0x0001
#define GENERICAPP_KEY_EVT 0x0002
#define GENERICAPP_END_DEVICE_REJOIN_EVT 0x0004
#define GENERICAPP_COUNTER_PIN_EVT 0x0008
#define GENERICAPP_SENSORS_SAMPLING_EVT 0x0010
#define GENERICAPP_BATTERY_SAMPLING_EVT 0x0020
#define GENERICAPP_INIT_SENSORS_READING_EVT 0x0040

/* GENERICAPP_TODO: define app events here */

#define GENERICAPP_EVT_1 0x0008
/*
#define GENERICAPP_EVT_2                    0x0010
#define GENERICAPP_EVT_3                    0x0020
*/

// Green Power Events
#define SAMPLEAPP_PROCESS_GP_DATA_SEND_EVT 0x0100
#define SAMPLEAPP_PROCESS_GP_EXPIRE_DUPLICATE_EVT 0x0200
#define SAMPLEAPP_PROCESS_GP_TEMP_MASTER_EVT 0x0400

// Application Display Modes
#define GENERIC_MAINMODE 0x00
#define GENERIC_HELPMODE 0x01

#define GENERICAPP_END_DEVICE_REJOIN_DELAY 1000
#define GENERICAPP_SENSORS_READING_INTERVAL (5 * 60 * 1000) // 5 min
#define GENERICAPP_BATTERY_SAMPLING_INTERVAL (30 * 60 * 1000) // 30 min
#define GENERICAPP_SENSORS_POWER_ON_TIME 50 // in millisecons

/*********************************************************************
 * MACROS
 */
/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * VARIABLES
 */

// Added to include Touchlink Target functionality
#if defined(BDB_TL_INITIATOR) || defined(BDB_TL_TARGET)
extern bdbTLDeviceInfo_t tlGenericApp_DeviceInfo;
#endif

extern SimpleDescriptionFormat_t zclGenericApp_SimpleDesc;
extern SimpleDescriptionFormat_t zclGenericApp_ChannelsSimpleDesc[];

extern CONST zclCommandRec_t zclGenericApp_Cmds[];

extern CONST uint8_t zclCmdsArraySize;

extern int16_t zclGenericApp_deviceTemperature;
extern uint8_t zclGenericApp_batteryVoltage;

// attribute list
extern CONST zclAttrRec_t zclGenericApp_Attrs[];
extern CONST zclAttrRec_t zclGenericApp_ChannelAttrs[][GENERICAPP_CHANNEL_ATTRS_COUNT];

extern CONST uint8_t zclGenericApp_NumAttributes;

extern uint16_t zclGenericApp_MutistateInputValues[GENERICAPP_CHANNELS_COUNT];
extern float zclGenericApp_ADCValues[GENERICAPP_CHANNELS_COUNT];
// Identify attributes
#ifdef ZCL_IDENTIFY
extern uint16_t zclGenericApp_IdentifyTime;
#endif
extern uint8_t zclGenericApp_IdentifyCommissionState;

// GENERICAPP_TODO: Declare application specific attributes here

/*********************************************************************
 * FUNCTIONS
 */

extern void zclGenericApp_ResetAttributesToDefaultValues(void);
extern void zclGenericApp_InitChannelsClusters(void);

#ifdef __cplusplus
}
#endif

#endif /* ZCL_GENERICAPP_H */
