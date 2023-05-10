project "REGenerator"
   kind "StaticLib"
   language "C++"

   files {"**.h", "**.cpp", "**.inl"}

   includedirs
   {
      "../../Vendor/simpleini",
   }