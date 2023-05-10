#include <stdio.h>
#include <Windows.h>
#include <cstdint>
#include <string>

static uint64_t s_testInt = 0;

bool GoToSleep(DWORD ms)
{
  Sleep(ms);
  printf("Test int: %lld\n", s_testInt);
  return true;
}

class Dummy
{
public:
  int a = 5;
  uint64_t b = 0xFFFF;
  Dummy* pAnotherDummy = nullptr;
  std::string dummyName = "Bobby";

  void PrintName()
  {
    printf("Dummy %s", dummyName.c_str());
  }
};


int main()
{
  printf("Program started.\n");

  Dummy dummy{};

  while (true)
  {
    bool result = GoToSleep(5000);
    if (result)
      printf("Waking up.\n");
    else
      printf("Failed to sleep.\n");

    dummy.PrintName();
  }

  printf("Exiting program.\n");
}
