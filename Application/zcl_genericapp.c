/**************************************************************************************************
  Filename:       zcl_genericapp.c
  Revised:        $Date: 2014-10-24 16:04:46 -0700 (Fri, 24 Oct 2014) $
  Revision:       $Revision: 40796 $


  Description:    Zigbee Cluster Library - sample device application.


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

/*********************************************************************
  This application is a template to get started writing an application
  from scratch.

  Look for the sections marked with "GENERICAPP_TODO" to add application
  specific code.

  Note: if you would like your application to support automatic attribute
  reporting, include the BDB_REPORTING compile flag.
*********************************************************************/

/*********************************************************************
 * INCLUDES
 */

#include "rom_jt_154.h"
#include "zcomdef.h"

#include "nvintf.h"
#include <string.h>

#include "zstackapi.h"
#include "zstackmsg.h"

#include "zcl.h"
#include "zcl_diagnostic.h"
#include "zcl_general.h"
#include "zcl_genericapp.h"
#include "zcl_port.h"
#include "zcl_ss.h"

#include "ti_drivers_config.h"
#include "util_timer.h"
#include <ti/drivers/ADC.h>
#include <ti/drivers/Temperature.h>
#include <ti/drivers/apps/Button.h>
#include <ti/drivers/apps/LED.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>

#include <aon_batmon.h>

#if defined(BDB_TL_INITIATOR)
#include "touchlink_initiator_app.h"
#elif defined(BDB_TL_TARGET)
#include "touchlink_target_app.h"
#endif

/*********************************************************************
 * MACROS
 */
/*********************************************************************
 * CONSTANTS
 */
static const uint8_t adcChannels[] = {CONFIG_ADC_CH1, CONFIG_ADC_CH2};
static const uint8_t gpioChannes[] = {CONFIG_BUTTON_CH1, CONFIG_BUTTON_CH2, CONFIG_BUTTON_CH3};
static const uint8_t powerChannels[] = {CONFIG_GPIO_POWER_CH1, CONFIG_GPIO_POWER_CH2, CONFIG_GPIO_POWER_CH3};
/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
byte zclGenericApp_TaskID;
static Button_Handle gRightButtonHandle;
static Button_Handle gLeftButtonHandle;
static Button_Handle counterHandles[GENERICAPP_CHANNELS_COUNT];

/*********************************************************************
 * GLOBAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

// Semaphore used to post events to the application thread
static Semaphore_Handle appSemHandle;
static Semaphore_Struct appSem;

/* App service ID used for messaging with stack service task */
static uint8_t appServiceTaskId;
/* App service task events, set by the stack service task when sending a message
 */
static uint32_t appServiceTaskEvents;
static endPointDesc_t zclGenericAppEpDesc = {0};
static endPointDesc_t zclGenericAppChannelsEpDesc[GENERICAPP_CHANNELS_COUNT];

#if ZG_BUILD_ENDDEVICE_TYPE
static Clock_Handle EndDeviceRejoinClkHandle;
static Clock_Struct EndDeviceRejoinClkStruct;
#endif

static Clock_Handle readSensorsClkHandle;
static Clock_Struct readSensorsClkStruct;

static Clock_Handle initSensorsReadingClkHandle;
static Clock_Struct initSensorsReadingClkStruct;

static Clock_Handle deviceMetricsSamplingClkHandle;
static Clock_Struct deviceMetricsSamplingClkStruct;

// Passed in function pointers to the NV driver
static NVINTF_nvFuncts_t *pfnZdlNV = NULL;

// Key press parameters
static Button_Handle keys = NULL;
static Button_Handle counters = NULL;
static Button_EventMask counterEvents[GENERICAPP_CHANNELS_COUNT];

afAddrType_t inderect_DstAddr =  {.addrMode = (afAddrMode_t)AddrNotPresent, .endPoint = 0, .addr.shortAddr = 0};


static LED_Handle gGreenLedHandle;
static LED_Handle gRedLedHandle;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void zclGenericApp_initialization(void);
static void zclGenericApp_process_loop(void);
static void zclGenericApp_initParameters(void);
static void zclGenericApp_processZStackMsgs(zstackmsg_genericReq_t *pMsg);
static void SetupZStackCallbacks(void);
static void zclGenericApp_processAfIncomingMsgInd(zstack_afIncomingMsgInd_t *pInMsg);
static void zclGenericApp_initializeClocks(void);
static void Initialize_UI(void);
static void Initialize_ChannelsButtons(void);
#if ZG_BUILD_ENDDEVICE_TYPE
static void zclGenericApp_processEndDeviceRejoinTimeoutCallback(UArg a0);
#endif
static void zclGenericApp_processSensorsSamplintTimeoutCallback(UArg a0);
static void zclGenericApp_processBatterySamplintTimeoutCallback(UArg a0);
static void zclGenericApp_processInitSensorsReadingTimeoutCallback(UArg a0);
static void zclGenericApp_changeKeyCallback(Button_Handle _btn, Button_EventMask _buttonEvents);
static void zclGenericApp_CounterPinCallback(Button_Handle _btn, Button_EventMask _buttonEvents);

static void zclGenericApp_processKey(Button_Handle _btn);
static void zclGenericApp_processCounter(Button_Handle _btn);
static void zclGenericApp_processAdc(void);
static void zclGenericApp_processSensorsReading(void);
static void zclGenericApp_processDeviceMeasuremts(void);
static void zclGenericApp_Init(void);
static void zclGenericApp_BasicResetCB(void);
static void zclGenericApp_RemoveAppNvmData(void);
static void zclGenericApp_ProcessCommissioningStatus(bdbCommissioningModeMsg_t *bdbCommissioningModeMsg);

static void zclGenericApp_initSensorRead(void);
static void zclGenericApp_powerOnSensors(void);
static void zclGenericApp_powerOffSensors(void);

// Functions to process ZCL Foundation incoming Command/Response messages
static uint8_t zclGenericApp_ProcessIncomingMsg(zclIncoming_t *pInMsg);
#ifdef ZCL_READ
static uint8_t zclGenericApp_ProcessInReadRspCmd(zclIncoming_t *pInMsg);
#endif
#ifdef ZCL_WRITE
static uint8_t zclGenericApp_ProcessInWriteRspCmd(zclIncoming_t *pInMsg);
#endif
static uint8_t zclGenericApp_ProcessInDefaultRspCmd(zclIncoming_t *pInMsg);
#ifdef ZCL_DISCOVER
static uint8_t zclGenericApp_ProcessInDiscCmdsRspCmd(zclIncoming_t *pInMsg);
static uint8_t zclGenericApp_ProcessInDiscAttrsRspCmd(zclIncoming_t *pInMsg);
static uint8_t zclGenericApp_ProcessInDiscAttrsExtRspCmd(zclIncoming_t *pInMsg);
#endif

