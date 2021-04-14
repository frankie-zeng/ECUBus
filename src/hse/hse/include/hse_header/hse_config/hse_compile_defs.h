/**
*   @file    hse_compiler.h
*

*   @brief   HSE Compiler options.
*   @details This file contains macros used for compiler options.
*
*   @addtogroup hse_compiler HSE Compilers
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

#ifndef HSE_COMPILE_DEFS_H
#define HSE_COMPILE_DEFS_H


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
* @file           hse_compiler.h
*/

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
* Define feature states
**************************************************/

/* ***********************************************
* Define all supported HSE platform
**************************************************/
#define HSE_S32G2XX          1U /**< @brief  S32G2xx platform. */
#define HSE_S32S2XX          2U /**< @brief  S32S2xx platform. */
#define HSE_S32K2TV          3U /**< @brief  S32K2TV platform. */
#define HSE_S32R45X          4U /**< @brief  S32R45X platform. */
#define HSE_S32K3XX          5U /**< @brief  S32K3XX platform. */

/* ***********************************************
* Define flags for features which can be ON of OFF
**************************************************/
#define HSE_SPT_OFF 0U /**< @brief HSE feature support is disabled */
#define HSE_SPT_ON  1U /**< @brief HSE feature support is enabled */

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

#endif /* HSE_COMPILE_DEFS_H */

/** @} */
