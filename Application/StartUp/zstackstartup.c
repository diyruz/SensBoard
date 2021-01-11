/*********************************************************************
 Filename:       stack_task.c
 Revised:        $Date: 2014-03-25 11:23:30 -0700 (Tue, 25 Mar 2014) $
 Revision:       $Revision: 37868 $

 Description:    This file contains all the settings and other functions
 that the user should set and change.


 Copyright 2014 - 2015 Texas Instruments Incorporated. All rights reserved.

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
 */

/*********************************************************************
 * INCLUDES
 */
#include "ti_zstack_config.h"
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/BIOS.h>

#include "rom_jt_154.h"
#include "osal_nv.h"
#include "ti_drivers_config.h"
#include "nwk.h"
#include "aps.h"
#include "aps_frag.h"
#include "zd_app.h"
#include "zd_nwk_mgr.h"
#include "stub_aps.h"
#if defined NPI
#include "mt_task.h"
#endif

#include "mac_user_config.h"
#ifndef TIMAC_ROM_IMAGE_BUILD
#include "crypto_mac_api.h"
#endif
#include <chipinfo.h>

#include "mac_low_level.h"

/*
#if defined( CC26XX )
#include "r2r_flash_jt.h"
#include "r2f_flash_jt.h"
#endif // CC26XX
*/

#if defined DEBUG_SW_TRACE || defined DBG_ENABLE
/* Header files required for tracer enabling */
#include <ioc.h>
#include <hw_ioc.h>
#include <hw_memmap.h>
#endif // defined DEBUG_SW_TRACE || defined DBG_ENABLE

#include "zstacktask.h"
#include "zstackconfig.h"
#include "zmac.h"

#if !defined (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE)
#include "gp_interface.h"
#endif
#if defined ( BDB_TL_INITIATOR )
#include "bdb_touchlink_initiator.h"
#endif
#if defined ( BDB_TL_TARGET )
#include "bdb_touchlink_target.h"
#endif

#ifdef FEATURE_UTC_TIME
  #include "utc_clock.h"
#endif //FEATURE_UTC_TIME

#include <ti/drivers/AESCCM.h>
#include <ti/drivers/AESECB.h>

#include <ti/drivers/utils/Random.h>
#include <ti/drivers/TRNG.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>
#include <xdc/runtime/System.h>

#include "bdb.h"

#include "ti_zstack_config.h"

/*********************************************************************
 * CONSTANTS
 */

#define STACK_TASK_PRIORITY   5
#define STACK_TASK_STACK_SIZE 3072


typedef uint32_t (*pZTaskEventHandlerFn)( uint8_t task_id, uint32_t event );

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*
 * The order in this table must be identical to the task initialization calls
 * below in osalInitTask.
 */

/* service function events, one for each stack layer service function */
#ifdef NPI
static uint32_t mtServiceEvents;
#endif
static uint32_t macServiceEvents;
static uint32_t nwkServiceEvents;
#if !defined (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE)
static uint32_t gpServiceEvents;
#endif
static uint32_t apsServiceEvents;
#if defined ( ZIGBEE_FRAGMENTATION )
static uint32_t apsfServiceEvents;
#endif
static uint32_t zdAppServiceEvents;
#if defined ( ZIGBEE_FREQ_AGILITY ) || defined ( ZIGBEE_PANID_CONFLICT )
static uint32_t zdpNwkMgrServiceEvents;
#endif
//Added to include TouchLink functionality
#if defined ( INTER_PAN ) || defined ( BDB_TL_INITIATOR ) || defined ( BDB_TL_TARGET )
static uint32_t stubApsServiceEvents;
#endif
// Added to include TouchLink initiator functionality
#if defined ( BDB_TL_INITIATOR )
static uint32_t touchLinkInitiatorServiceEvents;
#endif
// Added to include TouchLink target functionality
#if defined ( BDB_TL_TARGET )
static uint32_t touchLinkTargetServiceEvents;
#endif
static uint32_t bdbServiceEvents;
static uint32_t zstackServiceEvents;

/* Pointer to the passed in configuration structure */
zstack_Config_t *pZStackCfg = NULL;

/* Stack TIRTOS Task variables */
Task_Struct stackTask;        /* not static so you can see in ROV */
static Task_Handle stackTaskHndl;
static Task_Params stackTaskParams;
static uint8_t stackTaskCallStack[STACK_TASK_STACK_SIZE];