/*********************************************************************
 * STATUS STRINGS
 */

// TODO?

/*********************************************************************
 * ZCL General Profile Callback table
 */
static zclGeneral_AppCallbacks_t zclGenericApp_CmdCallbacks = {
    zclGenericApp_BasicResetCB, // Basic Cluster Reset command
    NULL,                       // Identfiy cmd
    NULL,                       // Identify Query command
    NULL,                       // Identify Query Response command
    NULL,                       // Identify Trigger Effect command
#ifdef ZCL_ON_OFF
    NULL, // On/Off cluster commands
    NULL, // On/Off cluster enhanced command Off with Effect
    NULL, // On/Off cluster enhanced command On with Recall Global Scene
    NULL, // On/Off cluster enhanced command On with Timed Off
#endif
#ifdef ZCL_LEVEL_CTRL
    NULL, // Level Control Move to Level command
    NULL, // Level Control Move command
    NULL, // Level Control Step command
    NULL, // Level Control Stop command
    NULL, // Level Control Move to Closest Frequency command
#endif
#ifdef ZCL_GROUPS
    NULL, // Group Response commands
#endif
#ifdef ZCL_SCENES
    NULL, // Scene Store Request command
    NULL, // Scene Recall Request command
    NULL, // Scene Response command
#endif
#ifdef ZCL_ALARMS
    NULL, // Alarm (Response) commands
#endif
#ifdef SE_UK_EXT
    NULL, // Get Event Log command
    NULL, // Publish Event Log command
#endif
    NULL, // RSSI Location command
    NULL  // RSSI Location Response command
};

/*********************************************************************
 * GENERICAPP_TODO: Add other callback structures for any additional application
 * specific Clusters being used, see available callback structures below.
 *
 *       bdbTL_AppCallbacks_t
 *       zclApplianceControl_AppCallbacks_t
 *       zclApplianceEventsAlerts_AppCallbacks_t
 *       zclApplianceStatistics_AppCallbacks_t
 *       zclElectricalMeasurement_AppCallbacks_t
 *       zclGeneral_AppCallbacks_t
 *       zclGp_AppCallbacks_t
 *       zclHVAC_AppCallbacks_t
 *       zclLighting_AppCallbacks_t
 *       zclMS_AppCallbacks_t
 *       zclPollControl_AppCallbacks_t
 *       zclPowerProfile_AppCallbacks_t
 *       zclSS_AppCallbacks_t
 *
 */

/*******************************************************************************
 * @fn          sampleApp_task
 *
 * @brief       Application task entry point for the Z-Stack
 *              Sample Application
 *
 * @param       pfnNV - pointer to the NV functions
 *
 * @return      none
 */
void sampleApp_task(NVINTF_nvFuncts_t *pfnNV) {
    // Save and register the function pointers to the NV drivers
    pfnZdlNV = pfnNV;
    zclport_registerNV(pfnZdlNV, ZCL_PORT_SCENE_TABLE_NV_ID);

    // Initialize application
    zclGenericApp_initialization();

    // No return from task process
    zclGenericApp_process_loop();
}

/*******************************************************************************
 * @fn          zclGenericApp_initialization
 *
 * @brief       Initialize the application
 *
 * @param       none
 *
 * @return      none
 */
static void zclGenericApp_initialization(void) {
    /* Initialize user clocks */
    zclGenericApp_initializeClocks();

    /* create semaphores for messages / events
     */
    Semaphore_Params semParam;
    Semaphore_Params_init(&semParam);
    semParam.mode = ti_sysbios_knl_Semaphore_Mode_COUNTING;
    Semaphore_construct(&appSem, 0, &semParam);
    appSemHandle = Semaphore_handle(&appSem);

    appServiceTaskId = OsalPort_registerTask(Task_self(), appSemHandle, &appServiceTaskEvents);

    // Initialize stack
    zclGenericApp_Init();
}

/*******************************************************************************
 * @fn          Initialize_UI
 *
 * @brief       Initialize the User Interface
 *
 * @param       none
 *
 * @return      none
 */
static void Initialize_UI(void) {
    /* Initialize btns */
    Button_Params bparams;
    Button_Params_init(&bparams);
    gLeftButtonHandle = Button_open(CONFIG_BTN_LEFT, zclGenericApp_changeKeyCallback, &bparams);
    // Open Right button without appCallBack
    gRightButtonHandle = Button_open(CONFIG_BTN_RIGHT, NULL, &bparams);

    if (!GPIO_read(((Button_HWAttrs *)gRightButtonHandle->hwAttrs)->gpioIndex)) {
        zclGenericApp_RemoveAppNvmData();
        Zstackapi_bdbResetLocalActionReq(appServiceTaskId);
    }

    // Set button callback
    Button_setCallback(gRightButtonHandle, zclGenericApp_changeKeyCallback);

    LED_Params ledParams;
    LED_Params_init(&ledParams);
    gGreenLedHandle = LED_open(CONFIG_LED_GREEN, &ledParams);
    gRedLedHandle = LED_open(CONFIG_LED_RED, &ledParams);
}

static void Initialize_ChannelsButtons(void) {
    /* Initialize counters */
    Button_Params bparams;
    Button_Params_init(&bparams);
    for (int i = 0; i < sizeof(gpioChannes) / sizeof(gpioChannes[0]); i++) {
        counterHandles[i] = Button_open(gpioChannes[i], zclGenericApp_CounterPinCallback, &bparams);
        counterEvents[i] = 0;
    }
}

/*******************************************************************************
 * @fn      SetupZStackCallbacks
 *
 * @brief   Setup the Zstack Callbacks wanted
 *
 * @param   none
 *
 * @return  none
 */
static void SetupZStackCallbacks(void) {
    zstack_devZDOCBReq_t zdoCBReq = {0};

    // Register for Callbacks, turn on:
    //  Device State Change,
    //  ZDO Match Descriptor Response,
    zdoCBReq.has_devStateChange = true;
    zdoCBReq.devStateChange = true;

    (void)Zstackapi_DevZDOCBReq(appServiceTaskId, &zdoCBReq);
}

/*********************************************************************
 * @fn          zclGenericApp_Init
 *
 * @brief       Initialization function for the zclGeneral layer.
 *
 * @param       none
 *
 * @return      none
 */
