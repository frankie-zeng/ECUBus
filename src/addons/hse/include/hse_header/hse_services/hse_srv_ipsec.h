/**
*   @file    hse_srv_ipsec.h
*   

*   @brief   HSE IPSEC acceleration service
*   @details This file contains the definition for the HSE IPSEC stateful protocol acceleration 
*            service
*
*   @addtogroup hse_srv_ipsec HSE IPSEC acceleration service
*   @ingroup class_network
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

#ifndef HSE_SRV_IPSEC_H
#define HSE_SRV_IPSEC_H


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
* @file           hse_srv_ipsec.h
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

/** @brief IPSEC security association ID type.
  * @details The IPSEC security association ID identifies an internal IPSEC context used to 
  * hold the state of any established IPSEC security association, managed by the HSE IPSEC acceleration service. */
typedef uint32_t hseIpsecSaId_t;

/** @brief    HSE IPSEC suite IDs.*/
typedef uint16_t hseIpsecSuite_t;
#define HSE_IPSEC_SUITE_TDES_NULL                   ((hseIpsecSuite_t)0x0300U)
#define HSE_IPSEC_SUITE_TDES_HMAC_MD5_96            ((hseIpsecSuite_t)0x0301U)
#define HSE_IPSEC_SUITE_TDES_HMAC_SHA1_96           ((hseIpsecSuite_t)0x0302U)
#define HSE_IPSEC_SUITE_TDES_AES_XCBC_MAC_96        ((hseIpsecSuite_t)0x0305U)
#define HSE_IPSEC_SUITE_TDES_HMAC_MD5_128           ((hseIpsecSuite_t)0x0306U)
#define HSE_IPSEC_SUITE_TDES_HMAC_SHA1_160          ((hseIpsecSuite_t)0x0307U)
#define HSE_IPSEC_SUITE_TDES_AES_CMAC_96            ((hseIpsecSuite_t)0x0308U)
#define HSE_IPSEC_SUITE_TDES_HMAC_SHA2_256_128      ((hseIpsecSuite_t)0x030cU)
#define HSE_IPSEC_SUITE_TDES_HMAC_SHA2_384_192      ((hseIpsecSuite_t)0x030dU)
#define HSE_IPSEC_SUITE_TDES_HMAC_SHA2_512_256      ((hseIpsecSuite_t)0x030eU)
#define HSE_IPSEC_SUITE_TDES_HMAC_SHA2_224_96       ((hseIpsecSuite_t)0x03f2U)
#define HSE_IPSEC_SUITE_TDES_HMAC_SHA2_224_112      ((hseIpsecSuite_t)0x03f4U)
#define HSE_IPSEC_SUITE_TDES_HMAC_SHA2_224_224      ((hseIpsecSuite_t)0x03f8U)
#define HSE_IPSEC_SUITE_NULL_NULL                   ((hseIpsecSuite_t)0x0b00U)
#define HSE_IPSEC_SUITE_NULL_HMAC_MD5_96            ((hseIpsecSuite_t)0x0b01U)
#define HSE_IPSEC_SUITE_NULL_HMAC_SHA1_96           ((hseIpsecSuite_t)0x0b02U)
#define HSE_IPSEC_SUITE_NULL_AES_XCBC_MAC_96        ((hseIpsecSuite_t)0x0b05U)
#define HSE_IPSEC_SUITE_NULL_HMAC_MD5_128           ((hseIpsecSuite_t)0x0b06U)
#define HSE_IPSEC_SUITE_NULL_HMAC_SHA1_160          ((hseIpsecSuite_t)0x0b07U)
#define HSE_IPSEC_SUITE_NULL_AES_CMAC_96            ((hseIpsecSuite_t)0x0b08U)
#define HSE_IPSEC_SUITE_NULL_HMAC_SHA2_256_128      ((hseIpsecSuite_t)0x0b0cU)
#define HSE_IPSEC_SUITE_NULL_HMAC_SHA2_384_192      ((hseIpsecSuite_t)0x0b0dU)
#define HSE_IPSEC_SUITE_NULL_HMAC_SHA2_512_256      ((hseIpsecSuite_t)0x0b0eU)
#define HSE_IPSEC_SUITE_NULL_HMAC_SHA2_224_96       ((hseIpsecSuite_t)0x0bf2U)
#define HSE_IPSEC_SUITE_NULL_HMAC_SHA2_224_112      ((hseIpsecSuite_t)0x0bf4U)
#define HSE_IPSEC_SUITE_NULL_HMAC_SHA2_224_224      ((hseIpsecSuite_t)0x0bf8U)
#define HSE_IPSEC_SUITE_AES_CBC_NULL                ((hseIpsecSuite_t)0x0c00U)
#define HSE_IPSEC_SUITE_AES_CBC_HMAC_MD5_96         ((hseIpsecSuite_t)0x0c01U)
#define HSE_IPSEC_SUITE_AES_CBC_HMAC_SHA1_96        ((hseIpsecSuite_t)0x0c02U)
#define HSE_IPSEC_SUITE_AES_CBC_AES_XCBC_MAC_96     ((hseIpsecSuite_t)0x0c05U)
#define HSE_IPSEC_SUITE_AES_CBC_HMAC_MD5_128        ((hseIpsecSuite_t)0x0c06U)
#define HSE_IPSEC_SUITE_AES_CBC_HMAC_SHA1_160       ((hseIpsecSuite_t)0x0c07U)
#define HSE_IPSEC_SUITE_AES_CBC_AES_CMAC_96         ((hseIpsecSuite_t)0x0c08U)
#define HSE_IPSEC_SUITE_AES_CBC_HMAC_SHA2_256_128   ((hseIpsecSuite_t)0x0c0cU)
#define HSE_IPSEC_SUITE_AES_CBC_HMAC_SHA2_384_192   ((hseIpsecSuite_t)0x0c0dU)
#define HSE_IPSEC_SUITE_AES_CBC_HMAC_SHA2_512_256   ((hseIpsecSuite_t)0x0c0eU)
#define HSE_IPSEC_SUITE_AES_CBC_HMAC_SHA2_224_96    ((hseIpsecSuite_t)0x0cf2U)
#define HSE_IPSEC_SUITE_AES_CBC_HMAC_SHA2_224_112   ((hseIpsecSuite_t)0x0cf4U)
#define HSE_IPSEC_SUITE_AES_CBC_HMAC_SHA2_224_224   ((hseIpsecSuite_t)0x0cf8U)
#define HSE_IPSEC_SUITE_AES_CTR_NULL                ((hseIpsecSuite_t)0x0d00U)
#define HSE_IPSEC_SUITE_AES_CTR_HMAC_MD5_96         ((hseIpsecSuite_t)0x0d01U)
#define HSE_IPSEC_SUITE_AES_CTR_HMAC_SHA1_96        ((hseIpsecSuite_t)0x0d02U)
#define HSE_IPSEC_SUITE_AES_CTR_AES_XCBC_MAC_96     ((hseIpsecSuite_t)0x0d05U)
#define HSE_IPSEC_SUITE_AES_CTR_HMAC_MD5_128        ((hseIpsecSuite_t)0x0d06U)
#define HSE_IPSEC_SUITE_AES_CTR_HMAC_SHA1_160       ((hseIpsecSuite_t)0x0d07U)
#define HSE_IPSEC_SUITE_AES_CTR_AES_CMAC_96         ((hseIpsecSuite_t)0x0d08U)
#define HSE_IPSEC_SUITE_AES_CTR_HMAC_SHA2_256_128   ((hseIpsecSuite_t)0x0d0cU)
#define HSE_IPSEC_SUITE_AES_CTR_HMAC_SHA2_384_192   ((hseIpsecSuite_t)0x0d0dU)
#define HSE_IPSEC_SUITE_AES_CTR_HMAC_SHA2_512_256   ((hseIpsecSuite_t)0x0d0eU)
#define HSE_IPSEC_SUITE_AES_CTR_HMAC_SHA2_224_96    ((hseIpsecSuite_t)0x0df2U)
#define HSE_IPSEC_SUITE_AES_CTR_HMAC_SHA2_224_112   ((hseIpsecSuite_t)0x0df4U)
#define HSE_IPSEC_SUITE_AES_CTR_HMAC_SHA2_224_224   ((hseIpsecSuite_t)0x0df8U)
#define HSE_IPSEC_SUITE_AES_CCM_8                   ((hseIpsecSuite_t)0x0e00U)
#define HSE_IPSEC_SUITE_AES_CCM_12                  ((hseIpsecSuite_t)0x0f00U)
#define HSE_IPSEC_SUITE_AES_CCM_16                  ((hseIpsecSuite_t)0x1000U)
#define HSE_IPSEC_SUITE_AES_GCM_8                   ((hseIpsecSuite_t)0x1200U)
#define HSE_IPSEC_SUITE_AES_GCM_12                  ((hseIpsecSuite_t)0x1300U)
#define HSE_IPSEC_SUITE_AES_GCM_16                  ((hseIpsecSuite_t)0x1400U)
#define HSE_IPSEC_SUITE_AES_NULL_GMAC               ((hseIpsecSuite_t)0x1500U)


