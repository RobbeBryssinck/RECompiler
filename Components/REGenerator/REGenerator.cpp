#include "REGenerator.h"

#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <SimpleIni.h>

namespace REGenerator
{

  void SetProjectName(const std::string& aProjectPath, const std::string& aProjectName)
  {
    std::vector<std::string> files{ aProjectPath + "/premake5.lua", aProjectPath + "/RECompilerProject/premake5.lua" };
    const std::string defaultName = "RECompilerProject";

    for (const auto& filename : files)
    {
      std::ifstream file(filename);
      std::vector<std::string> newContents{};
      std::string line{};
      size_t wordLength = defaultName.length();

      while (std::getline(file, line))
      {
        while (true)
        {
          size_t position = line.find(defaultName);
          if (position != std::string::npos)
            line.replace(position, wordLength, aProjectName);
          else
            break;
        }

        newContents.push_back(line);
      }

      file.close();

      std::ofstream newFile(filename);
      for (const auto& newLine : newContents)
      {
        newFile << newLine << "\n";
      }
    }

    std::filesystem::rename(aProjectPath + "/" + defaultName, aProjectPath + "/" + aProjectName);
  }

  bool GenerateSettingsFile(const std::string& aPath, const std::string& aProjectName)
  {
    const std::string iniFilename = aPath + "\\settings.ini";

    CSimpleIniA ini;
    ini.SetUnicode();

    SI_Error rc = ini.LoadFile(iniFilename.c_str());
    if (rc < 0)
      return false;

    std::string buildPath{ aPath + "\\Generated\\" + aProjectName + ".vcxproj" };
    ini.SetValue("global", "build_path", buildPath.c_str());

    std::string dllPath{ aPath + "\\Build\\Bin\\Debug\\" + aProjectName + ".dll" };
    ini.SetValue("global", "dll_path", dllPath.c_str());

    rc = ini.SaveFile(iniFilename.c_str());
    if (rc < 0)
      return false;

    return true;
  }

  GenerateResult Generate(const std::string& aPath)
  {
    if (!std::filesystem::exists(aPath))
      return GenerateResult::kInvalidTargetPath;

    if (!std::filesystem::exists("./Template"))
      return GenerateResult::kMissingTemplateDir;

    std::filesystem::copy("./Template", aPath, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);

    std::filesystem::path targetPath = aPath;
    std::string projectName = targetPath.filename().string();
    SetProjectName(aPath, projectName);

    GenerateSettingsFile(aPath, projectName);

    return GenerateResult::kOk;
  }

}
