/**
 *   @file    hse_compiler_abs.h
 *
 *
 *   @brief   HSE compiler specific macros.
 *   @details HSE compiler specific macros
 *
 *   @addtogroup hse_compiler_abs
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
 *  ==============================================================================================*/

/*==================================================================================================
 *  ==============================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#define PRAGMA_PACK_ERROR

#ifdef PRAGMA_PACK_H
/* Define at compile time the header file containing pragma pack definition for the compiler used. */
#include PRAGMA_PACK_H

#else

    #if defined(HSE_START_PRAGMA_PACK)

        #undef HSE_START_PRAGMA_PACK
        #if (   defined ( __ghs__ ) ||  /* GHS compiler */  \
                defined ( __CC_ARM ) || /* Arm Compiler 4/5  */  \
                defined (__ARMCC_VERSION)||/* Arm Compiler 6 (armclang) */\
                defined ( __GNUC__ ) || /* GNU Compiler*/  \
                defined ( _MSC_VER)  || /* Visual Studio */ \
                defined ( __ICCARM__ ) /* IAR Compiler*/ )
            #pragma pack(push, 4)
            #undef PRAGMA_PACK_ERROR
        #elif defined( __DCC__ ) /* DIAB compiler */
            #pragma pack(4,4,0)
            #undef PRAGMA_PACK_ERROR
        #else
            #error "Unknown compiler. Define pragma pack for the compiler."
        #endif

    #elif defined(HSE_STOP_PRAGMA_PACK)

        #undef HSE_STOP_PRAGMA_PACK
        #if (   defined ( __ghs__ ) ||  /* GHS compiler */  \
                defined ( __CC_ARM ) || /* Arm Compiler 4/5  */  \
                defined (__ARMCC_VERSION)||/* Arm Compiler 6 (armclang) */\
                defined ( __GNUC__ ) || /* GNU Compiler*/  \
                defined ( _MSC_VER)  || /* Visual Studio */ \
                defined ( __ICCARM__ ) /* IAR Compiler*/ )
            #pragma pack(pop)
            #undef PRAGMA_PACK_ERROR
        #elif defined( __DCC__ ) /* DIAB compiler */
            #pragma pack()
            #undef PRAGMA_PACK_ERROR
        #else
            #error "Unknown compiler. Define pragma pack for the compiler."
        #endif

    #else
        #error "Neither HSE_START_PRAGMA_PACK nor HSE_STOP_PRAGMA_PACK were defined before pragma pack include!"
    #endif /* HSE_START_PRAGMA_PACK || HSE_STOP_PRAGMA_PACK */

#endif /* PRAGMA_PACK_H */

#ifdef PRAGMA_PACK_ERROR
    #error "Error: Undefined or unhandled PRAGMA PACK directive!"
#endif /* PRAGMA_PACK_ERROR */

#ifdef __cplusplus
}
#endif

/** @} */
