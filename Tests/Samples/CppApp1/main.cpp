#include <stdio.h>
#include <Windows.h>

enum TestEnum1
{
  kTestA = 0,
  kTestB,
  kTestC = 4
};

struct TestStruct1
{
public:
  TestEnum1 a;
private:
  float b;
};

typedef int* pInt;

class TestClass1
{
public:
  TestStruct1 t1;
  pInt p;
};

bool PrintTestClass(TestClass1* apTc1)
{
  //std::cout << apTc1->t1.a << std::endl;
  return true;
}

int main(int argc, char** argv)
{
  if (argc == 1)
  {
    printf("No arguments given.\n");
    return 1;
  }

  if (!stricmp(argv[1], "GoodPath"))
  {
    printf("Executing the good path.\n");
    return 0;
  }
  else if (!stricmp(argv[1], "BadPath"))
  {
    printf("Executing the bad path.\n");
    return 2;
  }
  
  printf("Executing an unknown path.\n");
  return 3;
}