#pragma once

#include <string>

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

  LaunchResult Launch(const Settings& aSettings);

}