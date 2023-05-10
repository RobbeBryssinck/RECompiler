#include <iostream>
#include <Windows.h>
#include <RECore/FileHandling.h>
#include <filesystem>
#include <fstream>

std::string GetFolder()
{
  std::string path = "";

  while (path == "")
  {
    std::string title = "Select a target directory.";
    path = OpenFolderDialogue(&title);

    if (path == "")
      std::cerr << "Failed to select target directory.\n";
  }

  return path;
}

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

int main(int argc, char** argv)
{
  if (argc > 2)
  {
    std::cerr << "Too many arguments were given. Did you put the path in quotation marks?" << std::endl;
    return 1;
  }

  if (!std::filesystem::exists("./Template"))
  {
    std::cerr << "The 'Template' directory to generate the files does not exist in the current working directory." << std::endl;
    return 1;
  }

  std::string path = "";
  if (argc == 2)
    path = argv[1];
  else
    path = GetFolder();

  std::filesystem::copy("./Template", path, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);

#if 0
  std::cout << "Project name (blank for default):" << std::endl;
  
  std::string projectName;
  std::cin >> projectName;
  if (projectName != "")
    SetProjectName(path, projectName);
#endif

  std::filesystem::path targetPath = path;
  std::string projectName = targetPath.filename().string();
  SetProjectName(path, projectName);

  std::cout << "Project was successfully generated at '" << path << "'\n"
    << "Run 'generateSolution.bat' in the output directory to generate the Visual Studio project files." << std::endl;
}
