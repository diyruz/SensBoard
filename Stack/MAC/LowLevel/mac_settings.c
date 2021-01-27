/******************************************************************************

 @file  mac_settings.c

 @brief This file contains CM0 radio command variables.

 Group: WCS, LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2015-2021, Texas Instruments Incorporated
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

/* ------------------------------------------------------------------------------------------------
 *                                           Includes
 * ------------------------------------------------------------------------------------------------
 */

#ifndef LIBRARY
#include "ti_154stack_features.h"
#endif

#include "mac.h"
#include "mac_user_config.h"
#include "mac_settings.h"
#include "rom_jt_154.h"

extern macUserCfg_t *pMacRadioConfig;
extern RF_Mode *pRfMode;
extern RF_TxPowerTable_Entry *pRfPowerTable;
extern macRfCfg_maxPower_t *pMaxPower;

#if defined(COMBO_MAC) || defined(FREQ_2_4G)
// CMD_RADIO_SETUP for IEEE 15.4
rfc_CMD_RADIO_SETUP_PA_t RF_cmdRadioSetup;
// CMD_FS Rx
rfc_CMD_FS_t RF_cmdFsRx;
// CMD_FS Tx
rfc_CMD_FS_t RF_cmdFsTx;
/* IEEE TX Command */
rfc_CMD_IEEE_TX_t RF_cmdIEEETx;
/* IEEE RX Command */
rfc_CMD_IEEE_RX_t RF_cmdIEEERx;
rfc_CMD_IEEE_RX_ACK_t RF_cmdRxAckCmd;
rfc_CMD_IEEE_CSMA_t RF_cmdIEEECsma;

