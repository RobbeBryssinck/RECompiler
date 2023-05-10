project "RELauncher"
   kind "StaticLib"
   language "C++"

   files {"**.h", "**.cpp", "**.inl"}

   includedirs
   {
      "../../Vendor/simpleini",
      "../../Libraries"
   }

   libdirs
   {
      "../../Build/Bin/%{cfg.longname}"
   }

   links "RECore"