#ifndef CANTP_H
#define CANTP_H
#define NAPI_VERSION 6
#include <napi.h>


#ifdef _WIN32
#include <windows.h>
#include "PLinApi.h"

typedef TLINError (__stdcall *fpRegisterClient)(LPSTR, DWORD, HLINCLIENT*); 
typedef TLINError (__stdcall *fpRemoveClient)(HLINCLIENT); 
typedef TLINError (__stdcall *fpConnectClient)(HLINCLIENT, HLINHW); 
typedef TLINError (__stdcall *fpDisconnectClient)(HLINCLIENT,HLINHW);
typedef TLINError (__stdcall *fpResetClient)(HLINCLIENT);
typedef TLINError (__stdcall *fpSetClientParam)(HLINCLIENT,TLINClientParam,DWORD);
typedef TLINError (__stdcall *fpRead)(HLINCLIENT,TLINRcvMsg*); 
typedef TLINError (__stdcall *fpReadMulti)(HLINCLIENT,TLINRcvMsg*,int,int*);
typedef TLINError (__stdcall *fpWrite)(HLINCLIENT,HLINHW,TLINMsg*);
typedef TLINError (__stdcall *fpInitializeHardware)(HLINCLIENT,HLINHW,TLINHardwareMode,WORD);
typedef TLINError (__stdcall *fpSetHardwareParam)(HLINCLIENT,HLINHW,TLINHardwareParam,void*,WORD);
typedef TLINError (__stdcall *fpGetErrorText)(TLINError,BYTE,LPSTR,WORD);
typedef TLINError (__stdcall *fpGetPID)(BYTE*);
typedef TLINError (__stdcall *fpRegisterFrameId)(HLINCLIENT,HLINHW,BYTE,BYTE);
typedef TLINError (__stdcall *fpRegisterFrameId)(HLINCLIENT,HLINHW,BYTE,BYTE);
typedef TLINError (__stdcall *fpSetFrameEntry)(HLINCLIENT,HLINHW,TLINFrameEntry*);
typedef TLINError (__stdcall *fpCalculateChecksum)(TLINMsg*);
typedef TLINError (__stdcall *fpUpdateByteArray)(HLINCLIENT,HLINHW,BYTE,BYTE,BYTE,BYTE*);
typedef TLINError (__stdcall *fpSetClientFilter)(HLINCLIENT,HLINHW,unsigned __int64);
//schedule
typedef TLINError (__stdcall *fpSetSchedule)(HLINCLIENT,HLINHW,int,TLINScheduleSlot*,int);
typedef TLINError (__stdcall *fpDeleteSchedule)(HLINCLIENT,HLINHW,int);
typedef TLINError (__stdcall *fpStartSchedule)(HLINCLIENT,HLINHW,int);
typedef TLINError (__stdcall *fpResumechedule)(HLINCLIENT,HLINHW);
typedef TLINError (__stdcall *fpSuspendSchedule)(HLINCLIENT,HLINHW);
typedef TLINError (__stdcall *fpGetVersion)(TLINVersion*);

// typedef TLINError (__stdcall *fpReset)(TPCANTPHandle);
// typedef TLINError (__stdcall *fpGetStatus)(TPCANTPHandle);
// typedef TLINError (__stdcall *fpRemoveMapping)(TPCANTPHandle,DWORD);
 

class LINAPI : public Napi::ObjectWrap<LINAPI>{
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  LINAPI(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;
  static DWORD WINAPI CallReadThreadFunc(LPVOID lpParam);

  Napi::Value LINAPI::RegisterClient(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::RemoveClient(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::ConnectClient(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::DisconnectClient(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::ResetClient(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::SetClientParam(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::Read(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::ReadMulti(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::Write(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::InitializeHardware(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::SetHardwareParam(const Napi::CallbackInfo& info);
  
  Napi::Value LINAPI::Unload(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::GetErrorText(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::GetPID(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::RegisterFrameId(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::SetFrameEntry(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::UpdateByteArray(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::SetClientFilter(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::CalculateChecksum(const Napi::CallbackInfo& info);

  Napi::Value LINAPI::SetSchedule(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::DeleteSchedule(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::StartSchedule(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::SuspendSchedule(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::ResumeSchedule(const Napi::CallbackInfo& info);
  Napi::Value LINAPI::GetVersion(const Napi::CallbackInfo& info);

  HLINCLIENT client;
  HINSTANCE hDLL;
};
#else
class LINAPI : public Napi::ObjectWrap<LINAPI>{
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  LINAPI(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;
};


#endif


#endif