void rfSetConfigIeee(macRfCfg_ieee_t *pRfCfg)
{
    pRfMode = pRfCfg->pRfMode;
    pRfPowerTable = pRfCfg->pRfPowerTable;
    memcpy(&RF_cmdRadioSetup, pRfCfg->pRfSetup, sizeof(rfc_CMD_RADIO_SETUP_PA_t));

    memcpy(&RF_cmdFsRx, pRfCfg->pRfFs, sizeof(rfc_CMD_FS_t));
    memcpy(&RF_cmdFsTx, pRfCfg->pRfFs, sizeof(rfc_CMD_FS_t));
    memcpy(&RF_cmdIEEETx, pRfCfg->pRfTx, sizeof(rfc_CMD_IEEE_TX_t));
    RF_cmdIEEETx.startTrigger.pastTrig = 1;
    RF_cmdIEEETx.condition.rule = COND_NEVER;

    memcpy(&RF_cmdIEEERx, pRfCfg->pRfRx, sizeof(rfc_CMD_IEEE_RX_t));
    RF_cmdIEEERx.condition.rule = COND_NEVER;
    RF_cmdIEEERx.rxConfig.bAutoFlushCrc = 0x1;
    RF_cmdIEEERx.rxConfig.bAutoFlushIgn = 0x0;
    RF_cmdIEEERx.rxConfig.bIncludePhyHdr = 0x1;
    RF_cmdIEEERx.rxConfig.bIncludeCrc = 0x0;
    RF_cmdIEEERx.rxConfig.bAppendRssi = 0x1;
    RF_cmdIEEERx.rxConfig.bAppendCorrCrc = 0x0;
    RF_cmdIEEERx.rxConfig.bAppendSrcInd = 0x0;
    RF_cmdIEEERx.rxConfig.bAppendTimestamp = 0x1;
    RF_cmdIEEERx.frameFiltOpt.maxFrameVersion = 0x3;
    RF_cmdIEEERx.frameTypes.bAcceptFt0Beacon = 0x1;
    RF_cmdIEEERx.frameTypes.bAcceptFt1Data = 0x1;
    RF_cmdIEEERx.frameTypes.bAcceptFt2Ack = 0x1;
    RF_cmdIEEERx.frameTypes.bAcceptFt3MacCmd = 0x1;
    RF_cmdIEEERx.frameTypes.bAcceptFt4Reserved = 0x1;
    RF_cmdIEEERx.frameTypes.bAcceptFt5Reserved = 0x1;
    RF_cmdIEEERx.frameTypes.bAcceptFt6Reserved = 0x1;
    RF_cmdIEEERx.frameTypes.bAcceptFt7Reserved = 0x1;
    RF_cmdIEEERx.ccaOpt.ccaEnEnergy = 0x0;
    RF_cmdIEEERx.ccaOpt.ccaEnCorr = 0x0;
    RF_cmdIEEERx.ccaOpt.ccaEnSync = 0x0;
    RF_cmdIEEERx.ccaOpt.ccaCorrOp = 0x1;
    RF_cmdIEEERx.ccaOpt.ccaSyncOp = 0x1;
    RF_cmdIEEERx.ccaOpt.ccaCorrThr = 0x0;
    RF_cmdIEEERx.ccaRssiThr = 0x64;
    RF_cmdIEEERx.endTrigger.triggerType = 0x1;

    memcpy(&RF_cmdIEEECsma, pRfCfg->pRfCs, sizeof(rfc_CMD_IEEE_CSMA_t));
    RF_cmdIEEECsma.pNextOp = (rfc_radioOp_t *)&RF_cmdIEEETx;
    RF_cmdIEEECsma.startTrigger.pastTrig = 1;
    RF_cmdIEEECsma.condition.rule = COND_STOP_ON_FALSE;
    RF_cmdIEEECsma.macMaxBE = 5;
    RF_cmdIEEECsma.macMaxCSMABackoffs = 4;
    RF_cmdIEEECsma.csmaConfig.initCW = 1;
    RF_cmdIEEECsma.NB = 0;
    RF_cmdIEEECsma.BE = 3;
    RF_cmdIEEECsma.endTrigger.triggerType = 0x1;

    memcpy(&RF_cmdRxAckCmd, pRfCfg->pRxAck, sizeof(rfc_CMD_IEEE_RX_ACK_t));
    RF_cmdRxAckCmd.startTrigger.pastTrig = 1;
    RF_cmdRxAckCmd.endTrigger.triggerType = TRIG_REL_START;
    RF_cmdRxAckCmd.endTrigger.pastTrig = 1;
    RF_cmdRxAckCmd.condition.rule = COND_NEVER;
    RF_cmdRxAckCmd.endTime = ((IEEE802154_MAC_ACK_WAIT_DURATION * PLATFORM_RADIO_RAT_TICKS_PER_SEC)/IEEE802154_SYMBOLS_PER_SEC);

    pMaxPower = (macRfCfg_maxPower_t *)pRfCfg->data;

    if(macSrcMatchIsEnabled)
    {
        MAC_SrcMatchEnable();
    }

    /* Setup MAC RX command */
    macSetupReceiveCmd();
}
#endif
#if defined(COMBO_MAC) || !defined(FREQ_2_4G)
// CMD_PROP_RADIO_DIV_SETUP
rfc_CMD_PROP_RADIO_DIV_SETUP_PA_t RF_cmdPropRadioDivSetup;
// CMD_FS Rx
rfc_CMD_FS_t RF_cmdFsRx;
// CMD_FS Tx
rfc_CMD_FS_t RF_cmdFsTx;
// CMD_PROP_TX_ADV
rfc_CMD_PROP_TX_ADV_t RF_cmdPropTxAdv;
// CMD_PROP_RX_ADV
rfc_CMD_PROP_RX_ADV_t RF_cmdPropRxAdv;
// CMD_PROP_CS for CSMA or LBT
rfc_CMD_PROP_CS_t RF_cmdPropCs;
// CMD_PROP_CS
rfc_CMD_PROP_CS_t RF_cmdPropCsSlotted;

