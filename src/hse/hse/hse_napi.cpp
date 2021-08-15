#include <string.h>
#include <stdio.h>
#include "hse_napi.h"
#include "hse_interface.h"

//#define DEBUG


#define DECLARE_NAPI_UINT32(symbol)\
    InstanceValue(#symbol,Napi::Number::New(env, symbol))

#define DECLARE_NAPI_NAME_UINT32(name,symbol)\
    InstanceValue(name, Napi::Number::New(env, symbol))



Napi::FunctionReference HSE::constructor;


Napi::Object HSE::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);
    
  Napi::Function func =
      DefineClass(env,
                  "HSE",
                  {
                    DECLARE_NAPI_UINT32(HSE_APP_CORE0),
                    DECLARE_NAPI_UINT32(HSE_APP_CORE1),
                    DECLARE_NAPI_UINT32(HSE_CR_SANCTION_DIS_INDIV_KEYS),
                    DECLARE_NAPI_UINT32(HSE_CR_SANCTION_KEEP_CORE_IN_RESET),
                    DECLARE_NAPI_UINT32(HSE_CR_SANCTION_RESET_SOC),
                    DECLARE_NAPI_UINT32(HSE_CR_SANCTION_DIS_ALL_KEYS),
                    DECLARE_NAPI_UINT32(HSE_ACCESS_MODE_ONE_PASS),
                    DECLARE_NAPI_UINT32(HSE_ACCESS_MODE_START),
                    DECLARE_NAPI_UINT32(HSE_ACCESS_MODE_UPDATE),
                    DECLARE_NAPI_UINT32(HSE_ACCESS_MODE_FINISH),
#ifdef HSE_0_8_3
                    DECLARE_NAPI_UINT32(HSE_SMR_VERIF_PRE_BOOT_MASK),
                    DECLARE_NAPI_UINT32(HSE_SMR_VERIF_PRE_BOOT_ALT_MASK),
                    DECLARE_NAPI_UINT32(HSE_SMR_VERIF_POST_BOOT_MASK),
                    DECLARE_NAPI_UINT32(HSE_SMR_VERIF_RUN_TIME_MASK),
#endif
                    DECLARE_NAPI_UINT32(HSE_SMR_CFG_FLAG_INSTALL_AUTH),
                    DECLARE_NAPI_UINT32(HSE_HASH_ALGO_MD5),
                    DECLARE_NAPI_UINT32(HSE_HASH_ALGO_SHA_1),
                    DECLARE_NAPI_UINT32(HSE_HASH_ALGO_SHA2_224),
                    DECLARE_NAPI_UINT32(HSE_HASH_ALGO_SHA2_256),
                    DECLARE_NAPI_UINT32(HSE_HASH_ALGO_SHA2_384),
                    DECLARE_NAPI_UINT32(HSE_HASH_ALGO_SHA2_512),
                    DECLARE_NAPI_UINT32(HSE_HASH_ALGO_SHA2_512_224),
                    DECLARE_NAPI_UINT32(HSE_HASH_ALGO_SHA2_512_256),
                    DECLARE_NAPI_UINT32(HSE_HASH_ALGO_SHA3_224),
                    DECLARE_NAPI_UINT32(HSE_HASH_ALGO_SHA3_256),
                    DECLARE_NAPI_UINT32(HSE_HASH_ALGO_SHA3_384),
                    DECLARE_NAPI_UINT32(HSE_HASH_ALGO_SHA3_512),
                    DECLARE_NAPI_UINT32(HSE_HASH_ALGO_MP),
                    DECLARE_NAPI_UINT32(HSE_MU0_MASK),
                    DECLARE_NAPI_UINT32(HSE_MU1_MASK),
                    DECLARE_NAPI_UINT32(HSE_MU2_MASK),
                    DECLARE_NAPI_UINT32(HSE_MU3_MASK),
                    DECLARE_NAPI_UINT32(HSE_ALL_MU_MASK),
                    DECLARE_NAPI_UINT32(HSE_APP_CORE0),
                    DECLARE_NAPI_UINT32(HSE_APP_CORE1),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_0),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_1),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_2),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_3),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_4),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_5),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_6),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_7),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_8),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_9),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_10),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_11),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_12),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_13),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_14),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_15),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_16),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_17),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_18),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_19),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_20),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_21),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_22),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_23),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_24),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_25),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_26),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_27),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_28),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_29),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_30),
                    DECLARE_NAPI_UINT32(HSE_KF_SMR_31),
                    DECLARE_NAPI_UINT32(HSE_KEY_CATALOG_ID_ROM),
                    DECLARE_NAPI_UINT32(HSE_KEY_CATALOG_ID_NVM),
                    DECLARE_NAPI_UINT32(HSE_KEY_CATALOG_ID_RAM),
                    DECLARE_NAPI_UINT32(HSE_KEY_OWNER_ANY),
                    DECLARE_NAPI_UINT32(HSE_KEY_OWNER_CUST),
                    DECLARE_NAPI_UINT32(HSE_KEY_OWNER_OEM),
                    DECLARE_NAPI_UINT32(HSE_KEY_TYPE_SHE),
                    DECLARE_NAPI_UINT32(HSE_KEY_TYPE_AES),
                    DECLARE_NAPI_UINT32(HSE_KEY_TYPE_HMAC),
#ifdef HSE_0_8_3
                    DECLARE_NAPI_UINT32(HSE_KEY_TYPE_TDES),
