#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "someip.h"

#define DECLARE_NAPI_UINT32(symbol)\
    InstanceValue(#symbol,Napi::Number::New(env, symbol))

#define DECLARE_NAPI_NAME_UINT32(name,symbol)\
    InstanceValue(name, Napi::Number::New(env, symbol))


static vsomeip::service_t service_id = 0x1111;
static vsomeip::instance_t service_instance_id = 0x2222;
static vsomeip::method_t service_method_id = 0x3333;

Napi::FunctionReference SOMEIP::constructor;


SOMEIP::SOMEIP(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<SOMEIP>(info){
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  Napi::String nPath = info[0].As<Napi::String>();
  Napi::String appName = info[1].As<Napi::String>();
  this->appName=appName.Utf8Value();
  SetDllDirectory(nPath.Utf8Value().data());
//   this->rtm_=vsomeip::runtime::get();
//   this->app_=this->rtm_->create_application(appName.Utf8Value());
  this->core=new SomeipCore(appName.Utf8Value());
  this->rtm_=this->core->GetRtm();
  this->app_=this->core->GetApp();
}


SOMEIP::~SOMEIP(){
 
}

void  SOMEIP::on_state_cbk(vsomeip::state_type_e _state)
{
    auto callback = []( Napi::Env env, Napi::Function jsCallback, uint8_t* state ) {
      jsCallback.Call( {Napi::Number::New( env, *state )} );
      delete state;
    };
    uint8_t* state=new uint8_t((uint8_t)_state);
    if(this->stateFn){
        this->stateFn.BlockingCall(state,callback);
    }
    
}

void  SOMEIP::on_availability_cbk(vsomeip::service_t _service,
        vsomeip::instance_t _instance, bool _is_available)
{   
    typedef struct{
        uint16_t service;
        uint16_t instance;
        bool avl;
    }p;

    auto callback = []( Napi::Env env, Napi::Function jsCallback, p* payload ) {
      
      jsCallback.Call( {Napi::Number::New( env, payload->service),Napi::Number::New( env, payload->instance),Napi::Number::New( env, payload->avl)} );
      delete payload;
    };
   
    p* payload=new p;
    payload->service=_service;
    payload->instance=_instance;
    payload->avl=_is_available;

    auto found_service = avlFn.find(_service);
    if (found_service != avlFn.end()) {
        auto found_instance = found_service->second.find(_instance);
        if (found_instance != found_service->second.end()) {
            found_instance->second.BlockingCall(payload,callback);
        }
    }
}

void SOMEIP::stop(){
    this->app_->clear_all_handler();
    messageFn.clear();
    avlFn.clear();
    this->app_->stop();
    this->rtm_->remove_application(this->appName);
    delete this->core;
}

void  SOMEIP::on_message_cbk(const std::shared_ptr<vsomeip::message> &_response)
{   
    
    uint16_t  service=_response->get_service();
    uint16_t  instance=_response->get_instance();
    uint16_t  method=_response->get_method();

    typedef struct {
        uint16_t service;
        uint16_t instance;
        uint16_t method;
        uint16_t client;
        uint16_t session;
        uint8_t protocolV;
        uint8_t interfaceV;
        uint8_t type;
        uint8_t code;
        uint8_t* payload;
        uint32_t payloadLen;
        uint8_t e2e;
        bool valCrc;
    }msg_t;

    msg_t* tmpmsg = new msg_t;
    tmpmsg->service=service;
    tmpmsg->instance=instance;
    tmpmsg->method=method;
    tmpmsg->client=_response->get_client();
    tmpmsg->session=_response->get_session();
    tmpmsg->protocolV=_response->get_protocol_version();
    tmpmsg->interfaceV=_response->get_interface_version();
    tmpmsg->type=(uint8_t)_response->get_message_type();
    tmpmsg->code=(uint8_t)_response->get_return_code();
    tmpmsg->e2e=_response->get_check_result();
    tmpmsg->valCrc=_response->is_valid_crc();
    std::shared_ptr<vsomeip::payload> pl = _response->get_payload();
    tmpmsg->payload = new uint8_t[pl->get_length()];
    tmpmsg->payloadLen = pl->get_length();
    memcpy(tmpmsg->payload,pl->get_data(),pl->get_length());

    auto callback = []( Napi::Env env, Napi::Function jsCallback, msg_t* msg ) {
        Napi::Object objMsg = Napi::Object::New(env);
        objMsg.Set("service",msg->service);
        objMsg.Set("instance",msg->instance);
        objMsg.Set("method",msg->method);
        objMsg.Set("client",msg->client);
        objMsg.Set("session",msg->session);
        objMsg.Set("protocolV",msg->protocolV);
        objMsg.Set("interfaceV",msg->interfaceV);
        objMsg.Set("type",msg->type);
        objMsg.Set("code",msg->code);
        objMsg.Set("e2e",msg->e2e);
        objMsg.Set("valCrc",msg->valCrc);
        Napi::Buffer<uint8_t> bPayload=Napi::Buffer<uint8_t>::New(env,msg->payloadLen);
        memcpy(bPayload.Data(),msg->payload,msg->payloadLen);
        objMsg.Set("payload",bPayload);
        
        // objMsg.Set("type",msg->type);
        // objMsg.Set("type",msg->type);
        jsCallback.Call( {objMsg} );
        delete msg->payload;
        delete msg;
    };


    auto found_service = this->messageFn.find(service);
    if (found_service != this->messageFn.end()) {
        auto found_instance = found_service->second.find(instance);
        if (found_instance != found_service->second.end()) {
            auto found_method = found_instance->second.find(method);
            if (found_method != found_instance->second.end()) {
                found_method->second.BlockingCall(tmpmsg,callback);
            }
        }
    }
}

DWORD WINAPI SOMEIP::StartAppThread(LPVOID lpParam){
    SOMEIP* the=(SOMEIP*)lpParam;
    the->app_->start();
    printf("app thread exit\r\n");
    return 0; 
}

Napi::Value SOMEIP::IsRouting(const Napi::CallbackInfo& info){
    return Napi::Boolean::New(info.Env(),this->app_->is_routing());
}

Napi::Value SOMEIP::StartApp(const Napi::CallbackInfo& info){
    this->startThread=CreateThread(NULL, NULL, SOMEIP::StartAppThread, (LPVOID)this, NULL, NULL);
    return Napi::Number::New(info.Env(),0);
}

Napi::Value SOMEIP::StopApp(const Napi::CallbackInfo& info){
    this->stop();
    return Napi::Number::New(info.Env(),0);
}

Napi::Value SOMEIP::CreateApp(const Napi::CallbackInfo& info){
   
    if (!this->app_->init()) {
        printf("Couldn't initialize application");
        return Napi::Number::New(info.Env(),-1);
    }
    this->app_->register_state_handler(
                std::bind(&SOMEIP::on_state_cbk, this,
                        std::placeholders::_1));

    return Napi::Number::New(info.Env(),0);
}


Napi::Value SOMEIP::RegisterStateHandler(const Napi::CallbackInfo& info){
    this->stateFn=Napi::ThreadSafeFunction::New(info.Env(),info[0].As<Napi::Function>(),"stateCB",0,1);
    return Napi::Number::New(info.Env(),0);
}

Napi::Value SOMEIP::RegisterMsgHandler(const Napi::CallbackInfo& info){
    uint16_t service=info[0].As<Napi::Number>().Uint32Value();
    uint16_t instance=info[1].As<Napi::Number>().Uint32Value();
    uint16_t method=info[2].As<Napi::Number>().Uint32Value();
    char Name[128];
    sprintf(Name,"m:%04x:%04x:%04x",service,instance,method);
    messageFn[service][instance][method]=Napi::ThreadSafeFunction::New(info.Env(),info[3].As<Napi::Function>(),Name,0,1);
    // register a callback for responses from the service
    this->app_->register_message_handler(service,instance,method,
            std::bind(&SOMEIP::on_message_cbk, this,
                    std::placeholders::_1));
    return Napi::Number::New(info.Env(),0);
}

Napi::Value SOMEIP::UnRegisterMsgHandler(const Napi::CallbackInfo& info){
    uint16_t service=info[0].As<Napi::Number>().Uint32Value();
    uint16_t instance=info[1].As<Napi::Number>().Uint32Value();
    uint16_t method=info[2].As<Napi::Number>().Uint32Value();
    auto found_service = messageFn.find(service);
    if (found_service != messageFn.end()) {
        auto found_instance = found_service->second.find(instance);
        if (found_instance != found_service->second.end()) {
            auto found_method = found_instance->second.find(method);
            if (found_method != found_instance->second.end()) {
                found_instance->second.erase(method);
            }
        }
    }
    this->app_->unregister_message_handler(service,instance,method);
    return Napi::Number::New(info.Env(),0);
}
Napi::Value SOMEIP::RegisterAvlHandler(const Napi::CallbackInfo& info){
    uint16_t service=info[0].As<Napi::Number>().Uint32Value();
    uint16_t instance=info[1].As<Napi::Number>().Uint32Value();
    char Name[128];
    sprintf(Name,"m:%04x:%04x",service,instance);
   
    avlFn[service][instance]=Napi::ThreadSafeFunction::New(info.Env(),info[2].As<Napi::Function>(),Name,0,1);
    // register a callback for responses from the service
     this->app_->register_availability_handler(service, instance,
            std::bind(&SOMEIP::on_availability_cbk, this,
                    std::placeholders::_1, std::placeholders::_2,
                    std::placeholders::_3),vsomeip::ANY_MAJOR,vsomeip::ANY_MINOR);
    return Napi::Number::New(info.Env(),0);
}
Napi::Value SOMEIP::UnRegisterAvlHandler(const Napi::CallbackInfo& info){
    uint16_t service=info[0].As<Napi::Number>().Uint32Value();
    uint16_t instance=info[1].As<Napi::Number>().Uint32Value();
    auto found_service = avlFn.find(service);
    if (found_service != avlFn.end()) {
        auto found_instance = found_service->second.find(instance);
        if (found_instance != found_service->second.end()) {
           found_service->second.erase(instance);
        }
    }
    this->app_->unregister_availability_handler(service,instance,vsomeip::ANY_MAJOR,vsomeip::ANY_MINOR);
    return Napi::Number::New(info.Env(),0);
}

Napi::Value SOMEIP::Send(const Napi::CallbackInfo& info){
    Napi::Object nMsg=info[0].As<Napi::Object>();
    uint16_t service=nMsg.Get("service").ToNumber().Uint32Value();
    uint16_t instance=nMsg.Get("instance").ToNumber().Uint32Value();
    uint16_t method=nMsg.Get("method").ToNumber().Uint32Value();
    bool reliable = nMsg.Get("reliable").ToBoolean();
    Napi::Buffer<uint8_t> payload=Napi::Buffer<uint8_t>::Buffer(info.Env(),nMsg.Get("payload"));
    // The service is available then we send the request
    // Create a new request
    std::shared_ptr<vsomeip::message> rq = rtm_->create_request(reliable);
    // Set the hello world service as target of the request
    rq->set_service(service);
    rq->set_instance(instance);
    rq->set_method(method);

    // Create a payload which will be sent to the service
    std::shared_ptr<vsomeip::payload> pl = rtm_->create_payload();
    std::vector<vsomeip::byte_t> pl_data(payload.Data(), payload.Data()+payload.Length());
    
    pl->set_data(pl_data);
    rq->set_payload(pl);
    app_->send(rq);
    printf("Sending: %s", payload.Data());
    return Napi::Number::New(info.Env(),0);
}

Napi::Value SOMEIP::RequestService(const Napi::CallbackInfo& info){
    uint16_t service=info[0].As<Napi::Number>().Uint32Value();
    uint16_t instance=info[1].As<Napi::Number>().Uint32Value();
    uint8_t major=vsomeip::ANY_MAJOR;
    uint32_t minor=vsomeip::ANY_MINOR;
    if(info.Length()>2){
        major=info[2].As<Napi::Number>().Uint32Value();
    }
    if(info.Length()>3){
        minor=info[3].As<Napi::Number>().Uint32Value();
    }
    this->app_->request_service(service,instance,major,minor);
    return Napi::Number::New(info.Env(),0);
}

Napi::Value SOMEIP::ReleaseService(const Napi::CallbackInfo& info){
    uint16_t service=info[0].As<Napi::Number>().Uint32Value();
    uint16_t instance=info[1].As<Napi::Number>().Uint32Value();
    this->app_->release_service(service,instance);
    return Napi::Number::New(info.Env(),0);
}

Napi::Value SOMEIP::OfferService(const Napi::CallbackInfo& info){
    uint16_t service=info[0].As<Napi::Number>().Uint32Value();
    uint16_t instance=info[1].As<Napi::Number>().Uint32Value();
    uint8_t major=0;
    uint32_t minor=0;
    if(info.Length()>2){
        major=info[2].As<Napi::Number>().Uint32Value();
    }
    if(info.Length()>3){
        minor=info[3].As<Napi::Number>().Uint32Value();
    }
    this->app_->offer_service(service,instance,major,minor);
    return Napi::Number::New(info.Env(),0);
}

Napi::Value SOMEIP::StopOfferService(const Napi::CallbackInfo& info){
    uint16_t service=info[0].As<Napi::Number>().Uint32Value();
    uint16_t instance=info[1].As<Napi::Number>().Uint32Value();
    uint8_t major=0;
    uint32_t minor=0;
    if(info.Length()>2){
        major=info[2].As<Napi::Number>().Uint32Value();
    }
    if(info.Length()>3){
        minor=info[3].As<Napi::Number>().Uint32Value();
    }
    this->app_->stop_offer_service(service,instance,major,minor);
    return Napi::Number::New(info.Env(),0);
}


Napi::Object SOMEIP::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);
    
  Napi::Function func =
      DefineClass(env,
                  "SOMEIP",
                  {
                      DECLARE_NAPI_NAME_UINT32("ANY_SERVICE",vsomeip::ANY_SERVICE),
                      DECLARE_NAPI_NAME_UINT32("ANY_METHOD",vsomeip::ANY_METHOD),
                      DECLARE_NAPI_NAME_UINT32("ANY_INSTANCE",vsomeip::ANY_INSTANCE),
                      DECLARE_NAPI_NAME_UINT32("ANY_MAJOR",vsomeip::ANY_MAJOR),
                      DECLARE_NAPI_NAME_UINT32("ANY_MINOR",vsomeip::ANY_MINOR),
                      InstanceMethod("CreateApp", &SOMEIP::CreateApp),
                      InstanceMethod("StartApp", &SOMEIP::StartApp),
                      InstanceMethod("StopApp", &SOMEIP::StopApp),
                      InstanceMethod("RegisterStateHandler", &SOMEIP::RegisterStateHandler),
                      InstanceMethod("RegisterMsgHandler", &SOMEIP::RegisterMsgHandler),
                      InstanceMethod("UnRegisterMsgHandler", &SOMEIP::UnRegisterMsgHandler),
                      InstanceMethod("RegisterAvlHandler", &SOMEIP::RegisterAvlHandler),
                      InstanceMethod("UnRegisterAvlHandler", &SOMEIP::UnRegisterAvlHandler),
                      InstanceMethod("Send", &SOMEIP::Send),
                      InstanceMethod("RequestService", &SOMEIP::RequestService),
                      InstanceMethod("ReleaseService", &SOMEIP::ReleaseService),
                      InstanceMethod("OfferService", &SOMEIP::OfferService),
                      InstanceMethod("StopOfferService", &SOMEIP::StopOfferService),
                      InstanceMethod("IsRouting", &SOMEIP::IsRouting),
                   });

    constructor = Napi::Persistent(func);
//   constructor.SuppressDestruct();

  exports.Set("SOMEIP", func);
  return exports;
}





Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports=SOMEIP::Init(env,exports);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
