project "ImPlot"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
    staticruntime "on"

	targetdir ("../../../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../../../Binaries/Intermediates/" .. OutputDir ..  "/%{prj.name}")

	files
	{
		"implot.h",
		"implot.cpp",
		"implot_internal.h",
		"implot_items.cpp",
	}

	includedirs
	{
		"../ImGui/",
	}

	links
	{
		"ImGui"
	}

	warnings "Off"

	filter "system:windows"
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