/**
*   @file    hse_srv_sys_authorization.h
*
*   @brief   HSE System Authorization definition.
*   @details This file defines the System Authorization services.
*
*   @addtogroup hse_srv_sys_authorization HSE System Authorization Services
*   @ingroup class_admin_services
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

#ifndef HSE_SRV_SYS_AUTHORIZATION_H
#define HSE_SRV_SYS_AUTHORIZATION_H

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

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

/** @brief HSE System Access rights.
 *   @details
 *    After reset (default access rights):
 *    | Life Cycle | NVM CUST keys   | NVM OEM keys  | RAM keys  | NVM config|
 *    |-----------:|:---------------:|:-------------:|:---------:|:---------:|
 *    |CUST_DEL    |      SU/U*      |       U       |   SU/U*   |   SU/U*   |
 *    |OEM_PROD    |        U        |     SU/U*     |   SU/U*   |   SU/U*   |
 *    |IN_FIELD    |        U        |       U       |     U     |     U     |
 *
 *   After reset, the SYS rights are synchronized with Life cycle (LC) and CUST/OEM START_AS_USER policy attributes (see CUST/OEM policy attributes).
 *      - if LC = CUST_DEL:
 *           - if CUST_START_AS_USER policy = FALSE, CUST SuperUser rights are granted (CUST NVM Keys / NVM configuration updates)
 *           - otherwise User rights are granted (U* in the above table)
 *      - if LC = OEM_DEL:
 *           - if OEM_START_AS_USER policy = FALSE, OEM SuperUser rights are granted (OEM NVM Keys / NVM configuration updates)
 *           - otherwise User rights are granted (U* in the above table)
 *      - if LC = IN_FIELD, User rights are granted.
 */
typedef uint8_t hseSysRights_t;
/** @brief SuperUser rights: can install/update CUST/OEM NVM keys or RAM keys using less restrictions. <br>
           CUST/OEM SuperUser restrictions are specific to CUST_DEL/OEM_PROD Life cycle. */
#define HSE_RIGHTS_SUPER_USER           ((hseSysRights_t)1U)
/** @brief User rights: can install/update NVM/RAM keys using high restrictions. <br>
           User restrictions are specific to IN_FILED life cycle. */
#define HSE_RIGHTS_USER                 ((hseSysRights_t)2U)

/** @brief   HSE System Authorization options.
 *  @details Specifies the services for which the system authorization is performed.
 */
typedef uint8_t hseSysAuthOption_t;
/** @brief Request SuperUser rights for Key Management services (e.g. import/export/erase/key generate/key derive). <br>
 *         If SuperUser rights are granted, Key Management services can be performed using less restrictions. */
#define HSE_SYS_AUTH_KEY_MGMT            ((hseSysAuthOption_t)(1U << 0U))
/** @brief Request SuperUser rights to update/install the HSE NVM tables/attributes which are stored in SYS-IMAGE(HSE-H)/internal flash(HSE-M/B) (e.g. SMR, CR, OTFAD, NVM attributes). <br>
 *         If SuperUser rights are granted, updates of NVM configuration will be permitted. */
#define HSE_SYS_AUTH_NVM_CONFIG          ((hseSysAuthOption_t)(1U << 1U))
/** @brief Request SuperUser rights for both Key Management services and NVM configuration updates. */
#define HSE_SYS_AUTH_ALL                 ((HSE_SYS_AUTH_KEY_MGMT) | (HSE_SYS_AUTH_NVM_CONFIG))

/** @brief Challenge length: Length of the challenge (in bytes) returned by a successful authorization request. */
#define HSE_SYS_AUTH_CHALLENGE_LENGTH    (32UL)

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/


