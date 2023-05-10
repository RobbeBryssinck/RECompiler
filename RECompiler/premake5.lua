group "Apps"
project "RECompiler"
   kind "ConsoleApp"
   language "C++"

   files {"**.h", "**.cpp"}

   includedirs
   {
      "../Components"
   }

   libdirs
   {
      "../Build/Bin/%{cfg.longname}"
   }

   links "RELauncher"