project "ImGui"
	location "../../vendor/imgui"
	kind "StaticLib"
	language "C++"

	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../bin/obj/" .. outputdir .. "/%{prj.name}")

	files {
		"../../vendor/imgui/imconfig.h",
		"../../vendor/imgui/imgui.h",
		"../../vendor/imgui/imgui.cpp",
		"../../vendor/imgui/imgui_draw.cpp",
		"../../vendor/imgui/imgui_internal.h",
		"../../vendor/imgui/imgui_widgets.cpp",
		"../../vendor/imgui/imgui_tables.cpp",
		"../../vendor/imgui/imstb_rectpack.h",
		"../../vendor/imgui/imstb_textedit.h",
		"../../vendor/imgui/imstb_truetype.h",
		"../../vendor/imgui/imgui_demo.cpp",
		"../../vendor/imgui/backends/imgui_impl_glfw.cpp",
		"../../vendor/imgui/backends/imgui_impl_glfw.h",
		"../../vendor/imgui/backends/imgui_impl_opengl3.cpp",
		"../../vendor/imgui/backends/imgui_impl_opengl3.h"
	}

	includedirs {
		"../../vendor/imgui;",
		"../../vendor/imgui/backends;",
		"../../vendor/glfw/include;"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"
        
	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"
