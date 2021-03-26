#include <string.h>
#include <stdio.h>
#include "hse_napi.h"
#include "mbedtls/version.h"
#include "mbedtls/error.h"
#include "mbedtls/pk.h"
#include "mbedtls/bignum.h"
#include "hse_interface.h"



Napi::FunctionReference HSE::constructor;


Napi::Object HSE::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);
    
  Napi::Function func =
      DefineClass(env,
                  "HSE",
                  {
                   InstanceMethod("getVersion", &HSE::getVersion),
                   InstanceMethod("importSymKey", &HSE::importSymKey),
                   InstanceMethod("importPubKey", &HSE::importPubKey),
                   InstanceMethod("importPrivKey", &HSE::importPrivKey),
                   InstanceMethod("formatCatalog", &HSE::formatCatalog),
                   });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("HSE", func);
  return exports;
}

static char* getErrorMsg(int err){
    char* buf=new char[512];
    mbedtls_strerror(err,buf,512);
    return buf;
}

static hseEccCurveId_t getCurveId(mbedtls_ecp_group_id id){
    hseEccCurveId_t eccCurveId;
    switch(id){
        case MBEDTLS_ECP_DP_SECP256R1:
            eccCurveId=HSE_EC_SEC_SECP256R1;
            break;
        case MBEDTLS_ECP_DP_SECP384R1:
            eccCurveId=HSE_EC_SEC_SECP384R1;
            break;
        case MBEDTLS_ECP_DP_SECP521R1:
            eccCurveId=HSE_EC_SEC_SECP521R1;
            break;
        case MBEDTLS_ECP_DP_BP256R1:
            eccCurveId=HSE_EC_BRAINPOOL_BRAINPOOLP256R1;
            break;
        case MBEDTLS_ECP_DP_BP384R1:
            eccCurveId=HSE_EC_BRAINPOOL_BRAINPOOLP384R1;
            break;
        case MBEDTLS_ECP_DP_BP512R1:
            eccCurveId=HSE_EC_BRAINPOOL_BRAINPOOLP512R1;
            break;
        default:
            eccCurveId=MBEDTLS_ECP_DP_NONE;
            break;
    }
    return eccCurveId;
}
HSE::HSE(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<HSE>(info) {
    return;
}

Napi::Value HSE::getVersion(const Napi::CallbackInfo& info){
    char version[256];
    mbedtls_version_get_string(version);
    return Napi::String::New(info.Env(),version);
}

Napi::Value HSE::importPrivKey(const Napi::CallbackInfo& info){
    hseSrvDescriptor_t service;
    hseKeyInfo_t keyInfo;
    mbedtls_pk_context ctx;
    mbedtls_pk_init(&ctx);



    memset((void*)&service,0,sizeof(hseSrvDescriptor_t));
    memset((void*)&keyInfo,0,sizeof(hseKeyInfo_t));
    Napi::Object ret=Napi::Object::New(info.Env());

    Napi::Object i=info[0].As<Napi::Object>();
    uint32_t offset=i.Get("offset").ToNumber().Uint32Value();
    uint32_t keyHandle=i.Get("handle").ToNumber().Uint32Value();
    keyInfo.keyFlags=i.Get("flags").ToNumber().Uint32Value();
    keyInfo.keyCounter=i.Get("counter").ToNumber().Uint32Value();
    keyInfo.smrFlags=i.Get("smr").ToNumber().Uint32Value();
    keyInfo.keyType=i.Get("type").ToNumber().Uint32Value();

    std::string key=info[1].As<Napi::String>();
    int err=mbedtls_pk_parse_key(&ctx,(const uint8_t*)key.c_str(),key.length()+1,NULL,0);
    if(err==0){
        keyInfo.keyBitLen=mbedtls_pk_get_bitlen(&ctx);
        mbedtls_pk_type_t pkType=mbedtls_pk_get_type(&ctx);
        if(pkType==MBEDTLS_PK_RSA){
            mbedtls_rsa_context* rsa;
            rsa=mbedtls_pk_rsa(ctx);
            keyInfo.specific.pubExponentSize=mbedtls_mpi_size(&rsa->E);
            Napi::Buffer<uint8_t> payload=Napi::Buffer<uint8_t>::New(info.Env(),sizeof(hseSrvDescriptor_t)+sizeof(hseKeyInfo_t)+mbedtls_mpi_size(&rsa->N)+mbedtls_mpi_size(&rsa->E)+mbedtls_mpi_size(&rsa->D));
            uint8_t* data=payload.Data();
            service.srvId=HSE_SRV_ID_IMPORT_KEY;
            service.hseSrv.importKeyReq.cipher.cipherKeyHandle= HSE_INVALID_KEY_HANDLE;
            service.hseSrv.importKeyReq.keyContainer.authKeyHandle= HSE_INVALID_KEY_HANDLE;
            service.hseSrv.importKeyReq.pKeyInfo=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t));
            service.hseSrv.importKeyReq.keyLen[0]=mbedtls_mpi_size(&rsa->N);
            service.hseSrv.importKeyReq.pKey[0]=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseKeyInfo_t));
            service.hseSrv.importKeyReq.keyLen[1]=mbedtls_mpi_size(&rsa->E);
            service.hseSrv.importKeyReq.pKey[1]=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseKeyInfo_t)+mbedtls_mpi_size(&rsa->N));
            service.hseSrv.importKeyReq.keyLen[2]=mbedtls_mpi_size(&rsa->D);
            service.hseSrv.importKeyReq.pKey[2]=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseKeyInfo_t)+mbedtls_mpi_size(&rsa->N)+mbedtls_mpi_size(&rsa->E));
            service.hseSrv.importKeyReq.targetKeyHandle=(hseKeyHandle_t)keyHandle;
            memcpy(data,&service,sizeof(hseSrvDescriptor_t));
            data+=sizeof(hseSrvDescriptor_t);
            memcpy(data,&keyInfo,sizeof(hseKeyInfo_t));
            data+=sizeof(hseKeyInfo_t);
            mbedtls_mpi_write_binary(&rsa->N,data,mbedtls_mpi_size(&rsa->N));
            data+=mbedtls_mpi_size(&rsa->N);
            mbedtls_mpi_write_binary(&rsa->E,data,mbedtls_mpi_size(&rsa->E));
            data+=mbedtls_mpi_size(&rsa->E);
            mbedtls_mpi_write_binary(&rsa->D,data,mbedtls_mpi_size(&rsa->D));
            ret.Set("err",0);
            ret.Set("data",payload);
            ret.Set("msg","successful");
        }else if(pkType==MBEDTLS_PK_ECKEY){
            mbedtls_ecp_keypair* ec;
            ec=mbedtls_pk_ec(ctx);
            keyInfo.specific.eccCurveId=getCurveId(ec->grp.id);
            if(keyInfo.specific.eccCurveId!=MBEDTLS_ECP_DP_NONE){
                Napi::Buffer<uint8_t> payload=Napi::Buffer<uint8_t>::New(info.Env(),sizeof(hseSrvDescriptor_t)+sizeof(hseKeyInfo_t)+mbedtls_mpi_size(&ec->Q.X)+mbedtls_mpi_size(&ec->Q.Y)+mbedtls_mpi_size(&ec->d));
                uint8_t* data=payload.Data();
                service.srvId=HSE_SRV_ID_IMPORT_KEY;
                service.hseSrv.importKeyReq.cipher.cipherKeyHandle= HSE_INVALID_KEY_HANDLE;
                service.hseSrv.importKeyReq.keyContainer.authKeyHandle= HSE_INVALID_KEY_HANDLE;
                service.hseSrv.importKeyReq.pKeyInfo=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t));
                service.hseSrv.importKeyReq.keyLen[0]=mbedtls_mpi_size(&ec->Q.X)+mbedtls_mpi_size(&ec->Q.Y);
                service.hseSrv.importKeyReq.pKey[0]=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseKeyInfo_t));
                service.hseSrv.importKeyReq.keyLen[2]=mbedtls_mpi_size(&ec->d);
                service.hseSrv.importKeyReq.pKey[2]=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseKeyInfo_t)+mbedtls_mpi_size(&ec->Q.X)+mbedtls_mpi_size(&ec->Q.Y));
                service.hseSrv.importKeyReq.targetKeyHandle=(hseKeyHandle_t)keyHandle;
                memcpy(data,&service,sizeof(hseSrvDescriptor_t));
                data+=sizeof(hseSrvDescriptor_t);
                memcpy(data,&keyInfo,sizeof(hseKeyInfo_t));
                data+=sizeof(hseKeyInfo_t);
                mbedtls_mpi_write_binary(&ec->Q.X,data,mbedtls_mpi_size(&ec->Q.X));
                data+=mbedtls_mpi_size(&ec->Q.X);
                mbedtls_mpi_write_binary(&ec->Q.Y,data,mbedtls_mpi_size(&ec->Q.Y));
                data+=mbedtls_mpi_size(&ec->Q.Y);
                mbedtls_mpi_write_binary(&ec->d,data,mbedtls_mpi_size(&ec->d));
                ret.Set("err",0);
                ret.Set("data",payload);
                ret.Set("msg","successful");
            }else{
                ret.Set("err",-1);
                ret.Set("data",Napi::Buffer<uint8_t>::New(info.Env(),0));
                ret.Set("msg","EC unsupport curve id");
            }
        }
        
    }else{
        ret.Set("err",err);
        ret.Set("data",Napi::Buffer<uint8_t>::New(info.Env(),0));
        char* errMsg=getErrorMsg(err);
        ret.Set("msg",errMsg);
        delete errMsg;
    }
    //TODO EDDSA
    // else{
    // }
    mbedtls_pk_free(&ctx);
    
    return ret;
}
Napi::Value HSE::importPubKey(const Napi::CallbackInfo& info){
    hseSrvDescriptor_t service;
    hseKeyInfo_t keyInfo;
    mbedtls_pk_context ctx;
    mbedtls_pk_init(&ctx);



    memset((void*)&service,0,sizeof(hseSrvDescriptor_t));
    memset((void*)&keyInfo,0,sizeof(hseKeyInfo_t));
    Napi::Object ret=Napi::Object::New(info.Env());

    Napi::Object i=info[0].As<Napi::Object>();
    uint32_t offset=i.Get("offset").ToNumber().Uint32Value();
    uint32_t keyHandle=i.Get("handle").ToNumber().Uint32Value();
    keyInfo.keyFlags=i.Get("flags").ToNumber().Uint32Value();
    keyInfo.keyCounter=i.Get("counter").ToNumber().Uint32Value();
    keyInfo.smrFlags=i.Get("smr").ToNumber().Uint32Value();
    keyInfo.keyType=i.Get("type").ToNumber().Uint32Value();

    std::string key=info[1].As<Napi::String>();
    int err=mbedtls_pk_parse_public_key(&ctx,(const uint8_t*)key.c_str(),key.length()+1);
    if(err==0){
        keyInfo.keyBitLen=mbedtls_pk_get_bitlen(&ctx);
        mbedtls_pk_type_t pkType=mbedtls_pk_get_type(&ctx);
        if(pkType==MBEDTLS_PK_RSA){
            mbedtls_rsa_context* rsa;
            rsa=mbedtls_pk_rsa(ctx);
            keyInfo.specific.pubExponentSize=mbedtls_mpi_size(&rsa->E);
            Napi::Buffer<uint8_t> payload=Napi::Buffer<uint8_t>::New(info.Env(),sizeof(hseSrvDescriptor_t)+sizeof(hseKeyInfo_t)+mbedtls_mpi_size(&rsa->N)+mbedtls_mpi_size(&rsa->E));
            uint8_t* data=payload.Data();
            service.srvId=HSE_SRV_ID_IMPORT_KEY;
            service.hseSrv.importKeyReq.cipher.cipherKeyHandle= HSE_INVALID_KEY_HANDLE;
            service.hseSrv.importKeyReq.keyContainer.authKeyHandle= HSE_INVALID_KEY_HANDLE;
            service.hseSrv.importKeyReq.pKeyInfo=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t));
            service.hseSrv.importKeyReq.keyLen[0]=mbedtls_mpi_size(&rsa->N);
            service.hseSrv.importKeyReq.pKey[0]=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseKeyInfo_t));
            service.hseSrv.importKeyReq.keyLen[1]=mbedtls_mpi_size(&rsa->E);
            service.hseSrv.importKeyReq.pKey[1]=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseKeyInfo_t)+mbedtls_mpi_size(&rsa->N));
            service.hseSrv.importKeyReq.targetKeyHandle=(hseKeyHandle_t)keyHandle;
            memcpy(data,&service,sizeof(hseSrvDescriptor_t));
            data+=sizeof(hseSrvDescriptor_t);
            memcpy(data,&keyInfo,sizeof(hseKeyInfo_t));
            data+=sizeof(hseKeyInfo_t);
            mbedtls_mpi_write_binary(&rsa->N,data,mbedtls_mpi_size(&rsa->N));
            data+=mbedtls_mpi_size(&rsa->N);
            mbedtls_mpi_write_binary(&rsa->E,data,mbedtls_mpi_size(&rsa->E));
            ret.Set("err",0);
            ret.Set("data",payload);
            ret.Set("msg","successful");
        }else if(pkType==MBEDTLS_PK_ECKEY){
            mbedtls_ecp_keypair* ec;
            ec=mbedtls_pk_ec(ctx);
            keyInfo.specific.eccCurveId=getCurveId(ec->grp.id);
            if(keyInfo.specific.eccCurveId!=MBEDTLS_ECP_DP_NONE){
                Napi::Buffer<uint8_t> payload=Napi::Buffer<uint8_t>::New(info.Env(),sizeof(hseSrvDescriptor_t)+sizeof(hseKeyInfo_t)+mbedtls_mpi_size(&ec->Q.X)+mbedtls_mpi_size(&ec->Q.Y));
                uint8_t* data=payload.Data();
                service.srvId=HSE_SRV_ID_IMPORT_KEY;
                service.hseSrv.importKeyReq.cipher.cipherKeyHandle= HSE_INVALID_KEY_HANDLE;
                service.hseSrv.importKeyReq.keyContainer.authKeyHandle= HSE_INVALID_KEY_HANDLE;
                service.hseSrv.importKeyReq.pKeyInfo=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t));
                service.hseSrv.importKeyReq.keyLen[0]=mbedtls_mpi_size(&ec->Q.X)+mbedtls_mpi_size(&ec->Q.Y);
                service.hseSrv.importKeyReq.pKey[0]=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseKeyInfo_t));
                service.hseSrv.importKeyReq.targetKeyHandle=(hseKeyHandle_t)keyHandle;
                memcpy(data,&service,sizeof(hseSrvDescriptor_t));
                data+=sizeof(hseSrvDescriptor_t);
                memcpy(data,&keyInfo,sizeof(hseKeyInfo_t));
                data+=sizeof(hseKeyInfo_t);
                mbedtls_mpi_write_binary(&ec->Q.X,data,mbedtls_mpi_size(&ec->Q.X));
                data+=mbedtls_mpi_size(&ec->Q.X);
                mbedtls_mpi_write_binary(&ec->Q.Y,data,mbedtls_mpi_size(&ec->Q.Y));
                ret.Set("err",0);
                ret.Set("data",payload);
                ret.Set("msg","successful");
            }else{
                ret.Set("err",-1);
                ret.Set("data",Napi::Buffer<uint8_t>::New(info.Env(),0));
                ret.Set("msg","EC unsupport curve id");
            }
        }
        
    }else{
        ret.Set("err",err);
        ret.Set("data",Napi::Buffer<uint8_t>::New(info.Env(),0));
        char* errMsg=getErrorMsg(err);
        ret.Set("msg",errMsg);
        delete errMsg;
    }
    //TODO EDDSA
    // else{
    // }
    mbedtls_pk_free(&ctx);
    
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
    
    hseKeyGroupCfgEntry_t* nvmCat = new hseKeyGroupCfgEntry_t[nvmLen];
    hseKeyGroupCfgEntry_t* ramCat = new hseKeyGroupCfgEntry_t[ramLen];

    for(uint32_t i=0;i<nvmLen;i++){
        Napi::Value v=nvm[i];
        nvmCat[i].groupOwner=v.ToObject().Get("keyOwn").ToNumber().Uint32Value();
        nvmCat[i].keyType=v.ToObject().Get("keyType").ToNumber().Uint32Value();
        nvmCat[i].maxKeyBitLen=v.ToObject().Get("keySize").ToNumber().Uint32Value();
        nvmCat[i].muMask=v.ToObject().Get("muMask").ToNumber().Uint32Value();
        nvmCat[i].numOfKeySlots=v.ToObject().Get("keyNum").ToNumber().Uint32Value();
        nvmCat[i].hseReserved[0]=0;
        nvmCat[i].hseReserved[1]=0;
    } 
    for(uint32_t i=0;i<ramLen;i++){
        Napi::Value v=ram[i];
        ramCat[i].groupOwner=v.ToObject().Get("keyOwn").ToNumber().Uint32Value();
        ramCat[i].keyType=v.ToObject().Get("keyType").ToNumber().Uint32Value();
        ramCat[i].maxKeyBitLen=v.ToObject().Get("keySize").ToNumber().Uint32Value();
        ramCat[i].muMask=v.ToObject().Get("muMask").ToNumber().Uint32Value();
        ramCat[i].numOfKeySlots=v.ToObject().Get("keyNum").ToNumber().Uint32Value();
        ramCat[i].hseReserved[0]=0;
        ramCat[i].hseReserved[1]=0;
    } 

    Napi::Buffer<uint8_t> payload=Napi::Buffer<uint8_t>::New(info.Env(),sizeof(hseSrvDescriptor_t)+sizeof(hseKeyGroupCfgEntry_t)*nvmLen+sizeof(hseKeyGroupCfgEntry_t)*ramLen);
    uint8_t* data=payload.Data();
    service.srvId=HSE_SRV_ID_FORMAT_KEY_CATALOGS;
    service.hseSrv.formatKeyCatalogsReq.pNvmKeyCatalogCfg= (HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t));
    service.hseSrv.formatKeyCatalogsReq.pRamKeyCatalogCfg= (HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseKeyGroupCfgEntry_t)*nvmLen);
    memcpy(data,&service,sizeof(hseSrvDescriptor_t));
    data+=sizeof(hseSrvDescriptor_t);
    memcpy(data,nvmCat,sizeof(hseKeyGroupCfgEntry_t)*nvmLen);
    data+=sizeof(hseKeyGroupCfgEntry_t)*nvmLen;
    memcpy(data,ramCat,sizeof(hseKeyGroupCfgEntry_t)*ramLen);
    delete nvmCat;
    delete ramCat;
    
    ret.Set("err",0);
    ret.Set("data",payload);
    ret.Set("msg","successful");
    
    return ret;
}

