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

// Symbol defined, type: stub function, address: 0x140011118
unsigned __int8 __fastcall GenerateKey();
static IdaFunction<decltype(GenerateKey)> Real_GenerateKey { 69912, &GenerateKey };

// Symbol defined, type: stub function, address: 0x1400111fe
int j_scanf_s(const char *_Format, ...);
static IdaFunction<decltype(j_scanf_s)> Real_j_scanf_s { 70142, &j_scanf_s };

// Symbol defined, type: stub function, address: 0x140011339
bool __fastcall j_IsKeyValid(unsigned __int8 aKey);
static IdaFunction<decltype(j_IsKeyValid)> Real_j_IsKeyValid { 70457, &j_IsKeyValid };

// Symbol defined, type: stub function, address: 0x140011195
int j_printf(const char *_Format, ...);
static IdaFunction<decltype(j_printf)> Real_j_printf { 70037, &j_printf };

// Symbol defined, type: stub function, address: 0x140011311
void __fastcall j__RTC_CheckStackVars(void *frame, void *v);
static IdaFunction<decltype(j__RTC_CheckStackVars)> Real_j__RTC_CheckStackVars { 70417, &j__RTC_CheckStackVars };

// Symbol defined, type: global variable, address: 0x14001ad80
static IdaVariable<const char[]> _Format { 109952 };

// Symbol defined, type: global variable, address: 0x14001ad98
static IdaVariable<const char[]> a1GenerateKey { 109976 };

// Symbol defined, type: global variable, address: 0x14001adb0
static IdaVariable<const char[]> a2InputKey { 110000 };

// Symbol defined, type: global variable, address: 0x14001adc4
static IdaVariable<const char[]> aD { 110020 };

// Symbol defined, type: global variable, address: 0x14001adc8
static IdaVariable<const char[]> aGeneratedKeyD { 110024 };

// Symbol defined, type: global variable, address: 0x14001ade0
static IdaVariable<const char[]> aInputTheKey { 110048 };

// Symbol defined, type: global variable, address: 0x14001adc0
static IdaVariable<const char[]> asc_14001ADC0 { 110016 };

// Symbol defined, type: global variable, address: 0x14001adf4
static IdaVariable<const char[]> aHhd { 110068 };

// Symbol defined, type: global variable, address: 0x14001ae00
static IdaVariable<const char[]> aKeyIsValid { 110080 };

// Symbol defined, type: global variable, address: 0x14001ae18
static IdaVariable<const char[]> aKeyIsNotValid { 110104 };

// Symbol defined, type: global variable, address: 0x14001ad50
static IdaVariable<const _UNKNOWN> v { 109904 };

// Symbol defined, type: full function, address: 0x140011a90
__int64 __fastcall main_0();
static IdaFunction<decltype(main_0)> Real_main { 72336, &main_0 };

// Symbol defined, type: stub function, address: 0x14001137a
void __fastcall j___CheckForDebuggerJustMyCode(unsigned __int8 *JMC_flag);
static IdaFunction<decltype(j___CheckForDebuggerJustMyCode)> Real_j___CheckForDebuggerJustMyCode { 70522, &j___CheckForDebuggerJustMyCode };

// Symbol defined, type: global variable, address: 0x1400230d8
static IdaVariable<unsigned __int8> __14D56680_main_cpp { 143576 };

// Symbol defined, type: global variable, address: 0x14001e014
static IdaVariable<unsigned __int8> s_secret { 122900 };

// Symbol defined, type: global variable, address: 0x14001abb0
static IdaVariable<const unsigned __int8[3]> validXoredKeys { 109488 };

// Symbol defined, type: full function, address: 0x140011830
_BOOL8 __fastcall IsKeyValid(unsigned __int8 aKey);
static IdaFunction<decltype(IsKeyValid)> Real_IsKeyValid { 71728, &IsKeyValid };
