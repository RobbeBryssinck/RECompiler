#include <stdio.h>
#include <Windows.h>
#include <cstdint>

static uint64_t s_testInt = 0;

bool GoToSleep(DWORD ms)
{
  Sleep(ms);
  printf("Test int: %lld\n", s_testInt);
  return true;
}

class DummyString
{
public:
  DummyString(const char* apData)
  {
    size = strlen(apData);
    pData = new char[size];
    memcpy(pData, apData, size + 1);
  }

  ~DummyString()
  {
    delete[] pData;
  }

  size_t size{};
  char* pData{};
};

class Dummy
{
public:
  int a = 5;
  uint64_t b = 0xFFFF;
  Dummy* pAnotherDummy = nullptr;
  DummyString dummyName = DummyString("Bobby");

  void PrintName()
  {
    printf("Dummy %s\n", dummyName.pData);
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
