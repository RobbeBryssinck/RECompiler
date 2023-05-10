group "Apps"
project "RECompilerGenerator"
   kind "ConsoleApp"
   language "C++"

   files {"**.h", "**.cpp"}

   includedirs
   {
      "../Libraries"
   }

   libdirs
   {
      "../Build/Bin/%{cfg.longname}"
   }

   links "RECore"