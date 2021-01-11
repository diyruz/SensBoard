/**************************************************************************************************
  Filename:       gp_common.h
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

#ifndef GP_COMMON_H
#define GP_COMMON_H



#ifdef __cplusplus
extern "C"
{
#endif



/*********************************************************************
 * INCLUDES
 */
#include "cgp_stub.h"
#include "nwk_globals.h"

#if (!defined (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE))
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include "zcomdef.h"
#include "zmac.h"
#include "af.h"
#include "gp_proxy.h"
#include "gp_sink.h"
#include "zstack.h"
#include "gp_interface.h"
#endif
/*********************************************************************
 * MACROS
 */

 /*********************************************************************
 * ENUM
 */
enum
{
  DGP_HANDLE_TYPE,
  GPEP_HANDLE_TYPE,
};

 /*********************************************************************
 * CONSTANTS
 */

// GP Shared key
#define GP_SHARED_KEY             { 0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,\
                                    0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF }

#define GP_DATA_IND_QUEUE_MAX_ENTRY    5



#define GP_COMM_OPT_ACTION_MASK                     0x01
#define GP_COMM_OPT_EXIT_ON_WINDOW_EXPIRATION_MASK  0x02
#define GP_COMM_OPT_EXIT_ON_PAIRING_SUCCESS_MASK    0x04
#define GP_COMM_OPT_EXIT_ON_GP_COMM_MODE_EXIT_MASK  0x08
#define GP_COMM_OPT_CHANNEL_PRES_MASK               0x10
#define GP_COMM_OPT_UNICAST_COMM_MASK               0x20

#define GP_SHARED_KEY_TYPE                              0x00
#define GP_INDIVIDUAL_KEY_TYPE                          0x01

#define GP_SECURITY_KEY_TYPE_NO_KEY                     0x00
#define GP_SECURITY_KEY_TYPE_ZIGBEE_NWK_KEY             0x01
#define GP_SECURITY_KEY_TYPE_GPD_GROUP_KEY              0x02
#define GP_SECURITY_KEY_TYPE_NWK_KEY_DERIVED_GPD_GROUP  0x03
#define GP_SECURITY_KEY_TYPE_OUT_OF_BOX_GPD_KEY         0x04
#define GP_SECURITY_KEY_TYPE_DERIVED_IND_GPD_KEY        0x07

#define GP_RSP_CMD_OPT_APP_ID_MASK                       0x07
#define GP_RSP_CMD_OPT_TRANSMIT_ON_ENDPOINT_MATCH_MASK   0x08

#define GP_COMMISSIONING_COMMAND_ID                      0xE0  //Data frame
#define GP_DECOMMISSIONING_COMMAND_ID                    0xE1  //Data frame
#define GP_SUCCESS_COMMAND_ID                            0xE2  //Data frame
#define GP_CHANNEL_REQ_COMMAND_ID                        0xE3  //Maintenance frame
#define GP_COMMISSIONING_REPLY_COMMAND_ID                0xF0  //Data frame
#define GP_CHANNEL_CCONFIG_COMMAND_ID                    0xF3  //Maintenance frame


extern CONFIG_ITEM uint8_t zgpSharedKey[SEC_KEY_LEN];

#define GP_QUEUE_DATA_SEND_INTERVAL 50

 /*********************************************************************
 * TYPEDEFS
 */

typedef uint8_t* (*getHandleFunction_t) (uint8_t handle);


#if (!defined (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE))
typedef struct
{
  gpdID_t       gpdId;                //Address identifier for Green Power device that generates the notification
  uint8_t         GPDEndpoint;          //Endpoint of the GP if application ID is 0b010
  uint32_t        securityFrameCounter; //Security frame counter from Green Power Device frame
  uint8_t         gpCmdId;              //GreenPower Command Id
  uint8_t         DataLen;              //command payload length
  uint8_t        *pData;                //command payload
  uint16_t        GPPAddress;           //Address of the GPP that generates the Notification
} gpdIndication_t;

/*********************************************************************
 * GLOBAL VARIABLES
 */
extern byte gp_TaskID;

extern uint8_t const ppgCommissioningWindow;
extern uint8_t gpApplicationAllowChannelChange;  //Flag to indicate if application allows or not change channel during GP commissioning
extern gp_DataInd_t        *gp_DataIndList;
#endif

extern gp_DataInd_t        *dgp_DataIndList;

#if (!defined (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE))
extern  gp_DataReqPending_t gpTxQueueList[GP_TX_QUEUE_MAX_ENTRY];
extern const uint8_t gGP_TX_QUEUE_MAX_ENTRY;
extern gpCommissioningNotificationMsg_t commissioningNotificationMsg;
extern uint8_t gpAppEntity;
extern Clock_Struct gpAppExpireDuplicateClk;
extern Clock_Handle gpAppExpireDuplicateClkHandle;
extern Clock_Struct gpAppTempMasterTimeoutClk;
extern Clock_Handle gpAppTempMasterTimeoutClkHandle;
#endif
/*********************************************************************
 * FUNCTION MACROS
 */