/** @brief    HSE IPSEC security association type: transport/tunnel, 
 *            encapsulation/decapsulation */
typedef uint8_t hseIpsecType_t;
#define HSE_IPSEC_TYPE_TRANSPORT_ENCAP              ((hseIpsecType_t)0U)
#define HSE_IPSEC_TYPE_TRANSPORT_DECAP              ((hseIpsecType_t)1U)
#define HSE_IPSEC_TYPE_TUNNEL_ENCAP                 ((hseIpsecType_t)2U)
#define HSE_IPSEC_TYPE_TUNNEL_DECAP                 ((hseIpsecType_t)3U)

/** @brief    HSE IPSEC init option flags. Set the flag to enable the option.
 *            The options are set for the entire security association. */
typedef uint32_t hseIpsecOptionFlags_t;

/** @brief    Create an IPv6 SA. Without this flag and IPv4 SA will be created */
#define HSE_IPSEC_OPTION_IPV6                       ((hseIpsecOptionFlags_t)1U << 1U)

/** @brief    Use extended sequence numbers */
#define HSE_IPSEC_OPTION_EXTENDED_SEQNUM            ((hseIpsecOptionFlags_t)1U << 2U)

/** @brief    Use fully random IVs. Otherwise the IVs are chained from frame to frame */
#define HSE_IPSEC_OPTION_IV_SRC_RANDOM              ((hseIpsecOptionFlags_t)1U << 3U)

