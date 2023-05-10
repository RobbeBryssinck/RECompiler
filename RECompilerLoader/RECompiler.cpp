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
    kBuildPathNotFound,
    kTargetPathNotFound,
    kDllPathNotFound,
  };

  LoadResult LoadSettings()
  {
    CSimpleIniA ini;
    ini.SetUnicode();

    SI_Error rc = ini.LoadFile(".\\settings.ini");
    if (rc < 0)
      return LoadResult::kSettingsFileNotFound;

    buildPath = ini.GetValue("global", "build_path");
    if (buildPath.empty())
      return LoadResult::kBuildPathNotFound;

    targetPath = ini.GetValue("global", "target_path");
    if (targetPath.empty())
      return LoadResult::kTargetPathNotFound;

    dllPath = ini.GetValue("global", "dll_path");
    if (dllPath.empty())
      return LoadResult::kDllPathNotFound;

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

bool ExecuteRECompiler()
{
  switch (s_settings.LoadSettings())
  {
  case Settings::LoadResult::kOk:
    break;
  case Settings::LoadResult::kSettingsFileNotFound:
  case Settings::LoadResult::kBuildPathNotFound:
  case Settings::LoadResult::kTargetPathNotFound:
  case Settings::LoadResult::kDllPathNotFound:
  default:
    std::cerr << "Loading settings failed.\n";
    return false;
  }

  auto result = BuildInjectedCode();
  switch (result)
  {
  case BuildResult::kOk:
    std::cout << "Build succeeded.\n";
    break;
  default:
    std::cerr << "Build failed.\n";
    return false;
  }

  STARTUPINFOA startup;
  PROCESS_INFORMATION process;

  ZeroMemory(&startup, sizeof(startup));
  startup.cb = sizeof(startup);
  ZeroMemory(&process, sizeof(process));

  if (!CreateProcessA(NULL, const_cast<char*>(s_settings.targetPath.c_str()), NULL, NULL, FALSE, CREATE_SUSPENDED | CREATE_NEW_CONSOLE, NULL, NULL, &startup, &process))
  {
    std::cerr << "Failed to create process: " << GetLastError() << "\n";
    return false;
  }

  if (process.hProcess == NULL || process.hProcess == INVALID_HANDLE_VALUE)
  {
    std::cerr << "No process handle found.\n";
    return false;
  }

  void* pInjected = VirtualAllocEx(process.hProcess, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  if (!pInjected)
  {
    std::cerr << "Failed to allocate injected memory.\n";
    return false;
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

  return true;
}

void GenerateProject(std::string& aPath)
{
  while (aPath == "")
  {
    std::string title = "Select a target directory.";
    aPath = OpenFolderDialogue(&title);

    if (aPath == "")
      std::cerr << "Failed to select target directory.\n";
  }

  std::filesystem::copy("./Template", aPath, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
}

int main(int argc, char** argv)
{
  if (argc == 1)
  {
    ExecuteRECompiler();
  }
  else if (argc >= 2)
  {
    if (_stricmp(argv[1], "generate"))
    {
      std::cerr << "Second argument must be a valid keyword." << std::endl;
      return 1;
    }

    std::string path = "";
    if (argc == 3)
      path = argv[2];

    GenerateProject(path);

    std::cout << "Project was successfully generated at '" << path << "'.\n";
    std::cout << "Run 'generateSolution.bat' to generate the Visual Studio project files." << std::endl;
  }
}
