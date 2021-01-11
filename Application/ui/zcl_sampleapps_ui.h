 /**************************************************************************************************
  Filename:       zcl_sampleapps_ui.h
  Revised:        $Date: 2016-08-01 08:38:22 -0700 (Thu, 19 Jun 2014) $
  Revision:       $Revision: 39101 $

  Description:    This file contains the generic Sample Application User Interface.


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

#ifndef ZCL_SAMPLEAPPS_UI_H
#define ZCL_SAMPLEAPPS_UI_H


#ifndef CUI_DISABLE

#include "zstackmsg.h"
#include "zstackapi.h"
#include "cui.h"
#include <string.h>
#include "zcl_sample_app_def.h"
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/drivers/apps/Button.h>
#include "zcl_sample_app_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * MACROS
 */

// UI Events
#define SAMPLEAPP_UI_BDB_NWK_LINE_UPDATE_EVT  0x0020
#define SAMPLEAPP_UI_INPUT_EVT                0x0040
#define SAMPLEAPP_UI_GP_LINE_UPDATE_EVT       0x0100
#define SAMPLEAPP_KEY_EVT_UI                  0x0200

/*********************************************************************
 * TYPEDEFS
 */

typedef void (* uiAppFNResetCB_t)(void);
typedef void (* uiAppProcessKeyCB_t)(uint8_t key, Button_EventMask _buttonEvents);

/*********************************************************************
* FUNCTIONS
*/

/*
 * User-Interface intialization
 */
CUI_clientHandle_t UI_Init(uint8_t  zclSampleApp_Entity, uint32_t *zclSampleAppEvents, Semaphore_Handle zclSampleAppSem,
              uint16_t *ui_IdentifyTimeAttribute_value, uint16_t *defaultBdbCommisioningModes,
              CONST char *pAppStr, uiAppProcessKeyCB_t zclSampleApp_processKey, uiAppFNResetCB_t _uiAppFNResetCB);

/*
 * Process a change in the device's network-state
 */
void UI_DeviceStateUpdated(zstack_devStateChangeInd_t *pReq);
/*
 * Process updating the Nwk Status line
 */
void UI_UpdateNwkStatusLine(void);
/*
 * Process updating the Device Info line
 */
void UI_UpdateDeviceInfoLine(void);

/*
 * Process updating the Bdb Status line
 */
void UI_UpdateBdbStatusLine(bdbCommissioningModeMsg_t *bdbCommissioningModeMsg);
/*
 * Process updating the Bind info
 */
void UI_UpdateBindInfoLine(void);

#if !defined (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE)
/*
 * Process updating the Gp Status line
 */
void UI_UpdateGpStatusLine(void);
#endif

void zclsampleApp_ui_event_loop(void);

void uiProcessIdentifyTimeChange( uint8_t *endpoint );

void uiProcessBindNotification( bdbBindNotificationData_t *data );

#if !defined (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE)
/*
 * @brief   Interface to set the GP Proxy commissioning mode
 */
void UI_SetGPPCommissioningMode( zstack_gpCommissioningMode_t *Req );
#endif

#ifdef __cplusplus
}
#endif

#endif //#ifndef CUI_DISABLE

#endif
