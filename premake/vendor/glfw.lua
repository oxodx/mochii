project "glfw"
	location "../../vendor/glfw"
	kind "StaticLib"
	language "C"

	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../bin/obj/" .. outputdir .. "/%{prj.name}")

	files {
		"../../vendor/glfw/src/null_init.c",
		"../../vendor/glfw/src/null_joystick.c",
		"../../vendor/glfw/src/null_monitor.c",
		"../../vendor/glfw/src/null_window.c",
		"../../vendor/glfw/include/GLFW/glfw3.h",
		"../../vendor/glfw/include/GLFW/glfw3native.h",
		"../../vendor/glfw/src/internal.h",
		"../../vendor/glfw/src/platform.h",
		"../../vendor/glfw/src/mappings.h",
		"../../vendor/glfw/src/context.c",
		"../../vendor/glfw/src/init.c",
		"../../vendor/glfw/src/input.c",
		"../../vendor/glfw/src/monitor.c",
		"../../vendor/glfw/src/platform.c",
		"../../vendor/glfw/src/vulkan.c",
		"../../vendor/glfw/src/window.c",
		"../../vendor/glfw/src/egl_context.c",
		"../../vendor/glfw/src/osmesa_context.c"
	}

	includedirs {
		"../../vendor/glfw/include;",
		"../../vendor/glfw/src;"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		files {
			"../../vendor/glfw/src/win32_init.c",
			"../../vendor/glfw/src/win32_module.c",
			"../../vendor/glfw/src/win32_joystick.c",
			"../../vendor/glfw/src/win32_monitor.c",
			"../../vendor/glfw/src/win32_time.c",
			"../../vendor/glfw/src/win32_thread.c",
			"../../vendor/glfw/src/win32_window.c",
			"../../vendor/glfw/src/wgl_context.c"
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
