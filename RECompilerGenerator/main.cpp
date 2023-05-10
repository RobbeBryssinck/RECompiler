#include <REGenerator/REGenerator.h>
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <filesystem>
#include <RECore/FileHandling.h>

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

int main(int argc, char** argv)
{
  std::string path{};

  switch (argc)
  {
  case 1:
    std::cout << "Select a target directory in which the RECompiler project will be created.\n";
    path = GetFolder();
    break;
  case 2:
    path = argv[1];
    break;
  default:
    std::cerr << "Too many arguments were given. Did you put the path in quotation marks?" << std::endl;
    return 1;
  }

  while (!std::filesystem::exists(path))
  {
    std::cout << "The file path '" << path << "' is invalid. Please select a valid file path." << std::endl;
    path = GetFolder();
  }

  while (!std::filesystem::is_empty(path))
  {
    std::cout << "The file path '" << path << "' is not empty. Please select an empty target directory." << std::endl;
    path = GetFolder();
  }

  switch (REGenerator::Generate(path))
  {
  case REGenerator::GenerateResult::kOk:
    std::cout << "Project was successfully generated at '" << path << "'\n"
      << "Run 'generateSolution.bat' in the output directory to generate the Visual Studio project files." << std::endl;
    return 0;
  case REGenerator::GenerateResult::kTargetDirNotEmpty:
    std::cerr << "The target path '" << path << "' is not empty." << std::endl;
    return 1;
  case REGenerator::GenerateResult::kInvalidTargetPath:
    std::cerr << "The target path '" << path << "' is invalid." << std::endl;
    return 1;
  case REGenerator::GenerateResult::kMissingTemplateDir:
    std::cerr << "The 'Template' directory to generate the files does not exist in the current working directory." << std::endl;
    return 1;
  default:
    std::cerr << "Project generation has failed." << std::endl;
    return 1;
  }
}
