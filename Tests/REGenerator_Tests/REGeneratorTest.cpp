#include <gtest/gtest.h>
#include <REGenerator/REGenerator.h>

#include <filesystem>

namespace
{
  class ScopedSampleDir : public ::testing::Test
  {
  protected:
    void SetUp() override
    {
      if (std::filesystem::exists("./Sample"))
        std::filesystem::remove_all("./Sample");

      std::filesystem::create_directory("./Sample");
    }

    void TearDown() override
    {
      if (std::filesystem::exists("./Sample"))
        std::filesystem::remove_all("./Sample");
    }
  };

  TEST_F(ScopedSampleDir, GenerateProject)
  {
    auto result = REGenerator::Generate("./Sample");

    EXPECT_EQ(result, REGenerator::GenerateResult::kOk);
  }

  class DeleteTemplate : public ScopedSampleDir
  {
  protected:
    void SetUp() override
    {
      ScopedSampleDir::SetUp();

      std::filesystem::rename("./Template", "./TemplateOld");
    }

    void TearDown() override
    {
      ScopedSampleDir::TearDown();

      std::filesystem::rename("./TemplateOld", "./Template");
    }
  };

  TEST_F(DeleteTemplate, FaultNoTemplate)
  {
    auto result = REGenerator::Generate("./Sample");

    EXPECT_EQ(result, REGenerator::GenerateResult::kMissingTemplateDir);
  }

  TEST(REGenerator, InvalidTargetPath)
  {
    auto result = REGenerator::Generate("./InvalidTargetPath");

    EXPECT_EQ(result, REGenerator::GenerateResult::kInvalidTargetPath);
  }

  class NonEmptyTarget : public ScopedSampleDir
  {
  protected:
    void SetUp() override
    {
      ScopedSampleDir::SetUp();

      std::filesystem::create_directory("./Sample/Dir");
    }

    void TearDown() override
    {
      ScopedSampleDir::TearDown();
    }
  };

  TEST_F(NonEmptyTarget, TestNonEmptyTargetDir)
  {
    auto result = REGenerator::Generate("./Sample");

    EXPECT_EQ(result, REGenerator::GenerateResult::kTargetDirNotEmpty);
  }
}
