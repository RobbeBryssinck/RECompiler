project "Sandbox"
   kind "SharedLib"
   language "C++"

   files {"**.h", "**.cpp"}

   includedirs
   {
      "../../Libraries/minhook/include"
   }

   libdirs
   {
      "../Build/Bin/%{cfg.longname}"
   }

   links "minhook"