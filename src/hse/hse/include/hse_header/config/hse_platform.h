/**
*   @file    hse_platform.h
*
*   @brief   HSE platform options.
*   @details This file contains macros used for platform compiler options.
*
*   @addtogroup hse_platform HSE Platform
*   @{
*/
/*==================================================================================================
*
*   Copyright 2019 - 2021 NXP.
*
*   This software is owned or controlled by NXP and may only be used strictly in accordance with
*   the applicable license terms. By expressly accepting such terms or by downloading, installing,
*   activating and/or otherwise using the software, you are agreeing that you have read, and that
*   you agree to comply with and are bound by, such license terms. If you do not agree to
*   be bound by the applicable license terms, then you may not retain, install, activate or
*   otherwise use the software.
==================================================================================================*/

#ifndef HSE_PLATFORM_H
#define HSE_PLATFORM_H


#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "hse_target.h"

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

#if (HSE_PLATFORM == HSE_S32G2XX) || (HSE_PLATFORM == HSE_S32R45X)
    #define HSE_H
    #define HSE_SPT_64BIT_ADDR              /**< @brief  HSE supports 64-bit address. */
    /* ENABLE SPECIFIC FEATURES HERE */

#elif (HSE_PLATFORM == HSE_S32S2XX)
    #define HSE_H

#elif (HSE_PLATFORM == HSE_S32R41X)
    #define HSE_M

#elif ((HSE_PLATFORM == HSE_S32K3X0)||(HSE_PLATFORM == HSE_S32K3X1)||(HSE_PLATFORM == HSE_S32K3X2)||(HSE_PLATFORM == HSE_S32K3X4)||(HSE_PLATFORM == HSE_S32K3X6)||(HSE_PLATFORM == HSE_S32K3X8))
    #define HSE_B
#else
    #error "No platform is selected. At least one platform should be selected."
#endif


#if defined(HSE_H)
    #include "hse_h_config.h"
#elif defined(HSE_M)
    #include "hse_m_config.h"
#elif defined(HSE_B)
    #include "hse_b_config.h"
#else
    #error "No HSE HW variant is selected. At least one HSE HW variant should be selected."
#endif


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

#endif /* HSE_PLATFORM_H */

/** @} */
