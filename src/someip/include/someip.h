#ifndef HSE_HEADER_H
#define HSE_HEADER_H
#define NAPI_VERSION 6
#include <napi.h>


#include <windows.h>
#include "someipcore.h"
 

class SOMEIP : public Napi::ObjectWrap<SOMEIP>{
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  SOMEIP(const Napi::CallbackInfo& info);
  ~SOMEIP();
  Napi::Value SOMEIP::CreateApp(const Napi::CallbackInfo& info);
  Napi::Value SOMEIP::StartApp(const Napi::CallbackInfo& info);
  Napi::Value SOMEIP::StopApp(const Napi::CallbackInfo& info);
  Napi::Value SOMEIP::RegisterStateHandler(const Napi::CallbackInfo& info);
  Napi::Value SOMEIP::RegisterMsgHandler(const Napi::CallbackInfo& info);
  Napi::Value SOMEIP::UnRegisterMsgHandler(const Napi::CallbackInfo& info);
  Napi::Value SOMEIP::RegisterAvlHandler(const Napi::CallbackInfo& info);
  Napi::Value SOMEIP::UnRegisterAvlHandler(const Napi::CallbackInfo& info);
  Napi::Value SOMEIP::Send(const Napi::CallbackInfo& info);
  Napi::Value SOMEIP::RequestService(const Napi::CallbackInfo& info);
  Napi::Value SOMEIP::ReleaseService(const Napi::CallbackInfo& info);
  Napi::Value SOMEIP::OfferService(const Napi::CallbackInfo& info);
  Napi::Value SOMEIP::StopOfferService(const Napi::CallbackInfo& info);
  Napi::Value SOMEIP::IsRouting(const Napi::CallbackInfo& info);
  

 private:
  static Napi::FunctionReference constructor;
  static DWORD WINAPI StartAppThread(LPVOID lpParam);
  
  SomeipCore* core;
  std::shared_ptr<vsomeip::runtime> rtm_;
  std::shared_ptr<vsomeip::application> app_;



  HANDLE startThread;
  Napi::ThreadSafeFunction  stateFn;
  std::map<uint16_t,std::map<uint16_t, std::map<uint16_t, Napi::ThreadSafeFunction> > > messageFn;
  
  std::map<uint16_t,std::map<uint16_t, Napi::ThreadSafeFunction> > avlFn;
  std::string appName;
  void stop();
  void on_state_cbk(vsomeip::state_type_e _state);
  void on_availability_cbk(vsomeip::service_t _service,vsomeip::instance_t _instance, bool _is_available);
  void on_message_cbk(const std::shared_ptr<vsomeip::message> &_response);
};

#endif