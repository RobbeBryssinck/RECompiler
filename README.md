# RECompiler
Recompile functions decompiled by Ida.

# Usage

## Generating an RECompiler project
Download the latest release. Generate an RECompiler project by running `RECompilerGenerator.exe`. A target path can be given directly on the command line. If no path is given, the program will open a Windows folder selection dialogue. The project files will be copied from the included `Template` folder. Beware that the generator does not create a subdirectory for this.

In the generated project's root directory, the `generateSolution.bat` file can be run to generate the Visual Studio project files. The `Generated` folder hosts the solution file.

## Running an RECompiler project
Execute `RECompiler.exe`, which is located in the root of the previously generated RECompiler project. On a first time use, the program will ask for the path to the target binary. This path can be modified later in the `settings.ini` file, which can be found within the root directory of the generated project as well.

## Recompiling functions
The included Ida plugin can be used to dump functions to the generated project's SDK. Open a function in Ida and run the binary. On a first time use, the plugin will ask for the path to the RECompiler project. This path can be modified as well. The setting file can be found in the directory of where the Ida database is.

The dumped functions can be found in the SDK files of the project. The functions can be modified and can be called from `dllmain.cpp`. The generated data structures can be read from and written to as well.

To run the target binary with the newly written functions, simply run `RECompiler.exe` from the project's root directory.

# Build
To build, simply pull the repository, and use the premake binary (which is supplied with the repo) to generate project files and build as you normally would with premake. Here is an example workflow for Windows.

## Setting up the environment and building on Windows
Make sure you have Visual Studio 2022 installed, along with the appropriate VS C++ modules. Pull the git repository and generate the project files with the following command:

```cmd
> git clone --recursive https://github.com/RobbeBryssinck/RECompiler.git
> cd RECompiler/
> ./generateSolution.bat
```

The solution file can be found in `./Generated/`. The application can be built through Visual Studio.