/*********************************************************************
 * FUNCTIONS
 */

#if (!defined (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE))
#if (defined (USE_ICALL) || defined (OSAL_PORT2TIRTOS))
/*
 * @brief       Initialize the green power module.
 *              This function must be called by the application during its initialization.
 */
extern void app_Green_Power_Init(uint8_t  zclSampleApp_Entity, uint32_t *zclSampleAppEvents,
                          Semaphore_Handle zclSampleAppSem, uint16_t gpDataSendEventValue,
                          uint16_t gpExpireDuplicateEventValue, uint16_t gpTempMasterEventValue);
#endif
/*
 * @brief       This function registers the Green Power endpoint and initializes
 *              the proxy and sink table.
 */
extern void gp_endpointInit( uint8_t entity );

/*
 * @brief   Find if frame is duplicates
 */
extern gp_DataInd_t* gp_DataIndFindDuplicate(uint8_t handle, uint8_t secLvl);

/*
 * @brief       General function fill the proxy table vector item
 */
extern void gp_PopulateField( uint8_t* pField, uint8_t** pData, uint8_t size );

/*
 * @param       currEntryId - NV ID of the proxy table
 *              pNew - New table entry array to be added
 */
extern uint8_t gpLookForGpd( uint16_t currEntryId, uint8_t* pNew );

/*
 * @brief       Primitive from dGP stub to GP EndPoint asking how to process a GPDF.
 */
extern void GP_SecReq(gp_SecReq_t *gp_SecReq);

/*
 * @brief       Copy proxy table memory.
 */
extern void proxyTableCpy(void *dst, void *src);

/*
 * @brief      Copy sink table memory.
 */
extern void sinkTableCpy(void *dst, void *src);

/*
 * @brief       Process the expiration of the packets in the duplicate filtering
 *              list. Assumption is the first in the queue is the first into expire.
 */
extern void gp_expireDuplicateFiltering(void);

#endif

/*
 * @brief       Append a DataInd to a list of DataInd (waiting for GP Sec Rsp List,
 *              or list to filter duplicate packets)
 */
extern void gp_DataIndAppendToList(gp_DataInd_t *gp_DataInd, gp_DataInd_t **DataIndList);

/*
 * @brief  Returns a new handle for the type of msg.
 */
extern uint8_t gp_GetHandle(uint8_t handleType);

/*
* @brief       General function fill uint16_t from pointer.
*/
extern void gp_u16CastPointer( uint8_t *data, uint8_t *p );

/*
* @brief       General function fill uint16_t reversed from pointer.
*/
extern void gp_u16CastPointerReverse( uint8_t *data, uint8_t *p );

/*
* @brief       General function fill uint16_t from pointer.
*/
extern uint16_t gp_aliasDerivation( gpdID_t *pGpdId );

#if (!defined (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE))
/*
 * @brief       Search for a DataInd entry with matching handle
 */
extern gp_DataInd_t* gp_DataIndGet(uint8_t handle);

#if (defined (USE_ICALL) || defined (OSAL_PORT2TIRTOS))
/*
 * @brief       Process green power data indication message from gpStub
 *
 */
extern void gp_processDataIndMsg(zstack_gpDataInd_t *pInMsg);

/*
 * @brief       Process green power security request message from gpStub
 */
extern void gp_processSecRecMsg(zstack_gpSecReq_t *pInMsg);

/*
 * @brief       Process device announce with proxy table for address conflict resolution
 */
extern void gp_processCheckAnnceMsg(zstack_gpCheckAnnounce_t *pInMsg);

/*
 * @brief       Process Green Power Success command
 */
extern void gp_processGpCommissioningSuccesss(zstack_gpCommissioningSuccess_t *pInMsg);

/*
 * @brief       General function fill the proxy table vector
 */
extern void gp_sinkAddProxyEntry( uint8_t* sinkEntry );

/*
 * @brief       Process a Green Power Commissioning command security key
 *              either for key encryption or decryption
 */
extern ZStatus_t gp_processCommissioningKey(gpdID_t *pGPDId, gpdCommissioningCmd_t *pCommissioningCmd, zstack_gpEncryptDecryptCommissioningKeyRsp_t *pKeyRsp);


/*
 * @brief       Notify the application to send a Green Power Notification command
 */
extern void zcl_gpSendNotification(void);

/*
 * @brief       Notify the application to send a Green Power Commissioning Notification command
 */
extern void zcl_gpSendCommissioningNotification(void);

#endif  //(defined (USE_ICALL) || defined (OSAL_PORT2TIRTOS))

#endif  //(!defined (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE))


#ifdef __cplusplus
}
#endif

#endif  /* GP_COMMON_H */
