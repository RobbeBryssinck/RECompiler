#pragma once

#include <Windows.h>
#include "defs.h"

BOOL InitializeHooks();

namespace REImplementation
{

static uintptr_t s_baseAddress = 0;

uintptr_t GetProcessBaseAddress();
bool InstallHookEx(LPVOID apTarget, LPVOID apDetour, LPVOID* appOriginal);

}

template <class T>
class IdaAddress
{
public:
  IdaAddress() = delete;
  IdaAddress(uintptr_t aRva)
  {
    using namespace REImplementation;
    
    if (s_baseAddress == 0)
      s_baseAddress = GetProcessBaseAddress();

    pData = reinterpret_cast<T*>(s_baseAddress + aRva);
  }

  uintptr_t GetAddress()
  {
    return reinterpret_cast<uintptr_t>(pData);
  }

  uintptr_t GetRva()
  {
    using namespace REImplementation;
    return GetAddress() - s_baseAddress;
  }

protected:
  T* pData{};
};

template <class T>
class IdaVariable final : public IdaAddress<T>
{
public:
  using IdaAddress<T>::pData;

  IdaVariable() = delete;
  IdaVariable(uintptr_t aRva) : IdaAddress<T>(aRva) {}

  T& Get()
  {
    return *pData;
  }
};

template <class T>
class IdaFunction final : public IdaAddress<T>
{
public:
  using IdaAddress<T>::pData;

  IdaFunction() = delete;
  IdaFunction(uintptr_t aRva, T* apHookFunction) : IdaAddress<T>(aRva), pHookFunction(apHookFunction) {}

  template <class... TArguments>
  constexpr decltype(auto) operator()(TArguments&& ... aArguments) noexcept
  {
    return pData(std::forward<TArguments>(aArguments)...);
  }

  // Functions with variadic arguments should call the function directly.
  T* GetRawFunctionPointer()
  {
    return pData;
  }

  bool InstallHook()
  {
    return REImplementation::InstallHookEx(pData, pHookFunction, (LPVOID*)&pData);
  }

protected:
  T* pHookFunction{};
};

// Symbol defined, type: stub function, address: 0x1400116b3
void __fastcall j___CheckForDebuggerJustMyCode(unsigned __int8 *JMC_flag);
static IdaFunction<decltype(j___CheckForDebuggerJustMyCode)> Real_j___CheckForDebuggerJustMyCode { 71347, &j___CheckForDebuggerJustMyCode };

// Symbol defined, type: stub function, address: 0x14001120d
unsigned __int8 __fastcall GenerateKey();
static IdaFunction<decltype(GenerateKey)> Real_GenerateKey { 70157, &GenerateKey };

// Symbol defined, type: stub function, address: 0x1400113f7
int j_scanf_s(const char *_Format, ...);
static IdaFunction<decltype(j_scanf_s)> Real_j_scanf_s { 70647, &j_scanf_s };

// Symbol defined, type: stub function, address: 0x14001161d
bool __fastcall IsKeyValid(unsigned __int8 aKey);
static IdaFunction<decltype(IsKeyValid)> Real_IsKeyValid { 71197, &IsKeyValid };

// Symbol defined, type: stub function, address: 0x140011339
int j_printf(const char *_Format, ...);
static IdaFunction<decltype(j_printf)> Real_j_printf { 70457, &j_printf };

// Symbol defined, type: stub function, address: 0x1400115c3
void __fastcall j__RTC_CheckStackVars(void *frame, void *v);
static IdaFunction<decltype(j__RTC_CheckStackVars)> Real_j__RTC_CheckStackVars { 71107, &j__RTC_CheckStackVars };

// Symbol defined, type: global variable, address: 0x14003f0d8
static IdaVariable<unsigned __int8> __14D56680_main_cpp { 258264 };

// Symbol defined, type: global variable, address: 0x140033670
static IdaVariable<const char[]> _Format { 210544 };

// Symbol defined, type: global variable, address: 0x140033688
static IdaVariable<const char[]> a1GenerateKey { 210568 };

// Symbol defined, type: global variable, address: 0x1400336a0
static IdaVariable<const char[]> a2InputKey { 210592 };

// Symbol defined, type: global variable, address: 0x1400336b4
static IdaVariable<const char[]> aD { 210612 };

// Symbol defined, type: global variable, address: 0x1400336b8
static IdaVariable<const char[]> aGeneratedKeyD { 210616 };

// Symbol defined, type: global variable, address: 0x1400336d0
static IdaVariable<const char[]> aInputTheKey { 210640 };

// Symbol defined, type: global variable, address: 0x1400336b0
static IdaVariable<const char[]> asc_1400336B0 { 210608 };

// Symbol defined, type: global variable, address: 0x140032d20
static IdaVariable<const char[]> aHhd { 208160 };

// Symbol defined, type: global variable, address: 0x1400336e8
static IdaVariable<const char[]> aKeyIsValid { 210664 };

// Symbol defined, type: global variable, address: 0x140033700
static IdaVariable<const char[]> aKeyIsNotValid { 210688 };

// Symbol defined, type: global variable, address: 0x140033390
static IdaVariable<const _UNKNOWN> stru_140033390 { 209808 };

// Symbol defined, type: full function, address: 0x140017a40
__int64 __fastcall main_0();
static IdaFunction<decltype(main_0)> Real_main { 96832, &main_0 };