static void zclGenericApp_Init(void) {
    Initialize_UI();
    Initialize_ChannelsButtons();
    zclGenericApp_InitChannelsClusters();

    // Register Endpoint
    zclGenericAppEpDesc.endPoint = GENERICAPP_ENDPOINT;
    zclGenericAppEpDesc.simpleDesc = &zclGenericApp_SimpleDesc;
    zclport_registerEndpoint(appServiceTaskId, &zclGenericAppEpDesc);

    // Register the ZCL General Cluster Library callback functions
    zclGeneral_RegisterCmdCallbacks(GENERICAPP_ENDPOINT, &zclGenericApp_CmdCallbacks);

    // GENERICAPP_TODO: Register other cluster command callbacks here

    // Register the application's attribute list
    zclGenericApp_ResetAttributesToDefaultValues();
    zcl_registerAttrList(GENERICAPP_ENDPOINT, zclGenericApp_NumAttributes, zclGenericApp_Attrs);

    // Register the Application to receive the unprocessed Foundation
    // command/response messages
    zclport_registerZclHandleExternal(GENERICAPP_ENDPOINT, zclGenericApp_ProcessIncomingMsg);

    for (size_t i = 0; i < GENERICAPP_CHANNELS_COUNT; i++) {

        zcl_memset(&zclGenericAppChannelsEpDesc[i], 0, sizeof(endPointDesc_t));
        zclGenericAppChannelsEpDesc[i].endPoint = zclGenericApp_ChannelsSimpleDesc[i].EndPoint;
        zclGenericAppChannelsEpDesc[i].simpleDesc = &zclGenericApp_ChannelsSimpleDesc[i];

        zclport_registerEndpoint(appServiceTaskId, &zclGenericAppChannelsEpDesc[i]);
        zclGeneral_RegisterCmdCallbacks(zclGenericApp_ChannelsSimpleDesc[i].EndPoint, &zclGenericApp_CmdCallbacks);
        zcl_registerAttrList(zclGenericApp_ChannelsSimpleDesc[i].EndPoint, GENERICAPP_CHANNEL_ATTRS_COUNT, zclGenericApp_ChannelAttrs[i]);
        zclport_registerZclHandleExternal(zclGenericApp_ChannelsSimpleDesc[i].EndPoint, zclGenericApp_ProcessIncomingMsg);
    }

    // Write the bdb initialization parameters
    zclGenericApp_initParameters();

    // Setup ZDO callbacks
    SetupZStackCallbacks();

#ifdef ZCL_DISCOVER
    // Register the application's command list
    zcl_registerCmdList(GENERICAPP_ENDPOINT, zclCmdsArraySize, zclGenericApp_Cmds);
#endif

#ifdef ZCL_DIAGNOSTIC
    // Register the application's callback function to read/write attribute data.
    // This is only required when the attribute data format is unknown to ZCL.
    zcl_registerReadWriteCB(GENERICAPP_ENDPOINT, zclDiagnostic_ReadWriteAttrCB, NULL);

    if (zclDiagnostic_InitStats() == ZSuccess) {
        // Here the user could start the timer to save Diagnostics to NV
    }
#endif

#if defined(BDB_TL_INITIATOR)
    touchLinkInitiatorApp_Init(appServiceTaskId);
#elif defined(BDB_TL_TARGET)
    touchLinkTargetApp_Init(appServiceTaskId);
#endif

    // Call BDB initialization. Should be called once from application at startup
    // to restore previous network configuration, if applicable.
    zstack_bdbStartCommissioningReq_t zstack_bdbStartCommissioningReq;
    zstack_bdbStartCommissioningReq.commissioning_mode = 0;
    Zstackapi_bdbStartCommissioningReq(appServiceTaskId, &zstack_bdbStartCommissioningReq);

    UtilTimer_start(&deviceMetricsSamplingClkStruct);
    UtilTimer_start(&initSensorsReadingClkStruct);
}

/*********************************************************************
 * @fn          zclGenericApp_RemoveAppNvmData
 *
 * @brief       Callback when Application performs reset to Factory New Reset.
 *              Application must restore the application to default values
 *
 * @param       none
 *
 * @return      none
 */
static void zclGenericApp_RemoveAppNvmData(void) {}

/*********************************************************************
 * @fn          zclGenericApp_initParameters
 *
 * @brief       Initialization function for the bdb attribute set
 *
 * @param       none
 *
 * @return      none
 */
static void zclGenericApp_initParameters(void) {
    zstack_bdbSetAttributesReq_t zstack_bdbSetAttrReq;

    zstack_bdbSetAttrReq.bdbCommissioningGroupID = BDB_DEFAULT_COMMISSIONING_GROUP_ID;
    zstack_bdbSetAttrReq.bdbPrimaryChannelSet = BDB_DEFAULT_PRIMARY_CHANNEL_SET;
    zstack_bdbSetAttrReq.bdbScanDuration = BDB_DEFAULT_SCAN_DURATION;
    zstack_bdbSetAttrReq.bdbSecondaryChannelSet = BDB_DEFAULT_SECONDARY_CHANNEL_SET;
    zstack_bdbSetAttrReq.has_bdbCommissioningGroupID = TRUE;
    zstack_bdbSetAttrReq.has_bdbPrimaryChannelSet = TRUE;
    zstack_bdbSetAttrReq.has_bdbScanDuration = TRUE;
    zstack_bdbSetAttrReq.has_bdbSecondaryChannelSet = TRUE;
#if (ZG_BUILD_COORDINATOR_TYPE)
    zstack_bdbSetAttrReq.has_bdbJoinUsesInstallCodeKey = TRUE;
    zstack_bdbSetAttrReq.has_bdbTrustCenterNodeJoinTimeout = TRUE;
    zstack_bdbSetAttrReq.has_bdbTrustCenterRequireKeyExchange = TRUE;
    zstack_bdbSetAttrReq.bdbJoinUsesInstallCodeKey = BDB_DEFAULT_JOIN_USES_INSTALL_CODE_KEY;
    zstack_bdbSetAttrReq.bdbTrustCenterNodeJoinTimeout = BDB_DEFAULT_TC_NODE_JOIN_TIMEOUT;
    zstack_bdbSetAttrReq.bdbTrustCenterRequireKeyExchange = BDB_DEFAULT_TC_REQUIRE_KEY_EXCHANGE;
#endif
#if (ZG_BUILD_JOINING_TYPE)
    zstack_bdbSetAttrReq.has_bdbTCLinkKeyExchangeAttemptsMax = TRUE;
    zstack_bdbSetAttrReq.has_bdbTCLinkKeyExchangeMethod = TRUE;
    zstack_bdbSetAttrReq.bdbTCLinkKeyExchangeAttemptsMax = BDB_DEFAULT_TC_LINK_KEY_EXCHANGE_ATTEMPS_MAX;
    zstack_bdbSetAttrReq.bdbTCLinkKeyExchangeMethod = BDB_DEFAULT_TC_LINK_KEY_EXCHANGE_METHOD;
#endif

    Zstackapi_bdbSetAttributesReq(appServiceTaskId, &zstack_bdbSetAttrReq);
}

/*******************************************************************************
 * @fn      zclGenericApp_initializeClocks
 *
 * @brief   Initialize Clocks
 *
 * @param   none
 *
 * @return  none
 */
