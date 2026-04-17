project "glad"
	location "../../vendor/glad"
	kind "StaticLib"
	language "C"
	staticruntime "on"

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

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
