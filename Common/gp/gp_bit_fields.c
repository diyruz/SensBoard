/**************************************************************************************************
  Filename:       gp_bitFields.c
  Revised:        $Date: 2018-09-13 11:51:49 -0700 (Thu, 13 Feb 2018) $
  Revision:       $Revision: - $

  Description:    This file contains the Green Power bit fields deffinitions.


  Copyright 2006-2015 Texas Instruments Incorporated. All rights reserved.

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
#include "string.h"
#include "zglobals.h"
#include "gp_bit_fields.h"

 #if !defined (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE)

 /*********************************************************************
 * MACROS
 */

 /*********************************************************************
 * CONSTANTS
 */

 /*********************************************************************
 * TYPEDEFS
 */

 /*********************************************************************
 * GLOBAL VARIABLES
 */

 /*********************************************************************
 * LOCAL VARIABLES
 */

 /*********************************************************************
 * LOCAL FUNCTIONS
 */


/*********************************************************************
 * @fn      gp_bfParse_commissioningNotificationOptions
 *
 * @brief   Converts Commissioning Notification options field from
 *          uint16_t to gpCommissioningNotificationOptions_t struct
 *
 * @param   options - pointer to commissioning notification options struct.
 * @param   bitField - Green Power commissioning notification bitfield.
 *
 * @return  ZMemError if not able to allocate
 */
void gp_bfParse_commissioningNotificationOptions( gpCommissioningNotificationOptions_t* pOptions, uint16_t bitField )
{
    pOptions->appId = GP_GET_APPLICATION_ID(LO_UINT16(bitField));
    pOptions->rxAfterTx = GP_GET_RX_AFTER_TX(LO_UINT16(bitField));
    pOptions->securityLevel = GP_CNTF_GET_SEC_LEVEL(LO_UINT16(bitField));
    pOptions->securityKeyType = GP_CNTF_GET_SEC_KEY_TYPE(bitField);
    pOptions->securityProcessingFailed = GP_CNTF_GET_SEC_FAIL(HI_UINT16(bitField));
    pOptions->bidirectionalCapability = GP_CNTF_GET_BIDIRECTIONAL_CAP(HI_UINT16(bitField));
    pOptions->proxyInfoPresent = GP_CNTF_GET_PROXY_INFO(HI_UINT16(bitField));
}

#endif  // (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE)
