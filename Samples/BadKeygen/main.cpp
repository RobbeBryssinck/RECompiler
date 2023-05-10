#include <stdio.h>
#include <Windows.h>
#include <cstdint>
#include <vector>

static constexpr uint8_t s_secret = 23;
static const std::vector<uint8_t> validXoredKeys{ 23, 41, 244 };

uint8_t GenerateKey()
{
  auto index = rand() % validXoredKeys.size();
  return validXoredKeys[index] xor s_secret;
}

bool IsKeyValid(uint8_t aKey)
{
  uint8_t xoredKey = aKey xor s_secret;
  return std::find(validXoredKeys.begin(), validXoredKeys.end(), xoredKey) != validXoredKeys.end();
}

int main()
{
  printf("Choose an option:\n");
  printf("1. Generate key\n");
  printf("2. Input key\n");
  printf("> ");

  int choice;
  scanf_s("%d", &choice);

  switch (choice)
  {
  case 1:
  {
    uint8_t key = GenerateKey();
    printf("Generated key: %d\n", key);
    
    break;
  }
  case 2:
  {
    printf("Input the key:\n");
    printf("> ");

    uint8_t key;
    scanf_s("%hhd", &key);
    
    if (IsKeyValid(key))
      printf("Key is valid!\n");
    else
      printf("Key is not valid!\n");

    break;
  }
  }
}
