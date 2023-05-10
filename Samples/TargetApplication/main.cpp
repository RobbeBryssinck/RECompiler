#include <iostream>
#include <Windows.h>

static uint64_t s_testInt = 0;

bool GoToSleep(DWORD ms)
{
  Sleep(ms);
  printf("Test int: %d", s_testInt);
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
