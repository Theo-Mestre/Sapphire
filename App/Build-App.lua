project "App"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   staticruntime "on"

   files { "Source/**.h", "Source/**.cpp", "Source/**.inl"}

   includedirs
   {
      "Source",
      
	  -- Include Sapphire
	  "../Sapphire/Source",

      -- Include Third party libs 
      "../Vendor/ThirdParty/ImGui",
      "../Vendor/ThirdParty/spdlog",
      "../Vendor/include",   
    }

   links
   {
	  "Sapphire"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")
   debugdir "Assets"

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