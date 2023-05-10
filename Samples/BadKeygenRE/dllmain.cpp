#include <Windows.h>

// The SDK header should be included here.
#include "C:\dev\RECompiler\Samples\BadKeygenRE\BadKeygen.exe.h"

#include <stdio.h>

DWORD WINAPI MainThread(HMODULE hModule)
{
  // This thread can be used to invoke code from the target SDK.
  // An example of this could be written as follows:

  printf("Whenever you are ready to start the crack, press numpad 1.\n");

  while (true)
  {
    static bool s_startCrack = false;

    if (GetAsyncKeyState(VK_NUMPAD1) & 1)
      s_startCrack = true;

    if (!s_startCrack)
      continue;

    for (uint8_t i = 1; i <= 0xFF; i++)
    {
      if (IsKeyValid(i))
        break;
    }

    break;
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