static void zclGenericApp_initializeClocks(void) {
#if ZG_BUILD_ENDDEVICE_TYPE
    // Initialize the timers needed for this application
    EndDeviceRejoinClkHandle = UtilTimer_construct(&EndDeviceRejoinClkStruct, zclGenericApp_processEndDeviceRejoinTimeoutCallback,
                                                   GENERICAPP_END_DEVICE_REJOIN_DELAY, 0, false, 0);
#endif

    readSensorsClkHandle = UtilTimer_construct(&readSensorsClkStruct, zclGenericApp_processSensorsSamplintTimeoutCallback,
                                               GENERICAPP_SENSORS_POWER_ON_TIME, 0, false, 0);
    (void)readSensorsClkHandle;

    deviceMetricsSamplingClkHandle =
        UtilTimer_construct(&deviceMetricsSamplingClkStruct, zclGenericApp_processBatterySamplintTimeoutCallback, 0,
                            GENERICAPP_BATTERY_SAMPLING_INTERVAL, false, 0);
    (void)deviceMetricsSamplingClkHandle;

    initSensorsReadingClkHandle = UtilTimer_construct(&initSensorsReadingClkStruct, zclGenericApp_processInitSensorsReadingTimeoutCallback,
                                                      0, GENERICAPP_SENSORS_READING_INTERVAL, false, 0);
    (void)initSensorsReadingClkHandle;
}

#if ZG_BUILD_ENDDEVICE_TYPE
/*******************************************************************************
 * @fn      zclGenericApp_processEndDeviceRejoinTimeoutCallback
 *
 * @brief   Timeout handler function
 *
 * @param   a0 - ignored
 *
 * @return  none
 */
static void zclGenericApp_processEndDeviceRejoinTimeoutCallback(UArg a0) {
    (void)a0; // Parameter is not used

    appServiceTaskEvents |= GENERICAPP_END_DEVICE_REJOIN_EVT;

    // Wake up the application thread when it waits for clock event
    Semaphore_post(appSemHandle);
}
#endif

static void zclGenericApp_processSensorsSamplintTimeoutCallback(UArg a0) {
    (void)a0; // Parameter is not used

    appServiceTaskEvents |= GENERICAPP_SENSORS_SAMPLING_EVT;

    // Wake up the application thread when it waits for clock event
    Semaphore_post(appSemHandle);
}

static void zclGenericApp_processBatterySamplintTimeoutCallback(UArg a0) {
    (void)a0; // Parameter is not used

    appServiceTaskEvents |= GENERICAPP_BATTERY_SAMPLING_EVT;

    // Wake up the application thread when it waits for clock event
    Semaphore_post(appSemHandle);
}

static void zclGenericApp_processInitSensorsReadingTimeoutCallback(UArg a0) {
    (void)a0; // Parameter is not used

    appServiceTaskEvents |= GENERICAPP_INIT_SENSORS_READING_EVT;

    // Wake up the application thread when it waits for clock event
    Semaphore_post(appSemHandle);
}

/*********************************************************************
 * @fn          zclSample_event_loop
 *
 * @brief       Event Loop Processor for zclGeneral.
 *
 * @param       none
 *
 * @return      none
 */
static void zclGenericApp_process_loop(void) {
    /* Forever loop */
    for (;;) {
        zstackmsg_genericReq_t *pMsg = NULL;
        bool msgProcessed = FALSE;

        /* Wait for response message */
        if (Semaphore_pend(appSemHandle, BIOS_WAIT_FOREVER)) {
            /* Retrieve the response message */
            if ((pMsg = (zstackmsg_genericReq_t *)OsalPort_msgReceive(appServiceTaskId)) != NULL) {
                /* Process the message from the stack */
                zclGenericApp_processZStackMsgs(pMsg);

                // Free any separately allocated memory
                msgProcessed = Zstackapi_freeIndMsg(pMsg);
            }

            if ((msgProcessed == FALSE) && (pMsg != NULL)) {
                OsalPort_msgDeallocate((uint8_t *)pMsg);
            }

            if (appServiceTaskEvents & GENERICAPP_KEY_EVT) {
                // Process Key Presses
                zclGenericApp_processKey(keys);
                keys = NULL;
                appServiceTaskEvents &= ~GENERICAPP_KEY_EVT;
            }

            if (appServiceTaskEvents & GENERICAPP_COUNTER_PIN_EVT) {
                // Process counter
                zclGenericApp_processCounter(counters);
                counters = NULL;
                appServiceTaskEvents &= ~GENERICAPP_COUNTER_PIN_EVT;
            }

#if ZG_BUILD_ENDDEVICE_TYPE
            if (appServiceTaskEvents & GENERICAPP_END_DEVICE_REJOIN_EVT) {
                zstack_bdbZedAttemptRecoverNwkRsp_t zstack_bdbZedAttemptRecoverNwkRsp;

                Zstackapi_bdbZedAttemptRecoverNwkReq(appServiceTaskId, &zstack_bdbZedAttemptRecoverNwkRsp);

                appServiceTaskEvents &= ~GENERICAPP_END_DEVICE_REJOIN_EVT;
            }
#endif

            if (appServiceTaskEvents & GENERICAPP_SENSORS_SAMPLING_EVT) {
                zclGenericApp_processSensorsReading();
                appServiceTaskEvents &= ~GENERICAPP_SENSORS_SAMPLING_EVT;
            }

            if (appServiceTaskEvents & GENERICAPP_BATTERY_SAMPLING_EVT) {
                zclGenericApp_processDeviceMeasuremts();
                appServiceTaskEvents &= ~GENERICAPP_BATTERY_SAMPLING_EVT;
            }

            if (appServiceTaskEvents & GENERICAPP_INIT_SENSORS_READING_EVT) {
                zclGenericApp_initSensorRead();
                appServiceTaskEvents &= ~GENERICAPP_INIT_SENSORS_READING_EVT;
            }

            /*
            if ( appServiceTaskEvents & GENERICAPP_EVT_2 )
            {

              appServiceTaskEvents &= ~GENERICAPP_EVT_2;
            }

            if ( appServiceTaskEvents & GENERICAPP_EVT_3 )
            {

              appServiceTaskEvents &= ~GENERICAPP_EVT_3;
            }
            */
        }
    }
}

/*******************************************************************************
 * @fn      zclGenericApp_processZStackMsgs
 *
 * @brief   Process event from Stack
 *
 * @param   pMsg - pointer to incoming ZStack message to process
 *
 * @return  void
 */
