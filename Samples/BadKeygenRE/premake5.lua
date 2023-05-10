project "BadKeygenRE"
   kind "SharedLib"
   language "C++"

   files {"**.h", "**.cpp", "**.c"}

   includedirs
   {
      "../../Libraries/minhook/include"
   }

   libdirs
   {
      "../Build/Bin/%{cfg.longname}"
   }

   links "minhook"