/* Stack TIRTOS Task semaphore */
Semaphore_Struct stackSem;  /* not static so you can see in ROV */
static Semaphore_Handle stackSemHandle;

/* stack servic function ID */
static uint8_t stackServiceId;

/* handle returned from AESCCM_open() call */
AESCCM_Handle ZAESCCM_handle;
/* handle returned from AESECB_open() call */
AESECB_Handle ZAESECB_handle;
/* handle returned from TRNG_open() call */
TRNG_Handle TRNG_handle;

#ifdef NPI
extern Semaphore_Handle npiInitializationMutexHandle;
#endif

uint8_t aExtendedAddress[8];

/*
 * The order in this table must be identical to the task initialization calls
 * below in stackServiceFxnsInit.
 */
const pZTaskEventHandlerFn zstackTasksArr[] =
{
#ifdef NPI
  MT_ProcessEvent,
#endif
  ZMacEventLoop,
  nwk_event_loop,
#if !defined (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE)
  gp_event_loop,
#endif
  APS_event_loop,
#if defined ( ZIGBEE_FRAGMENTATION )
  APSF_ProcessEvent,
#endif
  ZDApp_event_loop,
#if defined ( ZIGBEE_FREQ_AGILITY ) || defined ( ZIGBEE_PANID_CONFLICT )
  ZDNwkMgr_event_loop,
#endif
//Added to include TouchLink functionality
#if defined ( INTER_PAN ) || defined ( BDB_TL_INITIATOR ) || defined ( BDB_TL_TARGET )
  StubAPS_ProcessEvent,
#endif
// Added to include TouchLink initiator functionality
#if defined ( BDB_TL_INITIATOR )
  touchLinkInitiator_event_loop,
#endif
// Added to include TouchLink target functionality
#if defined ( BDB_TL_TARGET )
  touchLinkTarget_event_loop,
#endif
  bdb_event_loop,
  ZStackTaskProcessEvent
};

/* Number of tasks in the OsalPort stack thread */
const uint8_t zstackTasksCnt = sizeof(zstackTasksArr) / sizeof(zstackTasksArr[0]);

/* */
uint32_t **pTasksEvents;

/*********************************************************************
 * FUNCTIONS
 */

static void zmain_ext_addr( uint8_t *pExtAddr );
static void stackInit(void);
static void stackServiceFxnsInit( void );
static void stackTaskFxn(UArg a0, UArg a1);

/*********************************************************************
 * @fn      stackTask_init
 *
 * @brief   This function creates the TIRTOS task for the Stack, registers the
 *          stack layers service task with OsalPort messaging and calls each of
 *          the service task init functions.
 *
 * @param   pUserCfg The stack configuration structure
 *
 * @return  none
 */
void stackTask_init( zstack_Config_t*  pUserCfg )
{
    pZStackCfg = pUserCfg;

    /* create semaphores for messages / events
     */
    Semaphore_Params semParam;
    Semaphore_Params_init(&semParam);
    semParam.mode = ti_sysbios_knl_Semaphore_Mode_COUNTING;
    Semaphore_construct(&stackSem, 0, &semParam);
    stackSemHandle = Semaphore_handle(&stackSem);

    /* create the Mac Task
     */
    Task_Params_init(&stackTaskParams);
    stackTaskParams.stackSize = STACK_TASK_STACK_SIZE;
    stackTaskParams.priority = STACK_TASK_PRIORITY;
    stackTaskParams.stack = &stackTaskCallStack;
    stackTaskParams.arg0 = (xdc_UArg) pUserCfg;
    Task_construct(&stackTask, stackTaskFxn, &stackTaskParams, NULL);
    stackTaskHndl = Task_handle(&stackTask);

#ifdef TIMAC_ROM_IMAGE_BUILD
  TIMAC_ROM_Init();
#endif

    return;
}

/*********************************************************************
 * @fn      stackTask_init
 *
 * @brief   This function creates the TIRTOS task for the Stack, registers the
 *          stack layers service functions with OsalPort messaging and calls each of
 *          the service task init functions.
 *
 * @param   none
 *
 * @return  task service ID for the main stack service task
 */
uint8_t stackTask_getStackServiceId(void)
{
    return stackServiceId;
}

