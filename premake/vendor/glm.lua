project "glm"
	location "../../vendor/glm"
	kind "StaticLib"
	language "C++"

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
		staticruntime "On"

	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"