#endif
                    DECLARE_NAPI_UINT32(HSE_KEY_TYPE_SHARED_SECRET),
                    DECLARE_NAPI_UINT32(HSE_KEY_TYPE_ECC_PAIR),
                    DECLARE_NAPI_UINT32(HSE_KEY_TYPE_ECC_PUB),
                    DECLARE_NAPI_UINT32(HSE_KEY_TYPE_ECC_PUB_EXT),
                    DECLARE_NAPI_UINT32(HSE_KEY_TYPE_RSA_PAIR),
                    DECLARE_NAPI_UINT32(HSE_KEY_TYPE_RSA_PUB),
                    DECLARE_NAPI_UINT32(HSE_KEY_TYPE_RSA_PUB_EXT),
                    DECLARE_NAPI_UINT32(HSE_KEY_TYPE_DH_PAIR),
                    DECLARE_NAPI_UINT32(HSE_KEY_TYPE_DH_PUB),
                    DECLARE_NAPI_UINT32(HSE_KF_USAGE_ENCRYPT),
                    DECLARE_NAPI_UINT32(HSE_KF_USAGE_DECRYPT),
                    DECLARE_NAPI_UINT32(HSE_KF_USAGE_SIGN),
                    DECLARE_NAPI_UINT32(HSE_KF_USAGE_VERIFY),
                    DECLARE_NAPI_UINT32(HSE_KF_USAGE_EXCHANGE),
                    DECLARE_NAPI_UINT32(HSE_KF_USAGE_DERIVE),
                    DECLARE_NAPI_UINT32(HSE_KF_USAGE_KEY_PROVISION),
                    DECLARE_NAPI_UINT32(HSE_KF_USAGE_AUTHORIZATION),
                    DECLARE_NAPI_UINT32(HSE_KF_ACCESS_WRITE_PROT),
                    DECLARE_NAPI_UINT32(HSE_KF_ACCESS_DEBUG_PROT),
                    DECLARE_NAPI_UINT32(HSE_KF_ACCESS_EXPORTABLE),
                    DECLARE_NAPI_UINT32(HSE_KF_USAGE_MASK),
                    DECLARE_NAPI_UINT32(HSE_KF_ACCESS_MASK),
                    DECLARE_NAPI_UINT32(HSE_ROM_KEY_AES256_KEY0),
                    DECLARE_NAPI_UINT32(HSE_ROM_KEY_AES256_KEY1),
                    DECLARE_NAPI_UINT32(HSE_ROM_KEY_RSA2048_PUB_KEY2),
                    DECLARE_NAPI_UINT32(HSE_ROM_KEY_ECC256_PUB_KEY3),
                    DECLARE_NAPI_UINT32(HSE_KEY_BITS_INVALID),
                    DECLARE_NAPI_UINT32(HSE_KEY_BITS_ZERO),
                    DECLARE_NAPI_UINT32(HSE_KEY128_BITS),
                    DECLARE_NAPI_UINT32(HSE_KEY160_BITS),
                    DECLARE_NAPI_UINT32(HSE_KEY192_BITS),
                    DECLARE_NAPI_UINT32(HSE_KEY224_BITS),
                    DECLARE_NAPI_UINT32(HSE_KEY240_BITS),
                    DECLARE_NAPI_UINT32(HSE_KEY256_BITS),
                    DECLARE_NAPI_UINT32(HSE_KEY320_BITS),
                    DECLARE_NAPI_UINT32(HSE_KEY384_BITS),
                    DECLARE_NAPI_UINT32(HSE_KEY512_BITS),
                    DECLARE_NAPI_UINT32(HSE_KEY521_BITS),
                    DECLARE_NAPI_UINT32(HSE_KEY638_BITS),
                    DECLARE_NAPI_UINT32(HSE_KEY1024_BITS),
                    DECLARE_NAPI_UINT32(HSE_KEY2048_BITS),
                    DECLARE_NAPI_UINT32(HSE_KEY3072_BITS),
                    DECLARE_NAPI_UINT32(HSE_KEY4096_BITS),
                    DECLARE_NAPI_UINT32(HSE_LC_CUST_DEL),
                    DECLARE_NAPI_UINT32(HSE_LC_OEM_PROD),
                    DECLARE_NAPI_UINT32(HSE_LC_IN_FIELD),
                    DECLARE_NAPI_UINT32(HSE_LC_PRE_FA),
#ifdef HSE_0_8_3
                    DECLARE_NAPI_UINT32(HSE_LC_FA),
#endif
                    DECLARE_NAPI_UINT32(HSE_LC_SIMULATED_OEM_PROD),
                    DECLARE_NAPI_UINT32(HSE_LC_SIMULATED_IN_FIELD),
                    DECLARE_NAPI_UINT32(HSE_IVT_NO_AUTH),
                    DECLARE_NAPI_UINT32(HSE_IVT_AUTH),
                    DECLARE_NAPI_UINT32(HSE_FIRC_NO_CONFIG),
                    DECLARE_NAPI_UINT32(HSE_FIRC_DIV_BY_1_CONFIG),
                    DECLARE_NAPI_UINT32(HSE_FIRC_DIV_BY_2_CONFIG),
                    DECLARE_NAPI_UINT32(HSE_FIRC_DIV_BY_16_CONFIG),
                    DECLARE_NAPI_UINT32(HSE_FW_VERSION_ATTR_ID),
                    DECLARE_NAPI_UINT32(HSE_CAPABILITIES_ATTR_ID),
                    DECLARE_NAPI_UINT32(HSE_SMR_CORE_BOOT_STATUS_ATTR_ID),
                    DECLARE_NAPI_UINT32(HSE_DEBUG_AUTH_MODE_ATTR_ID),
                    DECLARE_NAPI_UINT32(HSE_APP_DEBUG_KEY_ATTR_ID),
                    DECLARE_NAPI_UINT32(HSE_SECURE_LIFECYCLE_ATTR_ID),
                    DECLARE_NAPI_UINT32(HSE_ENABLE_BOOT_AUTH_ATTR_ID),
                    DECLARE_NAPI_UINT32(HSE_EXTEND_CUST_SECURITY_POLICY_ATTR_ID),
                    DECLARE_NAPI_UINT32(HSE_MU_CONFIG_ATTR_ID),
                    DECLARE_NAPI_UINT32(HSE_EXTEND_OEM_SECURITY_POLICY_ATTR_ID),
#ifdef HSE_0_8_3
                    DECLARE_NAPI_UINT32(HSE_TAMPER_ENABLE_ATTR_ID),
#endif
#ifdef HSE_0_10_0
                    DECLARE_NAPI_UINT32(HSE_PHYSICAL_TAMPER_ATTR_ID),
