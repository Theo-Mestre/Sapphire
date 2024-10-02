local projectName = "Sapphire"

project (projectName)
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   staticruntime "on"
   
   targetdir "Binaries/%{cfg.buildcfg}"
   targetdir ("../Binaries/" .. OutputDir .. "/" .. projectName)

   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/" .. projectName)
   libdirs { "../Vendor/lib" }

   pchheader "sphpch.h"
   pchsource "Source/sphpch.cpp"

   defines
   { 
       "SPH_BUILD", 
       string.upper(projectName) .. "_ENABLE_ASSERTS",
	   "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING", 
	   "_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
   }

   files { "Source/**.h", "Source/**.cpp", "Source/**.inl", "../Vendor/include/stb_image.cpp" }

   flags
   {
       "MultiProcessorCompile"
   }

   includedirs
   {
        "Source",
        "../Vendor/include",
        "../Vendor/ThirdParty/GLFW/include",
        "../Vendor/ThirdParty/Glad/include",
        "../Vendor/ThirdParty/ImGui",
        "../Vendor/ThirdParty/spdlog",
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
       symbols "on"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "on"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"