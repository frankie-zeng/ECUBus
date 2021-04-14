/**
*   @file    hse_target.h
*

*   @brief   HSE Target options.
*   @details This file contains macros used for target options.
*
*   @addtogroup hse_target HSE Targets
*   @{
*/
/*==================================================================================================
*
*   Platform             : HSE_S32K3XX
*   FW Type              : STANDARD
*   SW Version           : 0.8.3
*
*   (c) Copyright 2019 NXP.
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

#ifndef TARGET_H
#define TARGET_H


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
* @file           hse_target.h
*/
#include "hse_compile_defs.h"

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

/* ***********************************************
* Define HSE FW type
**************************************************/
#define STANDARD 0

/* ***********************************************
* Define current configuration
**************************************************/
#define HSE_PLATFORM       HSE_S32K3XX              /**< @brief  HSE Platform. */
#define HSE_FWTYPE         STANDARD                /**< @brief  HSE FW Type. */
/* #undef HSE_SPT_EXTENSION */

/** @brief    HSE Firmware Version */
#define HSE_FW_VERSION { \
    0U,     /* reserved */ \
    5U,     /* socTypeId */ \
    0U,     /* fwTypeId */ \
    0U,     /* majorVersion */ \
    8U,     /* minorVersion */ \
    3U      /* patchVersion */ \
}

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

#ifdef __cplusplus
}
#endif

#endif /* TARGET_H */

/** @} */
