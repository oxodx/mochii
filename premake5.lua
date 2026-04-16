workspace "Mochii"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

startproject "Sandbox"

project "GLFW"
	location "GLFW"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/obj/" .. outputdir .. "/%{prj.name}")

	files {
		"Mochii/vendor/glfw/src/null_init.c",
		"Mochii/vendor/glfw/src/null_joystick.c",
		"Mochii/vendor/glfw/src/null_monitor.c",
		"Mochii/vendor/glfw/src/null_window.c",
		"Mochii/vendor/glfw/include/GLFW/glfw3.h",
		"Mochii/vendor/glfw/include/GLFW/glfw3native.h",
		"Mochii/vendor/glfw/src/internal.h",
		"Mochii/vendor/glfw/src/platform.h",
		"Mochii/vendor/glfw/src/mappings.h",
		"Mochii/vendor/glfw/src/context.c",
		"Mochii/vendor/glfw/src/init.c",
		"Mochii/vendor/glfw/src/input.c",
		"Mochii/vendor/glfw/src/monitor.c",
		"Mochii/vendor/glfw/src/platform.c",
		"Mochii/vendor/glfw/src/vulkan.c",
		"Mochii/vendor/glfw/src/window.c",
		"Mochii/vendor/glfw/src/egl_context.c",
		"Mochii/vendor/glfw/src/osmesa_context.c"
	}

	includedirs {
		"Mochii/vendor/glfw/include;",
		"Mochii/vendor/glfw/src;"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		files {
			"Mochii/vendor/glfw/src/win32_init.c",
			"Mochii/vendor/glfw/src/win32_module.c",
			"Mochii/vendor/glfw/src/win32_joystick.c",
			"Mochii/vendor/glfw/src/win32_monitor.c",
			"Mochii/vendor/glfw/src/win32_time.c",
			"Mochii/vendor/glfw/src/win32_thread.c",
			"Mochii/vendor/glfw/src/win32_window.c",
			"Mochii/vendor/glfw/src/wgl_context.c"
		}

		defines {
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

project "Mochii"
	location "Mochii"
	kind "SharedLib"
	language "C++"

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
		"vendor/spdlog/include;",
		"Mochii/vendor/glfw/include;"
	}

	links {
		"GLFW",
		"opengl32"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		buildoptions { "/utf-8" }

		defines {
			"MI_PLATFORM_WINDOWS",
			"MI_BUILD_DLL"
		}

		postbuildcommands {
			('{COPY} "%{cfg.targetdir}%{cfg.targetname}%{cfg.targetext}" "../bin/' .. outputdir .. '/Sandbox/"')
		}

	filter "configurations:Debug"
		defines "MI_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "MI_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "MI_DIST"
		buildoptions "/MD"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/obj/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Mochii/src",
		"Mochii/vendor/glfw/include;",
		"vendor/spdlog/include;"
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
		staticruntime "On"
		systemversion "latest"
		buildoptions { "/utf-8" }

		defines {
			"MI_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "MI_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "MI_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "MI_DIST"
		buildoptions "/MD"
		optimize "On"