static void zclGenericApp_processZStackMsgs(zstackmsg_genericReq_t *pMsg) {
    switch (pMsg->hdr.event) {
    case zstackmsg_CmdIDs_BDB_NOTIFICATION: {
        zstackmsg_bdbNotificationInd_t *pInd;
        pInd = (zstackmsg_bdbNotificationInd_t *)pMsg;
        zclGenericApp_ProcessCommissioningStatus(&(pInd->Req));
    } break;

    case zstackmsg_CmdIDs_BDB_IDENTIFY_TIME_CB: {
        //                zstackmsg_bdbIdentifyTimeoutInd_t *pInd;
        //                pInd = (zstackmsg_bdbIdentifyTimeoutInd_t*) pMsg;
        //                uiProcessIdentifyTimeChange(&(pInd->EndPoint));
    } break;

    case zstackmsg_CmdIDs_BDB_BIND_NOTIFICATION_CB: {
        //                zstackmsg_bdbBindNotificationInd_t *pInd;
        //                pInd = (zstackmsg_bdbBindNotificationInd_t*) pMsg;
        //                uiProcessBindNotification(&(pInd->Req));
    } break;

    case zstackmsg_CmdIDs_AF_INCOMING_MSG_IND: {
        // Process incoming data messages
        zstackmsg_afIncomingMsgInd_t *pInd;
        pInd = (zstackmsg_afIncomingMsgInd_t *)pMsg;
        zclGenericApp_processAfIncomingMsgInd(&(pInd->req));
    } break;

    case zstackmsg_CmdIDs_DEV_PERMIT_JOIN_IND: {
        //                zstackmsg_devPermitJoinInd_t *pInd;
        //                pInd = (zstackmsg_devPermitJoinInd_t*)pMsg;
        //                uiProcessPermitJoin(&(pInd->Req));
    } break;

#if (ZG_BUILD_JOINING_TYPE)
    case zstackmsg_CmdIDs_BDB_CBKE_TC_LINK_KEY_EXCHANGE_IND: {
        zstack_bdbCBKETCLinkKeyExchangeAttemptReq_t zstack_bdbCBKETCLinkKeyExchangeAttemptReq;
        /* Z3.0 has not defined CBKE yet, so lets attempt default TC Link Key
         * exchange procedure by reporting CBKE failure.
         */

        zstack_bdbCBKETCLinkKeyExchangeAttemptReq.didSuccess = FALSE;

        Zstackapi_bdbCBKETCLinkKeyExchangeAttemptReq(appServiceTaskId, &zstack_bdbCBKETCLinkKeyExchangeAttemptReq);
    } break;

    case zstackmsg_CmdIDs_BDB_FILTER_NWK_DESCRIPTOR_IND:

        /*   User logic to remove networks that do not want to join
         *   Networks to be removed can be released with Zstackapi_bdbNwkDescFreeReq
         */

        Zstackapi_bdbFilterNwkDescComplete(appServiceTaskId);
        break;

#endif
    case zstackmsg_CmdIDs_DEV_STATE_CHANGE_IND: {
        // The ZStack Thread is indicating a State change
        //            zstackmsg_devStateChangeInd_t *pInd =
        //                (zstackmsg_devStateChangeInd_t *)pMsg;
        //                  UI_DeviceStateUpdated(&(pInd->req));
    } break;

#ifdef BDB_TL_TARGET
    case zstackmsg_CmdIDs_BDB_TOUCHLINK_TARGET_ENABLE_IND: {

    } break;
#endif

    case zstackmsg_CmdIDs_BDB_TC_LINK_KEY_EXCHANGE_NOTIFICATION_IND:
    case zstackmsg_CmdIDs_AF_DATA_CONFIRM_IND:
    case zstackmsg_CmdIDs_ZDO_DEVICE_ANNOUNCE:
    case zstackmsg_CmdIDs_ZDO_NWK_ADDR_RSP:
    case zstackmsg_CmdIDs_ZDO_IEEE_ADDR_RSP:
    case zstackmsg_CmdIDs_ZDO_NODE_DESC_RSP:
    case zstackmsg_CmdIDs_ZDO_POWER_DESC_RSP:
    case zstackmsg_CmdIDs_ZDO_SIMPLE_DESC_RSP:
    case zstackmsg_CmdIDs_ZDO_ACTIVE_EP_RSP:
    case zstackmsg_CmdIDs_ZDO_COMPLEX_DESC_RSP:
    case zstackmsg_CmdIDs_ZDO_USER_DESC_RSP:
    case zstackmsg_CmdIDs_ZDO_USER_DESC_SET_RSP:
    case zstackmsg_CmdIDs_ZDO_SERVER_DISC_RSP:
    case zstackmsg_CmdIDs_ZDO_END_DEVICE_BIND_RSP:
    case zstackmsg_CmdIDs_ZDO_BIND_RSP:
    case zstackmsg_CmdIDs_ZDO_UNBIND_RSP:
    case zstackmsg_CmdIDs_ZDO_MGMT_NWK_DISC_RSP:
    case zstackmsg_CmdIDs_ZDO_MGMT_LQI_RSP:
    case zstackmsg_CmdIDs_ZDO_MGMT_RTG_RSP:
    case zstackmsg_CmdIDs_ZDO_MGMT_BIND_RSP:
    case zstackmsg_CmdIDs_ZDO_MGMT_LEAVE_RSP:
    case zstackmsg_CmdIDs_ZDO_MGMT_DIRECT_JOIN_RSP:
    case zstackmsg_CmdIDs_ZDO_MGMT_PERMIT_JOIN_RSP:
    case zstackmsg_CmdIDs_ZDO_MGMT_NWK_UPDATE_NOTIFY:
    case zstackmsg_CmdIDs_ZDO_SRC_RTG_IND:
    case zstackmsg_CmdIDs_ZDO_CONCENTRATOR_IND:
    case zstackmsg_CmdIDs_ZDO_LEAVE_CNF:
    case zstackmsg_CmdIDs_ZDO_LEAVE_IND:
    case zstackmsg_CmdIDs_SYS_RESET_IND:
    case zstackmsg_CmdIDs_AF_REFLECT_ERROR_IND:
    case zstackmsg_CmdIDs_ZDO_TC_DEVICE_IND:
        break;

    default:
        break;
    }
}

/*******************************************************************************
 *
 * @fn          zclGenericApp_processAfIncomingMsgInd
 *
 * @brief       Process AF Incoming Message Indication message
 *
 * @param       pInMsg - pointer to incoming message
 *
 * @return      none
 *
 */
