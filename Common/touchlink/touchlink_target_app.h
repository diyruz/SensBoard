/**************************************************************************************************
  Filename:       touchlink_target_app.h
  Revised:        $Date: 2013-05-10 15:34:14 -0700 (Fri, 10 May 2013) $
  Revision:       $Revision: 34239 $

  Description:    This file contains the ZCL Touch Link Target definitions.


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

#ifndef TOUCHLINK_TARGET_APP_H
#define TOUCHLINK_TARGET_APP_H

#if defined ( BDB_TL_TARGET )

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "zcl_general.h"
#include "bdb_tl_commissioning.h"

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */
//! \brief Callback function type for starting bdb finding and binding
typedef void (*tl_BDBFindingBindingCb_t)(void);

/*********************************************************************
 * VARIABLES
 */
extern uint8_t touchlinkAppAllowStealing;;

/*********************************************************************
 * FUNCTIONS
 */

/*
 * @brief       Initialize the touchlink target module.
 *              This function must be called by the application during its initialization.
 */
extern void touchLinkTargetApp_Init(uint8_t  zclSampleApp_Entity);

/*
 * @brief       Register application finding and binding callback
 */
extern void touchLinkApp_registerFindingBindingCb(tl_BDBFindingBindingCb_t comCb);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* BDB_TL_TARGET */

#endif /* TOUCHLINK_TARGET_APP_H */
