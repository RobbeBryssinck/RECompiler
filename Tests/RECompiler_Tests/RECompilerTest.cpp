#include <gtest/gtest.h>
#include <RELauncher/RELauncher.h>
#include <filesystem>

/*
* How to run the integration tests:
* 1. Use RECompilerGenerator to generate a project called "LaunchSample".
* 2. Place the generated project's directory in the same directory as the test binary.
* 3. Build CppApp1 and place the binary (CppApp1.exe) in the same directory as the test binary.
* 4. Open CppApp1.exe in Ida and run the recompiler_generator.py plugin on the main function.
* 5. Run the test binary.
*/

namespace
{
  class LaunchFixture : public ::testing::Test
  {
  public:
    static void SetUpTestSuite()
    {
      std::filesystem::current_path(std::filesystem::current_path() / "LaunchSample");
    }

    static void TearDownTestSuite()
    {
      std::filesystem::current_path(std::filesystem::current_path().parent_path());
    }

  protected:
    void SetUp() override
    {
      settings.buildPath = ".\\Generated\\LaunchSample.vcxproj";
      settings.dllPath = ".\\Build\\Bin\\Debug\\LaunchSample.dll";
      settings.isInitialized = true;

      info = RELauncher::Launch(settings);
    }

    void TearDown() override
    {
      // TODO: exit code?
      TerminateProcess(info.process, 1);
      if (info.thread)
        CloseHandle(info.thread);
      if (info.process)
        CloseHandle(info.process);
    }

    RELauncher::LaunchInfo info;
    RELauncher::Settings settings;
  };

  class GoodPathLaunchFixture : public LaunchFixture
  {
  protected:
    void SetUp() override
    {
      settings.targetPath = "..\\CppApp1.exe GoodPath";
      LaunchFixture::SetUp();
    }
  };

  TEST_F(GoodPathLaunchFixture, LaunchFromSettings)
  {
    ASSERT_EQ(info.result, RELauncher::LaunchResult::kOk);

    ResumeThread(info.thread);
    Sleep(500);

    DWORD exitCode{};
    GetExitCodeProcess(info.process, &exitCode);

    EXPECT_EQ(exitCode, 0);
  }

  class BadPathLaunchFixture : public LaunchFixture
  {
  protected:
    void SetUp() override
    {
      settings.targetPath = "..\\CppApp1.exe BadPath";
      LaunchFixture::SetUp();
    }
  };

  TEST_F(BadPathLaunchFixture, LaunchFromSettings)
  {
    ASSERT_EQ(info.result, RELauncher::LaunchResult::kOk);

    ResumeThread(info.thread);
    Sleep(500);

    DWORD exitCode{};
    GetExitCodeProcess(info.process, &exitCode);

    EXPECT_EQ(exitCode, 2);
  }

  class WrongArgsPathLaunchFixture : public LaunchFixture
  {
  protected:
    void SetUp() override
    {
      settings.targetPath = "..\\CppApp1.exe";
      LaunchFixture::SetUp();
    }
  };

  TEST_F(WrongArgsPathLaunchFixture, LaunchFromSettings)
  {
    ASSERT_EQ(info.result, RELauncher::LaunchResult::kOk);

    ResumeThread(info.thread);
    Sleep(500);

    DWORD exitCode{};
    GetExitCodeProcess(info.process, &exitCode);

    EXPECT_EQ(exitCode, 1);
  }

  class UnknownPathLaunchFixture : public LaunchFixture
  {
  protected:
    void SetUp() override
    {
      settings.targetPath = "..\\CppApp1.exe ???";
      LaunchFixture::SetUp();
    }
  };

  TEST_F(UnknownPathLaunchFixture, LaunchFromSettings)
  {
    ASSERT_EQ(info.result, RELauncher::LaunchResult::kOk);

    ResumeThread(info.thread);
    Sleep(500);

    DWORD exitCode{};
    GetExitCodeProcess(info.process, &exitCode);

    EXPECT_EQ(exitCode, 3);
  }
}