static void zclGenericApp_processAfIncomingMsgInd(zstack_afIncomingMsgInd_t *pInMsg) {
    afIncomingMSGPacket_t afMsg;

    /*
     * All incoming messages are passed to the ZCL message processor,
     * first convert to a structure that ZCL can process.
     */
    afMsg.groupId = pInMsg->groupID;
    afMsg.clusterId = pInMsg->clusterId;
    afMsg.srcAddr.endPoint = pInMsg->srcAddr.endpoint;
    afMsg.srcAddr.panId = pInMsg->srcAddr.panID;
    afMsg.srcAddr.addrMode = (afAddrMode_t)pInMsg->srcAddr.addrMode;
    if ((afMsg.srcAddr.addrMode == afAddr16Bit) || (afMsg.srcAddr.addrMode == afAddrGroup) || (afMsg.srcAddr.addrMode == afAddrBroadcast)) {
        afMsg.srcAddr.addr.shortAddr = pInMsg->srcAddr.addr.shortAddr;
    } else if (afMsg.srcAddr.addrMode == afAddr64Bit) {
        OsalPort_memcpy(afMsg.srcAddr.addr.extAddr, &(pInMsg->srcAddr.addr.extAddr), 8);
    }
    afMsg.macDestAddr = pInMsg->macDestAddr;
    afMsg.endPoint = pInMsg->endpoint;
    afMsg.wasBroadcast = pInMsg->wasBroadcast;
    afMsg.LinkQuality = pInMsg->linkQuality;
    afMsg.correlation = pInMsg->correlation;
    afMsg.rssi = pInMsg->rssi;
    afMsg.SecurityUse = pInMsg->securityUse;
    afMsg.timestamp = pInMsg->timestamp;
    afMsg.nwkSeqNum = pInMsg->nwkSeqNum;
    afMsg.macSrcAddr = pInMsg->macSrcAddr;
    afMsg.radius = pInMsg->radius;
    afMsg.cmd.DataLength = pInMsg->n_payload;
    afMsg.cmd.Data = pInMsg->pPayload;

    zcl_ProcessMessageMSG(&afMsg);
}

/*********************************************************************
 * @fn      zclGenericApp_ProcessCommissioningStatus
 *
 * @brief   Callback in which the status of the commissioning process are
 * reported
 *
 * @param   bdbCommissioningModeMsg - Context message of the status of a
 * commissioning process
 *
 * @return  none
 */
static void zclGenericApp_ProcessCommissioningStatus(bdbCommissioningModeMsg_t *bdbCommissioningModeMsg) {
    switch (bdbCommissioningModeMsg->bdbCommissioningMode) {
    case BDB_COMMISSIONING_FORMATION:
        if (bdbCommissioningModeMsg->bdbCommissioningStatus == BDB_COMMISSIONING_SUCCESS) {
            // YOUR JOB:
        } else {
            // Want to try other channels?
            // try with bdb_setChannelAttribute
        }
        break;
    case BDB_COMMISSIONING_NWK_STEERING:
        if (bdbCommissioningModeMsg->bdbCommissioningStatus == BDB_COMMISSIONING_SUCCESS) {
            // YOUR JOB:
            // We are on the nwk, what now?
        } else {
            // See the possible errors for nwk steering procedure
            // No suitable networks found
            // Want to try other channels?
            // try with bdb_setChannelAttribute
        }
        break;
    case BDB_COMMISSIONING_FINDING_BINDING:
        if (bdbCommissioningModeMsg->bdbCommissioningStatus == BDB_COMMISSIONING_SUCCESS) {
            // YOUR JOB:
        } else {
            // YOUR JOB:
            // retry?, wait for user interaction?
        }
        break;
    case BDB_COMMISSIONING_INITIALIZATION:
        // Initialization notification can only be successful. Failure on
        // initialization only happens for ZED and is notified as
        // BDB_COMMISSIONING_PARENT_LOST notification

        // YOUR JOB:
        // We are on a network, what now?

        break;
#if ZG_BUILD_ENDDEVICE_TYPE
    case BDB_COMMISSIONING_PARENT_LOST:
        if (bdbCommissioningModeMsg->bdbCommissioningStatus == BDB_COMMISSIONING_NETWORK_RESTORED) {
            // We did recover from losing parent
        } else {
            // Parent not found, attempt to rejoin again after a fixed delay
            UtilTimer_setTimeout(EndDeviceRejoinClkHandle, GENERICAPP_END_DEVICE_REJOIN_DELAY);
            UtilTimer_start(&EndDeviceRejoinClkStruct);
        }
        break;
#endif
    }
}

/*********************************************************************
 * @fn      zclGenericApp_BasicResetCB
 *
 * @brief   Callback from the ZCL General Cluster Library
 *          to set all the Basic Cluster attributes to default values.
 *
 * @param   none
 *
 * @return  none
 */
static void zclGenericApp_BasicResetCB(void) {

    /* GENERICAPP_TODO: remember to update this function with any
       application-specific cluster attribute variables */

    zclGenericApp_ResetAttributesToDefaultValues();
}

/******************************************************************************
 *
 *  Functions for processing ZCL Foundation incoming Command/Response messages
 *
 *****************************************************************************/

/*********************************************************************
 * @fn      zclGenericApp_ProcessIncomingMsg
 *
 * @brief   Process ZCL Foundation incoming message
 *
 * @param   pInMsg - pointer to the received message
 *
 * @return  none
 */
static uint8_t zclGenericApp_ProcessIncomingMsg(zclIncoming_t *pInMsg) {
    uint8_t handled = FALSE;

    switch (pInMsg->hdr.commandID) {
#ifdef ZCL_READ
    case ZCL_CMD_READ_RSP:
        zclGenericApp_ProcessInReadRspCmd(pInMsg);
        handled = TRUE;
        break;
#endif
#ifdef ZCL_WRITE
    case ZCL_CMD_WRITE_RSP:
        zclGenericApp_ProcessInWriteRspCmd(pInMsg);
        handled = TRUE;
        break;
#endif
    case ZCL_CMD_CONFIG_REPORT:
    case ZCL_CMD_CONFIG_REPORT_RSP:
    case ZCL_CMD_READ_REPORT_CFG:
    case ZCL_CMD_READ_REPORT_CFG_RSP:
    case ZCL_CMD_REPORT:
        // bdb_ProcessIncomingReportingMsg( pInMsg );
        break;

    case ZCL_CMD_DEFAULT_RSP:
        zclGenericApp_ProcessInDefaultRspCmd(pInMsg);
        handled = TRUE;
        break;
#ifdef ZCL_DISCOVER
    case ZCL_CMD_DISCOVER_CMDS_RECEIVED_RSP:
        zclGenericApp_ProcessInDiscCmdsRspCmd(pInMsg);
        handled = TRUE;
        break;

    case ZCL_CMD_DISCOVER_CMDS_GEN_RSP:
        zclGenericApp_ProcessInDiscCmdsRspCmd(pInMsg);
        handled = TRUE;
        break;

    case ZCL_CMD_DISCOVER_ATTRS_RSP:
        zclGenericApp_ProcessInDiscAttrsRspCmd(pInMsg);
        handled = TRUE;
        break;

    case ZCL_CMD_DISCOVER_ATTRS_EXT_RSP:
        zclGenericApp_ProcessInDiscAttrsExtRspCmd(pInMsg);
        handled = TRUE;
        break;
#endif
    default:
        break;
    }

    if (pInMsg->attrCmd)
        OsalPort_free(pInMsg->attrCmd);

    return handled;
}

