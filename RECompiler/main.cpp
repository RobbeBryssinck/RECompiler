#include <iostream>
#include "SimpleIni.h"
#include <format>
#include <Windows.h>
#include <RECore/FileHandling.h>
#include <filesystem>

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

  LoadResult LoadSettings()
  {
    constexpr const char* pIniFilename = ".\\settings.ini";

    CSimpleIniA ini;
    ini.SetUnicode();

    SI_Error rc = ini.LoadFile(pIniFilename);
    if (rc < 0)
      return LoadResult::kSettingsFileNotFound;

    targetPath = ini.GetValue("global", "target_path");
    if (!std::filesystem::exists(targetPath))
    {
      std::cout << "Target path not found, please select the target binary." << std::endl;

      std::string title = "Target binary";
      FileFilters filters{ {"Executable", "*.exe"} };
      targetPath = OpenFileDialogue(&title, &filters);

      if (!std::filesystem::exists(targetPath))
        return LoadResult::kTargetPathNotFound;

      ini.SetValue("global", "target_path", targetPath.c_str());

    }

    buildPath = ini.GetValue("global", "build_path");
    if (!std::filesystem::exists(buildPath))
    {
      if (!std::filesystem::exists(".\\generateSolution.bat"))
        return LoadResult::kGenerateSolutionNotFound;

      int ret = system(".\\generateSolution.bat");
      if (ret != 0)
        return LoadResult::kGenerateSolutionFailed;

      if (!std::filesystem::exists(buildPath))
      {
        std::cout << "Project file to build not found, please select the target vcxproj file." << std::endl;

        std::string title = "Target VS project";
        FileFilters filters{ {"VS project", "*.vcxproj"} };
        buildPath = OpenFileDialogue(&title, &filters);

        ini.SetValue("global", "build_path", buildPath.c_str());

        if (!std::filesystem::exists(buildPath))
          return LoadResult::kBuildPathNotFound;
      }
    }

    dllPath = ini.GetValue("global", "dll_path");

    rc = ini.SaveFile(pIniFilename);
    if (rc < 0)
      return LoadResult::kIniSaveFailed;

    return LoadResult::kOk;
  }

  std::string buildPath{};
  std::string targetPath{};
  std::string dllPath{};
};

namespace
{
  Settings s_settings;
}

enum class BuildResult
{
  kOk = 0,
  kUnknown
};

BuildResult BuildInjectedCode()
{
  auto command = std::format("MSBuild.exe {} /property:Configuration=Debug /property:Platform=x64", s_settings.buildPath.c_str());
  int ret = system(command.c_str());

  if (ret == 0)
    return BuildResult::kOk;

  return BuildResult::kUnknown;
}

int main(int argc, char** argv)
{
  switch (s_settings.LoadSettings())
  {
  case Settings::LoadResult::kOk:
    break;
  case Settings::LoadResult::kTargetPathNotFound:
    std::cerr << "Target path not found." << std::endl;
    break;
  case Settings::LoadResult::kBuildPathNotFound:
    std::cerr << "Build path not found." << std::endl;
    break;
  case Settings::LoadResult::kDllPathNotFound:
    std::cerr << "Dll path not found." << std::endl;
    break;
  case Settings::LoadResult::kGenerateSolutionNotFound:
    std::cerr << "'generateSolution.bat' file not found." << std::endl;
    break;
  case Settings::LoadResult::kGenerateSolutionFailed:
    std::cerr << "Failed to generate solution files." << std::endl;
    break;
  case Settings::LoadResult::kSettingsFileNotFound:
    std::cerr << "'settings.ini' file not found." << std::endl;
    break;
  default:
    std::cerr << "Loading settings failed." << std::endl;
    return 1;
  }

  auto result = BuildInjectedCode();
  switch (result)
  {
  case BuildResult::kOk:
    std::cout << "Build succeeded.\n";
    break;
  default:
    std::cerr << "Build failed.\n";
    return 1;
  }

  STARTUPINFOA startup;
  PROCESS_INFORMATION process;

  ZeroMemory(&startup, sizeof(startup));
  startup.cb = sizeof(startup);
  ZeroMemory(&process, sizeof(process));

  if (!CreateProcessA(NULL, const_cast<char*>(s_settings.targetPath.c_str()), NULL, NULL, FALSE, CREATE_SUSPENDED | CREATE_NEW_CONSOLE, NULL, NULL, &startup, &process))
  {
    std::cerr << "Failed to create process: " << GetLastError() << "\n";
    return 1;
  }

  if (process.hProcess == NULL || process.hProcess == INVALID_HANDLE_VALUE)
  {
    std::cerr << "No process handle found.\n";
    return 1;
  }

  void* pInjected = VirtualAllocEx(process.hProcess, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  if (!pInjected)
  {
    std::cerr << "Failed to allocate injected memory.\n";
    return 1;
  }

  WriteProcessMemory(process.hProcess, pInjected, s_settings.dllPath.c_str(), s_settings.dllPath.length() + 1, nullptr);

  HANDLE dllLoadThread = CreateRemoteThread(process.hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, pInjected, 0, nullptr);

  DWORD threadResult = 0;
  do
  {
    threadResult = WaitForSingleObject(dllLoadThread, 1000);
  } while (threadResult != 0);

  if (dllLoadThread != NULL && dllLoadThread != INVALID_HANDLE_VALUE)
    CloseHandle(dllLoadThread);

  VirtualFreeEx(process.hProcess, pInjected, MAX_PATH, MEM_DECOMMIT | MEM_RELEASE);

  ResumeThread(process.hThread);

  CloseHandle(process.hProcess);

  std::cout << "Process successfully launched." << std::endl;
}