#endif
                    DECLARE_NAPI_UINT32(HSE_FIRC_DIVIDER_CONFIG_ATTR_ID),
                    DECLARE_NAPI_UINT32(HSE_DEBUG_AUTH_MODE_PW),
                    DECLARE_NAPI_UINT32(HSE_DEBUG_AUTH_MODE_CR),
                    DECLARE_NAPI_UINT32(HSE_EC_SEC_SECP256R1),
                    DECLARE_NAPI_UINT32(HSE_EC_SEC_SECP384R1),
                    DECLARE_NAPI_UINT32(HSE_EC_BRAINPOOL_BRAINPOOLP256R1),
                    DECLARE_NAPI_UINT32(HSE_EC_BRAINPOOL_BRAINPOOLP320R1),
                    DECLARE_NAPI_UINT32(HSE_EC_BRAINPOOL_BRAINPOOLP384R1),
                    DECLARE_NAPI_UINT32(HSE_EC_BRAINPOOL_BRAINPOOLP512R1),
                    DECLARE_NAPI_UINT32(HSE_EC_25519_ED25519),
                    DECLARE_NAPI_UINT32(HSE_EC_25519_CURVE25519),
                    DECLARE_NAPI_NAME_UINT32("ATTR_FW_VERSION_LEN",sizeof(hseAttrFwVersion_t)),
                    DECLARE_NAPI_NAME_UINT32("ATTR_SMR_CORE_STATUS_LEN",sizeof(hseAttrSmrCoreStatus_t)),
                    DECLARE_NAPI_NAME_UINT32("ATTR_MU_INST_CONFIG_LEN",sizeof(hseAttrMUInstanceConfig_t)),
                    DECLARE_NAPI_NAME_UINT32("ATTR_MU_CONFIG_LEN",sizeof(hseAttrMUConfig_t)),
#ifdef HSE_0_8_3
                    DECLARE_NAPI_NAME_UINT32("ATTR_CMU_INST_CONFIG_LEN",sizeof(hseAttrCMUInstanceConfig_t)),
                    DECLARE_NAPI_NAME_UINT32("ATTR_CMU_CONFIG_LEN",sizeof(hseAttrCMUConfig_t)),
#endif
                    DECLARE_NAPI_NAME_UINT32("ATTR_EXTEND_CUST_SECURITY_POLICY_LEN",sizeof(hseAttrExtendCustSecurityPolicy_t)),
                    DECLARE_NAPI_NAME_UINT32("ATTR_EXTEND_OEM_SECURITY_POLICY_LEN",sizeof(hseAttrExtendOemSecurityPolicy_t)),
                    DECLARE_NAPI_NAME_UINT32("ATTR_ADKP_LEN",sizeof(hseAttrApplDebugKey_t)),
                    DECLARE_NAPI_NAME_UINT32("ATTR_LC_LEN",sizeof(hseAttrSecureLifecycle_t)),
                    DECLARE_NAPI_NAME_UINT32("ATTR_BOOT_AUTH_LEN",sizeof(hseAttrConfigBootAuth_t)),
                    DECLARE_NAPI_NAME_UINT32("ATTR_LC_LEN",sizeof(hseAttrSecureLifecycle_t)),
                    DECLARE_NAPI_NAME_UINT32("ATTR_FIRC_LEN",sizeof(hseFircDivConfig_t)),
                    DECLARE_NAPI_NAME_UINT32("ATTR_DEBUG_MODE_LEN",sizeof(hseAttrDebugAuthMode_t)),
                    //DECLARE_NAPI_NAME_UINT32("ATTR_TAMPER_MODE_LEN",sizeof(hseAttrExternalTamperConfig_t)),
                    InstanceMethod("getVersion", &HSE::getVersion),
                    InstanceMethod("importSubSheKey", &HSE::importSheKey),
                    InstanceMethod("importKey", &HSE::importKey),
                    InstanceMethod("formatCatalog", &HSE::formatCatalog),
                    InstanceMethod("setAttr", &HSE::setAttr),
                    InstanceMethod("smrInstallWithData", &HSE::smrInstallWithData),
                    InstanceMethod("smrInstallWithoutData", &HSE::smrInstallWithoutData),
                    InstanceMethod("smrVerify", &HSE::smrVerify),
                    InstanceMethod("crInstall", &HSE::crInstall),
                    InstanceMethod("FWUpdate",&HSE::FWUpdate),
                    InstanceMethod("FWUpdate",&HSE::FWUpdateLegacy),
#ifdef HSE_0_10_0
                    InstanceMethod("SBAFUpdate",&HSE::SBAFUpdate),
#endif
                   });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("HSE", func);
  return exports;
}



static Napi::Buffer<uint8_t> hseAuthScheme(Napi::Env env,hseAuthScheme_t* s,Napi::Object o,uint32_t alloffset){

    std::string type=o.Get("type").ToString();
     
    if(type.compare("xcbc_mac")==0){
        s->macScheme.macAlgo=HSE_MAC_ALGO_XCBC_MAC;
        return Napi::Buffer<uint8_t>::New(env,0);
    }else if(type.compare("cmac-aes")==0){
        s->macScheme.macAlgo=HSE_MAC_ALGO_CMAC;
        s->macScheme.sch.cmac.cipherAlgo=HSE_CIPHER_ALGO_AES;
        return Napi::Buffer<uint8_t>::New(env,0);
    }
#ifdef HSE_0_8_3
    else if(type.compare("cmac-tdes")==0){
        s->macScheme.macAlgo=HSE_MAC_ALGO_CMAC;
        s->macScheme.sch.cmac.cipherAlgo=HSE_CIPHER_ALGO_3DES;
        return Napi::Buffer<uint8_t>::New(env,0);
    }
#endif
    else if(type.compare("gmac")==0){
        s->macScheme.macAlgo=HSE_MAC_ALGO_GMAC;
        Napi::Buffer<uint8_t> iv=o.Get("iv").As<Napi::Buffer<uint8_t>>();
        s->macScheme.sch.gmac.ivLength=iv.Length();
        s->macScheme.sch.gmac.pIV=(HOST_ADDR)alloffset;
        return Napi::Buffer<uint8_t>::Copy(env,iv.Data(),iv.Length());
    }else if(type.compare("hmac")==0){
        s->macScheme.macAlgo=HSE_MAC_ALGO_HMAC;
        s->macScheme.sch.hmac.hashAlgo=o.Get("hashAlgo").ToNumber().Uint32Value();
        return Napi::Buffer<uint8_t>::New(env,0);
    }else if(type.compare("rsa-pss")==0){
        s->sigScheme.signSch=HSE_SIGN_RSASSA_PSS;
        s->sigScheme.sch.rsaPss.hashAlgo=o.Get("hashAlgo").ToNumber().Uint32Value();
        s->sigScheme.sch.rsaPss.saltLength=o.Get("saltLength").ToNumber().Uint32Value();
        return Napi::Buffer<uint8_t>::New(env,0);
    }else if(type.compare("rsa-pkcs")==0){
        s->sigScheme.signSch=HSE_SIGN_RSASSA_PKCS1_V15;
        s->sigScheme.sch.rsaPss.hashAlgo=o.Get("hashAlgo").ToNumber().Uint32Value();
        return Napi::Buffer<uint8_t>::New(env,0);
    }else if(type.compare("ecdsa")==0){
        s->sigScheme.signSch=HSE_SIGN_ECDSA;
        s->sigScheme.sch.ecdsa.hashAlgo=o.Get("hashAlgo").ToNumber().Uint32Value();
        return Napi::Buffer<uint8_t>::New(env,0);
    }else if(type.compare("eddsa")==0){
        s->sigScheme.signSch=HSE_SIGN_EDDSA;
        s->sigScheme.sch.eddsa.bHashEddsa=o.Get("bHashEddsa").ToBoolean().Value();
        Napi::Buffer<uint8_t> context=o.Get("context").As<Napi::Buffer<uint8_t>>();
        s->sigScheme.sch.eddsa.contextLength=context.Length();
        s->sigScheme.sch.eddsa.pContext=(HOST_ADDR)alloffset;
        return Napi::Buffer<uint8_t>::Copy(env,context.Data(),context.Length());
    }

    return Napi::Buffer<uint8_t>::New(env,0);
}
#if 0
static hseEccCurveId_t getCurveId(int id){
    hseEccCurveId_t eccCurveId;

    switch(id){
        case NID_secp384r1:
            eccCurveId=HSE_EC_SEC_SECP384R1;
            break;
        case NID_secp521r1:
            eccCurveId=HSE_EC_SEC_SECP521R1;
            break;
        case NID_brainpoolP256r1:
            eccCurveId=HSE_EC_BRAINPOOL_BRAINPOOLP256R1;
            break;
        case NID_brainpoolP384r1:
            eccCurveId=HSE_EC_BRAINPOOL_BRAINPOOLP384R1;
            break;
        case NID_brainpoolP320r1:
            eccCurveId=HSE_EC_BRAINPOOL_BRAINPOOLP320R1;
            break;
        case NID_brainpoolP512r1:
            eccCurveId=HSE_EC_BRAINPOOL_BRAINPOOLP512R1;
            break;
        case NID_ED25519:
            eccCurveId=HSE_EC_25519_ED25519;
            break;
        default:
            eccCurveId=HSE_EC_CURVE_NONE;
            break;
    }
    return eccCurveId;
}
#endif

