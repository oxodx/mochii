workspace "Mochii"
	startproject "Sandbox"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["Glad"] = "Mochii/vendor/Glad/include"

include "Mochii/vendor/Glad"

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

project "ImGui"
	location "imgui"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/obj/" .. outputdir .. "/%{prj.name}")

	files {
		"Mochii/vendor/imgui/imconfig.h",
		"Mochii/vendor/imgui/imgui.h",
		"Mochii/vendor/imgui/imgui.cpp",
		"Mochii/vendor/imgui/imgui_draw.cpp",
        "Mochii/vendor/imgui/imgui_internal.h",
        "Mochii/vendor/imgui/imgui_widgets.cpp",
        "Mochii/vendor/imgui/imgui_tables.cpp",
        "Mochii/vendor/imgui/imstb_rectpack.h",
        "Mochii/vendor/imgui/imstb_textedit.h",
        "Mochii/vendor/imgui/imstb_truetype.h",
        "Mochii/vendor/imgui/imgui_demo.cpp",
        "Mochii/vendor/imgui/backends/imgui_impl_glfw.cpp",
        "Mochii/vendor/imgui/backends/imgui_impl_glfw.h",
        "Mochii/vendor/imgui/backends/imgui_impl_opengl3.cpp",
        "Mochii/vendor/imgui/backends/imgui_impl_opengl3.h"
	}

	includedirs {
		"Mochii/vendor/imgui;",
		"Mochii/vendor/imgui/backends;",
		"Mochii/vendor/glfw/include;"
	}

	filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"
        
    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"

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
		"vendor/spdlog/include;",
		"Mochii/vendor/glfw/include;",
		"Mochii/vendor/imgui;",
		"Mochii/vendor/imgui/backends;",
		"%{IncludeDir.Glad};"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
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
