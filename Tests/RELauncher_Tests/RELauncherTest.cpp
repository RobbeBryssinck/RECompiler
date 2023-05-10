#include <gtest/gtest.h>
#include <RELauncher/RELauncher.h>

namespace
{
#if 0
  class DiaInterfaceTest : public ::testing::Test
  {
  public:
    static void SetUpTestSuite()
    {
      pUsym = std::make_unique<USYM>(DiaInterface::CreateUsymFromFile("CppApp1.pdb").value());
    }

    static std::unique_ptr<USYM> pUsym;
  };

  std::unique_ptr<USYM> DiaInterfaceTest::pUsym = nullptr;

  TEST(DiaInterface, LoadPdbFile)
  {
    EXPECT_NO_THROW(DiaInterface::InitializeDia("CppApp1.pdb"));
    
    DiaInterface::Release();
  }

  TEST(DiaInterface, CreateUsymFromFile)
  {
    auto pUsym = DiaInterface::CreateUsymFromFile("CppApp1.pdb");
    
    ASSERT_TRUE(pUsym.has_value());
  }

  TEST_F(DiaInterfaceTest, TestHeader)
  {
    EXPECT_EQ(pUsym->header.magic, 'MYSU');
    EXPECT_EQ(pUsym->header.originalFormat, USYM::OriginalFormat::kPdb);
    EXPECT_EQ(pUsym->header.architecture, USYM::Architecture::kX86_64);
  }
#endif
}