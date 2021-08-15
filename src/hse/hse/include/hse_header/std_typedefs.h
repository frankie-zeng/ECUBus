/**
    @file    std_typedefs.h
*
*   @brief   Standard type definitions.
*   @details This file contains the Standard type definitions.
*
*   @addtogroup std_typedef Standard Type Definition
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

#ifndef STD_TYPEDEFS_H
#define STD_TYPEDEFS_H

#ifdef __cplusplus
extern "C"{
#endif


#if defined(CUSTOM_STDINT_H)
    /* Define at compile time the header file containing the definition for custom types from stdint */
    #include CUSTOM_STDINT_H
#else
    /* Default - include stdint.h */
    #include <stdint.h>
#endif

/* Standard typedefs used by header files, based on ISO C standard */
typedef volatile int8_t vint8_t;
typedef volatile uint8_t vuint8_t;

typedef volatile int16_t vint16_t;
typedef volatile uint16_t vuint16_t;

typedef volatile int32_t vint32_t;
typedef volatile uint32_t vuint32_t;

typedef volatile int64_t vint64_t;
typedef volatile uint64_t vuint64_t;

typedef uint8_t bool_t;

#ifndef TRUE
    /**
    * @brief Boolean true value
    */
    #define TRUE ((bool_t)1U)
#endif
#ifndef FALSE
    /**
    * @brief Boolean false value
    */
    #define FALSE ((bool_t)0U)
#endif


#ifndef NULL
    #define NULL ((void *)0)
#endif

#ifdef __cplusplus
}
#endif

#endif /*  STD_TYPEDEFS_H */

/** @} */
