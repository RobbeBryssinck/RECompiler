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

    std::cout << "DLL entry before!\n";
    Sleep(4000);
    std::cout << "DLL entry after!\n";

  }
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

