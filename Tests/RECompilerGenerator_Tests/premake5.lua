group("Tests")
project "RECompilerGenerator_Tests"
   kind "ConsoleApp"
   language "C++"

   files {"**.h", "**.cpp", "../main.cpp"}

   includedirs
   {
      "../../Vendor/googletest/include",
      "../../Components"
   }

   libdirs
   {
      "../../Build/Bin/%{cfg.longname}"
   }

   links "googletest"
   links "REGenerator"