#ifdef ZCL_READ
/*********************************************************************
 * @fn      zclGenericApp_ProcessInReadRspCmd
 *
 * @brief   Process the "Profile" Read Response Command
 *
 * @param   pInMsg - incoming message to process
 *
 * @return  none
 */
static uint8_t zclGenericApp_ProcessInReadRspCmd(zclIncoming_t *pInMsg) {
    zclReadRspCmd_t *readRspCmd;
    uint8_t i;

    readRspCmd = (zclReadRspCmd_t *)pInMsg->attrCmd;
    for (i = 0; i < readRspCmd->numAttr; i++) {
        // Notify the originator of the results of the original read attributes
        // attempt and, for each successfull request, the value of the requested
        // attribute
    }

    return (TRUE);
}
#endif // ZCL_READ

#ifdef ZCL_WRITE
/*********************************************************************
 * @fn      zclGenericApp_ProcessInWriteRspCmd
 *
 * @brief   Process the "Profile" Write Response Command
 *
 * @param   pInMsg - incoming message to process
 *
 * @return  none
 */
static uint8_t zclGenericApp_ProcessInWriteRspCmd(zclIncoming_t *pInMsg) {
    zclWriteRspCmd_t *writeRspCmd;
    uint8_t i;

    writeRspCmd = (zclWriteRspCmd_t *)pInMsg->attrCmd;
    for (i = 0; i < writeRspCmd->numAttr; i++) {
        // Notify the device of the results of the its original write attributes
        // command.
    }

    return (TRUE);
}
#endif // ZCL_WRITE

/*********************************************************************
 * @fn      zclGenericApp_ProcessInDefaultRspCmd
 *
 * @brief   Process the "Profile" Default Response Command
 *
 * @param   pInMsg - incoming message to process
 *
 * @return  none
 */
static uint8_t zclGenericApp_ProcessInDefaultRspCmd(zclIncoming_t *pInMsg) {
    // zclDefaultRspCmd_t *defaultRspCmd = (zclDefaultRspCmd_t *)pInMsg->attrCmd;

    // Device is notified of the Default Response command.
    (void)pInMsg;

    return (TRUE);
}

#ifdef ZCL_DISCOVER
/*********************************************************************
 * @fn      zclGenericApp_ProcessInDiscCmdsRspCmd
 *
 * @brief   Process the Discover Commands Response Command
 *
 * @param   pInMsg - incoming message to process
 *
 * @return  none
 */
static uint8_t zclGenericApp_ProcessInDiscCmdsRspCmd(zclIncoming_t *pInMsg) {
    zclDiscoverCmdsCmdRsp_t *discoverRspCmd;
    uint8_t i;

    discoverRspCmd = (zclDiscoverCmdsCmdRsp_t *)pInMsg->attrCmd;
    for (i = 0; i < discoverRspCmd->numCmd; i++) {
        // Device is notified of the result of its attribute discovery command.
    }

    return (TRUE);
}

/*********************************************************************
 * @fn      zclGenericApp_ProcessInDiscAttrsRspCmd
 *
 * @brief   Process the "Profile" Discover Attributes Response Command
 *
 * @param   pInMsg - incoming message to process
 *
 * @return  none
 */
static uint8_t zclGenericApp_ProcessInDiscAttrsRspCmd(zclIncoming_t *pInMsg) {
    zclDiscoverAttrsRspCmd_t *discoverRspCmd;
    uint8_t i;

    discoverRspCmd = (zclDiscoverAttrsRspCmd_t *)pInMsg->attrCmd;
    for (i = 0; i < discoverRspCmd->numAttr; i++) {
        // Device is notified of the result of its attribute discovery command.
    }

    return (TRUE);
}

/*********************************************************************
 * @fn      zclGenericApp_ProcessInDiscAttrsExtRspCmd
 *
 * @brief   Process the "Profile" Discover Attributes Extended Response Command
 *
 * @param   pInMsg - incoming message to process
 *
 * @return  none
 */
static uint8_t zclGenericApp_ProcessInDiscAttrsExtRspCmd(zclIncoming_t *pInMsg) {
    zclDiscoverAttrsExtRsp_t *discoverRspCmd;
    uint8_t i;

    discoverRspCmd = (zclDiscoverAttrsExtRsp_t *)pInMsg->attrCmd;
    for (i = 0; i < discoverRspCmd->numAttr; i++) {
        // Device is notified of the result of its attribute discovery command.
    }

    return (TRUE);
}
#endif // ZCL_DISCOVER

/****************************************************************************
****************************************************************************/

/*********************************************************************
 * @fn      zclGenericApp_changeKeyCallback
 *
 * @brief   Key event handler function
 *
 * @param   keysPressed - keys to be process in application context
 *
 * @return  none
 */
static void zclGenericApp_changeKeyCallback(Button_Handle _btn, Button_EventMask _buttonEvents) {
    if (_buttonEvents & Button_EV_CLICKED) {
        keys = _btn;

        appServiceTaskEvents |= GENERICAPP_KEY_EVT;

        // Wake up the application thread when it waits for clock event
        Semaphore_post(appSemHandle);
    }
}

static void zclGenericApp_CounterPinCallback(Button_Handle _btn, Button_EventMask _buttonEvents) {

    for (size_t i = 0; i < sizeof(gpioChannes) / sizeof(gpioChannes[0]); i++) {
        if (_btn == counterHandles[i]) {
            counterEvents[i] |= _buttonEvents;
            counters = _btn;
            appServiceTaskEvents |= GENERICAPP_COUNTER_PIN_EVT;
            // Wake up the application thread when it waits for clock event
            Semaphore_post(appSemHandle);
            break;
        }
    }

    //    if (_buttonEvents & Button_EV_CLICKED) {
    //        counters = _btn;
    //
    //        appServiceTaskEvents |= GENERICAPP_COUNTER_PIN_EVT;
    //
    //        // Wake up the application thread when it waits for clock event
    //        Semaphore_post(appSemHandle);
    //    }
}

/*********************************************************************
 * @fn      zclGenericApp_processKey
 *
 * @brief   Key event handler function
 *
 * @param   keysPressed - keys to be process in application context
 *
 * @return  none
 */
