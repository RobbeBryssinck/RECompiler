#include <gtest/gtest.h>
#include <RELauncher/RELauncher.h>
#include <filesystem>

namespace
{
  class ScopedSampleDir : public ::testing::Test
  {
  protected:
    void SetUp() override
    {
      std::filesystem::current_path(std::filesystem::current_path() / "LaunchSample");
    }

    void TearDown() override
    {
      std::filesystem::current_path(std::filesystem::current_path().parent_path());
    }
  };

  TEST_F(ScopedSampleDir, LoadExistingSettings)
  {
    RELauncher::Settings settings{};
    
    auto result = settings.LoadSettings();

    EXPECT_EQ(result, RELauncher::Settings::LoadResult::kOk);
  }

  class SettingsFixture : public ScopedSampleDir
  {
  protected:
    void SetUp() override
    {
      ScopedSampleDir::SetUp();
      settings.LoadSettings();
    }

    void TearDown() override
    {
      ScopedSampleDir::TearDown();
    }

    RELauncher::Settings settings;
  };

  TEST_F(SettingsFixture, LaunchSampleProject)
  {
    RELauncher::LaunchInfo info = RELauncher::Launch(settings);

    EXPECT_EQ(info.result, RELauncher::LaunchResult::kOk);

    // TODO: exit code?
    TerminateProcess(info.process, 1);
    CloseHandle(info.process);
  }

  class LaunchFixture : public SettingsFixture
  {
  protected:
    void SetUp() override
    {
      SettingsFixture::SetUp();
      info = RELauncher::Launch(settings);
    }

    void TearDown() override
    {
      SettingsFixture::TearDown();
      // TODO: exit code?
      TerminateProcess(info.process, 1);
      CloseHandle(info.thread);
      CloseHandle(info.process);
    }

    RELauncher::LaunchInfo info;
  };

  TEST_F(LaunchFixture, LaunchFromSettings)
  {
    ASSERT_EQ(info.result, RELauncher::LaunchResult::kOk);
  }
}