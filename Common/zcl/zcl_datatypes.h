/**************************************************************************************************
  Filename:       zcl_datatypes.h
  Revised:        $Date: 2019-6-18 09:19:55 -0800 (Tue, 25 Nov 2014) $
  Revision:       $Revision: 41240 $

  Description:    This file contains the Zigbee Cluster Library Foundation definitions.


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
#define map8                                        uint8_t
#define map16                                       uint16_t
#define map24                                       uint32_t
#define map32                                       uint32_t

#define enum8                                       uint8_t
#define enum16                                      uint16_t

#define CCColorLoopDirection                        uint8_t
#define SGroupId                                    uint16_t
#define UTC                                         uint32_t //  unsigned 32-bit - number of seconds since January 1st, 2000 UTC

#define CCColorOptions                              map8
#define LevelOptions                                map8
#define BatteryAlarmMask                            map8
#define IasZoneStatus                               map16
#define BatteryAlarmState                           map32

#define BatterySize                                 enum8
#define PumpOperationMode                           enum8
#define DrlkOperMode                                enum8
#define PumpControlMode                             enum8
#define IasZoneType                                 enum16

#define single                                      uint32_t				// Length of mantissa = 23 bits, length of exponent = 8 bits
#define EUI64		                                    uint64_t    	// IEEE Address
// #define key128	                                    uint128_t	    // 128-bit security key
// #define CSKey                                       key128			  // check paramater

#define octstr                                      uint8_t*
