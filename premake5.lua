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

project "sirius-gcs"
    includedirs {
        "src",
        "src/gui",
        "src/logging",
        "src/map",
        "external/mINI",
        "external/sirius-headers-common"
    }

    files {
        "src/**.h",
        "src/**.cpp",
        "external/mINI/ini.h",
        "external/sirius-headers-common/**.h"
    }

    defines { "STB_IMAGE_IMPLEMENTATION" }

    filter "system:windows"
        buildoptions {
            "/utf-8",
            "/wd4005",
            "/wd26498",
            "/wd26800"
        }

project "sirius-gcs-tests"
    includedirs {
        "src/test"
    }

    files {
        "tests/**.h",
        "tests/**.cpp"
    }


