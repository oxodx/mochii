project "ImGui"
	location "../../vendor/imgui"
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