/**************************************************************************************************
 * @fn          stackTaskGetTaskHndl
 *
 * @brief       This function returns the TIRTOS Task handle of the Stack Task.
 *
 * input parameters
 *
 * @param       pUserCfg - MAC user config
 *
 * output parameters
 *
 *
 * @return      Stack Task ID.
 **************************************************************************************************
 */
Task_Handle* stackTaskGetTaskHndl(void)
{
    return &stackTaskHndl;
}

/*********************************************************************
 * @fn      stackServiceFxnsInit
 *
 * @brief   This function registers the stack layers service task with
 *          OsalPort messaging and calls each of the service task init
 *          functions.
 *
 * @param   none
 *
 * @return  task service ID for the main stak service task
 */
static void stackServiceFxnsInit( void )
{
    uint8_t tmpServiceId, idx = 0;

    pTasksEvents = OsalPort_malloc( sizeof( uint32_t ) * zstackTasksCnt);

    if(pTasksEvents != NULL)
    {
#ifdef NPI
    tmpServiceId = OsalPort_registerTask(stackTaskHndl, stackSemHandle, &mtServiceEvents);
    MT_TaskInit( tmpServiceId );
    pTasksEvents[idx] = &mtServiceEvents;
    idx++;
#endif

    tmpServiceId = OsalPort_registerTask(stackTaskHndl, stackSemHandle, &macServiceEvents);
    MAP_macMainSetTaskId( tmpServiceId );
    pTasksEvents[idx] = &macServiceEvents;
    idx++;

    tmpServiceId = OsalPort_registerTask(stackTaskHndl, stackSemHandle, &nwkServiceEvents);
    nwk_init( tmpServiceId );
    pTasksEvents[idx] = &nwkServiceEvents;
    idx++;

#if !defined (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE)
    tmpServiceId = OsalPort_registerTask(stackTaskHndl, stackSemHandle, &gpServiceEvents);
    gp_Init( tmpServiceId );
    pTasksEvents[idx] = &gpServiceEvents;
    idx++;
#endif

    tmpServiceId = OsalPort_registerTask(stackTaskHndl, stackSemHandle, &apsServiceEvents);
    APS_Init( tmpServiceId );
    pTasksEvents[idx] = &apsServiceEvents;
    idx++;

#if defined ( ZIGBEE_FRAGMENTATION )
    tmpServiceId = OsalPort_registerTask(stackTaskHndl, stackSemHandle, &apsfServiceEvents);
    APSF_Init( tmpServiceId );
    pTasksEvents[idx] = &apsfServiceEvents;
    idx++;
#endif

    tmpServiceId = OsalPort_registerTask(stackTaskHndl, stackSemHandle, &zdAppServiceEvents);
    ZDApp_Init( tmpServiceId );
    pTasksEvents[idx] = &zdAppServiceEvents;
    idx++;

#if defined ( ZIGBEE_FREQ_AGILITY ) || defined ( ZIGBEE_PANID_CONFLICT )
    tmpServiceId = OsalPort_registerTask(stackTaskHndl, stackSemHandle, &zdpNwkMgrServiceEvents);
    ZDNwkMgr_Init( tmpServiceId );
    pTasksEvents[idx] = &zdpNwkMgrServiceEvents;
    idx++;
#endif

    // Added to include TouchLink functionality
#if defined ( INTER_PAN ) || defined ( BDB_TL_INITIATOR ) || defined ( BDB_TL_TARGET )
    tmpServiceId = OsalPort_registerTask(stackTaskHndl, stackSemHandle, &stubApsServiceEvents);
    StubAPS_Init( tmpServiceId );
    pTasksEvents[idx] = &stubApsServiceEvents;
    idx++;
#endif

      // Added to include TouchLink initiator functionality
#if defined( BDB_TL_INITIATOR )
    tmpServiceId = OsalPort_registerTask(stackTaskHndl, stackSemHandle, &touchLinkInitiatorServiceEvents);
    touchLinkInitiator_Init( tmpServiceId );
    pTasksEvents[idx] = &touchLinkInitiatorServiceEvents;
    idx++;
#endif

      // Added to include TouchLink target functionality
#if defined ( BDB_TL_TARGET )
    tmpServiceId = OsalPort_registerTask(stackTaskHndl, stackSemHandle, &touchLinkTargetServiceEvents);
    touchLinkTarget_Init( tmpServiceId );
    pTasksEvents[idx] = &touchLinkTargetServiceEvents;
    idx++;
#endif

    tmpServiceId = OsalPort_registerTask(stackTaskHndl, stackSemHandle, &bdbServiceEvents);
    bdb_Init( tmpServiceId );
    pTasksEvents[idx] = &bdbServiceEvents;
    idx++;

    stackServiceId = OsalPort_registerTask(stackTaskHndl, stackSemHandle, &zstackServiceEvents);
    ZStackTaskInit( stackServiceId );
    pTasksEvents[idx] = &zstackServiceEvents;
    }
    else
    {
        // handle malloc error
    }

  return;
}

