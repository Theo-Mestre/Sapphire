local projectName = "Sapphire"

project (projectName)
   kind "SharedLib"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   targetdir ("../Binaries/" .. OutputDir .. "/" .. projectName)
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/" .. projectName)
   libdirs { "../Vendor/lib" }

   pchheader "sphpch.h"
   pchsource "Sapphire/Source/sphpch.cpp"

   defines { string.upper(projectName) .. "_BUILD_DLL", string.upper(projectName) .. "_ENABLE_ASSERTS" }

   files { "Source/**.h", "Source/**.cpp", "Source/**.inl" }

   includedirs
   {
        "Source",
        "../Vendor/include",
        "../Vendor/ThirdParty/GLFW/include",
        "../Vendor/ThirdParty/Glad/include",
        "../Vendor/ThirdParty/ImGui",
        "../Vendor/ThirdParty/spdlog",
   }

   postbuildcommands   
   {
       "copy /y $(OutDir)$(TargetName).dll $(OutDir)..\\App"
   }

   links    
   {	  
       "GLFW",
       "Glad",
       "ImGui",
       "spdlog"
   }

   filter "system:windows"
       systemversion "latest"
       defines { "GLFW_INCLUDE_NONE" }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"