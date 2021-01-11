/******************************************************************************

 @file  mac_high_level.h

 @brief Contains interfaces shared between high and low level MAC.

 Group: WCS, LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2006-2020, Texas Instruments Incorporated
 All rights reserved.

 IMPORTANT: Your use of this Software is limited to those specific rights
 granted under the terms of a software license agreement between the user
 who downloaded the software, his/her employer (which must be your employer)
 and Texas Instruments Incorporated (the "License"). You may not use this
 Software unless you agree to abide by the terms of the License. The License
 limits your use, and you acknowledge, that the Software may not be modified,
 copied or distributed unless embedded on a Texas Instruments microcontroller
 or used solely and exclusively in conjunction with a Texas Instruments radio
 frequency transceiver, which is integrated into your product. Other than for
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

 ******************************************************************************
 
 
 *****************************************************************************/

#ifndef MAC_HIGH_LEVEL_H
#define MAC_HIGH_LEVEL_H

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */

#include "mac_api.h"
#include "mac_ie_parse.h"

/* ------------------------------------------------------------------------------------------------
 *                                           Macros
 * ------------------------------------------------------------------------------------------------
 */
#ifndef MAC_INTERNAL_API
#define MAC_INTERNAL_API
#endif

/* Call macPwrVote() only outside swi context */
#define MAC_PWR_VOTE_NO_SWI(x)        do { if (!bInSwi) MAP_macPwrVote(x, FALSE); } while (0);

/* ------------------------------------------------------------------------------------------------
 *                                           Constants
 * ------------------------------------------------------------------------------------------------
 */

/* RX flag masks */
#define MAC_RX_FLAG_VERSION           0x03    /* received frame's version */
#define MAC_RX_FLAG_ACK_PENDING       0x04    /* outgoing ACK has pending bit set */
#define MAC_RX_FLAG_SECURITY          0x08    /* received frame has security bit set */
#define MAC_RX_FLAG_PENDING           0x10    /* received frame has pending bit set */
#define MAC_RX_FLAG_ACK_REQUEST       0x20    /* received frame has ack request bit set */
#define MAC_RX_FLAG_INTRA_PAN         0x40    /* received frame has intra pan fcf bit set */
#define MAC_RX_FLAG_CRC_OK            0x80    /* received frame CRC OK bit */

/* Enhanced Beacon Request offsets for RX */
#define MAC_RX_EBR_EB_FILTER_IE_HEADER_LEN 0x02 /* received frame has 2 bytes
                                                 * for these - Type, ID and len
                                                 */

/* Enhanced Beacon Request payload bit position masks */
#define MAC_RX_EBR_PERMIT_JOINING_ON      0x01
#define MAC_RX_EBR_INCLUDE_LINK_QUALITY   0x02
#define MAC_RX_EBR_INCLUDE_PERCENT_FILTER 0x04

/* Enhanced Beacon Request End of IE List indication as per Table 4b of draft
 * for IEEE 802.15.4e
 *
 * Payload ID name space has terminator byte = 0x0F (ID) (bits 1,2,3,4)
 * Type of IE = 0x01
 */
#define MAC_RX_EBR_IE_PAYLOAD_LIST_TERMINATOR        0x1F
#define MAC_RX_EBR_IE_PAYLOAD_LIST_TERMINATOR_IE_LEN 0x03

/* FH Frame Types */
#define MAC_FH_FRAME_PAN_ADVERT         0x00    /* WiSUN PAN advertisement */
#define MAC_FH_FRAME_PAN_ADVERT_SOLICIT 0x01    /* WiSUN PAN advertisement solicit */
#define MAC_FH_FRAME_PAN_CONFIG         0x02    /* WiSUN PAN config */
#define MAC_FH_FRAME_PAN_CONFIG_SOLICIT 0x03    /* WiSUN PAN config solicit */
#define MAC_FH_FRAME_DATA               0x04    /* WiSUN Data frame */
#define MAC_FH_FRAME_ACK                0x05    /* WiSUN Ack frame */
#define MAC_FH_FRAME_EAPOL              0x06    /* WiSUN EAPOL frame */
#define MAC_FH_FRAME_INVALID            0xFF    /* Internal: WiSUN Invalid frame */

/* ------------------------------------------------------------------------------------------------
 *                                           Typedefs
 * ------------------------------------------------------------------------------------------------
 */

/* Structure for internal data tx */
typedef struct
{
  macEventHdr_t     hdr;
  sData_t           msdu;
  macTxIntData_t    internal;
  ApiMac_sec_t          sec;
} macTx_t;

struct _macHeaderIeInfo;
struct _macPayloadIeInfo;

/* Structure for internal data rx */
typedef struct _macRx
{
  macEventHdr_t       hdr;
  sData_t             msdu;
  macRxIntData_t      internal;
  ApiMac_sec_t            sec;
  macDataInd_t        mac;
  sData_t             mhr;
  uint8               payloadIePresent;
  uint8*              pHdrIes;
  uint16              headerIeLen;
  struct _macHeaderIeInfo*  pHeaderIes;
  struct _macPayloadIeInfo* pPayloadIes;
} macRx_t;

/* Function pointer for the 16 byte random seed callback */
typedef void (*macRNGFcn_t )(uint8* seed);

/* ------------------------------------------------------------------------------------------------
 *                                           Global Variables
 * ------------------------------------------------------------------------------------------------
 */

/* ------------------------------------------------------------------------------------------------
 *                                          Function Prototypes
 * ------------------------------------------------------------------------------------------------
 */

/* functions located in mac_data.c */
MAC_INTERNAL_API uint8 *macDataRxMemAlloc(uint16 len);
MAC_INTERNAL_API uint8 macDataRxMemFree(uint8 **pMsg);
MAC_INTERNAL_API uint16 macDataTxTimeAvailable(void);

/* functions located in mac_pwr.c */
MAC_INTERNAL_API void macPwrVote(bool pwrUp, bool bypassRAT);

/**************************************************************************************************
*/

#endif /* MAC_HIGH_LEVEL_H */
