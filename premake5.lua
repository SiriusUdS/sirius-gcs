workspace "sirius-gcs"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "sirius-gcs"

    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir "out/bin/%{cfg.buildcfg}"
    objdir "out/obj"

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"
	
    includedirs {
	"src/TemperatureSensor.h",
	"src/TemperatureSensor.cpp"
    }

project "sirius-gcs"
    includedirs {
        "src",
        "src/led",
        "src/logging",
        "src/map",
        "src/plot",
        "src/serial",
        "src/windows",
        "external/ceSerial",
        "external/mINI",
        "external/sirius-headers-common"
    }

    files {
        "src/**.h",
        "src/**.hpp",
        "src/**.cpp",
        "external/ceSerial/ceserial.h",
        "external/mINI/ini.h",
        "external/sirius-headers-common/**.h"
    }

    filter "system:windows"
        buildoptions {
            "/utf-8",
            "/wd4005",
            "/wd26498",
            "/wd26800"
        }

project "sirius-gcs-tests"
    includedirs {
        "src",
        "src/serial",
        "external/doctest",
        "external/sirius-headers-common"
    }

    files {
        "tests/**.cpp",
        "src/Constants.h",
        "src/serial/RecvBuffer.hpp",
        "src/serial/WordFormatter.h",
        "src/serial/WordFormatter.cpp",
        "external/sirius-headers-common/**.h"
    }
