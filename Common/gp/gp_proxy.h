/**************************************************************************************************
  Filename:       gp_proxy.h
  Revised:        $Date: 2016-05-23 11:51:49 -0700 (Mon, 23 May 2016) $
  Revision:       $Revision: - $

  Description:    This file contains the common Green Power stub definitions.


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

#if !defined (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE)

#ifndef PROXYBASIC_H
#define PROXYBASIC_H


#ifdef __cplusplus
extern "C"
{
#endif



/*********************************************************************
 * INCLUDES
 */

#include "zcomdef.h"
#include "zmac.h"
#include "af.h"
#include "zcl_green_power.h"
#include "cgp_stub.h"
#include "gp_interface.h"

/*********************************************************************
 * MACROS
 */

 /*********************************************************************
 * ENUM
 */

 /*********************************************************************
 * CONSTANTS
 */

 /*********************************************************************
 * TYPEDEFS
 */
typedef struct
{
  gpdID_t       gpdId;                //Address identifier for Green Power Device that is in commissioning.
  uint8_t         keyPresent;           //Flag that tells if key is present on commissioning frame.
  uint8_t         pkey[SEC_KEY_LEN];    //Pointer to security key.
} gpdCommissioningDevice_t;

/*********************************************************************
 * GLOBAL VARIABLES
 */
extern uint8_t  zgGP_ProxyCommissioningMode;     //Global flag that states if in commissioning mode or in operational mode.
extern uint16_t gp_commissionerAddress;          //Address to which send the notifications during commissioning mode
extern uint8_t  gp_unicastCommunication;         //Flag to indicate if the commissioner requested unicast communication or not.
#if !defined (USE_ICALL)
extern ZDO_DeviceAnnce_t*                  GP_aliasConflictAnnce;
#endif

/*********************************************************************
 * FUNCTION MACROS
 */


/*********************************************************************
 * FUNCTIONS
 */

/*
 * @brief   Callback from the ZCL GreenPower Cluster Library when
 *          it received an Gp Pairing Command for this application.
 */
extern void zclGp_GpPairingCommandCB( zclGpPairing_t *pCmd );

/*
 * @brief   Callback from the ZCL GreenPower Cluster Library when
 *          it received a Gp proxy table req.
 */
extern void zclGp_GpProxyTableReqCB( zclGpTableRequest_t *pCmd );

/*
 * @brief   Callback from the ZCL GreenPower Cluster Library when
 *          it received a Gp Response command. (Ref A.3.5.2.1)
 */
extern void zclGp_GpResponseCommandCB(zclGpResponse_t *pCmd);

/*
 * @brief   Callback from the ZCL GreenPower Cluster Library when
 *          it received a Gp Commissioning Mode command.
 */
extern void zclGp_GpProxyCommissioningModeCB(zclGpProxyCommissioningMode_t* pCmd);

/*
 * @brief       General function to init the NV items for proxy table
 */
extern uint8_t gp_ProxyTblInit( uint8_t resetTable );

/*
 * @brief   Stop Commissioning mode, either timeout or pairing success
 */
extern void gp_stopCommissioningMode(void);

/*
 * @brief   General function to get proxy table entry by gpdID (GP Src ID or Extended Adddress)
 */
extern uint8_t gp_getProxyTableByGpId(gpdID_t *gpdID, uint8_t *pEntry, uint16_t* NvProxyTableIndex);

/*
 * @brief   General function to get proxy table entry by NV index
 */
extern uint8_t gp_getProxyTableByIndex( uint16_t nvIndex, uint8_t *pEntry );

/*
 * @brief   Handle Gp attributes.
 */
extern ZStatus_t zclGpp_ReadWriteAttrCB( uint16_t clusterId, uint16_t attrId, uint8_t oper,
                                         uint8_t *pValue, uint16_t *pLen );

/*
 * @brief       General function fill the proxy table vector
 */
extern void gp_PairingUpdateProxyTbl( gpPairingCmd_t* payload );

/*
 * @brief This passes the MCPS data indications received in MAC to the application
 */
extern void gp_dataIndProxy(gp_DataInd_t *gp_DataInd);

/*
 * @brief       Performs Security Operations according to Proxy
 */
extern uint8_t gp_SecurityOperationProxy( gp_SecReq_t* pInd, uint8_t* pKeyType, void* pKey);

/*
 * @brief Primitive to notify GP EndPoint the status of a previews DataReq
 */
extern void GP_DataCnf(gp_DataCnf_t *gp_DataCnf);

/*
 * @brief   Register a callback in which the application will be notified about
 *          commissioning mode indication
 */
extern void gp_RegisterCommissioningModeCB(gpCommissioningMode_t gpCommissioningModeCB);

/*
 * @brief   Return to operational channel after commissioning a GPD
 */
extern void gp_returnOperationalChannel(void);

/*
* @brief       Get Green Power Proxy commissioning mode
*/
extern bool gp_GetProxyCommissioningMode(void);

/*
 * @brief       Enable or disable Green Power Proxy commissioning mode
 */
extern void gp_SetProxyCommissioningMode(bool enabled);

/*
 * @brief       General function to check if it has the announced device
 *              listed in the SinkAddressList and look for address conflict
 *              resolution.
 */
extern void gp_CheckAnnouncedDevice ( uint8_t *sinkIEEE, uint16_t sinkNwkAddr );

/*
 * @brief       Populate the given item data
 */
extern uint8_t pt_ZclReadGetProxyEntry( uint16_t nvId, uint8_t* pData, uint8_t* len );

/*
 * @brief       To update the proxy table NV vectors
 */
extern uint8_t gp_UpdateProxyTbl( uint8_t* pEntry, uint32_t options, uint8_t conflictResolution );

#ifdef __cplusplus
}
#endif


#endif /* PROXYBASIC_H */

#endif // #if !defined (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE)