/**************************************************************************************************
 * @fn          stackTaskFxn
 *
 * @brief       This function is the main event handling function for the stack service functions.
 *
 * input parameters
 *
 * @param       a0  Pointer to the MAC User Config structure
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 **************************************************************************************************
 */
static void stackTaskFxn(UArg a0, UArg a1)
{
    //  initialize the MAC

    stackInit();

    OsalPortTimers_registerCleanupEvent(stackServiceId, OSALPORT_CLEAN_UP_TIMERS_EVT);

// race condition: Stack task must register with OsalPort before NPI task,
// but NPI task must register with OsalPort *before* stack task is able to
// start running. This Mutex is unlocked by the NPI task after it has
// finished initializing.
#ifdef NPI
    Semaphore_pend(npiInitializationMutexHandle, BIOS_WAIT_FOREVER);
#endif

    while(1)
    {
      /* Block here until TIRTOS task gets an event */
      Semaphore_pend(stackSemHandle, BIOS_WAIT_FOREVER);

      uint8_t idx = 0;

     // copy the priority-based round-robin scheduler from OSAL
      do {
        if (*(pTasksEvents[idx]))  // Task is highest priority that is ready.
        {
          break;
        }
      } while (++idx < zstackTasksCnt);

      if (idx < zstackTasksCnt)
      {
        uint32_t events, key;

        key = OsalPort_enterCS();
        events = *(pTasksEvents[idx]);
        *(pTasksEvents[idx]) = 0;  // Clear the Events for this task.
        OsalPort_leaveCS(key);

        events = (zstackTasksArr[idx])( 0U, events );

        key = OsalPort_enterCS();
        *(pTasksEvents[idx]) |= events;  // Add back unprocessed events to the current task.
        OsalPort_leaveCS(key);
      }
    }
}

/**************************************************************************************************
 * @fn          stackInit
 *
 * @brief       This function is called to initialize the MAC.
 *
 * input parametersS
 *
 * @param       pUserCfg - MAC user config
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 **************************************************************************************************
 */