HSE::HSE(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<HSE>(info) {
    return;
}

Napi::Value HSE::getVersion(const Napi::CallbackInfo& info){
    return Napi::String::New(info.Env(),"OpenSSL 1.1.1j  16 Feb 2021");
}

Napi::Value HSE::importKey(const Napi::CallbackInfo& info){
    hseSrvDescriptor_t service;
    hseKeyInfo_t keyInfo;

    memset((void*)&service,0,sizeof(hseSrvDescriptor_t));
    memset((void*)&keyInfo,0,sizeof(hseKeyInfo_t));
    Napi::Object ret=Napi::Object::New(info.Env());

    uint32_t offset=info[0].As<Napi::Number>().Uint32Value();
    hseKeyHandle_t keyHandle=info[1].As<Napi::Number>().Uint32Value();
    Napi::Object i=info[2].As<Napi::Object>();
    
    Napi::Buffer<uint8_t> key0=info[3].As<Napi::Buffer<uint8_t>>();
    Napi::Buffer<uint8_t> key1=info[4].As<Napi::Buffer<uint8_t>>();
    Napi::Buffer<uint8_t> key2=info[5].As<Napi::Buffer<uint8_t>>();
    
    //printf("key0:%d,key1:%d,key2:%d\r\n",key0.Length(),key1.Length(),key2.Length());
    keyInfo.keyFlags=i.Get("keyFlags").ToNumber().Uint32Value();
    keyInfo.keyCounter=i.Get("keyCounter").ToNumber().Uint32Value();
    keyInfo.smrFlags=i.Get("smrFlags").ToNumber().Uint32Value();
    keyInfo.keyType=i.Get("keyType").ToNumber().Uint32Value();
    keyInfo.keyBitLen=i.Get("keyBitLen").ToNumber().Uint32Value();
    if(keyInfo.keyType==HSE_KEY_TYPE_ECC_PAIR||keyInfo.keyType==HSE_KEY_TYPE_ECC_PUB||keyInfo.keyType==HSE_KEY_TYPE_ECC_PUB_EXT){
        keyInfo.specific.eccCurveId=i.Get("curveId").ToNumber().Uint32Value();
    }
    if(keyInfo.keyType==HSE_KEY_TYPE_RSA_PAIR||keyInfo.keyType==HSE_KEY_TYPE_RSA_PUB||keyInfo.keyType==HSE_KEY_TYPE_RSA_PUB_EXT){
        keyInfo.specific.pubExponentSize=key1.Length();
    }
    
    
    
    
    Napi::Buffer<uint8_t> payload=Napi::Buffer<uint8_t>::New(info.Env(),sizeof(hseSrvDescriptor_t)+sizeof(hseKeyInfo_t)+key0.Length()+key1.Length()+key2.Length());
    uint8_t* data=payload.Data();
    service.srvId=HSE_SRV_ID_IMPORT_KEY;
    service.hseSrv.importKeyReq.cipher.cipherKeyHandle= HSE_INVALID_KEY_HANDLE;
    service.hseSrv.importKeyReq.keyContainer.authKeyHandle= HSE_INVALID_KEY_HANDLE;
    service.hseSrv.importKeyReq.pKeyInfo=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t));
    service.hseSrv.importKeyReq.keyLen[0]=key0.Length();
    service.hseSrv.importKeyReq.pKey[0]=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseKeyInfo_t));
    service.hseSrv.importKeyReq.keyLen[1]=key1.Length();
    service.hseSrv.importKeyReq.pKey[1]=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseKeyInfo_t)+key0.Length());
    service.hseSrv.importKeyReq.keyLen[2]=key2.Length();
    service.hseSrv.importKeyReq.pKey[2]=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseKeyInfo_t)+key0.Length()+key1.Length());
    service.hseSrv.importKeyReq.targetKeyHandle=(hseKeyHandle_t)keyHandle;
    memcpy(data,&service,sizeof(hseSrvDescriptor_t));
    data+=sizeof(hseSrvDescriptor_t);
    memcpy(data,&keyInfo,sizeof(hseKeyInfo_t));
    data+=sizeof(hseKeyInfo_t);
    memcpy(data,key0.Data(),key0.Length());
    data+=key0.Length();
    memcpy(data,key1.Data(),key1.Length());
    data+=key1.Length();
    memcpy(data,key2.Data(),key2.Length());
    ret.Set("err",0);
    ret.Set("data",payload);
    ret.Set("msg","successful");
    return ret;
}
Napi::Value HSE::formatCatalog(const Napi::CallbackInfo& info){
    hseSrvDescriptor_t service;
    memset((void*)&service,0,sizeof(hseSrvDescriptor_t));
    Napi::Object ret=Napi::Object::New(info.Env());
    uint32_t offset=info[0].As<Napi::Number>().Uint32Value();
    Napi::Array nvm= info[1].As<Napi::Array>();
    Napi::Array ram= info[2].As<Napi::Array>();

    uint32_t nvmLen= nvm.Length();
    uint32_t ramLen= ram.Length();
    
    hseKeyGroupCfgEntry_t* nvmCat = new hseKeyGroupCfgEntry_t[nvmLen+1];
    hseKeyGroupCfgEntry_t* ramCat = new hseKeyGroupCfgEntry_t[ramLen+1];

    for(uint32_t i=0;i<nvmLen;i++){
        Napi::Value v=nvm[i];
        nvmCat[i].groupOwner=v.ToObject().Get("groupOwner").ToNumber().Uint32Value();
        nvmCat[i].keyType=v.ToObject().Get("keyType").ToNumber().Uint32Value();
        nvmCat[i].maxKeyBitLen=v.ToObject().Get("maxKeyBitLen").ToNumber().Uint32Value();
        nvmCat[i].muMask=v.ToObject().Get("muMask").ToNumber().Uint32Value();
        nvmCat[i].numOfKeySlots=v.ToObject().Get("numOfKeySlots").ToNumber().Uint32Value();
        nvmCat[i].hseReserved[0]=0;
        nvmCat[i].hseReserved[1]=0;
    } 
    nvmCat[nvmLen].groupOwner=0;
    nvmCat[nvmLen].keyType=0;
    nvmCat[nvmLen].maxKeyBitLen=0;
    nvmCat[nvmLen].muMask=0;
    nvmCat[nvmLen].numOfKeySlots=0;
    nvmCat[nvmLen].hseReserved[0]=0;
    nvmCat[nvmLen].hseReserved[1]=0;
    for(uint32_t i=0;i<ramLen;i++){
        Napi::Value v=ram[i];
        ramCat[i].groupOwner=v.ToObject().Get("groupOwner").ToNumber().Uint32Value();
        ramCat[i].keyType=v.ToObject().Get("keyType").ToNumber().Uint32Value();
        ramCat[i].maxKeyBitLen=v.ToObject().Get("maxKeyBitLen").ToNumber().Uint32Value();
        ramCat[i].muMask=v.ToObject().Get("muMask").ToNumber().Uint32Value();
        ramCat[i].numOfKeySlots=v.ToObject().Get("numOfKeySlots").ToNumber().Uint32Value();
        ramCat[i].hseReserved[0]=0;
        ramCat[i].hseReserved[1]=0;
    } 
    ramCat[ramLen].groupOwner=0;
    ramCat[ramLen].keyType=0;
    ramCat[ramLen].maxKeyBitLen=0;
    ramCat[ramLen].muMask=0;
    ramCat[ramLen].numOfKeySlots=0;
    ramCat[ramLen].hseReserved[0]=0;
    ramCat[ramLen].hseReserved[1]=0;

    Napi::Buffer<uint8_t> payload=Napi::Buffer<uint8_t>::New(info.Env(),sizeof(hseSrvDescriptor_t)+sizeof(hseKeyGroupCfgEntry_t)*(nvmLen+1)+sizeof(hseKeyGroupCfgEntry_t)*(ramLen+1));
    uint8_t* data=payload.Data();
    service.srvId=HSE_SRV_ID_FORMAT_KEY_CATALOGS;
    service.hseSrv.formatKeyCatalogsReq.pNvmKeyCatalogCfg= (HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t));
    service.hseSrv.formatKeyCatalogsReq.pRamKeyCatalogCfg= (HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseKeyGroupCfgEntry_t)*(nvmLen+1));
    memcpy(data,&service,sizeof(hseSrvDescriptor_t));
    data+=sizeof(hseSrvDescriptor_t);
    memcpy(data,nvmCat,sizeof(hseKeyGroupCfgEntry_t)*(nvmLen+1));
    data+=sizeof(hseKeyGroupCfgEntry_t)*(nvmLen+1);
    memcpy(data,ramCat,sizeof(hseKeyGroupCfgEntry_t)*(ramLen+1));
    delete nvmCat;
    delete ramCat;
    
    ret.Set("err",0);
    ret.Set("data",payload);
    ret.Set("msg","successful");
    
    return ret;
}

