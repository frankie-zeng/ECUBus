/**
*   @file    hse_gpr_status.h
*
*   @brief   HSE GPR Status
*   @details This file contains the HSE GPR Status
*
*   @addtogroup hse_gpr HSE GPR Status
*   @ingroup class_interface
*   @{
*/
/*==================================================================================================
*
*   Copyright 2021 NXP.
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

#ifndef HSE_GPR_STATUS_H
#define HSE_GPR_STATUS_H

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "std_typedefs.h"


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

/** @brief    HSE Tamper Config Register Address
 *  @details  This status GPR register is updated when a tamper is configured in HSE during initialization or via attribute.
 *            (see #HSE_TEMP_SENSOR_VIO_CONFIG_ATTR_ID, #HSE_PHYSICAL_TAMPER_ATTR_ID).
 *            The HOST can read the HSE register to check what tampers are configured;
 *            This register is read-only.
 *            @note
 *            - For HSE_H/M, HSE-GPR REG3 used.
 *            - For HSE_B,   CONFIG_REG4 used.
 */
#if defined(HSE_H)
    #define HSE_GPR_STATUS_ADDRESS                (0x4007C920UL)   /**< @brief HSE-GPR REG3 is in Subsytem Register Description */
#elif defined(HSE_M)
    #define HSE_GPR_STATUS_ADDRESS                (0x400D8928UL)   /**< @brief HSE-GPR REG3 is in Subsytem Register Description */
#elif defined(HSE_B)
    #define HSE_GPR_STATUS_ADDRESS                (0x4039C02CUL)   /**< @brief CONFIG_REG4 is in Configuration GPR Description */
#endif

/** @brief    HSE Tamper Config Status bits */
typedef uint32_t hseTamperConfigStatus_t;
#define HSE_CMU_TAMPER_CONFIG_STATUS        ((hseTamperConfigStatus_t)1U << 0U) /**< @brief HSE-GPR REG3[0] This bit is set when the
                                                                                            configuration of CMU tamper:
                                                                                            - For HSE-H, the clock must be configured in
                                                                                               this range: 10Mhz < clock frequency < 420Mhz.
                                                                                            - For HSE-B, the clock must be configured in
                                                                                               this range: 3Mhz < clock frequency < 126Mhz. */

#ifdef HSE_SPT_PHYSICAL_TAMPER_CONFIG
#define HSE_PHYSICAL_TAMPER_CONFIG_STATUS    ((hseTamperConfigStatus_t)1U << 1U) /**< @brief HSE-GPR REG3[1] this bit is set when the
                                                                                             configuration of Physical Tamper:
                                                                                             - Application must configure SIUL2 Pads before
                                                                                               enabling the tamper. */
#endif /* HSE_SPT_PHYSICAL_TAMPER_CONFIG */

#ifdef HSE_SPT_TEMP_SENS_VIO_CONFIG
#define HSE_TEMP_SENSOR_VIO_CONFIG_STATUS    ((hseTamperConfigStatus_t)1U << 2U) /**< @brief HSE-GPR REG3[2] this bit is set when the
                                                                                             configuration of Temperature Sensor violation. */
#endif /* HSE_SPT_TEMP_SENS_VIO_CONFIG */


/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

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

#endif /* HSE_GPR_STATUS_H */

/** @} */
