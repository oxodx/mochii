workspace "Mochii"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Mochii"
	location "Mochii"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/obj/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		-- "%{prj.name}/vendor/spdlog/include;"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"MI_PLATFORM_WINDOWS",
			"MI_BUILD_DLL"
		}

		postbuildcommands {
			('{COPY} "%{cfg.targetdir}%{cfg.targetname}%{cfg.targetext}" "../bin/' .. outputdir .. '/Sandbox/"')
		}

	filter "configurations:Debug"
		defines "MI_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "MI_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "MI_DIST"
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
		-- "%{prj.name}/vendor/spdlog/include;",
		"Mochii/src"
	}

	libdirs {
		"bin/" .. outputdir .. "/Mochii"
	}

	links {
		"Mochii"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"MI_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "MI_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "MI_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "MI_DIST"
		optimize "On"