/** @brief    Decrement TTL on decapsulation */
#define HSE_IPSEC_OPTION_TTL_DECREMENT              ((hseIpsecOptionFlags_t)1U << 4U)

/** @brief    Rollover sequence numbers */
#define HSE_IPSEC_OPTION_SEQNUM_ROLLOVER            ((hseIpsecOptionFlags_t)1U << 5U)

/** @brief    Copy the DF bit*/
#define HSE_IPSEC_OPTION_DF_COPY                    ((hseIpsecOptionFlags_t)1U << 6U)

/** @brief    Copy the Diffserv bits */
#define HSE_IPSEC_OPTION_DIFFSERV_COPY              ((hseIpsecOptionFlags_t)1U << 7U)

/** @brief    Update the IP header checksum */
#define HSE_IPSEC_OPTION_CKSM_UPDATE                ((hseIpsecOptionFlags_t)1U << 8U)

/** @details    Set the IP header source, if any: context, frame or external.
 *            Use only one of the following 4 flags:
 *             - HSE_IPSEC_OPTION_IP_HDR_SRC_NONE
 *             - HSE_IPSEC_OPTION_IP_HDR_SRC_CONTEXT 
 *             - HSE_IPSEC_OPTION_IP_HDR_SRC_FRAME
 *             - HSE_IPSEC_OPTION_IP_HDR_SRC_EXTERNAL
 *
 *            HSE_IPSEC_OPTION_IP_HDR_SRC_NONE and HSE_IPSEC_OPTION_IP_HDR_SRC_EXTERNAL
 *            are only valid for tunnel mode
 */
#define HSE_IPSEC_OPTION_IP_HDR_SRC_NONE            ((hseIpsecOptionFlags_t)0U << 9U)
#define HSE_IPSEC_OPTION_IP_HDR_SRC_CONTEXT         ((hseIpsecOptionFlags_t)1U << 9U)
#define HSE_IPSEC_OPTION_IP_HDR_SRC_FRAME           ((hseIpsecOptionFlags_t)2U << 9U)
#define HSE_IPSEC_OPTION_IP_HDR_SRC_EXTERNAL        ((hseIpsecOptionFlags_t)3U << 9U)

/** @brief    Output only the PDU. Otherwise all the input fields will be copied to the output frame */
#define HSE_IPSEC_OPTION_OUTPUT_PDU_ONLY            ((hseIpsecOptionFlags_t)1U << 11U)

