project "spdlog"
	kind "StaticLib"
	cppdialect "C++17"
	language "C++"
    staticruntime "off"

	targetdir ("../../../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../../../Binaries/Intermediates/" .. OutputDir ..  "/%{prj.name}")

	files
	{
		"spdlog/**.h",
		"spdlog/**.cpp",
	}

	includedirs "$(SolutionDir)Vendor/ThirdParty/spdlog"
	
	warnings "Off"

	filter "system:windows"
		systemversion "latest"

	filter "system:linux"
		pic "On"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

    filter "configurations:Dist"
		runtime "Release"
		optimize "on"
        symbols "off"