void rfSetConfigSubG(macRfCfg_prop_t *pRfCfg)
{
    size_t setupLength;
    ChipType_t chipType = ChipInfo_GetChipType();

    pRfMode = pRfCfg->pRfMode;
    pRfPowerTable = pRfCfg->pRfPowerTable;
    memset(&RF_cmdPropRadioDivSetup, 0, sizeof(rfc_CMD_PROP_RADIO_DIV_SETUP_PA_t));
    if(chipType == CHIP_TYPE_CC1352P)
    {
        setupLength = sizeof(rfc_CMD_PROP_RADIO_DIV_SETUP_PA_t);
    }
    else
    {
        setupLength = sizeof(rfc_CMD_PROP_RADIO_DIV_SETUP_t);
    }
    memcpy(&RF_cmdPropRadioDivSetup, pRfCfg->pRfSetup, setupLength);
    RF_cmdPropRadioDivSetup.startTrigger.pastTrig = 1;
    RF_cmdPropRadioDivSetup.formatConf.bBitReversal = 0;
    RF_cmdPropRadioDivSetup.formatConf.bMsbFirst = 1;
    RF_cmdPropRadioDivSetup.formatConf.whitenMode = 7;
    RF_cmdPropRadioDivSetup.intFreq = 0x8000;

    memcpy(&RF_cmdFsRx, pRfCfg->pRfFs, sizeof(rfc_CMD_FS_t));
    RF_cmdFsRx.startTrigger.pastTrig = 1;

    memcpy(&RF_cmdFsTx, pRfCfg->pRfFs, sizeof(rfc_CMD_FS_t));
    RF_cmdFsTx.startTrigger.pastTrig = 1;


    memcpy(&RF_cmdPropTxAdv, pRfCfg->pRfTx, sizeof(rfc_CMD_PROP_TX_ADV_t));
    RF_cmdPropTxAdv.startTrigger.triggerType = TRIG_NOW;
    RF_cmdPropTxAdv.startTrigger.pastTrig = 1;
#ifdef MAC_PROTOCOL_TEST
    RF_cmdPropTxAdv.pktConf.bUseCrc = 0;
#else
    RF_cmdPropTxAdv.pktConf.bUseCrc = 1;
#endif
    RF_cmdPropTxAdv.numHdrBits = 0x10;
    RF_cmdPropTxAdv.pktLen = 0;
    RF_cmdPropTxAdv.preTrigger.triggerType = TRIG_REL_START;
    RF_cmdPropTxAdv.preTrigger.pastTrig = 1;

    memcpy(&RF_cmdPropRxAdv, pRfCfg->pRfRx, sizeof(rfc_CMD_PROP_RX_ADV_t));
    RF_cmdPropRxAdv.startTrigger.pastTrig = 1;
    RF_cmdPropRxAdv.pktConf.bFsOff = 0x0,
    RF_cmdPropRxAdv.pktConf.bRepeatOk = 0x0;
    RF_cmdPropRxAdv.pktConf.bRepeatNok = 0x0;
    RF_cmdPropRxAdv.pktConf.bUseCrc = 0x1;
    RF_cmdPropRxAdv.pktConf.bCrcIncSw = 0x0;
    RF_cmdPropRxAdv.pktConf.bCrcIncHdr = 0x0;
    RF_cmdPropRxAdv.pktConf.endType = 0x0;
    RF_cmdPropRxAdv.pktConf.filterOp = 0x1;
    RF_cmdPropRxAdv.rxConf.bAutoFlushIgnored = 0x0;
    RF_cmdPropRxAdv.rxConf.bAutoFlushCrcErr = 0x1;
    RF_cmdPropRxAdv.rxConf.bIncludeHdr = 0x1;
    RF_cmdPropRxAdv.rxConf.bIncludeCrc = 0x0;
    RF_cmdPropRxAdv.rxConf.bAppendRssi = 0x1;
    RF_cmdPropRxAdv.rxConf.bAppendTimestamp = 0x1;
    RF_cmdPropRxAdv.rxConf.bAppendStatus = 0x0;
    RF_cmdPropRxAdv.maxPktLen = macCfg.macMaxFrameSize;
    RF_cmdPropRxAdv.hdrConf.numHdrBits = 0x10;
    RF_cmdPropRxAdv.hdrConf.lenPos = 0x0;
    RF_cmdPropRxAdv.hdrConf.numLenBits = 0xB;
    RF_cmdPropRxAdv.lenOffset = 0xFC;
#ifdef RFCORE49
    RF_cmdPropRxAdv.endTrigger.triggerType = 0x4;
    RF_cmdPropRxAdv.endTrigger.bEnaCmd = 0x0;
    RF_cmdPropRxAdv.endTrigger.triggerNo = 0x0;
    RF_cmdPropRxAdv.endTrigger.pastTrig = 0x0;
    RF_cmdPropRxAdv.endTime = 1680000000;   //7 minutes
#endif

    memcpy(&RF_cmdPropCs, pRfCfg->pRfCs, sizeof(rfc_CMD_PROP_CS_t));
    RF_cmdPropCs.pNextOp = (rfc_radioOp_t *)&RF_cmdPropTxAdv;
    RF_cmdPropCs.startTrigger.triggerType = TRIG_REL_SUBMIT;
    RF_cmdPropCs.startTrigger.pastTrig = 1;
    RF_cmdPropCs.condition.rule = COND_STOP_ON_TRUE;
    RF_cmdPropCs.csConf.bEnaRssi = 0x1;
    RF_cmdPropCs.csConf.bEnaCorr = 0x0;
    RF_cmdPropCs.csConf.busyOp = 0x1;
    RF_cmdPropCs.csConf.idleOp = 0x1;
    RF_cmdPropCs.rssiThr = -83;
    RF_cmdPropCs.corrPeriod = 640;
    RF_cmdPropCs.corrConfig.numCorrInv = 0x03;
    RF_cmdPropCs.corrConfig.numCorrBusy = 0x00;
    RF_cmdPropCs.csEndTrigger.triggerType = TRIG_REL_START;
    RF_cmdPropCs.csEndTime = 8000; /* 2 ms timeout as a fallback */

    memcpy(&RF_cmdPropCsSlotted, pRfCfg->pRfCs, sizeof(rfc_CMD_PROP_CS_t));
    RF_cmdPropCsSlotted.pNextOp = (rfc_radioOp_t *)&RF_cmdPropCs;
    RF_cmdPropCsSlotted.startTrigger.triggerType = TRIG_ABSTIME;
    RF_cmdPropCsSlotted.startTrigger.pastTrig = 1;
    RF_cmdPropCsSlotted.condition.rule = COND_STOP_ON_TRUE;
    RF_cmdPropCsSlotted.csConf.bEnaRssi = 0x1;
    RF_cmdPropCsSlotted.csConf.bEnaCorr = 0x0; /* CC13xx sub-G can only support CCA mode 1 */
    RF_cmdPropCsSlotted.csConf.busyOp = 0x1;
    RF_cmdPropCsSlotted.csConf.idleOp = 0x1;
    RF_cmdPropCsSlotted.rssiThr = -83; /* -83 dBm */
    RF_cmdPropCsSlotted.corrPeriod = 640; /* Filler, used for correlation only */
    RF_cmdPropCsSlotted.corrConfig.numCorrInv = 0x03;
    RF_cmdPropCsSlotted.corrConfig.numCorrBusy = 0x00;
    RF_cmdPropCsSlotted.csEndTrigger.triggerType = TRIG_REL_START;
    RF_cmdPropCsSlotted.csEndTime = 8000; /* 2 ms timeout as a fallback */

    pMaxPower = (macRfCfg_maxPower_t *)pRfCfg->data;

    /* Setup MAC RX command */
    macSetupReceiveCmd();
}
#endif

// CMD_NOP
rfc_CMD_NOP_t RF_cmdNop =
{
    .commandNo = 0x0801,
    .status = 0x0000,
    .pNextOp = 0,
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
};

// CMD_NOP Foreground command 0x0400 = FG
rfc_CMD_NOP_t RF_cmdNopFg =
{
    .commandNo = 0x0C01,
    .status = 0x0000,
    .pNextOp = 0,
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x1,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
};

// CMD_SET_RAT_CMP
rfc_CMD_SET_RAT_CMP_t RF_cmdRat =
{
    .commandNo = 0x000A,
    .ratCh = 0,
    .__dummy0 = 0,
    .compareTime = 0,
};

// CMD_SET_RAT_CMP
rfc_CMD_SET_RAT_CMP_t RF_cmdRat1 =
{
    .commandNo = 0x000A,
    .ratCh = 1,
    .__dummy0 = 0,
    .compareTime = 0,
};
