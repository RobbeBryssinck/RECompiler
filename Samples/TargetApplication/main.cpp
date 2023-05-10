#include <iostream>
#include <Windows.h>

int main()
{
  std::cout << "Program started.\n";

  while (true)
  {
    Sleep(10000);
    std::cout << "Waking up.\n";
  }

  std::cout << "Exiting program.\n";
}
