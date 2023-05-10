project "BadKeygen"
   kind "ConsoleApp"
   language "C++"

   linkoptions {"/DYNAMICBASE:NO"}

   files {"**.h", "**.cpp"}