#include <iostream>
#include <Windows.h>
#include <RECore/FileHandling.h>
#include <filesystem>

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

  std::cout << "Project was successfully generated at '" << path << "'\n"
    << "Run 'generateSolution.bat' in the output directory to generate the Visual Studio project files." << std::endl;
}
