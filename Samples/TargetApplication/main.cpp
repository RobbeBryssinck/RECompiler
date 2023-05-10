#include <iostream>
#include <Windows.h>

bool GoToSleep(DWORD ms)
{
  Sleep(ms);
  return true;
}

int main()
{
  std::cout << "Program started.\n";

  while (true)
  {
    bool result = GoToSleep(5000);
    if (result)
      std::cout << "Waking up.\n";
    else
      std::cout << "Failed to sleep.\n";
  }

  std::cout << "Exiting program.\n";
}