static void zclGenericApp_processKey(Button_Handle _btn) {

    LED_setOn(gGreenLedHandle, 80);
    LED_startBlinking(gGreenLedHandle, 500, 5);

    zstack_bdbStartCommissioningReq_t zstack_bdbStartCommissioningReq;
    // Button 1
    if (_btn == gLeftButtonHandle) {
        if (ZG_BUILD_COORDINATOR_TYPE && ZG_DEVICE_COORDINATOR_TYPE) {

            zstack_bdbStartCommissioningReq.commissioning_mode =
                BDB_COMMISSIONING_MODE_NWK_FORMATION | BDB_COMMISSIONING_MODE_NWK_STEERING | BDB_COMMISSIONING_MODE_FINDING_BINDING;
            Zstackapi_bdbStartCommissioningReq(appServiceTaskId, &zstack_bdbStartCommissioningReq);
        } else if (ZG_BUILD_JOINING_TYPE && ZG_DEVICE_JOINING_TYPE) {
            zstack_bdbStartCommissioningReq.commissioning_mode =
                BDB_COMMISSIONING_MODE_NWK_STEERING | BDB_COMMISSIONING_MODE_FINDING_BINDING;
            Zstackapi_bdbStartCommissioningReq(appServiceTaskId, &zstack_bdbStartCommissioningReq);
        }
    }
    // Button 2
    if (_btn == gRightButtonHandle) {
        Zstackapi_bdbResetLocalActionReq(appServiceTaskId);
    }
}

static void zclGenericApp_processCounter(Button_Handle _btn) {
    for (size_t i = 0; i < sizeof(gpioChannes) / sizeof(gpioChannes[0]); i++) {
        if (_btn == counterHandles[i]) {

            if (counterEvents[i] & Button_EV_CLICKED) {

                LED_setOn(gGreenLedHandle, 80);
                LED_startBlinking(gGreenLedHandle, 500, 2);

                zclGenericApp_MutistateInputValues[i] += 1;
                zstack_bdbRepChangedAttrValueReq_t Req;
                Req.attrID = ATTRID_IOV_BASIC_PRESENT_VALUE;
                Req.cluster = ZCL_CLUSTER_ID_GENERAL_MULTISTATE_INPUT_BASIC;
                Req.endpoint = zclGenericApp_ChannelsSimpleDesc[i].EndPoint;
                Zstackapi_bdbRepChangedAttrValueReq(appServiceTaskId, &Req);

                counterEvents[i] &= ~(Button_EV_CLICKED);
            }

            if (counterEvents[i] & Button_EV_PRESSED) {
                LED_setOn(gRedLedHandle, 80);
                uint16 alarmStatus = 0;
                zclSS_IAS_Send_ZoneStatusChangeNotificationCmd(zclGenericApp_ChannelsSimpleDesc[i].EndPoint, &inderect_DstAddr, alarmStatus, 0, 0, 0, true, zcl_getFrameCounter());
                counterEvents[i] &= ~(Button_EV_PRESSED);
            }

            if (counterEvents[i] & Button_EV_RELEASED) {

                LED_setOff(gRedLedHandle);
                counterEvents[i] &= ~(Button_EV_RELEASED);
                uint16 alarmStatus = 0;
                alarmStatus |= SS_IAS_ZONE_STATUS_ALARM1_ALARMED;
                zclSS_IAS_Send_ZoneStatusChangeNotificationCmd(zclGenericApp_ChannelsSimpleDesc[i].EndPoint, &inderect_DstAddr, alarmStatus, 0, 0, 0, true, zcl_getFrameCounter());
            }

            break;
        }
    }
}

static void zclGenericApp_processDeviceMeasuremts(void) {
    // GPIO_toggle(CONFIG_GPIO_RLED);
    Temperature_init();
    zclGenericApp_deviceTemperature = Temperature_getTemperature();

    zstack_bdbRepChangedAttrValueReq_t Req;
    Req.attrID = ATTRID_DEVICE_TEMPERATURE_CONFIGURATION_CURRENT_TEMPERATURE;
    Req.cluster = ZCL_CLUSTER_ID_GENERAL_DEVICE_TEMP_CONFIG;
    Req.endpoint = 1;
    Zstackapi_bdbRepChangedAttrValueReq(appServiceTaskId, &Req);

    int_fast16_t res;
    ADC_init();
    ADC_Params params;
    ADC_Params_init(&params);
    ADC_Handle batteryADC = ADC_open(CONFIG_ADC_VDDS, &params);
    if (batteryADC != NULL) {
        uint16_t adcValue0;

        res = ADC_convert(batteryADC, &adcValue0);
        if (res == ADC_STATUS_SUCCESS) {
            zclGenericApp_batteryVoltage = ADC_convertRawToMicroVolts(batteryADC, adcValue0) / 100000.0;

            zstack_bdbRepChangedAttrValueReq_t Req;
            Req.attrID = ATTRID_POWER_CONFIGURATION_BATTERY_VOLTAGE;
            Req.cluster = ZCL_CLUSTER_ID_GENERAL_POWER_CFG;
            Req.endpoint = 1;
            Zstackapi_bdbRepChangedAttrValueReq(appServiceTaskId, &Req);
        }

        ADC_close(batteryADC);
    }
}
static void zclGenericApp_processAdc(void) {
    // GPIO_toggle(CONFIG_GPIO_GLED);
    int_fast16_t res;
    ADC_init();
    ADC_Params params;
    ADC_Params_init(&params);

    for (uint8_t i = 0; i < sizeof(adcChannels) / sizeof(adcChannels[0]); i++) {
        ADC_Handle adc = ADC_open(adcChannels[i], &params);
        if (adc == NULL) {
            continue;
        }
        uint16_t adcValue0;

        res = ADC_convert(adc, &adcValue0);
        if (res == ADC_STATUS_SUCCESS) {
            zclGenericApp_ADCValues[i] = ADC_convertRawToMicroVolts(adc, adcValue0) / 1000000.0;
            zstack_bdbRepChangedAttrValueReq_t Req;
            Req.attrID = ATTRID_IOV_BASIC_PRESENT_VALUE;
            Req.cluster = ZCL_CLUSTER_ID_GENERAL_ANALOG_INPUT_BASIC;
            Req.endpoint = zclGenericApp_ChannelsSimpleDesc[i].EndPoint;
            Zstackapi_bdbRepChangedAttrValueReq(appServiceTaskId, &Req);
        }
        ADC_close(adc);
    }
}

static void zclGenericApp_initSensorRead(void) {
    zclGenericApp_powerOnSensors();
    UtilTimer_setTimeout(readSensorsClkHandle, GENERICAPP_SENSORS_POWER_ON_TIME);
    UtilTimer_start(&readSensorsClkStruct);
}
static void zclGenericApp_powerOnSensors(void) {
    for (size_t i = 0; i < sizeof(powerChannels) / sizeof(powerChannels[0]); i++) {
        GPIO_write(powerChannels[i], 1);
    }
}
static void zclGenericApp_powerOffSensors(void) {
    for (size_t i = 0; i < sizeof(powerChannels) / sizeof(powerChannels[0]); i++) {
        GPIO_write(powerChannels[i], 0);
    }
}

static void zclGenericApp_processSensorsReading(void) {
    zclGenericApp_processAdc();
    zclGenericApp_powerOffSensors();
}
