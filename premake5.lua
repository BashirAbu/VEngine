workspace "VEngine"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Game_Debug",
        "Game_Release"
    }

outputDir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"


include "Fusion/premake5.lua"
include "VEngine/premake5.lua"
include "VEHeaderTool/premake5.lua"