/**
 * @brief     HSE SYS Authorization Request service.
 * @details
 *    During run-time (IN_FIELD Life cycle), the User rights can be temporarily elevated to SuperUser(CUST/OEM) using HSE Authorization Request/Response.
 *    - CUST SuperUser rights are granted using an authorization key owned by CUST.
 *    - OEM SuperUser rights are granted using an authorization key owned by OEM.
 *    - The User rights (non privilege rights) can be requested without authorization.
 *      In this case, HSE_SYS_Authorization_Resp shall not be used.
 *
 * @note
 *    - After reset, the default access rights are used (see #hseSysRights_t).
 *    - If no authorization key is installed during CUST_DEL or OEM_PROD life cycle, the keys can be updated only having USER rights.
 *    - HSE FW can perform only one SYS Authorization Request at a time. A second request will overwrite the first request.
 *    - An authorization key is a NVM key that can only be used for verify.
 *    - If authorization succeeds, it will be opened on the MU Interface on which the request was performed,
 *      and the services that needs authorization  (e.g. key import/generate/derive/export) must be performed on the same MU Interface.
 *    - The system authorization procedure can be used to emulate the SHE CMD_DEBUG using the MASTER_ECU_KEY key (as per SHE specification).
 *      In this case, if SU access rights are requested for Key Management services (see #hseSysAuthOption_t),
 *      the authorization using MASTER_ECU_KEY cannot be performed if any SHE key has the WRITE_PROTECTED flag set. <br>
 *      Access rights requested only for NVM Configuration services (see #hseSysAuthOption_t) are not bound to this condition.
 *      Note that SHE keys can be erased only if the authorization was performed with the MASTER_ECU_KEY (refer to #hseEraseKeySrv_t).
 */
typedef struct
{
    /** @brief   INPUT: Authorization option: Key management/NVM configuration/Both. */
    hseSysAuthOption_t  sysAuthOption;
    /** @brief   INPUT: Requested system rights: SuperUser (CUST/OEM) or User rights. */
    hseSysRights_t      sysRights;
    uint8_t             reserved[2];
    /** @brief   INPUT: The owner key handle:
     *           -  if sysRights = HSE_RIGHTS_SUPER_USER, it shall be a CUST or OEM key used for only for signature verification.
     *           -  if sysRights = HSE_RIGHTS_USER, the key handle is not used. */
    hseKeyHandle_t      ownerKeyHandle;
    /** @brief   INPUT: Authentication scheme. ONLY RSA, ECDSA, EDDSA and CMAC schemes are supported. <br>
     *                  If sysRights = HSE_RIGHTS_USER, authScheme is not used. 
     *                  @note
     *                      - EDDSA scheme with user provided context (eddsa.contextLength != 0) is NOT supported.*/
    hseAuthScheme_t     authScheme;
    /** @brief   OUTPUT: The output challenge that needs to be signed by the HOST.
     *                   In case SHE MASTER_ECU_KEY is used, the returned challenge is HSE_SYS_AUTH_CHALLENGE_LENGTH - 1 byte long and
     *                   is formed from 16 random bytes concatenated with SHE UID: (RANDOM(16 bytes) || SHE_UID(15 bytes)). <br>
     *                   Otherwise, for any other key type, the challenge size is HSE_SYS_AUTH_CHALLENGE_LENGTH bytes.
     *                   If sysRights = HSE_RIGHTS_USER, pChallenge is not used. */
    HOST_ADDR           pChallenge;
} hseSysAuthorizationReqSrv_t;


/**
 * @brief           HSE SYS Authorization Response service.
 * @details         Provides the signature for the requested challenge (using hseSysAuthorizationReqSrv_t service).
 * @note
 *      - In case SHE MASTER_ECU key is used, the HSE will return the HSE_SRV_RSP_VERIFY_FAILED status
 *        as the equivalent of ERC_NO_DEBUGGING status as specified by the SHE spec (returned when the tag over the challenge is not correct).
 */
typedef struct
{
    /** @brief   INPUT: Byte length(s) of the authentication tag(s).
     *                   @note
     *                   - For RSA signature and CMAC only authLen[0] is used.
     *                   - Both lengths are used for (R,S) (ECC). */
    uint16_t          authLen[2];
    /** @brief   INPUT: Address(es) to authentication tag.
     *                   @note
     *                   - For RSA signature and CMAC only pAuth[0] is used.
     *                   - Both pointers are used for (R,S) (ECC).
     *                   - If SHE MASTER_ECU_KEY is used, the CMAC must be computed over the challenge (31 bytes) using a derived key (as per SHE specification). */
    HOST_ADDR         pAuth[2];
} hseSysAuthorizationRespSrv_t;

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

#endif /* HSE_SRV_SYS_AUTHORIZATION_H */

/** @} */
