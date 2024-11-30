workspace "VEngine"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Development"
    }

outputDir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"


include "Fusion/premake5.lua"
include "VEngine/premake5.lua"
include "VEHeaderTool/premake5.lua"
