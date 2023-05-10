#include <iostream>
#include <Windows.h>
#include <RELauncher/RELauncher.h>

int main(int argc, char** argv)
{
  RELauncher::Settings settings{};

  switch (settings.LoadSettings())
  {
  case RELauncher::Settings::LoadResult::kOk:
    break;
  case RELauncher::Settings::LoadResult::kTargetPathNotFound:
    std::cerr << "Target path not found." << std::endl;
    return 1;
  case RELauncher::Settings::LoadResult::kBuildPathNotFound:
    std::cerr << "Build path not found." << std::endl;
    return 1;
  case RELauncher::Settings::LoadResult::kDllPathNotFound:
    std::cerr << "Dll path not found." << std::endl;
    return 1;
  case RELauncher::Settings::LoadResult::kGenerateSolutionNotFound:
    std::cerr << "'generateSolution.bat' file not found." << std::endl;
    return 1;
  case RELauncher::Settings::LoadResult::kGenerateSolutionFailed:
    std::cerr << "Failed to generate solution files." << std::endl;
    return 1;
  case RELauncher::Settings::LoadResult::kSettingsFileNotFound:
    std::cerr << "'settings.ini' file not found." << std::endl;
    return 1;
  default:
    std::cerr << "Loading settings failed." << std::endl;
    return 1;
  }

  auto launchInfo = RELauncher::Launch(settings);
  
  switch (launchInfo.result)
  {
  case RELauncher::LaunchResult::kOk:
    std::cout << "Process launched successfully." << std::endl;
    break;
  case RELauncher::LaunchResult::kSettingsNotInitialized:
    std::cerr << "Settings must be initialized first (call LoadSettings())." << std::endl;
    return 1;
  case RELauncher::LaunchResult::kBuildFailed:
    std::cerr << "Build failed." << std::endl;
    return 1;
  case RELauncher::LaunchResult::kCreateProcessFailed:
    std::cerr << "Failed to create process: " << GetLastError() << std::endl;
    return 1;
  case RELauncher::LaunchResult::kNoProcessHandle:
    std::cerr << "No process handle found." << std::endl;
    return 1;
  case RELauncher::LaunchResult::kAllocDllPathFailed:
    std::cerr << "Failed to allocate injected memory for dll path: " << GetLastError() << std::endl;
    return 1;
  default:
    std::cerr << "RELauncher launch failed." << std::endl;
    return 1;
  }
}
