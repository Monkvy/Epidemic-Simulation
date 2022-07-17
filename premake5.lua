
workspace "Epidemic-Simulation"
    architecture "x64"
    startproject "Epidemic-Simulation"

    configurations {
        "Debug",
        "Release"
    }


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Epidemic-Simulation"
    location "Epidemic-Simulation"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h"
	}

    filter "configurations:*"
		defines { "SFML_STATIC" }
		includedirs { 
			"%{prj.name}/vendor/SFML/include",
			"%{prj.name}/vendor/ImGui"
		}
		libdirs { "%{prj.name}/vendor/SFML/lib" }
		links {
			"ImGui",
			"opengl32",
			"freetype",
			"winmm",
			"gdi32",
			"flac",
			"vorbisenc",
			"vorbisfile",
			"vorbis",
			"ogg",
			"ws2_32"
		}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		links {	
			"ImGui",
			"sfml-graphics-s-d",
			"sfml-window-s-d",
			"sfml-system-s-d",
			"sfml-audio-s-d",
			"sfml-network-s-d"
		}

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		links {	
			"ImGui",
			"sfml-graphics-s",
			"sfml-window-s",
			"sfml-system-s",
			"sfml-audio-s",
			"sfml-network-s"
		}

include "Epidemic-Simulation/vendor/ImGui/premake5.lua"