Napi::Value HSE::importSheKey(const Napi::CallbackInfo& info){
    hseSrvDescriptor_t service;
    hseKeyInfo_t keyInfo;

    memset((void*)&service,0,sizeof(hseSrvDescriptor_t));
    memset((void*)&keyInfo,0,sizeof(hseKeyInfo_t));
    Napi::Object ret=Napi::Object::New(info.Env());

    uint32_t offset=info[0].As<Napi::Number>().Uint32Value();
    hseKeyGroupIdx_t sheGroupIndex=info[1].As<Napi::Number>().Uint32Value();
    Napi::Buffer<uint8_t> m1=info[2].As<Napi::Buffer<uint8_t>>();
    Napi::Buffer<uint8_t> m2=info[3].As<Napi::Buffer<uint8_t>>();
    Napi::Buffer<uint8_t> m3=info[4].As<Napi::Buffer<uint8_t>>();
    
  
    Napi::Buffer<uint8_t> payload=Napi::Buffer<uint8_t>::New(info.Env(),sizeof(hseSrvDescriptor_t)+m1.Length()+m2.Length()+m3.Length());
    uint8_t* data=payload.Data();
    service.srvId=HSE_SRV_ID_SHE_LOAD_KEY;
    service.hseSrv.sheLoadKeyReq.sheGroupIndex=sheGroupIndex;
    service.hseSrv.sheLoadKeyReq.pM1=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t));
    service.hseSrv.sheLoadKeyReq.pM2=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+m1.Length());
    service.hseSrv.sheLoadKeyReq.pM3=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+m1.Length()+m2.Length());
    memcpy(data,&service,sizeof(hseSrvDescriptor_t));
    data+=sizeof(hseSrvDescriptor_t);
    memcpy(data,m1.Data(),m1.Length());
    data+=m1.Length();
    memcpy(data,m2.Data(),m2.Length());
    data+=m2.Length();
    memcpy(data,m3.Data(),m3.Length());
    
    ret.Set("err",0);
    ret.Set("data",payload);
    ret.Set("msg","successful");
    
    return ret;
}