static void stackInit(void)
{
    uintptr_t key;
    uint8_t *pExtAddr = NULL;
#if !defined(USE_DMM) || (!defined(BLE_START) && defined(USE_DMM))
    AESCCM_Params AESCCMParams;
	AESECB_Params AESECBParams;

    uint32_t status;
#endif
    TRNG_Params TRNGParams;

    if ( pZStackCfg )
    {
      pZStackCfg->initFailStatus = ZSTACKCONFIG_INIT_SUCCESS;
    }

    /* User App Reconfiguration of TIMAC */
    macSetUserConfig( &(pZStackCfg->macConfig) );

#if defined( DEBUG_SW_TRACE )
#if defined(HAL_UART) && (HAL_UART==TRUE)
    // Enable tracer output on DIO24/ATEST1.
    // Set max drive strength and reduced slew rate.
    // PortID = 46 = RFcore tracer.
    HWREG(IOC_BASE + IOC_O_IOCFG24) = (3 << IOC_IOCFG24_IOSTR_S) | (1 << IOC_IOCFG24_SLEW_RED_S) | 46;

#else // no UART so allow trace on Tx pin - Debug Package only
    // Enable tracer output on DIO23/ATEST0.
    // Set max drive strength and reduced slew rate.
    // PortID = 46 = RFcore tracer.
    HWREG(IOC_BASE + IOC_O_IOCFG23) = (3 << IOC_IOCFG23_IOSTR_S) | (1 << IOC_IOCFG24_SLEW_RED_S) | 46;
#endif // HAL_UART
#endif // DEBUG_SW_TRACE

    /* Disable interrupts */
    key = HwiP_disable();

    // Have to turn on the TRNG power before HalTRNG_InitTRNG
    // but must not repeat it every time the device wakes up
    // hence the call cannot be added to HalTRNG_InitTRNG();
    Power_setDependency(PowerCC26XX_PERIPH_TRNG);
#if !defined(USE_DMM) || (!defined(BLE_START) && defined(USE_DMM))
    // Setup the PRNG
    status = Random_seedAutomatic();

    if (status != Random_STATUS_SUCCESS) {
        System_abort("Random_seedAutomatic() failed");
    }


    TRNG_init();
    TRNG_Params_init(&TRNGParams);
    TRNGParams.returnBehavior = TRNG_RETURN_BEHAVIOR_POLLING;
    TRNG_handle = TRNG_open(CONFIG_TRNG_0, &TRNGParams);

    // Initialize AESCCM Driver
    AESCCM_init();
    AESCCM_Params_init(&AESCCMParams);
    AESCCMParams.returnBehavior = AESCCM_RETURN_BEHAVIOR_POLLING;
    ZAESCCM_handle = AESCCM_open(CONFIG_AESCCM_0, &AESCCMParams);

    // Initialize AESECB Driver
    AESECB_init();
    AESECB_Params_init(&AESECBParams);
    AESECBParams.returnBehavior = AESECB_RETURN_BEHAVIOR_POLLING;
    ZAESECB_handle = AESECB_open(CONFIG_AESECB_0, &AESECBParams);
#else
    extern AESCCM_Handle encHandleCCM;
    ZAESCCM_handle = encHandleCCM;

    extern AESECB_Handle encHandleECB;
    ZAESECB_handle = encHandleECB;

    TRNG_init();
    TRNG_Params_init(&TRNGParams);
    TRNGParams.returnBehavior = TRNG_RETURN_BEHAVIOR_POLLING;
    TRNG_handle = TRNG_open(CONFIG_TRNG_1, &TRNGParams);
#endif

    if (!ZAESCCM_handle)
    {
        /* abort */
        System_abort("AESCCM open failed");
    }

    if (!ZAESECB_handle)
    {
        /* abort */
        System_abort("AESECB open failed");
    }

    if (!TRNG_handle)
    {
        /* abort */
        System_abort("TRNG open failed");
    }

    /* Initialize MAC */
    ZMacInit();

    if ( pZStackCfg )
    {
        uint8_t dummyAddr[8];
        memset( dummyAddr, 0xFF, Z_EXTADDR_LEN );

        if ( OsalPort_memcmp( pZStackCfg->extendedAddress, dummyAddr, 8 ) != TRUE )
        {
             pExtAddr = pZStackCfg->extendedAddress;
        }
    }

    // Determine the extended address
    zmain_ext_addr( pExtAddr );

    // Initialize basic NV items
    zgInit();

    // Since the AF isn't a task, call it's initialization routine
    afInit();

    stackServiceFxnsInit();

    //Initialize default poll rates
    nwk_InitializeDefaultPollRates();

    /* Initialize MAC buffer */
    macLowLevelBufferInit();

    /* Enable interrupts */
    HwiP_restore(key);

    // Must be done last
    macLowLevelInit();

    ZMacSrcMatchEnable();

    ZMacReset( TRUE );

    uint8_t responseWaitTime = 16;
    ZMacSetReq(MAC_RESPONSE_WAIT_TIME, &responseWaitTime);

    uint16_t transactionPersistenceTime = 500;
    ZMacSetReq(MAC_TRANSACTION_PERSISTENCE_TIME, (byte *)&transactionPersistenceTime);

    ZMacSetTransmitPower( (ZMacTransmitPower_t)TXPOWER );

    if(RFD_RCVC_ALWAYS_ON == TRUE)
    {
        nwk_SetCurrentPollRateType(POLL_RATE_RX_ON_TRUE,TRUE);
    }

#ifdef FEATURE_UTC_TIME
    UTC_init();
#endif //FEATURE_UTC_TIME
}

/*********************************************************************
 * @fn          zmain_ext_addr
 *
 * @brief       Execute a prioritized search for a valid extended address and
 *              write the results into the OSAL NV system for use by the system.
 *              Temporary address not saved to NV.
 *
 * @param       pExtAddr - Pointer to passed in extended address.
 *
 * @return      None.
 */
