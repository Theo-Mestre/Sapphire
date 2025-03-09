OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"
SolutionName = "SapphireEngine"
ProjectName = "Sapphire"
EditorName = "Editor"
AppName = "App"

workspace (SolutionName)
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject (EditorName)

   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

group (ProjectName)
	include (ProjectName .. "/Build-" .. ProjectName .. ".lua")
	include (EditorName .. "/Build-" .. EditorName .. ".lua")

group (ProjectName .. "/Dependencies")
	include "Vendor/ThirdParty/Glad/Build-Glad.lua"
	include "Vendor/ThirdParty/GLFW/Build-GLFW.lua"
	include "Vendor/ThirdParty/ImGui/Build-ImGui.lua"
	include "Vendor/ThirdParty/ImPlot/Build-ImPlot.lua"
	include "Vendor/ThirdParty/spdlog/Build-spdlog.lua"
	include "Vendor/ThirdParty/Yaml-cpp/Build-yaml-cpp.lua"

-- Dont build app as I work on the editor for now
--group ""
--include (AppName .. "/Build-" .. AppName .. ".lua")