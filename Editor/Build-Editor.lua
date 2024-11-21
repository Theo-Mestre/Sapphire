project (EditorName)
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   staticruntime "on"

   files { "Source/**.h", "Source/**.cpp", "Source/**.inl"}

   includedirs
   {
      "Source",
      
	  -- Include Sapphire
	  "../" .. ProjectName .. "/Source",

      -- Include Third party libs 
      "../Vendor/ThirdParty/ImGui",
      "../Vendor/ThirdParty/ImPlot",
      "../Vendor/ThirdParty/spdlog",
      "../Vendor/include", 

      -- Debug only
      "../Vendor/ThirdParty/GLFW/include",
      "../Vendor/ThirdParty/Glad/include",
    }

   links
   {
	  (ProjectName)
   }

   flags
   {
       "MultiProcessorCompile"
   }

   defines 
   {
	  "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
	  "_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")
   debugdir "../Assets"

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "on"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "on"
       symbols "off"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "on"
       symbols "off"