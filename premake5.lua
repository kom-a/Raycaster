workspace "Raycaster"
	configurations { "Debug", "Release" }


project "Raycaster"
	location "Raycaster"
	kind "ConsoleApp"
	language "C++"

	targetdir "Bin"
	objdir "Bin/Intermediates"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.cc",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/res/**.*",
	}

	includedirs
	{
		"Dependencies/includes"
	}

	libdirs
	{
		"Dependencies/libs"	
	}

	links
	{
		"glfw3.lib",
		"gorilla.lib",
		"legacy_stdio_definitions.lib"
	}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"