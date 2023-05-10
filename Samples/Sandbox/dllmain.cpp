#include <Windows.h>

// The SDK header should be included here.
#include "TargetApplication.exe.h"

DWORD WINAPI MainThread(HMODULE hModule)
{
  // This thread can be used to invoke code from the target SDk.
  // An example of this could be written as follows:

  while (true)
  {
    if (GetAsyncKeyState(VK_NUMPAD1) & 1)
    {
      // Call some function from the target SDK here.
    }
  }

  return 0;
}

extern "C" BOOL APIENTRY DllMain(HMODULE hModule,
  DWORD  ul_reason_for_call,
  LPVOID lpReserved
)
{
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
  {
    if (!InitializeHooks())
      return FALSE;

    CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));

    break;
  }
  case DLL_THREAD_ATTACH:
    break;
  case DLL_THREAD_DETACH:
    break;
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

