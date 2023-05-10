project "TargetApplication"
   kind "ConsoleApp"
   language "C++"

   linkoptions {"/DYNAMICBASE:NO"}

   files {"**.h", "**.cpp"}