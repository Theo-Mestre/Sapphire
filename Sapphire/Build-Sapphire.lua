project (ProjectName)
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   staticruntime "on"
   
   targetdir "Binaries/%{cfg.buildcfg}"
   targetdir ("../Binaries/" .. OutputDir .. "/" .. ProjectName)

   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/" .. ProjectName)
   libdirs { "../Vendor/lib" }

   pchheader "sphpch.h"
   pchsource "Source/sphpch.cpp"

   defines
   { 
       "SPH_BUILD", 
       "SAPPHIRE_ENABLE_ASSERTS",
       "SPH_VISUAL_PROFILING_ENABLED",
       "YAML_CPP_STATIC_DEFINE ",
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
        "../Vendor/ThirdParty/ImPlot",
        "../Vendor/ThirdParty/spdlog",
        "../Vendor/ThirdParty/yaml-cpp/include",
   }

   links    
   {
       "GLFW",
       "Glad",
       "ImGui",
       "ImPlot",
       "spdlog",
       "yaml-cpp",
   }

   filter "system:windows"
       systemversion "latest"
       defines { "GLFW_INCLUDE_NONE" }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "on"
       optimize "off"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       symbols "off" 
       optimize "on"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       symbols "off"
       optimize "On"