/** @brief    Adjust the frame length */
#define HSE_IPSEC_OPTION_OUTPUT_FRAME_LEN_ADJUST    ((hseIpsecOptionFlags_t)1U << 12U)

/** @brief    Override the DF bit */
#define HSE_IPSEC_OPTION_DF_OVERRIDE                ((hseIpsecOptionFlags_t)1U << 13U)

/** @brief    The value to override the DF bit with, if HSE_IPSEC_OPTION_DF_OVERRIDE is set */
#define HSE_IPSEC_OPTION_DF_OVERRIDE_VALUE_0        ((hseIpsecOptionFlags_t)0U << 14U)
#define HSE_IPSEC_OPTION_DF_OVERRIDE_VALUE_1        ((hseIpsecOptionFlags_t)1U << 14U)

/** @brief    Enable NAT */
#define HSE_IPSEC_OPTION_NAT                        ((hseIpsecOptionFlags_t)1U << 15U)

/** @brief    Update the UDP checksum, if NAT is on (HSE_IPSEC_OPTION_NAT is set)*/
#define HSE_IPSEC_OPTION_UDP_CHECKSUM               ((hseIpsecOptionFlags_t)1U << 16U)

/** @brief    Update the ether type */
#define HSE_IPSEC_OPTION_ETHER_TYPE_UPDATE          ((hseIpsecOptionFlags_t)1U << 17U)

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/**
* @brief          HSE IPSEC acceleration initialization service.
* @details        
*/
typedef struct
{
    /** @brief   INPUT: Specifies the context to use to store the IPSEC state of this SA.*/
    hseIpsecSaId_t              saId;

    /** @brief   INPUT: Security parameter index of this SA.*/
    uint32_t                    spi;

    /** @brief   INPUT: Initial sequence number.*/
    uint32_t                    seqNum;

    /** @brief   INPUT: Key handle for encryption/decryption.*/
    hseKeyHandle_t              cipherKeyHandle;

    /** @brief   INPUT: Key handle for tag generation/verification.*/
    hseKeyHandle_t              authKeyHandle;

    /** @brief   INPUT: IPSEC cipher suite used.*/
    hseIpsecSuite_t             ipsecSuite;

    /** @brief   INPUT: The IANA type of the next header.*/
    uint8_t                     nextHeader;

    /** @brief   INPUT: Offset of the next header.*/
    uint8_t                     nextHeaderOffset;

    /** @brief   INPUT: Offset of the outer IP header.*/
    uint8_t                     actualOuterIpHeaderOffset;

    uint8_t                     reserved1;

    /** @brief   INPUT: IP header length.*/
    uint16_t                    ipHeaderLen;

    /** @brief   INPUT: Pointer to the IP header for the output frame.*/
    HOST_ADDR                   pIpHeader;

    /** @brief   INPUT: Extended sequence number.*/
    uint32_t                    esn;

    /** @brief   INPUT: Pointer to the initialization vector.*/
    HOST_ADDR                   pIv;

    /** @details   INPUT: Salt used, where applicable, formated as a uint32_t
     *                  Salts less then 4 bytes must be placed in the least significant bytes
     *                  of the salt field
     */
    uint32_t                    salt;

    /** @brief   INPUT: CTR/CCM initial count.*/
    uint32_t                    initialCount;

    /** @brief   INPUT: CTR nonce.*/
    uint32_t                    nonce;

    /** @brief   INPUT: IPSEC type - Transport/Tunnel, Encap/Decap.*/
    hseIpsecType_t              ipsecType;

    /** @brief   INPUT: Anti-replay window size. 0 - disables anti-replay protection. 
     *                  Any other number is rounded up to the next power of two. Numbers
     *                  higher than 128 are rounded down to 128.*/
    uint8_t                     antiReplayWindowSize;

    uint8_t                    reserved2[2];
    /** @brief   INPUT: IPSEC type - Transport/Tunnel, Encap/Decap.*/
    hseIpsecOptionFlags_t       ipsecOptionFlags;
} hseIpsecInitSrv_t;