Napi::Value HSE::importSymKey(const Napi::CallbackInfo& info){
    hseSrvDescriptor_t service;
    hseKeyInfo_t keyInfo;

    memset((void*)&service,0,sizeof(hseSrvDescriptor_t));
    memset((void*)&keyInfo,0,sizeof(hseKeyInfo_t));
    Napi::Object ret=Napi::Object::New(info.Env());

    Napi::Object i=info[0].As<Napi::Object>();
    uint32_t offset=i.Get("offset").ToNumber().Uint32Value();
    uint32_t keyHandle=i.Get("handle").ToNumber().Uint32Value();
    keyInfo.keyFlags=i.Get("flags").ToNumber().Uint32Value();
    keyInfo.keyCounter=i.Get("counter").ToNumber().Uint32Value();
    keyInfo.smrFlags=i.Get("smr").ToNumber().Uint32Value();
    keyInfo.keyType=i.Get("type").ToNumber().Uint32Value();

    Napi::Buffer<uint8_t> key=info[1].As<Napi::Buffer<uint8_t>>();
    keyInfo.keyBitLen=key.Length()*8;
    
  
    Napi::Buffer<uint8_t> payload=Napi::Buffer<uint8_t>::New(info.Env(),sizeof(hseSrvDescriptor_t)+sizeof(hseKeyInfo_t)+key.Length());
    uint8_t* data=payload.Data();
    service.srvId=HSE_SRV_ID_IMPORT_KEY;
    service.hseSrv.importKeyReq.cipher.cipherKeyHandle= HSE_INVALID_KEY_HANDLE;
    service.hseSrv.importKeyReq.keyContainer.authKeyHandle= HSE_INVALID_KEY_HANDLE;
    service.hseSrv.importKeyReq.pKeyInfo=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t));
    service.hseSrv.importKeyReq.keyLen[2]=key.Length();
    service.hseSrv.importKeyReq.pKey[2]=(HOST_ADDR)(offset+sizeof(hseSrvDescriptor_t)+sizeof(hseKeyInfo_t));
    service.hseSrv.importKeyReq.targetKeyHandle=(hseKeyHandle_t)keyHandle;
    memcpy(data,&service,sizeof(hseSrvDescriptor_t));
    data+=sizeof(hseSrvDescriptor_t);
    memcpy(data,&keyInfo,sizeof(hseKeyInfo_t));
    data+=sizeof(hseKeyInfo_t);
    memcpy(data,key.Data(),key.Length());

    
    ret.Set("err",0);
    ret.Set("data",payload);
    ret.Set("msg","successful");
    
    return ret;
}




#define DECLARE_NAPI_METHOD(name,func) \
    exports.Set(Napi::String::New(env, name), \
              Napi::Function::New(env, func))

#define DECLARE_NAPI_UINT32(symbol)\
    exports.Set(Napi::String::New(env, #symbol), \
              Napi::Number::New(env, symbol))

#define DECLARE_NAPI_STRING(name,value)\
    exports.Set(Napi::String::New(env, #name), \
              Napi::String::New(env, value))

Napi::Object Init(Napi::Env env, Napi::Object exports) {

    exports=HSE::Init(env,exports);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
