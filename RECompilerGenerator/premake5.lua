group "Apps"
project "RECompilerGenerator"
   kind "ConsoleApp"
   language "C++"

   files {"**.h", "**.cpp"}

   includedirs
   {
      "../Vendor/simpleini",
      "../Libraries"
   }

   libdirs
   {
      "../Build/Bin/%{cfg.longname}"
   }

   links "RECore"