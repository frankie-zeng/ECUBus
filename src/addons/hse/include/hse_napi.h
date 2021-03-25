#ifndef HSE_HEADER_H
#define HSE_HEADER_H
#define NAPI_VERSION 6
#include <napi.h>



#include <windows.h>

 

class HSE : public Napi::ObjectWrap<HSE>{
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  HSE(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;

  Napi::Value HSE::getVersion(const Napi::CallbackInfo& info);
  Napi::Value HSE::importSymKey(const Napi::CallbackInfo& info);
  Napi::Value HSE::importPubKey(const Napi::CallbackInfo& info);
  Napi::Value HSE::importPrivKey(const Napi::CallbackInfo& info);
};

#endif