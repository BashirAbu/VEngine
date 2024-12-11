include "build_globals.lua"

workspace "VEngine"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }




include "Fusion/premake5.lua"
include "VEngine/premake5.lua"
include "VEHeaderTool/premake5.lua"
include "VEAssetsPackager/premake5.lua"
