project "VEngine"
    cppdialect "C++20"
    language "c++"
    
    targetdir("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir("%{wks.location}/bin_int/" .. outputDir .. "/%{prj.name}")

    files
    {
        prefixPaths("../", base_engine_src_files),
        prefixPaths("../", editor_engine_src_files),
    }

    includedirs
    {
        prefixPaths("../", base_engine_include_dirs),
        prefixPaths("../", editor_engine_include_dirs)
    }
    
    prebuildcommands
    {
        "{CHDIR} %{prj.location}",
        "%{wks.location}/bin/" .. outputDir .. "/VEHeaderTool/VEHeaderTool e src/generated/ src/components/ -c VE_CLASS -e VE_ENUM -f VE_FUNCTION -p VE_PROPERTY"
    }
    postbuildcommands
    {
        ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/Fusion")
    }
    defines 
    {
        editor_engine_defines,
        base_engine_defines,
    }

    filter "system:windows"
        links { windows_link_libs }
        systemversion "latest"
        buildoptions { windows_build_options }
        defines { windows_defines }

    filter "configurations:Debug"
        kind "SharedLib"
        staticruntime "Off"
        symbols "On"
        defines { debug_engine_defines, debug_editor_defines }

    filter "configurations:Release"
        kind "SharedLib"
        staticruntime "Off"
        optimize "On"
        defines { release_engine_defines, release_editor_defines }