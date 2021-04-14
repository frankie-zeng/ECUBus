/**
*   @file    hse_srv_self_test.h
*

*   @brief   HSE Self Test service definition.
*   @details This file contains the Self Test service definition.
*
*   @addtogroup hse_srv_self_test HSE Self Test service
*   @ingroup class_admin_services
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

#ifndef HSE_SRV_SELF_TEST_H
#define HSE_SRV_SELF_TEST_H


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
* @file           hse_srv_self_test.h
*/
#include "hse_common_types.h"
#include "hse_srv_attr.h"

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

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/** @brief Self Test service.
 *  @details Performs a self-test on a specific security block or a full self-test. */
typedef struct
{
    /** @brief   INPUT: Select bits to test a specific capability
     *                  (note that the selected bits should map the supported capabilities provided by HSE).
     *                  All bits zero means that a full self-test will be performed */
    hseAttrCapabilities_t       selfTest;
    /** @brief   OUTPUT: The address where the self-tests results bit mask is returned.
     *                   If a bit is set (1) the self-test of the corresponding capability failed. */
    HOST_ADDR                   pTestResultsBitMask;
} hseSelfTestSrv_t;


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

#endif /* HSE_SRV_SELF_TEST_H */

/** @} */
