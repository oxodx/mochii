project "glfw"
	location "../../vendor/glfw"
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