Napi::Value HSE::setAttr(const Napi::CallbackInfo& info){
    hseSrvDescriptor_t service;
    hseKeyInfo_t keyInfo;

    memset((void*)&service,0,sizeof(hseSrvDescriptor_t));
    memset((void*)&keyInfo,0,sizeof(hseKeyInfo_t));
    Napi::Object ret=Napi::Object::New(info.Env());

    uint32_t offset=info[0].As<Napi::Number>().Uint32Value();
    hseAttrId_t attrId=info[1].As<Napi::Number>().Uint32Value();

    Napi::Buffer<uint8_t> attr=info[2].As<Napi::Buffer<uint8_t>>();
    
  
    Napi::Buffer<uint8_t> payload=Napi::Buffer<uint8_t>::New(info.Env(),sizeof(hseSrvDescriptor_t)+attr.Length());
    uint8_t* data=payload.Data();
    service.srvId=HSE_SRV_ID_SET_ATTR;
    service.hseSrv.setAttrReq.attrId=attrId;
    service.hseSrv.setAttrReq.attrLen=attr.Length();
    service.hseSrv.setAttrReq.pAttr=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t));
    memcpy(data,&service,sizeof(hseSrvDescriptor_t));
    data+=sizeof(hseSrvDescriptor_t);
    memcpy(data,attr.Data(),attr.Length());
    
    ret.Set("err",0);
    ret.Set("data",payload);
    ret.Set("msg","successful");
    
    return ret;
}

/* data has store in flash, just give point*/
Napi::Value HSE::smrInstallWithoutData(const Napi::CallbackInfo& info){
    hseSrvDescriptor_t service;
    hseSmrEntry_t smr;

    memset((void*)&service,0,sizeof(hseSrvDescriptor_t));
    memset((void*)&smr,0,sizeof(hseSmrEntry_t));
    Napi::Object ret=Napi::Object::New(info.Env());

    uint32_t offset=info[0].As<Napi::Number>().Uint32Value();
    Napi::Object smrEntry=info[1].As<Napi::Object>();
    Napi::Object genInfo=info[2].As<Napi::Object>();
    Napi::Array tagInfo=info[3].As<Napi::Array>();
    uint32_t tagLen[2];
    if(tagInfo.Length()!=2){
        ret.Set("err",-1);
        ret.Set("data",Napi::Buffer<uint8_t>::New(info.Env(),0));
        ret.Set("msg","tagInfo length must equal 2");
        return ret;
    }
    service.srvId=HSE_SRV_ID_SMR_ENTRY_INSTALL;
    service.hseSrv.smrEntryInstallReq.accessMode=genInfo.Get("accessMode").ToNumber().Uint32Value();
#ifdef HSE_0_8_3
    if(genInfo.Has("streamId")){
        service.hseSrv.smrEntryInstallReq.streamId=genInfo.Get("streamId").ToNumber().Uint32Value();
    }
#endif
    service.hseSrv.smrEntryInstallReq.entryIndex=genInfo.Get("entryIndex").ToNumber().Uint32Value();
    service.hseSrv.smrEntryInstallReq.pSmrData=genInfo.Get("pSmrData").ToNumber().Uint32Value();
    service.hseSrv.smrEntryInstallReq.smrDataLength=genInfo.Get("smrDataLength").ToNumber().Uint32Value();
    Napi::Buffer<uint8_t> tagBuf[2];
    uint8_t tLen=0;
#ifdef HSE_0_8_3
    tLen=8;
#endif
#ifdef HSE_0_10_0
    tLen=0;
#endif
    for(int i=0;i<2;i++){
        Napi::Value v=tagInfo[i];
        tagBuf[i]=v.As<Napi::Buffer<uint8_t>>();
        tagLen[i]=tagBuf[i].Length();
#ifdef HSE_0_8_3
        service.hseSrv.smrEntryInstallReq.pAuthTagLength[i]=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseSmrEntry_t)+i*4);
#endif
#ifdef HSE_0_10_0
        service.hseSrv.smrEntryInstallReq.authTagLength[i]=tagLen[i];
#endif
    }
   
    //address assign
    service.hseSrv.smrEntryInstallReq.pAuthTag[0]=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseSmrEntry_t)+tLen);
    service.hseSrv.smrEntryInstallReq.pAuthTag[1]=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseSmrEntry_t)+tLen+tagLen[0]);
    service.hseSrv.smrEntryInstallReq.pSmrEntry=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t));

    //smr
    smr.pSmrSrc=(HOST_ADDR)smrEntry.Get("pSmrSrc").ToNumber().Uint32Value();
    smr.pSmrDest=(HOST_ADDR)NULL;
    smr.smrSize=(HOST_ADDR)smrEntry.Get("smrSize").ToNumber().Uint32Value();
    smr.configFlags=(HOST_ADDR)smrEntry.Get("configFlags").ToNumber().Uint32Value();
    smr.checkPeriod=smrEntry.Get("checkPeriod").ToNumber().Uint32Value();
#ifdef HSE_0_8_3
    smr.verifMethod=(HOST_ADDR)smrEntry.Get("verifMethod").ToNumber().Uint32Value();
    smr.keyHandle=smrEntry.Get("keyHandle").ToNumber().Uint32Value();
#endif
    Napi::Array initTag = Napi::Array::Array(info.Env(),smrEntry.Get("pInstAuthTag"));
    Napi::Object scheme =  smrEntry.Get("authScheme").ToObject();
    for(int i=0;i<2;i++){
        Napi::Value v=initTag[i];
        smr.pInstAuthTag[i]=v.ToNumber().Uint32Value();
    }
    Napi::Buffer<uint8_t> extBuf=hseAuthScheme(info.Env(),&smr.authScheme,scheme,offset+sizeof(hseSrvDescriptor_t)+sizeof(hseSmrEntry_t)+tLen+tagLen[0]+tagLen[1]);
    Napi::Buffer<uint8_t> payload=Napi::Buffer<uint8_t>::New(info.Env(),sizeof(hseSrvDescriptor_t)+sizeof(hseSmrEntry_t)+tLen+tagLen[0]+tagLen[1]+extBuf.Length());
    //*real copy
    uint8_t* data=payload.Data();
    memcpy(data,&service,sizeof(hseSrvDescriptor_t));
    data+=sizeof(hseSrvDescriptor_t);
    memcpy(data,&smr,sizeof(hseSmrEntry_t));
    data+=sizeof(hseSmrEntry_t);
