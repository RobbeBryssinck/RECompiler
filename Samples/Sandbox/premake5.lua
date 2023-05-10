project "Sandbox"
   kind "SharedLib"
   language "C++"

   files {"**.h", "**.cpp"}

   includedirs
   {
      "../../Vendor/minhook/include"
   }