static void zmain_ext_addr( uint8_t *pExtAddr )
{
  uint8_t pDefaultBuf[Z_EXTADDR_LEN] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  uint8_t *pLocalExtAddr = NULL;

  // if the input parameter is NULL, randomly generate an address
  if( pExtAddr == NULL )
  {
    pLocalExtAddr = OsalPort_malloc(Z_EXTADDR_LEN);
    if(pLocalExtAddr)
    {
      uint8_t idx;

      /* Attempt to create a sufficiently random extended address for expediency.
      * Note: this is only valid/legal in a test environment and
      *       must never be used for a commercial product.
      */
      for ( idx = 0; idx < (Z_EXTADDR_LEN - 2); )
      {
        uint16_t randy = OsalPort_rand();
        pLocalExtAddr[idx++] = LO_UINT16( randy );
        pLocalExtAddr[idx++] = HI_UINT16( randy );
      }

      // Next-to-MSB identifies ZigBee devicetype.
      #if ZG_BUILD_COORDINATOR_TYPE && !ZG_BUILD_JOINING_TYPE
        pLocalExtAddr[idx++] = 0x10;
      #elif ZG_BUILD_RTRONLY_TYPE
        pLocalExtAddr[idx++] = 0x20;
      #else
        pLocalExtAddr[idx++] = 0x30;
      #endif
        // MSB has historical significance.
        pLocalExtAddr[idx] = 0xF8;

      // initialize aExtendedAddress RAM variable to random address
      // if there is an IEEE Addr in NV, this will be overwritten below
      OsalPort_memcpy( aExtendedAddress, pLocalExtAddr, 8 );
      OsalPort_free( pLocalExtAddr );
    }
    else
    {
      // handle malloc error
      return;
    }
  }
  else
  {
    // initialize aExtendedAddress RAM variable to the input parameter
    // if there is an IEEE Addr in NV, this will be overwritten below
    OsalPort_memcpy( aExtendedAddress, pExtAddr, 8 );
  }

  uint8_t status = osal_nv_item_init(ZCD_NV_EXTADDR, Z_EXTADDR_LEN, pDefaultBuf);

  if((status != SUCCESS) && (status != NV_ITEM_UNINIT))
  {
    // handle NV failure. RAM variable should be valid at this point already but
    // NV will have not been initialized
  }
  else if( zgReadStartupOptions() & ZCD_STARTOPT_DEFAULT_NETWORK_STATE )
  {
    // default network state requested, so we do not want to restore the
    // IEEE Addr from NV. overwite it with the new address
    if ( SUCCESS != osal_nv_write(ZCD_NV_EXTADDR, Z_EXTADDR_LEN, aExtendedAddress) )
    {
      // handle NV write failure. RAM variable should be valid at this point already but
      // NV will have not been updated.
    }
  }
  else
  {
    // try to read IEEE Addr from NV. If it fails, we will update it based on
    // input parameter.

    uint8_t tempAddr[Z_EXTADDR_LEN];
    memset( pDefaultBuf, 0xFF, Z_EXTADDR_LEN );
    memset( tempAddr, 0xFF, Z_EXTADDR_LEN );

    if ( SUCCESS != osal_nv_read(ZCD_NV_EXTADDR, 0, Z_EXTADDR_LEN, tempAddr) )
    {
      // handle NV read failure.
    }
    else
    {
      // check if the address we just read from NV is 0xFF...
      if( OsalPort_memcmp(tempAddr, pDefaultBuf, Z_EXTADDR_LEN) )
      {
        // if so, overwrite the IEEE Addr in NV with the new address
        if ( SUCCESS != osal_nv_write(ZCD_NV_EXTADDR, Z_EXTADDR_LEN, aExtendedAddress) )
        {
          // handle NV write failure. RAM variable should be valid at this point already but
          // NV will have not been updated.
        }
      }
      else
      {
        // IEEE Addr in NV is assumed to be valid at this point, update aExtendedAddress
        OsalPort_memcpy( aExtendedAddress, tempAddr, 8 );
      }
    }
  }
  // Set the MAC PIB extended address according to results from above.
  (void) ZMacSetReq( MAC_EXTENDED_ADDRESS, aExtendedAddress );
}

/*********************************************************************
 *********************************************************************/
