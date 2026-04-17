project "glm"
	location "../../vendor/glm"
	kind "StaticLib"
	language "C++"
	staticruntime "on"

	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../bin/obj/" .. outputdir .. "/%{prj.name}")

	files {
		"../../vendor/glm/glm/**.hpp",
		"../../vendor/glm/glm/**.inl"
	}

	includedirs {
		"../../vendor/glm/glm"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
