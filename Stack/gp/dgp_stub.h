/**************************************************************************************************
  Filename:       cgp_stub.h
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



#ifndef DGP_STUB_H
#define DGP_STUB_H


#ifdef __cplusplus
extern "C"
{
#endif


/*********************************************************************
 * INCLUDES
 */
#include "cgp_stub.h"

 /*********************************************************************
 * CONSTANTS
 */

 /*********************************************************************
 * MACROS
 */


#define GP_NONCE_SEC_CONTROL_OUTGOING_APP_ID_GP   0xC5
#define GP_NONCE_SEC_CONTROL                      0x05


#define GP_TRANSMIT_SCHEDULED_FRAME_EVENT     0x0001
#define GP_DUPLICATE_FILTERING_TIMEOUT_EVENT  0x0002
#define GP_CHANNEL_CONFIGURATION_TIMEOUT      0x0004
#define GP_PROXY_ALIAS_CONFLICT_TIMEOUT       0x0008

 /*********************************************************************
 * TYPEDEFS
 */


typedef void   (*GP_DataCnfGCB_t)(gp_DataCnf_t* gp_DataCnf);
typedef void   (*GP_endpointInitGCB_t)(void);
typedef void   (*GP_expireDuplicateFilteringGCB_t)(void);
typedef void   (*GP_stopCommissioningModeGCB_t)(void);
typedef void   (*GP_returnOperationalChannelGCB_t)(void);
typedef void   (*GP_DataIndGCB_t)(gp_DataInd_t* gp_DataInd);
typedef void   (*GP_SecReqGCB_t)(gp_SecReq_t* gp_SecReq);
typedef void   (*GP_CheckAnnouncedDevice_t) ( uint8_t *sinkIEEE, uint16_t sinkNwkAddr );

typedef struct
{
gpEventHdr_t hdr;
uint32_t       timestamp;         //Timestamp in backoff units
sAddr_t      srcAddr;
sAddr_t      dstAddr;
uint16_t       srcPanID;
uint16_t       dstPanID;
int8_t         Rssi;
uint8_t        LinkQuality;
uint8_t        SeqNumber;
uint8_t        mpduLen;
uint8_t        mpdu[1];            //This is a place holder for the buffer, its length depends on mpdu_len
}dgp_DataInd_t;


#define GP_NONCE_LENGTH   13
typedef struct
{
uint8_t  SourceAddr[Z_EXTADDR_LEN];
uint32_t FrameCounter;
uint8_t  securityControl;
}gp_AESNonce_t;



/*********************************************************************
 * GLOBAL VARIABLES
 */

/* Callbacks for GP endpoint */
extern GP_DataCnfGCB_t                   GP_DataCnfGCB;
extern GP_expireDuplicateFilteringGCB_t  GP_expireDuplicateFilteringGCB;
extern GP_stopCommissioningModeGCB_t     GP_stopCommissioningModeGCB;
extern GP_DataIndGCB_t                   GP_DataIndGCB;
extern GP_SecReqGCB_t                    GP_SecReqGCB;
extern GP_CheckAnnouncedDevice_t         GP_CheckAnnouncedDeviceGCB;

 /*********************************************************************
 * FUNCTION MACROS
 */


 /*********************************************************************
 * FUNCTIONS
 */

/*
 * @brief       Handles GPDF to pass to application as GP SecReq to be validated.
 */
extern void dGP_DataInd(dgp_DataInd_t *dgp_DataInd);

/*
 * @brief       Notify the application about a GPDF being delivered
 */
extern void dGP_DataCnf(uint8_t GPmpduHandle);

/*
 * @brief   Find entry by handle in the list of dGP-DataInd pendings by GP-SecReq
 */
extern uint8_t* dGP_findHandle(uint8_t handle);

/*
 * @brief       Primitive by GP EndPoint to pass to dGP stub a request to send GPDF to a GPD
 */
extern bool GP_DataReq(gp_DataReq_t *gp_DataReq);

/*
 * @brief       Response to dGP stub from GP endpoint on how to process the GPDF
 */
extern void  GP_SecRsp(gp_SecRsp_t *gp_SecRsp);

#if (defined (USE_ICALL) || defined (OSAL_PORT2TIRTOS))
/*
 * @brief       Send a Green Power security response to Green Power Stub
 */
extern void sendGpSecRsp(gp_SecRsp_t *rsp);

/*
 * @brief       Resolves Address Conflict
 */
extern void GpResolveAddressConflict(uint16_t nwkAddr, bool conflict);

/*
 * @brief   Security and authentication processing function dedicated to GP.
 */
extern ZStatus_t gp_ccmStarKey(gpdID_t *gpdId, uint8_t keyType, uint8_t *pEncKey, uint8_t *pMic, uint8_t *pSecKey, uint32_t secFrameCounter, uint8_t encrypt);
#endif

/*
* @brief   Security and authentification processing function dedicated to GP.
*/
extern uint8_t gp_ccmStar(gp_DataInd_t *gpDataInd, uint8_t* key);

#ifdef __cplusplus
}
#endif


#endif /* DGP_STUB_H */


