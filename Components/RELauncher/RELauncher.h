#pragma once

#include <string>
#include <Windows.h>

namespace RELauncher
{

  struct Settings
  {
    enum class LoadResult
    {
      kOk = 0,
      kSettingsFileNotFound,
      kGenerateSolutionNotFound,
      kTargetPathNotFound,
      kBuildPathNotFound,
      kNoBuildPath,
      kGenerateSolutionFailed,
      kDllPathNotFound,
      kIniSaveFailed,
    };

    LoadResult LoadSettings();

    bool isInitialized{false};
    std::string buildPath{};
    std::string targetPath{};
    std::string dllPath{};
  };

  enum class LaunchResult
  {
    kOk = 0,
    kSettingsNotInitialized,
    kBuildFailed,
    kCreateProcessFailed,
    kNoProcessHandle,
    kAllocDllPathFailed,
  };

  struct LaunchInfo
  {
    LaunchInfo(LaunchResult aResult) : result(aResult), process(nullptr) {}
    LaunchInfo(LaunchResult aResult, HANDLE aProcess) : result(aResult), process(aProcess) {}

    LaunchResult result;
    HANDLE process;
  };

  LaunchInfo Launch(const Settings& aSettings);

}