#ifdef HSE_0_8_3
    memcpy(data,tagLen,8);
    data+=8;
#endif
    memcpy(data,tagBuf[0].Data(),tagLen[0]);
    data+=tagLen[0];
    memcpy(data,tagBuf[1].Data(),tagLen[1]);
    data+=tagLen[1];
    if(extBuf.Length()>0){
        memcpy(data,extBuf.Data(),extBuf.Length());
    }
    
    ret.Set("err",0);
    ret.Set("data",payload);
    ret.Set("msg","successful");
    return ret;
}

/* data has store in flash, just give point*/
Napi::Value HSE::smrInstallWithData(const Napi::CallbackInfo& info){
    hseSrvDescriptor_t service;
    hseSmrEntry_t smr;

    memset((void*)&service,0,sizeof(hseSrvDescriptor_t));
    memset((void*)&smr,0,sizeof(hseSmrEntry_t));
    Napi::Object ret=Napi::Object::New(info.Env());

    uint32_t offset=info[0].As<Napi::Number>().Uint32Value();
    Napi::Object smrEntry=info[1].As<Napi::Object>();
    Napi::Object genInfo=info[2].As<Napi::Object>();
    Napi::Array tagInfo=info[3].As<Napi::Array>();
    uint32_t tagLen[2];
    if(tagInfo.Length()!=2){
        ret.Set("err",-1);
        ret.Set("data",Napi::Buffer<uint8_t>::New(info.Env(),0));
        ret.Set("msg","tagInfo length must equal 2");
        return ret;
    }
    service.srvId=HSE_SRV_ID_SMR_ENTRY_INSTALL;
    service.hseSrv.smrEntryInstallReq.accessMode=genInfo.Get("accessMode").ToNumber().Uint32Value();
#ifdef HSE_0_8_3
    if(genInfo.Has("streamId")){
        service.hseSrv.smrEntryInstallReq.streamId=genInfo.Get("streamId").ToNumber().Uint32Value();
    }
#endif
    service.hseSrv.smrEntryInstallReq.entryIndex=genInfo.Get("entryIndex").ToNumber().Uint32Value();
    Napi::Buffer<uint8_t> smrData=genInfo.Get("smrData").As<Napi::Buffer<uint8_t>>();
    service.hseSrv.smrEntryInstallReq.smrDataLength=smrData.Length();
    Napi::Buffer<uint8_t> tagBuf[2];
    uint8_t tLen=0;
#ifdef HSE_0_8_3
    tLen=8;
#endif
#ifdef HSE_0_10_0
    tLen=0;
#endif
    for(int i=0;i<2;i++){
        Napi::Value v=tagInfo[i];
        tagBuf[i]=v.As<Napi::Buffer<uint8_t>>();
        tagLen[i]=tagBuf[i].Length();
#ifdef HSE_0_8_3
        service.hseSrv.smrEntryInstallReq.pAuthTagLength[i]=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseSmrEntry_t)+i*4);
#endif
#ifdef HSE_0_10_0
        service.hseSrv.smrEntryInstallReq.authTagLength[i]=tagLen[i];
#endif
    }
   
    //address assign
    service.hseSrv.smrEntryInstallReq.pAuthTag[0]=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseSmrEntry_t)+tLen);
    service.hseSrv.smrEntryInstallReq.pAuthTag[1]=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseSmrEntry_t)+tLen+tagLen[0]);
    service.hseSrv.smrEntryInstallReq.pSmrEntry=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t));
    
    service.hseSrv.smrEntryInstallReq.pSmrData=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseSmrEntry_t)+tLen+tagLen[0]+tagLen[1]);
    //smr
    smr.pSmrSrc=(HOST_ADDR)smrEntry.Get("pSmrSrc").ToNumber().Uint32Value();
    smr.pSmrDest=(HOST_ADDR)NULL;
    smr.smrSize=(HOST_ADDR)smrEntry.Get("smrSize").ToNumber().Uint32Value();
    smr.configFlags=(HOST_ADDR)smrEntry.Get("configFlags").ToNumber().Uint32Value();
    smr.checkPeriod=smrEntry.Get("checkPeriod").ToNumber().Uint32Value();
#ifdef HSE_0_8_3
    smr.verifMethod=(HOST_ADDR)smrEntry.Get("verifMethod").ToNumber().Uint32Value();
    smr.keyHandle=smrEntry.Get("keyHandle").ToNumber().Uint32Value();
