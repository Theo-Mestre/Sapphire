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

	defines
	{
		"SPDLOG_COMPILED_LIB",
		"_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING", 
		"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
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