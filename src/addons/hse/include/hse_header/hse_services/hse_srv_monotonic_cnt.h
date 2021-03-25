/**
*   @file    hse_srv_monotonic_cnt.h
*
*   @brief   HSE monotonic counters services definition.
*   @details This file contains the HSE services definition for monotonic counters.
*
*   @addtogroup hse_monotonic_counters HSE Monotonic Counters
*   @ingroup class_monotonic_counters
*   @{
*/
/*==================================================================================================
*
*   (c) Copyright 2020 NXP.
*
*   This software is owned or controlled by NXP and may only be used strictly in accordance with
*   the applicable license terms. By expressly accepting such terms or by downloading, installing,
*   activating and/or otherwise using the software, you are agreeing that you have read, and that
*   you agree to comply with and are bound by, such license terms. If you do not agree to
*   be bound by the applicable license terms, then you may not retain, install, activate or
*   otherwise use the software.
==================================================================================================*/
/*==================================================================================================
==================================================================================================*/

#ifndef HSE_SRV_MONOTONIC_CNT_H
#define HSE_SRV_MONOTONIC_CNT_H


#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
/**
* @file           hse_srv_monotonic_cnt.h
*/
#include "hse_common_types.h"

#define HSE_START_PRAGMA_PACK
#include "hse_compiler_abs.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#ifdef HSE_H
/** @brief  The size of the NVM container for the Monotonic Counter table (in bytes).*/
#define HSE_NVM_CONTAINER_CNT_TBL_SIZE   ((HSE_NUM_OF_MONOTONIC_COUNTERS * sizeof(uint64_t)) + 48U )

/** @brief  Publish or load the NVM container for the Monotonic Counter table. */
typedef uint8_t hseNvmCntTblAction_t;
#define HSE_NVM_CNT_TBL_ACTION_PUBLISH   ((hseNvmCntTblAction_t)0x01U)     /**< @brief  Publish the Nvm Container for the Monotonic Counter table. */
#define HSE_NVM_CNT_TBL_ACTION_LOAD      ((hseNvmCntTblAction_t)0x02U)     /**< @brief  Load the Nvm Container for the Monotonic Counter table. */
#endif /* HSE_H */
/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/** 
 * @brief Increment a monotonic counter service with a specific value.
 * @details        
 *         - For HSE-H, the counters are volatile. Host application has publish/load
 *           the monotonic counter table using hsePublishLoadCntTblSrv_t service
 *         - For HSE-B, the host application shall use hseConfigSecCounterSrv_t service
 *           to initialize and configure the secure counters.
 *         - If the counter is saturated, an error will be reported.
 * */
typedef struct
{
    /** @brief   INPUT: The counter Index. */
    uint32_t                counterIndex;
    /** @brief   INPUT: The value to be added. */
    uint32_t                value;
}hseIncrementCounterSrv_t;

/** @brief Read a monotonic counter service. */
typedef struct
{
    /** @brief   INPUT: The counter Index. */
    uint32_t                counterIndex;
    /** @brief   OUTPUT: The address where a counter value will be returned (a uint64_t value). */
    HOST_ADDR               pCounterVal;
} hseReadCounterSrv_t;


#ifdef HSE_H
/**
 * @brief    Publish or load the monotonic counter table.
 * @details   This is supported only for HSE-H, and should be used to publish/load
 *            the monotonic counter table in NVM 
 * */
typedef struct
{
    /** @brief  INPUT:   Publish/load the NVM container for the Monotonic Counter table. */
    hseNvmCntTblAction_t    action;
    uint8_t                 reserved[3];
    /** @brief  OUTPUT:  The address of the NVM container for the Monotonic Counter table.
     *                   The size of the NVM container  is HSE_NVM_CONTAINER_CNT_TBL_SIZE. */
    HOST_ADDR               pNvmContainerCntTbl;
} hsePublishLoadCntTblSrv_t;
#endif /*HSE_H*/

#if defined(HSE_B) || defined(HSE_M)
/**
 * @brief   Initialize and configure a secure counter.
 * @details HSE supports 16 X 64 bits secure counters, each counter having associated a CounterIndex from 0 to 15.
 *          By default, all the counters are disabled.  <br>
 *          The secure counter (SC) consists of 2 separate bitfields: Rollover Protection (RP) + Volatile Counter (VC).
 *          HSE stores the secure counter in data flash each time the Rollover Protection (RP) is updated.  <br>
 *          The purpose of this service is to enable the secure counter and configure the Rollover Protection bitfield size.
 *          The objective is to reduce the rate at which NVM programming operations occur.  <br>
 *          If the secure counter is already configured and this service is called, HSE will re-configure the counter
 *          with the new Rollover Protection (RP) and reset it to 0. <br>
 *         NOTE:
 *         - WARNING: HSE will erase a flash sector after 31 Rollover Protection updates in data flash.  <br> 
 *           The number of data flash erases is limited to 100.000. The application shall configure each secure counter
 *           depending on the SC update rate and the number of enabled counters.
 *         - The secure counter configuration is stored in data flash each time hseConfigSecCounterSrv_t is called.
 *         - If RPBitSize = 64bits, HSE will store the SC in flash each time is updated.
 * 
 *         Example:
 *         Let's consider the RPBitSize = 40bits and SC = 0x0000000000000001.  <br>
 *         This means Rollover Protection (40bits) + Volatile Counter (24bits).  <br>
 *         The secure counter (SC) will be stored in flash if the incremental value is >= 0xFFFFFF. Otherwise, the counter will be updated but not stored.  <br>
 *         SC = 0x0000000000000001+0xFFFFFF = 0x0000000001000000 (RP was changed) 
 */
typedef struct
{
    /** @brief   INPUT: The counter Index. */
    uint32_t                counterIndex;
    /** @brief   INPUT: The Rollover Protection bit size (refer to service comments). */
    uint8_t                 RPBitSize;
    uint8_t                 reserved[3];
} hseConfigSecCounterSrv_t;
#endif /*defined(HSE_B) || defined(HSE_M)*/



/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/

#define HSE_STOP_PRAGMA_PACK
#include "hse_compiler_abs.h"

#ifdef __cplusplus
}
#endif

#endif /* HSE_SRV_MONOTONIC_CNT_H */

/** @} */
