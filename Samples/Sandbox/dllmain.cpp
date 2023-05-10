#include <Windows.h>
#include <iostream>
#include <MinHook.h>

DWORD WINAPI MainThread(HMODULE hModule)
{
  AllocConsole();
  FILE* f;
  freopen_s(&f, "CONOUT$", "w", stdout);

  std::cout << "DLL entry before!\n";
  Sleep(4000);
  std::cout << "DLL entry after!\n";

  while (true)
  {
    Sleep(1000);
  }

  if (f)
    fclose(f);
  FreeConsole();
  FreeLibraryAndExitThread(hModule, 0);

  return 0;
}

using TGoToSleep = bool(DWORD);
static TGoToSleep* RealGoToSleep = (TGoToSleep*)0x140012170;

bool GoToSleep(DWORD ms)
{
  std::cout << "Hooked GoToSleep()!\n";
  return RealGoToSleep(ms);
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
    //CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));

    if (MH_Initialize() != MH_OK)
    {
      return FALSE;
    }

    if (MH_CreateHook(RealGoToSleep, &GoToSleep, (LPVOID*)&RealGoToSleep) != MH_OK)
    {
      return FALSE;
    }

    if (MH_EnableHook(RealGoToSleep) != MH_OK)
    {
      return FALSE;
    }

    std::cout << "DLL entry before!\n";
    Sleep(4000);
    std::cout << "DLL entry after!\n";
    break;
  }
  case DLL_THREAD_ATTACH:
    break;
  case DLL_THREAD_DETACH:
  {
    std::cout << "Thread detach" << std::endl;
    break;
  }
  case DLL_PROCESS_DETACH:
  {
    std::cout << "Process detach" << std::endl;
    break;
  }
  }
  return TRUE;
}