#endif
    Napi::Array initTag = Napi::Array::Array(info.Env(),smrEntry.Get("pInstAuthTag"));
    Napi::Object scheme =  smrEntry.Get("authScheme").ToObject();
    for(int i=0;i<2;i++){
        Napi::Value v=initTag[i];
        smr.pInstAuthTag[i]=v.ToNumber().Uint32Value();
    }
    Napi::Buffer<uint8_t> extBuf=hseAuthScheme(info.Env(),&smr.authScheme,scheme,offset+sizeof(hseSrvDescriptor_t)+sizeof(hseSmrEntry_t)+tLen+tagLen[0]+tagLen[1]+smrData.Length());
    Napi::Buffer<uint8_t> payload=Napi::Buffer<uint8_t>::New(info.Env(),sizeof(hseSrvDescriptor_t)+sizeof(hseSmrEntry_t)+tLen+tagLen[0]+tagLen[1]+smrData.Length()+extBuf.Length());
    //*real copy
    uint8_t* data=payload.Data();
    memcpy(data,&service,sizeof(hseSrvDescriptor_t));
    data+=sizeof(hseSrvDescriptor_t);
    memcpy(data,&smr,sizeof(hseSmrEntry_t));
    data+=sizeof(hseSmrEntry_t);
    memcpy(data,tagLen,8);
    data+=8;
    memcpy(data,tagBuf[0].Data(),tagLen[0]);
    data+=tagLen[0];
    memcpy(data,tagBuf[1].Data(),tagLen[1]);
    data+=tagLen[1];
    memcpy(data,smrData.Data(),smrData.Length());
    data+=smrData.Length();
    if(extBuf.Length()>0){
        memcpy(data,extBuf.Data(),extBuf.Length());
    }
    
    ret.Set("err",0);
    ret.Set("data",payload);
    ret.Set("msg","successful");
    return ret;
}
/* data has store in flash, just give point*/
Napi::Value HSE::crInstall(const Napi::CallbackInfo& info){
    hseSrvDescriptor_t service;
    hseCrEntry_t cr;

    memset((void*)&service,0,sizeof(hseSrvDescriptor_t));
    memset((void*)&cr,0,sizeof(hseCrEntry_t));
    Napi::Object ret=Napi::Object::New(info.Env());

    uint32_t offset=info[0].As<Napi::Number>().Uint32Value();
    uint32_t index=info[1].As<Napi::Number>().Uint32Value();
    Napi::Object crEntry=info[2].As<Napi::Object>();
    
    
    service.srvId=HSE_SRV_ID_CORE_RESET_ENTRY_INSTALL;
    
    cr.coreId=crEntry.Get("coreId").ToNumber().Uint32Value();
    cr.crSanction=crEntry.Get("crSanction").ToNumber().Uint32Value();
#ifdef HSE_0_8_3    
    cr.smrVerifMap=crEntry.Get("smrVerifMap").ToNumber().Uint32Value();
    cr.altSmrVerifMap=crEntry.Get("altSmrVerifMap").ToNumber().Uint32Value();
#endif
    cr.pPassReset=crEntry.Get("pPassReset").ToNumber().Uint32Value();
    cr.pAltReset=crEntry.Get("pAltReset").ToNumber().Uint32Value();
    
    //address assign
    service.hseSrv.crEntryInstallReq.crEntryIndex=index;
    service.hseSrv.crEntryInstallReq.pCrEntry=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t));
    Napi::Buffer<uint8_t> payload=Napi::Buffer<uint8_t>::New(info.Env(),sizeof(hseSrvDescriptor_t)+sizeof(hseCrEntry_t));
    //*real copy
    uint8_t* data=payload.Data();
    memcpy(data,&service,sizeof(hseSrvDescriptor_t));
    data+=sizeof(hseSrvDescriptor_t);
    memcpy(data,&cr,sizeof(hseCrEntry_t));
    ret.Set("err",0);
    ret.Set("data",payload);
    ret.Set("msg","successful");
    return ret;
}

Napi::Value HSE::smrVerify(const Napi::CallbackInfo& info){
    hseSrvDescriptor_t service;
    memset((void*)&service,0,sizeof(hseSrvDescriptor_t));
    Napi::Object ret=Napi::Object::New(info.Env());

    uint32_t offset=info[0].As<Napi::Number>().Uint32Value();
    uint32_t index=info[1].As<Napi::Number>().Uint32Value();

    service.srvId=HSE_SRV_ID_SMR_VERIFY;
    //address assign
    service.hseSrv.smrVerifyReq.entryIndex=index;
    Napi::Buffer<uint8_t> payload=Napi::Buffer<uint8_t>::New(info.Env(),sizeof(hseSrvDescriptor_t));
    //*real copy
    uint8_t* data=payload.Data();
    memcpy(data,&service,sizeof(hseSrvDescriptor_t));
    ret.Set("err",0);
    ret.Set("data",payload);
    ret.Set("msg","successful");
    return ret;
}

Napi::Value HSE::FWUpdateLegacy(const Napi::CallbackInfo& info){
    hseSrvDescriptor_t service;
    memset((void*)&service,0,sizeof(hseSrvDescriptor_t));
    Napi::Object ret=Napi::Object::New(info.Env());

    uint32_t offset=info[0].As<Napi::Number>().Uint32Value();
    uint32_t addr=info[1].As<Napi::Number>().Uint32Value();

    service.srvId=HSE_SRV_ID_FIRMWARE_UPDATE;
    //address assign
    service.hseSrv.firmwareUpdateLegacyReq.pInFwFile=(HOST_ADDR)addr;
    Napi::Buffer<uint8_t> payload=Napi::Buffer<uint8_t>::New(info.Env(),sizeof(hseSrvDescriptor_t));
    //*real copy
    uint8_t* data=payload.Data();
    memcpy(data,&service,sizeof(hseSrvDescriptor_t));
    ret.Set("err",0);
    ret.Set("data",payload);
    ret.Set("msg","successful");
    return ret;
}


Napi::Value HSE::FWUpdate(const Napi::CallbackInfo& info){
    hseSrvDescriptor_t service;
    memset((void*)&service,0,sizeof(hseSrvDescriptor_t));
    Napi::Object ret=Napi::Object::New(info.Env());

    uint32_t offset=info[0].As<Napi::Number>().Uint32Value();
    uint32_t addr=info[1].As<Napi::Number>().Uint32Value();

    service.srvId=HSE_SRV_ID_FIRMWARE_UPDATE;
    //address assign
    service.hseSrv.firmwareUpdateReq.accessMode=HSE_ACCESS_MODE_ONE_PASS;
    service.hseSrv.firmwareUpdateReq.streamLength=0;
    service.hseSrv.firmwareUpdateReq.pInFwFile=(HOST_ADDR)addr;
    Napi::Buffer<uint8_t> payload=Napi::Buffer<uint8_t>::New(info.Env(),sizeof(hseSrvDescriptor_t));
    //*real copy
    uint8_t* data=payload.Data();
    memcpy(data,&service,sizeof(hseSrvDescriptor_t));
    ret.Set("err",0);
    ret.Set("data",payload);
    ret.Set("msg","successful");
    return ret;
}

Napi::Value HSE::SBAFUpdate(const Napi::CallbackInfo& info){
    hseSrvDescriptor_t service;
    memset((void*)&service,0,sizeof(hseSrvDescriptor_t));
    Napi::Object ret=Napi::Object::New(info.Env());

    uint32_t offset=info[0].As<Napi::Number>().Uint32Value();
    uint32_t addr=info[1].As<Napi::Number>().Uint32Value();

    service.srvId=HSE_SRV_ID_SBAF_UPDATE;
    service.hseSrv.sbafUpdateReq.pInFwFile=(HOST_ADDR)addr;
    service.hseSrv.sbafUpdateReq.accessMode=HSE_ACCESS_MODE_ONE_PASS;
    service.hseSrv.sbafUpdateReq.streamLength=0;
    Napi::Buffer<uint8_t> payload=Napi::Buffer<uint8_t>::New(info.Env(),sizeof(hseSrvDescriptor_t));
    //*real copy
    uint8_t* data=payload.Data();
    memcpy(data,&service,sizeof(hseSrvDescriptor_t));
    ret.Set("err",0);
    ret.Set("data",payload);
    ret.Set("msg","successful");
    return ret;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports=HSE::Init(env,exports);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
