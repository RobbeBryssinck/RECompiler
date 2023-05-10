group "Apps"
project "RECompiler"
   kind "ConsoleApp"
   language "C++"

   files {"**.h", "**.cpp"}

   includedirs
   {
      "../Vendor/simpleini",
      "../Libraries",
      "../Components"
   }

   libdirs
   {
      "../Build/Bin/%{cfg.longname}"
   }

   links "RECore"
   links "RELauncher"