/**
* @brief          HSE IPSEC acceleration frame processing service.
*/
typedef struct
{
    /** @brief   INPUT: Specifies the context to use to store the IPSEC state of this SA.*/
    hseIpsecSaId_t              saId;

    /** @brief   INPUT: Input frame length.*/
    uint16_t                    inputFrameLength;

    /** @brief   INPUT: Whether to override SA params set at init, for this frame only.*/
    bool_t                      bOverrideInitParams;

    /** @brief  INPUT: Specify if pInputFrame/pOutputFrame are provided as hseScatterList_t list (the host address points to a hseScatterList_t list). 
     *                 Ignored if SGT is not supported. <br>
     *                 NOTE:
     *                 - If scatter option is selected (set), the length (e.g. inputFrameLength) shall specified the entire message length
     *                 (sum of all hseScatterList_t lengths).
     *                 - The number for SGT entries shall be less then HSE_MAX_NUM_OF_SGT_ENTRIES.<br>
    */
    hseSGTOption_t              sgtOption;

    /** @brief   INPUT: The location of the input frame.*/
    HOST_ADDR                   pInputFrame;

    /** @brief   INPUT: The capacity of the output frame.*/
    uint16_t                    maxOutputFrameLength;
    uint8_t                     reserved1[2];
    /** @brief   OUTPUT: The location of the output frame.*/
    HOST_ADDR                   pOutputFrame;

    /** @brief   INPUT: The override parameter values.
     *
     *  @note Not all parameters are relevant for all IPSEC types. The relevant params for each IPSEC type are:
     *       - HSE_IPSEC_TRANSPORT_ENCAP:  nextHeader, nextHeaderOffset, ipHeaderLen, bOverrideEcn, ecn
     *       - HSE_IPSEC_TRANSPORT_DECAP:  nextHeaderOffset, ipHeaderLen, bOverrideEcn, ecn
     *       - HSE_IPSEC_TUNNEL_ENCAP:     pIpHeader, nextHeader, ipHeaderLen, ipHeaderSource
     *       - HSE_IPSEC_TUNNEL_DECAP:     pIpHeader, ipHeaderLen, ipHeaderSource
     *  Not relevant parameters are ignored.
     *  
     *  @note Overriding is all or nothing. With the exception of ecn (which has a specific flag), 
     *  if overriding is enabled, and a parameter is relevant to the IPSEC type, it will be overridden
     *  with the value found here. */
    struct
    {
        /** @brief   INPUT: Override value for the IANA type of the next header.*/
        uint8_t                     nextHeader;

        /** @brief   INPUT: Override value for offset of the next header.*/
        uint8_t                     nextHeaderOffset;


        /** @brief   INPUT: Override value for the offset of the outer IP header.*/
        uint8_t                     actualOuterIpHeaderOffset;

        uint8_t                     reserved2[2];

        /** @brief   INPUT: Override value for IP header length.*/
        uint8_t                     ipHeaderLen;

        /** @brief   INPUT: Whether to override ECN.*/
        bool_t                      bOverrideEcn;

        /** @brief   INPUT: Override value for ECN (0 - 3).*/
        uint8_t                     ecn;

        /** @brief   INPUT: Override value for the IP header source.
         *  Only HSE_IPSEC_OPTION_IP_HDR_SRC_CONTEXT and HSE_IPSEC_OPTION_IP_HDR_SRC_FRAME are allowed here
         */
        hseIpsecOptionFlags_t    ipHeaderSource;

    } overrideParams;
} hseIpsecProcessSrv_t;

/**
* @brief          Get the current sequence number of the indicated SA.
*/
typedef struct
{
    /** @brief   INPUT: Specifies the context to use to store the IPSEC state of this SA.*/
    hseIpsecSaId_t              saId;

    /** @brief   INPUT: Whether we are managing the extended sequence number. 0 - regular SN, 1 - ESN */
    bool_t                      bExtendedSeqNum;

    uint8_t                     reserved[3];

    /** @brief   OUTPUT: Where to write the sequence number.*/
    HOST_ADDR                   pSeqNum;
} hseIpsecGetSeqNumSrv_t;

/**
* @brief          Set the current sequence number of the indicated SA.
*/
typedef struct
{
    /** @brief   INPUT: Specifies the context to use to store the IPSEC state of this SA.*/
    hseIpsecSaId_t              saId;

    /** @brief   INPUT: Whether we are managing the extended sequence number. 0 - regular SN, 1 - ESN */
    bool_t                      bExtendedSeqNum;

    uint8_t                     reserved[3];

    /** @brief   INPUT: The sequence number to set. */
    uint32_t                    seqNum;
} hseIpsecSetSeqNumSrv_t;

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

#endif /* HSE_SRV_IPSEC_H */

/** @} */
