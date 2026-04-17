project "glad"
	location "../../vendor/glad"
	kind "StaticLib"
	language "C"

	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../bin/obj/" .. outputdir .. "/%{prj.name}")

	files {
		"../../vendor/glad/include/glad/glad.h",
		"../../vendor/glad/include/KHR/khrplatform.h",
		"../../vendor/glad/src/glad.c"
	}

	includedirs {
		"../../vendor/glad/include"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"
