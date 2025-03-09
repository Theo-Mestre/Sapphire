project "yaml-cpp"
	kind "StaticLib"
	cppdialect "C++20"
	language "C++"
	staticruntime "on"

	targetdir ("../../../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../../../Binaries/Intermediates/" .. OutputDir ..  "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",

		"include/**.h",
	}

	includedirs
	{
		"include"
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

	flags
    {
        "MultiProcessorCompile"
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