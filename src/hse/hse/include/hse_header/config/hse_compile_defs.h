/**
*   @file    hse_compile_defs.h
*
*   @brief   HSE Compiler options.
*   @details This file contains macros used for compiler options.
*
*   @addtogroup hse_compiler HSE Compilers
*   @{
*/
/*==================================================================================================
*   Copyright 2019 - 2021 NXP.
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
#define HSE_S32G2XX          1U 	/**< @brief  S32G2xx platform. */
#define HSE_S32S2XX          2U 	/**< @brief  S32S2xx platform. */
#define HSE_S32K2TV          3U 	/**< @brief  S32K2TV platform. */
#define HSE_S32R45X          4U 	/**< @brief  S32R45X platform. */
#define HSE_S32K3X4          5U 	/**< @brief  S32K3X4 platform. */
#define HSE_S32R41X          6U 	/**< @brief  S32R41X platform. */
#define HSE_S32K3X0          11U 	/**< @brief  S32K3X0 platform. */
#define HSE_S32K3X1          12U 	/**< @brief  S32K3X1 platform. */
#define HSE_S32K3X2          13U 	/**< @brief  S32K3X2 platform. */
#define HSE_S32K3X8          14U 	/**< @brief  S32K3X8 platform. */
#define HSE_S32K3X6          15U 	/**< @brief  S32K3X6 platform. */

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
