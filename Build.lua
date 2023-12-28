-- premake5.lua
workspace "New Project"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "App"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Sapphire"
	include "Sapphire/Build-Sapphire.lua"
	include "Vendor/ThirdParty/Glad/Build-Glad.lua"
	include "Vendor/ThirdParty/GLFW/Build-GLFW.lua"
	include "Vendor/ThirdParty/ImGui/Build-ImGui.lua"
	include "Vendor/ThirdParty/spdlog/Build-spdlog.lua"
group ""
include "App/Build-App.lua"