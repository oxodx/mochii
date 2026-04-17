dofile "./premake/fetch.lua"

workspace "Mochii"
	startproject "Sandbox"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

fetch("glfw", "https://github.com/glfw/glfw", "3.4", "./vendor/glfw")
fetch("imgui", "https://github.com/ocornut/imgui", "v1.92.7", "./vendor/imgui")
fetch("spdlog", "https://github.com/gabime/spdlog", "v1.17.0", "./vendor/spdlog")
fetch("glm", "https://github.com/g-truc/glm.git", "1.0.3", "./vendor/glm")

IncludeDir = {}
IncludeDir["glfw"] = "vendor/glfw/include;"
IncludeDir["imgui"] = "vendor/imgui;vendor/imgui/backends;"
IncludeDir["spdlog"] = "vendor/spdlog/include;"
IncludeDir["glm"] = "vendor/glm;"
IncludeDir["glad"] = "vendor/glad/include;"

group "Dependencies"
	include "premake/vendor/glfw.lua"
	include "premake/vendor/imgui.lua"
	include "premake/vendor/glm.lua"
	include "premake/vendor/glad.lua"
group ""

project "Mochii"
	location "Mochii"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/obj/" .. outputdir .. "/%{prj.name}")

	pchheader "mzpch.h"
	pchsource "Mochii/src/mzpch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Mochii/src;",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.glad}"
	}

	links {
		"GLFW",
		"ImGui",
		"Glad",
		"opengl32"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		buildoptions { "/utf-8", "/FS" }

		defines {
			"MI_PLATFORM_WINDOWS",
			"MI_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands {
			('{COPY} "%{cfg.targetdir}%{cfg.targetname}%{cfg.targetext}" "../bin/' .. outputdir .. '/Sandbox/"')
		}

	filter "configurations:Debug"
		defines "MI_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "MI_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "MI_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/obj/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Mochii/src",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.spdlog}"
	}

	libdirs {
		"bin/" .. outputdir .. "/Mochii"
	}

	links {
		"Mochii",
		"GLFW"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		buildoptions { "/utf-8" }

		defines {
			"MI_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "MI_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "MI_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "MI_DIST"
		runtime "Release"
		optimize "On"
