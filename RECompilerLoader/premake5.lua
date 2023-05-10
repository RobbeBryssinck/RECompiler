group "Apps"
project "RECompilerLoader"
   kind "ConsoleApp"
   language "C++"

   files {"**.h", "**.cpp"}

   includedirs
   {
      "../Vendor/spdlog/include",
      "../Vendor/simpleini"
   }

   libdirs
   {
      "../Build/Bin/%{cfg.longname}"
   }

   links "RECore"