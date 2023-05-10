#include <gtest/gtest.h>
#include <RELauncher/RELauncher.h>

namespace
{
  TEST(RELauncherSettings, TestLoadExistingSettings)
  {
    RELauncher::Settings settings{};
    
    auto result = settings.LoadSettings();

    EXPECT_EQ(result, RELauncher::Settings::LoadResult::kOk);
  }

  class LaunchFixture : public ::testing::Test
  {
  public:
    static void SetUpTestSuite()
    {
      settings.LoadSettings();
    }

    static RELauncher::Settings settings;

  protected:
    void SetUp() override
    {
      info = RELauncher::Launch(settings);
    }

    void TearDown() override
    {
      // TODO: exit code?
      TerminateProcess(info.process, 1);
    }

    RELauncher::LaunchInfo info;
  };

  RELauncher::Settings LaunchFixture::settings = RELauncher::Settings();

  TEST_F(LaunchFixture, LaunchFromSettings)
  {
    ASSERT_EQ(info.result, RELauncher::LaunchResult::kOk);
  }
}