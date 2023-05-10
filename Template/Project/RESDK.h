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
