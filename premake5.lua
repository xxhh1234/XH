workspace "XH"
	architecture "X64"

	configurations
	{
		"Debug",
		"Release",
		"dist"
	}

	configuration "vs*"
		disablewarnings { 4251, 6285, 26437, 26450, 26495, 26498, 26800, 4996 }

outputdir="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (Solution Directory)
IncludeDir = {}
IncludeDir["GLFW"] = "XH/vendor/GLFW/include"

include "XH/vendor/GLFW"

project "XH"
	location "XH"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "xhpch.h"
	pchsource "XH/src/xhpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"XH_PLATFORM_WINDOWS",
			"XH_BUILD_DLL"
		}

		postbuildcommands
		{
			{"{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Examples"}
		}

	filter "configurations:Debug"
		defines "XH_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "XH_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "XH_DIST"
		buildoptions "/MD"
		optimize "On"

	

project "Examples"
	location "Examples"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"XH/vendor/spdlog/include",
		"XH/src"
	}

	links
	{
		"XH"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"XH_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "XH_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "XH_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "XH_DIST"
		buildoptions "/MD"
		optimize "On"